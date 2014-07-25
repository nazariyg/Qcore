#ifndef QHRTEXTUREBASE_H
#define QHRTEXTUREBASE_H

#include "QHRBufferable.h"
#include "QRasterImage.h"
#include "QHeapArrayTypedefs.h"

namespace Q
{

class Q_ITEM HRTextureBase : public HRBufferable
{
public:
    enum GeneralTextureType
    {
        GTT_1D,
        GTT_2D,
        GTT_3D,
        GTT_CUBE_MAP
    };

    enum TextureType
    {
        TT_1D,
        TT_2D,
        TT_DEPTH_1D,
        TT_DEPTH_2D,
        TT_3D,
        TT_CUBE_MAP,
        TT_COMPRESSED_RGB_DXT1_1D,
        TT_COMPRESSED_RGB_DXT1_2D,
        TT_COMPRESSED_RGB_DXT1_3D,
        TT_COMPRESSED_RGB_DXT1_CUBE_MAP,
        TT_COMPRESSED_RGBA_DXT1_1D,
        TT_COMPRESSED_RGBA_DXT1_2D,
        TT_COMPRESSED_RGBA_DXT1_3D,
        TT_COMPRESSED_RGBA_DXT1_CUBE_MAP,
        TT_COMPRESSED_RGBA_DXT3_1D,
        TT_COMPRESSED_RGBA_DXT3_2D,
        TT_COMPRESSED_RGBA_DXT3_3D,
        TT_COMPRESSED_RGBA_DXT3_CUBE_MAP,
        TT_COMPRESSED_RGBA_DXT5_1D,
        TT_COMPRESSED_RGBA_DXT5_2D,
        TT_COMPRESSED_RGBA_DXT5_3D,
        TT_COMPRESSED_RGBA_DXT5_CUBE_MAP,
        TT_COMPRESSED_RGB_FXT1_2D,
        TT_COMPRESSED_RGB_FXT1_3D,
        TT_COMPRESSED_RGBA_FXT1_2D,
        TT_COMPRESSED_RGBA_FXT1_3D
    };

    enum CorrectionType
    {
        CT_AFFINE,
        CT_PERSPECTIVE,
    };

    enum WrapType
    {
        WT_CLAMP,
        WT_CLAMP_TO_EDGE,
        WT_CLAMP_TO_BORDER,
        WT_REPEAT,
        WT_MIRRORED_REPEAT
    };

    enum ShrinkFilterType
    {
        SFT_NEAREST,
        SFT_LINEAR,
        SFT_MM_NEAREST_NEAREST,
        SFT_MM_NEAREST_LINEAR,
        SFT_MM_LINEAR_NEAREST,
        SFT_MM_LINEAR_LINEAR
    };

    enum EnlargeFilterType
    {
        EFT_NEAREST,
        EFT_LINEAR
    };

    enum ApplyType
    {
        AT_REPLACE,
        AT_MODULATE,
        AT_DECAL,
        AT_BLEND,
        AT_ADD,
        AT_COMBINE
    };

    enum MipmapsAutoGenHintType
    {
        MAGHT_FASTEST,
        MAGHT_NICEST
    };

    enum CompressionHintType
    {
        CHT_FASTEST,
        CHT_NICEST
    };

