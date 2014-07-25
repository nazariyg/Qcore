#ifndef QRASTERIMAGEQUANTIZATION_H
#define QRASTERIMAGEQUANTIZATION_H

// The class is to perform conversion of a non-paletted raster image into paletted one.  Two color models are
// acceptable for input: RGB and RGBA with RasterImage's types IT_RGB_8_8_8 and IT_RGBA_8_8_8_8.  There are two
// ways to quantize an image: adaptive quantization, which generates a close-to-optimal palette basing on
// colors present in the source image, and providential quantization, which implies some predefined assortment
// of colors to be placed into the resulting palette.  If an RGB image has a visibility mask, it will be simply
// reproduced.
// DEFAULTS. Dithering is set to DT_NONE; flag preserve_palette_entry_for_transparency is not set.

#include "QRasterImage.h"
#include "QProgressBase.h"

namespace Q
{

class Q_ITEM RasterImageQuantization : public ProgressBase
{
public:
    // if intended target palette size is less than approximately 64, AQT_MEDIAN_CUT is usually better
    enum AdaptiveQuantizationType
    {
        AQT_MEDIAN_CUT,  // faster, good for images with sharp color transitions
        AQT_NEURAL_NET   // slower, good for images with gradual color transitions
    };

    // only for adaptive quantization
    enum ForcingType
    {
        FT_NONE,
        FT_BLACK_AND_WHITE,
        FT_PRIMARY_COLORS,  // 8 colors: red, green, blue, cyan, magenta, yellow, black, and white
        FT_WEB_COLORS,      // 216 colors from the Web palette
        FT_CUSTOM_PALETTE
    };

    enum ProvidentialQuantizationType
    {
        PQT_BLACK_AND_WHITE,
        PQT_WEB_PALETTE,
        PQT_WIN_PALETTE,
        PQT_MAC_PALETTE,
        PQT_CUSTOM_PALETTE
    };

    enum DitheringType
    {
        DT_NONE,
        DT_DIFFUSION,
        DT_PATTERN,
        DT_NOISE
    };

    // Before-construction optional test.  If an image is represented by some number of different colors which
    // is less than or equal to 256, then this image can be converted into paletted without any color
    // information losses.  An exact palette is found only if the function has returned 'true'.  Later on, this
    // palette can be passed to the constructor oriented at providential quantization.
    static bool TryToGetExactPalette (const RasterImagePtr& rspqImage, RasterImage::Palette& rqPalette);

    // Construction.  For adaptive image quantization.  Target palette size is to be in the range [1, 256].  If
    // forcing is turned on, palette size must be greater than or equal to number of colors involved into
    // forcing (which is 2 for FT_BLACK_AND_WHITE, 8 for FT_PRIMARY_COLORS, and so on) because quota of forcing
    // colors is taken from value of palette size in order to ensure that number of colors in a resulting
    // palette will be equal to value provided to this constructor.  If forcing type is FT_CUSTOM_PALETTE, the
    // pointer to a custom forcing palette must be supplied.
    RasterImageQuantization (const RasterImagePtr& rspqImage, AdaptiveQuantizationType eAdaptiveQuantization,
        int iPaletteSize, ForcingType eForcing = FT_NONE, RasterImage::Palette* pqCustomForcingPalette = 0);

    // Construction.  For providential image quantization.  In case of PQT_CUSTOM_PALETTE, the pointer must be
    // provided.
    RasterImageQuantization (const RasterImagePtr& rspqImage,
        ProvidentialQuantizationType eProvidentialQuantization, RasterImage::Palette* pqCustomPalette = 0);

    // The dither option.  An amount must be >= 0.0.  DT_NOISE dithering resets random's seed each time it
    // starts.
    void SetDithering (DitheringType eDithering, float fAmount = 0.75f);

    // Aiming at a paletted-with-alpha image on output.  Only for cases when a source image is of RGBA color
    // model.  If 'true' is provided, resulting paletted image will have fully transparent pixels in areas
    // where the source image has alpha values less than 128.  For adaptive quantization, this
    // transparency-symbolizing entry will be taken in cost of adaptive colors number (just like space for
    // forcing colors is allocated), so the user must ensure that the algorithm will receive a valid parameter
    // of required palette size; in other words, it's must be true that
    //   target_palette_size - number_of_forcing_colors - (1 if 'true' is provided, or 0 if 'false') >= 0.
    // For providential quantization, in opposite to adaptive, the palette entry is added to the list of colors
    // associated with a certain quantization type, that is for PQT_WEB_PALETTE it will result into palette
    // size 217, but for PQT_WIN_PALETTE and for PQT_MAC_PALETTE no additional entry can be allocated at all.
    void SetPreservePaletteColorForTransparency (bool bSet);

    // Quantization.  Progressed.
    RasterImagePtr GetQuantized ();

private:
    static void FindPaletteForImageUsingMedianCut (const RasterImagePtr& rspqImage, int iPaletteSize,
        RasterImage::Palette& rqPalette, ProgressProbe& rqProgress);
    static void FindPaletteForImageUsingNeuralNet (const RasterImagePtr& rspqImage, int iPaletteSize,
        RasterImage::Palette& rqPalette, ProgressProbe& rqProgress);
    RasterImagePtr GetQuantizedOnPalette (const RasterImage::Palette& rqPalette, ProgressProbe& rqProgress,
        float fScopeCoefficient);

    RasterImagePtr m_spqImage;
    int m_iWidth, m_iHeight;
    bool m_bAdaptive;
    AdaptiveQuantizationType m_eAdaptiveQuantization;
    ProvidentialQuantizationType m_eProvidentialQuantization;
    int m_iPaletteSize;
    ForcingType m_eForcing;
    RasterImage::Palette m_qCustomPalette;
    DitheringType m_eDithering;                   // default: DT_NONE
    float m_fDitheringAmount;
    bool m_bPreservePaletteColorForTransparency;  // default: false
};

#include "QRasterImageQuantization.inl"

}

#endif



