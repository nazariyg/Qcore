#ifndef QPOLYGONSTIPPLESTATE_H
#define QPOLYGONSTIPPLESTATE_H

#include "QHeapArrayTypedefs.h"
#include "QStateBase.h"

namespace Q
{

class Q_ITEM PolygonStippleState : public StateBase
{

Q_DECLARE_RTTI;

public:
    // construction
    PolygonStippleState (const UCharHeapArrayPtr& rspqPattern);

    UCharHeapArrayPtr Pattern;
};

typedef SmartPointer<PolygonStippleState> PolygonStippleStatePtr;

}

#endif



