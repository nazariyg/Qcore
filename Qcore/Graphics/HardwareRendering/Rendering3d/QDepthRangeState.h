#ifndef QDEPTHRANGESTATE_H
#define QDEPTHRANGESTATE_H

#include "QStateBase.h"
#include "QSmartPointer.h"

namespace Q
{

class Q_ITEM DepthRangeState : public StateBase
{

Q_DECLARE_RTTI;

public:
    // construction
    DepthRangeState (double dNear, double dFar);

    double Near, Far;
};

typedef SmartPointer<DepthRangeState> DepthRangeStatePtr;

}

#endif



