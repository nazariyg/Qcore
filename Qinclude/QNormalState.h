#ifndef QNORMALSTATE_H
#define QNORMALSTATE_H

#include "QStateBase.h"
#include "QSmartPointer.h"

namespace Q
{

template <class Real>
class NormalState : public StateBase
{

Q_DECLARE_TEMPLATE_RTTI;

public:
    // construction
    NormalState (const Vector3<Real>& rqNormal);

    Vector3<Real> Normal;
};

#include "QNormalState.inl"

typedef NormalState<float> NormalStatef;
typedef NormalState<double> NormalStated;

typedef SmartPointer<NormalStatef> NormalStatefPtr;
typedef SmartPointer<NormalStated> NormalStatedPtr;

}

#endif



