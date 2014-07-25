//------------------------------------------------------------------------------------------------------------------
template <class Real>
Vector3<Real>::Vector3 ()
{
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Vector3<Real>::Vector3 (Real fX, Real fY, Real fZ)
{
    m_afTuple[0] = fX;
    m_afTuple[1] = fY;
    m_afTuple[2] = fZ;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Vector3<Real>::Vector3 (const Vector3& rqV)
{
    m_afTuple[0] = rqV.m_afTuple[0];
    m_afTuple[1] = rqV.m_afTuple[1];
    m_afTuple[2] = rqV.m_afTuple[2];
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Vector3<Real>::operator Real* ()
{
    return m_afTuple;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Vector3<Real>::operator const Real* () const
{
    return m_afTuple;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Real Vector3<Real>::operator[] (int i) const
{
    assert( 0 <= i && i <= 2 );
    return m_afTuple[i];
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Real& Vector3<Real>::operator[] (int i)
{
    assert( 0 <= i && i <= 2 );
    return m_afTuple[i];
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Real Vector3<Real>::X () const
{
    return m_afTuple[0];
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Real& Vector3<Real>::X ()
{
    return m_afTuple[0];
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Real Vector3<Real>::Y () const
{
    return m_afTuple[1];
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Real& Vector3<Real>::Y ()
{
    return m_afTuple[1];
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Real Vector3<Real>::Z () const
{
    return m_afTuple[2];
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Real& Vector3<Real>::Z ()
{
    return m_afTuple[2];
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Vector3<Real>& Vector3<Real>::operator= (const Vector3& rqV)
{
    m_afTuple[0] = rqV.m_afTuple[0];
    m_afTuple[1] = rqV.m_afTuple[1];
    m_afTuple[2] = rqV.m_afTuple[2];
    return *this;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
bool Vector3<Real>::operator== (const Vector3& rqV) const
{
    return ( MemCmp(m_afTuple,rqV.m_afTuple,3*sizeof(Real)) == 0 );
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
bool Vector3<Real>::operator!= (const Vector3& rqV) const
{
    return ( MemCmp(m_afTuple,rqV.m_afTuple,3*sizeof(Real)) != 0 );
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
bool Vector3<Real>::operator< (const Vector3& rqV) const
{
    return ( MemCmp(m_afTuple,rqV.m_afTuple,3*sizeof(Real)) < 0 );
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
bool Vector3<Real>::operator<= (const Vector3& rqV) const
{
    return ( MemCmp(m_afTuple,rqV.m_afTuple,3*sizeof(Real)) <= 0 );
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
bool Vector3<Real>::operator> (const Vector3& rqV) const
{
    return ( MemCmp(m_afTuple,rqV.m_afTuple,3*sizeof(Real)) > 0 );
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
bool Vector3<Real>::operator>= (const Vector3& rqV) const
{
    return ( MemCmp(m_afTuple,rqV.m_afTuple,3*sizeof(Real)) >= 0 );
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Vector3<Real> Vector3<Real>::operator+ (const Vector3& rqV) const
{
    return Vector3(
        m_afTuple[0]+rqV.m_afTuple[0],
        m_afTuple[1]+rqV.m_afTuple[1],
        m_afTuple[2]+rqV.m_afTuple[2]);
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Vector3<Real> Vector3<Real>::operator- (const Vector3& rqV) const
{
    return Vector3(
        m_afTuple[0]-rqV.m_afTuple[0],
        m_afTuple[1]-rqV.m_afTuple[1],
        m_afTuple[2]-rqV.m_afTuple[2]);
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Vector3<Real> Vector3<Real>::operator* (Real fScalar) const
{
    return Vector3(
        fScalar*m_afTuple[0],
        fScalar*m_afTuple[1],
        fScalar*m_afTuple[2]);
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Vector3<Real> Vector3<Real>::operator/ (Real fScalar) const
{
    Vector3 qQuot;

    if ( fScalar != (Real)0.0 )
    {
        Real fInvScalar = ((Real)1.0)/fScalar;
        qQuot.m_afTuple[0] = fInvScalar*m_afTuple[0];
        qQuot.m_afTuple[1] = fInvScalar*m_afTuple[1];
        qQuot.m_afTuple[2] = fInvScalar*m_afTuple[2];
    }
    else
    {
        qQuot.m_afTuple[0] = Math<Real>::MAX_REAL;
        qQuot.m_afTuple[1] = Math<Real>::MAX_REAL;
        qQuot.m_afTuple[2] = Math<Real>::MAX_REAL;
    }

    return qQuot;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Vector3<Real> Vector3<Real>::operator- () const
{
    return Vector3(
        -m_afTuple[0],
        -m_afTuple[1],
        -m_afTuple[2]);
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Vector3<Real> operator* (Real fScalar, const Vector3<Real>& rqV)
{
    return Vector3<Real>(
        fScalar*rqV[0],
        fScalar*rqV[1],
        fScalar*rqV[2]);
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Vector3<Real>& Vector3<Real>::operator+= (const Vector3& rqV)
{
    m_afTuple[0] += rqV.m_afTuple[0];
    m_afTuple[1] += rqV.m_afTuple[1];
    m_afTuple[2] += rqV.m_afTuple[2];
    return *this;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Vector3<Real>& Vector3<Real>::operator-= (const Vector3& rqV)
{
    m_afTuple[0] -= rqV.m_afTuple[0];
    m_afTuple[1] -= rqV.m_afTuple[1];
    m_afTuple[2] -= rqV.m_afTuple[2];
    return *this;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Vector3<Real>& Vector3<Real>::operator*= (Real fScalar)
{
    m_afTuple[0] *= fScalar;
    m_afTuple[1] *= fScalar;
    m_afTuple[2] *= fScalar;
    return *this;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Vector3<Real>& Vector3<Real>::operator/= (Real fScalar)
{
    if ( fScalar != (Real)0.0 )
    {
        Real fInvScalar = ((Real)1.0)/fScalar;
        m_afTuple[0] *= fInvScalar;
        m_afTuple[1] *= fInvScalar;
        m_afTuple[2] *= fInvScalar;
    }
    else
    {
        m_afTuple[0] = Math<Real>::MAX_REAL;
        m_afTuple[1] = Math<Real>::MAX_REAL;
        m_afTuple[2] = Math<Real>::MAX_REAL;
    }

    return *this;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Real Vector3<Real>::Length () const
{
    return Math<Real>::Sqrt(
        m_afTuple[0]*m_afTuple[0]+
        m_afTuple[1]*m_afTuple[1]+
        m_afTuple[2]*m_afTuple[2]);
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Real Vector3<Real>::SquaredLength () const
{
    return
        m_afTuple[0]*m_afTuple[0] +
        m_afTuple[1]*m_afTuple[1] +
        m_afTuple[2]*m_afTuple[2];
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Real Vector3<Real>::Dot (const Vector3& rqV) const
{
    return
        m_afTuple[0]*rqV.m_afTuple[0] +
        m_afTuple[1]*rqV.m_afTuple[1] +
        m_afTuple[2]*rqV.m_afTuple[2];
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Real Vector3<Real>::Normalize ()
{
    Real fLength = Length();

    if ( fLength > Math<Real>::ZERO_TOLERANCE )
    {
        Real fInvLength = ((Real)1.0)/fLength;
        m_afTuple[0] *= fInvLength;
        m_afTuple[1] *= fInvLength;
        m_afTuple[2] *= fInvLength;
    }
    else
    {
        fLength = (Real)0.0;
        m_afTuple[0] = (Real)0.0;
        m_afTuple[1] = (Real)0.0;
        m_afTuple[2] = (Real)0.0;
    }

    return fLength;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Vector3<Real> Vector3<Real>::Cross (const Vector3& rqV) const
{
    return Vector3(
        m_afTuple[1]*rqV.m_afTuple[2]-m_afTuple[2]*rqV.m_afTuple[1],
        m_afTuple[2]*rqV.m_afTuple[0]-m_afTuple[0]*rqV.m_afTuple[2],
        m_afTuple[0]*rqV.m_afTuple[1]-m_afTuple[1]*rqV.m_afTuple[0]);
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Vector3<Real> Vector3<Real>::UnitCross (const Vector3& rqV) const
{
    Vector3 qCross(
        m_afTuple[1]*rqV.m_afTuple[2]-m_afTuple[2]*rqV.m_afTuple[1],
        m_afTuple[2]*rqV.m_afTuple[0]-m_afTuple[0]*rqV.m_afTuple[2],
        m_afTuple[0]*rqV.m_afTuple[1]-m_afTuple[1]*rqV.m_afTuple[0]);
    qCross.Normalize();
    return qCross;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
void Vector3<Real>::GetBarycentrics (const Vector3<Real>& rqV0, const Vector3<Real>& rqV1,
    const Vector3<Real>& rqV2, const Vector3<Real>& rqV3, Real afBary[4]) const
{
    // compute the vectors relative to V3 of the tetrahedron
    Vector3<Real> aqDiff[4] =
    {
        rqV0 - rqV3,
        rqV1 - rqV3,
        rqV2 - rqV3,
        *this - rqV3
    };

    // If the vertices have large magnitude, the linear system of equations for computing barycentric
    // coordinates can be ill-conditioned.  To avoid this, uniformly scale the tetrahedron edges to be of order
    // 1.  The scaling of all differences does not change the barycentric coordinates.
    Real fMax = (Real)0.0;
    int i;
    for (i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
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
        for (i = 0; i < 4; i++)
            aqDiff[i] *= fInvMax;
    }

    Real fDet = aqDiff[0].Dot(aqDiff[1].Cross(aqDiff[2]));
    Vector3<Real> qE1cE2 = aqDiff[1].Cross(aqDiff[2]);
    Vector3<Real> qE2cE0 = aqDiff[2].Cross(aqDiff[0]);
    Vector3<Real> qE0cE1 = aqDiff[0].Cross(aqDiff[1]);
    if ( Math<Real>::FAbs(fDet) > Math<Real>::ZERO_TOLERANCE )
    {
        Real fInvDet = ((Real)1.0)/fDet;
        afBary[0] = aqDiff[3].Dot(qE1cE2)*fInvDet;
        afBary[1] = aqDiff[3].Dot(qE2cE0)*fInvDet;
        afBary[2] = aqDiff[3].Dot(qE0cE1)*fInvDet;
        afBary[3] = (Real)1.0 - afBary[0] - afBary[1] - afBary[2];
    }
    else
    {
        // The tetrahedron is potentially flat.  Determine the face of maximum area and compute barycentric
        // coordinates with respect to that face.
        Vector3<Real> qE02 = rqV0 - rqV2;
        Vector3<Real> qE12 = rqV1 - rqV2;
        Vector3<Real> qE02cE12 = qE02.Cross(qE12);
        Real fMaxSqrArea = qE02cE12.SquaredLength();
        int iMaxIndex = 3;
        Real fSqrArea = qE0cE1.SquaredLength();
        if ( fSqrArea > fMaxSqrArea )
        {
            iMaxIndex = 0;
            fMaxSqrArea = fSqrArea;
        }
        fSqrArea = qE1cE2.SquaredLength();
        if ( fSqrArea > fMaxSqrArea )
        {
            iMaxIndex = 1;
            fMaxSqrArea = fSqrArea;
        }
        fSqrArea = qE2cE0.SquaredLength();
        if ( fSqrArea > fMaxSqrArea )
        {
            iMaxIndex = 2;
            fMaxSqrArea = fSqrArea;
        }

        if ( fMaxSqrArea > Math<Real>::ZERO_TOLERANCE )
        {
            Real fInvSqrArea = ((Real)1.0)/fMaxSqrArea;
            Vector3<Real> qTmp;
            if ( iMaxIndex == 0 )
            {
                qTmp = aqDiff[3].Cross(aqDiff[1]);
                afBary[0] = qE0cE1.Dot(qTmp)*fInvSqrArea;
                qTmp = aqDiff[0].Cross(aqDiff[3]);
                afBary[1] = qE0cE1.Dot(qTmp)*fInvSqrArea;
                afBary[2] = (Real)0.0;
                afBary[3] = (Real)1.0 - afBary[0] - afBary[1];
            }
            else if ( iMaxIndex == 1 )
            {
                afBary[0] = (Real)0.0;
                qTmp = aqDiff[3].Cross(aqDiff[2]);
                afBary[1] = qE1cE2.Dot(qTmp)*fInvSqrArea;
                qTmp = aqDiff[1].Cross(aqDiff[3]);
                afBary[2] = qE1cE2.Dot(qTmp)*fInvSqrArea;
                afBary[3] = (Real)1.0 - afBary[1] - afBary[2];
            }
            else if ( iMaxIndex == 2 )
            {
                qTmp = aqDiff[2].Cross(aqDiff[3]);
                afBary[0] = qE2cE0.Dot(qTmp)*fInvSqrArea;
                afBary[1] = (Real)0.0;
                qTmp = aqDiff[3].Cross(aqDiff[0]);
                afBary[2] = qE2cE0.Dot(qTmp)*fInvSqrArea;
                afBary[3] = (Real)1.0 - afBary[0] - afBary[2];
            }
            else
            {
                aqDiff[3] = *this - rqV2;
                qTmp = aqDiff[3].Cross(qE12);
                afBary[0] = qE02cE12.Dot(qTmp)*fInvSqrArea;
                qTmp = qE02.Cross(aqDiff[3]);
                afBary[1] = qE02cE12.Dot(qTmp)*fInvSqrArea;
                afBary[2] = (Real)1.0 - afBary[0] - afBary[1];
                afBary[3] = (Real)0.0;
            }
        }
        else
        {
            // The tetrahedron is potentially a sliver.  Determine the edge of maximum length and compute
            // barycentric coordinates with respect to that edge.
            Real fMaxSqrLength = aqDiff[0].SquaredLength();
            iMaxIndex = 0;  // <V0,V3>
            Real fSqrLength = aqDiff[1].SquaredLength();
            if ( fSqrLength > fMaxSqrLength )
            {
                iMaxIndex = 1;  // <V1,V3>
                fMaxSqrLength = fSqrLength;
            }
            fSqrLength = aqDiff[2].SquaredLength();
            if ( fSqrLength > fMaxSqrLength )
            {
                iMaxIndex = 2;  // <V2,V3>
                fMaxSqrLength = fSqrLength;
            }
            fSqrLength = qE02.SquaredLength();
            if ( fSqrLength > fMaxSqrLength )
            {
                iMaxIndex = 3;  // <V0,V2>
                fMaxSqrLength = fSqrLength;
            }
            fSqrLength = qE12.SquaredLength();
            if ( fSqrLength > fMaxSqrLength )
            {
                iMaxIndex = 4;  // <V1,V2>
                fMaxSqrLength = fSqrLength;
            }
            Vector3<Real> qE01 = rqV0 - rqV1;
            fSqrLength = qE01.SquaredLength();
            if ( fSqrLength > fMaxSqrLength )
            {
                iMaxIndex = 5;  // <V0,V1>
                fMaxSqrLength = fSqrLength;
            }

            if ( fMaxSqrLength > Math<Real>::ZERO_TOLERANCE )
            {
                Real fInvSqrLength = ((Real)1.0)/fMaxSqrLength;
                if ( iMaxIndex == 0 )
                {
                    // P-V3 = t*(V0-V3)
                    afBary[0] = aqDiff[3].Dot(aqDiff[0])*fInvSqrLength;
                    afBary[1] = (Real)0.0;
                    afBary[2] = (Real)0.0;
                    afBary[3] = (Real)1.0 - afBary[0];
                }
                else if ( iMaxIndex == 1 )
                {
                    // P-V3 = t*(V1-V3)
                    afBary[0] = (Real)0.0;
                    afBary[1] = aqDiff[3].Dot(aqDiff[1])*fInvSqrLength;
                    afBary[2] = (Real)0.0;
                    afBary[3] = (Real)1.0 - afBary[1];
                }
                else if ( iMaxIndex == 2 )
                {
                    // P-V3 = t*(V2-V3)
                    afBary[0] = (Real)0.0;
                    afBary[1] = (Real)0.0;
                    afBary[2] = aqDiff[3].Dot(aqDiff[2])*fInvSqrLength;
                    afBary[3] = (Real)1.0 - afBary[2];
                }
                else if ( iMaxIndex == 3 )
                {
                    // P-V2 = t*(V0-V2)
                    aqDiff[3] = *this - rqV2;
                    afBary[0] = aqDiff[3].Dot(qE02)*fInvSqrLength;
                    afBary[1] = (Real)0.0;
                    afBary[2] = (Real)1.0 - afBary[0];
                    afBary[3] = (Real)0.0;
                }
                else if ( iMaxIndex == 4 )
                {
                    // P-V2 = t*(V1-V2)
                    aqDiff[3] = *this - rqV2;
                    afBary[0] = (Real)0.0;
                    afBary[1] = aqDiff[3].Dot(qE12)*fInvSqrLength;
                    afBary[2] = (Real)1.0 - afBary[1];
                    afBary[3] = (Real)0.0;
                }
                else
                {
                    // P-V1 = t*(V0-V1)
                    aqDiff[3] = *this - rqV1;
                    afBary[0] = aqDiff[3].Dot(qE01)*fInvSqrLength;
                    afBary[1] = (Real)1.0 - afBary[0];
                    afBary[2] = (Real)0.0;
                    afBary[3] = (Real)0.0;
                }
            }
            else
            {
                // tetrahedron is a nearly a point, just return equal weights
                afBary[0] = (Real)0.25;
                afBary[1] = afBary[0];
                afBary[2] = afBary[0];
                afBary[3] = afBary[0];
            }
        }
    }
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
void Vector3<Real>::Orthonormalize (Vector3& rqU, Vector3& rqV, Vector3& rqW)
{
    // If the input vectors are v0, v1, and v2, then the Gram-Schmidt orthonormalization produces vectors u0,
    // u1, and u2 as follows,
    //
    //   u0 = v0/|v0|
    //   u1 = (v1-(u0*v1)u0)/|v1-(u0*v1)u0|
    //   u2 = (v2-(u0*v2)u0-(u1*v2)u1)/|v2-(u0*v2)u0-(u1*v2)u1|
    //
    // where |A| indicates length of vector A and A*B indicates dot product of vectors A and B.

    // compute u0
    rqU.Normalize();

    // compute u1
    Real fDot0 = rqU.Dot(rqV);
    rqV -= fDot0*rqU;
    rqV.Normalize();

    // compute u2
    Real fDot1 = rqV.Dot(rqW);
    fDot0 = rqU.Dot(rqW);
    rqW -= fDot0*rqU + fDot1*rqV;
    rqW.Normalize();
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
void Vector3<Real>::Orthonormalize (Vector3* aqV)
{
    Orthonormalize(aqV[0],aqV[1],aqV[2]);
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
void Vector3<Real>::GenerateOrthonormalBasis (Vector3& rqU, Vector3& rqV, Vector3& rqW, bool bUnitLengthW)
{
    if ( !bUnitLengthW )
        rqW.Normalize();

    Real fInvLength;

    if ( Math<Real>::FAbs(rqW.m_afTuple[0]) >=
         Math<Real>::FAbs(rqW.m_afTuple[1]) )
    {
        // W.x or W.z is the largest magnitude component, swap them
        fInvLength = Math<Real>::InvSqrt(rqW.m_afTuple[0]*rqW.m_afTuple[0]+rqW.m_afTuple[2]*rqW.m_afTuple[2]);
        rqU.m_afTuple[0] = -rqW.m_afTuple[2]*fInvLength;
        rqU.m_afTuple[1] = (Real)0.0;
        rqU.m_afTuple[2] = +rqW.m_afTuple[0]*fInvLength;
        rqV.m_afTuple[0] = rqW.m_afTuple[1]*rqU.m_afTuple[2];
        rqV.m_afTuple[1] = rqW.m_afTuple[2]*rqU.m_afTuple[0] - rqW.m_afTuple[0]*rqU.m_afTuple[2];
        rqV.m_afTuple[2] = -rqW.m_afTuple[1]*rqU.m_afTuple[0];
    }
    else
    {
        // W.y or W.z is the largest magnitude component, swap them
        fInvLength = Math<Real>::InvSqrt(rqW.m_afTuple[1]*rqW.m_afTuple[1]+rqW.m_afTuple[2]*rqW.m_afTuple[2]);
        rqU.m_afTuple[0] = (Real)0.0;
        rqU.m_afTuple[1] = +rqW.m_afTuple[2]*fInvLength;
        rqU.m_afTuple[2] = -rqW.m_afTuple[1]*fInvLength;
        rqV.m_afTuple[0] = rqW.m_afTuple[1]*rqU.m_afTuple[2] - rqW.m_afTuple[2]*rqU.m_afTuple[1];
        rqV.m_afTuple[1] = -rqW.m_afTuple[0]*rqU.m_afTuple[2];
        rqV.m_afTuple[2] = rqW.m_afTuple[0]*rqU.m_afTuple[1];
    }
}
//------------------------------------------------------------------------------------------------------------------



