#ifndef QPOINTSIZESTATE_H
#define QPOINTSIZESTATE_H

#include "QStateBase.h"
#include "QSmartPointer.h"

namespace Q
{

class Q_ITEM PointSizeState : public StateBase
{

Q_DECLARE_RTTI;

public:
    // construction
    PointSizeState (float fSize);

    float Size;
};

typedef SmartPointer<PointSizeState> PointSizeStatePtr;

}

#endif



