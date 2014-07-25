#include "QPaletteColor.h"
using namespace Q;

const PaletteColor PaletteColor::Black(0,0,0);
const PaletteColor PaletteColor::White(255,255,255);

//------------------------------------------------------------------------------------------------------------------
PaletteColor::PaletteColor ()
{
    // uninitialized
}
//------------------------------------------------------------------------------------------------------------------
PaletteColor::PaletteColor (unsigned char ucRed, unsigned char ucGreen, unsigned char ucBlue, unsigned char ucAlpha)
{
    Components[2] = ucRed;
    Components[1] = ucGreen;
    Components[0] = ucBlue;
    Components[3] = ucAlpha;
}
//------------------------------------------------------------------------------------------------------------------
bool PaletteColor::operator== (const PaletteColor& rqEntry) const
{
    return ( MemCmp(Components,rqEntry.Components,4) == 0 );
}
//------------------------------------------------------------------------------------------------------------------
bool PaletteColor::operator!= (const PaletteColor& rqEntry) const
{
    return ( MemCmp(Components,rqEntry.Components,4) != 0 );
}
//------------------------------------------------------------------------------------------------------------------
bool PaletteColor::operator< (const PaletteColor& rqEntry) const
{
    return ( MemCmp(Components,rqEntry.Components,4) < 0 );
}
//------------------------------------------------------------------------------------------------------------------
bool PaletteColor::operator<= (const PaletteColor& rqEntry) const
{
    return ( MemCmp(Components,rqEntry.Components,4) <= 0 );
}
//------------------------------------------------------------------------------------------------------------------
bool PaletteColor::operator> (const PaletteColor& rqEntry) const
{
    return ( MemCmp(Components,rqEntry.Components,4) > 0 );
}
//------------------------------------------------------------------------------------------------------------------
bool PaletteColor::operator>= (const PaletteColor& rqEntry) const
{
    return ( MemCmp(Components,rqEntry.Components,4) >= 0 );
}
//------------------------------------------------------------------------------------------------------------------



