#include "QStencilMaskState.h"
using namespace Q;

Q_IMPLEMENT_RTTI(Q,HeapRtti,StencilMaskState);

//------------------------------------------------------------------------------------------------------------------
StencilMaskState::StencilMaskState (unsigned int uiMask)
    :
    Separate(false),
    Mask(uiMask)
{
}
//------------------------------------------------------------------------------------------------------------------
StencilMaskState::StencilMaskState (bool bEnableWrite)
    :
    Separate(false)
{
    Mask = ( bEnableWrite ? ~0 : 0 );
}
//------------------------------------------------------------------------------------------------------------------
StencilMaskState::StencilMaskState (FaceType eFace, unsigned int uiMask)
    :
    Separate(true),
    Mask(uiMask),
    Face(eFace)
{
}
//------------------------------------------------------------------------------------------------------------------
StencilMaskState::StencilMaskState (FaceType eFace, bool bEnableWrite)
    :
    Separate(true),
    Face(eFace)
{
    Mask = ( bEnableWrite ? ~0 : 0 );
}
//------------------------------------------------------------------------------------------------------------------



