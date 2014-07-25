#include "QDepthRangeState.h"
using namespace Q;

Q_IMPLEMENT_RTTI(Q,HeapRtti,DepthRangeState);

//------------------------------------------------------------------------------------------------------------------
DepthRangeState::DepthRangeState (double dNear, double dFar)
    :
    Near(dNear),
    Far(dFar)
{
}
//------------------------------------------------------------------------------------------------------------------



