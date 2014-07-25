//------------------------------------------------------------------------------------------------------------------
template <class Real>
Frustum3<Real>::Frustum3 ()
    :
    Origin(Vector3<Real>::ZERO),
    DVector(-Vector3<Real>::UNIT_Z),
    UVector(Vector3<Real>::UNIT_Y),
    RVector(Vector3<Real>::UNIT_X),
    DMin((Real)1.0),
    DMax((Real)2.0),
    UBound((Real)1.0),
    RBound((Real)1.0)
{
    Update();
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Frustum3<Real>::Frustum3 (const Vector3<Real>& rqOrigin, const Vector3<Real>& rqDVector,
    const Vector3<Real>& rqUVector, const Vector3<Real>& rqRVector, Real fDMin, Real fDMax, Real fUBound,
    Real fRBound)
    :
    Origin(rqOrigin),
    DVector(rqDVector),
    UVector(rqUVector),
    RVector(rqRVector),
    DMin(fDMin),
    DMax(fDMax),
    UBound(fUBound),
    RBound(fRBound)
{
    Update();
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
void Frustum3<Real>::Update ()
{
    m_fDRatio = DMax/DMin;
    m_fMTwoUF = ((Real)-2.0)*UBound*DMax;
    m_fMTwoRF = ((Real)-2.0)*RBound*DMax;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Real Frustum3<Real>::GetDRatio () const
{
    return m_fDRatio;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Real Frustum3<Real>::GetMTwoUF () const
{
    return m_fMTwoUF;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Real Frustum3<Real>::GetMTwoRF () const
{
    return m_fMTwoRF;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
void Frustum3<Real>::ComputeVertices (Vector3<Real> aqVertex[8]) const
{
    Vector3<Real> qDScaled = DMin*DVector;
    Vector3<Real> qUScaled = UBound*UVector;
    Vector3<Real> qRScaled = RBound*RVector;

    aqVertex[0] = qDScaled - qUScaled - qRScaled;
    aqVertex[1] = qDScaled - qUScaled + qRScaled;
    aqVertex[2] = qDScaled + qUScaled + qRScaled;
    aqVertex[3] = qDScaled + qUScaled - qRScaled;

    for (int i = 0, iP = 4; i < 4; i++, iP++)
    {
        aqVertex[iP] = Origin + m_fDRatio*aqVertex[i];
        aqVertex[i] += Origin;
    }
}
//------------------------------------------------------------------------------------------------------------------



