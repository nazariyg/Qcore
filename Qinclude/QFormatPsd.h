#ifndef QFORMATPSD_H
#define QFORMATPSD_H

// Adobe Photoshop Document Format.  File extension (on PC platform): .psd.  The implementation is based on the
// specification from Adobe Photoshop 3.0.4 Software Development Kit; however, the list of layer blending modes
// is modernized and support for ICC profiles is added.  A PSD file encompasses a composite image, which
// represents either an independent image alone or layer merging, and optional layers.  Because transparency
// stored with a composite image is not intended to be used by any application other than Adobe Photoshop and
// also for the reason that PSD layers predominantly use the alpha-based method of composing, the next
// regulation exists: for both saving and loading, a PSD composite image always has a without-alpha color model
// but a layer image is invariably of a color model which implies presence of transparency information.  A
// layer image is allowed to have zero dimensions.
//
// For saving, the next RasterImage's color models are acceptable for a composite image: CMT_PALETTED,
// CMT_GRAYSCALE, CMT_RGB, CMT_CMYK, and CMT_LAB.  But for a layer image they are: CMT_GRAYSCALE_WITH_ALPHA,
// CMT_RGBA, CMT_CMYK_WITH_ALPHA, and CMT_LAB_WITH_ALPHA.  The possible bits-per-pixel values, to influence all
// images in the file, are noted below in conjunction with the composite-image color models:
//   RasterImage::CMT_PALETTED  : the bits-per-pixel value can be 8 or 1 (to create a PSD bitmap image);
//   RasterImage::CMT_GRAYSCALE : the bits-per-pixel value can be 8 or 16;
//   RasterImage::CMT_RGB       : the bits-per-pixel value can be 24 or 48;
//   RasterImage::CMT_CMYK      : the bits-per-pixel value can be 32 or 64;
//   RasterImage::CMT_LAB       : the bits-per-pixel value can be 24 or 48.
// NOTE. When presence of layers is intended, the bits-per-pixel value must be the lower one (8, 24, or 32) as
// the format's design imposes.  In accordance with the format and Photoshop's logic, a paletted/bitmap
// composite image cannot have any layers in addition.  An image of any kind must be without a visibility mask.
// Dimensions of an image must not exceed 30000.  Saving of a PSD file with 16 bits per sample accompanied by
// RLE compression is supported, however Adobe Photoshop and some other applications do not read this case.
// The format's 2-byte layer-count value is always written as positive.  Within a layer, the transparency
// channel always goes first, which is the common practice among graphical applications.
// DEFAULTS. Compression is set to CT_RLE; resolution information is enabled and is 72 dpi for both horizontal
// and vertical directions; ICC profile is absent.
//
// For loading, a composite image can appear with type IT_PALETTED, IT_GRAYSCALE_8, IT_GRAYSCALE_16,
// IT_RGB_8_8_8, IT_RGB_16_16_16, IT_CMYK_8_8_8_8, IT_CMYK_16_16_16_16, IT_LAB_8_8_8, or IT_LAB_16_16_16; a
// layer image can arrive with type IT_GRAYSCALE_WITH_ALPHA_8_8, IT_RGBA_8_8_8_8, IT_CMYK_WITH_ALPHA_8_8_8_8_8,
// or IT_LAB_WITH_ALPHA_8_8_8_8.  All image kinds come without a visibility mask.  A PSD duotone image is
// interpreted as grayscale, as recommended by the specification.  A PSD bitmap image becomes paletted on
// loading.  Adobe Photoshop's masks naturally transform into transparency; however, a mask isn't taken into
// account if it is marked in the file as disabled.  There is no member function like GetCompression for
// loading because compression method can vary from one color channel to another within the file.  The PSD
// flags "position relative to layer" and "invert layer mask when blending" are meaningful, ensuring
// same-as-in-Photoshop appearance of layer compositions.  Sign of the format's 2-byte layer count is ignored.
// There is no checking for "8BIM" as OS type when reading through a resource section because various
// signatures are possible.

