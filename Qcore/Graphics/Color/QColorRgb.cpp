#include "QColorRgb.h"
using namespace Q;

const ColorRgb ColorRgb::Black(0.0f,0.0f,0.0f);
const ColorRgb ColorRgb::White(1.0f,1.0f,1.0f);

//------------------------------------------------------------------------------------------------------------------
ColorRgb::ColorRgb ()
{
}
//------------------------------------------------------------------------------------------------------------------
ColorRgb::ColorRgb (float fR, float fG, float fB)
{
    m_afTuple[0] = fR;
    m_afTuple[1] = fG;
    m_afTuple[2] = fB;
}
//------------------------------------------------------------------------------------------------------------------
ColorRgb::ColorRgb (const ColorRgb& rqColor)
{
    m_afTuple[0] = rqColor.m_afTuple[0];
    m_afTuple[1] = rqColor.m_afTuple[1];
    m_afTuple[2] = rqColor.m_afTuple[2];
}
//------------------------------------------------------------------------------------------------------------------
bool ColorRgb::operator== (const ColorRgb& rqColor) const
{
    return ( m_afTuple[0] == rqColor.m_afTuple[0]
        &&   m_afTuple[1] == rqColor.m_afTuple[1]
        &&   m_afTuple[2] == rqColor.m_afTuple[2] );
}
//------------------------------------------------------------------------------------------------------------------
bool ColorRgb::operator!= (const ColorRgb& rqColor) const
{
    return ( m_afTuple[0] != rqColor.m_afTuple[0]
        ||   m_afTuple[1] != rqColor.m_afTuple[1]
        ||   m_afTuple[2] != rqColor.m_afTuple[2] );
}
//------------------------------------------------------------------------------------------------------------------
bool ColorRgb::operator< (const ColorRgb& rqColor) const
{
    return ( MemCmp(m_afTuple,rqColor.m_afTuple,12) < 0 );
}
//------------------------------------------------------------------------------------------------------------------
bool ColorRgb::operator<= (const ColorRgb& rqColor) const
{
    return ( MemCmp(m_afTuple,rqColor.m_afTuple,12) <= 0 );
}
//------------------------------------------------------------------------------------------------------------------
bool ColorRgb::operator> (const ColorRgb& rqColor) const
{
    return ( MemCmp(m_afTuple,rqColor.m_afTuple,12) > 0 );
}
//------------------------------------------------------------------------------------------------------------------
bool ColorRgb::operator>= (const ColorRgb& rqColor) const
{
    return ( MemCmp(m_afTuple,rqColor.m_afTuple,12) >= 0 );
}
//------------------------------------------------------------------------------------------------------------------
ColorRgb ColorRgb::operator+ (const ColorRgb& rqColor) const
{
    return ColorRgb(
        m_afTuple[0] + rqColor.m_afTuple[0],
        m_afTuple[1] + rqColor.m_afTuple[1],
        m_afTuple[2] + rqColor.m_afTuple[2]);
}
//------------------------------------------------------------------------------------------------------------------
ColorRgb ColorRgb::operator- (const ColorRgb& rqColor) const
{
    return ColorRgb(
        m_afTuple[0] - rqColor.m_afTuple[0],
        m_afTuple[1] - rqColor.m_afTuple[1],
        m_afTuple[2] - rqColor.m_afTuple[2]);
}
//------------------------------------------------------------------------------------------------------------------
ColorRgb ColorRgb::operator* (const ColorRgb& rqColor) const
{
    return ColorRgb(
        m_afTuple[0]*rqColor.m_afTuple[0],
        m_afTuple[1]*rqColor.m_afTuple[1],
        m_afTuple[2]*rqColor.m_afTuple[2]);
}
//------------------------------------------------------------------------------------------------------------------
ColorRgb ColorRgb::operator* (float fScalar) const
{
    return ColorRgb(
        fScalar*m_afTuple[0],
        fScalar*m_afTuple[1],
        fScalar*m_afTuple[2]);
}
//------------------------------------------------------------------------------------------------------------------
ColorRgb& ColorRgb::operator+= (const ColorRgb& rqColor)
{
    m_afTuple[0] += rqColor.m_afTuple[0];
    m_afTuple[1] += rqColor.m_afTuple[1];
    m_afTuple[2] += rqColor.m_afTuple[2];
    return *this;
}
//------------------------------------------------------------------------------------------------------------------
ColorRgb& ColorRgb::operator-= (const ColorRgb& rqColor)
{
    m_afTuple[0] -= rqColor.m_afTuple[0];
    m_afTuple[1] -= rqColor.m_afTuple[1];
    m_afTuple[2] -= rqColor.m_afTuple[2];
    return *this;
}
//------------------------------------------------------------------------------------------------------------------
ColorRgb& ColorRgb::operator*= (const ColorRgb& rqColor)
{
    m_afTuple[0] *= rqColor.m_afTuple[0];
    m_afTuple[1] *= rqColor.m_afTuple[1];
    m_afTuple[2] *= rqColor.m_afTuple[2];
    return *this;
}
//------------------------------------------------------------------------------------------------------------------
ColorRgb& ColorRgb::operator*= (float fScalar)
{
    m_afTuple[0] *= fScalar;
    m_afTuple[1] *= fScalar;
    m_afTuple[2] *= fScalar;
    return *this;
}
//------------------------------------------------------------------------------------------------------------------
void ColorRgb::Clamp ()
{
    for (int i = 0; i < 3; i++)
    {
        if ( m_afTuple[i] > 1.0f )
            m_afTuple[i] = 1.0f;
        else if ( m_afTuple[i] < 0.0f )
            m_afTuple[i] = 0.0f;
    }
}
//------------------------------------------------------------------------------------------------------------------
void ColorRgb::ScaleByMax ()
{
    float fMax = m_afTuple[0];
    if ( m_afTuple[1] > fMax )
        fMax = m_afTuple[1];
    if ( m_afTuple[2] > fMax )
        fMax = m_afTuple[2];
    if ( fMax > 1.0f )
    {
        float fInvMax = 1.0f/fMax;
        for (int i = 0; i < 3; i++)
            m_afTuple[i] *= fInvMax;
    }
}
//------------------------------------------------------------------------------------------------------------------



