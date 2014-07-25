#ifndef QPOINTPARAMETERSTATE_H
#define QPOINTPARAMETERSTATE_H

#include "QStateBase.h"
#include "QSmartPointer.h"

namespace Q
{

class Q_ITEM PointParameterState : public StateBase
{

Q_DECLARE_RTTI;

public:
    // construction
    PointParameterState (float fMinSize, float fMaxSize, const Vector3f& rqDistAttenCoeffs,
        float fFadeThresholdSize);

    float MinSize;
    float MaxSize;
    Vector3f DistAttenCoeffs;
    float FadeThresholdSize;
};

typedef SmartPointer<PointParameterState> PointParameterStatePtr;

}

#endif



