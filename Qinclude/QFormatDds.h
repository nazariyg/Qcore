#ifndef QFORMATDDS_H
#define QFORMATDDS_H

// Direct Draw Surface Format.  File extension: .dds.  This implementation strictly follows the specification
// written by Microsoft.  Since DDS is not from among very plane formats, it requires some detailing and
// terminology clarification.  There are three sorts of resources which can be stored in a DDS file: regular
// two-dimensional textures, cubic environment maps, and volume textures.  2D textures and cubics are quite
// similar in their treatment with the only difference that a cubic has six faces of 2D-texture-like images,
// but a volume texture is somewhat special case.  The term "main image" within this implementation designates
// the first image contained by a file: for a 2D texture it will be the original full-resolution image, for a
// cubic environment map this is the full-resolution positive-x face, and for a volume texture it's the first
// full-resolution slice.  All the resource types can be stored with or without downsampled mipmap levels, and,
// if they present, their number is not necessary to be the maximal possible but is to be less than or equal to
// the limit; on the other hand, a complete set of mipmaps is usual and recommended case.  Here, for 2D
// textures and cubics, the term "mipmap" also includes the main image (for a 2D texture) and any
// full-resolution face (for a cubic) because a rendering process cares for all of them while drawing a scene;
// but, for volume textures "mipmap" is just one of downsampled images.  Finally, the term "resolution level
// quantity" means number of all images stored in a DDS file if it's a 2D texture, number of images per face if
// it's a cubic environment map, and number of non-full-resolution images if it's a volume texture.  So, mipmap
// level 0 is the main full-resolution image or a full-resolution cube face for the first two resource types,
// but for a volume texture it's the first downsampled image.  Accordingly to the specification on 2D textures
// and cubics, each next downsampled level gets width and height in half of dimensions of the higher level
// using integer division by 2 where 1 divided by 2 still results into 1, so if, for example, a level has
// dimensions 1x4, then the lower one will be 1x2; to make a complete set of mipmaps the reduction goes on
// until an image gets dimensions 1x1.  The rules of ordering and sizing of mipmaps for a volume texture, with
// which this implementation accords properly, are rather sophisticated and should be seen on the Microsoft's
// page "DDS File Layout for Volume Textures".
//
// For saving, two RasterImage's color models are acceptable: CMT_RGB and CMT_RGBA.  The next packing types can
// be used together with CMT_RGB: PT_R8_G8_B8, PT_R5_G6_B5, PT_X8_R8_G8_B8, PT_X1_R5_G5_B5, PT_R3_G3_B2,
// PT_X4_R4_G4_B4, PT_DXT_1.  And for CMT_RGBA they are: PT_A8_R8_G8_B8, PT_A1_R5_G5_B5, PT_A4_R4_G4_B4,
// PT_A2_R10_G10_B10, PT_A8_R3_G3_B2, PT_DXT_1, PT_DXT_2, PT_DXT_3, PT_DXT_4, PT_DXT_5.  Images with a
// visibility mask are not allowed.  DXT compression wants full-resolution images to have dimensions multiple
// of 4, however their downsampled variants are free of this regulation.  It may be noted that the DXT
// compression process is optimized for images with 8 bits per sample, and if a FormatDds object gets an image
// with a different precision, then conversion is forced to take place.  Now, about the DXT compression in
// detail.  DXT1 can be set for both color models while DXT2/3/4/5 only for RGBA.  This generality of DXT1 is
// caused by its ability to indicate presence of a transparency information selectively on per-block level.
// DXT2 and DXT4 kinds of compression, which store color components premultiplied by the alpha value, are fully
// supported but, however, not recommended due to their habit to shrink color information noticeably after
// packing-unpacking consecution (especially DXT2).  DXT4 and DXT5 compressions store transparency information
// in the way similar to how color is stored and with the option to include or not to include 0 and 255 alpha
// levels into the range of values every pixel makes its selection from; the common behavior implies
// considering fully opaque and fully transparent pixels as ones with "must-to-preserve" information, thus
// always placing these outermost values into the selection row if at least one pixel in the block is fully
// visible or completely invisible.
// DEFAULTS. DXT compression quality is DXTCQT_MEDIUM; DXT color weighting factors are ones which adapted to
// the human vision: (0.3086, 0.6094, 0.082); DXT1 transparency threshold is half-of-the-range: 128.
//
// For loading, an image can appear with type IT_RGB_8_8_8, IT_RGB_16_16_16, IT_RGBA_8_8_8_8, or
// IT_RGBA_16_16_16_16.  Without a visibility mask associated.

