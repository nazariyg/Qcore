#include "QDiscreteCosineTransform.h"
#include "QMath.h"
using namespace Q;

//------------------------------------------------------------------------------------------------------------------
template <class Real>
DiscreteCosineTransform<Real>::DiscreteCosineTransform (DctType eDct, int iSideExtension)
    :
    m_eDct(eDct),
    m_iSideExtension(iSideExtension)
{
    assert( iSideExtension >= 1 );

    // create helper tables
    if ( eDct == DCTT_ORTHOGONAL_2D )
    {
        m_aafTables = new Real*[iSideExtension];

        Real fVal = Math<Real>::PI/(2*iSideExtension);
        int iVal;
        for (int i0 = 0; i0 < iSideExtension; i0++)
        {
            m_aafTables[i0] = new Real[iSideExtension];

            iVal = 2*i0 + 1;
            for (int i1 = 0; i1 < iSideExtension; i1++)
                m_aafTables[i0][i1] = Math<Real>::Cos(iVal*i1*fVal);
        }
    }
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
DiscreteCosineTransform<Real>::~DiscreteCosineTransform ()
{
    for (int i = 0; i < m_iSideExtension; i++)
        delete[] m_aafTables[i];
    delete[] m_aafTables;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
void DiscreteCosineTransform<Real>::ApplyForward2d (Real* afElement) const
{
    int iArea = m_iSideExtension*m_iSideExtension;
    Real* afElementCopy = new Real[iArea];
    MemCpy(afElementCopy,afElement,iArea*sizeof(Real));

    Real fSum;
    if ( m_eDct == DCTT_ORTHOGONAL_2D )
    {
        Real fInvSqr2 = ((Real)1.0)/Math<Real>::Sqrt((Real)2.0);
        Real f2DSE = ((Real)2.0)/m_iSideExtension;
        int iInColsBefore0, iInColsBefore1;
        for (int iU = 0; iU < m_iSideExtension; iU++)
        {
            iInColsBefore0 = iU*m_iSideExtension;
            for (int iV = 0; iV < m_iSideExtension; iV++)
            {
                fSum = (Real)0.0;
                for (int iX = 0; iX < m_iSideExtension; iX++)
                {
                    iInColsBefore1 = iX*m_iSideExtension;
                    for (int iY = 0; iY < m_iSideExtension; iY++)
                        fSum += afElementCopy[iInColsBefore1+iY]*m_aafTables[iX][iU]*m_aafTables[iY][iV];
                }
                fSum *= f2DSE;
                if ( iU == 0 )
                    fSum *= fInvSqr2;
                if ( iV == 0 )
                    fSum *= fInvSqr2;

                afElement[iInColsBefore0+iV] = fSum;
            }
        }
    }

    delete[] afElementCopy;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
void DiscreteCosineTransform<Real>::ApplyInverse2d (Real* afElement) const
{
    int iArea = m_iSideExtension*m_iSideExtension;
    Real* afElementCopy = new Real[iArea];
    MemCpy(afElementCopy,afElement,iArea*sizeof(Real));

    Real fSum;
    if ( m_eDct == DCTT_ORTHOGONAL_2D )
    {
        Real fProd;
        Real fInvSqr2 = ((Real)1.0)/Math<Real>::Sqrt((Real)2.0);
        Real f2DSE = ((Real)2.0)/m_iSideExtension;
        int iInColsBefore0, iInColsBefore1;
        for (int iX = 0; iX < m_iSideExtension; iX++)
        {
            iInColsBefore0 = iX*m_iSideExtension;
            for (int iY = 0; iY < m_iSideExtension; iY++)
            {
                fSum = (Real)0.0;
                for (int iU = 0; iU < m_iSideExtension; iU++)
                {
                    iInColsBefore1 = iU*m_iSideExtension;
                    for (int iV = 0; iV < m_iSideExtension; iV++)
                    {
                        fProd = afElementCopy[iInColsBefore1+iV]*m_aafTables[iX][iU]*m_aafTables[iY][iV];
                        if ( iU == 0 )
                            fProd *= fInvSqr2;
                        if ( iV == 0 )
                            fProd *= fInvSqr2;

                        fSum += fProd;
                    }
                }
                afElement[iInColsBefore0+iY] = f2DSE*fSum;
            }
        }
    }

    delete[] afElementCopy;
}
//------------------------------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------------------------------
// explicit instantiation
//------------------------------------------------------------------------------------------------------------------
// float
template
class DiscreteCosineTransform<float>;

// double
template
class DiscreteCosineTransform<double>;
//------------------------------------------------------------------------------------------------------------------



