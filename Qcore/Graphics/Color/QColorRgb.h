#ifndef QCOLORRGB_H
#define QCOLORRGB_H

#include "QSystem.h"

namespace Q
{

class Q_ITEM ColorRgb
{
public:
    // construction
    ColorRgb ();  // uninitialized
    ColorRgb (float fR, float fG, float fB);
    ColorRgb (const ColorRgb& rqColor);

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

    // comparison
    bool operator== (const ColorRgb& rqColor) const;
    bool operator!= (const ColorRgb& rqColor) const;
    bool operator<  (const ColorRgb& rqColor) const;
    bool operator<= (const ColorRgb& rqColor) const;
    bool operator>  (const ColorRgb& rqColor) const;
    bool operator>= (const ColorRgb& rqColor) const;

    // arithmetic operations
    ColorRgb operator+ (const ColorRgb& rqColor) const;
    ColorRgb operator- (const ColorRgb& rqColor) const;
    ColorRgb operator* (const ColorRgb& rqColor) const;
    ColorRgb operator* (float fScalar) const;

    // arithmetic updates
    ColorRgb& operator+= (const ColorRgb& rqColor);
    ColorRgb& operator-= (const ColorRgb& rqColor);
    ColorRgb& operator*= (const ColorRgb& rqColor);
    ColorRgb& operator*= (float fScalar);

    // Transformation of the color components to [0.0, 1.0].  ScaleByMax function assumes the color components
    // are nonnegative, finds the maximum color component, and divides all components by that value.
    void Clamp ();
    void ScaleByMax ();

    static const ColorRgb Black;
    static const ColorRgb White;

private:
    float m_afTuple[3];
};

#include "QColorRgb.inl"

}

#endif



