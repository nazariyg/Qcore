//------------------------------------------------------------------------------------------------------------------
template <class Real>
GVector<Real>::GVector (int iSize)
{
    if ( iSize > 0 )
    {
        m_iSize = iSize;
        m_afTuple = new Real[m_iSize];
        MemSet(m_afTuple,0,m_iSize*sizeof(Real));
    }
    else
    {
        m_iSize = 0;
        m_afTuple = 0;
    }
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
GVector<Real>::GVector (int iSize, const Real* afTuple)
{
    if ( iSize > 0 )
    {
        m_iSize = iSize;
        m_afTuple = new Real[m_iSize];
        MemCpy(m_afTuple,afTuple,m_iSize*sizeof(Real));
    }
    else
    {
        m_iSize = 0;
        m_afTuple = 0;
    }
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
GVector<Real>::GVector (const GVector& rqV)
{
    m_iSize = rqV.m_iSize;
    if ( m_iSize > 0 )
    {
        m_afTuple = new Real[m_iSize];
        MemCpy(m_afTuple,rqV.m_afTuple,m_iSize*sizeof(Real));
    }
    else
    {
        m_afTuple = 0;
    }
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
GVector<Real>::~GVector ()
{
    delete[] m_afTuple;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
void GVector<Real>::SetSize (int iSize)
{
    delete[] m_afTuple;
    if ( iSize > 0 )
    {
        m_iSize = iSize;
        m_afTuple = new Real[m_iSize];
        MemSet(m_afTuple,0,m_iSize*sizeof(Real));
    }
    else
    {
        m_iSize = 0;
        m_afTuple = 0;
    }
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
int GVector<Real>::GetSize () const
{
    return m_iSize;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
GVector<Real>::operator const Real* () const
{
    return m_afTuple;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
GVector<Real>::operator Real* ()
{
    return m_afTuple;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Real GVector<Real>::operator[] (int i) const
{
    assert( 0 <= i && i < m_iSize );
    return m_afTuple[i];
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Real& GVector<Real>::operator[] (int i)
{
    assert( 0 <= i && i < m_iSize );
    return m_afTuple[i];
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
GVector<Real>& GVector<Real>::operator= (const GVector& rqV)
{
    if ( rqV.m_iSize > 0 )
    {
        if ( m_iSize != rqV.m_iSize )
        {
            delete[] m_afTuple;
            m_iSize = rqV.m_iSize;
            m_afTuple = new Real[m_iSize];
        }
        MemCpy(m_afTuple,rqV.m_afTuple,m_iSize*sizeof(Real));
    }
    else
    {
        delete[] m_afTuple;
        m_iSize = 0;
        m_afTuple = 0;
    }
    return *this;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
int GVector<Real>::CompareArrays (const GVector& rqV) const
{
    return MemCmp(m_afTuple,rqV.m_afTuple,m_iSize*sizeof(Real));
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
bool GVector<Real>::operator== (const GVector& rqV) const
{
    return ( CompareArrays(rqV) == 0 );
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
bool GVector<Real>::operator!= (const GVector& rqV) const
{
    return ( CompareArrays(rqV) != 0 );
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
bool GVector<Real>::operator< (const GVector& rqV) const
{
    return ( CompareArrays(rqV) < 0 );
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
bool GVector<Real>::operator<= (const GVector& rqV) const
{
    return ( CompareArrays(rqV) <= 0 );
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
bool GVector<Real>::operator> (const GVector& rqV) const
{
    return ( CompareArrays(rqV) > 0 );
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
bool GVector<Real>::operator>= (const GVector& rqV) const
{
    return ( CompareArrays(rqV) >= 0 );
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
GVector<Real> GVector<Real>::operator+ (const GVector& rqV) const
{
    GVector<Real> qSum(m_iSize);
    for (int i = 0; i < m_iSize; i++)
        qSum.m_afTuple[i] = m_afTuple[i] + rqV.m_afTuple[i];
    return qSum;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
GVector<Real> GVector<Real>::operator- (const GVector& rqV) const
{
    GVector<Real> qDiff(m_iSize);
    for (int i = 0; i < m_iSize; i++)
        qDiff.m_afTuple[i] = m_afTuple[i] - rqV.m_afTuple[i];
    return qDiff;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
GVector<Real> GVector<Real>::operator* (Real fScalar) const
{
    GVector<Real> qProd(m_iSize);
    for (int i = 0; i < m_iSize; i++)
        qProd.m_afTuple[i] = fScalar*m_afTuple[i];
    return qProd;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
GVector<Real> GVector<Real>::operator/ (Real fScalar) const
{
    GVector<Real> qQuot(m_iSize);
    int i;

    if ( fScalar != (Real)0.0 )
    {
        Real fInvScalar = ((Real)1.0)/fScalar;
        for (i = 0; i < m_iSize; i++)
            qQuot.m_afTuple[i] = fInvScalar*m_afTuple[i];
    }
    else
    {
        for (i = 0; i < m_iSize; i++)
            qQuot.m_afTuple[i] = Math<Real>::MAX_REAL;
    }

    return qQuot;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
GVector<Real> GVector<Real>::operator- () const
{
    GVector<Real> qNeg(m_iSize);
    for (int i = 0; i < m_iSize; i++)
        qNeg.m_afTuple[i] = -m_afTuple[i];
    return qNeg;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
GVector<Real> operator* (Real fScalar, const GVector<Real>& rqV)
{
    GVector<Real> qProd(rqV.GetSize());
    for (int i = 0; i < rqV.GetSize(); i++)
        qProd[i] = fScalar*rqV[i];
    return qProd;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
GVector<Real>& GVector<Real>::operator+= (const GVector& rqV)
{
    for (int i = 0; i < m_iSize; i++)
        m_afTuple[i] += rqV.m_afTuple[i];
    return *this;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
GVector<Real>& GVector<Real>::operator-= (const GVector& rqV)
{
    for (int i = 0; i < m_iSize; i++)
        m_afTuple[i] -= rqV.m_afTuple[i];
    return *this;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
GVector<Real>& GVector<Real>::operator*= (Real fScalar)
{
    for (int i = 0; i < m_iSize; i++)
        m_afTuple[i] *= fScalar;
    return *this;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
GVector<Real>& GVector<Real>::operator/= (Real fScalar)
{
    int i;

    if ( fScalar != (Real)0.0 )
    {
        Real fInvScalar = ((Real)1.0)/fScalar;
        for (i = 0; i < m_iSize; i++)
            m_afTuple[i] *= fInvScalar;
    }
    else
    {
        for (i = 0; i < m_iSize; i++)
            m_afTuple[i] = Math<Real>::MAX_REAL;
    }

    return *this;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Real GVector<Real>::Length () const
{
    Real fSqrLen = (Real)0.0;
    for (int i = 0; i < m_iSize; i++)
        fSqrLen += m_afTuple[i]*m_afTuple[i];
    return Math<Real>::Sqrt(fSqrLen);
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Real GVector<Real>::SquaredLength () const
{
    Real fSqrLen = (Real)0.0;
    for (int i = 0; i < m_iSize; i++)
        fSqrLen += m_afTuple[i]*m_afTuple[i];
    return fSqrLen;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Real GVector<Real>::Dot (const GVector& rqV) const
{
    Real fDot = (Real)0.0;
    for (int i = 0; i < m_iSize; i++)
        fDot += m_afTuple[i]*rqV.m_afTuple[i];
    return fDot;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Real GVector<Real>::Normalize ()
{
    Real fLength = Length();
    int i;

    if ( fLength > Math<Real>::ZERO_TOLERANCE )
    {
        Real fInvLength = ((Real)1.0)/fLength;
        for (i = 0; i < m_iSize; i++)
            m_afTuple[i] *= fInvLength;
    }
    else
    {
        fLength = (Real)0.0;
        for (i = 0; i < m_iSize; i++)
            m_afTuple[i] = (Real)0.0;
    }

    return fLength;
}
//------------------------------------------------------------------------------------------------------------------



