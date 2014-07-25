//------------------------------------------------------------------------------------------------------------------
inline void HRTextureBase::SetSubRect (const Recti& rqRect)
{
    // not for precompressed textures
    assert( !m_bCompressed );

    SubRect = rqRect;
    SubRectSet = true;
}
//------------------------------------------------------------------------------------------------------------------
inline void HRTextureBase::SetSubRect (const SubRectArrayPtr& rspqSubRectArray)
{
    // not for precompressed textures
    assert( !m_bCompressed );

    SubRectArray = rspqSubRectArray;
    SubRectSet = true;
}
//------------------------------------------------------------------------------------------------------------------
inline void HRTextureBase::UnsetSubRect ()
{
    // not for precompressed textures
    assert( !m_bCompressed );

    SubRectSet = false;
}
//------------------------------------------------------------------------------------------------------------------
inline void HRTextureBase::SetPriority (float fPriority)
{
    Priority = fPriority;
    PrioritySet = true;
}
//------------------------------------------------------------------------------------------------------------------
inline void HRTextureBase::UnsetPriority ()
{
    PrioritySet = false;
}
//------------------------------------------------------------------------------------------------------------------
inline void HRTextureBase::SetSubTexture (const SubTextureOffsetRecord& rqSubTexOffsetRec,
    const RasterImagePtr& rspqImage)
{
    SubTextureOffset = rqSubTexOffsetRec;
    Image = rspqImage;
    SubTextureMode = true;
}
//------------------------------------------------------------------------------------------------------------------
inline void HRTextureBase::SetSubTexture (const SubTexOffsetRecArrayPtr& rspqSubTexOffsetRecArray,
    const RasterImagePtrArrayPtr& rspqImageArray)
{
    SubTextureOffsetArray = rspqSubTexOffsetRecArray;
    ImageArray = rspqImageArray;
    SubTextureMode = true;
}
//------------------------------------------------------------------------------------------------------------------
inline void HRTextureBase::SetSubTexture (const SubTexOffsetRecArrayPtr& rspqSubTexOffsetRecArray,
    const ComprMipmapArrayPtr& rspqCompressedTextureMipmapArray)
{
    SubTextureOffsetArray = rspqSubTexOffsetRecArray;
    CompressedTextureMipmapArray = rspqCompressedTextureMipmapArray;
    SubTextureMode = true;
}
//------------------------------------------------------------------------------------------------------------------
inline void HRTextureBase::SetSubTexture (const SubTexOffsetRecArrayPtrArrayPtr& rspqSubTexOffsetRecArrayArray,
    const ComprMipmapArrayPtrArrayPtr& rspqCompressedTextureMipmapArrayArray)
{
    SubTextureOffsetArrayArray = rspqSubTexOffsetRecArrayArray;
    CompressedTextureMipmapArrayArray = rspqCompressedTextureMipmapArrayArray;
    SubTextureMode = true;
}
//------------------------------------------------------------------------------------------------------------------
inline void HRTextureBase::UnsetSubTexture ()
{
    SubTextureMode = false;
}
//------------------------------------------------------------------------------------------------------------------
inline void HRTextureBase::SetCorrection (CorrectionType eCorrection)
{
    Correction = eCorrection;
    CorrectionSet = true;
}
//------------------------------------------------------------------------------------------------------------------
inline void HRTextureBase::UnsetCorrection ()
{
    CorrectionSet = false;
}
//------------------------------------------------------------------------------------------------------------------
inline void HRTextureBase::SetWrapS (WrapType eWrapS)
{
    WrapS = eWrapS;
    WrapSSet = true;
}
//------------------------------------------------------------------------------------------------------------------
inline void HRTextureBase::UnsetWrapS ()
{
    WrapSSet = false;
}
//------------------------------------------------------------------------------------------------------------------
inline void HRTextureBase::SetWrapT (WrapType eWrapT)
{
    WrapT = eWrapT;
    WrapTSet = true;
}
//------------------------------------------------------------------------------------------------------------------
inline void HRTextureBase::UnsetWrapT ()
{
    WrapTSet = false;
}
//------------------------------------------------------------------------------------------------------------------
inline void HRTextureBase::SetWrapR (WrapType eWrapR)
{
    // only for three-dimensional textures
    assert( GeneralType == GTT_3D );

    WrapR = eWrapR;
    WrapRSet = true;
}
//------------------------------------------------------------------------------------------------------------------
inline void HRTextureBase::UnsetWrapR ()
{
    // only for three-dimensional textures
    assert( GeneralType == GTT_3D );

    WrapRSet = false;
}
//------------------------------------------------------------------------------------------------------------------
inline void HRTextureBase::SetShrinkFilter (ShrinkFilterType eShrinkFilter)
{
    ShrinkFilter = eShrinkFilter;
    ShrinkFilterSet = true;
}
//------------------------------------------------------------------------------------------------------------------
inline void HRTextureBase::UnsetShrinkFilter ()
{
    ShrinkFilterSet = false;
}
//------------------------------------------------------------------------------------------------------------------
inline void HRTextureBase::SetEnlargeFilter (EnlargeFilterType eEnlargeFilter)
{
    EnlargeFilter = eEnlargeFilter;
    EnlargeFilterSet = true;
}
//------------------------------------------------------------------------------------------------------------------
inline void HRTextureBase::UnsetEnlargeFilter ()
{
    EnlargeFilterSet = false;
}
//------------------------------------------------------------------------------------------------------------------
inline void HRTextureBase::SetApply (ApplyType eApply)
{
    Apply = eApply;
    ApplySet = true;
}
//------------------------------------------------------------------------------------------------------------------
inline void HRTextureBase::UnsetApply ()
{
    ApplySet = false;
}
//------------------------------------------------------------------------------------------------------------------
inline void HRTextureBase::SetBorder (const ColorRgba& rqBorderColor)
{
    // not for depth textures and FXT1 textures
    assert( !m_bDepth
        &&  Type != TT_COMPRESSED_RGB_FXT1_2D
        &&  Type != TT_COMPRESSED_RGB_FXT1_3D
        &&  Type != TT_COMPRESSED_RGBA_FXT1_2D
        &&  Type != TT_COMPRESSED_RGBA_FXT1_3D );

    BorderColor = rqBorderColor;
    BorderSet = true;
}
//------------------------------------------------------------------------------------------------------------------
inline void HRTextureBase::UnsetBorder ()
{
    // not for FXT1 textures
    assert( Type != TT_COMPRESSED_RGB_FXT1_2D
        &&  Type != TT_COMPRESSED_RGB_FXT1_3D
        &&  Type != TT_COMPRESSED_RGBA_FXT1_2D
        &&  Type != TT_COMPRESSED_RGBA_FXT1_3D );

    BorderSet = false;
}
//------------------------------------------------------------------------------------------------------------------
inline void HRTextureBase::SetStoreCompressed (CompressionHintType eCompressionHint)
{
    // not for precompressed textures
    assert( !m_bCompressed );

    CompressionHint = eCompressionHint;
    StoreCompressedSet = true;
}
//------------------------------------------------------------------------------------------------------------------
inline void HRTextureBase::UnsetStoreCompressed ()
{
    // not for precompressed textures
    assert( !m_bCompressed );

    StoreCompressedSet = false;
}
//------------------------------------------------------------------------------------------------------------------
inline void HRTextureBase::SetInternalFormat (InternalFormatType eInternalFormat)
{
    // not for precompressed textures
    assert( !m_bCompressed );

    InternalFormat = eInternalFormat;
    InternalFormatSet = true;
}
//------------------------------------------------------------------------------------------------------------------
inline void HRTextureBase::UnsetInternalFormat ()
{
    // not for precompressed textures
    assert( !m_bCompressed );

    InternalFormatSet = false;
}
//------------------------------------------------------------------------------------------------------------------
inline void HRTextureBase::SetDepthBorder (float fBorderDepth)
{
    // for depth textures only
    assert( m_bDepth );

    BorderColor.R() = fBorderDepth;
    BorderSet = true;
}
//------------------------------------------------------------------------------------------------------------------
inline void HRTextureBase::SetDepthMode (DepthModeType eDepthMode)
{
    // for depth textures only
    assert( m_bDepth );

    DepthMode = eDepthMode;
    DepthModeSet = true;
}
//------------------------------------------------------------------------------------------------------------------
inline void HRTextureBase::UnsetDepthMode ()
{
    // for depth textures only
    assert( m_bDepth );

    DepthModeSet = false;
}
//------------------------------------------------------------------------------------------------------------------
inline void HRTextureBase::SetDepthCompareMode (DepthCompareModeType eDepthCompareMode)
{
    // for depth textures only
    assert( m_bDepth );

    DepthCompareMode = eDepthCompareMode;
    DepthCompareModeSet = true;
}
//------------------------------------------------------------------------------------------------------------------
inline void HRTextureBase::UnsetDepthCompareMode ()
{
    // for depth textures only
    assert( m_bDepth );

    DepthCompareModeSet = false;
}
//------------------------------------------------------------------------------------------------------------------
inline void HRTextureBase::SetDepthCompareFunction (DepthCompareFunctionType eDepthCompareFunction)
{
    // for depth textures only
    assert( m_bDepth );

    DepthCompareFunction = eDepthCompareFunction;
    DepthCompareFunctionSet = true;
}
//------------------------------------------------------------------------------------------------------------------
inline void HRTextureBase::UnsetDepthCompareFunction ()
{
    // for depth textures only
    assert( m_bDepth );

    DepthCompareFunctionSet = false;
}
//------------------------------------------------------------------------------------------------------------------
inline void HRTextureBase::SetBlendingColor (const ColorRgba& rqBlendingColor)
{
    BlendingColor = rqBlendingColor;
    BlendingColorSet = true;
}
//------------------------------------------------------------------------------------------------------------------
inline void HRTextureBase::UnsetBlendingColor ()
{
    BlendingColorSet = false;
}
//------------------------------------------------------------------------------------------------------------------
inline void HRTextureBase::SetCombineRgbFunction (CombineRgbFunctionType eCombineRgbFunction)
{
    // not for depth textures
    assert( !m_bDepth );

    CombineRgbFunction = eCombineRgbFunction;
    CombineRgbFunctionSet = true;
}
//------------------------------------------------------------------------------------------------------------------
inline void HRTextureBase::UnsetCombineRgbFunction ()
{
    // not for depth textures
    assert( !m_bDepth );

    CombineRgbFunctionSet = false;
}
//------------------------------------------------------------------------------------------------------------------
inline void HRTextureBase::SetCombineAlphaFunction (CombineAlphaFunctionType eCombineAlphaFunction)
{
    // not for depth textures
    assert( !m_bDepth );

    CombineAlphaFunction = eCombineAlphaFunction;
    CombineAlphaFunctionSet = true;
}
//------------------------------------------------------------------------------------------------------------------
inline void HRTextureBase::UnsetCombineAlphaFunction ()
{
    // not for depth textures
    assert( !m_bDepth );

    CombineAlphaFunctionSet = false;
}
//------------------------------------------------------------------------------------------------------------------
inline void HRTextureBase::SetCombineRgbArgument0 (CombineSourceType eCombineSourceRgb0,
    CombineOperandRgbType eCombineOperandRgb0, int iCrossbarTextureSourceRgb0)
{
    // not for depth textures
    assert( !m_bDepth );

    assert( iCrossbarTextureSourceRgb0 >= 0 );
    CombineSourceRgb0 = eCombineSourceRgb0;
    CombineOperandRgb0 = eCombineOperandRgb0;
    if ( CombineSourceRgb0 == CST_TEXTURE_N )
        CrossbarTextureSourceRgb0 = iCrossbarTextureSourceRgb0;

    CombineRgbArgument0Set = true;
}
//------------------------------------------------------------------------------------------------------------------
inline void HRTextureBase::UnsetCombineRgbArgument0 ()
{
    // not for depth textures
    assert( !m_bDepth );

    CombineRgbArgument0Set = false;
}
//------------------------------------------------------------------------------------------------------------------
inline void HRTextureBase::SetCombineRgbArgument1 (CombineSourceType eCombineSourceRgb1,
    CombineOperandRgbType eCombineOperandRgb1, int iCrossbarTextureSourceRgb1)
{
    // not for depth textures
    assert( !m_bDepth );

    assert( iCrossbarTextureSourceRgb1 >= 0 );
    CombineSourceRgb1 = eCombineSourceRgb1;
    CombineOperandRgb1 = eCombineOperandRgb1;
    if ( CombineSourceRgb1 == CST_TEXTURE_N )
        CrossbarTextureSourceRgb1 = iCrossbarTextureSourceRgb1;

    CombineRgbArgument1Set = true;
}
//------------------------------------------------------------------------------------------------------------------
inline void HRTextureBase::UnsetCombineRgbArgument1 ()
{
    // not for depth textures
    assert( !m_bDepth );

    CombineRgbArgument1Set = false;
}
//------------------------------------------------------------------------------------------------------------------
inline void HRTextureBase::SetCombineRgbArgument2 (CombineSourceType eCombineSourceRgb2,
    CombineOperandRgbType eCombineOperandRgb2, int iCrossbarTextureSourceRgb2)
{
    // not for depth textures
    assert( !m_bDepth );

    assert( iCrossbarTextureSourceRgb2 >= 0 );
    CombineSourceRgb2 = eCombineSourceRgb2;
    CombineOperandRgb2 = eCombineOperandRgb2;
    if ( CombineSourceRgb2 == CST_TEXTURE_N )
        CrossbarTextureSourceRgb2 = iCrossbarTextureSourceRgb2;

    CombineRgbArgument2Set = true;
}
//------------------------------------------------------------------------------------------------------------------
inline void HRTextureBase::UnsetCombineRgbArgument2 ()
{
    // not for depth textures
    assert( !m_bDepth );

    CombineRgbArgument2Set = false;
}
//------------------------------------------------------------------------------------------------------------------
inline void HRTextureBase::SetCombineAlphaArgument0 (CombineSourceType eCombineSourceAlpha0,
    CombineOperandAlphaType eCombineOperandAlpha0, int iCrossbarTextureSourceAlpha0)
{
    // not for depth textures
    assert( !m_bDepth );

    assert( iCrossbarTextureSourceAlpha0 >= 0 );
    CombineSourceAlpha0 = eCombineSourceAlpha0;
    CombineOperandAlpha0 = eCombineOperandAlpha0;
    if ( CombineSourceAlpha0 == CST_TEXTURE_N )
        CrossbarTextureSourceAlpha0 = iCrossbarTextureSourceAlpha0;

    CombineAlphaArgument0Set = true;
}
//------------------------------------------------------------------------------------------------------------------
inline void HRTextureBase::UnsetCombineAlphaArgument0 ()
{
    // not for depth textures
    assert( !m_bDepth );

    CombineAlphaArgument0Set = false;
}
//------------------------------------------------------------------------------------------------------------------
inline void HRTextureBase::SetCombineAlphaArgument1 (CombineSourceType eCombineSourceAlpha1,
    CombineOperandAlphaType eCombineOperandAlpha1, int iCrossbarTextureSourceAlpha1)
{
    // not for depth textures
    assert( !m_bDepth );

    assert( iCrossbarTextureSourceAlpha1 >= 0 );
    CombineSourceAlpha1 = eCombineSourceAlpha1;
    CombineOperandAlpha1 = eCombineOperandAlpha1;
    if ( CombineSourceAlpha1 == CST_TEXTURE_N )
        CrossbarTextureSourceAlpha1 = iCrossbarTextureSourceAlpha1;

    CombineAlphaArgument1Set = true;
}
//------------------------------------------------------------------------------------------------------------------
inline void HRTextureBase::UnsetCombineAlphaArgument1 ()
{
    // not for depth textures
    assert( !m_bDepth );

    CombineAlphaArgument1Set = false;
}
//------------------------------------------------------------------------------------------------------------------
inline void HRTextureBase::SetCombineAlphaArgument2 (CombineSourceType eCombineSourceAlpha2,
    CombineOperandAlphaType eCombineOperandAlpha2, int iCrossbarTextureSourceAlpha2)
{
    // not for depth textures
    assert( !m_bDepth );

    assert( iCrossbarTextureSourceAlpha2 >= 0 );
    CombineSourceAlpha2 = eCombineSourceAlpha2;
    CombineOperandAlpha2 = eCombineOperandAlpha2;
    if ( CombineSourceAlpha2 == CST_TEXTURE_N )
        CrossbarTextureSourceAlpha2 = iCrossbarTextureSourceAlpha2;

    CombineAlphaArgument2Set = true;
}
//------------------------------------------------------------------------------------------------------------------
inline void HRTextureBase::UnsetCombineAlphaArgument2 ()
{
    // not for depth textures
    assert( !m_bDepth );

    CombineAlphaArgument2Set = false;
}
//------------------------------------------------------------------------------------------------------------------
inline void HRTextureBase::SetCombineScaleRgb (CombineScaleType eCombineScaleRgb)
{
    // not for depth textures
    assert( !m_bDepth );

    CombineScaleRgb = eCombineScaleRgb;
    CombineScaleRgbSet = true;
}
//------------------------------------------------------------------------------------------------------------------
inline void HRTextureBase::UnsetCombineScaleRgb ()
{
    // not for depth textures
    assert( !m_bDepth );

    CombineScaleRgbSet = false;
}
//------------------------------------------------------------------------------------------------------------------
inline void HRTextureBase::SetCombineScaleAlpha (CombineScaleType eCombineScaleAlpha)
{
    // not for depth textures
    assert( !m_bDepth );

    CombineScaleAlpha = eCombineScaleAlpha;
    CombineScaleAlphaSet = true;
}
//------------------------------------------------------------------------------------------------------------------
inline void HRTextureBase::UnsetCombineScaleAlpha ()
{
    // not for depth textures
    assert( !m_bDepth );

    CombineScaleAlphaSet = false;
}
//------------------------------------------------------------------------------------------------------------------
inline void HRTextureBase::SetBuildMipmaps (bool bBuildMipmaps)
{
    // not for precompressed textures
    assert( !m_bCompressed );

    BuildMipmaps = bBuildMipmaps;
}
//------------------------------------------------------------------------------------------------------------------
inline void HRTextureBase::SetLimitMipmapsBuilding (int iLevel, int iBaseLevel, int iMaxLevel)
{
    // not for precompressed textures
    assert( !m_bCompressed );

    LMBLevel = iLevel;
    LMBBaseLevel = iBaseLevel;
    LMBMaxLevel = iMaxLevel;
    LimitMipmapsBuildingSet = true;
}
//------------------------------------------------------------------------------------------------------------------
inline void HRTextureBase::UnsetLimitMipmapsBuilding ()
{
    // not for precompressed textures
    assert( !m_bCompressed );

    LimitMipmapsBuildingSet = false;
}
//------------------------------------------------------------------------------------------------------------------
inline void HRTextureBase::SetMipmapsAutoGen (bool bMipmapsAutoGen)
{
    MipmapsAutoGen = bMipmapsAutoGen;
    MipmapsAutoGenSet = true;
}
//------------------------------------------------------------------------------------------------------------------
inline void HRTextureBase::UnsetMipmapsAutoGen ()
{
    MipmapsAutoGenSet = false;
}
//------------------------------------------------------------------------------------------------------------------
inline void HRTextureBase::SetMipmapsAutoGenHint (MipmapsAutoGenHintType eMipmapsAutoGenHint)
{
    MipmapsAutoGenHint = eMipmapsAutoGenHint;
    MipmapsAutoGenHintSet = true;
}
//------------------------------------------------------------------------------------------------------------------
inline void HRTextureBase::UnsetMipmapsAutoGenHint ()
{
    MipmapsAutoGenHintSet = false;
}
//------------------------------------------------------------------------------------------------------------------
inline void HRTextureBase::SetClampLambda (float fMinLod, float fMaxLod)
{
    MinLod = fMinLod;
    MaxLod = fMaxLod;
    ClampLambdaSet = true;
}
//------------------------------------------------------------------------------------------------------------------
inline void HRTextureBase::UnsetClampLambda ()
{
    ClampLambdaSet = false;
}
//------------------------------------------------------------------------------------------------------------------
inline void HRTextureBase::SetClampMipmapLevels (int iCMLBaseLevel, int iCMLMaxLevel)
{
    CMLBaseLevel = iCMLBaseLevel;
    CMLMaxLevel = iCMLMaxLevel;
    ClampMipmapLevelsSet = true;
}
//------------------------------------------------------------------------------------------------------------------
inline void HRTextureBase::UnsetClampMipmapLevels ()
{
    ClampMipmapLevelsSet = false;
}
//------------------------------------------------------------------------------------------------------------------
inline void HRTextureBase::SetLodBias (float fLodBias)
{
    LodBias = fLodBias;
    LodBiasSet = true;
}
//------------------------------------------------------------------------------------------------------------------
inline void HRTextureBase::UnsetLodBias ()
{
    LodBiasSet = false;
}
//------------------------------------------------------------------------------------------------------------------



