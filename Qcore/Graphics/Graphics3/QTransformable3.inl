//------------------------------------------------------------------------------------------------------------------
template <class Real, bool TransposedMatrices>
Transformable3<Real,TransposedMatrices>::Transformable3 ()
    :
    m_qTranslate((Real)0.0,(Real)0.0,(Real)0.0),
    m_qRotate(
        (Real)1.0,(Real)0.0,(Real)0.0,
        (Real)0.0,(Real)1.0,(Real)0.0,
        (Real)0.0,(Real)0.0,(Real)1.0),
    m_qScale((Real)1.0,(Real)1.0,(Real)1.0),
    m_bMatrixNeedsUpdate(false),
    m_qTransformMatrix(Matrix4<Real>::IDENTITY)
{
}
//------------------------------------------------------------------------------------------------------------------
template <class Real, bool TransposedMatrices>
Transformable3<Real,TransposedMatrices>::Transformable3 (const Vector3<Real>& rqTranslate,
    const Matrix3<Real>& rqRotate, const Vector3<Real>& rqScale)
    :
    m_qTranslate(rqTranslate),
    m_qRotate(rqRotate),
    m_qScale(rqScale),
    m_bMatrixNeedsUpdate(true)
{
}
//------------------------------------------------------------------------------------------------------------------
template <class Real, bool TransposedMatrices>
void Transformable3<Real,TransposedMatrices>::SetTranslate (const Vector3<Real>& rqTranslate)
{
    m_qTranslate = rqTranslate;
    m_bMatrixNeedsUpdate = true;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real, bool TransposedMatrices>
const Vector3<Real>& Transformable3<Real,TransposedMatrices>::GetTranslate () const
{
    return m_qTranslate;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real, bool TransposedMatrices>
void Transformable3<Real,TransposedMatrices>::SetRotate (const Matrix3<Real>& rqRotate)
{
    m_qRotate = rqRotate;
    m_bMatrixNeedsUpdate = true;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real, bool TransposedMatrices>
const Matrix3<Real>& Transformable3<Real,TransposedMatrices>::GetRotate () const
{
    return m_qRotate;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real, bool TransposedMatrices>
void Transformable3<Real,TransposedMatrices>::SetScale (const Vector3<Real>& rqScale)
{
    m_qScale = rqScale;
    m_bMatrixNeedsUpdate = true;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real, bool TransposedMatrices>
const Vector3<Real>& Transformable3<Real,TransposedMatrices>::GetScale () const
{
    return m_qScale;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real, bool TransposedMatrices>
const Matrix4<Real>& Transformable3<Real,TransposedMatrices>::GetTransformationMatrix ()
{
    if ( !m_bMatrixNeedsUpdate )
        return m_qTransformMatrix;

    if ( !TransposedMatrices )
    {
        m_qTransformMatrix = Matrix4<Real>(
            m_qScale.X()*m_qRotate(0,0),m_qRotate(0,1),m_qRotate(0,2),m_qTranslate.X(),
            m_qRotate(1,0),m_qScale.Y()*m_qRotate(1,1),m_qRotate(1,2),m_qTranslate.Y(),
            m_qRotate(2,0),m_qRotate(2,1),m_qScale.Z()*m_qRotate(2,2),m_qTranslate.Z(),
            (Real)0.0,(Real)0.0,(Real)0.0,(Real)1.0);
    }
    else
    {
        m_qTransformMatrix = Matrix4<Real>(
            m_qScale.X()*m_qRotate(0,0),m_qRotate(1,0),m_qRotate(2,0),(Real)0.0,
            m_qRotate(0,1),m_qScale.Y()*m_qRotate(1,1),m_qRotate(2,1),(Real)0.0,
            m_qRotate(0,2),m_qRotate(1,2),m_qScale.Z()*m_qRotate(2,2),(Real)0.0,
            m_qTranslate.X(),m_qTranslate.Y(),m_qTranslate.Z(),(Real)1.0);
    }

    m_bMatrixNeedsUpdate = false;
    return m_qTransformMatrix;
}
//------------------------------------------------------------------------------------------------------------------



