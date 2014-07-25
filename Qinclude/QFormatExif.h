#ifndef QFORMATEXIF_H
#define QFORMATEXIF_H

// Exchangeable Image Format for Digital Still Cameras.  The implementation embodies Exif version 2.2 with
// Flashpix version 1.0.  No concessions were made in clearing up data's validity, and any discrepancy will
// cause an assert-false.  However, if it's encountered Exif (or Flashpix) version which seems to be a newer
// one, no assert-false takes place.  Accordingly to the specification, Exif metadata divides into two major
// sections:
//   Attribute Information Used in Exif;
//   Exif IFD Attribute Information.
// "Attribute Information Used in Exif" includes the next tag groups:
//   tags relating to image data structure;
//   tags relating to recording offset;
//   tags relating to image data characteristics;
//   other tags.
// "Exif IFD Attribute Information" is available only when ExifIFDPointer tag is present; this section embraces
// the next tag groups:
//   tags relating to version;
//   tags relating to image data characteristics;
//   tags relating to image configuration;
//   tags relating to user information;
//   tags relating to related file information;
//   tags relating to date and time;
//   tags relating to picture-taking conditions;
//   other tags.

#include "QFormatBase.h"
#include "QDate.h"

namespace Q
{

class Q_ITEM FormatExif : public FormatBase
{
public:
    enum ByteOrderType
    {
        BOT_LITTLE_ENDIAN,
        BOT_BIG_ENDIAN
    };

    // relates only to saving
    enum ExifDataDestinationType
    {
        EDDT_STANDALONE_OR_JPEG,
        EDDT_TIFF
    };

    enum CompressionSchemeType
    {
        CST_UNCOMPRESSED,
        CST_JPEG_COMPRESSION
    };

    enum PhotometricInterpretationType
    {
        PIT_RGB,
        PIT_YCC  // YCbCr
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

    enum PlanarConfigurationType
    {
        PCT_CHUNKY_FORMAT,
        PCT_PLANAR_FORMAT
    };

    enum YCbCrSubSamplingType
    {
        YCCSST_4_2_2,
        YCCSST_4_2_0
    };

    enum YCbCrPositioningType
    {
        YCCPT_CENTERED,
        YCCPT_CO_SITED
    };

    enum ResolutionUnitType
    {
        RUT_INCHES,
        RUT_CENTIMETERS
    };

    enum ExifVersionType
    {
        EXIFVT_2_0,
        EXIFVT_2_1,
        EXIFVT_2_2,
        EXIFVT_OTHER  // not for saving
    };

    enum FlashpixVersionType
    {
        FVT_1_0,
        FVT_OTHER  // not for saving
    };

    enum ColorSpaceType
    {
        CSTT_sRGB,
        CSTT_UNCALIBRATED
    };

    enum ComponentsConfigurationType
    {
        CCT_RGB,
        CCT_YCC  // YCbCr
    };

    enum UserCommentCharacterCodeType
    {
        UCCCT_ASCII,
        UCCCT_JIS,
        UCCCT_UNICODE,
        UCCCT_UNDEFINED
    };

    enum ExposureProgramType
    {
        EPT_NOT_DEFINED,
        EPT_MANUAL,
        EPT_NORMAL_PROGRAM,
        EPT_APERTURE_PRIORITY,
        EPT_SHUTTER_PRIORITY,
        EPT_CREATIVE_PROGRAM,
        EPT_ACTION_PROGRAM,
        EPT_PORTRAIT_MODE,
        EPT_LANDSCAPE_MODE
    };

    enum BrightnessValueStateType
    {
        BVST_KNOWN,
        BVST_UNKNOWN
    };

    enum SubjectDistanceStateType
    {
        SDST_KNOWN,
        SDST_INFINITY,
        SDST_UNKNOWN
    };

    enum MeteringModeType
    {
        MMT_UNKNOWN,
        MMT_AVERAGE,
        MMT_CENTER_WEIGHTED_AVERAGE,
        MMT_SPOT,
        MMT_MULTI_SPOT,
        MMT_PATTERN,
        MMT_PARTIAL,
        MMT_OTHER
    };

    enum LightSourceType
    {
        LST_UNKNOWN,
        LST_DAYLIGHT,
        LST_FLUORESCENT,
        LST_TUNGSTEN,
        LST_FLASH,
        LST_FINE_WEATHER,
        LST_CLOUDY_WEATHER,
        LST_SHADE,
        LST_DAYLIGHT_FLUORESCENT,
        LST_DAY_WHITE_FLUORESCENT,
        LST_COOL_WHITE_FLUORESCENT,
        LST_WHITE_FLUORESCENT,
        LST_STANDARD_LIGHT_A,
        LST_STANDARD_LIGHT_B,
        LST_STANDARD_LIGHT_C,
        LST_D55,
        LST_D65,
        LST_D75,
        LST_D50,
        LST_ISO_STUDIO_TUNGSTEN,
        LST_OTHER
    };

