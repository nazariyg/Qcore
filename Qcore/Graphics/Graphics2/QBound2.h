#ifndef QBOUND2_H
#define QBOUND2_H

namespace Q
{

template <class Real>
class Bound2
{
public:
    // construction
    Bound2 ();  // uninitialized
    Bound2 (
        Real fMinX, Real fMaxX,
        Real fMinY, Real fMaxY);

    Real MinX, MaxX, MinY, MaxY;
};

#include "QBound2.inl"

typedef Bound2<float> Bound2f;
typedef Bound2<double> Bound2d;

}

#endif



