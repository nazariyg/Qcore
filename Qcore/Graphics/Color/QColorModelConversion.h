#ifndef QCOLORMODELCONVERSION_H
#define QCOLORMODELCONVERSION_H

#include "QColorRgb.h"
#include "QColorCmyk.h"
#include "QColorYcc.h"
#include "QColorXyz.h"
#include "QColorLab.h"
#include "QMatrix3.h"

namespace Q
{

class Q_ITEM ColorModelConversion
{
public:
    // CMYK related.  NOTE. The GetConvertedToCmyk function is rather mapping than conversion.
    static ColorCmyk GetConvertedToCmyk (const ColorRgb& rqColorRgb, bool bClampResult = true);
    static ColorRgb GetConvertedToRgb (const ColorCmyk& rqColorCmyk, bool bClampResult = true);

    // YCbCr related.  NOTE. In YCbCr color representation, Y is in [0.0, 1.0] but Cb and Cr are in [-0.5, 0.5].
    static ColorYcc GetConvertedToYcc (const ColorRgb& rqColorRgb, bool bClampResult = true);
    static ColorRgb GetConvertedToRgb (const ColorYcc& rqColorYcc, bool bClampResult = true);

    // CIE XYZ related.  Custom matrices can be generated for these conversions.
    static ColorXyz GetConvertedToXyz (const ColorRgb& rqColorRgb, bool bClampResult = true,
        float fGamma = RgbToXyzDefaultGamma, const Matrix3f& rqMatrix = RgbToXyzDefaultMatrix);
    static ColorRgb GetConvertedToRgb (const ColorXyz& rqColorXyz, bool bClampResult = true,
        float fGamma = XyzToRgbDefaultGamma, const Matrix3f& rqMatrix = XyzToRgbDefaultMatrix);

    // CIE L*a*b* related.  Reference white point representation is the same as for the functions related to
    // XYZ color model.  Custom matrices can be generated for these conversions.
    static ColorLab GetConvertedToLab (const ColorRgb& rqColorRgb, bool bClampResult = true,
        float fGamma = RgbToXyzDefaultGamma, const Matrix3f& rqMatrix = RgbToXyzDefaultMatrix,
        float fXw = DefaultXw, float fYw = DefaultYw);
    static ColorRgb GetConvertedToRgb (const ColorLab& rqColorLab, bool bClampResult = true,
        float fGamma = XyzToRgbDefaultGamma, const Matrix3f& rqMatrix = XyzToRgbDefaultMatrix,
        float fXw = DefaultXw, float fYw = DefaultYw);

    // Getting of custom RGB/XYZ (and RGB/L*a*b*) conversion matrices.  The parameters are primary
    // chromaticities and reference white point in the proportional form of representation (i.e. little x and y
    // symbols in notation).
    static Matrix3f GenerateRgbToXyzMatrix (
        float fXr, float fYr,
        float fXg, float fYg,
        float fXb, float fYb,
        float fXw, float fYw);
    static Matrix3f GenerateXyzToRgbMatrix (
        float fXr, float fYr,
        float fXg, float fYg,
        float fXb, float fYb,
        float fXw, float fYw);

    // support for RGB-to-YCbCr conversion
    static const float RgbToYccRInY;              //  0.299000f
    static const float RgbToYccGInY;              //  0.587000f
    static const float RgbToYccBInY;              //  0.114000f
    static const float RgbToYccRInCb;             // -0.168736f
    static const float RgbToYccGInCb;             // -0.331264f
    static const float RgbToYccBInCb;             //  0.500000f
    static const float RgbToYccRInCr;             //  0.500000f
    static const float RgbToYccGInCr;             // -0.418688f
    static const float RgbToYccBInCr;             // -0.081312f

    // support for YCbCr-to-RGB conversion
    static const float YccToRgbCrInR;             //  1.402000f
    static const float YccToRgbCbInG;             // -0.344136f
    static const float YccToRgbCrInG;             // -0.714136f
    static const float YccToRgbCbInB;             //  1.772000f

    // Support for XYZ related conversions.  Matrices: sRGB, reference white point D65 (CIE 1931).
    static const float RgbToXyzDefaultGamma;      // 2.2
    static const float XyzToRgbDefaultGamma;      // 0.454545
    static const Matrix3f RgbToXyzDefaultMatrix;  // +-                             -+
                                                  // |  0.412424  0.357579  0.180464 |
                                                  // |  0.212656  0.715158  0.072186 |
                                                  // |  0.019332  0.119193  0.950444 |
                                                  // +-                             -+
    static const Matrix3f XyzToRgbDefaultMatrix;  // +-                             -+
                                                  // |  3.240710 -1.537260 -0.498571 |
                                                  // | -0.969258  1.875990  0.041556 |
                                                  // |  0.055635 -0.203996  1.057070 |
                                                  // +-                             -+

    // Support for L*a*b* related conversions.  Reference white point: D65 (CIE 1931).
    static const float DefaultXw;                 // 0.31273
    static const float DefaultYw;                 // 0.32902

private:
    static float GetPowered (float fBase, float fExponent);
};

}

#endif