#include "QFormatBase.h"
#include "QProgressBase.h"
#include "QRasterImage.h"
#include "QSystUtils.h"

namespace Q
{

class Q_ITEM FormatDds : public FormatBase, public ProgressBase
{
public:
    // relates only to saving
    enum PackingType
    {
        PT_A8_R8_G8_B8,
        PT_A1_R5_G5_B5,
        PT_A4_R4_G4_B4,
        PT_R8_G8_B8,
        PT_R5_G6_B5,
        PT_A2_R10_G10_B10,
        PT_X8_R8_G8_B8,
        PT_X1_R5_G5_B5,
        PT_R3_G3_B2,
        PT_A8_R3_G3_B2,
        PT_X4_R4_G4_B4,
        PT_DXT_1,
        PT_DXT_2,
        PT_DXT_3,
        PT_DXT_4,
        PT_DXT_5
    };

    // relates only to saving
    enum DxtCompressionQualityType
    {
        DXTCQT_LOW,     // very fast, but can produce visual artifacts for some images
        DXTCQT_MEDIUM,  // fast, usually ensures quality acceptable for the final production
        DXTCQT_HIGH     // slow, brings quality close to the highest possible, often indistinguishable from Medium
    };

    // relates only to loading
    enum ResourceType
    {
        RT_2D_TEXTURE,
        RT_CUBIC_ENVIRONMENT_MAP,
        RT_VOLUME_TEXTURE
    };

    // relates only to loading
    enum CompressionType
    {
        CT_NONE,
        CT_DXT_1,
        CT_DXT_2,
        CT_DXT_3,
        CT_DXT_4,
        CT_DXT_5
    };

    // relates only to loading
    enum ColorModelType
    {
        CMT_RGB,
        CMT_RGBA
    };

    // ET_FORMAT_IDENTIFIER_MISMATCH: signature discrepancy, causes no assert-false
    enum ExceptionType
    {
        ET_CANNOT_ACCESS_FILE,              // saving and loading
        ET_FORMAT_IDENTIFIER_MISMATCH,  // loading
        ET_FORMAT_MISMATCH              // loading
    };

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    // In the constructions for saving below, if parameter iResolutionLevelQuantity is -1, then all possible
    // mipmap levels will be generated automatically; if it is 1, then no mipmaps will be created and only the
    // main full-resolution image(s) will be stored.  Custom mipmaps, if they are supplied via pointer
    // designation, must not include the full-resolution image(s) since it's provided separately.  When custom
    // mipmaps are to be used, size of the array (in the case of a cubic there are six arrays) must be
    // resolution_level_quantity-1 for 2D textures and cubic environment maps, but must be equally
    // resolution_level_quantity for volume textures; sequence order and dimensions of these custom mipmaps
    // must correspond with the rules of mipmap storage within the resource type; a number of custom mipmaps
    // can be less than or equal to their maximal possible quantity accordingly to the resource type and to the
    // dimensions of the main image(s).

    // Construction.  For saving of a 2D texture.
    FormatDds (const RasterImagePtr& rspq2DTexture, PackingType ePacking, int iResolutionLevelQuantity = -1,
        StillArray<RasterImagePtr>* pqCustomMipmaps = 0);

    // Construction.  For saving of a cubic environment map.  All provided cube faces must be square and
    // maintain the same side extension.
    FormatDds (
        const RasterImagePtr& rspqFacePositiveX, const RasterImagePtr& rspqFaceNegativeX,
        const RasterImagePtr& rspqFacePositiveY, const RasterImagePtr& rspqFaceNegativeY,
        const RasterImagePtr& rspqFacePositiveZ, const RasterImagePtr& rspqFaceNegativeZ,
        PackingType ePacking, int iResolutionLevelQuantity = -1,
        const StillArray<RasterImagePtr> aqCustomMipmaps[6] = 0);