    enum FlashType
    {
        FT_FLASH_DID_NOT_FIRE,
        FT_FLASH_FIRED,
        FT_STROBE_RETURN_LIGHT_NOT_DETECTED,
        FT_STROBE_RETURN_LIGHT_DETECTED,
        FT_FLASH_FIRED__COMPULSORY_FLASH_MODE,
        FT_FLASH_FIRED__COMPULSORY_FLASH_MODE__RETURN_LIGHT_NOT_DETECTED,
        FT_FLASH_FIRED__COMPULSORY_FLASH_MODE__RETURN_LIGHT_DETECTED,
        FT_FLASH_DID_NOT_FIRE__COMPULSORY_FLASH_MODE,
        FT_FLASH_DID_NOT_FIRE__AUTO_MODE,
        FT_FLASH_FIRED__AUTO_MODE,
        FT_FLASH_FIRED__AUTO_MODE__RETURN_LIGHT_NOT_DETECTED,
        FT_FLASH_FIRED__AUTO_MODE__RETURN_LIGHT_DETECTED,
        FT_NO_FLASH_FUNCTION,
        FT_FLASH_FIRED__REDEYE_REDUCTION_MODE,
        FT_FLASH_FIRED__REDEYE_REDUCTION_MODE__RETURN_LIGHT_NOT_DETECTED,
        FT_FLASH_FIRED__REDEYE_REDUCTION_MODE__RETURN_LIGHT_DETECTED,
        FT_FLASH_FIRED__COMPULSORY_FLASH_MODE__REDEYE_REDUCTION_MODE,
        FT_FLASH_FIRED__COMPULSORY_FLASH_MODE__REDEYE_REDUCTION_MODE__RETURN_LIGHT_NOT_DETECTED,
        FT_FLASH_FIRED__COMPULSORY_FLASH_MODE__REDEYE_REDUCTION_MODE__RETURN_LIGHT_DETECTED,
        FT_FLASH_FIRED__AUTO_MODE__REDEYE_REDUCTION_MODE,
        FT_FLASH_FIRED__AUTO_MODE__RETURN_LIGHT_NOT_DETECTED__REDEYE_REDUCTION_MODE,
        FT_FLASH_FIRED__AUTO_MODE__RETURN_LIGHT_DETECTED__REDEYE_REDUCTION_MODE
    };

    enum SensingMethodType
    {
        SMT_NOT_DEFINED,
        SMT_ONECHIP_COLOR_AREA_SENSOR,
        SMT_TWOCHIP_COLOR_AREA_SENSOR,
        SMT_THREECHIP_COLOR_AREA_SENSOR,
        SMT_COLOR_SEQUENTIAL_AREA_SENSOR,
        SMT_TRILINEAR_SENSOR,
        SMT_COLOR_SEQUENTIAL_LINEAR_SENSOR
    };

    // the format defines that single item
    enum FileSourceType
    {
        FST_DSC
    };

    // the format defines that single item
    enum SceneType
    {
        ST_A_DIRECTLY_PHOTOGRAPHED_IMAGE
    };

    enum CustomRenderedType
    {
        CRT_NORMAL_PROCESS,
        CRT_CUSTOM_PROCESS
    };

    enum ExposureModeType
    {
        EMT_AUTO_EXPOSURE,
        EMT_MANUAL_EXPOSURE,
        EMT_AUTO_BRACKET
    };

    enum WhiteBalanceType
    {
        WBT_AUTO_WHITE_BALANCE,
        WBT_MANUAL_WHITE_BALANCE
    };

    enum DigitalZoomStateType
    {
        DZST_USED,
        DZST_NOT_USED
    };

    enum FocalLengthIn35mmFilmStateType
    {
        FLI35MMFST_KNOWN,
        FLI35MMFST_UNKNOWN
    };

    enum SceneCaptureType
    {
        SCT_STANDARD,
        SCT_LANDSCAPE,
        SCT_PORTRAIT,
        SCT_NIGHT_SCENE
    };

    enum GainControlType
    {
        GCT_NONE,
        GCT_LOW_GAIN_UP,
        GCT_HIGH_GAIN_UP,
        GCT_LOW_GAIN_DOWN,
        GCT_HIGH_GAIN_DOWN
    };

    enum ContrastType
    {
        CT_NORMAL,
        CT_SOFT,
        CT_HARD
    };

    enum SaturationType
    {
        STT_NORMAL,
        STT_LOW_SATURATION,
        STT_HIGH_SATURATION
    };

    enum SharpnessType
    {
        STTT_NORMAL,
        STTT_SOFT,
        STTT_HARD
    };

    enum SubjectDistanceRangeType
    {
        SDRT_UNKNOWN,
        SDRT_MACRO,
        SDRT_CLOSE_VIEW,
        SDRT_DISTANT_VIEW
    };

    // state-enumerands are more suitable as against booleans
    enum TagPresenceType
    {
        TPT_ABSENT,
        TPT_PRESENT,
        TPT_INCORRECT  // means "is present but has a content not allowed by the specification"
    };

    enum ExceptionType
    {
        ET_FORMAT_MISMATCH
    };

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    // Construction.  For saving.  NOTE. When constructing an Exif object for further placing into a TIFF file,
    // the next requirements must be met:
    //   byte order must be the same as for that TIFF file;
    //   no tags from the first section ("Attribute Information Used in Exif") must be present since they may
    //     contradict native TIFF fields (however, when loading, native TIFF fields naturally become part of Exif).
    FormatExif (ByteOrderType eByteOrder = BOT_LITTLE_ENDIAN);  // all tags are initially absent
    FormatExif (const FormatExif& rqSourceExif,                 // with tag copying from another Exif object
        ByteOrderType eByteOrder = BOT_LITTLE_ENDIAN);

    // SECTION: Attribute Information Used in Exif.

    // tags relating to image data structure
    void SetImageWidth (int iImageWidth);
    void SetImageLength (int iImageLength);  // that's the height
    void SetBitsPerSample (int iBitsPerSample0, int iBitsPerSample1, int iBitsPerSample2);
    void SetCompression (CompressionSchemeType eCompression);
    void SetPhotometricInterpretation (PhotometricInterpretationType ePhotometricInterpretation);
    void SetOrientation (OrientationType eOrientation);
    void SetSamplesPerPixel (int iSamplesPerPixel);
    void SetPlanarConfiguration (PlanarConfigurationType ePlanarConfiguration);
    void SetYCbCrSubSampling (YCbCrSubSamplingType eYCbCrSubSampling);
    void SetYCbCrPositioning (YCbCrPositioningType eYCbCrPositioning);
    void SetXResolution (float fXResolution);
    void SetYResolution (float fYResolution);
    void SetResolutionUnit (ResolutionUnitType eResolutionUnit);

