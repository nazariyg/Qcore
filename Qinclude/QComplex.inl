//------------------------------------------------------------------------------------------------------------------
template <class Real>
Complex<Real>::Complex ()
{
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Complex<Real>::Complex (Real fRe, Real fIm)
{
    m_afPart[0] = fRe;
    m_afPart[1] = fIm;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Complex<Real>::Complex (Real fValue)
{
    m_afPart[0] = fValue;
    m_afPart[1] = (Real)0.0;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Complex<Real>::Complex (const Complex& rqC)
{
    m_afPart[0] = rqC.m_afPart[0];
    m_afPart[1] = rqC.m_afPart[1];
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Real Complex<Real>::Re () const
{
    return m_afPart[0];
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Real& Complex<Real>::Re ()
{
    return m_afPart[0];
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Real Complex<Real>::Im () const
{
    return m_afPart[1];
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Real& Complex<Real>::Im ()
{
    return m_afPart[1];
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Complex<Real>& Complex<Real>::operator= (const Complex& rqC)
{
    m_afPart[0] = rqC.m_afPart[0];
    m_afPart[1] = rqC.m_afPart[1];
    return *this;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Complex<Real>& Complex<Real>::operator= (Real fValue)
{
    m_afPart[0] = fValue;
    m_afPart[1] = (Real)0.0;
    return *this;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
bool Complex<Real>::operator== (const Complex& rqC) const
{
    return ( MemCmp(m_afPart,rqC.m_afPart,2*sizeof(Real)) == 0 );
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
bool Complex<Real>::operator!= (const Complex& rqC) const
{
    return ( MemCmp(m_afPart,rqC.m_afPart,2*sizeof(Real)) != 0 );
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
bool Complex<Real>::operator< (const Complex& rqC) const
{
    return ( MemCmp(m_afPart,rqC.m_afPart,2*sizeof(Real)) < 0 );
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
bool Complex<Real>::operator<= (const Complex& rqC) const
{
    return ( MemCmp(m_afPart,rqC.m_afPart,2*sizeof(Real)) <= 0 );
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
bool Complex<Real>::operator> (const Complex& rqC) const
{
    return ( MemCmp(m_afPart,rqC.m_afPart,2*sizeof(Real)) > 0 );
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
bool Complex<Real>::operator>= (const Complex& rqC) const
{
    return ( MemCmp(m_afPart,rqC.m_afPart,2*sizeof(Real)) >= 0 );
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
bool Complex<Real>::operator== (Real fValue) const
{
    return ( m_afPart[0] == fValue && m_afPart[1] == (Real)0.0 );
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
bool Complex<Real>::operator!= (Real fValue) const
{
    return ( m_afPart[0] != fValue || m_afPart[1] != (Real)0.0 );
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Complex<Real> Complex<Real>::operator+ (const Complex& rqC) const
{
    return Complex(
        m_afPart[0]+rqC.m_afPart[0],
        m_afPart[1]+rqC.m_afPart[1]);
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Complex<Real> Complex<Real>::operator- (const Complex& rqC) const
{
    return Complex(
        m_afPart[0]-rqC.m_afPart[0],
        m_afPart[1]-rqC.m_afPart[1]);
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Complex<Real> Complex<Real>::operator* (const Complex& rqC) const
{
    return Complex(
        m_afPart[0]*rqC.m_afPart[0]-m_afPart[1]*rqC.m_afPart[1],
        m_afPart[0]*rqC.m_afPart[1]+m_afPart[1]*rqC.m_afPart[0]);
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Complex<Real> Complex<Real>::operator/ (const Complex& rqC) const
{
    Real fDenom = rqC.m_afPart[0]*rqC.m_afPart[0] + rqC.m_afPart[1]*rqC.m_afPart[1];
    if ( fDenom != (Real)0.0 )
    {
        Real fInvDenom = ((Real)1.0)/fDenom;
        return Complex(
            (m_afPart[0]*rqC.m_afPart[0]+m_afPart[1]*rqC.m_afPart[1])*fInvDenom,
            (m_afPart[1]*rqC.m_afPart[0]-m_afPart[0]*rqC.m_afPart[1])*fInvDenom);
    }
    else
    {
        assert( false );
        return Complex(
            Math<Real>::Sign(m_afPart[0]*rqC.m_afPart[0]+m_afPart[1]*rqC.m_afPart[1])*Math<Real>::MAX_REAL,
            Math<Real>::Sign(m_afPart[1]*rqC.m_afPart[0]-m_afPart[0]*rqC.m_afPart[1])*Math<Real>::MAX_REAL);
    }
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Complex<Real> Complex<Real>::operator- () const
{
    return Complex(
        -m_afPart[0],
        -m_afPart[1]);
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Complex<Real> Complex<Real>::operator+ (Real fValue) const
{
    return Complex(
        m_afPart[0]+fValue,
        m_afPart[1]);
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Complex<Real> Complex<Real>::operator- (Real fValue) const
{
    return Complex(
        m_afPart[0]-fValue,
        m_afPart[1]);
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Complex<Real> Complex<Real>::operator* (Real fValue) const
{
    return Complex(
        m_afPart[0]*fValue,
        m_afPart[1]*fValue);
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Complex<Real> Complex<Real>::operator/ (Real fValue) const
{
    if ( fValue != (Real)0.0 )
    {
        Real fInvValue = ((Real)1.0)/fValue;
        return Complex(
            m_afPart[0]*fInvValue,
            m_afPart[1]*fInvValue);
    }
    else
    {
        assert( false );
        return Complex(
            Math<Real>::Sign(m_afPart[0])*Math<Real>::MAX_REAL,
            Math<Real>::Sign(m_afPart[1])*Math<Real>::MAX_REAL);
    }
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Complex<Real>& Complex<Real>::operator+= (const Complex& rqC)
{
    m_afPart[0] += rqC.m_afPart[0];
    m_afPart[1] += rqC.m_afPart[1];
    return *this;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Complex<Real>& Complex<Real>::operator-= (const Complex& rqC)
{
    m_afPart[0] -= rqC.m_afPart[0];
    m_afPart[1] -= rqC.m_afPart[1];
    return *this;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Complex<Real>& Complex<Real>::operator*= (const Complex& rqC)
{
    Real fRe0 = m_afPart[0];
    m_afPart[0] = fRe0*rqC.m_afPart[0] - m_afPart[1]*rqC.m_afPart[1];
    m_afPart[1] = fRe0*rqC.m_afPart[1] + m_afPart[1]*rqC.m_afPart[0];
    return *this;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Complex<Real>& Complex<Real>::operator/= (const Complex& rqC)
{
    Real fDenom = rqC.m_afPart[0]*rqC.m_afPart[0] + rqC.m_afPart[1]*rqC.m_afPart[1];
    if ( fDenom != (Real)0.0 )
    {
        Real fInvDenom = ((Real)1.0)/fDenom;
        Real fRe0 = m_afPart[0];
        m_afPart[0] = (fRe0*rqC.m_afPart[0] + m_afPart[1]*rqC.m_afPart[1])*fInvDenom;
        m_afPart[1] = (m_afPart[1]*rqC.m_afPart[0] - fRe0*rqC.m_afPart[1])*fInvDenom;
    }
    else
    {
        assert( false );
        Real fRe0 = m_afPart[0];
        m_afPart[0] = Math<Real>::Sign(fRe0*rqC.m_afPart[0] + m_afPart[1]*rqC.m_afPart[1])*Math<Real>::MAX_REAL;
        m_afPart[1] = Math<Real>::Sign(m_afPart[1]*rqC.m_afPart[0] - fRe0*rqC.m_afPart[1])*Math<Real>::MAX_REAL;
    }
    return *this;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Complex<Real>& Complex<Real>::operator+= (Real fValue)
{
    m_afPart[0] += fValue;
    return *this;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Complex<Real>& Complex<Real>::operator-= (Real fValue)
{
    m_afPart[0] -= fValue;
    return *this;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Complex<Real>& Complex<Real>::operator*= (Real fValue)
{
    m_afPart[0] *= fValue;
    m_afPart[1] *= fValue;
    return *this;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Complex<Real>& Complex<Real>::operator/= (Real fValue)
{
    if ( fValue != (Real)0.0 )
    {
        Real fInvValue = ((Real)1.0)/fValue;
        m_afPart[0] *= fInvValue;
        m_afPart[1] *= fInvValue;
    }
    else
    {
        assert( false );
        m_afPart[0] = Math<Real>::Sign(m_afPart[0])*Math<Real>::MAX_REAL;
        m_afPart[1] = Math<Real>::Sign(m_afPart[1])*Math<Real>::MAX_REAL;
    }
    return *this;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Complex<Real> Complex<Real>::Conj () const
{
    return Complex(m_afPart[0],-m_afPart[1]);
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Real Complex<Real>::Abs () const
{
    return Math<Real>::Sqrt(m_afPart[0]*m_afPart[0]+m_afPart[1]*m_afPart[1]);
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Real Complex<Real>::Norm () const
{
    return m_afPart[0]*m_afPart[0] + m_afPart[1]*m_afPart[1];
}
//------------------------------------------------------------------------------------------------------------------



