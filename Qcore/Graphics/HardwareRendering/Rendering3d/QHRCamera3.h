#ifndef QHRCAMERA3_H
#define QHRCAMERA3_H

#include "QTransformableNoScale3.h"
#include "QHeap.h"
#include "QGrphUtils3.h"
#include "QGeomUtils3.h"

namespace Q
{

template <class Real>
class HRCamera3 : public TransformableNoScale3<Real,true>, public Heap
{
public:
    // construction
    HRCamera3 (const Vector3<Real>& rqTranslate, const Matrix3<Real>& rqRotate, Real fUpFovDeg,
        Real fWOnHAspectRatio, Real fNear, Real fFar);

    virtual void SetTranslate (const Vector3<Real>& rqTranslate);
    virtual void SetRotate (const Matrix3<Real>& rqRotate);
    void SetFrustumParams (Real fUpFovDeg, Real fWOnHAspectRatio, Real fNear, Real fFar);

    const Matrix4<Real>& GetProjectionMatrix () const;
    const Frustum3<Real>& GetFrustum () const;
    const AxisAlignedBox3<Real>& GetFrustumAABoundingBox () const;

private:
    Matrix4<Real> m_qProjectionMatrix;
    Frustum3<Real> m_qFrustum;
    AxisAlignedBox3<Real> m_qFrustumAABoundingBox;
};

#include "QHRCamera3.inl"

typedef HRCamera3<float> HRCamera3f;
typedef HRCamera3<double> HRCamera3d;

typedef SmartPointer<HRCamera3f> HRCamera3fPtr;
typedef SmartPointer<HRCamera3d> HRCamera3dPtr;

}

#endif



