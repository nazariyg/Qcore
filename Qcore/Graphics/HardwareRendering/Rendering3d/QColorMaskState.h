#ifndef QCOLORMASKSTATE_H
#define QCOLORMASKSTATE_H

#include "QStateBase.h"
#include "QSmartPointer.h"

namespace Q
{

class Q_ITEM ColorMaskState : public StateBase
{

Q_DECLARE_RTTI;

public:
    // construction
    ColorMaskState (bool bRed, bool bGreen, bool bBlue, bool bAlpha);
    ColorMaskState (bool bEnableWrite);

    bool Red;
    bool Green;
    bool Blue;
    bool Alpha;
};

typedef SmartPointer<ColorMaskState> ColorMaskStatePtr;

}

#endif



