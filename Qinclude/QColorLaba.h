#ifndef QCOLORLABA_H
#define QCOLORLABA_H

#include "QSystem.h"

namespace Q
{

class Q_ITEM ColorLaba
{
public:
    // construction
    ColorLaba ();  // uninitialized
    ColorLaba (float fL, float fA, float fB, float fAlpha);
    ColorLaba (const ColorLaba& rqColor);

    // component access
    operator float* ();
    operator const float* () const;
    float& operator[] (int i);
    float operator[] (int i) const;
    float L () const;
    float& L ();
    float A () const;
    float& A ();
    float B () const;
    float& B ();
    float Alpha () const;
    float& Alpha ();

    // comparison
    bool operator== (const ColorLaba& rqColor) const;
    bool operator!= (const ColorLaba& rqColor) const;
    bool operator<  (const ColorLaba& rqColor) const;
    bool operator<= (const ColorLaba& rqColor) const;
    bool operator>  (const ColorLaba& rqColor) const;
    bool operator>= (const ColorLaba& rqColor) const;

    // arithmetic operations
    ColorLaba operator+ (const ColorLaba& rqColor) const;
    ColorLaba operator- (const ColorLaba& rqColor) const;
    ColorLaba operator* (const ColorLaba& rqColor) const;
    ColorLaba operator* (float fScalar) const;

    // arithmetic updates
    ColorLaba& operator+= (const ColorLaba& rqColor);
    ColorLaba& operator-= (const ColorLaba& rqColor);
    ColorLaba& operator*= (const ColorLaba& rqColor);
    ColorLaba& operator*= (float fScalar);

    // transformation of L* component to [0.0, 100.0] and the alpha component to [0.0, 1.0]
    void Clamp ();

private:
    float m_afTuple[4];
};

#include "QColorLaba.inl"

}

#endif



