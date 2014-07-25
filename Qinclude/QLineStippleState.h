#ifndef QLINESTIPPLESTATE_H
#define QLINESTIPPLESTATE_H

#include "QStateBase.h"
#include "QSmartPointer.h"

namespace Q
{

class Q_ITEM LineStippleState : public StateBase
{

Q_DECLARE_RTTI;

public:
    // construction
    LineStippleState (int iFactor, unsigned short usPattern);

    int Factor;
    unsigned short Pattern;
};

typedef SmartPointer<LineStippleState> LineStippleStatePtr;

}

#endif



