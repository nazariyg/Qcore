#ifndef QBOUND3_H
#define QBOUND3_H

namespace Q
{

template <class Real>
class Bound3
{
public:
    // construction
    Bound3 ();  // uninitialized
    Bound3 (
        Real fMinX, Real fMaxX,
        Real fMinY, Real fMaxY,
        Real fMinZ, Real fMaxZ);

    // logical union
    void Merge (const Bound3& rqBound);

    Real MinX, MaxX, MinY, MaxY, MinZ, MaxZ;
};

#include "QBound3.inl"

typedef Bound3<float> Bound3f;
typedef Bound3<double> Bound3d;

}

#endif



