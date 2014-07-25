//------------------------------------------------------------------------------------------------------------------
template <class Real>
HRCamera3<Real>::HRCamera3 (const Vector3<Real>& rqTranslate, const Matrix3<Real>& rqRotate, Real fUpFovDeg,
    Real fWOnHAspectRatio, Real fNear, Real fFar)
    :
    TransformableNoScale3<Real,true>(rqTranslate,rqRotate)
{
    SetFrustumParams(fUpFovDeg,fWOnHAspectRatio,fNear,fFar);
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
void HRCamera3<Real>::SetTranslate (const Vector3<Real>& rqTranslate)
{
    TransformableNoScale3<Real,true>::SetTranslate(rqTranslate);

    m_qFrustum.Origin = rqTranslate;

    // frustum's axis-aligned bounding box
    m_qFrustumAABoundingBox = GeomUtils3<Real>::GetBoundingBoxOf(m_qFrustum);
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
void HRCamera3<Real>::SetRotate (const Matrix3<Real>& rqRotate)
{
    TransformableNoScale3<Real,true>::SetRotate(rqRotate);

    m_qFrustum.DVector = rqRotate*(-Vector3<Real>::UNIT_Z);
    m_qFrustum.UVector = rqRotate*Vector3<Real>::UNIT_Y;
    m_qFrustum.RVector = rqRotate*Vector3<Real>::UNIT_X;

    // frustum's axis-aligned bounding box
    m_qFrustumAABoundingBox = GeomUtils3<Real>::GetBoundingBoxOf(m_qFrustum);
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
void HRCamera3<Real>::SetFrustumParams (Real fUpFovDeg, Real fWOnHAspectRatio, Real fNear, Real fFar)
{
    // projection
    m_qProjectionMatrix = GrphUtils3<Real>::GetPerspProjMatrix(
        fUpFovDeg,fWOnHAspectRatio,fNear,fFar).GetTransposed();

    // frustum
    m_qFrustum.DMin = fNear;
    m_qFrustum.DMax = fFar;
    m_qFrustum.UBound = fNear*Math<Real>::Tan(((Real)0.5)*fUpFovDeg*Math<Real>::DEG_TO_RAD);
    m_qFrustum.RBound = fWOnHAspectRatio*m_qFrustum.UBound;
    m_qFrustum.Update();

    // frustum's axis-aligned bounding box
    m_qFrustumAABoundingBox = GeomUtils3<Real>::GetBoundingBoxOf(m_qFrustum);
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
const Matrix4<Real>& HRCamera3<Real>::GetProjectionMatrix () const
{
    return m_qProjectionMatrix;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
const Frustum3<Real>& HRCamera3<Real>::GetFrustum () const
{
    return m_qFrustum;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
const AxisAlignedBox3<Real>& HRCamera3<Real>::GetFrustumAABoundingBox () const
{
    return m_qFrustumAABoundingBox;
}
//------------------------------------------------------------------------------------------------------------------



