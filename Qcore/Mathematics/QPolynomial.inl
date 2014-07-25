//------------------------------------------------------------------------------------------------------------------
template <class Real>
Polynomial<Real>::Polynomial (int iDegree)
{
    if ( iDegree >= 0 )
    {
        m_iDegree = iDegree;
        m_afCoeff = new Real[m_iDegree+1];
    }
    else
    {
        // default creation
        m_iDegree = -1;
        m_afCoeff = 0;
    }
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Polynomial<Real>::Polynomial (const Polynomial& rqPoly)
{
    m_iDegree = rqPoly.m_iDegree;
    m_afCoeff = new Real[m_iDegree+1];
    for (int i = 0; i <= m_iDegree; i++)
        m_afCoeff[i] = rqPoly.m_afCoeff[i];
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Polynomial<Real>::~Polynomial ()
{
    delete[] m_afCoeff;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
void Polynomial<Real>::SetDegree (int iDegree)
{
    m_iDegree = iDegree;
    delete[] m_afCoeff;

    if ( m_iDegree >= 0 )
        m_afCoeff = new Real[m_iDegree+1];
    else
        m_afCoeff = 0;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
int Polynomial<Real>::GetDegree () const
{
    return m_iDegree;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Polynomial<Real>::operator const Real* () const
{
    return m_afCoeff;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Polynomial<Real>::operator Real* ()
{
    return m_afCoeff;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Real Polynomial<Real>::operator[] (int i) const
{
    assert( 0 <= i && i <= m_iDegree );
    return m_afCoeff[i];
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Real& Polynomial<Real>::operator[] (int i)
{
    assert( 0 <= i && i <= m_iDegree );
    return m_afCoeff[i];
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Polynomial<Real>& Polynomial<Real>::operator= (const Polynomial& rqPoly)
{
    delete[] m_afCoeff;
    m_iDegree = rqPoly.m_iDegree;

    if ( m_iDegree >= 0 )
    {
        m_afCoeff = new Real[m_iDegree+1];
        for (int i = 0; i <= m_iDegree; i++)
            m_afCoeff[i] = rqPoly.m_afCoeff[i];
    }

    return *this;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Real Polynomial<Real>::operator() (Real fT) const
{
    assert( m_iDegree >= 0 );

    Real fResult = m_afCoeff[m_iDegree];
    for (int i = m_iDegree-1; i >= 0; i--)
    {
        fResult *= fT;
        fResult += m_afCoeff[i];
    }
    return fResult;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Polynomial<Real> Polynomial<Real>::operator+ (const Polynomial& rqPoly) const
{
    assert( m_iDegree >= 0 && rqPoly.m_iDegree >= 0 );

    Polynomial qSum;
    int i;

    if ( m_iDegree > rqPoly.m_iDegree )
    {
        qSum.SetDegree(m_iDegree);
        for (i = 0; i <= rqPoly.m_iDegree; i++)
            qSum.m_afCoeff[i] = m_afCoeff[i] + rqPoly.m_afCoeff[i];
        for (i = rqPoly.m_iDegree+1; i <= m_iDegree; i++)
            qSum.m_afCoeff[i] = m_afCoeff[i];
    }
    else
    {
        qSum.SetDegree(rqPoly.m_iDegree);
        for (i = 0; i <= m_iDegree; i++)
            qSum.m_afCoeff[i] = m_afCoeff[i] + rqPoly.m_afCoeff[i];
        for (i = m_iDegree+1; i <= rqPoly.m_iDegree; i++)
            qSum.m_afCoeff[i] = rqPoly.m_afCoeff[i];
    }

    return qSum;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Polynomial<Real> Polynomial<Real>::operator- (const Polynomial& rqPoly) const
{
    assert( m_iDegree >= 0 && rqPoly.m_iDegree >= 0 );

    Polynomial qDiff;
    int i;

    if ( m_iDegree > rqPoly.m_iDegree )
    {
        qDiff.SetDegree(m_iDegree);
        for (i = 0; i <= rqPoly.m_iDegree; i++)
            qDiff.m_afCoeff[i] = m_afCoeff[i] - rqPoly.m_afCoeff[i];
        for (i = rqPoly.m_iDegree+1; i <= m_iDegree; i++)
            qDiff.m_afCoeff[i] = m_afCoeff[i];
    }
    else
    {
        qDiff.SetDegree(rqPoly.m_iDegree);
        for (i = 0; i <= m_iDegree; i++)
            qDiff.m_afCoeff[i] = m_afCoeff[i] - rqPoly.m_afCoeff[i];
        for (i = m_iDegree+1; i <= rqPoly.m_iDegree; i++)
            qDiff.m_afCoeff[i] = -rqPoly.m_afCoeff[i];
    }

    return qDiff;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Polynomial<Real> Polynomial<Real>::operator* (const Polynomial& rqPoly) const
{
    assert( m_iDegree >= 0 && rqPoly.m_iDegree >= 0 );

    Polynomial qProd(m_iDegree+rqPoly.m_iDegree);

    MemSet(qProd.m_afCoeff,0,(qProd.m_iDegree+1)*sizeof(Real));
    for (int i0 = 0; i0 <= m_iDegree; i0++)
    {
        for (int i1 = 0; i1 <= rqPoly.m_iDegree; i1++)
        {
            int i2 = i0 + i1;
            qProd.m_afCoeff[i2] += m_afCoeff[i0]*rqPoly.m_afCoeff[i1];
        }
    }

    return qProd;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Polynomial<Real> Polynomial<Real>::operator+ (Real fScalar) const
{
    assert( m_iDegree >= 0 );
    Polynomial qSum(m_iDegree);
    qSum.m_afCoeff[0] += fScalar;
    return qSum;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Polynomial<Real> Polynomial<Real>::operator- (Real fScalar) const
{
    assert( m_iDegree >= 0 );
    Polynomial qDiff(m_iDegree);
    qDiff.m_afCoeff[0] -= fScalar;
    return qDiff;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Polynomial<Real> Polynomial<Real>::operator* (Real fScalar) const
{
    assert( m_iDegree >= 0 );

    Polynomial qProd(m_iDegree);
    for (int i = 0; i <= m_iDegree; i++)
        qProd.m_afCoeff[i] = fScalar*m_afCoeff[i];

    return qProd;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Polynomial<Real> Polynomial<Real>::operator/ (Real fScalar) const
{
    assert( m_iDegree >= 0 );

    Polynomial qProd(m_iDegree);
    int i;

    if ( fScalar != (Real)0.0 )
    {
        Real fInvScalar = ((Real)1.0)/fScalar;
        for (i = 0; i <= m_iDegree; i++)
            qProd.m_afCoeff[i] = fInvScalar*m_afCoeff[i];
    }
    else
    {
        for (i = 0; i <= m_iDegree; i++)
            qProd.m_afCoeff[i] = Math<Real>::MAX_REAL;
    }

    return qProd;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Polynomial<Real> Polynomial<Real>::operator- () const
{
    assert( m_iDegree >= 0 );

    Polynomial qNeg(m_iDegree);
    for (int i = 0; i <= m_iDegree; i++)
        qNeg.m_afCoeff[i] = -m_afCoeff[i];

    return qNeg;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Polynomial<Real> operator* (Real fScalar, const Polynomial<Real>& rqPoly)
{
    assert( rqPoly.GetDegree() >= 0 );

    Polynomial<Real> qProd(rqPoly.GetDegree());
    for (int i = 0; i <= rqPoly.GetDegree(); i++)
        qProd[i] = fScalar*rqPoly[i];

    return qProd;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Polynomial<Real>& Polynomial<Real>::operator+= (const Polynomial& rqPoly)
{
    assert( m_iDegree >= 0 );
    *this = *this + rqPoly;
    return *this;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Polynomial<Real>& Polynomial<Real>::operator-= (const Polynomial& rqPoly)
{
    assert( m_iDegree >= 0 );
    *this = *this - rqPoly;
    return *this;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Polynomial<Real>& Polynomial<Real>::operator*= (const Polynomial& rqPoly)
{
    assert( m_iDegree >= 0 );
    *this = (*this)*rqPoly;
    return *this;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Polynomial<Real>& Polynomial<Real>::operator+= (Real fScalar)
{
    assert( m_iDegree >= 0 );
    m_afCoeff[0] += fScalar;
    return *this;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Polynomial<Real>& Polynomial<Real>::operator-= (Real fScalar)
{
    assert( m_iDegree >= 0 );
    m_afCoeff[0] -= fScalar;
    return *this;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Polynomial<Real>& Polynomial<Real>::operator*= (Real fScalar)
{
    assert( m_iDegree >= 0 );
    *this = (*this)*fScalar;
    return *this;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Polynomial<Real>& Polynomial<Real>::operator/= (Real fScalar)
{
    assert( m_iDegree >= 0 );
    *this = (*this)/fScalar;
    return *this;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Polynomial<Real> Polynomial<Real>::GetDerivative () const
{
    if ( m_iDegree > 0 )
    {
        Polynomial qDeriv(m_iDegree-1);
        for (int i0 = 0, i1 = 1; i0 < m_iDegree; i0++, i1++)
            qDeriv.m_afCoeff[i0] = i1*m_afCoeff[i1];
        return qDeriv;
    }
    else if ( m_iDegree == 0 )
    {
        Polynomial qConst(0);
        qConst.m_afCoeff[0] = (Real)0.0;
        return qConst;
    }
    return Polynomial<Real>();  // invalid in, invalid out
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Polynomial<Real> Polynomial<Real>::GetInversion () const
{
    Polynomial qInvPoly(m_iDegree);
    for (int i = 0; i <= m_iDegree; i++)
        qInvPoly.m_afCoeff[i] = m_afCoeff[m_iDegree-i];
    return qInvPoly;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
void Polynomial<Real>::Compress (Real fEpsilon)
{
    int i;
    for (i = m_iDegree; i >= 0; i--)
    {
        if ( Math<Real>::FAbs(m_afCoeff[i]) <= fEpsilon )
            m_iDegree--;
        else
            break;
    }

    if ( m_iDegree >= 0 )
    {
        Real fInvLeading = ((Real)1.0)/m_afCoeff[m_iDegree];
        m_afCoeff[m_iDegree] = (Real)1.0;
        for (i = 0; i < m_iDegree; i++)
            m_afCoeff[i] *= fInvLeading;
    }
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
void Polynomial<Real>::Divide (const Polynomial& rqDiv, Polynomial& rqQuot, Polynomial& rqRem, Real fEpsilon) const
{
    int iQuotDegree = m_iDegree - rqDiv.m_iDegree;
    if ( iQuotDegree >= 0 )
    {
        rqQuot.SetDegree(iQuotDegree);

        // temporary storage for the remainder
        Polynomial qTmp = *this;

        // do the division (Euclidean algorithm)
        Real fInv = ((Real)1.0)/rqDiv[rqDiv.m_iDegree];
        for (int iQ = iQuotDegree; iQ >= 0; iQ--)
        {
            int iR = rqDiv.m_iDegree + iQ;
            rqQuot[iQ] = fInv*qTmp[iR];
            for (iR--; iR >= iQ; iR--)
                qTmp[iR] -= rqQuot[iQ]*rqDiv[iR-iQ];
        }

        // calculate the correct degree for the remainder
        int iRemDeg = rqDiv.m_iDegree - 1;
        while ( iRemDeg > 0 && Math<Real>::FAbs(qTmp[iRemDeg]) < fEpsilon )
            iRemDeg--;

        if ( iRemDeg == 0 && Math<Real>::FAbs(qTmp[0]) < fEpsilon )
            qTmp[0] = (Real)0.0;

        rqRem.SetDegree(iRemDeg);
        MemCpy(rqRem.m_afCoeff,qTmp.m_afCoeff,(iRemDeg+1)*sizeof(Real));
    }
    else
    {
        rqQuot.SetDegree(0);
        rqQuot[0] = (Real)0.0;
        rqRem = *this;
    }
}
//------------------------------------------------------------------------------------------------------------------



