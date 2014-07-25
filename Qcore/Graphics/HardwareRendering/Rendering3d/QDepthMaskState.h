#ifndef QDEPTHMASKSTATE_H
#define QDEPTHMASKSTATE_H

#include "QStateBase.h"
#include "QSmartPointer.h"

namespace Q
{

class Q_ITEM DepthMaskState : public StateBase
{

Q_DECLARE_RTTI;

public:
    // construction
    DepthMaskState (bool bEnableWrite);

    bool EnableWrite;
};

typedef SmartPointer<DepthMaskState> DepthMaskStatePtr;

}

#endif



