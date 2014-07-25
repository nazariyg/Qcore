#include "QFogState.h"
using namespace Q;

Q_IMPLEMENT_RTTI(Q,HeapRtti,FogState);

//------------------------------------------------------------------------------------------------------------------
FogState::FogState (EquationType eEquation, CoordinateSourceType eCoordinateSource, HintType eHint,
    const ColorRgba& rqColor, float fDensity, float fStart, float fEnd)
    :
    Equation(eEquation),
    CoordinateSource(eCoordinateSource),
    Hint(eHint),
    Color(rqColor),
    Density(fDensity),
    Start(fStart),
    End(fEnd)
{
}
//------------------------------------------------------------------------------------------------------------------



