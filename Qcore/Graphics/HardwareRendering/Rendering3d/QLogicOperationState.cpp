#include "QLogicOperationState.h"
using namespace Q;

Q_IMPLEMENT_RTTI(Q,HeapRtti,LogicOperationState);

//------------------------------------------------------------------------------------------------------------------
LogicOperationState::LogicOperationState (OperationType eOperation)
    :
    Operation(eOperation)
{
}
//------------------------------------------------------------------------------------------------------------------