    enum InternalFormatType
    {
        IFT_ALPHA,
        IFT_DEPTH_COMPONENT,
        IFT_LUMINANCE,
        IFT_LUMINANCE_ALPHA,
        IFT_INTENSITY,
        IFT_RGB,
        IFT_RGBA,
        IFT_ALPHA4,
        IFT_ALPHA8,
        IFT_ALPHA12,
        IFT_ALPHA16,
        IFT_DEPTH_COMPONENT16,
        IFT_DEPTH_COMPONENT24,
        IFT_DEPTH_COMPONENT32,
        IFT_LUMINANCE4,
        IFT_LUMINANCE8,
        IFT_LUMINANCE12,
        IFT_LUMINANCE16,
        IFT_LUMINANCE4_ALPHA4,
        IFT_LUMINANCE6_ALPHA2,
        IFT_LUMINANCE8_ALPHA8,
        IFT_LUMINANCE12_ALPHA4,
        IFT_LUMINANCE12_ALPHA12,
        IFT_LUMINANCE16_ALPHA16,
        IFT_INTENSITY4,
        IFT_INTENSITY8,
        IFT_INTENSITY12,
        IFT_INTENSITY16,
        IFT_R3_G3_B2,
        IFT_RGB4,
        IFT_RGB5,
        IFT_RGB8,
        IFT_RGB10,
        IFT_RGB12,
        IFT_RGB16,
        IFT_RGBA2,
        IFT_RGBA4,
        IFT_RGB5_A1,
        IFT_RGBA8,
        IFT_RGB10_A2,
        IFT_RGBA12,
        IFT_RGBA16,
        IFT_COMPRESSED_ALPHA,
        IFT_COMPRESSED_LUMINANCE,
        IFT_COMPRESSED_LUMINANCE_ALPHA,
        IFT_COMPRESSED_INTENSITY,
        IFT_COMPRESSED_RGB,
        IFT_COMPRESSED_RGBA
    };

    enum DepthModeType
    {
        DMT_LUMINANCE,
        DMT_INTENSITY,
        DMT_ALPHA
    };

    enum DepthCompareModeType
    {
        DCMT_NONE,
        DCMT_COMPARE_R_TO_TEXTURE
    };

    enum DepthCompareFunctionType
    {
        DCFT_NEVER,
        DCFT_ALWAYS,
        DCFT_LESS,
        DCFT_LESS_OR_EQUAL,
        DCFT_EQUAL,
        DCFT_GREATER_OR_EQUAL,
        DCFT_GREATER,
        DCFT_NOT_EQUAL
    };

    enum CombineRgbFunctionType
    {
        CRGBFT_REPLACE,
        CRGBFT_MODULATE,
        CRGBFT_ADD,
        CRGBFT_ADD_SIGNED,
        CRGBFT_INTERPOLATE,
        CRGBFT_SUBTRACT,
        CRGBFT_DOT3_RGB,
        CRGBFT_DOT3_RGBA
    };

    enum CombineAlphaFunctionType
    {
        CAFT_REPLACE,
        CAFT_MODULATE,
        CAFT_ADD,
        CAFT_ADD_SIGNED,
        CAFT_INTERPOLATE,
        CAFT_SUBTRACT
    };

    enum CombineSourceType
    {
        CST_TEXTURE,
        CST_TEXTURE_N,
        CST_CONSTANT,
        CST_PRIMARY_COLOR,
        CST_PREVIOUS
    };

    enum CombineOperandRgbType
    {
        CORGBT_SRC_COLOR,
        CORGBT_ONE_MINUS_SRC_COLOR,
        CORGBT_SRC_ALPHA,
        CORGBT_ONE_MINUS_SRC_ALPHA
    };

    enum CombineOperandAlphaType
    {
        COAT_SRC_ALPHA,
        COAT_ONE_MINUS_SRC_ALPHA
    };

    enum CombineScaleType
    {
        CST_ONE,
        CST_TWO,
        CST_FOUR
    };

    enum TexCoordGenType
    {
        TCGT_LOCAL_PLANE,
        TCGT_EYE_PLANE,
        TCGT_SPHERE_MAP,
        TCGT_REFLECTION_MAP,
        TCGT_NORMAL_MAP
    };

    class CompressedTextureMipmap
    {
    public:
        CompressedTextureMipmap () {};
        CompressedTextureMipmap (int iWidth, int iHeight, int iSize, const CharHeapArrayPtr& rspqData)
            :
            Width(iWidth),
            Height(iHeight),
            Size(iSize),
            Data(rspqData)
        {
        }

        int Width;
        int Height;
        int Size;
        CharHeapArrayPtr Data;
    };

    class SubTextureOffsetRecord
    {
    public:
        SubTextureOffsetRecord () {};
        SubTextureOffsetRecord (int iX, int iY = 0, int iZ = 0)
            :
            X(iX),
            Y(iY),
            Z(iZ)
        {
        };

        int X, Y, Z;
    };

