//------------------------------------------------------------------------------------------------------------------
template <class Real>
Plane3<Real>::Plane3 ()
{
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Plane3<Real>::Plane3 (const Vector3<Real>& rqNormal, Real fConstant)
    :
    Normal(rqNormal),
    Constant(fConstant)
{
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Plane3<Real>::Plane3 (const Vector3<Real>& rqNormal, const Vector3<Real>& rqP)
    :
    Normal(rqNormal)
{
    Constant = rqNormal.Dot(rqP);
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Plane3<Real>::Plane3 (const Vector3<Real>& rqP0, const Vector3<Real>& rqP1, const Vector3<Real>& rqP2)
{
    Vector3<Real> qEdge1 = rqP1 - rqP0;
    Vector3<Real> qEdge2 = rqP2 - rqP0;
    Normal = qEdge1.UnitCross(qEdge2);
    Constant = Normal.Dot(rqP0);
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Real Plane3<Real>::DistanceTo (const Vector3<Real>& rqP) const
{
    return Normal.Dot(rqP) - Constant;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
int Plane3<Real>::WhichSide (const Vector3<Real>& rqQ) const
{
    Real fDistance = DistanceTo(rqQ);
    if ( fDistance < (Real)0.0 )
        return -1;
    if ( fDistance > (Real)0.0 )
        return +1;
    return 0;
}
//------------------------------------------------------------------------------------------------------------------



