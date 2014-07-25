#ifndef QBOUND2I_H
#define QBOUND2I_H

#include "QSystem.h"

namespace Q
{

class Q_ITEM Bound2i
{
public:
    // construction
    Bound2i ();  // uninitialized
    Bound2i (
        int iMinX, int iMaxX,
        int iMinY, int iMaxY);

    int MinX, MaxX, MinY, MaxY;
};

}

#endif



