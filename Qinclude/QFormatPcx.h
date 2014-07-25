#ifndef QFORMATPCX_H
#define QFORMATPCX_H

// ZSoft Image Format (PaintBrush).  File extension: .pcx.
//
// For saving, acceptable RasterImage's color models are: CMT_PALETTED, CMT_GRAYSCALE, and CMT_RGB.  The
// possible bits-per-pixel values in conjunction with the color models are noted below:
//   RasterImage::CMT_PALETTED  : the bits-per-pixel value can be 1, 2, 4, or 8;
//   RasterImage::CMT_GRAYSCALE : the bits-per-pixel value can be 2, 4, or 8;
//   RasterImage::CMT_RGB       : the bits-per-pixel value can be 3, 6, 12, or 24.
// For paletted images, the bpp value must allow image's pixels to address any entry in the palette, making
// true the next: 2^bpp >= palette_size.  Images must be without a visibility mask.  An image for saving must
// have dimensions which do not exceed 65536.  Bits-per-pixel value 1 is not allowed for CMT_GRAYSCALE in order
// to avoid some decoder's confusion about black-and-white images.  NOTE. PCX files with bits-per-sample value
// 1 for RGB images, and also 2 and 4 for any image kind, are rarely supported by common graphical
// applications.
// DEFAULTS. Horizontal and vertical resolutions both are 72 dpi.
//
// For loading, the next image types can appear: IT_PALETTED, IT_GRAYSCALE_8, and IT_RGB_8_8_8.  Without a
// visibility mask.  Loading treats the first three bytes as a signature.  Images with color plane number 1, 3,
// and 4 are readable.  The whole range of PCX's bits-per-sample values is supported.

#include "QFormatBase.h"
#include "QProgressBase.h"
#include "QRasterImage.h"

namespace Q
{

class Q_ITEM FormatPcx : public FormatBase, public ProgressBase
{
public:
    // relates only to loading
    enum ColorModelType
    {
        CMT_PALETTED,
        CMT_GRAYSCALE,
        CMT_RGB
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
    FormatPcx (const RasterImagePtr& rspqImage, int iBitsPerPixel = -1);

    // resolution settings; in dpi
    void SetResolution (int iHorizontalResolution, int iVerticalResolution);

    // Saving.  Progressed.
    bool SaveImage (const char* acFilename);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    // Construction.  For loading.  Initially tries to read data from the header.
    FormatPcx (const char* acFilename, bool& rbSuccess);

    // file attributes
    int GetWidth () const;
    int GetHeight () const;
    int GetBitsPerPixel () const;
    ColorModelType GetColorModel () const;
    int GetHorizontalResolution () const;  // in dpi
    int GetVerticalResolution () const;    // in dpi

    // Loading.  Progressed.
    bool LoadImage (RasterImagePtr& rspqImage);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

private:
    RasterImagePtr m_spqImageSaveTo;
    int m_iBitsPerSample;
    int m_iWidth, m_iHeight;
    int m_iHorizontalResolution;    // (default: 72)
    int m_iVerticalResolution;      // (default: 72)
    int m_iColorPlaneQuantity;
    int m_iBitsPerPixel;
    int m_iBytesPerPlaneRow;
    ColorModelType m_eColorModel;
};

#include "QFormatPcx.inl"

}

#endif



