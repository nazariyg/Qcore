#ifndef QRASTERIMAGECONVERSION_H
#define QRASTERIMAGECONVERSION_H

// The class offers cross-type and, in that way, cross-color-model conversion of raster images, but except
// quantization from a non-paletted type to a paletted type.  When converting from an image with alpha
// information to some opaque, alpha is just dropped if no matte is set and try_to_hold_transparency is
// 'false'.  A matte color, if provided for flattening, must accord with the target color model;
// try_to_hold_transparency set to 'true' enables the next: in case of with-alpha to without-alpha conversion a
// visibility mask is generated for the resulting image marking as invisible those pixels which has zero alpha
// values in the source image; in case of without-alpha-but-with-visibility-mask to with-alpha conversion zero
// alphas are assigned to pixels which were marked as invisible in the source image.  NOTE. When converting
// from paletted to paletted-with-alpha, the target image scheme makes impossible to reinterpret a visibility
// mask if it is present in the source image.  An HDR image is converted into another HDR image.
// DEFAULTS. Matte is not specified; try_to_hold_transparency is set to 'false'.

#include "QProgressBase.h"
#include "QRasterImage.h"

namespace Q
{

class Q_ITEM RasterImageConversion : public ProgressBase
{
public:
    // supporting class for flattening
    class Matte
    {
    public:
        Matte (float fIntensity) : Intensity(fIntensity), Specified(true) {};
        Matte (const ColorRgb& rqColor) : RgbColor(rqColor), Specified(true) {};
        Matte (const ColorCmyk& rqColor) : CmykColor(rqColor), Specified(true) {};
        Matte (const ColorLab& rqColor) : LabColor(rqColor), Specified(true) {};

    private:
        friend class RasterImageConversion;
        Matte () : Specified(false) {};

        float Intensity;
        ColorRgb RgbColor;
        ColorCmyk CmykColor;
        ColorLab LabColor;
        bool Specified;
    };

    // construction
    RasterImageConversion (const RasterImagePtr& rspqImage, RasterImage::ImageType eTargetType);

    // settings
    void SetMatte (const Matte& rqMatte);
    void SetTryToHoldTransparency (bool bSet);

    // Conversion.  Progressed.
    RasterImagePtr GetConverted ();

private:
    RasterImagePtr m_spqSrcImage;
    RasterImage::ImageType m_eTargetType;
    int m_iWidth, m_iHeight;
    Matte m_qMatte;                 // default: unspecified
    bool m_bTryToHoldTransparency;  // default: false
};

#include "QRasterImageConversion.inl"

}

#endif