    // tags relating to recording offset
    void SetStripOffsets (const StillArray<int>& rqStripOffsets);
    void SetRowsPerStrip (int iRowsPerStrip);
    void SetStripByteCounts (const StillArray<int>& rqStripByteCounts);
    void SetJPEGInterchangeFormat (int iJPEGInterchangeFormat);
    void SetJPEGInterchangeFormatLength (int iJPEGInterchangeFormatLength);

    // tags relating to image data characteristics
    void SetTransferFunction (const StillArray<int>& rqTransferFunction);  // array length must be 768
    void SetWhitePoint (float fWhitePointX, float fWhitePointY);
    void SetPrimaryChromaticities (
        float fPrimaryChromaticityRedX, float fPrimaryChromaticityRedY,
        float fPrimaryChromaticityGreenX, float fPrimaryChromaticityGreenY,
        float fPrimaryChromaticityBlueX, float fPrimaryChromaticityBlueY);
    void SetYCbCrCoefficients (float fCoefficientY, float fCoefficientCb, float fCoefficientCr);
    void SetReferenceBlackWhite (
        const Link<float,float>& rqReferenceBlackWhiteComponent0,
        const Link<float,float>& rqReferenceBlackWhiteComponent1,
        const Link<float,float>& rqReferenceBlackWhiteComponent2);

    // other tags (in Attribute Information)
    void SetDateTime (const Date& rqDateTime);
    void SetImageDescription (const String& rqImageDescription);
    void SetMake (const String& rqMake);
    void SetModel (const String& rqModel);
    void SetSoftware (const String& rqSoftware);
    void SetArtist (const String& rqArtist);
    void SetCopyright (const String& rqPhotographerCopyright, const String& rqEditorCopyright);  // empty if none

    // SECTION: Exif IFD Attribute Information.

    // tags relating to version
    void SetExifVersion (ExifVersionType eExifVersion);
    void SetFlashpixVersion (FlashpixVersionType eFlashpixVersion);

    // tags relating to image data characteristics
    void SetColorSpace (ColorSpaceType eColorSpace);

    // tags relating to image configuration
    void SetPixelXDimension (int iPixelXDimension);
    void SetPixelYDimension (int iPixelYDimension);
    void SetComponentsConfiguration (ComponentsConfigurationType eComponentsConfiguration);
    void SetCompressedBitsPerPixel (float fCompressedBitsPerPixel);

    // tags relating to user information
    void SetMakerNote (const StillArray<char>& rqMakerNote);
    void SetUserComment (  // since it is rather data than a string, null-termination is redundant
        UserCommentCharacterCodeType eUserCommentCharacterCode, const StillArray<char>& rqUserComment);

    // tags relating to related file information
    void SetRelatedSoundFile (const String& rqRelatedSoundFile);  // filename must be 12 characters in length

    // tags relating to date and time
    void SetDateTimeOriginal (const Date& rqDateTimeOriginal);
    void SetDateTimeDigitized (const Date& rqDateTimeDigitized);
    void SetSubsecTime (const String& rqSubsecTime);
    void SetSubsecTimeOriginal (const String& rqSubsecTimeOriginal);
    void SetSubsecTimeDigitized (const String& rqSubsecTimeDigitized);

    // tags relating to picture-taking conditions
    void SetExposureTime (float fExposureTime);
    void SetFNumber (float fFNumber);
    void SetExposureProgram (ExposureProgramType eExposureProgram);
    void SetSpectralSensitivity (const String& rqSpectralSensitivity);
    void SetISOSpeedRatings (const StillArray<int>& rqISOSpeedRatings);
    void SetOECF (const StillArray<char>& rqOECF);
    void SetShutterSpeedValue (float fShutterSpeedValue);
    void SetApertureValue (float fApertureValue);
    void SetBrightnessValue (BrightnessValueStateType eBrightnessValueState,  // actual value needed only if known
        float fBrightnessValue);
    void SetExposureBiasValue (float fExposureBiasValue);
    void SetMaxApertureValue (float fMaxApertureValue);
    void SetSubjectDistance (SubjectDistanceStateType eSubjectDistanceState,  // actual value needed only if known
        float fSubjectDistance);
    void SetMeteringMode (MeteringModeType eMeteringMode);
    void SetLightSource (LightSourceType eLightSource);
    void SetFlash (FlashType eFlash);
    void SetSubjectArea (const StillArray<int>& rqSubjectAreaCoordinates);  // 2, 3, or 4 coordinates
    void SetFocalLength (float fFocalLength);
    void SetFlashEnergy (float fFlashEnergy);
    void SetSpatialFrequencyResponse (const StillArray<char>& rqSpatialFrequencyResponse);
    void SetFocalPlaneXResolution (float fFocalPlaneXResolution);
    void SetFocalPlaneYResolution (float fFocalPlaneYResolution);
    void SetFocalPlaneResolutionUnit (ResolutionUnitType eFocalPlaneResolutionUnit);
    void SetSubjectLocation (int iSubjectLocationX, int iSubjectLocationY);
    void SetExposureIndex (float fExposureIndex);
    void SetSensingMethod (SensingMethodType eSensingMethod);
    void SetFileSource (FileSourceType eFileSource);
    void SetSceneType (SceneType eSceneType);
    void SetCFAPattern (const StillArray<char>& rqCFAPattern);
    void SetCustomRendered (CustomRenderedType eCustomRendered);
    void SetExposureMode (ExposureModeType eExposureMode);
    void SetWhiteBalance (WhiteBalanceType eWhiteBalance);
    void SetDigitalZoomRatio (DigitalZoomStateType eDigitalZoomState,  // actual value needed only if used
        float fDigitalZoomRatio);
    void SetFocalLengthIn35mmFilm (  // actual value needed only if known
        FocalLengthIn35mmFilmStateType eFocalLengthIn35mmFilmState, int iFocalLengthIn35mmFilm);
    void SetSceneCaptureType (SceneCaptureType eSceneCaptureType);
    void SetGainControl (GainControlType eGainControl);
    void SetContrast (ContrastType eContrast);
    void SetSaturation (SaturationType eSaturation);
    void SetSharpness (SharpnessType eSharpness);
    void SetDeviceSettingDescription (const StillArray<char>& rqDeviceSettingDescription);
    void SetSubjectDistanceRange (SubjectDistanceRangeType eSubjectDistanceRange);

