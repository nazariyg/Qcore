#include "QColorXyz.h"
using namespace Q;

//------------------------------------------------------------------------------------------------------------------
ColorXyz::ColorXyz ()
{
}
//------------------------------------------------------------------------------------------------------------------
ColorXyz::ColorXyz (float fX, float fY, float fZ)
{
    m_afTuple[0] = fX;
    m_afTuple[1] = fY;
    m_afTuple[2] = fZ;
}
//------------------------------------------------------------------------------------------------------------------
ColorXyz::ColorXyz (const ColorXyz& rqColor)
{
    m_afTuple[0] = rqColor.m_afTuple[0];
    m_afTuple[1] = rqColor.m_afTuple[1];
    m_afTuple[2] = rqColor.m_afTuple[2];
}
//------------------------------------------------------------------------------------------------------------------
bool ColorXyz::operator== (const ColorXyz& rqColor) const
{
    return ( m_afTuple[0] == rqColor.m_afTuple[0]
        &&   m_afTuple[1] == rqColor.m_afTuple[1]
        &&   m_afTuple[2] == rqColor.m_afTuple[2] );
}
//------------------------------------------------------------------------------------------------------------------
bool ColorXyz::operator!= (const ColorXyz& rqColor) const
{
    return ( m_afTuple[0] != rqColor.m_afTuple[0]
        ||   m_afTuple[1] != rqColor.m_afTuple[1]
        ||   m_afTuple[2] != rqColor.m_afTuple[2] );
}
//------------------------------------------------------------------------------------------------------------------
bool ColorXyz::operator< (const ColorXyz& rqColor) const
{
    return ( MemCmp(m_afTuple,rqColor.m_afTuple,12) < 0 );
}
//------------------------------------------------------------------------------------------------------------------
bool ColorXyz::operator<= (const ColorXyz& rqColor) const
{
    return ( MemCmp(m_afTuple,rqColor.m_afTuple,12) <= 0 );
}
//------------------------------------------------------------------------------------------------------------------
bool ColorXyz::operator> (const ColorXyz& rqColor) const
{
    return ( MemCmp(m_afTuple,rqColor.m_afTuple,12) > 0 );
}
//------------------------------------------------------------------------------------------------------------------
bool ColorXyz::operator>= (const ColorXyz& rqColor) const
{
    return ( MemCmp(m_afTuple,rqColor.m_afTuple,12) >= 0 );
}
//------------------------------------------------------------------------------------------------------------------
ColorXyz ColorXyz::operator+ (const ColorXyz& rqColor) const
{
    return ColorXyz(
        m_afTuple[0] + rqColor.m_afTuple[0],
        m_afTuple[1] + rqColor.m_afTuple[1],
        m_afTuple[2] + rqColor.m_afTuple[2]);
}
//------------------------------------------------------------------------------------------------------------------
ColorXyz ColorXyz::operator- (const ColorXyz& rqColor) const
{
    return ColorXyz(
        m_afTuple[0] - rqColor.m_afTuple[0],
        m_afTuple[1] - rqColor.m_afTuple[1],
        m_afTuple[2] - rqColor.m_afTuple[2]);
}
//------------------------------------------------------------------------------------------------------------------
ColorXyz ColorXyz::operator* (const ColorXyz& rqColor) const
{
    return ColorXyz(
        m_afTuple[0]*rqColor.m_afTuple[0],
        m_afTuple[1]*rqColor.m_afTuple[1],
        m_afTuple[2]*rqColor.m_afTuple[2]);
}
//------------------------------------------------------------------------------------------------------------------
ColorXyz ColorXyz::operator* (float fScalar) const
{
    return ColorXyz(
        fScalar*m_afTuple[0],
        fScalar*m_afTuple[1],
        fScalar*m_afTuple[2]);
}
//------------------------------------------------------------------------------------------------------------------
ColorXyz& ColorXyz::operator+= (const ColorXyz& rqColor)
{
    m_afTuple[0] += rqColor.m_afTuple[0];
    m_afTuple[1] += rqColor.m_afTuple[1];
    m_afTuple[2] += rqColor.m_afTuple[2];
    return *this;
}
//------------------------------------------------------------------------------------------------------------------
ColorXyz& ColorXyz::operator-= (const ColorXyz& rqColor)
{
    m_afTuple[0] -= rqColor.m_afTuple[0];
    m_afTuple[1] -= rqColor.m_afTuple[1];
    m_afTuple[2] -= rqColor.m_afTuple[2];
    return *this;
}
//------------------------------------------------------------------------------------------------------------------
ColorXyz& ColorXyz::operator*= (const ColorXyz& rqColor)
{
    m_afTuple[0] *= rqColor.m_afTuple[0];
    m_afTuple[1] *= rqColor.m_afTuple[1];
    m_afTuple[2] *= rqColor.m_afTuple[2];
    return *this;
}
//------------------------------------------------------------------------------------------------------------------
ColorXyz& ColorXyz::operator*= (float fScalar)
{
    m_afTuple[0] *= fScalar;
    m_afTuple[1] *= fScalar;
    m_afTuple[2] *= fScalar;
    return *this;
}
//------------------------------------------------------------------------------------------------------------------
void ColorXyz::Clamp ()
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