    typedef SmartPointer<HeapArray<CompressedTextureMipmap> > ComprMipmapArrayPtr;
    typedef SmartPointer<HeapArray<ComprMipmapArrayPtr> > ComprMipmapArrayPtrArrayPtr;
    typedef SmartPointer<HeapArray<Recti> > SubRectArrayPtr;
    typedef SmartPointer<HeapArray<SubTextureOffsetRecord> > SubTexOffsetRecArrayPtr;
    typedef SmartPointer<HeapArray<SubTexOffsetRecArrayPtr> > SubTexOffsetRecArrayPtrArrayPtr;

protected:
    // Construction.

    // one- and two-dimensional texture and depth texture
    HRTextureBase (TextureType eType, const RasterImagePtr& rspqImage);

    // Three-dimensional texture and cubemap texture.  Three-dimensional texture images must be arranged from
    // back to front.  Cubemap texture images must be in sequence: POS X, NEG X, POS Y, NEG Y, POS Z, NEG Z.
    HRTextureBase (TextureType eType, const RasterImagePtrArrayPtr& rspqImageArray);

    // one- and two-dimensional precompressed texture
    HRTextureBase (TextureType eType, const ComprMipmapArrayPtr& rspqCompressedTextureMipmapArray);

    // Three-dimensional and cubemap precompressed texture.  Three-dimensional texture images must be arranged
    // from back to front.  Cubemap texture images must be in sequence: POS X, NEG X, POS Y, NEG Y, POS Z,
    // NEG Z.
    HRTextureBase (TextureType eType, const ComprMipmapArrayPtrArrayPtr& rspqCompressedTextureMipmapArrayArray);

public:
    // subrectangle
    void SetSubRect (const Recti& rqRect);
    void SetSubRect (const SubRectArrayPtr& rspqSubRectArray);
    void UnsetSubRect ();

    // priority
    void SetPriority (float fPriority);
    void UnsetPriority ();

    // Subtexture.  One- and two-dimensional texture and depth texture; three-dimensional texture and cubemap
    // texture; one- and two-dimensional precompressed texture; three-dimensional and cubemap precompressed
    // texture.
    void SetSubTexture (const SubTextureOffsetRecord& rqSubTexOffsetRec, const RasterImagePtr& rspqImage);
    void SetSubTexture (const SubTexOffsetRecArrayPtr& rspqSubTexOffsetRecArray,
        const RasterImagePtrArrayPtr& rspqImageArray);
    void SetSubTexture (const SubTexOffsetRecArrayPtr& rspqSubTexOffsetRecArray,
        const ComprMipmapArrayPtr& rspqCompressedTextureMipmapArray);
    void SetSubTexture (const SubTexOffsetRecArrayPtrArrayPtr& rspqSubTexOffsetRecArrayArray,
        const ComprMipmapArrayPtrArrayPtr& rspqCompressedTextureMipmapArrayArray);
    void UnsetSubTexture ();

    // regular attributes
    void SetCorrection (CorrectionType eCorrection);
    void UnsetCorrection ();
    void SetWrapS (WrapType eWrapS);
    void UnsetWrapS ();
    void SetWrapT (WrapType eWrapT);
    void UnsetWrapT ();
    void SetWrapR (WrapType eWrapR);
    void UnsetWrapR ();
    void SetShrinkFilter (ShrinkFilterType eShrinkFilter);
    void UnsetShrinkFilter ();
    void SetEnlargeFilter (EnlargeFilterType eEnlargeFilter);
    void UnsetEnlargeFilter ();
    void SetApply (ApplyType eApply);
    void UnsetApply ();

    // border
    void SetBorder (const ColorRgba& rqBorderColor);
    void UnsetBorder ();

    // Storing compressed.  Actual for internal format autodetect.
    void SetStoreCompressed (CompressionHintType eCompressionHint);
    void UnsetStoreCompressed ();

    // optional internal format
    void SetInternalFormat (InternalFormatType eInternalFormat);
    void UnsetInternalFormat ();  // to do autodetect

    // depth texture related
    void SetDepthBorder (float fBorderDepth);
    void SetDepthMode (DepthModeType eDepthMode);
    void UnsetDepthMode ();
    void SetDepthCompareMode (DepthCompareModeType eDepthCompareMode);
    void UnsetDepthCompareMode ();
    void SetDepthCompareFunction (DepthCompareFunctionType eDepthCompareFunction);
    void UnsetDepthCompareFunction ();

