#ifndef QGEOMUTILS3_H
#define QGEOMUTILS3_H

#include "QAxisAlignedBox3.h"
#include "QBox3.h"
#include "QFrustum3.h"
#include "QSphere3.h"

namespace Q
{

template <class Real>
class Q_ITEM GeomUtils3
{
public:
    static AxisAlignedBox3<Real> GetBoundingBoxOf (int iQuantity, const Vector3<Real>* aqPoint);
    static AxisAlignedBox3<Real> GetBoundingBoxOf (const Box3<Real>& rqBox);
    static AxisAlignedBox3<Real> GetBoundingBoxOf (const Frustum3<Real>& rqFrustum);
    static AxisAlignedBox3<Real> GetBoundingBoxOf (const Sphere3<Real>& rqSphere);
};

typedef GeomUtils3<float> GeomUtils3f;
typedef GeomUtils3<double> GeomUtils3d;

}

#endif



