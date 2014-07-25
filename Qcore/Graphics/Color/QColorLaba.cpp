#include "QColorLaba.h"
using namespace Q;

//------------------------------------------------------------------------------------------------------------------
ColorLaba::ColorLaba ()
{
}
//------------------------------------------------------------------------------------------------------------------
ColorLaba::ColorLaba (float fL, float fA, float fB, float fAlpha)
{
    m_afTuple[0] = fL;
    m_afTuple[1] = fA;
    m_afTuple[2] = fB;
    m_afTuple[3] = fAlpha;
}
//------------------------------------------------------------------------------------------------------------------
ColorLaba::ColorLaba (const ColorLaba& rqColor)
{
    m_afTuple[0] = rqColor.m_afTuple[0];
    m_afTuple[1] = rqColor.m_afTuple[1];
    m_afTuple[2] = rqColor.m_afTuple[2];
    m_afTuple[3] = rqColor.m_afTuple[3];
}
//------------------------------------------------------------------------------------------------------------------
bool ColorLaba::operator== (const ColorLaba& rqColor) const
{
    return ( m_afTuple[0] == rqColor.m_afTuple[0]
        &&   m_afTuple[1] == rqColor.m_afTuple[1]
        &&   m_afTuple[2] == rqColor.m_afTuple[2]
        &&   m_afTuple[3] == rqColor.m_afTuple[3] );
}
//------------------------------------------------------------------------------------------------------------------
bool ColorLaba::operator!= (const ColorLaba& rqColor) const
{
    return ( m_afTuple[0] != rqColor.m_afTuple[0]
        ||   m_afTuple[1] != rqColor.m_afTuple[1]
        ||   m_afTuple[2] != rqColor.m_afTuple[2]
        ||   m_afTuple[3] != rqColor.m_afTuple[3] );
}
//------------------------------------------------------------------------------------------------------------------
bool ColorLaba::operator< (const ColorLaba& rqColor) const
{
    return ( MemCmp(m_afTuple,rqColor.m_afTuple,16) < 0 );
}
//------------------------------------------------------------------------------------------------------------------
bool ColorLaba::operator<= (const ColorLaba& rqColor) const
{
    return ( MemCmp(m_afTuple,rqColor.m_afTuple,16) <= 0 );
}
//------------------------------------------------------------------------------------------------------------------
bool ColorLaba::operator> (const ColorLaba& rqColor) const
{
    return ( MemCmp(m_afTuple,rqColor.m_afTuple,16) > 0 );
}
//------------------------------------------------------------------------------------------------------------------
bool ColorLaba::operator>= (const ColorLaba& rqColor) const
{
    return ( MemCmp(m_afTuple,rqColor.m_afTuple,16) >= 0 );
}
//------------------------------------------------------------------------------------------------------------------
ColorLaba ColorLaba::operator+ (const ColorLaba& rqColor) const
{
    return ColorLaba(
        m_afTuple[0] + rqColor.m_afTuple[0],
        m_afTuple[1] + rqColor.m_afTuple[1],
        m_afTuple[2] + rqColor.m_afTuple[2],
        m_afTuple[3] + rqColor.m_afTuple[3]);
}
//------------------------------------------------------------------------------------------------------------------
ColorLaba ColorLaba::operator- (const ColorLaba& rqColor) const
{
    return ColorLaba(
        m_afTuple[0] - rqColor.m_afTuple[0],
        m_afTuple[1] - rqColor.m_afTuple[1],
        m_afTuple[2] - rqColor.m_afTuple[2],
        m_afTuple[3] - rqColor.m_afTuple[3]);
}
//------------------------------------------------------------------------------------------------------------------
ColorLaba ColorLaba::operator* (const ColorLaba& rqColor) const
{
    return ColorLaba(
        m_afTuple[0]*rqColor.m_afTuple[0],
        m_afTuple[1]*rqColor.m_afTuple[1],
        m_afTuple[2]*rqColor.m_afTuple[2],
        m_afTuple[3]*rqColor.m_afTuple[3]);
}
//------------------------------------------------------------------------------------------------------------------
ColorLaba ColorLaba::operator* (float fScalar) const
{
    return ColorLaba(
        fScalar*m_afTuple[0],
        fScalar*m_afTuple[1],
        fScalar*m_afTuple[2],
        fScalar*m_afTuple[3]);
}
//------------------------------------------------------------------------------------------------------------------
ColorLaba& ColorLaba::operator+= (const ColorLaba& rqColor)
{
    m_afTuple[0] += rqColor.m_afTuple[0];
    m_afTuple[1] += rqColor.m_afTuple[1];
    m_afTuple[2] += rqColor.m_afTuple[2];
    m_afTuple[3] += rqColor.m_afTuple[3];
    return *this;
}
//------------------------------------------------------------------------------------------------------------------
ColorLaba& ColorLaba::operator-= (const ColorLaba& rqColor)
{
    m_afTuple[0] -= rqColor.m_afTuple[0];
    m_afTuple[1] -= rqColor.m_afTuple[1];
    m_afTuple[2] -= rqColor.m_afTuple[2];
    m_afTuple[3] -= rqColor.m_afTuple[3];
    return *this;
}
//------------------------------------------------------------------------------------------------------------------
ColorLaba& ColorLaba::operator*= (const ColorLaba& rqColor)
{
    m_afTuple[0] *= rqColor.m_afTuple[0];
    m_afTuple[1] *= rqColor.m_afTuple[1];
    m_afTuple[2] *= rqColor.m_afTuple[2];
    m_afTuple[3] *= rqColor.m_afTuple[3];
    return *this;
}
//------------------------------------------------------------------------------------------------------------------
ColorLaba& ColorLaba::operator*= (float fScalar)
{
    m_afTuple[0] *= fScalar;
    m_afTuple[1] *= fScalar;
    m_afTuple[2] *= fScalar;
    m_afTuple[3] *= fScalar;
    return *this;
}
//------------------------------------------------------------------------------------------------------------------
void ColorLaba::Clamp ()
{
    if ( m_afTuple[0] > 100.0f )
        m_afTuple[0] = 100.0f;
    else if ( m_afTuple[0] < 0.0f )
        m_afTuple[0] = 0.0f;
    if ( m_afTuple[3] > 1.0f )
        m_afTuple[3] = 1.0f;
    else if ( m_afTuple[3] < 0.0f )
        m_afTuple[3] = 0.0f;
}
//------------------------------------------------------------------------------------------------------------------