    // other tags (in Exif IFD Attribute Information)
    void SetImageUniqueId (const String& rqImageUniqueId);  // string must be 32 characters in length

    // tag removal
    void RemoveAllFirstSectionTags ();  // may be useful when further placing into a TIFF file is implied
    void RemoveImageWidthTag ();
    void RemoveImageLengthTag ();
    void RemoveBitsPerSampleTag ();
    void RemoveCompressionTag ();
    void RemovePhotometricInterpretationTag ();
    void RemoveOrientationTag ();
    void RemoveSamplesPerPixelTag ();
    void RemovePlanarConfigurationTag ();
    void RemoveYCbCrSubSamplingTag ();
    void RemoveYCbCrPositioningTag ();
    void RemoveXResolutionTag ();
    void RemoveYResolutionTag ();
    void RemoveResolutionUnitTag ();
    void RemoveStripOffsetsTag ();
    void RemoveRowsPerStripTag ();
    void RemoveStripByteCountsTag ();
    void RemoveJPEGInterchangeFormatTag ();
    void RemoveJPEGInterchangeFormatLengthTag ();
    void RemoveTransferFunctionTag ();
    void RemoveWhitePointTag ();
    void RemovePrimaryChromaticitiesTag ();
    void RemoveYCbCrCoefficientsTag ();
    void RemoveReferenceBlackWhiteTag ();
    void RemoveDateTimeTag ();
    void RemoveImageDescriptionTag ();
    void RemoveMakeTag ();
    void RemoveModelTag ();
    void RemoveSoftwareTag ();
    void RemoveArtistTag ();
    void RemoveCopyrightTag ();
    void RemoveExifVersionTag ();
    void RemoveFlashpixVersionTag ();
    void RemoveColorSpaceTag ();
    void RemovePixelXDimensionTag ();
    void RemovePixelYDimensionTag ();
    void RemoveComponentsConfigurationTag ();
    void RemoveCompressedBitsPerPixelTag ();
    void RemoveMakerNoteTag ();
    void RemoveUserCommentTag ();
    void RemoveRelatedSoundFileTag ();
    void RemoveDateTimeOriginalTag ();
    void RemoveDateTimeDigitizedTag ();
    void RemoveSubsecTimeTag ();
    void RemoveSubsecTimeOriginalTag ();
    void RemoveSubsecTimeDigitizedTag ();
    void RemoveExposureTimeTag ();
    void RemoveFNumberTag ();
    void RemoveExposureProgramTag ();
    void RemoveSpectralSensitivityTag ();
    void RemoveISOSpeedRatingsTag ();
    void RemoveOECFTag ();
    void RemoveShutterSpeedValueTag ();
    void RemoveApertureValueTag ();
    void RemoveBrightnessValueTag ();
    void RemoveExposureBiasValueTag ();
    void RemoveMaxApertureValueTag ();
    void RemoveSubjectDistanceTag ();
    void RemoveMeteringModeTag ();
    void RemoveLightSourceTag ();
    void RemoveFlashTag ();
    void RemoveSubjectAreaTag ();
    void RemoveFocalLengthTag ();
    void RemoveFlashEnergyTag ();
    void RemoveSpatialFrequencyResponseTag ();
    void RemoveFocalPlaneXResolutionTag ();
    void RemoveFocalPlaneYResolutionTag ();
    void RemoveFocalPlaneResolutionUnitTag ();
    void RemoveSubjectLocationTag ();
    void RemoveExposureIndexTag ();
    void RemoveSensingMethodTag ();
    void RemoveFileSourceTag ();
    void RemoveSceneTypeTag ();
    void RemoveCFAPatternTag ();
    void RemoveCustomRenderedTag ();
    void RemoveExposureModeTag ();
    void RemoveWhiteBalanceTag ();
    void RemoveDigitalZoomRatioTag ();
    void RemoveFocalLengthIn35mmFilmTag ();
    void RemoveSceneCaptureTypeTag ();
    void RemoveGainControlTag ();
    void RemoveContrastTag ();
    void RemoveSaturationTag ();
    void RemoveSharpnessTag ();
    void RemoveDeviceSettingDescriptionTag ();
    void RemoveSubjectDistanceRangeTag ();
    void RemoveImageUniqueIdTag ();

    // Saving.  If Exif data destination is EDDT_TIFF, parameter iTiffPreOffset must be non-negative.
    void SaveData (StillArray<char>& rqStaticBytestream, ExifDataDestinationType eExifDataDestination,
        int iTiffPreOffset = -1);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    // Construction.  For loading.
    FormatExif (bool bBytestreamIsStatic, const char* acBytestream, int iBytestreamSize);

