//------------------------------------------------------------------------------------------------------------------
template <class Real>
Line3<Real>::Line3 ()
{
    // uninitialized
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Line3<Real>::Line3 (const Vector3<Real>& rqOrigin, const Vector3<Real>& rqDirection)
    :
    Origin(rqOrigin),
    Direction(rqDirection)
{
}
//------------------------------------------------------------------------------------------------------------------



