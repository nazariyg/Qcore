#ifndef QSTENCILMASKSTATE_H
#define QSTENCILMASKSTATE_H

#include "QStateBase.h"
#include "QSmartPointer.h"

namespace Q
{

class Q_ITEM StencilMaskState : public StateBase
{

Q_DECLARE_RTTI;

public:
    enum FaceType
    {
        FT_FRONT,
        FT_BACK,
        FT_BOTH
    };

    // Construction.

    // regular
    StencilMaskState (unsigned int uiMask);
    StencilMaskState (bool bEnableWrite);

    // separate
    StencilMaskState (FaceType eFace, unsigned int uiMask);
    StencilMaskState (FaceType eFace, bool bEnableWrite);

    bool Separate;
    unsigned int Mask;
    FaceType Face;
};

typedef SmartPointer<StencilMaskState> StencilMaskStatePtr;

}

#endif



