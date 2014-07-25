#ifndef QAXISALIGNEDBOX3_H
#define QAXISALIGNEDBOX3_H

#include "QVector3.h"

namespace Q
{

template <class Real>
class AxisAlignedBox3
{
public:
    // construction
    AxisAlignedBox3 ();  // uninitialized
    AxisAlignedBox3 (
        Real fMinX, Real fMaxX,
        Real fMinY, Real fMaxY,
        Real fMinZ, Real fMaxZ);

    // the containment and intersection queries
    bool Contains (const Vector3<Real>& rqPoint) const;
    bool TestIntersection (const AxisAlignedBox3<Real>& rqBox) const;

    // the same but with tolerance: a positive fTolerance value may make 'true' returned even if the objects
    // aren't contacting in reality
    bool Contains (const Vector3<Real>& rqPoint, Real fTolerance) const;
    bool TestIntersection (const AxisAlignedBox3<Real>& rqBox, Real fTolerance) const;

    // The return value is 'true' if there is overlap; in this case the intersection is stored in rqIntr.  The
    // return value is 'false', if there is no overlap; in this case rqIntr is undefined.
    bool FindIntersection (const AxisAlignedBox3& rqBox, AxisAlignedBox3& rqIntr) const;

    // merging of two boxes
    static void MergeBounds (const AxisAlignedBox3<Real>& rqBox0, const AxisAlignedBox3<Real>& rqBox1,
        AxisAlignedBox3& rqMrg);

    Real Min[3], Max[3];
};

#include "QAxisAlignedBox3.inl"

typedef AxisAlignedBox3<float> AxisAlignedBox3f;
typedef AxisAlignedBox3<double> AxisAlignedBox3d;

}

#endif