#include "QFormatBase.h"
#include "QProgressBase.h"
#include "QRasterImage.h"

namespace Q
{

class Q_ITEM FormatPsd : public FormatBase, public ProgressBase
{
public:
    // relates only to saving
    enum CompressionType
    {
        CT_NONE,
        CT_RLE  // PackBits compression scheme
    };

    // the list of supported blending modes
    enum BlendingModeType
    {
        BMT_NORMAL,
        BMT_DISSOLVE,
        BMT_DARKEN,
        BMT_MULTIPLY,
        BMT_COLOR_BURN,
        BMT_LINEAR_BURN,
        BMT_LIGHTEN,
        BMT_SCREEN,
        BMT_COLOR_DODGE,
        BMT_LINEAR_DODGE,
        BMT_OVERLAY,
        BMT_SOFT_LIGHT,
        BMT_HARD_LIGHT,
        BMT_VIVID_LIGHT,
        BMT_LINEAR_LIGHT,
        BMT_PIN_LIGHT,
        BMT_HARD_MIX,
        BMT_DIFFERENCE,
        BMT_EXCLUSION,
        BMT_HUE,
        BMT_SATURATION,
        BMT_COLOR,
        BMT_LUMINOSITY,
        BMT_UNKNOWN  // some unidentified, relates only to loading
    };

    // support for layers
    enum ClippingType
    {
        CTT_BASE,  // the most usual case
        CTT_NON_BASE
    };

    // relates only to loading
    enum ColorModelType
    {
        CMT_BITMAP,  // black-and-white (also known as "bilevel") with 1 bit per pixel
        CMT_PALETTED,
        CMT_GRAYSCALE,
        CMT_RGB,
        CMT_CMYK,
        CMT_LAB
    };

    // ET_FORMAT_IDENTIFIER_MISMATCH: signature discrepancy, causes no assert-false
    // ET_MULTICHANNEL_COLOR_MODE_NOT_SUPPORTED: only Adobe Photoshop can interpret a multichannel PSD image
    enum ExceptionType
    {
        ET_CANNOT_ACCESS_FILE,                    // saving and loading
        ET_FORMAT_IDENTIFIER_MISMATCH,        // loading
        ET_FORMAT_MISMATCH,                   // loading
        ET_MULTICHANNEL_COLOR_MODE_NOT_SUPPORTED  // loading
    };

    // support for layer blending ranges
    class ChannelRange
    {
    public:
        unsigned char SrcBlack0, SrcBlack1, SrcWhite0, SrcWhite1;
        unsigned char DstBlack0, DstBlack1, DstWhite0, DstWhite1;
    };

    // layer container
    class Layer
    {
    public:
        // construction
        Layer () {};  // uninitialized
        Layer (const RasterImagePtr& rspqImage, int iPositionX, int iPositionY, unsigned char ucOpacity,
            BlendingModeType eBlendingMode, bool bVisible, const String& rqName, bool bTransparencyProtected,
            ClippingType eClipping = CTT_BASE, bool bHasBlendingRangesData = false)
            :
            ImagePtr(rspqImage),
            PosRect(iPositionX,iPositionY,rspqImage->GetWidth(),rspqImage->GetHeight()),
            Opacity(ucOpacity),
            BlendingMode(eBlendingMode),
            Visible(bVisible),
            Name(rqName),
            TransparencyProtected(bTransparencyProtected),
            Clipping(eClipping),
            HasBlendingRangesData(bHasBlendingRangesData)
        {
        };

        RasterImagePtr ImagePtr;
        Recti PosRect;            // can have a negative position, extensions must correspond with the image
        unsigned char Opacity;    // 0 = fully transparent ... 255 = fully opaque
        BlendingModeType BlendingMode;
        bool Visible;
        String Name;              // length must be <= 255
        bool TransparencyProtected;
        ClippingType Clipping;

