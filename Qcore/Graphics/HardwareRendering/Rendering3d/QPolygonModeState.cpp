#include "QPolygonModeState.h"
using namespace Q;

Q_IMPLEMENT_RTTI(Q,HeapRtti,PolygonModeState);

//------------------------------------------------------------------------------------------------------------------
PolygonModeState::PolygonModeState (FaceType eFace, ModeType eMode)
    :
    Face(eFace),
    Mode(eMode)
{
}
//------------------------------------------------------------------------------------------------------------------



