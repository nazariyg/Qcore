#ifndef QFRONTFACESTATE_H
#define QFRONTFACESTATE_H

#include "QStateBase.h"
#include "QSmartPointer.h"

namespace Q
{

class Q_ITEM FrontFaceState : public StateBase
{

Q_DECLARE_RTTI;

public:
    enum OrientationType
    {
        OT_CCW,
        OT_CW
    };

    // construction
    FrontFaceState (OrientationType eOrientation);

    OrientationType Orientation;
};

typedef SmartPointer<FrontFaceState> FrontFaceStatePtr;

}

#endif



