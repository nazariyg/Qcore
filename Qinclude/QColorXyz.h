#ifndef QCOLORXYZ_H
#define QCOLORXYZ_H

#include "QSystem.h"

namespace Q
{

class Q_ITEM ColorXyz
{
public:
    // construction
    ColorXyz ();  // uninitialized
    ColorXyz (float fX, float fY, float fZ);
    ColorXyz (const ColorXyz& rqColor);

    // component access
    operator float* ();
    operator const float* () const;
    float& operator[] (int i);
    float operator[] (int i) const;
    float X () const;
    float& X ();
    float Y () const;
    float& Y ();
    float Z () const;
    float& Z ();

    // comparison
    bool operator== (const ColorXyz& rqColor) const;
    bool operator!= (const ColorXyz& rqColor) const;
    bool operator<  (const ColorXyz& rqColor) const;
    bool operator<= (const ColorXyz& rqColor) const;
    bool operator>  (const ColorXyz& rqColor) const;
    bool operator>= (const ColorXyz& rqColor) const;

    // arithmetic operations
    ColorXyz operator+ (const ColorXyz& rqColor) const;
    ColorXyz operator- (const ColorXyz& rqColor) const;
    ColorXyz operator* (const ColorXyz& rqColor) const;
    ColorXyz operator* (float fScalar) const;

    // arithmetic updates
    ColorXyz& operator+= (const ColorXyz& rqColor);
    ColorXyz& operator-= (const ColorXyz& rqColor);
    ColorXyz& operator*= (const ColorXyz& rqColor);
    ColorXyz& operator*= (float fScalar);

    // transform the color components to [0.0, 1.0]
    void Clamp ();

private:
    float m_afTuple[3];
};

#include "QColorXyz.inl"

}

#endif



