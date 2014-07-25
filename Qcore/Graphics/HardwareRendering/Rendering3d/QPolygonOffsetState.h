#ifndef QPOLYGONOFFSETSTATE_H
#define QPOLYGONOFFSETSTATE_H

#include "QStateBase.h"
#include "QSmartPointer.h"

namespace Q
{

class Q_ITEM PolygonOffsetState : public StateBase
{

Q_DECLARE_RTTI;

public:
    enum ModeType
    {
        MT_POINT,
        MT_LINE,
        MT_FILL
    };

    // construction
    PolygonOffsetState (ModeType eMode, float fFactor, float fUnits);

    ModeType Mode;
    float Factor;
    float Units;
};

typedef SmartPointer<PolygonOffsetState> PolygonOffsetStatePtr;

}

#endif



