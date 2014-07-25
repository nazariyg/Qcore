#ifndef QFORMATHDR_H
#define QFORMATHDR_H

// High Dynamic Range Image Format.  File extensions: .hdr .pic.
//
// For saving, only images with RasterImage's CMT_RGB color model are acceptable.  A RasterImage object is not
// required to have the high-dynamic-range flag set.  An image must be without a visibility mask.  Concerning
// usage or advanced features, it should be noted that almost all common HDR readers do not take note of
// exposure, color correction, and gamma; also, they are able to recognize only OT_TOP_LEFT orientation type.
// DEFAULTS. Compression is CT_NONE; exposure, color correction, software, pixel aspect ratio, and gamma
// informations are absent; orientation is OT_TOP_LEFT.
//
// For loading, images appear with IT_RGB_FLOAT type and with the high-dynamic-range flag set.  Without a
// visibility mask associated.  NOTE. This format has no signature in its design.  Images with XYZ color model
// are supported (become RGB on loading).  Alas, due to the format's design, it is unknown after construction
// if the image is wholly or partially RLE compressed.  WARNING. If an image, which is about to be loaded, has
// one of the next orientation types: OT_LEFT_TOP, OT_RIGHT_TOP, OT_RIGHT_BOTTOM, or OT_LEFT_BOTTOM, then an
// arrived RasterImage object will have width and height different from those returned by GetWidth and
// GetHeight member functions, because of a rotation on 90 degrees.
// DEFAULTS. By default, the loading process does not take into account exposure and color correction.

#include "QFormatBase.h"
#include "QProgressBase.h"
#include "QRasterImage.h"

namespace Q
{

class Q_ITEM FormatHdr : public FormatBase, public ProgressBase
{
public:
    // relates only to saving
    enum CompressionType
    {
        CT_NONE,
        CT_RLE
    };

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

    enum ExceptionType
    {
        ET_CANNOT_ACCESS_FILE,  // saving and loading
        ET_FORMAT_MISMATCH  // loading
    };

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    // Construction.  For saving.
    FormatHdr (const RasterImagePtr& rspqImage);

    // file settings
    void SetCompression (CompressionType eCompression);
    void SetExposure (float fExposure);
    void SetColorCorrection (float fCorrR, float fCorrG, float fCorrB);
    void SetSoftware (const String& rqSoftware);
    void SetPixelAspectRatio (float fPixelAspectRatio);
    void SetGamma (float fGamma);
    void SetOrientation (OrientationType eOrientation);

    // Saving.  Progressed.
    bool SaveImage (const char* acFilename);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    // Construction.  For loading.  Initially tries to read the heading data.
    FormatHdr (const char* acFilename, bool& rbSuccess);

    // file attributes
    int GetWidth () const;
    int GetHeight () const;
    bool HasSoftwareInfo () const;
    const String& GetSoftware () const;
    bool HasPixelAspectRatioInfo () const;
    float GetPixelAspectRatio () const;
    bool HasGammaInfo () const;
    float GetGamma () const;
    OrientationType GetOrientation () const;

    // may be called to change the way of processing of pixel values
    void SetDoRelyOnExposureAndColorCorrection (bool bSet);

    // Loading.  Progressed.
    bool LoadImage (RasterImagePtr& rspqImage);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

private:
    enum FormatType
    {
        FT_RLE_RGBE,
        FT_RLE_XYZE
    };

    static void GetByteQuartetFromFloatTriplet (const float afFloat[3], unsigned char aucByte[4]);
    static void GetFloatTripletFromByteQuartet (const unsigned char aucByte[4], float afFloat[3]);

    RasterImagePtr m_spqImageSaveTo;
    CompressionType m_eCompression;     // (default: CT_NONE)
    FormatType m_eFormat;
    bool m_bHasExposureInfo;            // (default: false)
    float m_fExposure;
    bool m_bHasColorCorrectionInfo;     // (default: false)
    float m_afColorCorr[3];
    bool m_bHasSoftwareInfo;            // (default: false)
    String m_qSoftware;
    bool m_bHasPixelAspectRatioInfo;    // (default: false)
    float m_fPixelAspectRatio;
    bool m_bHasGammaInfo;               // (default: false)
    float m_fGamma;
    bool m_bInvCompCoeffsAreMeaning;
    float m_afInvCompCoeffs[3];
    Matrix3f m_qXyzToRgbMatrix;
    int m_iWidth, m_iHeight;
    OrientationType m_eOrientation;     // (default: OT_TOP_LEFT)
    int m_iImageDataPos;
    bool m_bDoRelyOnExposureAndColorCorrection;  // default: false

    // support for conversion from XYZ color model to RGB
    static const float ms_fXyzToRgbGamma;  // 1.0
};

#include "QFormatHdr.inl"

}

#endif



