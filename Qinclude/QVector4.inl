//------------------------------------------------------------------------------------------------------------------
template <class Real>
Vector4<Real>::Vector4 ()
{
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Vector4<Real>::Vector4 (Real fX, Real fY, Real fZ, Real fW)
{
    m_afTuple[0] = fX;
    m_afTuple[1] = fY;
    m_afTuple[2] = fZ;
    m_afTuple[3] = fW;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Vector4<Real>::Vector4 (const Vector4& rqV)
{
    m_afTuple[0] = rqV.m_afTuple[0];
    m_afTuple[1] = rqV.m_afTuple[1];
    m_afTuple[2] = rqV.m_afTuple[2];
    m_afTuple[3] = rqV.m_afTuple[3];
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Vector4<Real>::operator Real* ()
{
    return m_afTuple;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Vector4<Real>::operator const Real* () const
{
    return m_afTuple;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Real Vector4<Real>::operator[] (int i) const
{
    assert( 0 <= i && i <= 3 );
    return m_afTuple[i];
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Real& Vector4<Real>::operator[] (int i)
{
    assert( 0 <= i && i <= 3 );
    return m_afTuple[i];
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Real Vector4<Real>::X () const
{
    return m_afTuple[0];
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Real& Vector4<Real>::X ()
{
    return m_afTuple[0];
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Real Vector4<Real>::Y () const
{
    return m_afTuple[1];
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Real& Vector4<Real>::Y ()
{
    return m_afTuple[1];
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Real Vector4<Real>::Z () const
{
    return m_afTuple[2];
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Real& Vector4<Real>::Z ()
{
    return m_afTuple[2];
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Real Vector4<Real>::W () const
{
    return m_afTuple[3];
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Real& Vector4<Real>::W ()
{
    return m_afTuple[3];
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Vector4<Real>& Vector4<Real>::operator= (const Vector4& rqV)
{
    m_afTuple[0] = rqV.m_afTuple[0];
    m_afTuple[1] = rqV.m_afTuple[1];
    m_afTuple[2] = rqV.m_afTuple[2];
    m_afTuple[3] = rqV.m_afTuple[3];
    return *this;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
bool Vector4<Real>::operator== (const Vector4& rqV) const
{
    return ( MemCmp(m_afTuple,rqV.m_afTuple,4*sizeof(Real)) == 0 );
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
bool Vector4<Real>::operator!= (const Vector4& rqV) const
{
    return ( MemCmp(m_afTuple,rqV.m_afTuple,4*sizeof(Real)) != 0 );
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
bool Vector4<Real>::operator< (const Vector4& rqV) const
{
    return ( MemCmp(m_afTuple,rqV.m_afTuple,4*sizeof(Real)) < 0 );
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
bool Vector4<Real>::operator<= (const Vector4& rqV) const
{
    return ( MemCmp(m_afTuple,rqV.m_afTuple,4*sizeof(Real)) <= 0 );
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
bool Vector4<Real>::operator> (const Vector4& rqV) const
{
    return ( MemCmp(m_afTuple,rqV.m_afTuple,4*sizeof(Real)) > 0 );
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
bool Vector4<Real>::operator>= (const Vector4& rqV) const
{
    return ( MemCmp(m_afTuple,rqV.m_afTuple,4*sizeof(Real)) >= 0 );
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Vector4<Real> Vector4<Real>::operator+ (const Vector4& rqV) const
{
    return Vector4(
        m_afTuple[0]+rqV.m_afTuple[0],
        m_afTuple[1]+rqV.m_afTuple[1],
        m_afTuple[2]+rqV.m_afTuple[2],
        m_afTuple[3]+rqV.m_afTuple[3]);
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Vector4<Real> Vector4<Real>::operator- (const Vector4& rqV) const
{
    return Vector4(
        m_afTuple[0]-rqV.m_afTuple[0],
        m_afTuple[1]-rqV.m_afTuple[1],
        m_afTuple[2]-rqV.m_afTuple[2],
        m_afTuple[3]-rqV.m_afTuple[3]);
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Vector4<Real> Vector4<Real>::operator* (Real fScalar) const
{
    return Vector4(
        fScalar*m_afTuple[0],
        fScalar*m_afTuple[1],
        fScalar*m_afTuple[2],
        fScalar*m_afTuple[3]);
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Vector4<Real> Vector4<Real>::operator/ (Real fScalar) const
{
    Vector4 qQuot;

    if ( fScalar != (Real)0.0 )
    {
        Real fInvScalar = ((Real)1.0)/fScalar;
        qQuot.m_afTuple[0] = fInvScalar*m_afTuple[0];
        qQuot.m_afTuple[1] = fInvScalar*m_afTuple[1];
        qQuot.m_afTuple[2] = fInvScalar*m_afTuple[2];
        qQuot.m_afTuple[3] = fInvScalar*m_afTuple[3];
    }
    else
    {
        qQuot.m_afTuple[0] = Math<Real>::MAX_REAL;
        qQuot.m_afTuple[1] = Math<Real>::MAX_REAL;
        qQuot.m_afTuple[2] = Math<Real>::MAX_REAL;
        qQuot.m_afTuple[3] = Math<Real>::MAX_REAL;
    }

    return qQuot;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Vector4<Real> Vector4<Real>::operator- () const
{
    return Vector4(
        -m_afTuple[0],
        -m_afTuple[1],
        -m_afTuple[2],
        -m_afTuple[3]);
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Vector4<Real> operator* (Real fScalar, const Vector4<Real>& rqV)
{
    return Vector4<Real>(
        fScalar*rqV[0],
        fScalar*rqV[1],
        fScalar*rqV[2],
        fScalar*rqV[3]);
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Vector4<Real>& Vector4<Real>::operator+= (const Vector4& rqV)
{
    m_afTuple[0] += rqV.m_afTuple[0];
    m_afTuple[1] += rqV.m_afTuple[1];
    m_afTuple[2] += rqV.m_afTuple[2];
    m_afTuple[3] += rqV.m_afTuple[3];
    return *this;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Vector4<Real>& Vector4<Real>::operator-= (const Vector4& rqV)
{
    m_afTuple[0] -= rqV.m_afTuple[0];
    m_afTuple[1] -= rqV.m_afTuple[1];
    m_afTuple[2] -= rqV.m_afTuple[2];
    m_afTuple[3] -= rqV.m_afTuple[3];
    return *this;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Vector4<Real>& Vector4<Real>::operator*= (Real fScalar)
{
    m_afTuple[0] *= fScalar;
    m_afTuple[1] *= fScalar;
    m_afTuple[2] *= fScalar;
    m_afTuple[3] *= fScalar;
    return *this;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Vector4<Real>& Vector4<Real>::operator/= (Real fScalar)
{
    if ( fScalar != (Real)0.0 )
    {
        Real fInvScalar = ((Real)1.0)/fScalar;
        m_afTuple[0] *= fInvScalar;
        m_afTuple[1] *= fInvScalar;
        m_afTuple[2] *= fInvScalar;
        m_afTuple[3] *= fInvScalar;
    }
    else
    {
        m_afTuple[0] = Math<Real>::MAX_REAL;
        m_afTuple[1] = Math<Real>::MAX_REAL;
        m_afTuple[2] = Math<Real>::MAX_REAL;
        m_afTuple[3] = Math<Real>::MAX_REAL;
    }

    return *this;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Real Vector4<Real>::Length () const
{
    return Math<Real>::Sqrt(
        m_afTuple[0]*m_afTuple[0]+
        m_afTuple[1]*m_afTuple[1]+
        m_afTuple[2]*m_afTuple[2]+
        m_afTuple[3]*m_afTuple[3]);
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Real Vector4<Real>::SquaredLength () const
{
    return
        m_afTuple[0]*m_afTuple[0] +
        m_afTuple[1]*m_afTuple[1] +
        m_afTuple[2]*m_afTuple[2] +
        m_afTuple[3]*m_afTuple[3];
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Real Vector4<Real>::Dot (const Vector4& rqV) const
{
    return
        m_afTuple[0]*rqV.m_afTuple[0] +
        m_afTuple[1]*rqV.m_afTuple[1] +
        m_afTuple[2]*rqV.m_afTuple[2] +
        m_afTuple[3]*rqV.m_afTuple[3];
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Real Vector4<Real>::Normalize ()
{
    Real fLength = Length();

    if ( fLength > Math<Real>::ZERO_TOLERANCE )
    {
        Real fInvLength = ((Real)1.0)/fLength;
        m_afTuple[0] *= fInvLength;
        m_afTuple[1] *= fInvLength;
        m_afTuple[2] *= fInvLength;
        m_afTuple[3] *= fInvLength;
    }
    else
    {
        fLength = (Real)0.0;
        m_afTuple[0] = (Real)0.0;
        m_afTuple[1] = (Real)0.0;
        m_afTuple[2] = (Real)0.0;
        m_afTuple[3] = (Real)0.0;
    }

    return fLength;
}
//------------------------------------------------------------------------------------------------------------------



