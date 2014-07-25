#ifndef QFORMATEXR_H
#define QFORMATEXR_H

// Industrial Light & Magic High Dynamic Range Image Format.  File extension: .exr.  Since the EXR format is
// rather closed than opened, implementation had to be based on the library provided by developers (v1.6.1).
// Sadly, but the library isn't very good shaped at the time, lacks some essential features, and sometimes
// causes minor memory leaks.  All these restrain from a full format implementation while an official format
// specification isn't issued.  Both saving and loading have options similar to those present in the EXR Adobe
// Photoshop plug-in: the control over alpha association, exposure, and gamma.  Usage of custom values of
// exposure and gamma when saving an image is called "off-screen mode", but when loading it is "on-screen
// mode"; see comments at the related member functions about the modes' utility.
//
// For saving, the acceptable RasterImage's color models are: CMT_GRAYSCALE, CMT_GRAYSCALE_WITH_ALPHA, CMT_RGB,
// and CMT_RGBA.  RasterImage objects passed for saving are not required to have the high-dynamic-range flag
// set.  An EXR internal color model can be chosen when storing an image, but only a one compatible with the
// color model of that image:
//   RasterImage::CMT_GRAYSCALE            : only CMT_Y;
//   RasterImage::CMT_GRAYSCALE_WITH_ALPHA : only CMT_YA;
//   RasterImage::CMT_RGB                  : CMT_RGB or CMT_YC;
//   RasterImage::CMT_RGBA                 : CMT_RGBA or CMT_YCA.
// If the internal color model is not set, then the most matching one will be derived automatically:
//   RasterImage::CMT_GRAYSCALE            : CMT_Y;
//   RasterImage::CMT_GRAYSCALE_WITH_ALPHA : CMT_YA;
//   RasterImage::CMT_RGB                  : CMT_RGB;
//   RasterImage::CMT_RGBA                 : CMT_RGBA.
// For being able to use CMT_YC or CMT_YCA, the image must be with even dimensions because 1:2 subsampling is
// to occur.  A provided image must be without a visibility mask.
// DEFAULTS. Display and data windows are resulting from the image's dimensions and are equal; compression is
// CT_PIZ; pixel aspect ratio is 1.0; screen window center is in (0.0, 0.0); screen window width is 1.0; line
// order is LOT_INCREASING_Y; the image is not tile-oriented; the preview image is absent; without standard
// attributes (i.e. metadata); the alpha association flag is set; "off-screen" mode is turned off.
//
// For loading, the next image types can arise: IT_GRAYSCALE_FLOAT, IT_GRAYSCALE_WITH_ALPHA_FLOAT,
// IT_RGB_FLOAT, and IT_RGBA_FLOAT.  Without a visibility mask.  Surely, RasterImage objects appear with the
// high-dynamic-range flag set.
// DEFAULTS. The alpha unassociation flag is set; "on-screen" mode is turned off.

#include "QFormatBase.h"
#include "QProgressBase.h"
#include "QRasterImage.h"
#include "QBound2i.h"
#include "QDate.h"

namespace Q
{

class Q_ITEM FormatExr : public FormatBase, public ProgressBase
{
public:
    enum ColorModelType
    {
        CMT_RGB,
        CMT_RGBA,
        CMT_YC,     // luminance and chrominance; for an image with even dimensions
        CMT_YCA,    // luminance, chrominance, and alpha; for an image with even dimensions
        CMT_Y,      // luminance
        CMT_YA      // luminance and alpha
    };

    enum CompressionType
    {
        CT_NONE,
        CT_RLE,
        CT_ZIPS,
        CT_ZIP,
        CT_PIZ,
        CT_PXR24,
        CT_B44,
        CT_B44A
    };

    enum LineOrderType
    {
        LOT_INCREASING_Y,
        LOT_DECREASING_Y,
        LOT_RANDOM_Y        // for a tiled image only
    };

    enum LevelModeType
    {
        LMT_ONE_LEVEL,
        LMT_MIPMAP_LEVELS,
        LMT_RIPMAP_LEVELS
    };

    enum LevelRoundingModeType
    {
        LRMT_ROUND_DOWN,
        LRMT_ROUND_UP
    };

    enum EnvironmentMapType
    {
        EMT_LATITUDE_LONGITUDE,  // spherical
        EMT_CUBE
    };

    // relates only to loading
    enum ChannelDataType
    {
        CDT_UINT,   // 32-bit
        CDT_HALF,   // 16-bit
        CDT_FLOAT   // 32-bit
    };

