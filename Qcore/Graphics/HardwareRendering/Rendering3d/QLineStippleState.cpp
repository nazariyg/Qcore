#include "QLineStippleState.h"
using namespace Q;

Q_IMPLEMENT_RTTI(Q,HeapRtti,LineStippleState);

//------------------------------------------------------------------------------------------------------------------
LineStippleState::LineStippleState (int iFactor, unsigned short usPattern)
    :
    Factor(iFactor),
    Pattern(usPattern)
{
}
//------------------------------------------------------------------------------------------------------------------



