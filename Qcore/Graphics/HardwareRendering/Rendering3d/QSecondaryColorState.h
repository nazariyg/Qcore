#ifndef QSECONDARYCOLORSTATE_H
#define QSECONDARYCOLORSTATE_H

#include "QStateBase.h"
#include "QSmartPointer.h"

namespace Q
{

class Q_ITEM SecondaryColorState : public StateBase
{

Q_DECLARE_RTTI;

public:
    // construction
    SecondaryColorState (const ColorRgb& rqColor);

    ColorRgb Color;
};

typedef SmartPointer<SecondaryColorState> SecondaryColorStatePtr;

}

#endif



