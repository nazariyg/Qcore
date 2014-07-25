#ifndef QPLANE3_H
#define QPLANE3_H

// The plane is represented as Dot(N,X) = c where N is a unit-length normal vector, c is the plane constant,
// and X is any point on the plane.  The user must ensure that the normal vector satisfies this condition.

#include "QVector3.h"

namespace Q
{

template <class Real>
class Plane3
{
public:
    // construction
    Plane3 ();  // uninitialized

    // specify N and c directly
    Plane3 (const Vector3<Real>& rqNormal, Real fConstant);

    // N is specified, c = Dot(N,P) where P is on the plane
    Plane3 (const Vector3<Real>& rqNormal, const Vector3<Real>& rqP);

    // N = Cross(P1-P0,P2-P0)/Length(Cross(P1-P0,P2-P0)), c = Dot(N,P0) where P0, P1, P2 are points on the
    // plane.
    Plane3 (const Vector3<Real>& rqP0, const Vector3<Real>& rqP1, const Vector3<Real>& rqP2);

    // The "positive side" of the plane is the half space to which the plane normal points.  The "negative
    // side" is the other half space.  The function returns +1 for the positive side, -1 for the negative side,
    // and 0 for the point being on the plane.
    int WhichSide (const Vector3<Real>& rqP) const;

    // Compute d = Dot(N,Q)-c where N is the plane normal and c is the plane constant.  This is a signed
    // distance.  The sign of the return value is positive if the point is on the positive side of the plane,
    // negative if the point is on the negative side, and zero if the point is on the plane.
    Real DistanceTo (const Vector3<Real>& rqQ) const;

    Vector3<Real> Normal;
    Real Constant;
};

#include "QPlane3.inl"

typedef Plane3<float> Plane3f;
typedef Plane3<double> Plane3d;

}

#endif



