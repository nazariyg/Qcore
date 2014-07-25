#ifndef QLINE3_H
#define QLINE3_H

#include "QVector3.h"

namespace Q
{

template <class Real>
class Line3
{
public:
    // The line is represented as P+t*D where P is the line origin and D is a unit-length direction vector.
    // The user must ensure that the direction vector satisfies this condition.

    // construction
    Line3 ();  // uninitialized
    Line3 (const Vector3<Real>& rqOrigin, const Vector3<Real>& rqDirection);

    Vector3<Real> Origin, Direction;
};

#include "QLine3.inl"

typedef Line3<float> Line3f;
typedef Line3<double> Line3d;

}

#endif



