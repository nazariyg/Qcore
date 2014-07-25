#include "QDiscreteFourierTransform.h"
#include "QSystUtils.h"
using namespace Q;

//------------------------------------------------------------------------------------------------------------------
template <class Real>
DiscreteFourierTransform<Real>::DiscreteFourierTransform (DftType eDft, int iN)
    :
    m_eDft(eDft),
    m_iN(iN)
{
    assert( iN > 0 );
    assert( !((eDft == DFTT_1D_FAST_FORWARD || eDft == DFTT_1D_FAST_INVERSE
        ||  eDft == DFTT_1D_FAST_FORWARD_REAL || eDft == DFTT_1D_FAST_INVERSE_REAL) && !IsPowerOfTwo(iN)) );

    if ( eDft == DFTT_1D_FAST_FORWARD || eDft == DFTT_1D_FAST_INVERSE
    ||   eDft == DFTT_1D_FAST_FORWARD_REAL || eDft == DFTT_1D_FAST_INVERSE_REAL )
    {
        // pre-compute according to the Cooley-Tukey FFT algorithm

        bool bInputIsComplex;
        bool bTransformIsForward;
        int iLutN;

        if ( eDft == DFTT_1D_FAST_FORWARD || eDft == DFTT_1D_FAST_INVERSE )
        {
            bInputIsComplex = true;
            bTransformIsForward = ( eDft == DFTT_1D_FAST_FORWARD );
            iLutN = iN;
        }
        else
        {
            bInputIsComplex = false;
            bTransformIsForward = ( eDft == DFTT_1D_FAST_FORWARD_REAL );
            m_iHalfN = iN >> 1;
            iLutN = m_iHalfN;
        }

        m_iCtLuTPointQuantityL0 = 0;
        int iValue = iLutN;
        while ( iValue > 1 )
        {
            iValue >>= 1;
            m_iCtLuTPointQuantityL0++;
        }
        m_aaqCtLuTable = new Complex<Real>*[m_iCtLuTPointQuantityL0];

        Real fPi = ( bTransformIsForward ? -Math<Real>::PI : Math<Real>::PI );
        Real fPhi, fSin;
        Complex<Real> qMult, qFactor;
        int iStep = 1;
        int iC = 0;
        while ( iStep < iLutN )
        {
            m_aaqCtLuTable[iC] = new Complex<Real>[iStep];

            fPhi = fPi/iStep;
            fSin = Math<Real>::Sin(fPhi*((Real)0.5));
            qMult.Re() = ((Real)-2.0)*fSin*fSin;
            qMult.Im() = Math<Real>::Sin(fPhi);

            qFactor = (Real)1.0;
            for (int i = 0; i < iStep; i++)
            {
                m_aaqCtLuTable[iC][i] = qFactor;
                qFactor = qMult*qFactor + qFactor;
            }

            iStep <<= 1;
            iC++;
        }

        if ( !bInputIsComplex )
        {
            // pre-compute according to the "packing" algorithm for real and derived-from-real input
            m_aqRpA = new Complex<Real>[m_iHalfN];
            m_aqRpB = new Complex<Real>[m_iHalfN];
            for (int i = 0; i < m_iHalfN; i++)
            {
                fPhi = ((Real)i)/m_iHalfN*Math<Real>::PI;
                Real fHalfSin = Math<Real>::Sin(fPhi)*((Real)0.5);
                Real fHalfCos = Math<Real>::Cos(fPhi)*((Real)0.5);
                m_aqRpA[i].Re() = (Real)0.5 - fHalfSin;
                m_aqRpA[i].Im() = -fHalfCos;
                m_aqRpB[i].Re() = (Real)0.5 + fHalfSin;
                m_aqRpB[i].Im() = fHalfCos;
            }

            int iPackedPointQuantity = ( bTransformIsForward ? m_iHalfN + 1 : m_iHalfN );
            m_aqPackedPoint = new Complex<Real>[iPackedPointQuantity];
        }
    }
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
DiscreteFourierTransform<Real>::~DiscreteFourierTransform ()
{
    if ( m_eDft == DFTT_1D_FAST_FORWARD || m_eDft == DFTT_1D_FAST_INVERSE
    ||   m_eDft == DFTT_1D_FAST_FORWARD_REAL || m_eDft == DFTT_1D_FAST_INVERSE_REAL )
    {
        for (int i = 0; i < m_iCtLuTPointQuantityL0; i++)
            delete[] m_aaqCtLuTable[i];
        delete[] m_aaqCtLuTable;

        if ( m_eDft == DFTT_1D_FAST_FORWARD_REAL || m_eDft == DFTT_1D_FAST_INVERSE_REAL )
        {
            delete[] m_aqRpA;
            delete[] m_aqRpB;

            delete[] m_aqPackedPoint;
        }
    }
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
void DiscreteFourierTransform<Real>::ApplyForward (Complex<Real>* aqPoint) const
{
    assert( m_eDft == DFTT_1D || m_eDft == DFTT_1D_FAST_FORWARD );

    if ( m_iN == 1 )
        return;

    if ( m_eDft == DFTT_1D )
    {
        Complex<Real>* aqPointCpy = new Complex<Real>[m_iN];
        MemCpy(aqPointCpy,aqPoint,m_iN*sizeof(Complex<Real>));

        Complex<Real> qSum;
        Real fVar0 = -Math<Real>::TWO_PI/m_iN;
        Real fVar1, fVar2;
        for (int i0 = 0; i0 < m_iN; i0++)
        {
            qSum = Complex<Real>::ZERO;
            fVar1 = fVar0*i0;
            for (int i1 = 0; i1 < m_iN; i1++)
            {
                fVar2 = fVar1*i1;
                qSum += aqPointCpy[i1]*Complex<Real>(Math<Real>::Cos(fVar2),Math<Real>::Sin(fVar2));
            }
            aqPoint[i0] = qSum;
        }

        delete[] aqPointCpy;
    }
    else  // m_eDft = DFTT_1D_FAST_FORWARD
    {
        CtApply(aqPoint,m_iN);
    }
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
void DiscreteFourierTransform<Real>::ApplyInverse (Complex<Real>* aqPoint) const
{
    assert( m_eDft == DFTT_1D || m_eDft == DFTT_1D_FAST_INVERSE );

    if ( m_iN == 1 )
        return;

    if ( m_eDft == DFTT_1D )
    {
        Complex<Real>* aqPointCpy = new Complex<Real>[m_iN];
        MemCpy(aqPointCpy,aqPoint,m_iN*sizeof(Complex<Real>));

        Complex<Real> qSum;
        Real fInvN = ((Real)1.0)/m_iN;
        Real fVar0 = Math<Real>::TWO_PI/m_iN;
        Real fVar1, fVar2;
        for (int i0 = 0; i0 < m_iN; i0++)
        {
            qSum = Complex<Real>::ZERO;
            fVar1 = fVar0*i0;
            for (int i1 = 0; i1 < m_iN; i1++)
            {
                fVar2 = fVar1*i1;
                qSum += aqPointCpy[i1]*Complex<Real>(Math<Real>::Cos(fVar2),Math<Real>::Sin(fVar2));
            }
            aqPoint[i0] = qSum*fInvN;
        }

        delete[] aqPointCpy;
    }
    else  // m_eDft = DFTT_1D_FAST_INVERSE
    {
        CtApply(aqPoint,m_iN);
        Real fInvN = ((Real)1.0)/m_iN;
        for (int i = 0; i < m_iN; i++)
            aqPoint[i] *= fInvN;
    }
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
void DiscreteFourierTransform<Real>::ApplyForwardReal (Complex<Real>* aqPoint) const
{
    assert( m_eDft == DFTT_1D || m_eDft == DFTT_1D_FAST_FORWARD_REAL );

    if ( m_iN == 1 )
        return;

    if ( m_eDft == DFTT_1D )
    {
        Complex<Real>* aqPointCpy = new Complex<Real>[m_iN];
        MemCpy(aqPointCpy,aqPoint,m_iN*sizeof(Complex<Real>));

        int iNDiv2Po = (m_iN >> 1) + 1;
        Complex<Real> qSum;
        Real fVar0 = -Math<Real>::TWO_PI/m_iN;
        Real fVar1, fVar2;
        for (int i0 = 0; i0 < iNDiv2Po; i0++)
        {
            qSum = Complex<Real>::ZERO;
            fVar1 = fVar0*i0;
            for (int i1 = 0; i1 < m_iN; i1++)
            {
                fVar2 = fVar1*i1;
                qSum += Complex<Real>(
                    aqPointCpy[i1].Re()*Math<Real>::Cos(fVar2),
                    aqPointCpy[i1].Re()*Math<Real>::Sin(fVar2));
            }
            aqPoint[i0] = qSum;
        }

        delete[] aqPointCpy;

        int iRevSymmStart = iNDiv2Po - ( (m_iN & 1) == 0 ? 2 : 1 );
        for (int i0 = iNDiv2Po, i1 = iRevSymmStart; i0 < m_iN; i0++, i1--)
            aqPoint[i0] = aqPoint[i1].Conj();
    }
    else  // m_eDft = DFTT_1D_FAST_FORWARD_REAL
    {
        // the "packing" algorithm for real and derived-from-real input

        for (int i0 = 0, i1 = 0, i2 = 1; i0 < m_iHalfN; i0++, i1 += 2, i2 += 2)
        {
            m_aqPackedPoint[i0].Re() = aqPoint[i1].Re();
            m_aqPackedPoint[i0].Im() = aqPoint[i2].Re();
        }

        CtApply(m_aqPackedPoint,m_iHalfN);

        m_aqPackedPoint[m_iHalfN] = m_aqPackedPoint[0];
        for (int i = 0; i < m_iHalfN; i++)
        {
            Complex<Real>& rqPoint0 = m_aqPackedPoint[i];
            Complex<Real>& rqPoint1 = m_aqPackedPoint[m_iHalfN-i];
            Complex<Real>& rqA = m_aqRpA[i];
            Complex<Real>& rqB = m_aqRpB[i];
            aqPoint[i].Re() =
                rqPoint0.Re()*rqA.Re() - rqPoint0.Im()*rqA.Im() +
                rqPoint1.Re()*rqB.Re() + rqPoint1.Im()*rqB.Im();
            aqPoint[i].Im() =
                rqPoint0.Im()*rqA.Re() + rqPoint0.Re()*rqA.Im() +
                rqPoint1.Re()*rqB.Im() - rqPoint1.Im()*rqB.Re();
        }
        aqPoint[m_iHalfN] = m_aqPackedPoint[0].Re() - m_aqPackedPoint[0].Im();
        for (int i0 = m_iHalfN+1, i1 = m_iHalfN-1; i0 < m_iN; i0++, i1--)
            aqPoint[i0] = aqPoint[i1].Conj();
    }
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
void DiscreteFourierTransform<Real>::ApplyInverseReal (Complex<Real>* aqPoint) const
{
    assert( m_eDft == DFTT_1D || m_eDft == DFTT_1D_FAST_INVERSE_REAL );

    if ( m_iN == 1 )
        return;

    if ( m_eDft == DFTT_1D )
    {
        Complex<Real>* aqPointCpy = new Complex<Real>[m_iN];
        MemCpy(aqPointCpy,aqPoint,m_iN*sizeof(Complex<Real>));

        int iNDiv2Po = (m_iN >> 1) + 1;
        Complex<Real> qSum;
        Real fInvN = ((Real)1.0)/m_iN;
        Real fVar0 = Math<Real>::TWO_PI/m_iN;
        Real fVar1, fVar2;
        for (int i0 = 0; i0 < iNDiv2Po; i0++)
        {
            qSum = Complex<Real>::ZERO;
            fVar1 = fVar0*i0;
            for (int i1 = 0; i1 < m_iN; i1++)
            {
                fVar2 = fVar1*i1;
                qSum += Complex<Real>(
                    aqPointCpy[i1].Re()*Math<Real>::Cos(fVar2),
                    aqPointCpy[i1].Re()*Math<Real>::Sin(fVar2));
            }
            aqPoint[i0] = qSum*fInvN;
        }

        delete[] aqPointCpy;

        int iRevSymmStart = iNDiv2Po - ( (m_iN & 1) == 0 ? 2 : 1 );
        for (int i0 = iNDiv2Po, i1 = iRevSymmStart; i0 < m_iN; i0++, i1--)
            aqPoint[i0] = aqPoint[i1].Conj();
    }
    else  // m_eDft = DFTT_1D_FAST_INVERSE_REAL
    {
        // the "packing" algorithm for real and derived-from-real input

        for (int i = 0; i < m_iHalfN; i++)
        {
            Complex<Real>& rqPoint0 = aqPoint[i];
            Complex<Real>& rqPoint1 = aqPoint[m_iHalfN-i];
            Complex<Real>& rqA = m_aqRpA[i];
            Complex<Real>& rqB = m_aqRpB[i];
            m_aqPackedPoint[i].Re() =
                rqPoint0.Re()*rqA.Re() + rqPoint0.Im()*rqA.Im() +
                rqPoint1.Re()*rqB.Re() - rqPoint1.Im()*rqB.Im();
            m_aqPackedPoint[i].Im() =
                rqPoint0.Im()*rqA.Re() - rqPoint0.Re()*rqA.Im() -
                rqPoint1.Re()*rqB.Im() - rqPoint1.Im()*rqB.Re();
        }

        CtApply(m_aqPackedPoint,m_iHalfN);
        Real fInvHalfN = ((Real)1.0)/m_iHalfN;
        for (int i = 0; i < m_iHalfN; i++)
            m_aqPackedPoint[i] *= fInvHalfN;

        for (int i0 = 0, i1 = 0, i2 = 1; i0 < m_iHalfN; i0++, i1 += 2, i2 += 2)
        {
            aqPoint[i1] = m_aqPackedPoint[i0].Re();
            aqPoint[i2] = m_aqPackedPoint[i0].Im();
        }
    }
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
void DiscreteFourierTransform<Real>::CtApply (Complex<Real>* aqPoint, int iN) const
{
    // Cooley-Tukey FFT algorithm

    // reorder
    int iDst = 0;
    Complex<Real> qSave;
    int iMask;
    for (int i = 0; i < iN; i++)
    {
        if ( iDst > i )
        {
            qSave = aqPoint[iDst];
            aqPoint[iDst] = aqPoint[i];
            aqPoint[i] = qSave;
        }

        iMask = iN;
        for (/**/; /**/; /**/)
        {
            iMask >>= 1;
            if ( (iDst & iMask) == 0 )
                break;
            iDst &= ~iMask;
        }
        iDst |= iMask;
    }

    // apply
    int iJump, iMatch;
    Complex<Real> qFactor, qProd;
    int iStep = 1;
    int iC = 0;
    while ( iStep < iN )
    {
        iJump = iStep << 1;
        for (int i0 = 0; i0 < iStep; i0++)
        {
            qFactor = m_aaqCtLuTable[iC][i0];
            for (int i1 = i0; i1 < iN; i1 += iJump)
            {
                iMatch = i1 + iStep;
                qProd = qFactor*aqPoint[iMatch];
                aqPoint[iMatch] = aqPoint[i1] - qProd;
                aqPoint[i1] += qProd;
            }
        }

        iStep <<= 1;
        iC++;
    }
}
//------------------------------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------------------------------
// explicit instantiation
//------------------------------------------------------------------------------------------------------------------
// float
template
class DiscreteFourierTransform<float>;

// double
template
class DiscreteFourierTransform<double>;
//------------------------------------------------------------------------------------------------------------------



