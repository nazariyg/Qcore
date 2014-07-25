#include "QHRTextureBase.h"
using namespace Q;

//------------------------------------------------------------------------------------------------------------------
HRTextureBase::HRTextureBase (TextureType eType, const RasterImagePtr& rspqImage)
    :
    TimesBound(0),
    Type(eType),
    Image(rspqImage),
    ImageArray(0),
    CompressedTextureMipmapArray(0),
    CompressedTextureMipmapArrayArray(0),
    SubRectSet(false),
    PrioritySet(false),
    SubTextureMode(false),
    CorrectionSet(false),
    WrapSSet(false),
    WrapTSet(false),
    WrapRSet(false),
    ShrinkFilterSet(false),
    EnlargeFilterSet(false),
    ApplySet(false),
    BorderSet(false),
    StoreCompressedSet(false),
    InternalFormatSet(false),
    DepthModeSet(false),
    DepthCompareModeSet(false),
    DepthCompareFunctionSet(false),
    BlendingColorSet(false),
    CombineRgbFunctionSet(false),
    CombineAlphaFunctionSet(false),
    CombineRgbArgument0Set(false),
    CombineRgbArgument1Set(false),
    CombineRgbArgument2Set(false),
    CombineAlphaArgument0Set(false),
    CombineAlphaArgument1Set(false),
    CombineAlphaArgument2Set(false),
    CombineScaleRgbSet(false),
    CombineScaleAlphaSet(false),
    BuildMipmaps(true),
    LimitMipmapsBuildingSet(false),
    MipmapsAutoGenSet(false),
    MipmapsAutoGenHintSet(false),
    ClampLambdaSet(false),
    ClampMipmapLevelsSet(false),
    LodBiasSet(false),
    m_bCompressed(false)
{
    assert( eType == TT_1D || eType == TT_2D || eType == TT_DEPTH_1D || eType == TT_DEPTH_2D );

    if ( eType == TT_1D || eType == TT_DEPTH_1D )
        GeneralType = GTT_1D;
    else
        GeneralType = GTT_2D;

    m_bDepth = ( eType == TT_DEPTH_1D || eType == TT_DEPTH_2D );
}
//------------------------------------------------------------------------------------------------------------------
HRTextureBase::HRTextureBase (TextureType eType, const RasterImagePtrArrayPtr& rspqImageArray)
    :
    TimesBound(0),
    Type(eType),
    Image(0),
    ImageArray(rspqImageArray),
    CompressedTextureMipmapArray(0),
    CompressedTextureMipmapArrayArray(0),
    SubRectSet(false),
    PrioritySet(false),
    SubTextureMode(false),
    CorrectionSet(false),
    WrapSSet(false),
    WrapTSet(false),
    WrapRSet(false),
    ShrinkFilterSet(false),
    EnlargeFilterSet(false),
    ApplySet(false),
    BorderSet(false),
    StoreCompressedSet(false),
    InternalFormatSet(false),
    DepthModeSet(false),
    DepthCompareModeSet(false),
    DepthCompareFunctionSet(false),
    BlendingColorSet(false),
    CombineRgbFunctionSet(false),
    CombineAlphaFunctionSet(false),
    CombineRgbArgument0Set(false),
    CombineRgbArgument1Set(false),
    CombineRgbArgument2Set(false),
    CombineAlphaArgument0Set(false),
    CombineAlphaArgument1Set(false),
    CombineAlphaArgument2Set(false),
    CombineScaleRgbSet(false),
    CombineScaleAlphaSet(false),
    BuildMipmaps(true),
    LimitMipmapsBuildingSet(false),
    MipmapsAutoGenSet(false),
    MipmapsAutoGenHintSet(false),
    ClampLambdaSet(false),
    ClampMipmapLevelsSet(false),
    LodBiasSet(false),
    m_bDepth(false),
    m_bCompressed(false)
{
    assert( eType == TT_3D || eType == TT_CUBE_MAP );

    if ( eType == TT_3D )
        GeneralType = GTT_3D;
    else
        GeneralType = GTT_CUBE_MAP;
}
//------------------------------------------------------------------------------------------------------------------
HRTextureBase::HRTextureBase (TextureType eType, const ComprMipmapArrayPtr& rspqCompressedTextureMipmapArray)
    :
    TimesBound(0),
    Type(eType),
    Image(0),
    ImageArray(0),
    CompressedTextureMipmapArray(rspqCompressedTextureMipmapArray),
    CompressedTextureMipmapArrayArray(0),
    SubRectSet(false),
    PrioritySet(false),
    SubTextureMode(false),
    CorrectionSet(false),
    WrapSSet(false),
    WrapTSet(false),
    WrapRSet(false),
    ShrinkFilterSet(false),
    EnlargeFilterSet(false),
    ApplySet(false),
    BorderSet(false),
    StoreCompressedSet(false),
    InternalFormatSet(false),
    DepthModeSet(false),
    DepthCompareModeSet(false),
    DepthCompareFunctionSet(false),
    BlendingColorSet(false),
    CombineRgbFunctionSet(false),
    CombineAlphaFunctionSet(false),
    CombineRgbArgument0Set(false),
    CombineRgbArgument1Set(false),
    CombineRgbArgument2Set(false),
    CombineAlphaArgument0Set(false),
    CombineAlphaArgument1Set(false),
    CombineAlphaArgument2Set(false),
    CombineScaleRgbSet(false),
    CombineScaleAlphaSet(false),
    BuildMipmaps(false),
    LimitMipmapsBuildingSet(false),
    MipmapsAutoGenSet(false),
    MipmapsAutoGenHintSet(false),
    ClampLambdaSet(false),
    ClampMipmapLevelsSet(false),
    LodBiasSet(false),
    m_bDepth(false),
    m_bCompressed(true)
{
    assert( eType == TT_COMPRESSED_RGB_DXT1_1D
        ||  eType == TT_COMPRESSED_RGB_DXT1_2D
        ||  eType == TT_COMPRESSED_RGBA_DXT1_1D
        ||  eType == TT_COMPRESSED_RGBA_DXT1_2D
        ||  eType == TT_COMPRESSED_RGBA_DXT3_1D
        ||  eType == TT_COMPRESSED_RGBA_DXT3_2D
        ||  eType == TT_COMPRESSED_RGBA_DXT5_1D
        ||  eType == TT_COMPRESSED_RGBA_DXT5_2D
        ||  eType == TT_COMPRESSED_RGB_FXT1_2D
        ||  eType == TT_COMPRESSED_RGBA_FXT1_2D );

    if ( eType == TT_COMPRESSED_RGB_DXT1_1D
    ||   eType == TT_COMPRESSED_RGBA_DXT1_1D
    ||   eType == TT_COMPRESSED_RGBA_DXT3_1D
    ||   eType == TT_COMPRESSED_RGBA_DXT5_1D )
    {
        GeneralType = GTT_1D;
    }
    else  // 2D
    {
        GeneralType = GTT_2D;
    }
}
//------------------------------------------------------------------------------------------------------------------
HRTextureBase::HRTextureBase (TextureType eType,
    const ComprMipmapArrayPtrArrayPtr& rspqCompressedTextureMipmapArrayArray)
    :
    TimesBound(0),
    Type(eType),
    Image(0),
    ImageArray(0),
    CompressedTextureMipmapArray(0),
    CompressedTextureMipmapArrayArray(rspqCompressedTextureMipmapArrayArray),
    SubRectSet(false),
    PrioritySet(false),
    SubTextureMode(false),
    CorrectionSet(false),
    WrapSSet(false),
    WrapTSet(false),
    WrapRSet(false),
    ShrinkFilterSet(false),
    EnlargeFilterSet(false),
    ApplySet(false),
    BorderSet(false),
    StoreCompressedSet(false),
    InternalFormatSet(false),
    DepthModeSet(false),
    DepthCompareModeSet(false),
    DepthCompareFunctionSet(false),
    BlendingColorSet(false),
    CombineRgbFunctionSet(false),
    CombineAlphaFunctionSet(false),
    CombineRgbArgument0Set(false),
    CombineRgbArgument1Set(false),
    CombineRgbArgument2Set(false),
    CombineAlphaArgument0Set(false),
    CombineAlphaArgument1Set(false),
    CombineAlphaArgument2Set(false),
    CombineScaleRgbSet(false),
    CombineScaleAlphaSet(false),
    BuildMipmaps(false),
    LimitMipmapsBuildingSet(false),
    MipmapsAutoGenSet(false),
    MipmapsAutoGenHintSet(false),
    ClampLambdaSet(false),
    ClampMipmapLevelsSet(false),
    LodBiasSet(false),
    m_bDepth(false),
    m_bCompressed(true)
{
    assert( eType == TT_COMPRESSED_RGB_DXT1_3D
        ||  eType == TT_COMPRESSED_RGB_DXT1_CUBE_MAP
        ||  eType == TT_COMPRESSED_RGBA_DXT1_3D
        ||  eType == TT_COMPRESSED_RGBA_DXT1_CUBE_MAP
        ||  eType == TT_COMPRESSED_RGBA_DXT3_3D
        ||  eType == TT_COMPRESSED_RGBA_DXT3_CUBE_MAP
        ||  eType == TT_COMPRESSED_RGBA_DXT5_3D
        ||  eType == TT_COMPRESSED_RGBA_DXT5_CUBE_MAP
        ||  eType == TT_COMPRESSED_RGB_FXT1_3D
        ||  eType == TT_COMPRESSED_RGBA_FXT1_3D );

    if ( eType == TT_COMPRESSED_RGB_DXT1_3D
    ||   eType == TT_COMPRESSED_RGBA_DXT1_3D
    ||   eType == TT_COMPRESSED_RGBA_DXT3_3D
    ||   eType == TT_COMPRESSED_RGBA_DXT5_3D
    ||   eType == TT_COMPRESSED_RGB_FXT1_3D
    ||   eType == TT_COMPRESSED_RGBA_FXT1_3D )
    {
        GeneralType = GTT_3D;
    }
    else  // CUBE_MAP
    {
        GeneralType = GTT_CUBE_MAP;
    }
}
//------------------------------------------------------------------------------------------------------------------



