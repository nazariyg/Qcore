#ifndef QFORMATRAS_H
#define QFORMATRAS_H

// Sun Rasterfile Image Format.  File extension: .ras.
//
// For saving, acceptable RasterImage's color models are: CMT_PALETTED, CMT_GRAYSCALE, CMT_RGB, and CMT_RGBA.
// The possible bits-per-pixel values in conjunction with the color models are noted below:
//   RasterImage::CMT_PALETTED  : the bits-per-pixel value can be 1 or 8;
//   RasterImage::CMT_GRAYSCALE : the bits-per-pixel value can be 1 or 8;
//   RasterImage::CMT_RGB       : the bits-per-pixel value must be 24;
//   RasterImage::CMT_RGBA      : the bits-per-pixel value must be 32.
// For paletted images, the bpp value must allow image's pixels to address any entry in the palette, making
// true the next: 2^bpp >= palette_size.  An image must be without a visibility mask.  For images with
// CMT_GRAYSCALE color model, the RAS format's design forces to write the grayscale palette in order to avoid
// some decoder's confusion, and these images will be treated as paletted when loaded by this class.
// DEFAULTS. Compression is set to CT_NONE.
//
// For loading, the next image types can appear: IT_PALETTED, IT_GRAYSCALE_8, IT_RGB_8_8_8, and
// IT_RGBA_8_8_8_8.  An image arrives without a visibility mask.

#include "QFormatBase.h"
#include "QProgressBase.h"
#include "QRasterImage.h"

namespace Q
{

class Q_ITEM FormatRas : public FormatBase, public ProgressBase
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

    // ET_FORMAT_IDENTIFIER_MISMATCH: signature discrepancy, causes no assert-false
    enum ExceptionType
    {
        ET_CANNOT_ACCESS_FILE,              // saving and loading
        ET_FORMAT_IDENTIFIER_MISMATCH,  // loading
        ET_FORMAT_MISMATCH              // loading
    };

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    // Construction.  For saving.  Parameter iBitsPerPixel can be omitted only for paletted images, and in this
    // case the minimal possible bpp is automatically derived from the palette size.
    FormatRas (const RasterImagePtr& rspqImage, int iBitsPerPixel = -1);

    // setting of compression
    void SetCompression (CompressionType eCompression);

    // Saving.  Progressed.
    bool SaveImage (const char* acFilename);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    // Construction.  For loading.  Initially tries to read the heading data.
    FormatRas (const char* acFilename, bool& rbSuccess);

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
    RasterImagePtr m_spqImageSaveTo;
    int m_iFileSize;
    int m_iWidth, m_iHeight;
    int m_iBitsPerPixel;
    CompressionType m_eCompression;  // (default: CT_NONE)
    bool m_bOrderRgb;
    ColorModelType m_eColorModel;
    int m_iPaletteSize;
};

#include "QFormatRas.inl"

}

#endif



