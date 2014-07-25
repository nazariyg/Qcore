#ifndef QCOLORYCC_H
#define QCOLORYCC_H

#include "QSystem.h"

namespace Q
{

class Q_ITEM ColorYcc
{
public:
    // construction
    ColorYcc ();  // uninitialized
    ColorYcc (float fY, float fCb, float fCr);
    ColorYcc (const ColorYcc& rqColor);

    // component access
    operator float* ();
    operator const float* () const;
    float& operator[] (int i);
    float operator[] (int i) const;
    float Y () const;
    float& Y ();
    float Cb () const;
    float& Cb ();
    float Cr () const;
    float& Cr ();

    // comparison
    bool operator== (const ColorYcc& rqColor) const;
    bool operator!= (const ColorYcc& rqColor) const;
    bool operator<  (const ColorYcc& rqColor) const;
    bool operator<= (const ColorYcc& rqColor) const;
    bool operator>  (const ColorYcc& rqColor) const;
    bool operator>= (const ColorYcc& rqColor) const;

    // arithmetic operations
    ColorYcc operator+ (const ColorYcc& rqColor) const;
    ColorYcc operator- (const ColorYcc& rqColor) const;
    ColorYcc operator* (const ColorYcc& rqColor) const;
    ColorYcc operator* (float fScalar) const;

    // arithmetic updates
    ColorYcc& operator+= (const ColorYcc& rqColor);
    ColorYcc& operator-= (const ColorYcc& rqColor);
    ColorYcc& operator*= (const ColorYcc& rqColor);
    ColorYcc& operator*= (float fScalar);

    // transform Y component to [0.0, 1.0] and Cb with Cr to [-0.5, 0.5]
    void Clamp ();

private:
    float m_afTuple[3];
};

#include "QColorYcc.inl"

}

#endif



