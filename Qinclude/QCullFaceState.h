#ifndef QCULLFACESTATE_H
#define QCULLFACESTATE_H

#include "QStateBase.h"
#include "QSmartPointer.h"

namespace Q
{

class Q_ITEM CullFaceState : public StateBase
{

Q_DECLARE_RTTI;

public:
    enum FaceType
    {
        FT_FRONT,
        FT_BACK,
        FT_BOTH
    };

    // construction
    CullFaceState (FaceType eFace);

    FaceType Face;
};

typedef SmartPointer<CullFaceState> CullFaceStatePtr;

}

#endif