    // loading
    bool LoadData (int iDynamicBytestreamSize = -1);

    // endianness
    ByteOrderType GetByteOrder () const;

    // Functions which relate to the same tag are joined together.

    // SECTION: Attribute Information Used in Exif.

    // Tags relating to image data structure.

    TagPresenceType GetImageWidthTagPresence () const;
    int GetImageWidth () const;

    TagPresenceType GetImageLengthTagPresence () const;
    int GetImageLength () const;  // that's the height

    TagPresenceType GetBitsPerSampleTagPresence () const;
    int GetBitsPerSample0 () const;
    int GetBitsPerSample1 () const;
    int GetBitsPerSample2 () const;

    TagPresenceType GetCompressionTagPresence () const;
    CompressionSchemeType GetCompression () const;

    TagPresenceType GetPhotometricInterpretationTagPresence () const;
    PhotometricInterpretationType GetPhotometricInterpretation () const;

    TagPresenceType GetOrientationTagPresence () const;
    OrientationType GetOrientation () const;

    TagPresenceType GetSamplesPerPixelTagPresence () const;
    int GetSamplesPerPixel () const;

    TagPresenceType GetPlanarConfigurationTagPresence () const;
    PlanarConfigurationType GetPlanarConfiguration () const;

    TagPresenceType GetYCbCrSubSamplingTagPresence () const;
    YCbCrSubSamplingType GetYCbCrSubSampling () const;

    TagPresenceType GetYCbCrPositioningTagPresence () const;
    YCbCrPositioningType GetYCbCrPositioning () const;

    TagPresenceType GetXResolutionTagPresence () const;
    float GetXResolution () const;

    TagPresenceType GetYResolutionTagPresence () const;
    float GetYResolution () const;

    TagPresenceType GetResolutionUnitTagPresence () const;
    ResolutionUnitType GetResolutionUnit () const;

    // Tags relating to recording offset.

    TagPresenceType GetStripOffsetsTagPresence () const;
    const StillArray<int>& GetStripOffsets () const;

    TagPresenceType GetRowsPerStripTagPresence () const;
    int GetRowsPerStrip () const;

    TagPresenceType GetStripByteCountsTagPresence () const;
    const StillArray<int>& GetStripByteCounts () const;

    TagPresenceType GetJPEGInterchangeFormatTagPresence () const;
    int GetJPEGInterchangeFormat () const;

    TagPresenceType GetJPEGInterchangeFormatLengthTagPresence () const;
    int GetJPEGInterchangeFormatLength () const;

    // Tags relating to image data characteristics.

    TagPresenceType GetTransferFunctionTagPresence () const;
    const StillArray<int>& GetTransferFunction () const;

    TagPresenceType GetWhitePointTagPresence () const;
    float GetWhitePointX () const;
    float GetWhitePointY () const;

    TagPresenceType GetPrimaryChromaticitiesTagPresence () const;
    float GetPrimaryChromaticityRedX () const;
    float GetPrimaryChromaticityRedY () const;
    float GetPrimaryChromaticityGreenX () const;
    float GetPrimaryChromaticityGreenY () const;
    float GetPrimaryChromaticityBlueX () const;
    float GetPrimaryChromaticityBlueY () const;

    TagPresenceType GetYCbCrCoefficientsTagPresence () const;
    float GetCoefficientY () const;
    float GetCoefficientCb () const;
    float GetCoefficientCr () const;

    TagPresenceType GetReferenceBlackWhiteTagPresence () const;
    const Link<float,float>& GetReferenceBlackWhiteComponent0 () const;
    const Link<float,float>& GetReferenceBlackWhiteComponent1 () const;
    const Link<float,float>& GetReferenceBlackWhiteComponent2 () const;

    // Other tags (in Attribute Information).

    TagPresenceType GetDateTimeTagPresence () const;
    const Date& GetDateTime () const;

    TagPresenceType GetImageDescriptionTagPresence () const;
    const String& GetImageDescription () const;

    TagPresenceType GetMakeTagPresence () const;
    const String& GetMake () const;

    TagPresenceType GetModelTagPresence () const;
    const String& GetModel () const;

    TagPresenceType GetSoftwareTagPresence () const;
    const String& GetSoftware () const;

    TagPresenceType GetArtistTagPresence () const;
    const String& GetArtist () const;

    TagPresenceType GetCopyrightTagPresence () const;
    bool HasPhotographerCopyright () const;
    bool HasEditorCopyright () const;
    const String& GetPhotographerCopyright () const;
    const String& GetEditorCopyright () const;

    // SECTION: Exif IFD Attribute Information.

    // should be called to get know if Exif IFD Attribute Information is present upon the whole
    TagPresenceType GetExifIFDPointerTagPresence () const;

    // Tags relating to version.

    TagPresenceType GetExifVersionTagPresence () const;
    ExifVersionType GetExifVersion () const;

    TagPresenceType GetFlashpixVersionTagPresence () const;
    FlashpixVersionType GetFlashpixVersion () const;

    // Tags relating to image data characteristics.

    TagPresenceType GetColorSpaceTagPresence () const;
    ColorSpaceType GetColorSpace () const;

    // Tags relating to image configuration.

    TagPresenceType GetPixelXDimensionTagPresence () const;
    int GetPixelXDimension () const;

    TagPresenceType GetPixelYDimensionTagPresence () const;
    int GetPixelYDimension () const;

    TagPresenceType GetComponentsConfigurationTagPresence () const;
    ComponentsConfigurationType GetComponentsConfiguration () const;

    TagPresenceType GetCompressedBitsPerPixelTagPresence () const;
    float GetCompressedBitsPerPixel () const;

    // Tags relating to user information.

