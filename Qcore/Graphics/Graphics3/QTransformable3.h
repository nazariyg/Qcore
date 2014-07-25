#ifndef QTRANSFORMABLE3_H
#define QTRANSFORMABLE3_H

#include "QMatrix3.h"
#include "QMatrix4.h"

namespace Q
{

template <class Real, bool TransposedMatrices>
class Transformable3
{
protected:
    // construction
    Transformable3 ();  // void transformation
    Transformable3 (const Vector3<Real>& rqTranslate, const Matrix3<Real>& rqRotate, const Vector3<Real>& rqScale);

public:
    // member access
    virtual void SetTranslate (const Vector3<Real>& rqTranslate);
    const Vector3<Real>& GetTranslate () const;
    virtual void SetRotate (const Matrix3<Real>& rqRotate);
    const Matrix3<Real>& GetRotate () const;
    virtual void SetScale (const Vector3<Real>& rqScale);
    const Vector3<Real>& GetScale () const;

    // general transformation matrix
    const Matrix4<Real>& GetTransformationMatrix ();

private:
    Vector3<Real> m_qTranslate;
    Matrix3<Real> m_qRotate;
    Vector3<Real> m_qScale;
    bool m_bMatrixNeedsUpdate;
    Matrix4<Real> m_qTransformMatrix;
};

#include "QTransformable3.inl"

}

#endif



