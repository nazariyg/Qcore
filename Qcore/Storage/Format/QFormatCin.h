#ifndef QFORMATCIN_H
#define QFORMATCIN_H

// Kodak Cineon Image Format.  File extension: .cin.  The class writes and reads RGB images with 10 bits per
// sample in the CIN's logarithmic format.
//
// For saving, a provided image must have RasterImage's color model CMT_RGB.  An image is to be without a
// visibility mask.  Saving writes "V4.5" as the format's version.  Multibyte fields are written using
// Big-endian order.  Non-critical informational fields are filled with zeroes.
//
// For loading, a created RasterImage object has the type IT_RGB_16_16_16 (since bpp is 30).  An image arrives
// without a visibility mask.  Loading is able to handle both cases of byte ordering.

#include "QFormatBase.h"
#include "QProgressBase.h"
#include "QRasterImage.h"

namespace Q
{

class Q_ITEM FormatCin : public FormatBase, public ProgressBase
{
public:
    // ET_FORMAT_IDENTIFIER_MISMATCH: signature discrepancy, causes no assert-false
    enum ExceptionType
    {
        ET_CANNOT_ACCESS_FILE,              // saving and loading
        ET_FORMAT_IDENTIFIER_MISMATCH,  // loading
        ET_FORMAT_MISMATCH,             // loading
        ET_UNSUPPORTED_FILE                 // loading
    };

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    // Construction.  For saving.
    FormatCin (const RasterImagePtr& rspqImage);

    // Saving.  Progressed.
    bool SaveImage (const char* acFilename);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    // Construction.  For loading.  Initially tries to read the heading data.
    FormatCin (const char* acFilename, bool& rbSuccess);

    // file attributes
    int GetWidth () const;
    int GetHeight () const;

    // Loading.  Progressed.
    bool LoadImage (RasterImagePtr& rspqImage);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

private:
    RasterImagePtr m_spqImageSaveTo;
    int m_iImageDataOffset;
    int m_iWidth, m_iHeight;

    // support for color conversion
    static const int ms_iRefBlack;      // 0
    static const int ms_iRefWhite;      // 684
    static const float ms_fDispGamma;   // 1.7
};

#include "QFormatCin.inl"

}

#endif



