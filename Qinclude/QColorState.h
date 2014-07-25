#ifndef QCOLORSTATE_H
#define QCOLORSTATE_H

#include "QStateBase.h"
#include "QSmartPointer.h"

namespace Q
{

class Q_ITEM ColorState : public StateBase
{

Q_DECLARE_RTTI;

public:
    // construction
    ColorState (const ColorRgba& rqColor);

    ColorRgba Color;
};

typedef SmartPointer<ColorState> ColorStatePtr;

}

#endif



