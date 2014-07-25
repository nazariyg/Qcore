#ifndef QLOGICOPERATIONSTATE_H
#define QLOGICOPERATIONSTATE_H

#include "QStateBase.h"
#include "QSmartPointer.h"

namespace Q
{

class Q_ITEM LogicOperationState : public StateBase
{

Q_DECLARE_RTTI;

public:
    enum OperationType
    {
        OT_CLEAR,
        OT_AND,
        OT_AND_REVERSE,
        OT_COPY,
        OT_AND_INVERTED,
        OT_NOOP,
        OT_XOR,
        OT_OR,
        OT_NOR,
        OT_EQUIV,
        OT_INVERT,
        OT_OR_REVERSE,
        OT_COPY_INVERTED,
        OT_OR_INVERTED,
        OT_NAND,
        OT_SET
    };

    // construction
    LogicOperationState (OperationType eOperation);

    OperationType Operation;
};

typedef SmartPointer<LogicOperationState> LogicOperationStatePtr;

}

#endif



