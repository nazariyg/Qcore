#include "QAlphaTestState.h"
using namespace Q;

Q_IMPLEMENT_RTTI(Q,HeapRtti,AlphaTestState);

//------------------------------------------------------------------------------------------------------------------
AlphaTestState::AlphaTestState (FunctionType eFunction, float fReference)
    :
    Function(eFunction),
    Reference(fReference)
{
}
//------------------------------------------------------------------------------------------------------------------