    // ET_OPEN_EXR_ERROR: vague exception from deeps of OpenEXR
    // ET_FORMAT_IDENTIFIER_MISMATCH: signature discrepancy, causes no assert-false
    enum ExceptionType
    {
        ET_OPEN_EXR_ERROR,                  // saving and loading
        ET_CANNOT_ACCESS_FILE,              // loading
        ET_FORMAT_IDENTIFIER_MISMATCH,  // loading
        ET_FILE_IS_INCOMPLETE,              // loading
        ET_FORMAT_MISMATCH              // loading
    };

    // relates only to loading
    class ChannelRecord
    {
    public:
        String Name;
        ChannelDataType DataType;
        int SamplingX, SamplingY;
    };

    // support for handling of preview images
    class UcRgba
    {
    public:
        unsigned char R, G, B, A;
    };

    class KeyCode
    {
    public:
        int FilmManufacturerCode;   // [0, 99]
        int FilmTypeCode;           // [0, 99]
        int FilmRollPrefix;         // [0, 999999]
        int Count;                  // [0, 9999]; increments once every PerforationsPerCount times
        int FrameOffset;            // [0, 119]; measure is in perforations from zero-frame reference mark
        int PerforationsPerFrame;   // [1, 15]
        int PerforationsPerCount;   // [20, 120]
    };

    class TimeCode
    {
    public:
        int Hours;                  // [0, 23]
        int Minutes;                // [0, 59]
        int Seconds;                // [0, 59]
        int Frame;                  // [0, 29]
        bool DropFrame;
        bool ColorFrame;
        bool FieldPhase;
        bool Bgf0;
        bool Bgf1;
        bool Bgf2;
        int BinaryGroup[8];         // each is in [0, 15]
    };

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    // Construction.  For saving.
    FormatExr (const RasterImagePtr& rspqImage);

    // File settings.

    // predefined attributes
    void SetImageDisplayWindow (const Bound2i& rqWindow);
    void SetImageDataWindow (const Bound2i& rqWindow);          // must correspond with the image's dimensions
    void SetColorModel (ColorModelType eColorModel);
    void SetCompression (CompressionType eCompression);
    void SetPixelAspectRatio (float fPixelAspectRatio);         // must be > 0.0
    void SetScreenWindowCenter (float fX, float fY);
    void SetScreenWindowWidth (float fWidth);                   // must be > 0.0
    void SetLineOrder (LineOrderType eLineOrder);

    // tile description; calling of this function turns on tiled image output
    void SetTileDescription (LevelModeType eLevelMode, LevelRoundingModeType eLevelRoundingMode = LRMT_ROUND_DOWN,
        int iTileWidth = 16, int iTileHeight = 16);

    // preview image; pixel array is non-padded continuous sequence in the standard manner (left-to-right,
    // top-to-bottom)
    void SetPreviewImage (int iWidth, int iHeight, const StillArray<UcRgba>& rqPixels);

    // primary chromaticities and white point
    void SetChromaticities (
        float fRedX, float fRedY,
        float fGreenX, float fGreenY,
        float fBlueX, float fBlueY,
        float fWhitePointX, float fWhitePointY);

    // luminance of white
    void SetWhiteLuminance (float fWhiteLuminance);

    // horizontal output density; vertical_density = horizontal_density*pixel_aspect_ratio
    void SetHorizontalDensity (float fDensity);  // must be > 0.0

    // name of the owner
    void SetOwner (const String& rqOwner);

    // image information
    void SetComments (const String& rqComments);

    // the date when the image was captured or created
    void SetCaptureDate (const Date& rqDate);

    // offset of local time from UTC, in seconds; UTC = local_time + UTC_offset
    void SetUtcOffset (float fUtcOffset);

    // the location where the image was recorded; longitude and latitude in degrees, altitude in meters
    void SetLongitude (float fLongitude);
    void SetLatitude (float fLatitude);
    void SetAltitude (float fAltitude);

    // the camera's focus distance, in meters
    void SetFocus (float fFocus);

    // exposure time
    void SetExposureTime (float fExposureTime);

    // the camera's lens aperture, in f-stops (focal length of the lens divided by the diameter of the iris
    // opening)
    void SetAperture (float fAperture);

    // ISO speed of the film or image sensor that was used to record the image
    void SetIsoSpeed (float fIsoSpeed);

    // type of the environment map
    void SetEnvironmentMapType (EnvironmentMapType eType);

    // key-code for motion picture film frames
    void SetKeyCode (const KeyCode& rqKeyCode);

    // time-code
    void SetTimeCode (const TimeCode& rqTimeCode);

    // how texture map images are extrapolated
    void SetWrapmodes (const String& rqWrapmodes);

    // how to store color components in the EXR file, as alpha-premultiplied or as not
    void SetAlphaAssociationHandling (bool bMultiplyAlpha);

