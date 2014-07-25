#ifndef QRASTERIMAGETRANSFORMATION_H
#define QRASTERIMAGETRANSFORMATION_H

// Common class for raster image transformations.  A transformation returns a new image, the input image
// remains the same.  All the operations are optimized for images with 8 and 16 bits per sample.

#include "QProgressBase.h"
#include "QRasterImage.h"

namespace Q
{

class Q_ITEM RasterImageTransformation : public ProgressBase
{
public:
    // Interpolation methods (filters).  The only interpolation that can be used for paletted and
    // paletted-with-alpha images is Nearest Neighbor.
    enum InterpolationType
    {
        IT_NEAREST_NEIGHBOR,  // (no interpolation) fastest, results in jagged effects
        IT_BILINEAR,          // medium both in speed and accuracy
        IT_BICUBIC,           // slower, produces the most accurate results
        IT_CATMULL_ROM,       // slower, similar to Bicubic but results in slightly smoother images
        IT_LANCZOS_3,         // slower, similar to Bicubic but results in slightly sharper images
        IT_B_SPLINE,          // slower, about x2 blurring
        IT_GAUSSIAN           // slower, about x4 blurring
    };

    // support for non-rectangular transformations
    class OuterColor
    {
    public:
        OuterColor (int iIndex) : Index(iIndex), Specified(true) {};
        OuterColor (float fIntensity) : Intensity(fIntensity), Specified(true) {};
        OuterColor (const ColorRgb& rqColor) : RgbColor(rqColor), Specified(true) {};
        OuterColor (const ColorCmyk& rqColor) : CmykColor(rqColor), Specified(true) {};
        OuterColor (const ColorLab& rqColor) : LabColor(rqColor), Specified(true) {};

    private:
        friend class RasterImageTransformation;
        OuterColor () : Specified(false) {};

        int Index;
        float Intensity;
        ColorRgb RgbColor;
        ColorCmyk CmykColor;
        ColorLab LabColor;
        bool Specified;
    };

    // Construction.  The parameter is an image to be transformed (via copy).
    RasterImageTransformation (const RasterImagePtr& rspqImage);

    // flipping
    RasterImagePtr GetFlippedHorizontally () const;
    RasterImagePtr GetFlippedVertically () const;

    // Resampling (resizing).  All the functions are progressed.
    RasterImagePtr GetResampledHorizontally (int iNewWidth, InterpolationType eInterpolation);
    RasterImagePtr GetResampledVertically (int iNewHeight, InterpolationType eInterpolation);
    RasterImagePtr GetResampled (int iNewWidth, int iNewHeight, InterpolationType eInterpolation);

    // rotation by an angle multiple of 90 degrees
    RasterImagePtr GetRotated90Ccw () const;  // counter-clockwise
    RasterImagePtr GetRotated90Cw () const;   // clockwise
    RasterImagePtr GetRotated180 () const;

    // May be called before arbitrary-angle rotation or skewing.  Specifies a color for the areas that appear
    // outside the transformed image in order to make the whole frame rectangular.  In case of an image type
    // that assumes transparency (except paletted-with-alpha), the color is understood as fully transparent.
    void SetOuterColor (const OuterColor& rqOuterColor);

    // Rotation by an arbitrary angle.  The angle is in degrees.  Progressed.
    RasterImagePtr GetRotated (float fAngle, InterpolationType eInterpolation);

    // Skewing.  The angles are in degrees.  All the functions are progressed.
    RasterImagePtr GetSkewedHorizontally (float fAngle, InterpolationType eInterpolation);
    RasterImagePtr GetSkewedVertically (float fAngle, InterpolationType eInterpolation);

private:
    void GetOuterColorAsBytes (StillArray<unsigned char>& rqOCBytes) const;

    RasterImagePtr m_spqSrcImage;
    OuterColor m_qOuterColor;
};

#include "QRasterImageTransformation.inl"

}

#endif