    // environment and combining
    void SetBlendingColor (const ColorRgba& rqBlendingColor);
    void UnsetBlendingColor ();
    void SetCombineRgbFunction (CombineRgbFunctionType eCombineRgbFunction);
    void UnsetCombineRgbFunction ();
    void SetCombineAlphaFunction (CombineAlphaFunctionType eCombineAlphaFunction);
    void UnsetCombineAlphaFunction ();
    void SetCombineRgbArgument0 (CombineSourceType eCombineSourceRgb0, CombineOperandRgbType eCombineOperandRgb0,
        int iCrossbarTextureSourceRgb0 = -1);
    void UnsetCombineRgbArgument0 ();
    void SetCombineRgbArgument1 (CombineSourceType eCombineSourceRgb1, CombineOperandRgbType eCombineOperandRgb1,
        int iCrossbarTextureSourceRgb1 = -1);
    void UnsetCombineRgbArgument1 ();
    void SetCombineRgbArgument2 (CombineSourceType eCombineSourceRgb2, CombineOperandRgbType eCombineOperandRgb2,
        int iCrossbarTextureSourceRgb2 = -1);
    void UnsetCombineRgbArgument2 ();
    void SetCombineAlphaArgument0 (CombineSourceType eCombineSourceAlpha0,
        CombineOperandAlphaType eCombineOperandAlpha0, int iCrossbarTextureSourceAlpha0 = -1);
    void UnsetCombineAlphaArgument0 ();
    void SetCombineAlphaArgument1 (CombineSourceType eCombineSourceAlpha1,
        CombineOperandAlphaType eCombineOperandAlpha1, int iCrossbarTextureSourceAlpha1 = -1);
    void UnsetCombineAlphaArgument1 ();
    void SetCombineAlphaArgument2 (CombineSourceType eCombineSourceAlpha2,
        CombineOperandAlphaType eCombineOperandAlpha2, int iCrossbarTextureSourceAlpha2 = -1);
    void UnsetCombineAlphaArgument2 ();
    void SetCombineScaleRgb (CombineScaleType eCombineScaleRgb);
    void UnsetCombineScaleRgb ();
    void SetCombineScaleAlpha (CombineScaleType eCombineScaleAlpha);
    void UnsetCombineScaleAlpha ();

    // mipmapping and level-of-detail
    void SetBuildMipmaps (bool bBuildMipmaps);
    void SetLimitMipmapsBuilding (int iLevel, int iBaseLevel, int iMaxLevel);
    void UnsetLimitMipmapsBuilding ();
    void SetMipmapsAutoGen (bool bMipmapsAutoGen);
    void UnsetMipmapsAutoGen ();
    void SetMipmapsAutoGenHint (MipmapsAutoGenHintType eMipmapsAutoGenHint);
    void UnsetMipmapsAutoGenHint ();
    void SetClampLambda (float fMinLod, float fMaxLod);
    void UnsetClampLambda ();
    void SetClampMipmapLevels (int iCMLBaseLevel, int iCMLMaxLevel);
    void UnsetClampMipmapLevels ();
    void SetLodBias (float fLodBias);
    void UnsetLodBias ();

    // heading
    int TimesBound;                                                 // default: 0
    GeneralTextureType GeneralType;
    TextureType Type;
    RasterImagePtr Image;                                           // (default: 0)
    RasterImagePtrArrayPtr ImageArray;                              // (default: 0)
    ComprMipmapArrayPtr CompressedTextureMipmapArray;               // (default: 0)
    ComprMipmapArrayPtrArrayPtr CompressedTextureMipmapArrayArray;  // (default: 0)
    bool SubRectSet;                                                // default: false
    Recti SubRect;
    SubRectArrayPtr SubRectArray;
    bool PrioritySet;                                               // default: false
    float Priority;
    bool SubTextureMode;                                            // default: false
    SubTextureOffsetRecord SubTextureOffset;
    SubTexOffsetRecArrayPtr SubTextureOffsetArray;
    SubTexOffsetRecArrayPtrArrayPtr SubTextureOffsetArrayArray;

