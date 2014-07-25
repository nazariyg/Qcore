#include "QColorLab.h"
using namespace Q;

//------------------------------------------------------------------------------------------------------------------
ColorLab::ColorLab ()
{
}
//------------------------------------------------------------------------------------------------------------------
ColorLab::ColorLab (float fL, float fA, float fB)
{
    m_afTuple[0] = fL;
    m_afTuple[1] = fA;
    m_afTuple[2] = fB;
}
//------------------------------------------------------------------------------------------------------------------
ColorLab::ColorLab (const ColorLab& rqColor)
{
    m_afTuple[0] = rqColor.m_afTuple[0];
    m_afTuple[1] = rqColor.m_afTuple[1];
    m_afTuple[2] = rqColor.m_afTuple[2];
}
//------------------------------------------------------------------------------------------------------------------
bool ColorLab::operator== (const ColorLab& rqColor) const
{
    return ( m_afTuple[0] == rqColor.m_afTuple[0]
        &&   m_afTuple[1] == rqColor.m_afTuple[1]
        &&   m_afTuple[2] == rqColor.m_afTuple[2] );
}
//------------------------------------------------------------------------------------------------------------------
bool ColorLab::operator!= (const ColorLab& rqColor) const
{
    return ( m_afTuple[0] != rqColor.m_afTuple[0]
        ||   m_afTuple[1] != rqColor.m_afTuple[1]
        ||   m_afTuple[2] != rqColor.m_afTuple[2] );
}
//------------------------------------------------------------------------------------------------------------------
bool ColorLab::operator< (const ColorLab& rqColor) const
{
    return ( MemCmp(m_afTuple,rqColor.m_afTuple,12) < 0 );
}
//------------------------------------------------------------------------------------------------------------------
bool ColorLab::operator<= (const ColorLab& rqColor) const
{
    return ( MemCmp(m_afTuple,rqColor.m_afTuple,12) <= 0 );
}
//------------------------------------------------------------------------------------------------------------------
bool ColorLab::operator> (const ColorLab& rqColor) const
{
    return ( MemCmp(m_afTuple,rqColor.m_afTuple,12) > 0 );
}
//------------------------------------------------------------------------------------------------------------------
bool ColorLab::operator>= (const ColorLab& rqColor) const
{
    return ( MemCmp(m_afTuple,rqColor.m_afTuple,12) >= 0 );
}
//------------------------------------------------------------------------------------------------------------------
ColorLab ColorLab::operator+ (const ColorLab& rqColor) const
{
    return ColorLab(
        m_afTuple[0] + rqColor.m_afTuple[0],
        m_afTuple[1] + rqColor.m_afTuple[1],
        m_afTuple[2] + rqColor.m_afTuple[2]);
}
//------------------------------------------------------------------------------------------------------------------
ColorLab ColorLab::operator- (const ColorLab& rqColor) const
{
    return ColorLab(
        m_afTuple[0] - rqColor.m_afTuple[0],
        m_afTuple[1] - rqColor.m_afTuple[1],
        m_afTuple[2] - rqColor.m_afTuple[2]);
}
//------------------------------------------------------------------------------------------------------------------
ColorLab ColorLab::operator* (const ColorLab& rqColor) const
{
    return ColorLab(
        m_afTuple[0]*rqColor.m_afTuple[0],
        m_afTuple[1]*rqColor.m_afTuple[1],
        m_afTuple[2]*rqColor.m_afTuple[2]);
}
//------------------------------------------------------------------------------------------------------------------
ColorLab ColorLab::operator* (float fScalar) const
{
    return ColorLab(
        fScalar*m_afTuple[0],
        fScalar*m_afTuple[1],
        fScalar*m_afTuple[2]);
}
//------------------------------------------------------------------------------------------------------------------
ColorLab& ColorLab::operator+= (const ColorLab& rqColor)
{
    m_afTuple[0] += rqColor.m_afTuple[0];
    m_afTuple[1] += rqColor.m_afTuple[1];
    m_afTuple[2] += rqColor.m_afTuple[2];
    return *this;
}
//------------------------------------------------------------------------------------------------------------------
ColorLab& ColorLab::operator-= (const ColorLab& rqColor)
{
    m_afTuple[0] -= rqColor.m_afTuple[0];
    m_afTuple[1] -= rqColor.m_afTuple[1];
    m_afTuple[2] -= rqColor.m_afTuple[2];
    return *this;
}
//------------------------------------------------------------------------------------------------------------------
ColorLab& ColorLab::operator*= (const ColorLab& rqColor)
{
    m_afTuple[0] *= rqColor.m_afTuple[0];
    m_afTuple[1] *= rqColor.m_afTuple[1];
    m_afTuple[2] *= rqColor.m_afTuple[2];
    return *this;
}
//------------------------------------------------------------------------------------------------------------------
ColorLab& ColorLab::operator*= (float fScalar)
{
    m_afTuple[0] *= fScalar;
    m_afTuple[1] *= fScalar;
    m_afTuple[2] *= fScalar;
    return *this;
}
//------------------------------------------------------------------------------------------------------------------
void ColorLab::Clamp ()
{
    if ( m_afTuple[0] > 100.0f )
        m_afTuple[0] = 100.0f;
    else if ( m_afTuple[0] < 0.0f )
        m_afTuple[0] = 0.0f;
}
//------------------------------------------------------------------------------------------------------------------