    // Setting of the off-screen mode causes a provided exposure and gamma to be applied in the inverse way to
    // EXR pixels (the on-screen mode for loading is the direct one).  The function is useful when the user
    // knows that this image is actually a low-dynamic-range but originates from a high-dynamic-range and also
    // knows the values of exposure and gamma which were applied in that transformation.  So, the aim of this
    // function is to try to restore the original HDR image using the information preserved in LDR image.
    void SetScreenModeOff (float fExposure, float fGamma);

    // Saving.  Progressed.
    bool SaveImage (const char* acFilename);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    // Construction.  For loading.  Initially tries to read the heading data.
    FormatExr (const char* acFilename, bool& rbSuccess);

    // File attributes.

    // predefined attributes
    int GetVersion () const;
    int GetImageWidth () const;    // data window's width
    int GetImageHeight () const;   // data window's height
    const Bound2i& GetImageDisplayWindow () const;
    const Bound2i& GetImageDataWindow () const;
    int GetBitsPerPixel () const;
    ColorModelType GetColorModel () const;
    CompressionType GetCompression () const;
    float GetPixelAspectRatio () const;
    void GetScreenWindowCenter (float& rfX, float& rfY) const;
    float GetScreenWindowWidth () const;
    int GetChannelQuantity () const;
    const ChannelRecord& GetChannelRecord (int i) const;
    LineOrderType GetLineOrder () const;

    // tile description; attributes peculiar only for tiled images
    bool IsTiled () const;
    int GetTileWidth () const;
    int GetTileHeight () const;
    LevelModeType GetLevelMode () const;
    LevelRoundingModeType GetLevelRoundingMode () const;
    int GetMipmapLevelQuantity () const;
    void GetRipmapLevelQuantities (int& riQuantityX, int& riQuantityY) const;

    // preview image; pixel array is non-padded continuous sequence in the standard manner (left-to-right,
    // top-to-bottom)
    bool HasPreviewImage () const;
    int GetPreviewImageWidth () const;
    int GetPreviewImageHeight () const;
    const StillArray<UcRgba>& GetPreviewImagePixels () const;

    // primary chromaticities and white point
    bool HasChromaticities () const;
    float GetChromaticityRedX () const;
    float GetChromaticityRedY () const;
    float GetChromaticityGreenX () const;
    float GetChromaticityGreenY () const;
    float GetChromaticityBlueX () const;
    float GetChromaticityBlueY () const;
    float GetWhitePointX () const;
    float GetWhitePointY () const;

    // luminance of white
    bool HasWhiteLuminance () const;
    float GetWhiteLuminance () const;

    // horizontal output density; vertical_density = horizontal_density*pixel_aspect_ratio
    bool HasHorizontalDensity () const;
    float GetHorizontalDensity () const;

    // name of the owner
    bool HasOwner () const;
    const String& GetOwner () const;

    // image information
    bool HasComments () const;
    const String& GetComments () const;

    // the date when the image was captured or created
    bool HasCaptureDate () const;
    const Date& GetCaptureDate () const;

    // offset of local time from UTC, in seconds; UTC = local_time + UTC_offset
    bool HasUtcOffset () const;
    float GetUtcOffset () const;

    // the location where the image was recorded; longitude and latitude in degrees, altitude in meters
    bool HasLongitude () const;
    float GetLongitude () const;
    bool HasLatitude () const;
    float GetLatitude () const;
    bool HasAltitude () const;
    float GetAltitude () const;

    // the camera's focus distance, in meters
    bool HasFocus () const;
    float GetFocus () const;

    // exposure time
    bool HasExposureTime () const;
    float GetExposureTime () const;

    // the camera's lens aperture, in f-stops (focal length of the lens divided by the diameter of the iris
    // opening)
    bool HasAperture () const;
    float GetAperture () const;

    // ISO speed of the film or image sensor that was used to record the image
    bool HasIsoSpeed () const;
    float GetIsoSpeed () const;

    // type of the environment map
    bool IsEnvironmentMap () const;
    EnvironmentMapType GetEnvironmentMapType () const;

    // key-code for motion picture film frames
    bool HasKeyCode () const;
    const KeyCode& GetKeyCode () const;

    // time-code
    bool HasTimeCode () const;
    const TimeCode& GetTimeCode () const;

    // how texture map images are extrapolated
    bool HasWrapmodes () const;
    const String& GetWrapmodes () const;

    // how to treat color components while loading, as alpha-premultiplied or as untouched by alpha
    void SetAlphaUnassociationHandling (bool bUnmultiplyAlpha);

