//------------------------------------------------------------------------------------------------------------------
template <class Real>
ProjectionState<Real>::ProjectionState (const Matrix4<Real>& rqProjMatrix)
    :
    ProjectionMatrix(rqProjMatrix)
{
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
ProjectionState<Real>::ProjectionState (ProjectionType eProjection, Real fLeft, Real fRight, Real fBottom,
    Real fTop, Real fNear, Real fFar)
{
    if ( eProjection == PT_PERSPECTIVE )
        ProjectionMatrix = GrphUtils3<Real>::GetPerspProjMatrix(fLeft,fRight,fBottom,fTop).GetTransposed();
    else
        ProjectionMatrix = GrphUtils3<Real>::GetOrthoProjMatrix(fLeft,fRight,fBottom,fTop).GetTransposed();
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
ProjectionState<Real>::ProjectionState (ProjectionType eProjection, Real fParam0, Real fParam1, Real fParam2,
    Real fParam3)
{
    if ( eProjection == PT_PERSPECTIVE )
        ProjectionMatrix = GrphUtils3<Real>::GetPerspProjMatrix(fParam0,fParam1,fParam2,fParam3).GetTransposed();
    else
        ProjectionMatrix = GrphUtils3<Real>::GetOrthoProjMatrix(fParam0,fParam1,fParam2,fParam3).GetTransposed();
}
//------------------------------------------------------------------------------------------------------------------



