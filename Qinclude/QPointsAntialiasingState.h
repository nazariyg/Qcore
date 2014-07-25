#ifndef QPOINTSANTIALIASINGSTATE_H
#define QPOINTSANTIALIASINGSTATE_H

#include "QStateBase.h"
#include "QSmartPointer.h"

namespace Q
{

class Q_ITEM PointsAntialiasingState : public StateBase
{

Q_DECLARE_RTTI;

public:
    enum HintType
    {
        HT_FASTEST,
        HT_NICEST
    };

    // construction
    PointsAntialiasingState (HintType eHint);

    HintType Hint;
};

typedef SmartPointer<PointsAntialiasingState> PointsAntialiasingStatePtr;

}

#endif