    TagPresenceType GetMakerNoteTagPresence () const;
    const StillArray<char>& GetMakerNote () const;

    TagPresenceType GetUserCommentTagPresence () const;
    UserCommentCharacterCodeType GetUserCommentCharacterCode () const;
    const StillArray<char>& GetUserComment () const;

    // Tags relating to related file information.

    TagPresenceType GetRelatedSoundFileTagPresence () const;
    const String& GetRelatedSoundFile () const;

    // Tags relating to date and time.

    TagPresenceType GetDateTimeOriginalTagPresence () const;
    const Date& GetDateTimeOriginal () const;

    TagPresenceType GetDateTimeDigitizedTagPresence () const;
    const Date& GetDateTimeDigitized () const;

    TagPresenceType GetSubsecTimeTagPresence () const;
    const String& GetSubsecTime () const;

    TagPresenceType GetSubsecTimeOriginalTagPresence () const;
    const String& GetSubsecTimeOriginal () const;

    TagPresenceType GetSubsecTimeDigitizedTagPresence () const;
    const String& GetSubsecTimeDigitized () const;

    // Tags relating to picture-taking conditions.

    TagPresenceType GetExposureTimeTagPresence () const;
    float GetExposureTime () const;

    TagPresenceType GetFNumberTagPresence () const;
    float GetFNumber () const;

    TagPresenceType GetExposureProgramTagPresence () const;
    ExposureProgramType GetExposureProgram () const;

    TagPresenceType GetSpectralSensitivityTagPresence () const;
    const String& GetSpectralSensitivity () const;

    TagPresenceType GetISOSpeedRatingsTagPresence () const;
    const StillArray<int>& GetISOSpeedRatings () const;

    TagPresenceType GetOECFTagPresence () const;
    const StillArray<char>& GetOECF () const;

    TagPresenceType GetShutterSpeedValueTagPresence () const;
    float GetShutterSpeedValue () const;

    TagPresenceType GetApertureValueTagPresence () const;
    float GetApertureValue () const;

    TagPresenceType GetBrightnessValueTagPresence () const;
    BrightnessValueStateType GetBrightnessValueState () const;
    float GetBrightnessValue () const;

    TagPresenceType GetExposureBiasValueTagPresence () const;
    float GetExposureBiasValue () const;

    TagPresenceType GetMaxApertureValueTagPresence () const;
    float GetMaxApertureValue () const;

    TagPresenceType GetSubjectDistanceTagPresence () const;
    SubjectDistanceStateType GetSubjectDistanceState () const;
    float GetSubjectDistance () const;

    TagPresenceType GetMeteringModeTagPresence () const;
    MeteringModeType GetMeteringMode () const;

    TagPresenceType GetLightSourceTagPresence () const;
    LightSourceType GetLightSource () const;

    TagPresenceType GetFlashTagPresence () const;
    FlashType GetFlash () const;

    TagPresenceType GetSubjectAreaTagPresence () const;
    const StillArray<int>& GetSubjectAreaCoordinates () const;

    TagPresenceType GetFocalLengthTagPresence () const;
    float GetFocalLength () const;

    TagPresenceType GetFlashEnergyTagPresence () const;
    float GetFlashEnergy () const;

    TagPresenceType GetSpatialFrequencyResponseTagPresence () const;
    const StillArray<char>& GetSpatialFrequencyResponse () const;

    TagPresenceType GetFocalPlaneXResolutionTagPresence () const;
    float GetFocalPlaneXResolution () const;

    TagPresenceType GetFocalPlaneYResolutionTagPresence () const;
    float GetFocalPlaneYResolution () const;

    TagPresenceType GetFocalPlaneResolutionUnitTagPresence () const;
    ResolutionUnitType GetFocalPlaneResolutionUnit () const;

    TagPresenceType GetSubjectLocationTagPresence () const;
    int GetSubjectLocationX () const;
    int GetSubjectLocationY () const;

    TagPresenceType GetExposureIndexTagPresence () const;
    float GetExposureIndex () const;

    TagPresenceType GetSensingMethodTagPresence () const;
    SensingMethodType GetSensingMethod () const;

    TagPresenceType GetFileSourceTagPresence () const;
    FileSourceType GetFileSource () const;

    TagPresenceType GetSceneTypeTagPresence () const;
    SceneType GetSceneType () const;

    TagPresenceType GetCFAPatternTagPresence () const;
    const StillArray<char>& GetCFAPattern () const;

    TagPresenceType GetCustomRenderedTagPresence () const;
    CustomRenderedType GetCustomRendered () const;

    TagPresenceType GetExposureModeTagPresence () const;
    ExposureModeType GetExposureMode () const;

    TagPresenceType GetWhiteBalanceTagPresence () const;
    WhiteBalanceType GetWhiteBalance () const;

    TagPresenceType GetDigitalZoomRatioTagPresence () const;
    DigitalZoomStateType GetDigitalZoomState () const;
    float GetDigitalZoomRatio () const;

    TagPresenceType GetFocalLengthIn35mmFilmTagPresence () const;
    FocalLengthIn35mmFilmStateType GetFocalLengthIn35mmFilmState () const;
    int GetFocalLengthIn35mmFilm () const;

    TagPresenceType GetSceneCaptureTypeTagPresence () const;
    SceneCaptureType GetSceneCaptureType () const;

    TagPresenceType GetGainControlTagPresence () const;
    GainControlType GetGainControl () const;

    TagPresenceType GetContrastTagPresence () const;
    ContrastType GetContrast () const;

    TagPresenceType GetSaturationTagPresence () const;
    SaturationType GetSaturation () const;

    TagPresenceType GetSharpnessTagPresence () const;
    SharpnessType GetSharpness () const;

