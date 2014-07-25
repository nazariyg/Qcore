#ifndef QFORMATTGA_H
#define QFORMATTGA_H

// Targa Image Format.  File extensions: .tga .vda .icb .vst.  The implementation is done on the level of
// "Original TGA Format".
//
// For saving, acceptable RasterImage's color models are: CMT_PALETTED, CMT_GRAYSCALE, CMT_RGB, and CMT_RGBA.
// The possible bits-per-pixel values in conjunction with the color models are noted below:
//   RasterImage::CMT_PALETTED  : the bits-per-pixel value must be 8;
//   RasterImage::CMT_GRAYSCALE : the bits-per-pixel value must be 8;
//   RasterImage::CMT_RGB       : the bits-per-pixel value can be 16 or 24;
//   RasterImage::CMT_RGBA      : the bits-per-pixel value must be 32.
// A provided image must be without a visibility mask.  An image also must have dimensions which do not exceed
// 65535.  To be on the safe side, saving appends the signature-like TGA footer of "New TGA Format".
// DEFAULTS. Compression is CT_NONE.
//
// For loading, the next image types can appear: IT_PALETTED, IT_GRAYSCALE_8, IT_RGB_8_8_8, and
// IT_RGBA_8_8_8_8.  An image arrives without a visibility mask.  NOTE. This format has no established
// signature in its design.

#include "QFormatBase.h"
#include "QProgressBase.h"
#include "QRasterImage.h"

namespace Q
{

class Q_ITEM FormatTga : public FormatBase, public ProgressBase
{
public:
    enum CompressionType
    {
        CT_NONE,
        CT_RLE
    };

    // relates only to loading
    enum ColorModelType
    {
        CMT_PALETTED,
        CMT_GRAYSCALE,
        CMT_RGB,
        CMT_RGBA
    };

    enum ExceptionType
    {
        ET_CANNOT_ACCESS_FILE,  // saving and loading
        ET_FORMAT_MISMATCH  // loading
    };

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    // Construction.  For saving.  Parameter iBitsPerPixel can be omitted only for paletted images, and in this
    // case the bpp value is determined automatically.
    FormatTga (const RasterImagePtr& rspqImage, int iBitsPerPixel = -1);

    // setting of compression
    void SetCompression (CompressionType eCompression);

    // Saving.  Progressed.
    bool SaveImage (const char* acFilename);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    // Construction.  For loading.  Initially tries to read the heading data.
    FormatTga (const char* acFilename, bool& rbSuccess);

    // file attributes
    int GetWidth () const;
    int GetHeight () const;
    int GetBitsPerPixel () const;
    ColorModelType GetColorModel () const;
    CompressionType GetCompression () const;

    // Loading.  Progressed.
    bool LoadImage (RasterImagePtr& rspqImage);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

private:
    enum ColorMapPresenceType
    {
        CMPT_NOT_INCLUDED,
        CMPT_INCLUDED
    };

    enum ImageType
    {
        IT_PALETTED,
        IT_CHANNELED,
        IT_GRAYSCALE
    };

    RasterImagePtr m_spqImageSaveTo;
    int m_iWidth, m_iHeight;
    ImageType m_eImage;                 // (default: autodetect)
    CompressionType m_eCompression;     // (default: CT_NONE)
    bool m_bPaletted;
    int m_iBitsPerPixel, m_iPaletteSize, m_iIdLength, m_iFirstEntryIndex, m_iBitsPerPaletteColor,
        m_iBitsPerAttribute, m_iImageOrientation;
    char m_cImageType;
};

#include "QFormatTga.inl"

}

#endif



