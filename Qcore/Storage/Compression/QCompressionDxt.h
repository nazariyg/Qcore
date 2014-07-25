#ifndef QCOMPRESSIONDXT_H
#define QCOMPRESSIONDXT_H

// DirectX Texture compression/decompression (S3TC).  Only images with dimensions which are multiples of four
// can be involved.  The formulas used to generate intermediate color/alpha values from two color/alpha
// extremes are that which stated in the Microsoft documentation on DXT compression since they have got more
// spreading than ones declared in the official S3TC specification.
//
// For packing, two RasterImage's types are acceptable: IT_RGB_8_8_8 and IT_RGBA_8_8_8_8.  DXT1 can be set for
// both color models while DXT2/3/4/5 only for RGBA.  This generality of DXT1 is caused by its ability to
// indicate presence of a transparency information selectively on per-block level.  DXT2 and DXT4 kinds of
// compression, which store color components premultiplied by the alpha value, are fully supported but,
// however, not recommended due to their habit to shrink color information noticeably after packing-unpacking
// consecution (especially DXT2).  DXT4 and DXT5 compressions store transparency information in the way similar
// to how color is stored and with the option to include or not to include 0 and 255 alpha levels into the
// range of values every pixel makes its selection from; the common behavior implies considering fully opaque
// and fully transparent pixels as ones with "must-to-preserve" information, thus always placing these
// outermost values into the selection row if at least one pixel in the block is fully visible or completely
// invisible.
// DEFAULTS. The default weighting factors for RGB components are those which represent specificity of the
// human vision: (0.3086, 0.6094, 0.082); the default threshold involved into DXT1-with-alpha compression is
// half-of-the-range: 128.
//
// For unpacking, image type IT_RGBA_8_8_8_8 is always outputted since it provides generalization of DXT1 with
// its two subtypes and corresponds to transparency-oriented DXT2/3/4/5 types of compression.

#include "QProgressBase.h"
#include "QRasterImage.h"

namespace Q
{

class Q_ITEM CompressionDxt : public ProgressBase
{
public:
    enum CompressionType
    {
        CT_DXT_1,
        CT_DXT_2,
        CT_DXT_3,
        CT_DXT_4,
        CT_DXT_5
    };

    // relates only to packing
    enum CompressionQualityType
    {
        CQT_LOW,     // very fast, but may produce visual artifacts for some images
        CQT_MEDIUM,  // fast, typically ensures a quality acceptable for final production
        CQT_HIGH     // slow, brings a quality near the highest possible, often indistinguishable from CQT_MEDIUM
    };

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    // Construction.  For packing.
    CompressionDxt (CompressionType eCompression, CompressionQualityType eCompressionQuality = CQT_MEDIUM);

    // Setting of custom color weightings.  Each coefficient must be in the range [0.01, 1.0].
    void SetWeightingFactors (float fFcrR, float fFcrG, float fFcrB);

    // The function may be useful for DXT1 compression of images which contain partially transparent pixels.
    // Pixels with alpha less than the threshold will become fully transparent within the DXT1 domain or fully
    // opaque otherwise.  A provided value must be in the range [0, 255].
    void SetDxt1aThreshold (int iThreshold);

    // Packing.  The dimensions of a provided image must be multiples of four.  Progressed.
    void Compress (const RasterImagePtr& rspqRasterImage, StillArray<char>& rqBytes);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    // Construction.  For unpacking.  The dimensions must be multiples of four.
    CompressionDxt (CompressionType eCompression, int iWidth, int iHeight);

    // Unpacking.  The user must ensure that the number of available bytes pointed by acByte is equal to or
    // greater than (width*height/16)*blocksize, where blocksize is 8 for DXT1 and 16 for other compression
    // types.  Progressed.
    void Decompress (const char* acByte, RasterImagePtr& rspqRasterImage);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

private:
    void OnWeightingFactorsUpdate (float fFcrR, float fFcrG, float fFcrB);
    void FindExtremeColorsForBlock (const unsigned char aaucBlock[16][4], bool bDxt1a,
        unsigned short ausColor[2]) const;
    void FindExtremeAlphasForBlock (const unsigned char aucBlock[16], bool bHasOutermostAlpha,
        unsigned char aucAlpha[2]) const;
    void FindColorIndicesForBlock (const unsigned char aaucBlock[16][4], const unsigned short ausColor[2],
        int aiIndex[16]) const;
    void FindAlphaIndicesForBlock (const unsigned char aucBlock[16], const unsigned char aucAlpha[2],
        int aiIndex[16]) const;
    void WriteColorData (const unsigned char* pucBasePix, int& riBytePos, int& riBitRatio, bool bOpaque,
        int iBytesPerPixel, int iRowStride, const int aiAlphaForAssoc[16], StillArray<char>& rqBytes) const;
    void ReadColorData (const char* acByte, int& riBytePos, int& riBitRatio, RasterImagePtr& rspqRasterImage,
        int iBlockLTX, int iBlockLTY) const;

    CompressionType m_eCompression;
    CompressionQualityType m_eCompressionQuality;
    float m_afWeightingFactor[3];   // (default: 0.3086, 0.6094, 0.082)
    int m_aiWeightingSqMult[3];
    int m_iDxt1aThreshold;          // (default: 128)
    int m_iWidth, m_iHeight;
    int m_ai5To8[32], m_ai6To8[64];
};

#include "QCompressionDxt.inl"

}

#endif