    // Setting of the on-screen mode implies application of exposure, gamma, and clamping to pixels of the
    // image during its loading.  Thus, only a sub-range of color component values is preserved, which is
    // suitable for preview purposes and on-display editing.  The typical values for exposure and gamma are
    // respectively 0.0 and 2.2.
    void SetScreenModeOn (float fExposure, float fGamma);

    // Loading.  Progressed.
    bool LoadImage (RasterImagePtr& rspqImage);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

private:
    void OnWritePixel (ColorRgba& rqPixel, bool bHasAlpha) const;
    void AllocOutImage (RasterImagePtr& rspqImage, int iWidth, int iHeight) const;
    void OnReadPixel (float* afPix, int iSQ, bool bHasAlpha) const;

    RasterImagePtr m_spqImageSaveTo;
    int m_iVersion;
    int m_iImageWidth, m_iImageHeight;
    Bound2i m_qImageDisplayWindow;                          // (default: from the image's dimensions)
    Bound2i m_qImageDataWindow;                             // (default: from the image's dimensions)
    float m_fPixelAspectRatio;                              // (default: 1.0)
    float m_fScreenWindowCenterX;                           // (default: 0.0)
    float m_fScreenWindowCenterY;                           // (default: 0.0)
    float m_fScreenWindowWidth;                             // (default: 1.0)
    int m_iChannelQuantity;
    ModestArray<ChannelRecord> m_qChannelRecords;
    bool m_bColorModelPrecised;
    ColorModelType m_eColorModel;                           // (default: autodetect)
    LineOrderType m_eLineOrder;                             // (default: LOT_INCREASING_Y)
    CompressionType m_eCompression;                         // (default: CT_PIZ)
    bool m_bIsTiled;                                        // (default: false)
    int m_iTileWidth, m_iTileHeight;
    LevelModeType m_eLevelMode;
    LevelRoundingModeType m_eLevelRoundingMode;
    int m_iMipmapLevelQuantity;
    int m_iRipmapLevelQuantityX, m_iRipmapLevelQuantityY;
    bool m_bHasPreviewImage;                                // (default: false)
    int m_iPreviewImageWidth, m_iPreviewImageHeight;
    StillArray<UcRgba> m_qPreviewImagePixels;
    bool m_bHasChromaticities;                              // (default: false)
    float m_fChromaticityRedX, m_fChromaticityRedY;
    float m_fChromaticityGreenX, m_fChromaticityGreenY;
    float m_fChromaticityBlueX, m_fChromaticityBlueY;
    float m_fWhitePointX, m_fWhitePointY;
    bool m_bHasWhiteLuminance;                              // (default: false)
    float m_fWhiteLuminance;
    bool m_bHasHorizontalDensity;                           // (default: false)
    float m_fHorizontalDensity;
    bool m_bHasOwner;                                       // (default: false)
    String m_qOwner;
    bool m_bHasComments;                                    // (default: false)
    String m_qComments;
    bool m_bHasCaptureDate;                                 // (default: false)
    Date m_qCaptureDate;
    bool m_bHasUtcOffset;                                   // (default: false)
    float m_fUtcOffset;
    bool m_bHasLongitude;                                   // (default: false)
    float m_fLongitude;
    bool m_bHasLatitude;                                    // (default: false)
    float m_fLatitude;
    bool m_bHasAltitude;                                    // (default: false)
    float m_fAltitude;
    bool m_bHasFocus;                                       // (default: false)
    float m_fFocus;
    bool m_bHasExposureTime;                                // (default: false)
    float m_fExposureTime;
    bool m_bHasAperture;                                    // (default: false)
    float m_fAperture;
    bool m_bHasIsoSpeed;                                    // (default: false)
    float m_fIsoSpeed;
    bool m_bIsEnvironmentMap;                               // (default: false)
    EnvironmentMapType m_eEnvironmentMapType;
    bool m_bHasKeyCode;                                     // (default: false)
    KeyCode m_qKeyCode;
    bool m_bHasTimeCode;                                    // (default: false)
    TimeCode m_qTimeCode;
    bool m_bHasWrapmodes;                                   // (default: false)
    String m_qWrapmodes;
    bool m_bMultiplyAlpha;                                  // (default: true)
    bool m_bOffScreenMode;                                  // (default: false)
    float m_fOffScreenExposure, m_fOffScreenExposureCoeff;
    float m_fOffScreenGamma;
    bool m_bUnmultiplyAlpha;                                // (default: true)
    bool m_bOnScreenMode;                                   // (default: false)
    float m_fOnScreenExposure, m_fOnScreenExposureCoeff;
    float m_fOnScreenGamma, m_fOnScreenInvGamma;
};

#include "QFormatExr.inl"

}

#endif



