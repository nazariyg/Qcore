#ifndef QRECTI_H
#define QRECTI_H

#include "QSystem.h"

namespace Q
{

class Q_ITEM Recti
{
public:
    // construction
    Recti ();  // uninitialized
    Recti (int iX, int iY, int iWidth, int iHeight);

    // operations
    bool Contains (int iX, int iY) const;
    Recti GetMerged (const Recti& rqRect) const;  // logical addition

    int X, Y, Width, Height;
};

#include "QRecti.inl"

}

#endif



