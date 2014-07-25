#ifndef QTRANSFORMABLENOSCALE3_H
#define QTRANSFORMABLENOSCALE3_H

#include "QMatrix3.h"
#include "QMatrix4.h"

namespace Q
{

template <class Real, bool TransposedMatrices>
class TransformableNoScale3
{
protected:
    // construction
    TransformableNoScale3 ();  // void transformation
    TransformableNoScale3 (const Vector3<Real>& rqTranslate, const Matrix3<Real>& rqRotate);

public:
    // member access
    virtual void SetTranslate (const Vector3<Real>& rqTranslate);
    const Vector3<Real>& GetTranslate () const;
    virtual void SetRotate (const Matrix3<Real>& rqRotate);
    const Matrix3<Real>& GetRotate () const;

    // general transformation matrix
    const Matrix4<Real>& GetTransformationMatrix ();

private:
    Vector3<Real> m_qTranslate;
    Matrix3<Real> m_qRotate;
    bool m_bMatrixNeedsUpdate;
    Matrix4<Real> m_qTransformMatrix;
};

#include "QTransformableNoScale3.inl"

}

#endif