    // regular attributes
    bool CorrectionSet;                                             // default: false
    CorrectionType Correction;
    bool WrapSSet;                                                  // default: false
    WrapType WrapS;
    bool WrapTSet;                                                  // default: false
    WrapType WrapT;
    bool WrapRSet;                                                  // default: false
    WrapType WrapR;
    bool ShrinkFilterSet;                                           // default: false
    ShrinkFilterType ShrinkFilter;
    bool EnlargeFilterSet;                                          // default: false
    EnlargeFilterType EnlargeFilter;
    bool ApplySet;                                                  // default: false
    ApplyType Apply;

    // border
    bool BorderSet;                                                 // default: false
    ColorRgba BorderColor;

    // storing compressed
    bool StoreCompressedSet;                                        // default: false
    CompressionHintType CompressionHint;

    // Optional internal format.  'false' means do autodetect.
    bool InternalFormatSet;                                         // default: false
    InternalFormatType InternalFormat;

    // depth texture related
    bool DepthModeSet;                                              // default: false
    DepthModeType DepthMode;
    bool DepthCompareModeSet;                                       // default: false
    DepthCompareModeType DepthCompareMode;
    bool DepthCompareFunctionSet;                                   // default: false
    DepthCompareFunctionType DepthCompareFunction;

    // environment and combining
    bool BlendingColorSet;                                          // default: false
    ColorRgba BlendingColor;
    bool CombineRgbFunctionSet;                                     // default: false
    CombineRgbFunctionType CombineRgbFunction;
    bool CombineAlphaFunctionSet;                                   // default: false
    CombineAlphaFunctionType CombineAlphaFunction;
    bool CombineRgbArgument0Set;                                    // default: false
    CombineSourceType CombineSourceRgb0;
    CombineOperandRgbType CombineOperandRgb0;
    int CrossbarTextureSourceRgb0;
    bool CombineRgbArgument1Set;                                    // default: false
    CombineSourceType CombineSourceRgb1;
    CombineOperandRgbType CombineOperandRgb1;
    int CrossbarTextureSourceRgb1;
    bool CombineRgbArgument2Set;                                    // default: false
    CombineSourceType CombineSourceRgb2;
    CombineOperandRgbType CombineOperandRgb2;
    int CrossbarTextureSourceRgb2;
    bool CombineAlphaArgument0Set;                                  // default: false
    CombineSourceType CombineSourceAlpha0;
    CombineOperandAlphaType CombineOperandAlpha0;
    int CrossbarTextureSourceAlpha0;
    bool CombineAlphaArgument1Set;                                  // default: false
    CombineSourceType CombineSourceAlpha1;
    CombineOperandAlphaType CombineOperandAlpha1;
    int CrossbarTextureSourceAlpha1;
    bool CombineAlphaArgument2Set;                                  // default: false
    CombineSourceType CombineSourceAlpha2;
    CombineOperandAlphaType CombineOperandAlpha2;
    int CrossbarTextureSourceAlpha2;
    bool CombineScaleRgbSet;                                        // default: false
    CombineScaleType CombineScaleRgb;
    bool CombineScaleAlphaSet;                                      // default: false
    CombineScaleType CombineScaleAlpha;

    // mipmapping and level-of-detail
    bool BuildMipmaps;                                              // (default: true)
    bool LimitMipmapsBuildingSet;                                   // default: false
    int LMBLevel;
    int LMBBaseLevel;
    int LMBMaxLevel;
    bool MipmapsAutoGenSet;                                         // default: false
    bool MipmapsAutoGen;
    bool MipmapsAutoGenHintSet;                                     // default: false
    MipmapsAutoGenHintType MipmapsAutoGenHint;
    bool ClampLambdaSet;                                            // default: false
    float MinLod;
    float MaxLod;
    bool ClampMipmapLevelsSet;                                      // default: false
    int CMLBaseLevel;
    int CMLMaxLevel;
    bool LodBiasSet;                                                // default: false
    float LodBias;

private:
    bool m_bDepth;
    bool m_bCompressed;
};

#include "QHRTextureBase.inl"

}

#endif