    TagPresenceType GetDeviceSettingDescriptionTagPresence () const;
    const StillArray<char>& GetDeviceSettingDescription () const;

    TagPresenceType GetSubjectDistanceRangeTagPresence () const;
    SubjectDistanceRangeType GetSubjectDistanceRange () const;

    // Other tags (in Exif IFD Attribute Information).

    TagPresenceType GetImageUniqueIdTagPresence () const;
    const String& GetImageUniqueId () const;

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

private:
    enum ExifType
    {
        EXIFT_BYTE,
        EXIFT_ASCII,
        EXIFT_SHORT,
        EXIFT_LONG,
        EXIFT_RATIONAL,
        EXIFT_UNDEFINED,
        EXIFT_SLONG,
        EXIFT_SRATIONAL
    };

    ByteOrderType m_eByteOrder;

    TagPresenceType m_eImageWidthTagPresence;
    int m_iImageWidth;

    TagPresenceType m_eImageLengthTagPresence;
    int m_iImageLength;

    TagPresenceType m_eBitsPerSampleTagPresence;
    int m_iBitsPerSample0, m_iBitsPerSample1, m_iBitsPerSample2;

    TagPresenceType m_eCompressionTagPresence;
    CompressionSchemeType m_eCompression;

    TagPresenceType m_ePhotometricInterpretationTagPresence;
    PhotometricInterpretationType m_ePhotometricInterpretation;

    TagPresenceType m_eOrientationTagPresence;
    OrientationType m_eOrientation;

    TagPresenceType m_eSamplesPerPixelTagPresence;
    int m_iSamplesPerPixel;

    TagPresenceType m_ePlanarConfigurationTagPresence;
    PlanarConfigurationType m_ePlanarConfiguration;

    TagPresenceType m_eYCbCrSubSamplingTagPresence;
    YCbCrSubSamplingType m_eYCbCrSubSampling;

    TagPresenceType m_eYCbCrPositioningTagPresence;
    YCbCrPositioningType m_eYCbCrPositioning;

    TagPresenceType m_eXResolutionTagPresence;
    float m_fXResolution;

    TagPresenceType m_eYResolutionTagPresence;
    float m_fYResolution;

    TagPresenceType m_eResolutionUnitTagPresence;
    ResolutionUnitType m_eResolutionUnit;

    TagPresenceType m_eStripOffsetsTagPresence;
    StillArray<int> m_qStripOffsets;

    TagPresenceType m_eRowsPerStripTagPresence;
    int m_iRowsPerStrip;

    TagPresenceType m_eStripByteCountsTagPresence;
    StillArray<int> m_qStripByteCounts;

    TagPresenceType m_eJPEGInterchangeFormatTagPresence;
    int m_iJPEGInterchangeFormat;

    TagPresenceType m_eJPEGInterchangeFormatLengthTagPresence;
    int m_iJPEGInterchangeFormatLength;

    TagPresenceType m_eTransferFunctionTagPresence;
    StillArray<int> m_qTransferFunction;

    TagPresenceType m_eWhitePointTagPresence;
    float m_fWhitePointX, m_fWhitePointY;

    TagPresenceType m_ePrimaryChromaticitiesTagPresence;
    float m_fPrimaryChromaticityRedX, m_fPrimaryChromaticityRedY, m_fPrimaryChromaticityGreenX,
        m_fPrimaryChromaticityGreenY, m_fPrimaryChromaticityBlueX, m_fPrimaryChromaticityBlueY;

    TagPresenceType m_eYCbCrCoefficientsTagPresence;
    float m_fCoefficientY, m_fCoefficientCb, m_fCoefficientCr;

    TagPresenceType m_eReferenceBlackWhiteTagPresence;
    Link<float,float> m_qReferenceBlackWhiteComponent0, m_qReferenceBlackWhiteComponent1,
        m_qReferenceBlackWhiteComponent2;

    TagPresenceType m_eDateTimeTagPresence;
    Date m_qDateTime;

    TagPresenceType m_eImageDescriptionTagPresence;
    String m_qImageDescription;

    TagPresenceType m_eMakeTagPresence;
    String m_qMake;

    TagPresenceType m_eModelTagPresence;
    String m_qModel;

    TagPresenceType m_eSoftwareTagPresence;
    String m_qSoftware;

    TagPresenceType m_eArtistTagPresence;
    String m_qArtist;

    TagPresenceType m_eCopyrightTagPresence;
    bool m_bPhotographerCopyrightIsPresent;
    bool m_bEditorCopyrightIsPresent;
    String m_qPhotographerCopyright;
    String m_qEditorCopyright;

    TagPresenceType m_eExifIFDPointerTagPresence;

    TagPresenceType m_eExifVersionTagPresence;
    ExifVersionType m_eExifVersion;

    TagPresenceType m_eFlashpixVersionTagPresence;
    FlashpixVersionType m_eFlashpixVersion;

    TagPresenceType m_eColorSpaceTagPresence;
    ColorSpaceType m_eColorSpace;

    TagPresenceType m_ePixelXDimensionTagPresence;
    int m_iPixelXDimension;

    TagPresenceType m_ePixelYDimensionTagPresence;
    int m_iPixelYDimension;

    TagPresenceType m_eComponentsConfigurationTagPresence;
    ComponentsConfigurationType m_eComponentsConfiguration;

    TagPresenceType m_eCompressedBitsPerPixelTagPresence;
    float m_fCompressedBitsPerPixel;

    TagPresenceType m_eMakerNoteTagPresence;
    StillArray<char> m_qMakerNote;

    TagPresenceType m_eUserCommentTagPresence;
    UserCommentCharacterCodeType m_eUserCommentCharacterCode;
    StillArray<char> m_qUserComment;

