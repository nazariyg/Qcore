#ifndef QCOLORRGBA_H
#define QCOLORRGBA_H

#include "QSystem.h"

namespace Q
{

class Q_ITEM ColorRgba
{
public:
    // construction
    ColorRgba ();  // uninitialized
    ColorRgba (float fR, float fG, float fB, float fA);
    ColorRgba (const ColorRgba& rqColor);

    // component access
    operator float* ();
    operator const float* () const;
    float& operator[] (int i);
    float operator[] (int i) const;
    float R () const;
    float& R ();
    float G () const;
    float& G ();
    float B () const;
    float& B ();
    float A () const;
    float& A ();

    // comparison
    bool operator== (const ColorRgba& rqColor) const;
    bool operator!= (const ColorRgba& rqColor) const;
    bool operator<  (const ColorRgba& rqColor) const;
    bool operator<= (const ColorRgba& rqColor) const;
    bool operator>  (const ColorRgba& rqColor) const;
    bool operator>= (const ColorRgba& rqColor) const;

    // arithmetic operations
    ColorRgba operator+ (const ColorRgba& rqColor) const;
    ColorRgba operator- (const ColorRgba& rqColor) const;
    ColorRgba operator* (const ColorRgba& rqColor) const;
    ColorRgba operator* (float fScalar) const;

    // arithmetic updates
    ColorRgba& operator+= (const ColorRgba& rqColor);
    ColorRgba& operator-= (const ColorRgba& rqColor);
    ColorRgba& operator*= (const ColorRgba& rqColor);
    ColorRgba& operator*= (float fScalar);

    // Transformation of the color components to [0.0, 1.0].  ScaleByMax function assumes the color components
    // are nonnegative, finds the maximum RGB component, and divides all RGB components by that value.  The
    // alpha component is clamped to [0.0, 1.0] with the both functions.
    void Clamp ();
    void ScaleByMax ();

    static const ColorRgba Black;
    static const ColorRgba White;

private:
    float m_afTuple[4];
};

#include "QColorRgba.inl"

}

#endif



