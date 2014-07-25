#ifndef QCOLORCMYK_H
#define QCOLORCMYK_H

#include "QSystem.h"

namespace Q
{

class Q_ITEM ColorCmyk
{
public:
    // construction
    ColorCmyk ();  // uninitialized
    ColorCmyk (float fC, float fM, float fY, float fK);
    ColorCmyk (const ColorCmyk& rqColor);

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

    // comparison
    bool operator== (const ColorCmyk& rqColor) const;
    bool operator!= (const ColorCmyk& rqColor) const;
    bool operator<  (const ColorCmyk& rqColor) const;
    bool operator<= (const ColorCmyk& rqColor) const;
    bool operator>  (const ColorCmyk& rqColor) const;
    bool operator>= (const ColorCmyk& rqColor) const;

    // arithmetic operations
    ColorCmyk operator+ (const ColorCmyk& rqColor) const;
    ColorCmyk operator- (const ColorCmyk& rqColor) const;
    ColorCmyk operator* (const ColorCmyk& rqColor) const;
    ColorCmyk operator* (float fScalar) const;

    // arithmetic updates
    ColorCmyk& operator+= (const ColorCmyk& rqColor);
    ColorCmyk& operator-= (const ColorCmyk& rqColor);
    ColorCmyk& operator*= (const ColorCmyk& rqColor);
    ColorCmyk& operator*= (float fScalar);

    // transformation of the components to [0.0, 1.0]
    void Clamp ();

private:
    float m_afTuple[4];
};

#include "QColorCmyk.inl"

}

#endif



