#include "QPointParameterState.h"
using namespace Q;

Q_IMPLEMENT_RTTI(Q,HeapRtti,PointParameterState);

//------------------------------------------------------------------------------------------------------------------
PointParameterState::PointParameterState (float fMinSize, float fMaxSize, const Vector3f& rqDistAttenCoeffs,
    float fFadeThresholdSize)
    :
    MinSize(fMinSize),
    MaxSize(fMaxSize),
    DistAttenCoeffs(rqDistAttenCoeffs),
    FadeThresholdSize(fFadeThresholdSize)
{
}
//------------------------------------------------------------------------------------------------------------------



