#ifndef QFORMATSCT_H
#define QFORMATSCT_H

// Scitex CT Image Format.  File extension: .sct.  Number of bits per sample is always 8 for SCT format.
//
// For saving, acceptable RasterImage's color models are: CMT_GRAYSCALE, CMT_RGB, and CMT_CMYK.  A provided
// image must be without a visibility mask.
// DEFAULTS. Horizontal and vertical resolutions are 72 dpi; orientation is OT_TOP_LEFT.
//
// For loading, an image can appear with type IT_GRAYSCALE_8, IT_RGB_8_8_8, or IT_CMYK_8_8_8_8.  An image
// arrives without a visibility mask.  WARNING. If an image, which is about to be loaded, has one of the next
// orientation types: OT_LEFT_TOP, OT_RIGHT_TOP, OT_RIGHT_BOTTOM, or OT_LEFT_BOTTOM, then an arrived
// RasterImage object will have width and height different from those returned by GetWidth and GetHeight member
// functions, because of a rotation on 90 degrees.

#include "QFormatBase.h"
#include "QProgressBase.h"
#include "QRasterImage.h"

namespace Q
{

class Q_ITEM FormatSct : public FormatBase, public ProgressBase
{
public:
    enum OrientationType
    {
        OT_TOP_LEFT,      // 0th row is at the image's visual top, 0th column is the visual left-hand side
        OT_TOP_RIGHT,     // 0th row is at the image's visual top, 0th column is the visual right-hand side
        OT_BOTTOM_RIGHT,  // 0th row is at the image's visual bottom, 0th column is the visual right-hand side
        OT_BOTTOM_LEFT,   // 0th row is at the image's visual bottom, 0th column is the visual left-hand side
        OT_LEFT_TOP,      // 0th row is the image's visual left-hand side, 0th column is the visual top
        OT_RIGHT_TOP,     // 0th row is the image's visual right-hand side, 0th column is the visual top
        OT_RIGHT_BOTTOM,  // 0th row is the image's visual right-hand side, 0th column is the visual bottom
        OT_LEFT_BOTTOM    // 0th row is the image's visual left-hand side, 0th column is the visual bottom
    };

    // relates only to loading
    enum ColorModelType
    {
        CMT_GRAYSCALE,
        CMT_RGB,
        CMT_CMYK
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
    FormatSct (const RasterImagePtr& rspqImage);

    // file settings
    void SetDpi (int iHorizontalDpi, int iVerticalDpi);
    void SetOrientation (OrientationType eOrientation);
    void SetNameComment (const String& rqNameComment);  // a string must be <= 80 in length

    // Saving.  Progressed.
    bool SaveImage (const char* acFilename);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    // Construction.  For loading.  Initially tries to read the heading data.
    FormatSct (const char* acFilename, bool& rbSuccess);

    // file attributes
    int GetWidth () const;
    int GetHeight () const;
    ColorModelType GetColorModel () const;
    OrientationType GetOrientation () const;
    int GetHorizontalDpi () const;
    int GetVerticalDpi () const;
    const String& GetNameComment () const;

    // Loading.  Progressed.
    bool LoadImage (RasterImagePtr& rspqImage);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

private:
    enum MeasurementUnitType
    {
        MUT_MILLIMETER,
        MUT_INCH
    };

    static void WriteString (const String& rqString, Stream& rqStream);
    static void WriteInteger (int iValue, Stream& rqStream);
    static void WriteLongInteger (int iValue, Stream& rqStream);
    static void WriteFloat (float fValue, Stream& rqStream);
    static bool ReadString (String& rqString, Stream& rqStream);
    static bool ReadInteger (int& riValue, Stream& rqStream);
    static bool ReadLongInteger (int& riValue, Stream& rqStream);
    static bool ReadFloat (float& rfValue, Stream& rqStream);

    RasterImagePtr m_spqImageSaveTo;
    String m_qNameComment;                  // (default: empty)
    ColorModelType m_eColorModel;
    int m_iWidth, m_iHeight;
    int m_iHorizontalDpi;                   // (default: 72)
    int m_iVerticalDpi;                     // (default: 72)
    unsigned char m_ucNumColorSeparations;
    OrientationType m_eOrientation;         // (default: OT_TOP_LEFT)
};

#include "QFormatSct.inl"

}

#endif



