#ifndef QFORMATWBMP_H
#define QFORMATWBMP_H

// Wireless Bitmap Format.  File extensions: .wbmp .wbm.  The implementation accepts for saving and produces on
// loading only a RasterImage object with type IT_PALETTED and having its palette filled by two color entries:
// black and white (in this order).  Visibility masks are not supported by the format.  NOTE. This format has
// no signature in its design.

#include "QFormatBase.h"
#include "QProgressBase.h"
#include "QRasterImage.h"

namespace Q
{

class Q_ITEM FormatWbmp : public FormatBase, public ProgressBase
{
public:
    // ET_UNSUPPORTED_WBMP_TYPE: this image has a type which differs from "WBMP Type 0" (B/W, uncompressed), or
    //   it looks like the file contains extension header field(s) forbidden for "Type 0"
    enum ExceptionType
    {
        ET_CANNOT_ACCESS_FILE,    // saving and loading
        ET_FORMAT_MISMATCH,   // loading
        ET_UNSUPPORTED_WBMP_TYPE  // loading
    };

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    // Construction.  For saving.
    FormatWbmp (const RasterImagePtr& rspqImage);

    // Saving.  Progressed.
    bool SaveImage (const char* acFilename);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    // Construction.  For loading.  Initially tries to read the heading data.
    FormatWbmp (const char* acFilename, bool& rbSuccess);

    // image dimensions
    int GetWidth () const;
    int GetHeight () const;

    // Loading.  Progressed.
    bool LoadImage (RasterImagePtr& rspqImage);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

private:
    static void WriteMultiByteUI32 (unsigned int uiValue, Stream& rqStream);
    static bool ReadMultiByteUI32 (unsigned int& ruiValue, Stream& rqStream);

    RasterImagePtr m_spqImageSaveTo;
    int m_iWidth, m_iHeight;
    int m_iFileSize, m_iImageStartByte;
};

#include "QFormatWbmp.inl"

}

#endif



