#include "QPolygonOffsetState.h"
using namespace Q;

Q_IMPLEMENT_RTTI(Q,HeapRtti,PolygonOffsetState);

//------------------------------------------------------------------------------------------------------------------
PolygonOffsetState::PolygonOffsetState (ModeType eMode, float fFactor, float fUnits)
    :
    Mode(eMode),
    Factor(fFactor),
    Units(fUnits)
{
}
//------------------------------------------------------------------------------------------------------------------



