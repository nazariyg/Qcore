#include "QLightState.h"
using namespace Q;

Q_IMPLEMENT_RTTI(Q,HeapRtti,LightState);

//------------------------------------------------------------------------------------------------------------------
LightState::LightState ()
    :
    ShadeModelSet(false),
    GlobalAmbientSet(false),
    ActualViewpointForSpecularSet(false),
    TwoSidedSet(false),
    SeparateSpecularSet(false)
{
}
//------------------------------------------------------------------------------------------------------------------
int LightState::AddLight (const HRLight3Ptr& rspqLight)
{
    return m_qLights.Push(rspqLight);
}
//------------------------------------------------------------------------------------------------------------------
void LightState::RemoveLight (int i)
{
    m_qLights.Remove(i);
}
//------------------------------------------------------------------------------------------------------------------



