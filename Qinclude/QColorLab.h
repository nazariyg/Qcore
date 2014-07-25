#ifndef QCOLORLAB_H
#define QCOLORLAB_H

#include "QSystem.h"

namespace Q
{

class Q_ITEM ColorLab
{
public:
    // construction
    ColorLab ();  // uninitialized
    ColorLab (float fL, float fA, float fB);
    ColorLab (const ColorLab& rqColor);

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

    // comparison
    bool operator== (const ColorLab& rqColor) const;
    bool operator!= (const ColorLab& rqColor) const;
    bool operator<  (const ColorLab& rqColor) const;
    bool operator<= (const ColorLab& rqColor) const;
    bool operator>  (const ColorLab& rqColor) const;
    bool operator>= (const ColorLab& rqColor) const;

    // arithmetic operations
    ColorLab operator+ (const ColorLab& rqColor) const;
    ColorLab operator- (const ColorLab& rqColor) const;
    ColorLab operator* (const ColorLab& rqColor) const;
    ColorLab operator* (float fScalar) const;

    // arithmetic updates
    ColorLab& operator+= (const ColorLab& rqColor);
    ColorLab& operator-= (const ColorLab& rqColor);
    ColorLab& operator*= (const ColorLab& rqColor);
    ColorLab& operator*= (float fScalar);

    // transformation of L* component to [0.0, 100.0]
    void Clamp ();

private:
    float m_afTuple[3];
};

#include "QColorLab.inl"

}

#endif



