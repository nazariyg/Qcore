#ifndef QMATERIALSTATE_H
#define QMATERIALSTATE_H

#include "QStateBase.h"
#include "QHRMaterial.h"

namespace Q
{

class Q_ITEM MaterialState : public StateBase
{

Q_DECLARE_RTTI;

public:
    // construction
    MaterialState (const HRMaterialPtr& rspqMaterial);

    HRMaterialPtr Material;
};

typedef SmartPointer<MaterialState> MaterialStatePtr;

}

#endif



