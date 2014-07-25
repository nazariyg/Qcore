#ifndef QBOX3_H
#define QBOX3_H

// A box has center C, axis directions U[0], U[1], and U[2] (all unit-length vectors), and extents e[0], e[1],
// and e[2] (all nonnegative numbers).  A point X = C+y[0]*U[0]+y[1]*U[1]+y[2]*U[2] is inside or on the box
// whenever |y[i]| <= e[i] for all i.

#include "QVector3.h"

namespace Q
{

template <class Real>
class Box3
{
public:
    // construction
    Box3 ();  // uninitialized
    Box3 (const Vector3<Real>& rqCenter, const Vector3<Real>* aqAxis, const Real* afExtent);
    Box3 (const Vector3<Real>& rqCenter, const Vector3<Real>& rqAxis0, const Vector3<Real>& rqAxis1,
        const Vector3<Real>& rqAxis2, Real fExtent0, Real fExtent1, Real fExtent2);

    void ComputeVertices (Vector3<Real> aqVertex[8]) const;

    Vector3<Real> Center;
    Vector3<Real> Axis[3];  // must be an orthonormal set of vectors
    Real Extent[3];         // must be nonnegative
};

#include "QBox3.inl"

typedef Box3<float> Box3f;
typedef Box3<double> Box3d;

}

#endif



