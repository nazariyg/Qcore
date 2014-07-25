#include "QColorRgba.h"
using namespace Q;

const ColorRgba ColorRgba::Black(0.0f,0.0f,0.0f,1.0f);
const ColorRgba ColorRgba::White(1.0f,1.0f,1.0f,1.0f);

//------------------------------------------------------------------------------------------------------------------
ColorRgba::ColorRgba ()
{
}
//------------------------------------------------------------------------------------------------------------------
ColorRgba::ColorRgba (float fR, float fG, float fB, float fA)
{
    m_afTuple[0] = fR;
    m_afTuple[1] = fG;
    m_afTuple[2] = fB;
    m_afTuple[3] = fA;
}
//------------------------------------------------------------------------------------------------------------------
ColorRgba::ColorRgba (const ColorRgba& rqColor)
{
    m_afTuple[0] = rqColor.m_afTuple[0];
    m_afTuple[1] = rqColor.m_afTuple[1];
    m_afTuple[2] = rqColor.m_afTuple[2];
    m_afTuple[3] = rqColor.m_afTuple[3];
}
//------------------------------------------------------------------------------------------------------------------
bool ColorRgba::operator== (const ColorRgba& rqColor) const
{
    return ( m_afTuple[0] == rqColor.m_afTuple[0]
        &&   m_afTuple[1] == rqColor.m_afTuple[1]
        &&   m_afTuple[2] == rqColor.m_afTuple[2]
        &&   m_afTuple[3] == rqColor.m_afTuple[3] );
}
//------------------------------------------------------------------------------------------------------------------
bool ColorRgba::operator!= (const ColorRgba& rqColor) const
{
    return ( m_afTuple[0] != rqColor.m_afTuple[0]
        ||   m_afTuple[1] != rqColor.m_afTuple[1]
        ||   m_afTuple[2] != rqColor.m_afTuple[2]
        ||   m_afTuple[3] != rqColor.m_afTuple[3] );
}
//------------------------------------------------------------------------------------------------------------------
bool ColorRgba::operator< (const ColorRgba& rqColor) const
{
    return ( MemCmp(m_afTuple,rqColor.m_afTuple,16) < 0 );
}
//------------------------------------------------------------------------------------------------------------------
bool ColorRgba::operator<= (const ColorRgba& rqColor) const
{
    return ( MemCmp(m_afTuple,rqColor.m_afTuple,16) <= 0 );
}
//------------------------------------------------------------------------------------------------------------------
bool ColorRgba::operator> (const ColorRgba& rqColor) const
{
    return ( MemCmp(m_afTuple,rqColor.m_afTuple,16) > 0 );
}
//------------------------------------------------------------------------------------------------------------------
bool ColorRgba::operator>= (const ColorRgba& rqColor) const
{
    return ( MemCmp(m_afTuple,rqColor.m_afTuple,16) >= 0 );
}
//------------------------------------------------------------------------------------------------------------------
ColorRgba ColorRgba::operator+ (const ColorRgba& rqColor) const
{
    return ColorRgba(
        m_afTuple[0] + rqColor.m_afTuple[0],
        m_afTuple[1] + rqColor.m_afTuple[1],
        m_afTuple[2] + rqColor.m_afTuple[2],
        m_afTuple[3] + rqColor.m_afTuple[3]);
}
//------------------------------------------------------------------------------------------------------------------
ColorRgba ColorRgba::operator- (const ColorRgba& rqColor) const
{
    return ColorRgba(
        m_afTuple[0] - rqColor.m_afTuple[0],
        m_afTuple[1] - rqColor.m_afTuple[1],
        m_afTuple[2] - rqColor.m_afTuple[2],
        m_afTuple[3] - rqColor.m_afTuple[3]);
}
//------------------------------------------------------------------------------------------------------------------
ColorRgba ColorRgba::operator* (const ColorRgba& rqColor) const
{
    return ColorRgba(
        m_afTuple[0]*rqColor.m_afTuple[0],
        m_afTuple[1]*rqColor.m_afTuple[1],
        m_afTuple[2]*rqColor.m_afTuple[2],
        m_afTuple[3]*rqColor.m_afTuple[3]);
}
//------------------------------------------------------------------------------------------------------------------
ColorRgba ColorRgba::operator* (float fScalar) const
{
    return ColorRgba(
        fScalar*m_afTuple[0],
        fScalar*m_afTuple[1],
        fScalar*m_afTuple[2],
        fScalar*m_afTuple[3]);
}
//------------------------------------------------------------------------------------------------------------------
ColorRgba& ColorRgba::operator+= (const ColorRgba& rqColor)
{
    m_afTuple[0] += rqColor.m_afTuple[0];
    m_afTuple[1] += rqColor.m_afTuple[1];
    m_afTuple[2] += rqColor.m_afTuple[2];
    m_afTuple[3] += rqColor.m_afTuple[3];
    return *this;
}
//------------------------------------------------------------------------------------------------------------------
ColorRgba& ColorRgba::operator-= (const ColorRgba& rqColor)
{
    m_afTuple[0] -= rqColor.m_afTuple[0];
    m_afTuple[1] -= rqColor.m_afTuple[1];
    m_afTuple[2] -= rqColor.m_afTuple[2];
    m_afTuple[3] -= rqColor.m_afTuple[3];
    return *this;
}
//------------------------------------------------------------------------------------------------------------------
ColorRgba& ColorRgba::operator*= (const ColorRgba& rqColor)
{
    m_afTuple[0] *= rqColor.m_afTuple[0];
    m_afTuple[1] *= rqColor.m_afTuple[1];
    m_afTuple[2] *= rqColor.m_afTuple[2];
    m_afTuple[3] *= rqColor.m_afTuple[3];
    return *this;
}
//------------------------------------------------------------------------------------------------------------------
ColorRgba& ColorRgba::operator*= (float fScalar)
{
    m_afTuple[0] *= fScalar;
    m_afTuple[1] *= fScalar;
    m_afTuple[2] *= fScalar;
    m_afTuple[3] *= fScalar;
    return *this;
}
//------------------------------------------------------------------------------------------------------------------
void ColorRgba::Clamp ()
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
void ColorRgba::ScaleByMax ()
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

    // clamp alpha to [0.0, 1.0]
    if ( m_afTuple[3] > 1.0f )
        m_afTuple[3] = 1.0f;
    else if ( m_afTuple[3] < 0.0f )
        m_afTuple[3] = 0.0f;
}
//------------------------------------------------------------------------------------------------------------------



