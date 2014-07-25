#ifndef QRASTERIMAGEADJUSTMENT_H
#define QRASTERIMAGEADJUSTMENT_H

// Common class for raster image adjustments.  An adjustment returns a new image, the input image remains the
// same.  All the procedures are optimized for images with 8 and 16 bits per sample.

#include "QProgressBase.h"
#include "QRasterImage.h"

namespace Q
{

class Q_ITEM RasterImageAdjustment : public ProgressBase
{
public:
    // Construction.  The parameter is an image to be adjusted (via copy).
    RasterImageAdjustment (const RasterImagePtr& rspqImage);

    // Brightness.  The value must be in the range [-1.0, 1.0]; value 0.0 means no variation.
    RasterImagePtr GetWithBrightness (float fBrightness) const;

    // Contrast.  The value must be in the range [-1.0, 1.0]; value 0.0 means no variation.
    RasterImagePtr GetWithContrast (float fContrast) const;

    // Hue.  The value must be in the range [-1.0, 1.0]; value 0.0 means no variation.  (A grayscale image is
    // copied impassive.)
    RasterImagePtr GetWithHue (float fHue) const;

    // Saturation.  The value must be in the range [-1.0, 1.0]; value 0.0 means no variation.  (A grayscale
    // image is copied impassive.)
    RasterImagePtr GetWithSaturation (float fSaturation) const;

    // Lightness.  The value must be in the range [-1.0, 1.0]; value 0.0 means no variation.
    RasterImagePtr GetWithLightness (float fLightness) const;

    // Gamma.  The value must be >= 0.0; value 1.0 means no variation.
    RasterImagePtr GetWithGamma (float fGamma) const;

    // Filters related to image definition.  Cannot be used on paletted images.  All the functions are
    // progressed.
    RasterImagePtr GetBlurred ();
    RasterImagePtr GetBlurredMore ();
    RasterImagePtr GetSharpened ();
    RasterImagePtr GetSharpenedMore ();

    // Custom filtering.  Parameter aiMatrix is the pointer to an array of values (can be negative) that
    // represent the filter to be applied; the values are assumed to be arranged in the row-major way, going
    // left-to-right within a row and top-to-bottom within the matrix.  Parameter iN is the matrix' dimension
    // that must be always odd.  Parameter iWeight is a non-zero weighting factor (divisor) for the filter; if
    // not specified, the sum of the matrix' elements will be the factor.  Cannot be used on paletted images.
    // Progressed.
    RasterImagePtr GetCustomFiltered (int* aiMatrix, int iN, int iWeight = 0);

private:
    RasterImagePtr m_spqSrcImage;
};

}

#endif



