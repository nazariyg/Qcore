#include "QScissorState.h"
using namespace Q;

Q_IMPLEMENT_RTTI(Q,HeapRtti,ScissorState);

//------------------------------------------------------------------------------------------------------------------
ScissorState::ScissorState (int iX, int iY, int iWidth, int iHeight)
    :
    X(iX),
    Y(iY),
    Width(iWidth),
    Height(iHeight)
{
    assert( iX >= 0 && iY >= 0 && iWidth >= 0 && iHeight >= 0 );
}
//------------------------------------------------------------------------------------------------------------------
ScissorState::ScissorState (const Recti& rqRect)
    :
    X(rqRect.X),
    Y(rqRect.Y),
    Width(rqRect.Width),
    Height(rqRect.Height)
{
}
//------------------------------------------------------------------------------------------------------------------



