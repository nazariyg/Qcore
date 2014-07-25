#include "QColorMaskState.h"
using namespace Q;

Q_IMPLEMENT_RTTI(Q,HeapRtti,ColorMaskState);

//------------------------------------------------------------------------------------------------------------------
ColorMaskState::ColorMaskState (bool bRed, bool bGreen, bool bBlue, bool bAlpha)
    :
    Red(bRed),
    Green(bGreen),
    Blue(bBlue),
    Alpha(bAlpha)
{
}
//------------------------------------------------------------------------------------------------------------------
ColorMaskState::ColorMaskState (bool bEnableWrite)
{
    if ( bEnableWrite )
    {
        Red = true;
        Green = true;
        Blue = true;
        Alpha = true;
    }
    else
    {
        Red = false;
        Green = false;
        Blue = false;
        Alpha = false;
    }
}
//------------------------------------------------------------------------------------------------------------------



