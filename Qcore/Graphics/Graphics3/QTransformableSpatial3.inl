//------------------------------------------------------------------------------------------------------------------
template <class Real, bool TransposedMatrices>
TransformableSpatial3<Real,TransposedMatrices>::TransformableSpatial3 ()
    :
    m_bGlobalBoundNeedsUpdate(true)
{
    m_qLocalBoundingBox.Axis[0] = Vector3<Real>::UNIT_X;
    m_qLocalBoundingBox.Axis[1] = Vector3<Real>::UNIT_Y;
    m_qLocalBoundingBox.Axis[2] = Vector3<Real>::UNIT_Z;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real, bool TransposedMatrices>
const Box3<Real>& TransformableSpatial3<Real,TransposedMatrices>::
GetLocalBoundingBox () const
{
    return m_qLocalBoundingBox;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real, bool TransposedMatrices>
const Box3<Real>& TransformableSpatial3<Real,TransposedMatrices>::
GetGlobalBoundingBox ()
{
    if ( m_bGlobalBoundNeedsUpdate )
    {
        m_qGlobalBoundingBox.Center = m_qLocalBoundingBox.Center + GetTranslate();
        m_qGlobalBoundingBox.Axis[0] = m_qLocalBoundingBox.Axis[0]*GetRotate();
        m_qGlobalBoundingBox.Axis[1] = m_qLocalBoundingBox.Axis[1]*GetRotate();
        m_qGlobalBoundingBox.Axis[2] = m_qLocalBoundingBox.Axis[2]*GetRotate();
        m_qGlobalBoundingBox.Extent[0] = m_qLocalBoundingBox.Extent[0]*GetScale().X();
        m_qGlobalBoundingBox.Extent[1] = m_qLocalBoundingBox.Extent[1]*GetScale().Y();
        m_qGlobalBoundingBox.Extent[2] = m_qLocalBoundingBox.Extent[2]*GetScale().Z();

        m_bGlobalBoundNeedsUpdate = false;
    }
    return m_qGlobalBoundingBox;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real, bool TransposedMatrices>
void TransformableSpatial3<Real,TransposedMatrices>::SetTranslate (const Vector3<Real>& rqTranslate)
{
    Transformable3<Real,TransposedMatrices>::SetTranslate(rqTranslate);
    m_bGlobalBoundNeedsUpdate = true;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real, bool TransposedMatrices>
void TransformableSpatial3<Real,TransposedMatrices>::SetRotate (const Matrix3<Real>& rqRotate)
{
    Transformable3<Real,TransposedMatrices>::SetRotate(rqRotate);
    m_bGlobalBoundNeedsUpdate = true;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real, bool TransposedMatrices>
void TransformableSpatial3<Real,TransposedMatrices>::SetScale (const Vector3<Real>& rqScale)
{
    Transformable3<Real,TransposedMatrices>::SetScale(rqScale);
    m_bGlobalBoundNeedsUpdate = true;
}
//------------------------------------------------------------------------------------------------------------------