    TagPresenceType m_eRelatedSoundFileTagPresence;
    String m_qRelatedSoundFile;

    TagPresenceType m_eDateTimeOriginalTagPresence;
    Date m_qDateTimeOriginal;

    TagPresenceType m_eDateTimeDigitizedTagPresence;
    Date m_qDateTimeDigitized;

    TagPresenceType m_eSubsecTimeTagPresence;
    String m_qSubsecTime;

    TagPresenceType m_eSubsecTimeOriginalTagPresence;
    String m_qSubsecTimeOriginal;

    TagPresenceType m_eSubsecTimeDigitizedTagPresence;
    String m_qSubsecTimeDigitized;

    TagPresenceType m_eExposureTimeTagPresence;
    float m_fExposureTime;

    TagPresenceType m_eFNumberTagPresence;
    float m_fFNumber;

    TagPresenceType m_eExposureProgramTagPresence;
    ExposureProgramType m_eExposureProgram;

    TagPresenceType m_eSpectralSensitivityTagPresence;
    String m_qSpectralSensitivity;

    TagPresenceType m_eISOSpeedRatingsTagPresence;
    StillArray<int> m_qISOSpeedRatings;

    TagPresenceType m_eOECFTagPresence;
    StillArray<char> m_qOECF;

    TagPresenceType m_eShutterSpeedValueTagPresence;
    float m_fShutterSpeedValue;

    TagPresenceType m_eApertureValueTagPresence;
    float m_fApertureValue;

    TagPresenceType m_eBrightnessValueTagPresence;
    BrightnessValueStateType m_eBrightnessValueState;
    float m_fBrightnessValue;

    TagPresenceType m_eExposureBiasValueTagPresence;
    float m_fExposureBiasValue;

    TagPresenceType m_eMaxApertureValueTagPresence;
    float m_fMaxApertureValue;

    TagPresenceType m_eSubjectDistanceTagPresence;
    SubjectDistanceStateType m_eSubjectDistanceState;
    float m_fSubjectDistance;

    TagPresenceType m_eMeteringModeTagPresence;
    MeteringModeType m_eMeteringMode;

    TagPresenceType m_eLightSourceTagPresence;
    LightSourceType m_eLightSource;

    TagPresenceType m_eFlashTagPresence;
    FlashType m_eFlash;

    TagPresenceType m_eSubjectAreaTagPresence;
    StillArray<int> m_qSubjectAreaCoordinates;

    TagPresenceType m_eFocalLengthTagPresence;
    float m_fFocalLength;

    TagPresenceType m_eFlashEnergyTagPresence;
    float m_fFlashEnergy;

    TagPresenceType m_eSpatialFrequencyResponseTagPresence;
    StillArray<char> m_qSpatialFrequencyResponse;

    TagPresenceType m_eFocalPlaneXResolutionTagPresence;
    float m_fFocalPlaneXResolution;

    TagPresenceType m_eFocalPlaneYResolutionTagPresence;
    float m_fFocalPlaneYResolution;

    TagPresenceType m_eFocalPlaneResolutionUnitTagPresence;
    ResolutionUnitType m_eFocalPlaneResolutionUnit;

    TagPresenceType m_eSubjectLocationTagPresence;
    int m_iSubjectLocationX, m_iSubjectLocationY;

    TagPresenceType m_eExposureIndexTagPresence;
    float m_fExposureIndex;

    TagPresenceType m_eSensingMethodTagPresence;
    SensingMethodType m_eSensingMethod;

    TagPresenceType m_eFileSourceTagPresence;
    FileSourceType m_eFileSource;

    TagPresenceType m_eSceneTypeTagPresence;
    SceneType m_eSceneType;

    TagPresenceType m_eCFAPatternTagPresence;
    StillArray<char> m_qCFAPattern;

    TagPresenceType m_eCustomRenderedTagPresence;
    CustomRenderedType m_eCustomRendered;

    TagPresenceType m_eExposureModeTagPresence;
    ExposureModeType m_eExposureMode;

    TagPresenceType m_eWhiteBalanceTagPresence;
    WhiteBalanceType m_eWhiteBalance;

    TagPresenceType m_eDigitalZoomRatioTagPresence;
    DigitalZoomStateType m_eDigitalZoomState;
    float m_fDigitalZoomRatio;

    TagPresenceType m_eFocalLengthIn35mmFilmTagPresence;
    FocalLengthIn35mmFilmStateType m_eFocalLengthIn35mmFilmState;
    int m_iFocalLengthIn35mmFilm;

    TagPresenceType m_eSceneCaptureTypeTagPresence;
    SceneCaptureType m_eSceneCaptureType;

    TagPresenceType m_eGainControlTagPresence;
    GainControlType m_eGainControl;

    TagPresenceType m_eContrastTagPresence;
    ContrastType m_eContrast;

    TagPresenceType m_eSaturationTagPresence;
    SaturationType m_eSaturation;

    TagPresenceType m_eSharpnessTagPresence;
    SharpnessType m_eSharpness;

    TagPresenceType m_eDeviceSettingDescriptionTagPresence;
    StillArray<char> m_qDeviceSettingDescription;

    TagPresenceType m_eSubjectDistanceRangeTagPresence;
    SubjectDistanceRangeType m_eSubjectDistanceRange;

    TagPresenceType m_eImageUniqueIdTagPresence;
    String m_qImageUniqueId;

    static const int ms_iRationalNumbersScaler;  // 10000

    // specifies if to perform trimming of read text strings
    static const bool ms_bTrimTexts;  // true
};

#include "QFormatExif.inl"

typedef SmartPointer<FormatExif> FormatExifPtr;

}

#endif