        // Blending ranges will be saved if HasBlendingRangesData is 'true'; on writing, the alpha channel goes
        // first.  (Blending ranges are rarely used in PSD files.)
        bool HasBlendingRangesData;
        unsigned char CGSrcBlack0, CGSrcBlack1, CGSrcWhite0, CGSrcWhite1;
        unsigned char CGDstBlack0, CGDstBlack1, CGDstWhite0, CGDstWhite1;
        StillArray<ChannelRange> ChannelRanges;
    };

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    // Construction.  For saving.  A provided composite image must have a without-alpha color model.  Parameter
    // iBitsPerPixel can be omitted only for paletted images, and in this case the bpp value is determined
    // automatically.  Parameter bTreatPalettedAsBitmap is for saving of PSD bitmap images; it can be 'true'
    // only if the composite image is paletted and its palette has two entries: black and white (in this
    // order).
    FormatPsd (const RasterImagePtr& rspqCompositeImage, int iBitsPerPixel = -1,
        bool bTreatPalettedAsBitmap = false);

    // file settings
    void SetCompression (CompressionType eCompression);
    void SetResolutionInfo (int iHorizontalResolution, int iVerticalResolution);  // in pixels-per-inch
    void UnsetResolutionInfo ();
    void SetICCProfile (const StillArray<char>& rqICCProfile);

    // Insertion of layers for saving.  Lower layers are to be inserted first, followed by upper ones.  A layer
    // image must have a color model which assumes presence of transparency information and accords with the
    // composite image's color model (like CMT_RGBA accords with CMT_RGB), hence a paletted/bitmap image cannot
    // be a layer.
    void InsertLayer (const Layer& rqLayer);

    // Saving.  Progressed.
    bool SaveFile (const char* acFilename);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    // Construction.  For loading.  Initially tries to read the heading data.
    FormatPsd (const char* acFilename, bool& rbSuccess);

    // file attribitues
    int GetCompositeImageWidth () const;
    int GetCompositeImageHeight () const;
    int GetBitsPerPixel () const;
    ColorModelType GetColorModel () const;
    int GetLayerQuantity () const;

    // resolution; in pixels-per-inch
    bool HasResolutionInfo () const;
    int GetHorizontalResolution () const;
    int GetVerticalResolution () const;

    // ICC profile
    bool HasICCProfile () const;
    const StillArray<char>& GetICCProfile () const;

    // Loading of the composite image.  Progressed.
    bool LoadCompositeImage (RasterImagePtr& rspqImage);

    // Loading of a layer.  Parameter iLayer must be in the range [0, layer_quantity-1]; the layer with index 0
    // is the bottommost.  Progressed.
    bool LoadLayer (Layer& rqLayer, int iLayer);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

private:
    class ChannelInfo
    {
    public:
        short Id;
        unsigned int Length;
    };

    class LayerMaskRecord
    {
    public:
        LayerMaskRecord ()
            :
            IsPresent(false)
        {
        };

        bool IsPresent;
        Recti PosRect;
        unsigned char DefaultColor;
        bool PositionRelativeToLayer, LayerMaskDisabled, InvertLayerMaskWhenBlending;
    };

    class LayerRecord
    {
    public:
        Recti PosRect;
        int ChannelQuantity;
        StillArray<ChannelInfo> ChannelInfos;
        LayerMaskRecord LMR;
    };

    RasterImagePtr m_spqCRISaveTo;
    bool m_bTreatPalettedAsBitmap;
    ModestArray<Layer> m_qLayersToSave;
    CompressionType m_eCompression;     // (default: CT_RLE)
    int m_iChannelQuantity;
    int m_iCIWidth, m_iCIHeight;
    int m_iBitsPerChannel;
    ColorModelType m_eColorModel;
    bool m_bHasResolutionInfo;          // (default: true)
    int m_iHorizontalResolution;        // (default: 72)
    int m_iVerticalResolution;          // (default: 72)
    bool m_bHasICCProfile;              // default: false
    StillArray<char> m_qICCProfile;
    int m_iLayerQuantity;               // default: 0
    int m_iAfterLQOffset, m_iIDOffset;
};

#include "QFormatPsd.inl"

}

#endif



