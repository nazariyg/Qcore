#ifndef QSCISSORSTATE_H
#define QSCISSORSTATE_H

#include "QStateBase.h"
#include "QSmartPointer.h"
#include "QRecti.h"

namespace Q
{

class Q_ITEM ScissorState : public StateBase
{

Q_DECLARE_RTTI;

public:
    // construction
    ScissorState (int iX, int iY, int iWidth, int iHeight);
    ScissorState (const Recti& rqRect);

    int X, Y, Width, Height;
};

typedef SmartPointer<ScissorState> ScissorStatePtr;

}

#endif



