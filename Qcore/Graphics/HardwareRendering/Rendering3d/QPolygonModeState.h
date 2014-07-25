#ifndef QPOLYGONMODESTATE_H
#define QPOLYGONMODESTATE_H

#include "QStateBase.h"
#include "QSmartPointer.h"

namespace Q
{

class Q_ITEM PolygonModeState : public StateBase
{

Q_DECLARE_RTTI;

public:
    enum FaceType
    {
        FT_FRONT,
        FT_BACK,
        FT_BOTH
    };

    enum ModeType
    {
        MT_POINT,
        MT_LINE,
        MT_FILL
    };

    // construction
    PolygonModeState (FaceType eFace, ModeType eMode);

    FaceType Face;
    ModeType Mode;
};

typedef SmartPointer<PolygonModeState> PolygonModeStatePtr;

}

#endif



