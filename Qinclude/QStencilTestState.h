#ifndef QSTENCILTESTSTATE_H
#define QSTENCILTESTSTATE_H

#include "QStateBase.h"
#include "QSmartPointer.h"

namespace Q
{

class Q_ITEM StencilTestState : public StateBase
{

Q_DECLARE_RTTI;

public:
    enum FunctionType
    {
        FT_NEVER,
        FT_ALWAYS,
        FT_LESS,
        FT_LESS_OR_EQUAL,
        FT_EQUAL,
        FT_GREATER_OR_EQUAL,
        FT_GREATER,
        FT_NOT_EQUAL
    };

    enum OperationType
    {
        OT_KEEP,
        OT_ZERO,
        OT_REPLACE,
        OT_INCR,
        OT_DECR,
        OT_INVERT,
        OT_INCR_WRAP,
        OT_DECR_WRAP
    };

    enum FaceType
    {
        FT_FRONT,
        FT_BACK,
        FT_BOTH
    };

    // Construction.

    // regular
    StencilTestState (FunctionType eFunction, int iReference, unsigned int uiMask, OperationType eStencilFail,
        OperationType eStencilPassDepthFail, OperationType eStencilPassDepthPass);

    // separate
    StencilTestState (FaceType eFace, FunctionType eFunction, int iReference, unsigned int uiMask,
        OperationType eStencilFail, OperationType eStencilPassDepthFail, OperationType eStencilPassDepthPass);

    bool Separate;
    FunctionType Function;
    FaceType Face;
    int Reference;
    unsigned int Mask;
    OperationType StencilFail;
    OperationType StencilPassDepthFail;
    OperationType StencilPassDepthPass;
};

typedef SmartPointer<StencilTestState> StencilTestStatePtr;

}

#endif



