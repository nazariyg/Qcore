#ifndef QFORMATSGI_H
#define QFORMATSGI_H

// SGI Image Format.  File extensions: .sgi .rgb .bw .rgba .int .inta.
//
// For saving, acceptable RasterImage's color models are: CMT_GRAYSCALE, CMT_GRAYSCALE_WITH_ALPHA, CMT_RGB, and
// CMT_RGBA.  The possible bits-per-pixel values in conjunction with the color models are noted below:
//   RasterImage::CMT_GRAYSCALE            : the bits-per-pixel value can be 8 or 16;
//   RasterImage::CMT_GRAYSCALE_WITH_ALPHA : the bits-per-pixel value can be 16 or 32;
//   RasterImage::CMT_RGB                  : the bits-per-pixel value can be 24 or 48;
//   RasterImage::CMT_RGBA                 : the bits-per-pixel value can be 32 or 64.
// An image must be without a visibility mask.  An image also must have dimensions which do not exceed 65535.
// SGI files with grayscale-with-alpha color model and ones having two bytes per sample -- are creatable, but
// rarely read by common graphical applications, especially when RLE compression is used in addition.  The
// implementation of RLE compression strictly follows the rules stated in the format specification about
// putting of "count of 0" termination code to flag the end of scanline encoding; that makes wider the
// aggregation of programs which will read files saved by this class.
// DEFAULTS. Compression is CT_NONE.
//
// For loading, an image can appear with type IT_GRAYSCALE_8, IT_GRAYSCALE_16, IT_GRAYSCALE_WITH_ALPHA_8_8,
// IT_GRAYSCALE_WITH_ALPHA_16_16, IT_RGB_8_8_8, IT_RGB_16_16_16, IT_RGBA_8_8_8_8, or IT_RGBA_16_16_16_16.
// An image arrives without a visibility mask.

#include "QFormatBase.h"
#include "QProgressBase.h"
#include "QRasterImage.h"

namespace Q
{

class Q_ITEM FormatSgi : public FormatBase, public ProgressBase
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
        CMT_GRAYSCALE,
        CMT_GRAYSCALE_WITH_ALPHA,
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

    // Construction.  For saving.
    FormatSgi (const RasterImagePtr& rspqImage, int iBitsPerPixel);

    // setting of compression
    void SetCompression (CompressionType eCompression);

    // Saving.  Progressed.
    bool SaveImage (const char* acFilename);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    // Construction.  For loading.  Initially tries to read the heading data.
    FormatSgi (const char* acFilename, bool& rbSuccess);

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
    static unsigned short ExtractUShortValue (const StillArray<unsigned char>& rqUncompressedScanline,
        int iSamplePos);

    RasterImagePtr m_spqImageSaveTo;
    CompressionType m_eCompression;             // (default: CT_NONE)
    int m_iBytesPerSample, m_iSampleQuantity;
    int m_iBitsPerPixel;
    int m_iWidth, m_iHeight;
    ColorModelType m_eColorModel;
};

#include "QFormatSgi.inl"

}

#endif



