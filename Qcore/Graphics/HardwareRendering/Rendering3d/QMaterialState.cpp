#include "QMaterialState.h"
using namespace Q;

Q_IMPLEMENT_RTTI(Q,HeapRtti,MaterialState);

//------------------------------------------------------------------------------------------------------------------
MaterialState::MaterialState (const HRMaterialPtr& rspqMaterial)
    :
    Material(rspqMaterial)
{
}
//------------------------------------------------------------------------------------------------------------------



