#ifndef QFORMATIFF_H
#define QFORMATIFF_H

// Amiga & Electronic Arts Interleaved Bitmap Format (ILBM).  File extensions: .iff .ilbm .lbm .ham .ham8 .ehb.
//
// For saving, acceptable RasterImage's color models are: CMT_PALETTED, CMT_GRAYSCALE, CMT_RGB, and CMT_RGBA.
// The possible bits-per-pixel values in conjunction with the color models are noted below:
//   RasterImage::CMT_PALETTED  : the bits-per-pixel value must be in the range [1, 8];
//   RasterImage::CMT_GRAYSCALE : the bits-per-pixel value must be 8;
//   RasterImage::CMT_RGB       : the bits-per-pixel value must be 24;
//   RasterImage::CMT_RGBA      : the bits-per-pixel value must be 32.
// For paletted images, the bpp value must allow image's pixels to address any entry in the palette, making
// true the next: 2^bpp >= palette_size.  A visibility mask can be saved into IFF representation, however
// masked IFF images are rarely supported by mainstream graphical applications.  An image provided for saving
// must have dimensions which do not exceed 65535.  Conforming to the common practice in regard to
// CMT_GRAYSCALE images, the grayscale palette will be written to avoid some decoder's confusion (these images
// will be treated as paletted when being loaded by this class).
// DEFAULTS. Compression is set to CT_NONE; DPI chunk is enabled with horizontal and vertical dpi set to 72.
//
// For loading, the next image types can result: IT_PALETTED, IT_GRAYSCALE_8, IT_RGB_8_8_8, and
// IT_RGBA_8_8_8_8.  Number of supported IFF bit planes is from 1 to 32.  Only the first two IFF masking
// techniques are supported as suggested by the format specification, which means that an image can arrive with
// a visibility mask.  Three Commodore Amiga imaging specifics are supported: Hold-and-Modify (HAM),
// Hold-and-Modify with 8-bit codes (HAM8), and Extra-Halfbrite (EHB).  NOTE. Due to the format's peculiarity,
// if GetPalettedImageSpecific member function after construction returns PIST_HAM or PIST_HAM_8, then the
// output image will be actually RGB but not paletted (GetColorModel will return CMT_PALETTED).  Handling of
// HAM and HAM8 involves betterments empowering reading of IFF images in the same way as NewTek LightWave does.

#include "QFormatBase.h"
#include "QProgressBase.h"
#include "QRasterImage.h"

namespace Q
{

class Q_ITEM FormatIff : public FormatBase, public ProgressBase
{
public:
    enum CompressionType
    {
        CT_NONE,
        CT_RLE  // PackBits compression scheme
    };

    // relates only to loading
    enum ColorModelType
    {
        CMT_PALETTED,
        CMT_GRAYSCALE,
        CMT_RGB,
        CMT_RGBA
    };

    // relates only to loading
    enum PalettedImageSpecificType
    {
        PIST_NONE,
        PIST_HAM,
        PIST_HAM_8,
        PIST_EHB
    };

    // ET_FORMAT_IDENTIFIER_MISMATCH: signature discrepancy, causes no assert-false
    enum ExceptionType
    {
        ET_CANNOT_ACCESS_FILE,              // saving and loading
        ET_FORMAT_IDENTIFIER_MISMATCH,  // loading
        ET_FORMAT_MISMATCH              // loading
    };

    // WT_UNSUPPORTED_MASKING_TECHNIQUE: "Transparent Color" and "Lasso" masking methods are not supported
    enum WarningType
    {
        WT_UNSUPPORTED_MASKING_TECHNIQUE,  // loading
        WT_DPI_CHUNK_IS_INCORRECT          // loading
    };

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    // Construction.  For saving.  Parameter iBitsPerPixel can be omitted only for paletted images, and in this
    // case the minimal possible bpp is automatically derived from the palette size.
    FormatIff (const RasterImagePtr& rspqImage, int iBitsPerPixel = -1);

    // file settings
    void SetCompression (CompressionType eCompression);
    void UnsetDpi ();  // turns DPI chunk off
    void SetDpi (int iHorizontalDpi, int iVerticalDpi);

    // Saving.  Progressed.
    bool SaveImage (const char* acFilename);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    // Construction.  For loading.  Initially tries to read the heading data (from BMHD and DPI chunks).
    FormatIff (const char* acFilename, bool& rbSuccess);

    // file attributes
    int GetWidth () const;
    int GetHeight () const;
    int GetBitsPerPixel () const;
    ColorModelType GetColorModel () const;
    CompressionType GetCompression () const;
    bool HasDPIChunk () const;
    int GetHorizontalDpi () const;
    int GetVerticalDpi () const;
    PalettedImageSpecificType GetPalettedImageSpecific () const;

    // Loading.  Progressed.
    bool LoadImage (RasterImagePtr& rspqImage);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

private:
    enum MaskingType
    {
        MT_NONE,
        MT_HAS_MASK
    };

    RasterImagePtr m_spqImageSaveTo;
    int m_iNextReadPos;
    int m_iWidth, m_iHeight;
    int m_iBitPlaneQuantity, m_iBitsPerPixel;
    MaskingType m_eMasking;
    CompressionType m_eCompression;             // (default: CT_NONE)
    RasterImage::Palette m_qPalette;
    bool m_bHasDPIChunk;                        // (default: true)
    int m_iHorizontalDpi;                       // (default: 72)
    int m_iVerticalDpi;                         // (default: 72)
    ColorModelType m_eColorModel;
    PalettedImageSpecificType m_ePalettedImageSpecific;
};

#include "QFormatIff.inl"

}

#endif



