#ifndef QSPHERE3_H
#define QSPHERE3_H

// The sphere is represented as |X-C| = R where C is the center and R is the radius.

#include "QVector3.h"

namespace Q
{

template <class Real>
class Sphere3
{
public:
    // construction
    Sphere3 ();  // uninitialized
    Sphere3 (const Vector3<Real>& rqCenter, Real fRadius);

    Vector3<Real> Center;
    Real Radius;
};

#include "QSphere3.inl"

typedef Sphere3<float> Sphere3f;
typedef Sphere3<double> Sphere3d;

}

#endif