    // Construction.  For saving of a volume texture.  A number of slices must be power of two as well as
    // dimensions of these slices.  Due to the peculiar properties of volume textures, it's up to the user's
    // program to generate mipmap levels if they are required.
    FormatDds (const StillArray<RasterImagePtr>& rqSlices, PackingType ePacking,
        StillArray<RasterImagePtr>* pqCustomMipmaps = 0);

    // Setting of the DXT compression parameters.  May be used only if the type of packing implies a DXT
    // compression method.  SetDxtWeightingFactors function can be called to establish a correlation among the
    // color primaries accordingly to their importance for the user's needs, thus forcing the DXT compression
    // process to preserve information from some color channels at the cost of others; each factor must be in
    // the range [0.01, 1.0] and it's not necessary that their sum equals to 1.  SetDxt1aThreshold function can
    // be useful for DXT1 compression of images with transparency (RGBA color model): pixels with an alpha less
    // than the threshold will become fully transparent within compression domain or fully opaque otherwise; a
    // provided value must be in the range [0, 255].
    void SetDxtQuality (DxtCompressionQualityType eDxtQuality);
    void SetDxtWeightingFactors (float fFcrR, float fFcrG, float fFcrB);
    void SetDxt1aThreshold (int iThreshold);

    // Saving.  Progressed.
    bool SaveFile (const char* acFilename);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    // Construction.  For loading.  Initially tries to read the heading data.
    FormatDds (const char* acFilename, bool& rbSuccess);

    // File attributes.

    // resource type regardless
    ResourceType GetResourceType () const;
    bool AreMainImageDimensionsPowerOfTwo () const;
    int GetMainImageWidth () const;
    int GetMainImageHeight () const;
    int GetBitsPerPixel () const;
    ColorModelType GetColorModel () const;
    CompressionType GetCompression () const;
    bool HasNonFullResolutionImages () const;
    int GetResolutionLevelQuantity () const;  // using the meaning remarked at the beginning of this page

    // additional calls for a cubic environment map
    bool HasAllFaces () const;
    bool HasFacePositiveX () const;
    bool HasFaceNegativeX () const;
    bool HasFacePositiveY () const;
    bool HasFaceNegativeY () const;
    bool HasFacePositiveZ () const;
    bool HasFaceNegativeZ () const;

    // for a volume texture
    int GetVolumeTextureDepth () const;  // returns the number of slices

    // Image retrieving.

    // Loading of the main full-resolution image (title image), resource type regardless.  Progressed.
    bool LoadMainImage (RasterImagePtr& rspqMainImage);

    // The next calls for image retrieving are specific to the file's resource type which can be obtained by
    // GetResourceType function.

    // Loading of a mipmap level of a 2D texture.  Parameter iLevel must be in the range
    // [0, resolution_level_quantity-1]; level 0 identifies the main full-resolution image.  Progressed.
    bool Load2DTextureMipmapLevel (int iLevel, RasterImagePtr& rspqMipmapLevel);

    // Loading of faces of a cubic environment map.  Progressed.
    bool LoadFacePositiveX (RasterImagePtr& rspqFace);
    bool LoadFaceNegativeX (RasterImagePtr& rspqFace);
    bool LoadFacePositiveY (RasterImagePtr& rspqFace);
    bool LoadFaceNegativeY (RasterImagePtr& rspqFace);
    bool LoadFacePositiveZ (RasterImagePtr& rspqFace);
    bool LoadFaceNegativeZ (RasterImagePtr& rspqFace);

    // Loading of mipmap levels of faces of a cubic environment map.  Parameter iLevel must be in the range
    // [0, resolution_level_quantity-1]; level 0 identifies the full-resolution face image.  Progressed.
    bool LoadFacePositiveXMipmapLevel (int iLevel, RasterImagePtr& rspqMipmapLevel);
    bool LoadFaceNegativeXMipmapLevel (int iLevel, RasterImagePtr& rspqMipmapLevel);
    bool LoadFacePositiveYMipmapLevel (int iLevel, RasterImagePtr& rspqMipmapLevel);
    bool LoadFaceNegativeYMipmapLevel (int iLevel, RasterImagePtr& rspqMipmapLevel);
    bool LoadFacePositiveZMipmapLevel (int iLevel, RasterImagePtr& rspqMipmapLevel);
    bool LoadFaceNegativeZMipmapLevel (int iLevel, RasterImagePtr& rspqMipmapLevel);

