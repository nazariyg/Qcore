//------------------------------------------------------------------------------------------------------------------
template <class Real>
Vector2<Real>::Vector2 ()
{
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Vector2<Real>::Vector2 (Real fX, Real fY)
{
    m_afTuple[0] = fX;
    m_afTuple[1] = fY;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Vector2<Real>::Vector2 (const Vector2& rqV)
{
    m_afTuple[0] = rqV.m_afTuple[0];
    m_afTuple[1] = rqV.m_afTuple[1];
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Vector2<Real>::operator Real* ()
{
    return m_afTuple;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Vector2<Real>::operator const Real* () const
{
    return m_afTuple;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Real Vector2<Real>::operator[] (int i) const
{
    assert( 0 <= i && i <= 1 );
    return m_afTuple[i];
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Real& Vector2<Real>::operator[] (int i)
{
    assert( 0 <= i && i <= 1 );
    return m_afTuple[i];
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Real Vector2<Real>::X () const
{
    return m_afTuple[0];
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Real& Vector2<Real>::X ()
{
    return m_afTuple[0];
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Real Vector2<Real>::Y () const
{
    return m_afTuple[1];
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Real& Vector2<Real>::Y ()
{
    return m_afTuple[1];
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Vector2<Real>& Vector2<Real>::operator= (const Vector2& rqV)
{
    m_afTuple[0] = rqV.m_afTuple[0];
    m_afTuple[1] = rqV.m_afTuple[1];
    return *this;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
bool Vector2<Real>::operator== (const Vector2& rqV) const
{
    return ( MemCmp(m_afTuple,rqV.m_afTuple,2*sizeof(Real)) == 0 );
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
bool Vector2<Real>::operator!= (const Vector2& rqV) const
{
    return ( MemCmp(m_afTuple,rqV.m_afTuple,2*sizeof(Real)) != 0 );
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
bool Vector2<Real>::operator< (const Vector2& rqV) const
{
    return ( MemCmp(m_afTuple,rqV.m_afTuple,2*sizeof(Real)) < 0 );
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
bool Vector2<Real>::operator<= (const Vector2& rqV) const
{
    return ( MemCmp(m_afTuple,rqV.m_afTuple,2*sizeof(Real)) <= 0 );
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
bool Vector2<Real>::operator> (const Vector2& rqV) const
{
    return ( MemCmp(m_afTuple,rqV.m_afTuple,2*sizeof(Real)) > 0 );
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
bool Vector2<Real>::operator>= (const Vector2& rqV) const
{
    return ( MemCmp(m_afTuple,rqV.m_afTuple,2*sizeof(Real)) >= 0 );
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Vector2<Real> Vector2<Real>::operator+ (const Vector2& rqV) const
{
    return Vector2(
        m_afTuple[0]+rqV.m_afTuple[0],
        m_afTuple[1]+rqV.m_afTuple[1]);
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Vector2<Real> Vector2<Real>::operator- (const Vector2& rqV) const
{
    return Vector2(
        m_afTuple[0]-rqV.m_afTuple[0],
        m_afTuple[1]-rqV.m_afTuple[1]);
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Vector2<Real> Vector2<Real>::operator* (Real fScalar) const
{
    return Vector2(
        fScalar*m_afTuple[0],
        fScalar*m_afTuple[1]);
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Vector2<Real> Vector2<Real>::operator/ (Real fScalar) const
{
    Vector2 qQuot;

    if ( fScalar != (Real)0.0 )
    {
        Real fInvScalar = ((Real)1.0)/fScalar;
        qQuot.m_afTuple[0] = fInvScalar*m_afTuple[0];
        qQuot.m_afTuple[1] = fInvScalar*m_afTuple[1];
    }
    else
    {
        qQuot.m_afTuple[0] = Math<Real>::MAX_REAL;
        qQuot.m_afTuple[1] = Math<Real>::MAX_REAL;
    }

    return qQuot;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Vector2<Real> Vector2<Real>::operator- () const
{
    return Vector2(
        -m_afTuple[0],
        -m_afTuple[1]);
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Vector2<Real> operator* (Real fScalar, const Vector2<Real>& rqV)
{
    return Vector2<Real>(
        fScalar*rqV[0],
        fScalar*rqV[1]);
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Vector2<Real>& Vector2<Real>::operator+= (const Vector2& rqV)
{
    m_afTuple[0] += rqV.m_afTuple[0];
    m_afTuple[1] += rqV.m_afTuple[1];
    return *this;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Vector2<Real>& Vector2<Real>::operator-= (const Vector2& rqV)
{
    m_afTuple[0] -= rqV.m_afTuple[0];
    m_afTuple[1] -= rqV.m_afTuple[1];
    return *this;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Vector2<Real>& Vector2<Real>::operator*= (Real fScalar)
{
    m_afTuple[0] *= fScalar;
    m_afTuple[1] *= fScalar;
    return *this;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Vector2<Real>& Vector2<Real>::operator/= (Real fScalar)
{
    if ( fScalar != (Real)0.0 )
    {
        Real fInvScalar = ((Real)1.0)/fScalar;
        m_afTuple[0] *= fInvScalar;
        m_afTuple[1] *= fInvScalar;
    }
    else
    {
        m_afTuple[0] = Math<Real>::MAX_REAL;
        m_afTuple[1] = Math<Real>::MAX_REAL;
    }

    return *this;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Real Vector2<Real>::Length () const
{
    return Math<Real>::Sqrt(
        m_afTuple[0]*m_afTuple[0]+
        m_afTuple[1]*m_afTuple[1]);
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Real Vector2<Real>::SquaredLength () const
{
    return
        m_afTuple[0]*m_afTuple[0] +
        m_afTuple[1]*m_afTuple[1];
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Real Vector2<Real>::Dot (const Vector2& rqV) const
{
    return
        m_afTuple[0]*rqV.m_afTuple[0] +
        m_afTuple[1]*rqV.m_afTuple[1];
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Real Vector2<Real>::Normalize ()
{
    Real fLength = Length();

    if ( fLength > Math<Real>::ZERO_TOLERANCE )
    {
        Real fInvLength = ((Real)1.0)/fLength;
        m_afTuple[0] *= fInvLength;
        m_afTuple[1] *= fInvLength;
    }
    else
    {
        fLength = (Real)0.0;
        m_afTuple[0] = (Real)0.0;
        m_afTuple[1] = (Real)0.0;
    }

    return fLength;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Vector2<Real> Vector2<Real>::Perp () const
{
    return Vector2(m_afTuple[1],-m_afTuple[0]);
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Vector2<Real> Vector2<Real>::UnitPerp () const
{
    Vector2 qPerp(m_afTuple[1],-m_afTuple[0]);
    qPerp.Normalize();
    return qPerp;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Real Vector2<Real>::DotPerp (const Vector2& rqV) const
{
    return m_afTuple[0]*rqV.m_afTuple[1] - m_afTuple[1]*rqV.m_afTuple[0];
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
void Vector2<Real>::GetBarycentrics (const Vector2<Real>& rqV0, const Vector2<Real>& rqV1,
    const Vector2<Real>& rqV2, Real afBary[3]) const
{
    // compute the vectors relative to V2 of the triangle
    Vector2<Real> aqDiff[3] =
    {
        rqV0 - rqV2,
        rqV1 - rqV2,
        *this - rqV2
    };

    // If the vertices have large magnitude, the linear system of equations for computing barycentric
    // coordinates can be ill-conditioned.  To avoid this, uniformly scale the triangle edges to be of order 1.
    // The scaling of all differences does not change the barycentric coordinates.
    Real fMax = (Real)0.0;
    int i;
    for (i = 0; i < 2; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            Real fValue = Math<Real>::FAbs(aqDiff[i][j]);
            if ( fValue > fMax )
                fMax = fValue;
        }
    }

    // scale down only large data
    if ( fMax > (Real)1.0 )
    {
        Real fInvMax = ((Real)1.0)/fMax;
        for (i = 0; i < 3; i++)
            aqDiff[i] *= fInvMax;
    }

    Real fDet = aqDiff[0].DotPerp(aqDiff[1]);
    if ( Math<Real>::FAbs(fDet) > Math<Real>::ZERO_TOLERANCE )
    {
        Real fInvDet = ((Real)1.0)/fDet;
        afBary[0] = aqDiff[2].DotPerp(aqDiff[1])*fInvDet;
        afBary[1] = aqDiff[0].DotPerp(aqDiff[2])*fInvDet;
        afBary[2] = (Real)1.0 - afBary[0] - afBary[1];
    }
    else
    {
        // The triangle is a sliver.  Determine the longest edge and compute barycentric coordinates with
        // respect to that edge.
        Vector2<Real> qE2 = rqV0 - rqV1;
        Real fMaxSqrLength = qE2.SquaredLength();
        int iMaxIndex = 2;
        Real fSqrLength = aqDiff[1].SquaredLength();
        if ( fSqrLength > fMaxSqrLength )
        {
            iMaxIndex = 1;
            fMaxSqrLength = fSqrLength;
        }
        fSqrLength = aqDiff[0].SquaredLength();
        if ( fSqrLength > fMaxSqrLength )
        {
            iMaxIndex = 2;
            fMaxSqrLength = fSqrLength;
        }

        if ( fMaxSqrLength > Math<Real>::ZERO_TOLERANCE )
        {
            Real fInvSqrLength = ((Real)1.0)/fMaxSqrLength;
            if ( iMaxIndex == 0 )
            {
                // P-V2 = t(V0-V2)
                afBary[0] = aqDiff[2].Dot(aqDiff[0])*fInvSqrLength;
                afBary[1] = (Real)0.0;
                afBary[2] = (Real)1.0 - afBary[0];
            }
            else if ( iMaxIndex == 1 )
            {
                // P-V2 = t(V1-V2)
                afBary[0] = (Real)0.0;
                afBary[1] = aqDiff[2].Dot(aqDiff[1])*fInvSqrLength;
                afBary[2] = (Real)1.0 - afBary[1];
            }
            else
            {
                // P-V1 = t(V0-V1)
                aqDiff[2] = *this - rqV1;
                afBary[0] = aqDiff[2].Dot(qE2)*fInvSqrLength;
                afBary[1] = (Real)1.0 - afBary[0];
                afBary[2] = (Real)0.0;
            }
        }
        else
        {
            // triangle is a nearly a point, just return equal weights
            afBary[0] = ((Real)1.0)/(Real)3.0;
            afBary[1] = afBary[0];
            afBary[2] = afBary[0];
        }
    }
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
void Vector2<Real>::Orthonormalize (Vector2& rqU, Vector2& rqV)
{
    // If the input vectors are v0 and v1, then the Gram-Schmidt orthonormalization produces vectors u0 and u1
    // as follows,
    //
    //   u0 = v0/|v0|
    //   u1 = (v1-(u0*v1)u0)/|v1-(u0*v1)u0|
    //
    // where |A| indicates length of vector A and A*B indicates dot product of vectors A and B.

    // compute u0
    rqU.Normalize();

    // compute u1
    Real fDot0 = rqU.Dot(rqV);
    rqV -= rqU*fDot0;
    rqV.Normalize();
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
void Vector2<Real>::GenerateOrthonormalBasis (Vector2& rqU, Vector2& rqV, bool bUnitLengthV)
{
    if ( !bUnitLengthV )
        rqV.Normalize();
    rqU = rqV.Perp();
}
//------------------------------------------------------------------------------------------------------------------



