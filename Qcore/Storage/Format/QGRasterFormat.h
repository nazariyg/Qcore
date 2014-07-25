#ifndef QGRASTERFORMAT_H
#define QGRASTERFORMAT_H

// The class provides the interface for image format detection and for image loading that identifies the format
// automatically and outputs an image into one of the standard layouts: IT_RGB_8_8_8 or IT_RGBA_8_8_8_8.  For
// the member functions that take a filename for input, the file's extension is not required to represent the
// true formatting and may even be absent; however, a relevant file extension makes format recognition a bit
// smoother.

#include "QExptWarnBase.h"
#include "QProgressBase.h"
#include "QRasterImage.h"

namespace Q
{

class Q_ITEM GRasterFormat : public ExptWarnBase, public ProgressBase
{
public:
    enum FormatType
    {
        FT_BMP,
        FT_CIN,
        FT_DDS,
        FT_EXR,
        FT_GIF,
        FT_HDR,
        FT_ICO,
        FT_IFF,
        FT_JPEG,
        FT_PCX,
        FT_PNG,
        FT_PSD,
        FT_RAS,
        FT_RLA,
        FT_SCT,
        FT_SGI,
        FT_TGA,
        FT_TIFF,
        FT_WBMP
    };

    enum ExceptionType
    {
        ET_CANNOT_ACCESS_FILE,
        ET_FORMAT_MISMATCH,
        ET_FORMAT_UNRECOGNIZED
    };

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    // static function to detect the format of an image file; returns success
    static bool DetectFormat (const char* acFilename, FormatType& reFormat);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    // Construction.  Initially tries to sense the format.
    GRasterFormat (const char* acFilename, bool& rbSuccess);

    // destruction
    ~GRasterFormat ();

    // for informative purposes
    FormatType GetFormat () const;

    // Image loading.  With the formats that can store multiple images, the function loads the first image in
    // case of GIF, ICO, and TIFF, the main/title image in case of DDS and EXR, and the composite image in case
    // of PSD.  When loaded, the image's type is IT_RGB_8_8_8 or IT_RGBA_8_8_8_8, depending whether
    // transparency was implied by the storage.  Progressed.
    bool LoadImage (RasterImagePtr& rspqImage);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

private:
    bool InitAsBmp ();
    bool InitAsCin ();
    bool InitAsDds ();
    bool InitAsExr ();
    bool InitAsGif ();
    bool InitAsHdr ();
    bool InitAsIco ();
    bool InitAsIff ();
    bool InitAsJpeg ();
    bool InitAsPcx ();
    bool InitAsPng ();
    bool InitAsPsd ();
    bool InitAsRas ();
    bool InitAsRla ();
    bool InitAsSct ();
    bool InitAsSgi ();
    bool InitAsTga ();
    bool InitAsTiff ();
    bool InitAsWbmp ();

    virtual void OnException (int iExceptionType);

    String m_qFilename;
    bool m_bFormatUnrecognized;  // default: false
    FormatType m_eFormat;
    FILE* m_pqFile;
};

#include "QGRasterFormat.inl"

}

#endif



