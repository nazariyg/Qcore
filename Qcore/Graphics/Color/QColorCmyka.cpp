#include "QColorCmyka.h"
using namespace Q;

//------------------------------------------------------------------------------------------------------------------
ColorCmyka::ColorCmyka ()
{
}
//------------------------------------------------------------------------------------------------------------------
ColorCmyka::ColorCmyka (float fC, float fM, float fY, float fK, float fAlpha)
{
    m_afTuple[0] = fC;
    m_afTuple[1] = fM;
    m_afTuple[2] = fY;
    m_afTuple[3] = fK;
    m_afTuple[4] = fAlpha;
}
//------------------------------------------------------------------------------------------------------------------
ColorCmyka::ColorCmyka (const ColorCmyka& rqColor)
{
    m_afTuple[0] = rqColor.m_afTuple[0];
    m_afTuple[1] = rqColor.m_afTuple[1];
    m_afTuple[2] = rqColor.m_afTuple[2];
    m_afTuple[3] = rqColor.m_afTuple[3];
    m_afTuple[4] = rqColor.m_afTuple[4];
}
//------------------------------------------------------------------------------------------------------------------
bool ColorCmyka::operator== (const ColorCmyka& rqColor) const
{
    return ( m_afTuple[0] == rqColor.m_afTuple[0]
        &&   m_afTuple[1] == rqColor.m_afTuple[1]
        &&   m_afTuple[2] == rqColor.m_afTuple[2]
        &&   m_afTuple[3] == rqColor.m_afTuple[3]
        &&   m_afTuple[4] == rqColor.m_afTuple[4] );
}
//------------------------------------------------------------------------------------------------------------------
bool ColorCmyka::operator!= (const ColorCmyka& rqColor) const
{
    return ( m_afTuple[0] != rqColor.m_afTuple[0]
        ||   m_afTuple[1] != rqColor.m_afTuple[1]
        ||   m_afTuple[2] != rqColor.m_afTuple[2]
        ||   m_afTuple[3] != rqColor.m_afTuple[3]
        ||   m_afTuple[4] != rqColor.m_afTuple[4] );
}
//------------------------------------------------------------------------------------------------------------------
bool ColorCmyka::operator< (const ColorCmyka& rqColor) const
{
    return ( MemCmp(m_afTuple,rqColor.m_afTuple,20) < 0 );
}
//------------------------------------------------------------------------------------------------------------------
bool ColorCmyka::operator<= (const ColorCmyka& rqColor) const
{
    return ( MemCmp(m_afTuple,rqColor.m_afTuple,20) <= 0 );
}
//------------------------------------------------------------------------------------------------------------------
bool ColorCmyka::operator> (const ColorCmyka& rqColor) const
{
    return ( MemCmp(m_afTuple,rqColor.m_afTuple,20) > 0 );
}
//------------------------------------------------------------------------------------------------------------------
bool ColorCmyka::operator>= (const ColorCmyka& rqColor) const
{
    return ( MemCmp(m_afTuple,rqColor.m_afTuple,20) >= 0 );
}
//------------------------------------------------------------------------------------------------------------------
ColorCmyka ColorCmyka::operator+ (const ColorCmyka& rqColor) const
{
    return ColorCmyka(
        m_afTuple[0] + rqColor.m_afTuple[0],
        m_afTuple[1] + rqColor.m_afTuple[1],
        m_afTuple[2] + rqColor.m_afTuple[2],
        m_afTuple[3] + rqColor.m_afTuple[3],
        m_afTuple[4] + rqColor.m_afTuple[4]);
}
//------------------------------------------------------------------------------------------------------------------
ColorCmyka ColorCmyka::operator- (const ColorCmyka& rqColor) const
{
    return ColorCmyka(
        m_afTuple[0] - rqColor.m_afTuple[0],
        m_afTuple[1] - rqColor.m_afTuple[1],
        m_afTuple[2] - rqColor.m_afTuple[2],
        m_afTuple[3] - rqColor.m_afTuple[3],
        m_afTuple[4] - rqColor.m_afTuple[4]);
}
//------------------------------------------------------------------------------------------------------------------
ColorCmyka ColorCmyka::operator* (const ColorCmyka& rqColor) const
{
    return ColorCmyka(
        m_afTuple[0]*rqColor.m_afTuple[0],
        m_afTuple[1]*rqColor.m_afTuple[1],
        m_afTuple[2]*rqColor.m_afTuple[2],
        m_afTuple[3]*rqColor.m_afTuple[3],
        m_afTuple[4]*rqColor.m_afTuple[4]);
}
//------------------------------------------------------------------------------------------------------------------
ColorCmyka ColorCmyka::operator* (float fScalar) const
{
    return ColorCmyka(
        fScalar*m_afTuple[0],
        fScalar*m_afTuple[1],
        fScalar*m_afTuple[2],
        fScalar*m_afTuple[3],
        fScalar*m_afTuple[4]);
}
//------------------------------------------------------------------------------------------------------------------
ColorCmyka& ColorCmyka::operator+= (const ColorCmyka& rqColor)
{
    m_afTuple[0] += rqColor.m_afTuple[0];
    m_afTuple[1] += rqColor.m_afTuple[1];
    m_afTuple[2] += rqColor.m_afTuple[2];
    m_afTuple[3] += rqColor.m_afTuple[3];
    m_afTuple[4] += rqColor.m_afTuple[4];
    return *this;
}
//------------------------------------------------------------------------------------------------------------------
ColorCmyka& ColorCmyka::operator-= (const ColorCmyka& rqColor)
{
    m_afTuple[0] -= rqColor.m_afTuple[0];
    m_afTuple[1] -= rqColor.m_afTuple[1];
    m_afTuple[2] -= rqColor.m_afTuple[2];
    m_afTuple[3] -= rqColor.m_afTuple[3];
    m_afTuple[4] -= rqColor.m_afTuple[4];
    return *this;
}
//------------------------------------------------------------------------------------------------------------------
ColorCmyka& ColorCmyka::operator*= (const ColorCmyka& rqColor)
{
    m_afTuple[0] *= rqColor.m_afTuple[0];
    m_afTuple[1] *= rqColor.m_afTuple[1];
    m_afTuple[2] *= rqColor.m_afTuple[2];
    m_afTuple[3] *= rqColor.m_afTuple[3];
    m_afTuple[4] *= rqColor.m_afTuple[4];
    return *this;
}
//------------------------------------------------------------------------------------------------------------------
ColorCmyka& ColorCmyka::operator*= (float fScalar)
{
    m_afTuple[0] *= fScalar;
    m_afTuple[1] *= fScalar;
    m_afTuple[2] *= fScalar;
    m_afTuple[3] *= fScalar;
    m_afTuple[4] *= fScalar;
    return *this;
}
//------------------------------------------------------------------------------------------------------------------
void ColorCmyka::Clamp ()
{
    for (int i = 0; i < 5; i++)
    {
        if ( m_afTuple[i] > 1.0f )
            m_afTuple[i] = 1.0f;
        else if ( m_afTuple[i] < 0.0f )
            m_afTuple[i] = 0.0f;
    }
}
//------------------------------------------------------------------------------------------------------------------



