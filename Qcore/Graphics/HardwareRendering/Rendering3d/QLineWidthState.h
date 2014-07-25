#ifndef QLINEWIDTHSTATE_H
#define QLINEWIDTHSTATE_H

#include "QStateBase.h"
#include "QSmartPointer.h"

namespace Q
{

class Q_ITEM LineWidthState : public StateBase
{

Q_DECLARE_RTTI;

public:
    // construction
    LineWidthState (float fWidth);

    float Width;
};

typedef SmartPointer<LineWidthState> LineWidthStatePtr;

}

#endif



