#include "QRecti.h"
using namespace Q;

//------------------------------------------------------------------------------------------------------------------
Recti::Recti ()
{
}
//------------------------------------------------------------------------------------------------------------------
Recti::Recti (int iX, int iY, int iWidth, int iHeight)
    :
    X(iX),
    Y(iY),
    Width(iWidth),
    Height(iHeight)
{
    assert( iWidth >= 0 && iHeight >= 0 );
}
//------------------------------------------------------------------------------------------------------------------
Recti Recti::GetMerged (const Recti& rqRect) const
{
    Recti qNewRect;

    int iXBound0 = X + Width;
    int iXBound1 = rqRect.X + rqRect.Width;
    int iYBound0 = Y + Height;
    int iYBound1 = rqRect.Y + rqRect.Height;
    if ( X < rqRect.X )
    {
        qNewRect.X = X;
        qNewRect.Width = ( iXBound0 > iXBound1 ? Width : iXBound1 - X );
    }
    else
    {
        qNewRect.X = rqRect.X;
        qNewRect.Width = ( iXBound1 > iXBound0 ? rqRect.Width : iXBound0 - rqRect.X );
    }
    if ( Y < rqRect.Y )
    {
        qNewRect.Y = Y;
        qNewRect.Height = ( iYBound0 > iYBound1 ? Height : iYBound1 - Y );
    }
    else
    {
        qNewRect.Y = rqRect.Y;
        qNewRect.Height = ( iYBound1 > iYBound0 ? rqRect.Height : iYBound0 - rqRect.Y );
    }

    return qNewRect;
}
//------------------------------------------------------------------------------------------------------------------



