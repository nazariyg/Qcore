#ifndef QPALETTECOLOR_H
#define QPALETTECOLOR_H

// The class serves to represent a color from an RGB or RGBA palette.

#include "QColorRgb.h"
#include "QColorRgba.h"

namespace Q
{

class Q_ITEM PaletteColor
{
public:
    // construction
    PaletteColor ();  // uninitialized
    PaletteColor (
        unsigned char ucRed,
        unsigned char ucGreen,
        unsigned char ucBlue,
        unsigned char ucAlpha = 255);  // fully opaque by default

    // component access
    unsigned char R () const;
    unsigned char& R ();
    unsigned char G () const;
    unsigned char& G ();
    unsigned char B () const;
    unsigned char& B ();
    unsigned char A () const;
    unsigned char& A ();

    // conversion into floating-point representation
    ColorRgb GetColorRgb () const;
    ColorRgba GetColorRgba () const;

    // comparison
    bool operator== (const PaletteColor& rqEntry) const;
    bool operator!= (const PaletteColor& rqEntry) const;
    bool operator<  (const PaletteColor& rqEntry) const;
    bool operator<= (const PaletteColor& rqEntry) const;
    bool operator>  (const PaletteColor& rqEntry) const;
    bool operator>= (const PaletteColor& rqEntry) const;

    static const PaletteColor Black;
    static const PaletteColor White;

    // the components are located in the next order: blue, green, red, alpha
    unsigned char Components[4];
};

#include "QPaletteColor.inl"

}

#endif



