#ifndef QTRANSFORMABLESPATIAL3_H
#define QTRANSFORMABLESPATIAL3_H

#include "QTransformable3.h"
#include "QBox3.h"

namespace Q
{

template <class Real, bool TransposedMatrices>
class TransformableSpatial3 : public Transformable3<Real,TransposedMatrices>
{
protected:
    // construction
    TransformableSpatial3 ();

public:
    // member access
    const Box3<Real>& GetLocalBoundingBox () const;
    const Box3<Real>& GetGlobalBoundingBox ();

    virtual void UpdateLocalBound () = 0;

    virtual void SetTranslate (const Vector3<Real>& rqTranslate);
    virtual void SetRotate (const Matrix3<Real>& rqRotate);
    virtual void SetScale (const Vector3<Real>& rqScale);

protected:
    Box3<Real> m_qLocalBoundingBox;

private:
    bool m_bGlobalBoundNeedsUpdate;  // default: true
    Box3<Real> m_qGlobalBoundingBox;
};

#include "QTransformableSpatial3.inl"

}

#endif