    // Loading of a slice of a volume texture.  Parameter iSlice must be in the range
    // [0, volume_texture_depth-1].  Progressed.
    bool LoadVolumeTextureSlice (int iSlice, RasterImagePtr& rspqSlice);

    // Loading of a mipmap level of a volume texture.  Parameter iLevel must be in the range
    // [0, resolution_level_quantity-1]; level 0 identifies the first downsampled mipmap level.  Progressed.
    bool LoadVolumeTextureMipmapLevel (int iLevel, RasterImagePtr& rspqMipmapLevel);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

private:
    int GetMaxPossibleResolutionLevelQuantity () const;
    void InitAndPrepareForSaving (const RasterImagePtr& rspqMainImage, int iResolutionLevelQuantity);
    void WriteImage (const RasterImagePtr& rspqImage, ProgressProbe& rqProgress, float fScopeCoefficient,
        Stream& rqStream);
    bool ReadImage (int iOffset, int iWidth, int iHeight, RasterImagePtr& rspqImage);
    int CalculateImageOccupancy (int iWidth, int iHeight) const;
    void WriteMipmapLevels (const RasterImagePtr& rspqFullResImage, ProgressProbe& rqProgress,
        float fScopeCoefficient, Stream& rqStream);
    void WriteCustomMipmapLevels (const StillArray<RasterImagePtr>& rqCustomMipmaps, ProgressProbe& rqProgress,
        float fScopeCoefficient, Stream& rqStream);
    bool ReadMipmapLevel (int iLevel, int iImageDataOffset, RasterImagePtr& rspqMipmapLevel);

    RasterImagePtr m_spq2DTextureSaveTo;
    RasterImagePtr m_aspqCubicEnvMapSaveTo[6];
    StillArray<RasterImagePtr> m_qSlices;
    bool m_bHasCustomMipmaps;
    StillArray<RasterImagePtr> m_qCustomMipmaps;
    StillArray<RasterImagePtr> m_aqCEMCustomMipmaps[6];
    PackingType m_ePacking;
    int m_iMainImageWidth, m_iMainImageHeight;
    int m_iVolumeTextureDepth;
    int m_iResolutionLevelQuantity;
    CompressionType m_eCompression;
    unsigned int m_uiRedBitMask, m_uiGreenBitMask, m_uiBlueBitMask, m_uiAlphaBitMask;
    int
        m_iRedLeftShift, m_iRedMaxValue, m_iRedLength, m_iRedRightShift,
        m_iGreenLeftShift, m_iGreenMaxValue, m_iGreenLength, m_iGreenRightShift,
        m_iBlueLeftShift, m_iBlueMaxValue, m_iBlueLength, m_iBlueRightShift,
        m_iAlphaLeftShift, m_iAlphaMaxValue, m_iAlphaRightShift;
    bool m_bEightBitsPerSample;
    bool m_bHasNonFullResImages;
    ResourceType m_eResource;
    bool
        m_bHasFacePositiveX, m_bHasFaceNegativeX,
        m_bHasFacePositiveY, m_bHasFaceNegativeY,
        m_bHasFacePositiveZ, m_bHasFaceNegativeZ;
    int m_iBitsPerPixel;
    ColorModelType m_eColorModel;
    int m_iOccupancyPerCubeFace;
    bool m_bDxtCompression;
    DxtCompressionQualityType m_eDxtQuality;                // (default: DXTCQT_MEDIUM)
    float m_fDxtWeigFcrR, m_fDxtWeigFcrG, m_fDxtWeigFcrB;   // (default: 0.3086, 0.6094, 0.082)
    int m_iDxt1aThreshold;                                  // (default: 128)
};

#include "QFormatDds.inl"

}

#endif



