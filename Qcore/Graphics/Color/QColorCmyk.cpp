#include "QColorCmyk.h"
using namespace Q;

//------------------------------------------------------------------------------------------------------------------
ColorCmyk::ColorCmyk ()
{
}
//------------------------------------------------------------------------------------------------------------------
ColorCmyk::ColorCmyk (float fC, float fM, float fY, float fK)
{
    m_afTuple[0] = fC;
    m_afTuple[1] = fM;
    m_afTuple[2] = fY;
    m_afTuple[3] = fK;
}
//------------------------------------------------------------------------------------------------------------------
ColorCmyk::ColorCmyk (const ColorCmyk& rqColor)
{
    m_afTuple[0] = rqColor.m_afTuple[0];
    m_afTuple[1] = rqColor.m_afTuple[1];
    m_afTuple[2] = rqColor.m_afTuple[2];
    m_afTuple[3] = rqColor.m_afTuple[3];
}
//------------------------------------------------------------------------------------------------------------------
bool ColorCmyk::operator== (const ColorCmyk& rqColor) const
{
    return ( m_afTuple[0] == rqColor.m_afTuple[0]
        &&   m_afTuple[1] == rqColor.m_afTuple[1]
        &&   m_afTuple[2] == rqColor.m_afTuple[2]
        &&   m_afTuple[3] == rqColor.m_afTuple[3] );
}
//------------------------------------------------------------------------------------------------------------------
bool ColorCmyk::operator!= (const ColorCmyk& rqColor) const
{
    return ( m_afTuple[0] != rqColor.m_afTuple[0]
        ||   m_afTuple[1] != rqColor.m_afTuple[1]
        ||   m_afTuple[2] != rqColor.m_afTuple[2]
        ||   m_afTuple[3] != rqColor.m_afTuple[3] );
}
//------------------------------------------------------------------------------------------------------------------
bool ColorCmyk::operator< (const ColorCmyk& rqColor) const
{
    return ( MemCmp(m_afTuple,rqColor.m_afTuple,16) < 0 );
}
//------------------------------------------------------------------------------------------------------------------
bool ColorCmyk::operator<= (const ColorCmyk& rqColor) const
{
    return ( MemCmp(m_afTuple,rqColor.m_afTuple,16) <= 0 );
}
//------------------------------------------------------------------------------------------------------------------
bool ColorCmyk::operator> (const ColorCmyk& rqColor) const
{
    return ( MemCmp(m_afTuple,rqColor.m_afTuple,16) > 0 );
}
//------------------------------------------------------------------------------------------------------------------
bool ColorCmyk::operator>= (const ColorCmyk& rqColor) const
{
    return ( MemCmp(m_afTuple,rqColor.m_afTuple,16) >= 0 );
}
//------------------------------------------------------------------------------------------------------------------
ColorCmyk ColorCmyk::operator+ (const ColorCmyk& rqColor) const
{
    return ColorCmyk(
        m_afTuple[0] + rqColor.m_afTuple[0],
        m_afTuple[1] + rqColor.m_afTuple[1],
        m_afTuple[2] + rqColor.m_afTuple[2],
        m_afTuple[3] + rqColor.m_afTuple[3]);
}
//------------------------------------------------------------------------------------------------------------------
ColorCmyk ColorCmyk::operator- (const ColorCmyk& rqColor) const
{
    return ColorCmyk(
        m_afTuple[0] - rqColor.m_afTuple[0],
        m_afTuple[1] - rqColor.m_afTuple[1],
        m_afTuple[2] - rqColor.m_afTuple[2],
        m_afTuple[3] - rqColor.m_afTuple[3]);
}
//------------------------------------------------------------------------------------------------------------------
ColorCmyk ColorCmyk::operator* (const ColorCmyk& rqColor) const
{
    return ColorCmyk(
        m_afTuple[0]*rqColor.m_afTuple[0],
        m_afTuple[1]*rqColor.m_afTuple[1],
        m_afTuple[2]*rqColor.m_afTuple[2],
        m_afTuple[3]*rqColor.m_afTuple[3]);
}
//------------------------------------------------------------------------------------------------------------------
ColorCmyk ColorCmyk::operator* (float fScalar) const
{
    return ColorCmyk(
        fScalar*m_afTuple[0],
        fScalar*m_afTuple[1],
        fScalar*m_afTuple[2],
        fScalar*m_afTuple[3]);
}
//------------------------------------------------------------------------------------------------------------------
ColorCmyk& ColorCmyk::operator+= (const ColorCmyk& rqColor)
{
    m_afTuple[0] += rqColor.m_afTuple[0];
    m_afTuple[1] += rqColor.m_afTuple[1];
    m_afTuple[2] += rqColor.m_afTuple[2];
    m_afTuple[3] += rqColor.m_afTuple[3];
    return *this;
}
//------------------------------------------------------------------------------------------------------------------
ColorCmyk& ColorCmyk::operator-= (const ColorCmyk& rqColor)
{
    m_afTuple[0] -= rqColor.m_afTuple[0];
    m_afTuple[1] -= rqColor.m_afTuple[1];
    m_afTuple[2] -= rqColor.m_afTuple[2];
    m_afTuple[3] -= rqColor.m_afTuple[3];
    return *this;
}
//------------------------------------------------------------------------------------------------------------------
ColorCmyk& ColorCmyk::operator*= (const ColorCmyk& rqColor)
{
    m_afTuple[0] *= rqColor.m_afTuple[0];
    m_afTuple[1] *= rqColor.m_afTuple[1];
    m_afTuple[2] *= rqColor.m_afTuple[2];
    m_afTuple[3] *= rqColor.m_afTuple[3];
    return *this;
}
//------------------------------------------------------------------------------------------------------------------
ColorCmyk& ColorCmyk::operator*= (float fScalar)
{
    m_afTuple[0] *= fScalar;
    m_afTuple[1] *= fScalar;
    m_afTuple[2] *= fScalar;
    m_afTuple[3] *= fScalar;
    return *this;
}
//------------------------------------------------------------------------------------------------------------------
void ColorCmyk::Clamp ()
{
    for (int i = 0; i < 4; i++)
    {
        if ( m_afTuple[i] > 1.0f )
            m_afTuple[i] = 1.0f;
        else if ( m_afTuple[i] < 0.0f )
            m_afTuple[i] = 0.0f;
    }
}
//------------------------------------------------------------------------------------------------------------------



