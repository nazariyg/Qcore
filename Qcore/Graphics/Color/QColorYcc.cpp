#include "QColorYcc.h"
using namespace Q;

//------------------------------------------------------------------------------------------------------------------
ColorYcc::ColorYcc ()
{
}
//------------------------------------------------------------------------------------------------------------------
ColorYcc::ColorYcc (float fY, float fCb, float fCr)
{
    m_afTuple[0] = fY;
    m_afTuple[1] = fCb;
    m_afTuple[2] = fCr;
}
//------------------------------------------------------------------------------------------------------------------
ColorYcc::ColorYcc (const ColorYcc& rqColor)
{
    m_afTuple[0] = rqColor.m_afTuple[0];
    m_afTuple[1] = rqColor.m_afTuple[1];
    m_afTuple[2] = rqColor.m_afTuple[2];
}
//------------------------------------------------------------------------------------------------------------------
bool ColorYcc::operator== (const ColorYcc& rqColor) const
{
    return ( m_afTuple[0] == rqColor.m_afTuple[0]
        &&   m_afTuple[1] == rqColor.m_afTuple[1]
        &&   m_afTuple[2] == rqColor.m_afTuple[2] );
}
//------------------------------------------------------------------------------------------------------------------
bool ColorYcc::operator!= (const ColorYcc& rqColor) const
{
    return ( m_afTuple[0] != rqColor.m_afTuple[0]
        ||   m_afTuple[1] != rqColor.m_afTuple[1]
        ||   m_afTuple[2] != rqColor.m_afTuple[2] );
}
//------------------------------------------------------------------------------------------------------------------
bool ColorYcc::operator< (const ColorYcc& rqColor) const
{
    return ( MemCmp(m_afTuple,rqColor.m_afTuple,12) < 0 );
}
//------------------------------------------------------------------------------------------------------------------
bool ColorYcc::operator<= (const ColorYcc& rqColor) const
{
    return ( MemCmp(m_afTuple,rqColor.m_afTuple,12) <= 0 );
}
//------------------------------------------------------------------------------------------------------------------
bool ColorYcc::operator> (const ColorYcc& rqColor) const
{
    return ( MemCmp(m_afTuple,rqColor.m_afTuple,12) > 0 );
}
//------------------------------------------------------------------------------------------------------------------
bool ColorYcc::operator>= (const ColorYcc& rqColor) const
{
    return ( MemCmp(m_afTuple,rqColor.m_afTuple,12) >= 0 );
}
//------------------------------------------------------------------------------------------------------------------
ColorYcc ColorYcc::operator+ (const ColorYcc& rqColor) const
{
    return ColorYcc(
        m_afTuple[0] + rqColor.m_afTuple[0],
        m_afTuple[1] + rqColor.m_afTuple[1],
        m_afTuple[2] + rqColor.m_afTuple[2]);
}
//------------------------------------------------------------------------------------------------------------------
ColorYcc ColorYcc::operator- (const ColorYcc& rqColor) const
{
    return ColorYcc(
        m_afTuple[0] - rqColor.m_afTuple[0],
        m_afTuple[1] - rqColor.m_afTuple[1],
        m_afTuple[2] - rqColor.m_afTuple[2]);
}
//------------------------------------------------------------------------------------------------------------------
ColorYcc ColorYcc::operator* (const ColorYcc& rqColor) const
{
    return ColorYcc(
        m_afTuple[0]*rqColor.m_afTuple[0],
        m_afTuple[1]*rqColor.m_afTuple[1],
        m_afTuple[2]*rqColor.m_afTuple[2]);
}
//------------------------------------------------------------------------------------------------------------------
ColorYcc ColorYcc::operator* (float fScalar) const
{
    return ColorYcc(
        fScalar*m_afTuple[0],
        fScalar*m_afTuple[1],
        fScalar*m_afTuple[2]);
}
//------------------------------------------------------------------------------------------------------------------
ColorYcc& ColorYcc::operator+= (const ColorYcc& rqColor)
{
    m_afTuple[0] += rqColor.m_afTuple[0];
    m_afTuple[1] += rqColor.m_afTuple[1];
    m_afTuple[2] += rqColor.m_afTuple[2];
    return *this;
}
//------------------------------------------------------------------------------------------------------------------
ColorYcc& ColorYcc::operator-= (const ColorYcc& rqColor)
{
    m_afTuple[0] -= rqColor.m_afTuple[0];
    m_afTuple[1] -= rqColor.m_afTuple[1];
    m_afTuple[2] -= rqColor.m_afTuple[2];
    return *this;
}
//------------------------------------------------------------------------------------------------------------------
ColorYcc& ColorYcc::operator*= (const ColorYcc& rqColor)
{
    m_afTuple[0] *= rqColor.m_afTuple[0];
    m_afTuple[1] *= rqColor.m_afTuple[1];
    m_afTuple[2] *= rqColor.m_afTuple[2];
    return *this;
}
//------------------------------------------------------------------------------------------------------------------
ColorYcc& ColorYcc::operator*= (float fScalar)
{
    m_afTuple[0] *= fScalar;
    m_afTuple[1] *= fScalar;
    m_afTuple[2] *= fScalar;
    return *this;
}
//------------------------------------------------------------------------------------------------------------------
void ColorYcc::Clamp ()
{
    if ( m_afTuple[0] > 1.0f )
        m_afTuple[0] = 1.0f;
    else if ( m_afTuple[0] < 0.0f )
        m_afTuple[0] = 0.0f;
    for (int i = 1; i < 3; i++)
    {
        if ( m_afTuple[i] > 0.5f )
            m_afTuple[i] = 0.5f;
        else if ( m_afTuple[i] < -0.5f )
            m_afTuple[i] = -0.5f;
    }
}
//------------------------------------------------------------------------------------------------------------------



