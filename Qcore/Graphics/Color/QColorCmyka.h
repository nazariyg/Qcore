#ifndef QCOLORCMYKA_H
#define QCOLORCMYKA_H

#include "QSystem.h"

namespace Q
{

class Q_ITEM ColorCmyka
{
public:
    // construction
    ColorCmyka ();  // uninitialized
    ColorCmyka (float fC, float fM, float fY, float fK, float fAlpha);
    ColorCmyka (const ColorCmyka& rqColor);

    // component access
    operator float* ();
    operator const float* () const;
    float& operator[] (int i);
    float operator[] (int i) const;
    float C () const;
    float& C ();
    float M () const;
    float& M ();
    float Y () const;
    float& Y ();
    float K () const;
    float& K ();
    float Alpha () const;
    float& Alpha ();

    // comparison
    bool operator== (const ColorCmyka& rqColor) const;
    bool operator!= (const ColorCmyka& rqColor) const;
    bool operator<  (const ColorCmyka& rqColor) const;
    bool operator<= (const ColorCmyka& rqColor) const;
    bool operator>  (const ColorCmyka& rqColor) const;
    bool operator>= (const ColorCmyka& rqColor) const;

    // arithmetic operations
    ColorCmyka operator+ (const ColorCmyka& rqColor) const;
    ColorCmyka operator- (const ColorCmyka& rqColor) const;
    ColorCmyka operator* (const ColorCmyka& rqColor) const;
    ColorCmyka operator* (float fScalar) const;

    // arithmetic updates
    ColorCmyka& operator+= (const ColorCmyka& rqColor);
    ColorCmyka& operator-= (const ColorCmyka& rqColor);
    ColorCmyka& operator*= (const ColorCmyka& rqColor);
    ColorCmyka& operator*= (float fScalar);

    // transformation of the components to [0.0, 1.0]
    void Clamp ();

private:
    float m_afTuple[5];
};

#include "QColorCmyka.inl"

}

#endif



