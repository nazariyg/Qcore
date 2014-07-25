#include "QStencilTestState.h"
using namespace Q;

Q_IMPLEMENT_RTTI(Q,HeapRtti,StencilTestState);

//------------------------------------------------------------------------------------------------------------------
StencilTestState::StencilTestState (FunctionType eFunction, int iReference, unsigned int uiMask,
    OperationType eStencilFail, OperationType eStencilPassDepthFail, OperationType eStencilPassDepthPass)
    :
    Separate(false),
    Function(eFunction),
    Reference(iReference),
    Mask(uiMask),
    StencilFail(eStencilFail),
    StencilPassDepthFail(eStencilPassDepthFail),
    StencilPassDepthPass(eStencilPassDepthPass)
{
}
//------------------------------------------------------------------------------------------------------------------
StencilTestState::StencilTestState (FaceType eFace, FunctionType eFunction, int iReference, unsigned int uiMask,
    OperationType eStencilFail, OperationType eStencilPassDepthFail, OperationType eStencilPassDepthPass)
    :
    Separate(true),
    Face(eFace),
    Function(eFunction),
    Reference(iReference),
    Mask(uiMask),
    StencilFail(eStencilFail),
    StencilPassDepthFail(eStencilPassDepthFail),
    StencilPassDepthPass(eStencilPassDepthPass)
{
}
//------------------------------------------------------------------------------------------------------------------



