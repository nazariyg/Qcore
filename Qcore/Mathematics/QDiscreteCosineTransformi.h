#ifndef QDISCRETECOSINETRANSFORMI_H
#define QDISCRETECOSINETRANSFORMI_H

#include "QSystem.h"

namespace Q
{

class Q_ITEM DiscreteCosineTransformi
{
public:
    // Two-dimensional DCT-II and DCT-III with orthogonal matrix up to a scale factor (i.e. with 1/sqrt(2) in
    // the formula).  Integer arithmetic algorithm is based on the method developed by Loeffler, Ligtenberg and
    // Moschytz.

    // forward
    static void Apply8x8ForwardOrthogonal2d (short* asCoefElement);

    // inverse; stores an output in acCoefElement (signed) previously clamping elements to the proper range
    static void Get8x8InverseOrthogonal2dWithDequantizationAndClamping (char* acCoefElement,
        const short* asFreqElement, const int* aiQuantTable);

private:
    static unsigned char GetClampedToUChar (int iValue);
};

#include "QDiscreteCosineTransformi.inl"

}

#endif



