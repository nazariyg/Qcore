#include "QFormatExif.h"
#include "QExptWarnBase.mcr"
#include "QFormatBase.mcr"
#include "QFormatExif.mcr"
using namespace Q;

// support for the UserComment tag
static const char gs_acCharacterCodeASCII[8] = {0x41, 0x53, 0x43, 0x49, 0x49, 0x00, 0x00, 0x00};
static const char gs_acCharacterCodeJIS[8] = {0x4a, 0x49, 0x53, 0x00, 0x00, 0x00, 0x00, 0x00};
static const char gs_acCharacterCodeUnicode[8] = {0x55, 0x4e, 0x49, 0x43, 0x4f, 0x44, 0x45, 0x00};
static const char gs_acCharacterCodeUndefined[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

const int FormatExif::ms_iRationalNumbersScaler(10000);
const bool FormatExif::ms_bTrimTexts(true);

//------------------------------------------------------------------------------------------------------------------
FormatExif::FormatExif (ByteOrderType eByteOrder)
    :
    m_eByteOrder(eByteOrder),
    ALL_TAGS_ARE_ABSENT
{
}
//------------------------------------------------------------------------------------------------------------------
FormatExif::FormatExif (const FormatExif& rqSourceExif, ByteOrderType eByteOrder)
    :
    m_eByteOrder(eByteOrder),
    m_eExifIFDPointerTagPresence(TPT_ABSENT)
{
    m_eImageWidthTagPresence = rqSourceExif.m_eImageWidthTagPresence;
    m_iImageWidth = rqSourceExif.m_iImageWidth;

    m_eImageLengthTagPresence = rqSourceExif.m_eImageLengthTagPresence;
    m_iImageLength = rqSourceExif.m_iImageLength;

    m_eBitsPerSampleTagPresence = rqSourceExif.m_eBitsPerSampleTagPresence;
    m_iBitsPerSample0 = rqSourceExif.m_iBitsPerSample0;
    m_iBitsPerSample1 = rqSourceExif.m_iBitsPerSample1;
    m_iBitsPerSample2 = rqSourceExif.m_iBitsPerSample2;

    m_eCompressionTagPresence = rqSourceExif.m_eCompressionTagPresence;
    m_eCompression = rqSourceExif.m_eCompression;

    m_ePhotometricInterpretationTagPresence = rqSourceExif.m_ePhotometricInterpretationTagPresence;
    m_ePhotometricInterpretation = rqSourceExif.m_ePhotometricInterpretation;

    m_eOrientationTagPresence = rqSourceExif.m_eOrientationTagPresence;
    m_eOrientation = rqSourceExif.m_eOrientation;

    m_eSamplesPerPixelTagPresence = rqSourceExif.m_eSamplesPerPixelTagPresence;
    m_iSamplesPerPixel = rqSourceExif.m_iSamplesPerPixel;

    m_ePlanarConfigurationTagPresence = rqSourceExif.m_ePlanarConfigurationTagPresence;
    m_ePlanarConfiguration = rqSourceExif.m_ePlanarConfiguration;

    m_eYCbCrSubSamplingTagPresence = rqSourceExif.m_eYCbCrSubSamplingTagPresence;
    m_eYCbCrSubSampling = rqSourceExif.m_eYCbCrSubSampling;

    m_eYCbCrPositioningTagPresence = rqSourceExif.m_eYCbCrPositioningTagPresence;
    m_eYCbCrPositioning = rqSourceExif.m_eYCbCrPositioning;

    m_eXResolutionTagPresence = rqSourceExif.m_eXResolutionTagPresence;
    m_fXResolution = rqSourceExif.m_fXResolution;

    m_eYResolutionTagPresence = rqSourceExif.m_eYResolutionTagPresence;
    m_fYResolution = rqSourceExif.m_fYResolution;

    m_eResolutionUnitTagPresence = rqSourceExif.m_eResolutionUnitTagPresence;
    m_eResolutionUnit = rqSourceExif.m_eResolutionUnit;

    m_eStripOffsetsTagPresence = rqSourceExif.m_eStripOffsetsTagPresence;
    m_qStripOffsets = rqSourceExif.m_qStripOffsets;

    m_eRowsPerStripTagPresence = rqSourceExif.m_eRowsPerStripTagPresence;
    m_iRowsPerStrip = rqSourceExif.m_iRowsPerStrip;

    m_eStripByteCountsTagPresence = rqSourceExif.m_eStripByteCountsTagPresence;
    m_qStripByteCounts = rqSourceExif.m_qStripByteCounts;

    m_eJPEGInterchangeFormatTagPresence = rqSourceExif.m_eJPEGInterchangeFormatTagPresence;
    m_iJPEGInterchangeFormat = rqSourceExif.m_iJPEGInterchangeFormat;

    m_eJPEGInterchangeFormatLengthTagPresence = rqSourceExif.m_eJPEGInterchangeFormatLengthTagPresence;
    m_iJPEGInterchangeFormatLength = rqSourceExif.m_iJPEGInterchangeFormatLength;

    m_eTransferFunctionTagPresence = rqSourceExif.m_eTransferFunctionTagPresence;
    m_qTransferFunction = rqSourceExif.m_qTransferFunction;

    m_eWhitePointTagPresence = rqSourceExif.m_eWhitePointTagPresence;
    m_fWhitePointX = rqSourceExif.m_fWhitePointX;
    m_fWhitePointY = rqSourceExif.m_fWhitePointY;

    m_ePrimaryChromaticitiesTagPresence = rqSourceExif.m_ePrimaryChromaticitiesTagPresence;
    m_fPrimaryChromaticityRedX = rqSourceExif.m_fPrimaryChromaticityRedX;
    m_fPrimaryChromaticityRedY = rqSourceExif.m_fPrimaryChromaticityRedY;
    m_fPrimaryChromaticityGreenX = rqSourceExif.m_fPrimaryChromaticityGreenX;
    m_fPrimaryChromaticityGreenY = rqSourceExif.m_fPrimaryChromaticityGreenY;
    m_fPrimaryChromaticityBlueX = rqSourceExif.m_fPrimaryChromaticityBlueX;
    m_fPrimaryChromaticityBlueY = rqSourceExif.m_fPrimaryChromaticityBlueY;

    m_eYCbCrCoefficientsTagPresence = rqSourceExif.m_eYCbCrCoefficientsTagPresence;
    m_fCoefficientY = rqSourceExif.m_fCoefficientY;
    m_fCoefficientCb = rqSourceExif.m_fCoefficientCb;
    m_fCoefficientCr = rqSourceExif.m_fCoefficientCr;

    m_eReferenceBlackWhiteTagPresence = rqSourceExif.m_eReferenceBlackWhiteTagPresence;
    m_qReferenceBlackWhiteComponent0 = rqSourceExif.m_qReferenceBlackWhiteComponent0;
    m_qReferenceBlackWhiteComponent1 = rqSourceExif.m_qReferenceBlackWhiteComponent1;
    m_qReferenceBlackWhiteComponent2 = rqSourceExif.m_qReferenceBlackWhiteComponent2;

    m_eDateTimeTagPresence = rqSourceExif.m_eDateTimeTagPresence;
    m_qDateTime = rqSourceExif.m_qDateTime;

    m_eImageDescriptionTagPresence = rqSourceExif.m_eImageDescriptionTagPresence;
    m_qImageDescription = rqSourceExif.m_qImageDescription;

    m_eMakeTagPresence = rqSourceExif.m_eMakeTagPresence;
    m_qMake = rqSourceExif.m_qMake;

    m_eModelTagPresence = rqSourceExif.m_eModelTagPresence;
    m_qModel = rqSourceExif.m_qModel;

    m_eSoftwareTagPresence = rqSourceExif.m_eSoftwareTagPresence;
    m_qSoftware = rqSourceExif.m_qSoftware;

    m_eArtistTagPresence = rqSourceExif.m_eArtistTagPresence;
    m_qArtist = rqSourceExif.m_qArtist;

    m_eCopyrightTagPresence = rqSourceExif.m_eCopyrightTagPresence;
    m_qPhotographerCopyright = rqSourceExif.m_qPhotographerCopyright;
    m_qEditorCopyright = rqSourceExif.m_qEditorCopyright;

    m_eExifVersionTagPresence = rqSourceExif.m_eExifVersionTagPresence;
    m_eExifVersion = rqSourceExif.m_eExifVersion;

    m_eFlashpixVersionTagPresence = rqSourceExif.m_eFlashpixVersionTagPresence;
    m_eFlashpixVersion = rqSourceExif.m_eFlashpixVersion;

    m_eColorSpaceTagPresence = rqSourceExif.m_eColorSpaceTagPresence;
    m_eColorSpace = rqSourceExif.m_eColorSpace;

    m_ePixelXDimensionTagPresence = rqSourceExif.m_ePixelXDimensionTagPresence;
    m_iPixelXDimension = rqSourceExif.m_iPixelXDimension;

    m_ePixelYDimensionTagPresence = rqSourceExif.m_ePixelYDimensionTagPresence;
    m_iPixelYDimension = rqSourceExif.m_iPixelYDimension;

    m_eComponentsConfigurationTagPresence = rqSourceExif.m_eComponentsConfigurationTagPresence;
    m_eComponentsConfiguration = rqSourceExif.m_eComponentsConfiguration;

    m_eCompressedBitsPerPixelTagPresence = rqSourceExif.m_eCompressedBitsPerPixelTagPresence;
    m_fCompressedBitsPerPixel = rqSourceExif.m_fCompressedBitsPerPixel;

    m_eMakerNoteTagPresence = rqSourceExif.m_eMakerNoteTagPresence;
    m_qMakerNote = rqSourceExif.m_qMakerNote;

    m_eUserCommentTagPresence = rqSourceExif.m_eUserCommentTagPresence;
    m_eUserCommentCharacterCode = rqSourceExif.m_eUserCommentCharacterCode;
    m_qUserComment = rqSourceExif.m_qUserComment;

    m_eRelatedSoundFileTagPresence = rqSourceExif.m_eRelatedSoundFileTagPresence;
    m_qRelatedSoundFile = rqSourceExif.m_qRelatedSoundFile;

    m_eDateTimeOriginalTagPresence = rqSourceExif.m_eDateTimeOriginalTagPresence;
    m_qDateTimeOriginal = rqSourceExif.m_qDateTimeOriginal;

    m_eDateTimeDigitizedTagPresence = rqSourceExif.m_eDateTimeDigitizedTagPresence;
    m_qDateTimeDigitized = rqSourceExif.m_qDateTimeDigitized;

    m_eSubsecTimeTagPresence = rqSourceExif.m_eSubsecTimeTagPresence;
    m_qSubsecTime = rqSourceExif.m_qSubsecTime;

    m_eSubsecTimeOriginalTagPresence = rqSourceExif.m_eSubsecTimeOriginalTagPresence;
    m_qSubsecTimeOriginal = rqSourceExif.m_qSubsecTimeOriginal;

    m_eSubsecTimeDigitizedTagPresence = rqSourceExif.m_eSubsecTimeDigitizedTagPresence;
    m_qSubsecTimeDigitized = rqSourceExif.m_qSubsecTimeDigitized;

    m_eExposureTimeTagPresence = rqSourceExif.m_eExposureTimeTagPresence;
    m_fExposureTime = rqSourceExif.m_fExposureTime;

    m_eFNumberTagPresence = rqSourceExif.m_eFNumberTagPresence;
    m_fFNumber = rqSourceExif.m_fFNumber;

    m_eExposureProgramTagPresence = rqSourceExif.m_eExposureProgramTagPresence;
    m_eExposureProgram = rqSourceExif.m_eExposureProgram;

    m_eSpectralSensitivityTagPresence = rqSourceExif.m_eSpectralSensitivityTagPresence;
    m_qSpectralSensitivity = rqSourceExif.m_qSpectralSensitivity;

    m_eISOSpeedRatingsTagPresence = rqSourceExif.m_eISOSpeedRatingsTagPresence;
    m_qISOSpeedRatings = rqSourceExif.m_qISOSpeedRatings;

    m_eOECFTagPresence = rqSourceExif.m_eOECFTagPresence;
    m_qOECF = rqSourceExif.m_qOECF;

    m_eShutterSpeedValueTagPresence = rqSourceExif.m_eShutterSpeedValueTagPresence;
    m_fShutterSpeedValue = rqSourceExif.m_fShutterSpeedValue;

    m_eApertureValueTagPresence = rqSourceExif.m_eApertureValueTagPresence;
    m_fApertureValue = rqSourceExif.m_fApertureValue;

    m_eBrightnessValueTagPresence = rqSourceExif.m_eBrightnessValueTagPresence;
    m_eBrightnessValueState = rqSourceExif.m_eBrightnessValueState;
    m_fBrightnessValue = rqSourceExif.m_fBrightnessValue;

    m_eExposureBiasValueTagPresence = rqSourceExif.m_eExposureBiasValueTagPresence;
    m_fExposureBiasValue = rqSourceExif.m_fExposureBiasValue;

    m_eMaxApertureValueTagPresence = rqSourceExif.m_eMaxApertureValueTagPresence;
    m_fMaxApertureValue = rqSourceExif.m_fMaxApertureValue;

    m_eSubjectDistanceTagPresence = rqSourceExif.m_eSubjectDistanceTagPresence;
    m_eSubjectDistanceState = rqSourceExif.m_eSubjectDistanceState;
    m_fSubjectDistance = rqSourceExif.m_fSubjectDistance;

    m_eMeteringModeTagPresence = rqSourceExif.m_eMeteringModeTagPresence;
    m_eMeteringMode = rqSourceExif.m_eMeteringMode;

    m_eLightSourceTagPresence = rqSourceExif.m_eLightSourceTagPresence;
    m_eLightSource = rqSourceExif.m_eLightSource;

    m_eFlashTagPresence = rqSourceExif.m_eFlashTagPresence;
    m_eFlash = rqSourceExif.m_eFlash;

    m_eSubjectAreaTagPresence = rqSourceExif.m_eSubjectAreaTagPresence;
    m_qSubjectAreaCoordinates = rqSourceExif.m_qSubjectAreaCoordinates;

    m_eFocalLengthTagPresence = rqSourceExif.m_eFocalLengthTagPresence;
    m_fFocalLength = rqSourceExif.m_fFocalLength;

    m_eFlashEnergyTagPresence = rqSourceExif.m_eFlashEnergyTagPresence;
    m_fFlashEnergy = rqSourceExif.m_fFlashEnergy;

    m_eSpatialFrequencyResponseTagPresence = rqSourceExif.m_eSpatialFrequencyResponseTagPresence;
    m_qSpatialFrequencyResponse = rqSourceExif.m_qSpatialFrequencyResponse;

    m_eFocalPlaneXResolutionTagPresence = rqSourceExif.m_eFocalPlaneXResolutionTagPresence;
    m_fFocalPlaneXResolution = rqSourceExif.m_fFocalPlaneXResolution;

    m_eFocalPlaneYResolutionTagPresence = rqSourceExif.m_eFocalPlaneYResolutionTagPresence;
    m_fFocalPlaneYResolution = rqSourceExif.m_fFocalPlaneYResolution;

    m_eFocalPlaneResolutionUnitTagPresence = rqSourceExif.m_eFocalPlaneResolutionUnitTagPresence;
    m_eFocalPlaneResolutionUnit = rqSourceExif.m_eFocalPlaneResolutionUnit;

    m_eSubjectLocationTagPresence = rqSourceExif.m_eSubjectLocationTagPresence;
    m_iSubjectLocationX = rqSourceExif.m_iSubjectLocationX;
    m_iSubjectLocationY = rqSourceExif.m_iSubjectLocationY;

    m_eExposureIndexTagPresence = rqSourceExif.m_eExposureIndexTagPresence;
    m_fExposureIndex = rqSourceExif.m_fExposureIndex;

    m_eSensingMethodTagPresence = rqSourceExif.m_eSensingMethodTagPresence;
    m_eSensingMethod = rqSourceExif.m_eSensingMethod;

    m_eFileSourceTagPresence = rqSourceExif.m_eFileSourceTagPresence;
    m_eFileSource = rqSourceExif.m_eFileSource;

    m_eSceneTypeTagPresence = rqSourceExif.m_eSceneTypeTagPresence;
    m_eSceneType = rqSourceExif.m_eSceneType;

    m_eCFAPatternTagPresence = rqSourceExif.m_eCFAPatternTagPresence;
    m_qCFAPattern = rqSourceExif.m_qCFAPattern;

    m_eCustomRenderedTagPresence = rqSourceExif.m_eCustomRenderedTagPresence;
    m_eCustomRendered = rqSourceExif.m_eCustomRendered;

    m_eExposureModeTagPresence = rqSourceExif.m_eExposureModeTagPresence;
    m_eExposureMode = rqSourceExif.m_eExposureMode;

    m_eWhiteBalanceTagPresence = rqSourceExif.m_eWhiteBalanceTagPresence;
    m_eWhiteBalance = rqSourceExif.m_eWhiteBalance;

    m_eDigitalZoomRatioTagPresence = rqSourceExif.m_eDigitalZoomRatioTagPresence;
    m_eDigitalZoomState = rqSourceExif.m_eDigitalZoomState;
    m_fDigitalZoomRatio = rqSourceExif.m_fDigitalZoomRatio;

    m_eFocalLengthIn35mmFilmTagPresence = rqSourceExif.m_eFocalLengthIn35mmFilmTagPresence;
    m_eFocalLengthIn35mmFilmState = rqSourceExif.m_eFocalLengthIn35mmFilmState;
    m_iFocalLengthIn35mmFilm = rqSourceExif.m_iFocalLengthIn35mmFilm;

    m_eSceneCaptureTypeTagPresence = rqSourceExif.m_eSceneCaptureTypeTagPresence;
    m_eSceneCaptureType = rqSourceExif.m_eSceneCaptureType;

    m_eGainControlTagPresence = rqSourceExif.m_eGainControlTagPresence;
    m_eGainControl = rqSourceExif.m_eGainControl;

    m_eContrastTagPresence = rqSourceExif.m_eContrastTagPresence;
    m_eContrast = rqSourceExif.m_eContrast;

    m_eSaturationTagPresence = rqSourceExif.m_eSaturationTagPresence;
    m_eSaturation = rqSourceExif.m_eSaturation;

    m_eSharpnessTagPresence = rqSourceExif.m_eSharpnessTagPresence;
    m_eSharpness = rqSourceExif.m_eSharpness;

    m_eDeviceSettingDescriptionTagPresence = rqSourceExif.m_eDeviceSettingDescriptionTagPresence;
    m_qDeviceSettingDescription = rqSourceExif.m_qDeviceSettingDescription;

    m_eSubjectDistanceRangeTagPresence = rqSourceExif.m_eSubjectDistanceRangeTagPresence;
    m_eSubjectDistanceRange = rqSourceExif.m_eSubjectDistanceRange;

    m_eImageUniqueIdTagPresence = rqSourceExif.m_eImageUniqueIdTagPresence;
    m_qImageUniqueId = rqSourceExif.m_qImageUniqueId;
}
//------------------------------------------------------------------------------------------------------------------
void FormatExif::SaveData (StillArray<char>& rqStaticBytestream, ExifDataDestinationType eExifDataDestination,
    int iTiffPreOffset)
{
    assert( !(eExifDataDestination == EDDT_TIFF && iTiffPreOffset < 0) );

    rqStaticBytestream.RemoveAll();

    // Count tags to be written.
    unsigned short usTagQuantity0 = 0;
    unsigned short usTagQuantity1 = 0;
    if ( m_eExifVersionTagPresence == TPT_PRESENT )
        usTagQuantity1++;
    if ( m_eFlashpixVersionTagPresence == TPT_PRESENT )
        usTagQuantity1++;
    if ( m_eColorSpaceTagPresence == TPT_PRESENT )
        usTagQuantity1++;
    if ( m_ePixelXDimensionTagPresence == TPT_PRESENT )
        usTagQuantity1++;
    if ( m_ePixelYDimensionTagPresence == TPT_PRESENT )
        usTagQuantity1++;
    if ( m_eComponentsConfigurationTagPresence == TPT_PRESENT )
        usTagQuantity1++;
    if ( m_eCompressedBitsPerPixelTagPresence == TPT_PRESENT )
        usTagQuantity1++;
    if ( m_eMakerNoteTagPresence == TPT_PRESENT )
        usTagQuantity1++;
    if ( m_eUserCommentTagPresence == TPT_PRESENT )
        usTagQuantity1++;
    if ( m_eRelatedSoundFileTagPresence == TPT_PRESENT )
        usTagQuantity1++;
    if ( m_eDateTimeOriginalTagPresence == TPT_PRESENT )
        usTagQuantity1++;
    if ( m_eDateTimeDigitizedTagPresence == TPT_PRESENT )
        usTagQuantity1++;
    if ( m_eSubsecTimeTagPresence == TPT_PRESENT )
        usTagQuantity1++;
    if ( m_eSubsecTimeOriginalTagPresence == TPT_PRESENT )
        usTagQuantity1++;
    if ( m_eSubsecTimeDigitizedTagPresence == TPT_PRESENT )
        usTagQuantity1++;
    if ( m_eExposureTimeTagPresence == TPT_PRESENT )
        usTagQuantity1++;
    if ( m_eFNumberTagPresence == TPT_PRESENT )
        usTagQuantity1++;
    if ( m_eExposureProgramTagPresence == TPT_PRESENT )
        usTagQuantity1++;
    if ( m_eSpectralSensitivityTagPresence == TPT_PRESENT )
        usTagQuantity1++;
    if ( m_eISOSpeedRatingsTagPresence == TPT_PRESENT )
        usTagQuantity1++;
    if ( m_eOECFTagPresence == TPT_PRESENT )
        usTagQuantity1++;
    if ( m_eShutterSpeedValueTagPresence == TPT_PRESENT )
        usTagQuantity1++;
    if ( m_eApertureValueTagPresence == TPT_PRESENT )
        usTagQuantity1++;
    if ( m_eBrightnessValueTagPresence == TPT_PRESENT )
        usTagQuantity1++;
    if ( m_eExposureBiasValueTagPresence == TPT_PRESENT )
        usTagQuantity1++;
    if ( m_eMaxApertureValueTagPresence == TPT_PRESENT )
        usTagQuantity1++;
    if ( m_eSubjectDistanceTagPresence == TPT_PRESENT )
        usTagQuantity1++;
    if ( m_eMeteringModeTagPresence == TPT_PRESENT )
        usTagQuantity1++;
    if ( m_eLightSourceTagPresence == TPT_PRESENT )
        usTagQuantity1++;
    if ( m_eFlashTagPresence == TPT_PRESENT )
        usTagQuantity1++;
    if ( m_eSubjectAreaTagPresence == TPT_PRESENT )
        usTagQuantity1++;
    if ( m_eFocalLengthTagPresence == TPT_PRESENT )
        usTagQuantity1++;
    if ( m_eFlashEnergyTagPresence == TPT_PRESENT )
        usTagQuantity1++;
    if ( m_eSpatialFrequencyResponseTagPresence == TPT_PRESENT )
        usTagQuantity1++;
    if ( m_eFocalPlaneXResolutionTagPresence == TPT_PRESENT )
        usTagQuantity1++;
    if ( m_eFocalPlaneYResolutionTagPresence == TPT_PRESENT )
        usTagQuantity1++;
    if ( m_eFocalPlaneResolutionUnitTagPresence == TPT_PRESENT )
        usTagQuantity1++;
    if ( m_eSubjectLocationTagPresence == TPT_PRESENT )
        usTagQuantity1++;
    if ( m_eExposureIndexTagPresence == TPT_PRESENT )
        usTagQuantity1++;
    if ( m_eSensingMethodTagPresence == TPT_PRESENT )
        usTagQuantity1++;
    if ( m_eFileSourceTagPresence == TPT_PRESENT )
        usTagQuantity1++;
    if ( m_eSceneTypeTagPresence == TPT_PRESENT )
        usTagQuantity1++;
    if ( m_eCFAPatternTagPresence == TPT_PRESENT )
        usTagQuantity1++;
    if ( m_eCustomRenderedTagPresence == TPT_PRESENT )
        usTagQuantity1++;
    if ( m_eExposureModeTagPresence == TPT_PRESENT )
        usTagQuantity1++;
    if ( m_eWhiteBalanceTagPresence == TPT_PRESENT )
        usTagQuantity1++;
    if ( m_eDigitalZoomRatioTagPresence == TPT_PRESENT )
        usTagQuantity1++;
    if ( m_eFocalLengthIn35mmFilmTagPresence == TPT_PRESENT )
        usTagQuantity1++;
    if ( m_eSceneCaptureTypeTagPresence == TPT_PRESENT )
        usTagQuantity1++;
    if ( m_eGainControlTagPresence == TPT_PRESENT )
        usTagQuantity1++;
    if ( m_eContrastTagPresence == TPT_PRESENT )
        usTagQuantity1++;
    if ( m_eSaturationTagPresence == TPT_PRESENT )
        usTagQuantity1++;
    if ( m_eSharpnessTagPresence == TPT_PRESENT )
        usTagQuantity1++;
    if ( m_eDeviceSettingDescriptionTagPresence == TPT_PRESENT )
        usTagQuantity1++;
    if ( m_eSubjectDistanceRangeTagPresence == TPT_PRESENT )
        usTagQuantity1++;
    if ( m_eImageUniqueIdTagPresence == TPT_PRESENT )
        usTagQuantity1++;

    if ( usTagQuantity1 != 0 && eExifDataDestination != EDDT_TIFF )
        m_eExifIFDPointerTagPresence = TPT_PRESENT;

    if ( m_eImageWidthTagPresence == TPT_PRESENT )
        usTagQuantity0++;
    if ( m_eImageLengthTagPresence == TPT_PRESENT )
        usTagQuantity0++;
    if ( m_eBitsPerSampleTagPresence == TPT_PRESENT )
        usTagQuantity0++;
    if ( m_eCompressionTagPresence == TPT_PRESENT )
        usTagQuantity0++;
    if ( m_ePhotometricInterpretationTagPresence == TPT_PRESENT )
        usTagQuantity0++;
    if ( m_eOrientationTagPresence == TPT_PRESENT )
        usTagQuantity0++;
    if ( m_eSamplesPerPixelTagPresence == TPT_PRESENT )
        usTagQuantity0++;
    if ( m_ePlanarConfigurationTagPresence == TPT_PRESENT )
        usTagQuantity0++;
    if ( m_eYCbCrSubSamplingTagPresence == TPT_PRESENT )
        usTagQuantity0++;
    if ( m_eYCbCrPositioningTagPresence == TPT_PRESENT )
        usTagQuantity0++;
    if ( m_eXResolutionTagPresence == TPT_PRESENT )
        usTagQuantity0++;
    if ( m_eYResolutionTagPresence == TPT_PRESENT )
        usTagQuantity0++;
    if ( m_eResolutionUnitTagPresence == TPT_PRESENT )
        usTagQuantity0++;
    if ( m_eStripOffsetsTagPresence == TPT_PRESENT )
        usTagQuantity0++;
    if ( m_eRowsPerStripTagPresence == TPT_PRESENT )
        usTagQuantity0++;
    if ( m_eStripByteCountsTagPresence == TPT_PRESENT )
        usTagQuantity0++;
    if ( m_eJPEGInterchangeFormatTagPresence == TPT_PRESENT )
        usTagQuantity0++;
    if ( m_eJPEGInterchangeFormatLengthTagPresence == TPT_PRESENT )
        usTagQuantity0++;
    if ( m_eTransferFunctionTagPresence == TPT_PRESENT )
        usTagQuantity0++;
    if ( m_eWhitePointTagPresence == TPT_PRESENT )
        usTagQuantity0++;
    if ( m_ePrimaryChromaticitiesTagPresence == TPT_PRESENT )
        usTagQuantity0++;
    if ( m_eYCbCrCoefficientsTagPresence == TPT_PRESENT )
        usTagQuantity0++;
    if ( m_eReferenceBlackWhiteTagPresence == TPT_PRESENT )
        usTagQuantity0++;
    if ( m_eDateTimeTagPresence == TPT_PRESENT )
        usTagQuantity0++;
    if ( m_eImageDescriptionTagPresence == TPT_PRESENT )
        usTagQuantity0++;
    if ( m_eMakeTagPresence == TPT_PRESENT )
        usTagQuantity0++;
    if ( m_eModelTagPresence == TPT_PRESENT )
        usTagQuantity0++;
    if ( m_eSoftwareTagPresence == TPT_PRESENT )
        usTagQuantity0++;
    if ( m_eArtistTagPresence == TPT_PRESENT )
        usTagQuantity0++;
    if ( m_eCopyrightTagPresence == TPT_PRESENT )
        usTagQuantity0++;
    if ( m_eExifIFDPointerTagPresence == TPT_PRESENT )
        usTagQuantity0++;

    Stream qSaveStream(( m_eByteOrder == BOT_LITTLE_ENDIAN ));
    qSaveStream.SetDoNotFreeBufferAfterComplete();

    unsigned short usTagId, usType, usValue;
    int iTagByteQuantity, iBytePosF, iBytePosV, iCount, iLength, iOldBytePosV, iNumerator;
    bool bValueFitsFourBytes;
    unsigned char ucValue;
    char cZero = 0;

    if ( eExifDataDestination == EDDT_TIFF )
    {
        assert( usTagQuantity0 == 0 );
        goto ExifIFD;
    }

    // byte order
    unsigned short usByteOrder = ( m_eByteOrder == BOT_LITTLE_ENDIAN ? 18761 : 19789 );
    qSaveStream.Write(usByteOrder);

    // TIFF signature and offset to the 0th IFD
    unsigned short usSignature = 42;
    int iFirstIFDOffset = 8;
    qSaveStream.Write(usSignature);
    qSaveStream.Write(iFirstIFDOffset);

    // estimate room for tags which are to be followed by out-of-four-bytes values and reserve room for tags
    iTagByteQuantity = 6;
    if ( usTagQuantity0 != 0 )
        iTagByteQuantity += 12*usTagQuantity0;
    if ( usTagQuantity1 != 0 )
        iTagByteQuantity += 6 + 12*usTagQuantity1;
    iBytePosF = qSaveStream.GetBytePosition();
    for (int i = 0; i < iTagByteQuantity; i++)
        qSaveStream.Write(cZero);
    iBytePosV = qSaveStream.GetBytePosition();
    qSaveStream.SetBytePosition(iBytePosF);
    if ( usTagQuantity0 == 0 )
    {
        // no Exif, zeroes are on their places already
        qSaveStream.OffsetBytePosition(6);
        goto End;
    }

    // Save data.
    qSaveStream.Write(usTagQuantity0);

    // image width
    if ( m_eImageWidthTagPresence == TPT_PRESENT )
    {
        BEFORE_TAG_VALUE_WRITE(256,EXIFT_LONG,1);
        qSaveStream.Write(m_iImageWidth);
        AFTER_TAG_VALUE_WRITE;
    }

    // image height
    if ( m_eImageLengthTagPresence == TPT_PRESENT )
    {
        BEFORE_TAG_VALUE_WRITE(257,EXIFT_LONG,1);
        qSaveStream.Write(m_iImageLength);
        AFTER_TAG_VALUE_WRITE;
    }

    // number of bits per component
    if ( m_eBitsPerSampleTagPresence == TPT_PRESENT )
    {
        BEFORE_TAG_VALUE_WRITE(258,EXIFT_SHORT,3);
        WRITE_SHORT(m_iBitsPerSample0);
        WRITE_SHORT(m_iBitsPerSample1);
        WRITE_SHORT(m_iBitsPerSample2);
        AFTER_TAG_VALUE_WRITE;
    }

    // compression scheme
    if ( m_eCompressionTagPresence == TPT_PRESENT )
    {
        BEFORE_TAG_VALUE_WRITE(259,EXIFT_SHORT,1);
        switch ( m_eCompression )
        {
        case CST_UNCOMPRESSED:
            usValue = 1;
            break;
        case CST_JPEG_COMPRESSION:
            usValue = 6;
            break;
        }
        qSaveStream.Write(usValue);
        AFTER_TAG_VALUE_WRITE;
    }

    // pixel composition
    if ( m_ePhotometricInterpretationTagPresence == TPT_PRESENT )
    {
        BEFORE_TAG_VALUE_WRITE(262,EXIFT_SHORT,1);
        switch ( m_ePhotometricInterpretation )
        {
        case PIT_RGB:
            usValue = 2;
            break;
        case PIT_YCC:
            usValue = 6;
            break;
        }
        qSaveStream.Write(usValue);
        AFTER_TAG_VALUE_WRITE;
    }

    // image title
    if ( m_eImageDescriptionTagPresence == TPT_PRESENT )
    {
        BEFORE_TAG_VALUE_WRITE(270,EXIFT_ASCII,m_qImageDescription.GetMemLength());
        qSaveStream.Write(m_qImageDescription.GetMemLength(),m_qImageDescription);
        AFTER_TAG_VALUE_WRITE;
    }

    // image input equipment manufacturer
    if ( m_eMakeTagPresence == TPT_PRESENT )
    {
        BEFORE_TAG_VALUE_WRITE(271,EXIFT_ASCII,m_qMake.GetMemLength());
        qSaveStream.Write(m_qMake.GetMemLength(),m_qMake);
        AFTER_TAG_VALUE_WRITE;
    }

    // image input equipment model
    if ( m_eModelTagPresence == TPT_PRESENT )
    {
        BEFORE_TAG_VALUE_WRITE(272,EXIFT_ASCII,m_qModel.GetMemLength());
        qSaveStream.Write(m_qModel.GetMemLength(),m_qModel);
        AFTER_TAG_VALUE_WRITE;
    }

    // image data location
    if ( m_eStripOffsetsTagPresence == TPT_PRESENT )
    {
        BEFORE_TAG_VALUE_WRITE(273,EXIFT_LONG,m_qStripOffsets.GetQuantity());
        for (int i = 0; i < m_qStripOffsets.GetQuantity(); i++)
            qSaveStream.Write(m_qStripOffsets[i]);
        AFTER_TAG_VALUE_WRITE;
    }

    // orientation of the image
    if ( m_eOrientationTagPresence == TPT_PRESENT )
    {
        BEFORE_TAG_VALUE_WRITE(274,EXIFT_SHORT,1);
        switch ( m_eOrientation )
        {
        case OT_TOP_LEFT:
            usValue = 1;
            break;
        case OT_TOP_RIGHT:
            usValue = 2;
            break;
        case OT_BOTTOM_RIGHT:
            usValue = 3;
            break;
        case OT_BOTTOM_LEFT:
            usValue = 4;
            break;
        case OT_LEFT_TOP:
            usValue = 5;
            break;
        case OT_RIGHT_TOP:
            usValue = 6;
            break;
        case OT_RIGHT_BOTTOM:
            usValue = 7;
            break;
        case OT_LEFT_BOTTOM:
            usValue = 8;
            break;
        }
        qSaveStream.Write(usValue);
        AFTER_TAG_VALUE_WRITE;
    }

    // number of components
    if ( m_eSamplesPerPixelTagPresence == TPT_PRESENT )
    {
        BEFORE_TAG_VALUE_WRITE(277,EXIFT_SHORT,1);
        WRITE_SHORT(m_iSamplesPerPixel);
        AFTER_TAG_VALUE_WRITE;
    }

    // number of rows per strip
    if ( m_eRowsPerStripTagPresence == TPT_PRESENT )
    {
        BEFORE_TAG_VALUE_WRITE(278,EXIFT_LONG,1);
        qSaveStream.Write(m_iRowsPerStrip);
        AFTER_TAG_VALUE_WRITE;
    }

    // bytes per a compressed strip
    if ( m_eStripByteCountsTagPresence == TPT_PRESENT )
    {
        BEFORE_TAG_VALUE_WRITE(279,EXIFT_LONG,m_qStripByteCounts.GetQuantity());
        for (int i = 0; i < m_qStripByteCounts.GetQuantity(); i++)
            qSaveStream.Write(m_qStripByteCounts[i]);
        AFTER_TAG_VALUE_WRITE;
    }

    // image resolution in width-direction
    if ( m_eXResolutionTagPresence == TPT_PRESENT )
    {
        BEFORE_TAG_VALUE_WRITE(282,EXIFT_RATIONAL,1);
        WRITE_RATIONAL(m_fXResolution);
        AFTER_TAG_VALUE_WRITE;
    }

    // image resolution in height-direction
    if ( m_eYResolutionTagPresence == TPT_PRESENT )
    {
        BEFORE_TAG_VALUE_WRITE(283,EXIFT_RATIONAL,1);
        WRITE_RATIONAL(m_fYResolution);
        AFTER_TAG_VALUE_WRITE;
    }

    // image data arrangement
    if ( m_ePlanarConfigurationTagPresence == TPT_PRESENT )
    {
        BEFORE_TAG_VALUE_WRITE(284,EXIFT_SHORT,1);
        switch ( m_ePlanarConfiguration )
        {
        case PCT_CHUNKY_FORMAT:
            usValue = 1;
            break;
        case PCT_PLANAR_FORMAT:
            usValue = 2;
            break;
        }
        qSaveStream.Write(usValue);
        AFTER_TAG_VALUE_WRITE;
    }

    // unit of X and Y resolution
    if ( m_eResolutionUnitTagPresence == TPT_PRESENT )
    {
        BEFORE_TAG_VALUE_WRITE(296,EXIFT_SHORT,1);
        switch ( m_eResolutionUnit )
        {
        case RUT_INCHES:
            usValue = 2;
            break;
        case RUT_CENTIMETERS:
            usValue = 3;
            break;
        }
        qSaveStream.Write(usValue);
        AFTER_TAG_VALUE_WRITE;
    }

    // transfer function
    if ( m_eTransferFunctionTagPresence == TPT_PRESENT )
    {
        BEFORE_TAG_VALUE_WRITE(301,EXIFT_SHORT,768);
        for (int i = 0; i < 768; i++)
            WRITE_SHORT(m_qTransferFunction[i]);
        AFTER_TAG_VALUE_WRITE;
    }

    // software used
    if ( m_eSoftwareTagPresence == TPT_PRESENT )
    {
        BEFORE_TAG_VALUE_WRITE(305,EXIFT_ASCII,m_qSoftware.GetMemLength());
        qSaveStream.Write(m_qSoftware.GetMemLength(),m_qSoftware);
        AFTER_TAG_VALUE_WRITE;
    }

    // file change date and time
    if ( m_eDateTimeTagPresence == TPT_PRESENT )
    {
        BEFORE_TAG_VALUE_WRITE(306,EXIFT_ASCII,20);
        String qStrDateTime;
        m_qDateTime.ToString(qStrDateTime);
        qSaveStream.Write(20,qStrDateTime);
        AFTER_TAG_VALUE_WRITE;
    }

    // person who created the image
    if ( m_eArtistTagPresence == TPT_PRESENT )
    {
        BEFORE_TAG_VALUE_WRITE(315,EXIFT_ASCII,m_qArtist.GetMemLength());
        qSaveStream.Write(m_qArtist.GetMemLength(),m_qArtist);
        AFTER_TAG_VALUE_WRITE;
    }

    // white point chromaticity (x, y)
    if ( m_eWhitePointTagPresence == TPT_PRESENT )
    {
        BEFORE_TAG_VALUE_WRITE(318,EXIFT_RATIONAL,2);
        WRITE_RATIONAL(m_fWhitePointX);
        WRITE_RATIONAL(m_fWhitePointY);
        AFTER_TAG_VALUE_WRITE;
    }

    // chromaticities of primaries
    if ( m_ePrimaryChromaticitiesTagPresence == TPT_PRESENT )
    {
        BEFORE_TAG_VALUE_WRITE(319,EXIFT_RATIONAL,6);
        WRITE_RATIONAL(m_fPrimaryChromaticityRedX);
        WRITE_RATIONAL(m_fPrimaryChromaticityRedY);
        WRITE_RATIONAL(m_fPrimaryChromaticityGreenX);
        WRITE_RATIONAL(m_fPrimaryChromaticityGreenY);
        WRITE_RATIONAL(m_fPrimaryChromaticityBlueX);
        WRITE_RATIONAL(m_fPrimaryChromaticityBlueY);
        AFTER_TAG_VALUE_WRITE;
    }

    // offset to JPEG SOI
    if ( m_eJPEGInterchangeFormatTagPresence == TPT_PRESENT )
    {
        BEFORE_TAG_VALUE_WRITE(513,EXIFT_LONG,1);
        qSaveStream.Write(m_iJPEGInterchangeFormat);
        AFTER_TAG_VALUE_WRITE;
    }

    // bytes of JPEG data
    if ( m_eJPEGInterchangeFormatLengthTagPresence == TPT_PRESENT )
    {
        BEFORE_TAG_VALUE_WRITE(514,EXIFT_LONG,1);
        qSaveStream.Write(m_iJPEGInterchangeFormatLength);
        AFTER_TAG_VALUE_WRITE;
    }

    // color space transformation matrix coefficients
    if ( m_eYCbCrCoefficientsTagPresence == TPT_PRESENT )
    {
        BEFORE_TAG_VALUE_WRITE(529,EXIFT_RATIONAL,3);
        WRITE_RATIONAL(m_fCoefficientY);
        WRITE_RATIONAL(m_fCoefficientCb);
        WRITE_RATIONAL(m_fCoefficientCr);
        AFTER_TAG_VALUE_WRITE;
    }

    // subsampling ratio of Y (luminance) to C (chrominance)
    if ( m_eYCbCrSubSamplingTagPresence == TPT_PRESENT )
    {
        BEFORE_TAG_VALUE_WRITE(530,EXIFT_SHORT,2);
        switch ( m_eYCbCrSubSampling )
        {
        case YCCSST_4_2_2:
            WRITE_SHORT(2);
            WRITE_SHORT(1);
            break;
        case YCCSST_4_2_0:
            WRITE_SHORT(2);
            WRITE_SHORT(2);
            break;
        }
        AFTER_TAG_VALUE_WRITE;
    }

    // Y (luminance) and C (chrominance) positioning
    if ( m_eYCbCrPositioningTagPresence == TPT_PRESENT )
    {
        BEFORE_TAG_VALUE_WRITE(531,EXIFT_SHORT,1);
        switch ( m_eYCbCrPositioning )
        {
        case YCCPT_CENTERED:
            usValue = 1;
            break;
        case YCCPT_CO_SITED:
            usValue = 2;
            break;
        }
        qSaveStream.Write(usValue);
        AFTER_TAG_VALUE_WRITE;
    }

    // pair of black and white reference values
    if ( m_eReferenceBlackWhiteTagPresence == TPT_PRESENT )
    {
        BEFORE_TAG_VALUE_WRITE(532,EXIFT_RATIONAL,6);
        WRITE_RATIONAL(m_qReferenceBlackWhiteComponent0.First);
        WRITE_RATIONAL(m_qReferenceBlackWhiteComponent0.Second);
        WRITE_RATIONAL(m_qReferenceBlackWhiteComponent1.First);
        WRITE_RATIONAL(m_qReferenceBlackWhiteComponent1.Second);
        WRITE_RATIONAL(m_qReferenceBlackWhiteComponent2.First);
        WRITE_RATIONAL(m_qReferenceBlackWhiteComponent2.Second);
        AFTER_TAG_VALUE_WRITE;
    }

    // copyright holder
    if ( m_eCopyrightTagPresence == TPT_PRESENT )
    {
        String qCopyright0 = m_qPhotographerCopyright;
        String qCopyright1 = m_qEditorCopyright;
        if ( qCopyright0.IsEmpty() && !qCopyright1.IsEmpty() )
            qCopyright0 = " ";
        Array<char> qGCopyright;
        for (int i = 0; i < qCopyright0.GetMemLength(); i++)
            qGCopyright.Push(qCopyright0.GetChars()[i]);
        if ( !qCopyright1.IsEmpty() )
        {
            for (int i = 0; i < qCopyright1.GetMemLength(); i++)
                qGCopyright.Push(qCopyright1.GetChars()[i]);
        }

        BEFORE_TAG_VALUE_WRITE(33432,EXIFT_ASCII,qGCopyright.GetQuantity());
        qSaveStream.Write(qGCopyright.GetQuantity(),qGCopyright);
        AFTER_TAG_VALUE_WRITE;
    }

    // pointer to the Exif IFD
    if ( m_eExifIFDPointerTagPresence == TPT_PRESENT )
    {
        BEFORE_TAG_VALUE_WRITE(34665,EXIFT_LONG,1);
        int iExifIFDPointer = qSaveStream.GetBytePosition() + 8;
        qSaveStream.Write(iExifIFDPointer);
        AFTER_TAG_VALUE_WRITE;
    }

    qSaveStream.OffsetBytePosition(4);

    if ( usTagQuantity1 == 0 )
        goto End;

ExifIFD:
    if ( eExifDataDestination == EDDT_TIFF )
    {
        // estimate room for tags which are to be followed by out-of-four-bytes values and reserve room for
        // tags
        iTagByteQuantity = 6;
        if ( usTagQuantity1 != 0 )
            iTagByteQuantity += 12*usTagQuantity1;
        iBytePosF = qSaveStream.GetBytePosition();
        for (int i = 0; i < iTagByteQuantity; i++)
            qSaveStream.Write(cZero);
        iBytePosV = qSaveStream.GetBytePosition();
        qSaveStream.SetBytePosition(iBytePosF);
        if ( usTagQuantity1 == 0 )
        {
            // no Exif IFD, zeroes are on their places already
            qSaveStream.OffsetBytePosition(6);
            goto End;
        }
    }

    qSaveStream.Write(usTagQuantity1);

    // exposure time
    if ( m_eExposureTimeTagPresence == TPT_PRESENT )
    {
        BEFORE_TAG_VALUE_WRITE(33434,EXIFT_RATIONAL,1);
        WRITE_RATIONAL(m_fExposureTime);
        AFTER_TAG_VALUE_WRITE;
    }

    // F number
    if ( m_eFNumberTagPresence == TPT_PRESENT )
    {
        BEFORE_TAG_VALUE_WRITE(33437,EXIFT_RATIONAL,1);
        WRITE_RATIONAL(m_fFNumber);
        AFTER_TAG_VALUE_WRITE;
    }

    // exposure program
    if ( m_eExposureProgramTagPresence == TPT_PRESENT )
    {
        BEFORE_TAG_VALUE_WRITE(34850,EXIFT_SHORT,1);
        switch ( m_eExposureProgram )
        {
        case EPT_NOT_DEFINED:
            usValue = 0;
            break;
        case EPT_MANUAL:
            usValue = 1;
            break;
        case EPT_NORMAL_PROGRAM:
            usValue = 2;
            break;
        case EPT_APERTURE_PRIORITY:
            usValue = 3;
            break;
        case EPT_SHUTTER_PRIORITY:
            usValue = 4;
            break;
        case EPT_CREATIVE_PROGRAM:
            usValue = 5;
            break;
        case EPT_ACTION_PROGRAM:
            usValue = 6;
            break;
        case EPT_PORTRAIT_MODE:
            usValue = 7;
            break;
        case EPT_LANDSCAPE_MODE:
            usValue = 8;
            break;
        }
        qSaveStream.Write(usValue);
        AFTER_TAG_VALUE_WRITE;
    }

    // spectral sensitivity
    if ( m_eSpectralSensitivityTagPresence == TPT_PRESENT )
    {
        BEFORE_TAG_VALUE_WRITE(34852,EXIFT_ASCII,m_qSpectralSensitivity.GetMemLength());
        qSaveStream.Write(m_qSpectralSensitivity.GetMemLength(),m_qSpectralSensitivity);
        AFTER_TAG_VALUE_WRITE;
    }

    // ISO speed rating
    if ( m_eISOSpeedRatingsTagPresence == TPT_PRESENT )
    {
        BEFORE_TAG_VALUE_WRITE(34855,EXIFT_SHORT,m_qISOSpeedRatings.GetQuantity());
        for (int i = 0; i < m_qISOSpeedRatings.GetQuantity(); i++)
            WRITE_SHORT(m_qISOSpeedRatings[i]);
        AFTER_TAG_VALUE_WRITE;
    }

    // optoelectric conversion factor
    if ( m_eOECFTagPresence == TPT_PRESENT )
    {
        BEFORE_TAG_VALUE_WRITE(34856,EXIFT_UNDEFINED,m_qOECF.GetQuantity());
        qSaveStream.Write(m_qOECF.GetQuantity(),m_qOECF);
        AFTER_TAG_VALUE_WRITE;
    }

    // Exif version
    if ( m_eExifVersionTagPresence == TPT_PRESENT )
    {
        BEFORE_TAG_VALUE_WRITE(36864,EXIFT_UNDEFINED,4);
        char acVersion[4];
        switch ( m_eExifVersion )
        {
        case EXIFVT_2_0:
            acVersion[0] = '0';
            acVersion[1] = '2';
            acVersion[2] = '0';
            acVersion[3] = '0';
            break;
        case EXIFVT_2_1:
            acVersion[0] = '0';
            acVersion[1] = '2';
            acVersion[2] = '1';
            acVersion[3] = '0';
            break;
        case EXIFVT_2_2:
            acVersion[0] = '0';
            acVersion[1] = '2';
            acVersion[2] = '2';
            acVersion[3] = '0';
            break;
        }
        qSaveStream.Write(4,acVersion);
        AFTER_TAG_VALUE_WRITE;
    }

    // date and time of original data generation
    if ( m_eDateTimeOriginalTagPresence == TPT_PRESENT )
    {
        BEFORE_TAG_VALUE_WRITE(36867,EXIFT_ASCII,20);
        String qStrDateTimeOriginal;
        m_qDateTimeOriginal.ToString(qStrDateTimeOriginal);
        qSaveStream.Write(20,qStrDateTimeOriginal);
        AFTER_TAG_VALUE_WRITE;
    }

    // date and time of digital data generation
    if ( m_eDateTimeDigitizedTagPresence == TPT_PRESENT )
    {
        BEFORE_TAG_VALUE_WRITE(36868,EXIFT_ASCII,20);
        String qStrDateTimeDigitized;
        m_qDateTimeDigitized.ToString(qStrDateTimeDigitized);
        qSaveStream.Write(20,qStrDateTimeDigitized);
        AFTER_TAG_VALUE_WRITE;
    }

    // meaning of each component
    if ( m_eComponentsConfigurationTagPresence == TPT_PRESENT )
    {
        BEFORE_TAG_VALUE_WRITE(37121,EXIFT_UNDEFINED,4);
        char acMeaning[4];
        switch ( m_eComponentsConfiguration )
        {
        case CCT_RGB:
            acMeaning[0] = 4;
            acMeaning[1] = 5;
            acMeaning[2] = 6;
            acMeaning[3] = 0;
            break;
        case CCT_YCC:
            acMeaning[0] = 1;
            acMeaning[1] = 2;
            acMeaning[2] = 3;
            acMeaning[3] = 0;
            break;
        }
        qSaveStream.Write(4,acMeaning);
        AFTER_TAG_VALUE_WRITE;
    }

    // image compression mode
    if ( m_eCompressedBitsPerPixelTagPresence == TPT_PRESENT )
    {
        BEFORE_TAG_VALUE_WRITE(37122,EXIFT_RATIONAL,1);
        WRITE_RATIONAL(m_fCompressedBitsPerPixel);
        AFTER_TAG_VALUE_WRITE;
    }

    // shutter speed
    if ( m_eShutterSpeedValueTagPresence == TPT_PRESENT )
    {
        BEFORE_TAG_VALUE_WRITE(37377,EXIFT_SRATIONAL,1);
        WRITE_RATIONAL(m_fShutterSpeedValue);
        AFTER_TAG_VALUE_WRITE;
    }

    // aperture
    if ( m_eApertureValueTagPresence == TPT_PRESENT )
    {
        BEFORE_TAG_VALUE_WRITE(37378,EXIFT_RATIONAL,1);
        WRITE_RATIONAL(m_fApertureValue);
        AFTER_TAG_VALUE_WRITE;
    }

    // brightness
    if ( m_eBrightnessValueTagPresence == TPT_PRESENT )
    {
        BEFORE_TAG_VALUE_WRITE(37379,EXIFT_SRATIONAL,1);
        if ( m_eBrightnessValueState == BVST_KNOWN )
            iNumerator = Mathf::RoundToInt(m_fBrightnessValue*ms_iRationalNumbersScaler);
        else  // m_eBrightnessValueState = BVST_UNKNOWN
            iNumerator = 4294967295;
        qSaveStream.Write(iNumerator);
        qSaveStream.Write(ms_iRationalNumbersScaler);
        AFTER_TAG_VALUE_WRITE;
    }

    // exposure bias
    if ( m_eExposureBiasValueTagPresence == TPT_PRESENT )
    {
        BEFORE_TAG_VALUE_WRITE(37380,EXIFT_SRATIONAL,1);
        WRITE_RATIONAL(m_fExposureBiasValue);
        AFTER_TAG_VALUE_WRITE;
    }

    // maximum lens aperture
    if ( m_eMaxApertureValueTagPresence == TPT_PRESENT )
    {
        BEFORE_TAG_VALUE_WRITE(37381,EXIFT_RATIONAL,1);
        WRITE_RATIONAL(m_fMaxApertureValue);
        AFTER_TAG_VALUE_WRITE;
    }

    // subject distance
    if ( m_eSubjectDistanceTagPresence == TPT_PRESENT )
    {
        BEFORE_TAG_VALUE_WRITE(37382,EXIFT_RATIONAL,1);
        if ( m_eSubjectDistanceState == SDST_KNOWN )
            iNumerator = Mathf::RoundToInt(m_fSubjectDistance*ms_iRationalNumbersScaler);
        else if ( m_eSubjectDistanceState == SDST_INFINITY )
            iNumerator = 4294967295;
        else  // m_eSubjectDistanceState = SDST_UNKNOWN
            iNumerator = 0;
        qSaveStream.Write(iNumerator);
        qSaveStream.Write(ms_iRationalNumbersScaler);
        AFTER_TAG_VALUE_WRITE;
    }

    // metering mode
    if ( m_eMeteringModeTagPresence == TPT_PRESENT )
    {
        BEFORE_TAG_VALUE_WRITE(37383,EXIFT_SHORT,1);
        switch ( m_eMeteringMode )
        {
        case MMT_UNKNOWN:
            usValue = 0;
            break;
        case MMT_AVERAGE:
            usValue = 1;
            break;
        case MMT_CENTER_WEIGHTED_AVERAGE:
            usValue = 2;
            break;
        case MMT_SPOT:
            usValue = 3;
            break;
        case MMT_MULTI_SPOT:
            usValue = 4;
            break;
        case MMT_PATTERN:
            usValue = 5;
            break;
        case MMT_PARTIAL:
            usValue = 6;
            break;
        case MMT_OTHER:
            usValue = 255;
            break;
        }
        qSaveStream.Write(usValue);
        AFTER_TAG_VALUE_WRITE;
    }

    // light source
    if ( m_eLightSourceTagPresence == TPT_PRESENT )
    {
        BEFORE_TAG_VALUE_WRITE(37384,EXIFT_SHORT,1);
        switch ( m_eLightSource )
        {
        case LST_UNKNOWN:
            usValue = 0;
            break;
        case LST_DAYLIGHT:
            usValue = 1;
            break;
        case LST_FLUORESCENT:
            usValue = 2;
            break;
        case LST_TUNGSTEN:
            usValue = 3;
            break;
        case LST_FLASH:
            usValue = 4;
            break;
        case LST_FINE_WEATHER:
            usValue = 9;
            break;
        case LST_CLOUDY_WEATHER:
            usValue = 10;
            break;
        case LST_SHADE:
            usValue = 11;
            break;
        case LST_DAYLIGHT_FLUORESCENT:
            usValue = 12;
            break;
        case LST_DAY_WHITE_FLUORESCENT:
            usValue = 13;
            break;
        case LST_COOL_WHITE_FLUORESCENT:
            usValue = 14;
            break;
        case LST_WHITE_FLUORESCENT:
            usValue = 15;
            break;
        case LST_STANDARD_LIGHT_A:
            usValue = 17;
            break;
        case LST_STANDARD_LIGHT_B:
            usValue = 18;
            break;
        case LST_STANDARD_LIGHT_C:
            usValue = 19;
            break;
        case LST_D55:
            usValue = 20;
            break;
        case LST_D65:
            usValue = 21;
            break;
        case LST_D75:
            usValue = 22;
            break;
        case LST_D50:
            usValue = 23;
            break;
        case LST_ISO_STUDIO_TUNGSTEN:
            usValue = 24;
            break;
        case LST_OTHER:
            usValue = 255;
            break;
        }
        qSaveStream.Write(usValue);
        AFTER_TAG_VALUE_WRITE;
    }

    // flash
    if ( m_eFlashTagPresence == TPT_PRESENT )
    {
        BEFORE_TAG_VALUE_WRITE(37385,EXIFT_SHORT,1);
        switch ( m_eFlash )
        {
        case FT_FLASH_DID_NOT_FIRE:
            usValue = 0;
            break;
        case FT_FLASH_FIRED:
            usValue = 1;
            break;
        case FT_STROBE_RETURN_LIGHT_NOT_DETECTED:
            usValue = 5;
            break;
        case FT_STROBE_RETURN_LIGHT_DETECTED:
            usValue = 7;
            break;
        case FT_FLASH_FIRED__COMPULSORY_FLASH_MODE:
            usValue = 9;
            break;
        case FT_FLASH_FIRED__COMPULSORY_FLASH_MODE__RETURN_LIGHT_NOT_DETECTED:
            usValue = 13;
            break;
        case FT_FLASH_FIRED__COMPULSORY_FLASH_MODE__RETURN_LIGHT_DETECTED:
            usValue = 15;
            break;
        case FT_FLASH_DID_NOT_FIRE__COMPULSORY_FLASH_MODE:
            usValue = 16;
            break;
        case FT_FLASH_DID_NOT_FIRE__AUTO_MODE:
            usValue = 24;
            break;
        case FT_FLASH_FIRED__AUTO_MODE:
            usValue = 25;
            break;
        case FT_FLASH_FIRED__AUTO_MODE__RETURN_LIGHT_NOT_DETECTED:
            usValue = 29;
            break;
        case FT_FLASH_FIRED__AUTO_MODE__RETURN_LIGHT_DETECTED:
            usValue = 31;
            break;
        case FT_NO_FLASH_FUNCTION:
            usValue = 32;
            break;
        case FT_FLASH_FIRED__REDEYE_REDUCTION_MODE:
            usValue = 65;
            break;
        case FT_FLASH_FIRED__REDEYE_REDUCTION_MODE__RETURN_LIGHT_NOT_DETECTED:
            usValue = 69;
            break;
        case FT_FLASH_FIRED__REDEYE_REDUCTION_MODE__RETURN_LIGHT_DETECTED:
            usValue = 71;
            break;
        case FT_FLASH_FIRED__COMPULSORY_FLASH_MODE__REDEYE_REDUCTION_MODE:
            usValue = 73;
            break;
        case FT_FLASH_FIRED__COMPULSORY_FLASH_MODE__REDEYE_REDUCTION_MODE__RETURN_LIGHT_NOT_DETECTED:
            usValue = 77;
            break;
        case FT_FLASH_FIRED__COMPULSORY_FLASH_MODE__REDEYE_REDUCTION_MODE__RETURN_LIGHT_DETECTED:
            usValue = 79;
            break;
        case FT_FLASH_FIRED__AUTO_MODE__REDEYE_REDUCTION_MODE:
            usValue = 89;
            break;
        case FT_FLASH_FIRED__AUTO_MODE__RETURN_LIGHT_NOT_DETECTED__REDEYE_REDUCTION_MODE:
            usValue = 93;
            break;
        case FT_FLASH_FIRED__AUTO_MODE__RETURN_LIGHT_DETECTED__REDEYE_REDUCTION_MODE:
            usValue = 95;
            break;
        }
        qSaveStream.Write(usValue);
        AFTER_TAG_VALUE_WRITE;
    }

    // lens focal length
    if ( m_eFocalLengthTagPresence == TPT_PRESENT )
    {
        BEFORE_TAG_VALUE_WRITE(37386,EXIFT_RATIONAL,1);
        WRITE_RATIONAL(m_fFocalLength);
        AFTER_TAG_VALUE_WRITE;
    }

    // subject area
    if ( m_eSubjectAreaTagPresence == TPT_PRESENT )
    {
        BEFORE_TAG_VALUE_WRITE(37396,EXIFT_SHORT,m_qSubjectAreaCoordinates.GetQuantity());
        for (int i = 0; i < m_qSubjectAreaCoordinates.GetQuantity(); i++)
            WRITE_SHORT(m_qSubjectAreaCoordinates[i]);
        AFTER_TAG_VALUE_WRITE;
    }

    // manufacturer notes
    if ( m_eMakerNoteTagPresence == TPT_PRESENT )
    {
        BEFORE_TAG_VALUE_WRITE(37500,EXIFT_UNDEFINED,m_qMakerNote.GetQuantity());
        qSaveStream.Write(m_qMakerNote.GetQuantity(),m_qMakerNote);
        AFTER_TAG_VALUE_WRITE;
    }

    // user comments
    if ( m_eUserCommentTagPresence == TPT_PRESENT )
    {
        BEFORE_TAG_VALUE_WRITE(37510,EXIFT_UNDEFINED,(8+m_qUserComment.GetQuantity()));
        switch ( m_eUserCommentCharacterCode )
        {
        case UCCCT_ASCII:
            qSaveStream.Write(8,gs_acCharacterCodeASCII);
            break;
        case UCCCT_JIS:
            qSaveStream.Write(8,gs_acCharacterCodeJIS);
            break;
        case UCCCT_UNICODE:
            qSaveStream.Write(8,gs_acCharacterCodeUnicode);
            break;
        case UCCCT_UNDEFINED:
            qSaveStream.Write(8,gs_acCharacterCodeUndefined);
            break;
        }
        qSaveStream.Write(m_qUserComment.GetQuantity(),m_qUserComment);
        AFTER_TAG_VALUE_WRITE;
    }

    // DateTime subseconds
    if ( m_eSubsecTimeTagPresence == TPT_PRESENT )
    {
        BEFORE_TAG_VALUE_WRITE(37520,EXIFT_ASCII,m_qSubsecTime.GetMemLength());
        qSaveStream.Write(m_qSubsecTime.GetMemLength(),m_qSubsecTime);
        AFTER_TAG_VALUE_WRITE;
    }

    // DateTimeOriginal subseconds
    if ( m_eSubsecTimeOriginalTagPresence == TPT_PRESENT )
    {
        BEFORE_TAG_VALUE_WRITE(37521,EXIFT_ASCII,m_qSubsecTimeOriginal.GetMemLength());
        qSaveStream.Write(m_qSubsecTimeOriginal.GetMemLength(),m_qSubsecTimeOriginal);
        AFTER_TAG_VALUE_WRITE;
    }

    // DateTimeDigitized subseconds
    if ( m_eSubsecTimeDigitizedTagPresence == TPT_PRESENT )
    {
        BEFORE_TAG_VALUE_WRITE(37522,EXIFT_ASCII,m_qSubsecTimeDigitized.GetMemLength());
        qSaveStream.Write(m_qSubsecTimeDigitized.GetMemLength(),m_qSubsecTimeDigitized);
        AFTER_TAG_VALUE_WRITE;
    }

    // supported Flashpix version
    if ( m_eFlashpixVersionTagPresence == TPT_PRESENT )
    {
        BEFORE_TAG_VALUE_WRITE(40960,EXIFT_UNDEFINED,4);
        char acVersion[4];
        switch ( m_eFlashpixVersion )
        {
        case FVT_1_0:
            acVersion[0] = '0';
            acVersion[1] = '1';
            acVersion[2] = '0';
            acVersion[3] = '0';
            break;
        }
        qSaveStream.Write(4,acVersion);
        AFTER_TAG_VALUE_WRITE;
    }

    // color space information
    if ( m_eColorSpaceTagPresence == TPT_PRESENT )
    {
        BEFORE_TAG_VALUE_WRITE(40961,EXIFT_SHORT,1);
        switch ( m_eColorSpace )
        {
        case CSTT_sRGB:
            usValue = 1;
            break;
        case CSTT_UNCALIBRATED:
            usValue = 65535;
            break;
        }
        qSaveStream.Write(usValue);
        AFTER_TAG_VALUE_WRITE;
    }

    // valid image width
    if ( m_ePixelXDimensionTagPresence == TPT_PRESENT )
    {
        BEFORE_TAG_VALUE_WRITE(40962,EXIFT_LONG,1);
        qSaveStream.Write(m_iPixelXDimension);
        AFTER_TAG_VALUE_WRITE;
    }

    // valid image height
    if ( m_ePixelYDimensionTagPresence == TPT_PRESENT )
    {
        BEFORE_TAG_VALUE_WRITE(40963,EXIFT_LONG,1);
        qSaveStream.Write(m_iPixelYDimension);
        AFTER_TAG_VALUE_WRITE;
    }

    // related audio file
    if ( m_eRelatedSoundFileTagPresence == TPT_PRESENT )
    {
        BEFORE_TAG_VALUE_WRITE(40964,EXIFT_ASCII,13);
        qSaveStream.Write(13,m_qRelatedSoundFile);
        AFTER_TAG_VALUE_WRITE;
    }

    // flash energy
    if ( m_eFlashEnergyTagPresence == TPT_PRESENT )
    {
        BEFORE_TAG_VALUE_WRITE(41483,EXIFT_RATIONAL,1);
        WRITE_RATIONAL(m_fFlashEnergy);
        AFTER_TAG_VALUE_WRITE;
    }

    // spatial frequency response
    if ( m_eSpatialFrequencyResponseTagPresence == TPT_PRESENT )
    {
        BEFORE_TAG_VALUE_WRITE(41484,EXIFT_UNDEFINED,m_qSpatialFrequencyResponse.GetQuantity());
        qSaveStream.Write(m_qSpatialFrequencyResponse.GetQuantity(),m_qSpatialFrequencyResponse);
        AFTER_TAG_VALUE_WRITE;
    }

    // focal plane X resolution
    if ( m_eFocalPlaneXResolutionTagPresence == TPT_PRESENT )
    {
        BEFORE_TAG_VALUE_WRITE(41486,EXIFT_RATIONAL,1);
        WRITE_RATIONAL(m_fFocalPlaneXResolution);
        AFTER_TAG_VALUE_WRITE;
    }

    // focal plane Y resolution
    if ( m_eFocalPlaneYResolutionTagPresence == TPT_PRESENT )
    {
        BEFORE_TAG_VALUE_WRITE(41487,EXIFT_RATIONAL,1);
        WRITE_RATIONAL(m_fFocalPlaneYResolution);
        AFTER_TAG_VALUE_WRITE;
    }

    // focal plane resolution unit
    if ( m_eFocalPlaneResolutionUnitTagPresence == TPT_PRESENT )
    {
        BEFORE_TAG_VALUE_WRITE(41488,EXIFT_SHORT,1);
        switch ( m_eFocalPlaneResolutionUnit )
        {
        case RUT_INCHES:
            usValue = 2;
            break;
        case RUT_CENTIMETERS:
            usValue = 3;
            break;
        }
        qSaveStream.Write(usValue);
        AFTER_TAG_VALUE_WRITE;
    }

    // subject location
    if ( m_eSubjectLocationTagPresence == TPT_PRESENT )
    {
        BEFORE_TAG_VALUE_WRITE(41492,EXIFT_SHORT,2);
        WRITE_SHORT(m_iSubjectLocationX);
        WRITE_SHORT(m_iSubjectLocationY);
        AFTER_TAG_VALUE_WRITE;
    }

    // exposure index
    if ( m_eExposureIndexTagPresence == TPT_PRESENT )
    {
        BEFORE_TAG_VALUE_WRITE(41493,EXIFT_RATIONAL,1);
        WRITE_RATIONAL(m_fExposureIndex);
        AFTER_TAG_VALUE_WRITE;
    }

    // sensing method
    if ( m_eSensingMethodTagPresence == TPT_PRESENT )
    {
        BEFORE_TAG_VALUE_WRITE(41495,EXIFT_SHORT,1);
        switch ( m_eSensingMethod )
        {
        case SMT_NOT_DEFINED:
            usValue = 1;
            break;
        case SMT_ONECHIP_COLOR_AREA_SENSOR:
            usValue = 2;
            break;
        case SMT_TWOCHIP_COLOR_AREA_SENSOR:
            usValue = 3;
            break;
        case SMT_THREECHIP_COLOR_AREA_SENSOR:
            usValue = 4;
            break;
        case SMT_COLOR_SEQUENTIAL_AREA_SENSOR:
            usValue = 5;
            break;
        case SMT_TRILINEAR_SENSOR:
            usValue = 7;
            break;
        case SMT_COLOR_SEQUENTIAL_LINEAR_SENSOR:
            usValue = 8;
            break;
        }
        qSaveStream.Write(usValue);
        AFTER_TAG_VALUE_WRITE;
    }

    // file source
    if ( m_eFileSourceTagPresence == TPT_PRESENT )
    {
        BEFORE_TAG_VALUE_WRITE(41728,EXIFT_UNDEFINED,1);
        switch ( m_eFileSource )
        {
        case FST_DSC:
            ucValue = 3;
            break;
        }
        qSaveStream.Write(ucValue);
        AFTER_TAG_VALUE_WRITE;
    }

    // scene type
    if ( m_eSceneTypeTagPresence == TPT_PRESENT )
    {
        BEFORE_TAG_VALUE_WRITE(41729,EXIFT_UNDEFINED,1);
        switch ( m_eSceneType )
        {
        case ST_A_DIRECTLY_PHOTOGRAPHED_IMAGE:
            ucValue = 1;
            break;
        }
        qSaveStream.Write(ucValue);
        AFTER_TAG_VALUE_WRITE;
    }

    // CFA pattern
    if ( m_eCFAPatternTagPresence == TPT_PRESENT )
    {
        BEFORE_TAG_VALUE_WRITE(41730,EXIFT_UNDEFINED,m_qCFAPattern.GetQuantity());
        qSaveStream.Write(m_qCFAPattern.GetQuantity(),m_qCFAPattern);
        AFTER_TAG_VALUE_WRITE;
    }

    // custom image processing
    if ( m_eCustomRenderedTagPresence == TPT_PRESENT )
    {
        BEFORE_TAG_VALUE_WRITE(41985,EXIFT_SHORT,1);
        switch ( m_eCustomRendered )
        {
        case CRT_NORMAL_PROCESS:
            usValue = 0;
            break;
        case CRT_CUSTOM_PROCESS:
            usValue = 1;
            break;
        }
        qSaveStream.Write(usValue);
        AFTER_TAG_VALUE_WRITE;
    }

    // exposure mode
    if ( m_eExposureModeTagPresence == TPT_PRESENT )
    {
        BEFORE_TAG_VALUE_WRITE(41986,EXIFT_SHORT,1);
        switch ( m_eExposureMode )
        {
        case EMT_AUTO_EXPOSURE:
            usValue = 0;
            break;
        case EMT_MANUAL_EXPOSURE:
            usValue = 1;
            break;
        case EMT_AUTO_BRACKET:
            usValue = 2;
            break;
        }
        qSaveStream.Write(usValue);
        AFTER_TAG_VALUE_WRITE;
    }

    // white balance
    if ( m_eWhiteBalanceTagPresence == TPT_PRESENT )
    {
        BEFORE_TAG_VALUE_WRITE(41987,EXIFT_SHORT,1);
        switch ( m_eWhiteBalance )
        {
        case WBT_AUTO_WHITE_BALANCE:
            usValue = 0;
            break;
        case WBT_MANUAL_WHITE_BALANCE:
            usValue = 1;
            break;
        }
        qSaveStream.Write(usValue);
        AFTER_TAG_VALUE_WRITE;
    }

    // digital zoom ratio
    if ( m_eDigitalZoomRatioTagPresence == TPT_PRESENT )
    {
        BEFORE_TAG_VALUE_WRITE(41988,EXIFT_RATIONAL,1);
        if ( m_eDigitalZoomState == DZST_USED )
            iNumerator = Mathf::RoundToInt(m_fDigitalZoomRatio*ms_iRationalNumbersScaler);
        else  // m_eDigitalZoomState = DZST_NOT_USED
            iNumerator = 0;
        qSaveStream.Write(iNumerator);
        qSaveStream.Write(ms_iRationalNumbersScaler);
        AFTER_TAG_VALUE_WRITE;
    }

    // focal length in 35 mm film
    if ( m_eFocalLengthIn35mmFilmTagPresence == TPT_PRESENT )
    {
        BEFORE_TAG_VALUE_WRITE(41989,EXIFT_SHORT,1);
        if ( m_eFocalLengthIn35mmFilmState == FLI35MMFST_KNOWN )
            usValue = m_iFocalLengthIn35mmFilm;
        else  // m_eFocalLengthIn35mmFilmState = FLI35MMFST_UNKNOWN
            usValue = 0;
        qSaveStream.Write(usValue);
        AFTER_TAG_VALUE_WRITE;
    }

    // scene capture type
    if ( m_eSceneCaptureTypeTagPresence == TPT_PRESENT )
    {
        BEFORE_TAG_VALUE_WRITE(41990,EXIFT_SHORT,1);
        switch ( m_eSceneCaptureType )
        {
        case SCT_STANDARD:
            usValue = 0;
            break;
        case SCT_LANDSCAPE:
            usValue = 1;
            break;
        case SCT_PORTRAIT:
            usValue = 2;
            break;
        case SCT_NIGHT_SCENE:
            usValue = 3;
            break;
        }
        qSaveStream.Write(usValue);
        AFTER_TAG_VALUE_WRITE;
    }

    // gain control
    if ( m_eGainControlTagPresence == TPT_PRESENT )
    {
        BEFORE_TAG_VALUE_WRITE(41991,EXIFT_SHORT,1);
        switch ( m_eGainControl )
        {
        case GCT_NONE:
            usValue = 0;
            break;
        case GCT_LOW_GAIN_UP:
            usValue = 1;
            break;
        case GCT_HIGH_GAIN_UP:
            usValue = 2;
            break;
        case GCT_LOW_GAIN_DOWN:
            usValue = 3;
            break;
        case GCT_HIGH_GAIN_DOWN:
            usValue = 4;
            break;
        }
        qSaveStream.Write(usValue);
        AFTER_TAG_VALUE_WRITE;
    }

    // contrast
    if ( m_eContrastTagPresence == TPT_PRESENT )
    {
        BEFORE_TAG_VALUE_WRITE(41992,EXIFT_SHORT,1);
        switch ( m_eContrast )
        {
        case CT_NORMAL:
            usValue = 0;
            break;
        case CT_SOFT:
            usValue = 1;
            break;
        case CT_HARD:
            usValue = 2;
            break;
        }
        qSaveStream.Write(usValue);
        AFTER_TAG_VALUE_WRITE;
    }

    // saturation
    if ( m_eSaturationTagPresence == TPT_PRESENT )
    {
        BEFORE_TAG_VALUE_WRITE(41993,EXIFT_SHORT,1);
        switch ( m_eSaturation )
        {
        case STT_NORMAL:
            usValue = 0;
            break;
        case STT_LOW_SATURATION:
            usValue = 1;
            break;
        case STT_HIGH_SATURATION:
            usValue = 2;
            break;
        }
        qSaveStream.Write(usValue);
        AFTER_TAG_VALUE_WRITE;
    }

    // sharpness
    if ( m_eSharpnessTagPresence == TPT_PRESENT )
    {
        BEFORE_TAG_VALUE_WRITE(41994,EXIFT_SHORT,1);
        switch ( m_eSharpness )
        {
        case STTT_NORMAL:
            usValue = 0;
            break;
        case STTT_SOFT:
            usValue = 1;
            break;
        case STTT_HARD:
            usValue = 2;
            break;
        }
        qSaveStream.Write(usValue);
        AFTER_TAG_VALUE_WRITE;
    }

    // device settings description
    if ( m_eDeviceSettingDescriptionTagPresence == TPT_PRESENT )
    {
        BEFORE_TAG_VALUE_WRITE(41995,EXIFT_UNDEFINED,m_qDeviceSettingDescription.GetQuantity());
        qSaveStream.Write(m_qDeviceSettingDescription.GetQuantity(),m_qDeviceSettingDescription);
        AFTER_TAG_VALUE_WRITE;
    }

    // subject distance range
    if ( m_eSubjectDistanceRangeTagPresence == TPT_PRESENT )
    {
        BEFORE_TAG_VALUE_WRITE(41996,EXIFT_SHORT,1);
        switch ( m_eSubjectDistanceRange )
        {
        case SDRT_UNKNOWN:
            usValue = 0;
            break;
        case SDRT_MACRO:
            usValue = 1;
            break;
        case SDRT_CLOSE_VIEW:
            usValue = 2;
            break;
        case SDRT_DISTANT_VIEW:
            usValue = 3;
            break;
        }
        qSaveStream.Write(usValue);
        AFTER_TAG_VALUE_WRITE;
    }

    // unique image ID
    if ( m_eImageUniqueIdTagPresence == TPT_PRESENT )
    {
        BEFORE_TAG_VALUE_WRITE(42016,EXIFT_ASCII,33);
        qSaveStream.Write(33,m_qImageUniqueId);
        AFTER_TAG_VALUE_WRITE;
    }

    qSaveStream.OffsetBytePosition(4);

End:
    qSaveStream.SetBytePosition(iBytePosV);
    qSaveStream.Finalize();  // calls AlignBytes

    rqStaticBytestream.BecomeOwner((char*)qSaveStream.GetBytes(),qSaveStream.GetBytePosition());
}
//------------------------------------------------------------------------------------------------------------------
FormatExif::FormatExif (bool bBytestreamIsStatic, const char* acBytestream, int iBytestreamSize)
    :
    FormatBase(bBytestreamIsStatic,acBytestream,( bBytestreamIsStatic ? iBytestreamSize : -1 )),
    ALL_TAGS_ARE_ABSENT
{
}
//------------------------------------------------------------------------------------------------------------------
bool FormatExif::LoadData (int iDynamicBytestreamSize)
{
    assert( ( m_eStreaming == Stream::ST_READING_FROM_DYNAMIC_BYTESTREAM ) == ( iDynamicBytestreamSize >= 0 ) );

    int iBytestreamSize = ( m_eStreaming == Stream::ST_READING_FROM_STATIC_BYTESTREAM ? m_iStaticBytestreamSize :
        iDynamicBytestreamSize );

    bool bSuccess;

    Stream* pqValueStream = 0;

    // TIFF header; 14: header + at least one IFD
    if ( iBytestreamSize < 14 )
    {
        if ( m_eStreaming == Stream::ST_READING_FROM_STATIC_BYTESTREAM )
        {
            THROW_EXCEPTION(ET_FORMAT_MISMATCH)
        }
        else
        {
            delete pqValueStream;
            return false;
        }
    }

    // byte order
    Stream qByteOrderStream(m_eStreaming,m_acStreamSrcParam,0,2,true,bSuccess,iBytestreamSize);
    RETURN_ON_FAIL(bSuccess);
    unsigned short usByteOrder;
    RETURN_ON_FAIL(qByteOrderStream.Read(usByteOrder));
    bool bLittleEndian;
    switch ( usByteOrder )
    {
    case 18761:
        bLittleEndian = true;
        m_eByteOrder = BOT_LITTLE_ENDIAN;
        break;
    case 19789:
        bLittleEndian = false;
        m_eByteOrder = BOT_BIG_ENDIAN;
        break;
    default:
        THROW_EXCEPTION(ET_FORMAT_MISMATCH);
    }
    qByteOrderStream.Finalize();

    // TIFF signature and offset to the 0th IFD
    Stream qSignatureAndOffsetStream(m_eStreaming,m_acStreamSrcParam,2,6,bLittleEndian,bSuccess,iBytestreamSize);
    RETURN_ON_FAIL(bSuccess);
    unsigned short usSignature;
    RETURN_ON_FAIL(qSignatureAndOffsetStream.Read(usSignature));
    CONDITIONAL_THROW_EXCEPTION(( usSignature == 42 ),ET_FORMAT_MISMATCH);
    int iFirstIFDOffset;
    RETURN_ON_FAIL(qSignatureAndOffsetStream.Read(iFirstIFDOffset));
    qSignatureAndOffsetStream.Finalize();

    // Load data.
    Stream qFirstIFDStream(m_eStreaming,m_acStreamSrcParam,iFirstIFDOffset,iBytestreamSize-iFirstIFDOffset,
        bLittleEndian,bSuccess,iBytestreamSize);
    RETURN_ON_FAIL(bSuccess);

    unsigned char ucValue;
    unsigned short usTagQuantity;
    RETURN_ON_FAIL(qFirstIFDStream.Read(usTagQuantity));
    unsigned short usTagId, usType, usValue, usValue0, usValue1;
    int iCount, iValueOffset, iBytePosAfterCountTag, iLength, iValue0, iValue1;
    bool bValueFitsFourBytes;
    ExifType eType;
    int iExifIFDOffset;
    for (int i0 = 0; i0 < usTagQuantity; i0++)
    {
        BEFORE_TAG_VALUE_READ(qFirstIFDStream,MarkerSkipping0);

        if ( usTagId == 256 )
        {
            // image width
            CONDITIONAL_PROCEED_TAG_AS_INCORRECT(( (eType == EXIFT_SHORT || eType == EXIFT_LONG) && iCount == 1 ),
                m_eImageWidthTagPresence,MarkerSkipping0);

            if ( eType == EXIFT_SHORT )
            {
                RETURN_ON_FAIL(qFirstIFDStream.Read(usValue));
                m_iImageWidth = usValue;
            }
            else  // eType = EXIFT_LONG
            {
                RETURN_ON_FAIL(qFirstIFDStream.Read(m_iImageWidth));
            }

            m_eImageWidthTagPresence = TPT_PRESENT;
        }
        else if ( usTagId == 257 )
        {
            // image height
            CONDITIONAL_PROCEED_TAG_AS_INCORRECT(( (eType == EXIFT_SHORT || eType == EXIFT_LONG) && iCount == 1 ),
                m_eImageLengthTagPresence,MarkerSkipping0);

            if ( eType == EXIFT_SHORT )
            {
                RETURN_ON_FAIL(qFirstIFDStream.Read(usValue));
                m_iImageLength = usValue;
            }
            else  // eType = EXIFT_LONG
            {
                RETURN_ON_FAIL(qFirstIFDStream.Read(m_iImageLength));
            }

            m_eImageLengthTagPresence = TPT_PRESENT;
        }
        else if ( usTagId == 258 )
        {
            // number of bits per component
            CONDITIONAL_PROCEED_TAG_AS_INCORRECT(( eType == EXIFT_SHORT && iCount == 3 ),
                m_eBitsPerSampleTagPresence,MarkerSkipping0);

            RETURN_ON_FAIL(pqValueStream->Read(usValue));
            m_iBitsPerSample0 = usValue;
            RETURN_ON_FAIL(pqValueStream->Read(usValue));
            m_iBitsPerSample1 = usValue;
            RETURN_ON_FAIL(pqValueStream->Read(usValue));
            m_iBitsPerSample2 = usValue;

            m_eBitsPerSampleTagPresence = TPT_PRESENT;
        }
        else if ( usTagId == 259 )
        {
            // compression scheme
            CONDITIONAL_PROCEED_TAG_AS_INCORRECT(( eType == EXIFT_SHORT && iCount == 1 ),m_eCompressionTagPresence,
                MarkerSkipping0);

            RETURN_ON_FAIL(qFirstIFDStream.Read(usValue));
            switch ( usValue )
            {
            case 1:
                m_eCompression = CST_UNCOMPRESSED;
                break;
            case 6:
                m_eCompression = CST_JPEG_COMPRESSION;
                break;
            default:
                PROCEED_TAG_AS_INCORRECT(m_eCompressionTagPresence,MarkerSkipping0);
            }

            m_eCompressionTagPresence = TPT_PRESENT;
        }
        else if ( usTagId == 262 )
        {
            // pixel composition
            CONDITIONAL_PROCEED_TAG_AS_INCORRECT(( eType == EXIFT_SHORT && iCount == 1 ),
                m_ePhotometricInterpretationTagPresence,MarkerSkipping0);

            RETURN_ON_FAIL(qFirstIFDStream.Read(usValue));
            switch ( usValue )
            {
            case 2:
                m_ePhotometricInterpretation = PIT_RGB;
                break;
            case 6:
                m_ePhotometricInterpretation = PIT_YCC;
                break;
            default:
                PROCEED_TAG_AS_INCORRECT(m_ePhotometricInterpretationTagPresence,MarkerSkipping0);
            }

            m_ePhotometricInterpretationTagPresence = TPT_PRESENT;
        }
        else if ( usTagId == 270 )
        {
            // image title
            CONDITIONAL_PROCEED_TAG_AS_INCORRECT(( eType == EXIFT_ASCII && iCount >= 0 ),
                m_eImageDescriptionTagPresence,MarkerSkipping0);

            m_qImageDescription.SetLength(( iCount != 0 ? iCount - 1 : 0 ));
            if ( bValueFitsFourBytes )
                RETURN_ON_FAIL(qFirstIFDStream.Read(iCount,m_qImageDescription))
            else
                RETURN_ON_FAIL(pqValueStream->Read(iCount,m_qImageDescription));

            if ( ms_bTrimTexts )
                m_qImageDescription.Trim();

            m_eImageDescriptionTagPresence = TPT_PRESENT;
        }
        else if ( usTagId == 271 )
        {
            // image input equipment manufacturer
            CONDITIONAL_PROCEED_TAG_AS_INCORRECT(( eType == EXIFT_ASCII && iCount >= 0 ),m_eMakeTagPresence,
                MarkerSkipping0);

            m_qMake.SetLength(( iCount != 0 ? iCount - 1 : 0 ));
            if ( bValueFitsFourBytes )
                RETURN_ON_FAIL(qFirstIFDStream.Read(iCount,m_qMake))
            else
                RETURN_ON_FAIL(pqValueStream->Read(iCount,m_qMake));

            if ( ms_bTrimTexts )
                m_qMake.Trim();

            m_eMakeTagPresence = TPT_PRESENT;
        }
        else if ( usTagId == 272 )
        {
            // image input equipment model
            CONDITIONAL_PROCEED_TAG_AS_INCORRECT(( eType == EXIFT_ASCII && iCount >= 0 ),m_eModelTagPresence,
                MarkerSkipping0);

            m_qModel.SetLength(( iCount != 0 ? iCount - 1 : 0 ));
            if ( bValueFitsFourBytes )
                RETURN_ON_FAIL(qFirstIFDStream.Read(iCount,m_qModel))
            else
                RETURN_ON_FAIL(pqValueStream->Read(iCount,m_qModel));

            if ( ms_bTrimTexts )
                m_qModel.Trim();

            m_eModelTagPresence = TPT_PRESENT;
        }
        else if ( usTagId == 273 )
        {
            // image data location
            CONDITIONAL_PROCEED_TAG_AS_INCORRECT(( (eType == EXIFT_SHORT || eType == EXIFT_LONG) && iCount >= 0 ),
                m_eStripOffsetsTagPresence,MarkerSkipping0);

            m_qStripOffsets.SetQuantity(iCount);
            if ( eType == EXIFT_SHORT )
            {
                if ( bValueFitsFourBytes )
                {
                    for (int i1 = 0; i1 < iCount; i1++)
                    {
                        RETURN_ON_FAIL(qFirstIFDStream.Read(usValue));
                        m_qStripOffsets[i1] = usValue;
                    }
                }
                else
                {
                    for (int i1 = 0; i1 < iCount; i1++)
                    {
                        RETURN_ON_FAIL(pqValueStream->Read(usValue));
                        m_qStripOffsets[i1] = usValue;
                    }
                }
            }
            else  // eType = EXIFT_LONG
            {
                if ( bValueFitsFourBytes )
                {
                    if ( iCount != 0 )
                        RETURN_ON_FAIL(qFirstIFDStream.Read(m_qStripOffsets[0]))
                }
                else
                {
                    RETURN_ON_FAIL(pqValueStream->Read(iCount,m_qStripOffsets));
                }
            }

            m_eStripOffsetsTagPresence = TPT_PRESENT;
        }
        else if ( usTagId == 274 )
        {
            // orientation of the image
            CONDITIONAL_PROCEED_TAG_AS_INCORRECT(( eType == EXIFT_SHORT && iCount == 1 ),m_eOrientationTagPresence,
                MarkerSkipping0);

            RETURN_ON_FAIL(qFirstIFDStream.Read(usValue));
            switch ( usValue )
            {
            case 1:
                m_eOrientation = OT_TOP_LEFT;
                break;
            case 2:
                m_eOrientation = OT_TOP_RIGHT;
                break;
            case 3:
                m_eOrientation = OT_BOTTOM_RIGHT;
                break;
            case 4:
                m_eOrientation = OT_BOTTOM_LEFT;
                break;
            case 5:
                m_eOrientation = OT_LEFT_TOP;
                break;
            case 6:
                m_eOrientation = OT_RIGHT_TOP;
                break;
            case 7:
                m_eOrientation = OT_RIGHT_BOTTOM;
                break;
            case 8:
                m_eOrientation = OT_LEFT_BOTTOM;
                break;
            default:
                PROCEED_TAG_AS_INCORRECT(m_eOrientationTagPresence,MarkerSkipping0);
            }

            m_eOrientationTagPresence = TPT_PRESENT;
        }
        else if ( usTagId == 277 )
        {
            // number of components
            CONDITIONAL_PROCEED_TAG_AS_INCORRECT(( eType == EXIFT_SHORT && iCount == 1 ),
                m_eSamplesPerPixelTagPresence,MarkerSkipping0);

            RETURN_ON_FAIL(qFirstIFDStream.Read(usValue));
            m_iSamplesPerPixel = usValue;

            m_eSamplesPerPixelTagPresence = TPT_PRESENT;
        }
        else if ( usTagId == 278 )
        {
            // number of rows per strip
            CONDITIONAL_PROCEED_TAG_AS_INCORRECT(( (eType == EXIFT_SHORT || eType == EXIFT_LONG) && iCount == 1 ),
                m_eRowsPerStripTagPresence,MarkerSkipping0);

            if ( eType == EXIFT_SHORT )
            {
                RETURN_ON_FAIL(qFirstIFDStream.Read(usValue));
                m_iRowsPerStrip = usValue;
            }
            else  // eType = EXIFT_LONG
            {
                RETURN_ON_FAIL(qFirstIFDStream.Read(m_iRowsPerStrip));
            }

            m_eRowsPerStripTagPresence = TPT_PRESENT;
        }
        else if ( usTagId == 279 )
        {
            // bytes per a compressed strip
            CONDITIONAL_PROCEED_TAG_AS_INCORRECT(( (eType == EXIFT_SHORT || eType == EXIFT_LONG) && iCount >= 0 ),
                m_eStripByteCountsTagPresence,MarkerSkipping0);

            m_qStripByteCounts.SetQuantity(iCount);
            if ( eType == EXIFT_SHORT )
            {
                if ( bValueFitsFourBytes )
                {
                    for (int i1 = 0; i1 < iCount; i1++)
                    {
                        RETURN_ON_FAIL(qFirstIFDStream.Read(usValue));
                        m_qStripByteCounts[i1] = usValue;
                    }
                }
                else
                {
                    for (int i1 = 0; i1 < iCount; i1++)
                    {
                        RETURN_ON_FAIL(pqValueStream->Read(usValue));
                        m_qStripByteCounts[i1] = usValue;
                    }
                }
            }
            else  // eType = EXIFT_LONG
            {
                if ( bValueFitsFourBytes )
                {
                    if ( iCount != 0 )
                        RETURN_ON_FAIL(qFirstIFDStream.Read(m_qStripByteCounts[0]))
                }
                else
                {
                    RETURN_ON_FAIL(pqValueStream->Read(iCount,m_qStripByteCounts));
                }
            }

            m_eStripByteCountsTagPresence = TPT_PRESENT;
        }
        else if ( usTagId == 282 )
        {
            // image resolution in width-direction
            CONDITIONAL_PROCEED_TAG_AS_INCORRECT(( eType == EXIFT_RATIONAL && iCount == 1 ),
                m_eXResolutionTagPresence,MarkerSkipping0);

            RETURN_ON_FAIL(pqValueStream->Read(iValue0));
            RETURN_ON_FAIL(pqValueStream->Read(iValue1));
            m_fXResolution = ((float)iValue0)/iValue1;

            m_eXResolutionTagPresence = TPT_PRESENT;
        }
        else if ( usTagId == 283 )
        {
            // image resolution in height-direction
            CONDITIONAL_PROCEED_TAG_AS_INCORRECT(( eType == EXIFT_RATIONAL && iCount == 1 ),
                m_eYResolutionTagPresence,MarkerSkipping0);

            RETURN_ON_FAIL(pqValueStream->Read(iValue0));
            RETURN_ON_FAIL(pqValueStream->Read(iValue1));
            m_fYResolution = ((float)iValue0)/iValue1;

            m_eYResolutionTagPresence = TPT_PRESENT;
        }
        else if ( usTagId == 284 )
        {
            // image data arrangement
            CONDITIONAL_PROCEED_TAG_AS_INCORRECT(( eType == EXIFT_SHORT && iCount == 1 ),
                m_ePlanarConfigurationTagPresence,MarkerSkipping0);

            RETURN_ON_FAIL(qFirstIFDStream.Read(usValue));
            switch ( usValue )
            {
            case 1:
                m_ePlanarConfiguration = PCT_CHUNKY_FORMAT;
                break;
            case 2:
                m_ePlanarConfiguration = PCT_PLANAR_FORMAT;
                break;
            default:
                PROCEED_TAG_AS_INCORRECT(m_ePlanarConfigurationTagPresence,MarkerSkipping0);
            }

            m_ePlanarConfigurationTagPresence = TPT_PRESENT;
        }
        else if ( usTagId == 296 )
        {
            // unit of X and Y resolution
            CONDITIONAL_PROCEED_TAG_AS_INCORRECT(( eType == EXIFT_SHORT && iCount == 1 ),
                m_eResolutionUnitTagPresence,MarkerSkipping0);

            RETURN_ON_FAIL(qFirstIFDStream.Read(usValue));
            switch ( usValue )
            {
            case 2:
                m_eResolutionUnit = RUT_INCHES;
                break;
            case 3:
                m_eResolutionUnit = RUT_CENTIMETERS;
                break;
            default:
                PROCEED_TAG_AS_INCORRECT(m_eResolutionUnitTagPresence,MarkerSkipping0);
            }

            m_eResolutionUnitTagPresence = TPT_PRESENT;
        }
        else if ( usTagId == 301 )
        {
            // transfer function
            CONDITIONAL_PROCEED_TAG_AS_INCORRECT(( eType == EXIFT_SHORT && iCount == 768 ),
                m_eTransferFunctionTagPresence,MarkerSkipping0);

            m_qTransferFunction.SetQuantity(768);
            for (int i1 = 0; i1 < 768; i1++)
            {
                RETURN_ON_FAIL(pqValueStream->Read(usValue));
                m_qTransferFunction[i1] = usValue;
            }

            m_eTransferFunctionTagPresence = TPT_PRESENT;
        }
        else if ( usTagId == 305 )
        {
            // software used
            CONDITIONAL_PROCEED_TAG_AS_INCORRECT(( eType == EXIFT_ASCII && iCount >= 0 ),m_eSoftwareTagPresence,
                MarkerSkipping0);

            m_qSoftware.SetLength(( iCount != 0 ? iCount - 1 : 0 ));
            if ( bValueFitsFourBytes )
                RETURN_ON_FAIL(qFirstIFDStream.Read(iCount,m_qSoftware))
            else
                RETURN_ON_FAIL(pqValueStream->Read(iCount,m_qSoftware));

            if ( ms_bTrimTexts )
                m_qSoftware.Trim();

            m_eSoftwareTagPresence = TPT_PRESENT;
        }
        else if ( usTagId == 306 )
        {
            // file change date and time
            CONDITIONAL_PROCEED_TAG_AS_INCORRECT(( eType == EXIFT_ASCII && iCount == 20 ),m_eDateTimeTagPresence,
                MarkerSkipping0);

            String qDateTimeStr;
            qDateTimeStr.SetLength(19);
            RETURN_ON_FAIL(pqValueStream->Read(20,qDateTimeStr));
            if ( ms_bTrimTexts )
            {
                if ( qDateTimeStr.Trim() )
                {
                    // when date and time are unknown, all character spaces except colons may be filled with
                    // blanks
                    if ( !qDateTimeStr.IsEmpty() && qDateTimeStr[qDateTimeStr.GetLength()-1] == ':' )
                    {
                        qDateTimeStr.SetLength(qDateTimeStr.GetLength()+2);
                        qDateTimeStr[qDateTimeStr.GetLength()-2] = ' ';
                        qDateTimeStr[qDateTimeStr.GetLength()-1] = ' ';
                    }
                }
            }
            m_qDateTime = Date(qDateTimeStr);

            m_eDateTimeTagPresence = TPT_PRESENT;
        }
        else if ( usTagId == 315 )
        {
            // person who created the image
            CONDITIONAL_PROCEED_TAG_AS_INCORRECT(( eType == EXIFT_ASCII && iCount >= 0 ),m_eArtistTagPresence,
                MarkerSkipping0);

            m_qArtist.SetLength(( iCount != 0 ? iCount - 1 : 0 ));
            if ( bValueFitsFourBytes )
                RETURN_ON_FAIL(qFirstIFDStream.Read(iCount,m_qArtist))
            else
                RETURN_ON_FAIL(pqValueStream->Read(iCount,m_qArtist));

            if ( ms_bTrimTexts )
                m_qArtist.Trim();

            m_eArtistTagPresence = TPT_PRESENT;
        }
        else if ( usTagId == 318 )
        {
            // white point chromaticity (x, y)
            CONDITIONAL_PROCEED_TAG_AS_INCORRECT(( eType == EXIFT_RATIONAL && iCount == 2 ),
                m_eWhitePointTagPresence,MarkerSkipping0);

            RETURN_ON_FAIL(pqValueStream->Read(iValue0));
            RETURN_ON_FAIL(pqValueStream->Read(iValue1));
            m_fWhitePointX = ((float)iValue0)/iValue1;
            RETURN_ON_FAIL(pqValueStream->Read(iValue0));
            RETURN_ON_FAIL(pqValueStream->Read(iValue1));
            m_fWhitePointY = ((float)iValue0)/iValue1;

            m_eWhitePointTagPresence = TPT_PRESENT;
        }
        else if ( usTagId == 319 )
        {
            // chromaticities of primaries
            CONDITIONAL_PROCEED_TAG_AS_INCORRECT(( eType == EXIFT_RATIONAL && iCount == 6 ),
                m_ePrimaryChromaticitiesTagPresence,MarkerSkipping0);

            RETURN_ON_FAIL(pqValueStream->Read(iValue0));
            RETURN_ON_FAIL(pqValueStream->Read(iValue1));
            m_fPrimaryChromaticityRedX = ((float)iValue0)/iValue1;
            RETURN_ON_FAIL(pqValueStream->Read(iValue0));
            RETURN_ON_FAIL(pqValueStream->Read(iValue1));
            m_fPrimaryChromaticityRedY = ((float)iValue0)/iValue1;
            RETURN_ON_FAIL(pqValueStream->Read(iValue0));
            RETURN_ON_FAIL(pqValueStream->Read(iValue1));
            m_fPrimaryChromaticityGreenX = ((float)iValue0)/iValue1;
            RETURN_ON_FAIL(pqValueStream->Read(iValue0));
            RETURN_ON_FAIL(pqValueStream->Read(iValue1));
            m_fPrimaryChromaticityGreenY = ((float)iValue0)/iValue1;
            RETURN_ON_FAIL(pqValueStream->Read(iValue0));
            RETURN_ON_FAIL(pqValueStream->Read(iValue1));
            m_fPrimaryChromaticityBlueX = ((float)iValue0)/iValue1;
            RETURN_ON_FAIL(pqValueStream->Read(iValue0));
            RETURN_ON_FAIL(pqValueStream->Read(iValue1));
            m_fPrimaryChromaticityBlueY = ((float)iValue0)/iValue1;

            m_ePrimaryChromaticitiesTagPresence = TPT_PRESENT;
        }
        else if ( usTagId == 513 )
        {
            // offset to JPEG SOI
            CONDITIONAL_PROCEED_TAG_AS_INCORRECT(( eType == EXIFT_LONG && iCount == 1 ),
                m_eJPEGInterchangeFormatTagPresence,MarkerSkipping0);

            RETURN_ON_FAIL(qFirstIFDStream.Read(m_iJPEGInterchangeFormat));

            m_eJPEGInterchangeFormatTagPresence = TPT_PRESENT;
        }
        else if ( usTagId == 514 )
        {
            // bytes of JPEG data
            CONDITIONAL_PROCEED_TAG_AS_INCORRECT(( eType == EXIFT_LONG && iCount == 1 ),
                m_eJPEGInterchangeFormatLengthTagPresence,MarkerSkipping0);

            RETURN_ON_FAIL(qFirstIFDStream.Read(m_iJPEGInterchangeFormatLength));

            m_eJPEGInterchangeFormatLengthTagPresence = TPT_PRESENT;
        }
        else if ( usTagId == 529 )
        {
            // color space transformation matrix coefficients
            CONDITIONAL_PROCEED_TAG_AS_INCORRECT(( eType == EXIFT_RATIONAL && iCount == 3 ),
                m_eYCbCrCoefficientsTagPresence,MarkerSkipping0);

            RETURN_ON_FAIL(pqValueStream->Read(iValue0));
            RETURN_ON_FAIL(pqValueStream->Read(iValue1));
            m_fCoefficientY = ((float)iValue0)/iValue1;
            RETURN_ON_FAIL(pqValueStream->Read(iValue0));
            RETURN_ON_FAIL(pqValueStream->Read(iValue1));
            m_fCoefficientCb = ((float)iValue0)/iValue1;
            RETURN_ON_FAIL(pqValueStream->Read(iValue0));
            RETURN_ON_FAIL(pqValueStream->Read(iValue1));
            m_fCoefficientCr = ((float)iValue0)/iValue1;

            m_eYCbCrCoefficientsTagPresence = TPT_PRESENT;
        }
        else if ( usTagId == 530 )
        {
            // subsampling ratio of Y (luminance) to C (chrominance)
            CONDITIONAL_PROCEED_TAG_AS_INCORRECT(( eType == EXIFT_SHORT && iCount == 2 ),
                m_eYCbCrSubSamplingTagPresence,MarkerSkipping0);

            RETURN_ON_FAIL(qFirstIFDStream.Read(usValue0));
            RETURN_ON_FAIL(qFirstIFDStream.Read(usValue1));
            if ( usValue0 == 2 && usValue1 == 1 )
                m_eYCbCrSubSampling = YCCSST_4_2_2;
            else if ( usValue0 == 2 && usValue1 == 2 )
                m_eYCbCrSubSampling = YCCSST_4_2_0;
            else
                PROCEED_TAG_AS_INCORRECT(m_eYCbCrSubSamplingTagPresence,MarkerSkipping0);

            m_eYCbCrSubSamplingTagPresence = TPT_PRESENT;
        }
        else if ( usTagId == 531 )
        {
            // Y (luminance) and C (chrominance) positioning
            CONDITIONAL_PROCEED_TAG_AS_INCORRECT(( eType == EXIFT_SHORT && iCount == 1 ),
                m_eYCbCrPositioningTagPresence,MarkerSkipping0);

            RETURN_ON_FAIL(qFirstIFDStream.Read(usValue));
            switch ( usValue )
            {
            case 1:
                m_eYCbCrPositioning = YCCPT_CENTERED;
                break;
            case 2:
                m_eYCbCrPositioning = YCCPT_CO_SITED;
                break;
            default:
                PROCEED_TAG_AS_INCORRECT(m_eYCbCrPositioningTagPresence,MarkerSkipping0);
            }

            m_eYCbCrPositioningTagPresence = TPT_PRESENT;
        }
        else if ( usTagId == 532 )
        {
            // pair of black and white reference values
            CONDITIONAL_PROCEED_TAG_AS_INCORRECT(( eType == EXIFT_RATIONAL && iCount == 6 ),
                m_eReferenceBlackWhiteTagPresence,MarkerSkipping0);

            RETURN_ON_FAIL(pqValueStream->Read(iValue0));
            RETURN_ON_FAIL(pqValueStream->Read(iValue1));
            m_qReferenceBlackWhiteComponent0.First = ((float)iValue0)/iValue1;
            RETURN_ON_FAIL(pqValueStream->Read(iValue0));
            RETURN_ON_FAIL(pqValueStream->Read(iValue1));
            m_qReferenceBlackWhiteComponent0.Second = ((float)iValue0)/iValue1;
            RETURN_ON_FAIL(pqValueStream->Read(iValue0));
            RETURN_ON_FAIL(pqValueStream->Read(iValue1));
            m_qReferenceBlackWhiteComponent1.First = ((float)iValue0)/iValue1;
            RETURN_ON_FAIL(pqValueStream->Read(iValue0));
            RETURN_ON_FAIL(pqValueStream->Read(iValue1));
            m_qReferenceBlackWhiteComponent1.Second = ((float)iValue0)/iValue1;
            RETURN_ON_FAIL(pqValueStream->Read(iValue0));
            RETURN_ON_FAIL(pqValueStream->Read(iValue1));
            m_qReferenceBlackWhiteComponent2.First = ((float)iValue0)/iValue1;
            RETURN_ON_FAIL(pqValueStream->Read(iValue0));
            RETURN_ON_FAIL(pqValueStream->Read(iValue1));
            m_qReferenceBlackWhiteComponent2.Second = ((float)iValue0)/iValue1;

            m_eReferenceBlackWhiteTagPresence = TPT_PRESENT;
        }
        else if ( usTagId == 33432 )
        {
            // copyright holder
            CONDITIONAL_PROCEED_TAG_AS_INCORRECT(( eType == EXIFT_ASCII && iCount >= 0 ),m_eCopyrightTagPresence,
                MarkerSkipping0);

            if ( iCount != 0 )
            {
                StillArray<char> qChars(iCount);
                if ( bValueFitsFourBytes )
                    RETURN_ON_FAIL(qFirstIFDStream.Read(iCount,qChars))
                else
                    RETURN_ON_FAIL(pqValueStream->Read(iCount,qChars));

                bool bHasTwoZeros = false;
                int iFirstPartZeroPos;
                int iCMO = iCount - 1;
                for (int i1 = 0; i1 < iCMO; i1++)
                {
                    if ( qChars[i1] == 0 )
                    {
                        bHasTwoZeros = true;
                        iFirstPartZeroPos = i1;
                        break;
                    }
                }
                if ( bHasTwoZeros )
                {
                    CONDITIONAL_PROCEED_TAG_AS_INCORRECT(( iCount >= 4 && iFirstPartZeroPos != 0 ),
                        m_eCopyrightTagPresence,MarkerSkipping0);
                    if ( iFirstPartZeroPos != 1 || (iFirstPartZeroPos == 1 && qChars[0] != ' ') )
                    {
                        // both photographer copyright and editor copyright are given
                        m_bPhotographerCopyrightIsPresent = true;
                        m_bEditorCopyrightIsPresent = true;
                        m_qPhotographerCopyright.SetLength(iFirstPartZeroPos);
                        StrNCpy(m_qPhotographerCopyright,qChars,m_qPhotographerCopyright.GetLength()+1);
                        int iFPZPPO = iFirstPartZeroPos + 1;
                        m_qEditorCopyright.SetLength(iCMO-iFPZPPO);
                        StrNCpy(m_qEditorCopyright,qChars.GetElements()+iFPZPPO,m_qEditorCopyright.GetLength()+1);
                        if ( ms_bTrimTexts )
                        {
                            m_qPhotographerCopyright.Trim();
                            m_qEditorCopyright.Trim();
                        }
                    }
                    else
                    {
                        // only editor copyright is given
                        m_bPhotographerCopyrightIsPresent = false;
                        m_bEditorCopyrightIsPresent = true;
                        m_qEditorCopyright.SetLength(iCount-3);
                        StrNCpy(m_qEditorCopyright,qChars.GetElements()+2,m_qEditorCopyright.GetLength()+1);
                        if ( ms_bTrimTexts )
                            m_qEditorCopyright.Trim();
                    }
                }
                else
                {
                    // only photographer copyright is given
                    m_bPhotographerCopyrightIsPresent = true;
                    m_bEditorCopyrightIsPresent = false;
                    m_qPhotographerCopyright.SetLength(iCMO);
                    StrNCpy(m_qPhotographerCopyright,qChars,iCount);
                    if ( ms_bTrimTexts )
                        m_qPhotographerCopyright.Trim();
                }
            }
            else
            {
                m_bPhotographerCopyrightIsPresent = false;
                m_bEditorCopyrightIsPresent = false;
            }

            m_eCopyrightTagPresence = TPT_PRESENT;
        }
        else if ( usTagId == 34665 )
        {
            // pointer to the Exif IFD
            CONDITIONAL_PROCEED_TAG_AS_INCORRECT(( eType == EXIFT_LONG && iCount == 1 ),
                m_eExifIFDPointerTagPresence,MarkerSkipping0);

            RETURN_ON_FAIL(qFirstIFDStream.Read(iExifIFDOffset));

            m_eExifIFDPointerTagPresence = TPT_PRESENT;
        }
        else
        {
MarkerSkipping0:
            if ( bValueFitsFourBytes )
                qFirstIFDStream.SetBytePosition(iBytePosAfterCountTag+iLength);
        }

        AFTER_TAG_VALUE_READ(qFirstIFDStream);
    }

    qFirstIFDStream.Finalize();

    if ( m_eExifIFDPointerTagPresence == TPT_PRESENT )
    {
        Stream qExifIFDStream(m_eStreaming,m_acStreamSrcParam,iExifIFDOffset,iBytestreamSize-iExifIFDOffset,
            bLittleEndian,bSuccess,iBytestreamSize);
        RETURN_ON_FAIL(bSuccess);

        RETURN_ON_FAIL(qExifIFDStream.Read(usTagQuantity));
        for (int i0 = 0; i0 < usTagQuantity; i0++)
        {
            BEFORE_TAG_VALUE_READ(qExifIFDStream,MarkerSkipping1);

            if ( usTagId == 33434 )
            {
                // exposure time
                CONDITIONAL_PROCEED_TAG_AS_INCORRECT(( eType == EXIFT_RATIONAL && iCount == 1 ),
                    m_eExposureTimeTagPresence,MarkerSkipping1);

                RETURN_ON_FAIL(pqValueStream->Read(iValue0));
                RETURN_ON_FAIL(pqValueStream->Read(iValue1));
                m_fExposureTime = ((float)iValue0)/iValue1;

                m_eExposureTimeTagPresence = TPT_PRESENT;
            }
            else if ( usTagId == 33437 )
            {
                // F number
                CONDITIONAL_PROCEED_TAG_AS_INCORRECT(( eType == EXIFT_RATIONAL && iCount == 1 ),
                    m_eFNumberTagPresence,MarkerSkipping1);

                RETURN_ON_FAIL(pqValueStream->Read(iValue0));
                RETURN_ON_FAIL(pqValueStream->Read(iValue1));
                m_fFNumber = ((float)iValue0)/iValue1;

                m_eFNumberTagPresence = TPT_PRESENT;
            }
            else if ( usTagId == 34850 )
            {
                // exposure program
                CONDITIONAL_PROCEED_TAG_AS_INCORRECT(( eType == EXIFT_SHORT && iCount == 1 ),
                    m_eExposureProgramTagPresence,MarkerSkipping1);

                RETURN_ON_FAIL(qExifIFDStream.Read(usValue));
                switch ( usValue )
                {
                case 0:
                    m_eExposureProgram = EPT_NOT_DEFINED;
                    break;
                case 1:
                    m_eExposureProgram = EPT_MANUAL;
                    break;
                case 2:
                    m_eExposureProgram = EPT_NORMAL_PROGRAM;
                    break;
                case 3:
                    m_eExposureProgram = EPT_APERTURE_PRIORITY;
                    break;
                case 4:
                    m_eExposureProgram = EPT_SHUTTER_PRIORITY;
                    break;
                case 5:
                    m_eExposureProgram = EPT_CREATIVE_PROGRAM;
                    break;
                case 6:
                    m_eExposureProgram = EPT_ACTION_PROGRAM;
                    break;
                case 7:
                    m_eExposureProgram = EPT_PORTRAIT_MODE;
                    break;
                case 8:
                    m_eExposureProgram = EPT_LANDSCAPE_MODE;
                    break;
                default:
                    PROCEED_TAG_AS_INCORRECT(m_eExposureProgramTagPresence,MarkerSkipping1);
                }

                m_eExposureProgramTagPresence = TPT_PRESENT;
            }
            else if ( usTagId == 34852 )
            {
                // spectral sensitivity
                CONDITIONAL_PROCEED_TAG_AS_INCORRECT(( eType == EXIFT_ASCII && iCount >= 0 ),
                    m_eSpectralSensitivityTagPresence,MarkerSkipping1);

                m_qSpectralSensitivity.SetLength(( iCount != 0 ? iCount - 1 : 0 ));
                if ( bValueFitsFourBytes )
                    RETURN_ON_FAIL(qExifIFDStream.Read(iCount,m_qSpectralSensitivity))
                else
                    RETURN_ON_FAIL(pqValueStream->Read(iCount,m_qSpectralSensitivity));

                if ( ms_bTrimTexts )
                    m_qSpectralSensitivity.Trim();

                m_eSpectralSensitivityTagPresence = TPT_PRESENT;
            }
            else if ( usTagId == 34855 )
            {
                // ISO speed rating
                CONDITIONAL_PROCEED_TAG_AS_INCORRECT(( eType == EXIFT_SHORT && iCount >= 0 ),
                    m_eISOSpeedRatingsTagPresence,MarkerSkipping1);

                m_qISOSpeedRatings.SetQuantity(iCount);
                if ( bValueFitsFourBytes )
                {
                    for (int i1 = 0; i1 < iCount; i1++)
                    {
                        RETURN_ON_FAIL(qExifIFDStream.Read(usValue));
                        m_qISOSpeedRatings[i1] = usValue;
                    }
                }
                else
                {
                    for (int i1 = 0; i1 < iCount; i1++)
                    {
                        RETURN_ON_FAIL(pqValueStream->Read(usValue));
                        m_qISOSpeedRatings[i1] = usValue;
                    }
                }

                m_eISOSpeedRatingsTagPresence = TPT_PRESENT;
            }
            else if ( usTagId == 34856 )
            {
                // optoelectric conversion factor
                CONDITIONAL_PROCEED_TAG_AS_INCORRECT(( eType == EXIFT_UNDEFINED && iCount >= 0 ),m_eOECFTagPresence,
                    MarkerSkipping1);

                m_qOECF.SetQuantity(iCount);
                if ( bValueFitsFourBytes )
                    RETURN_ON_FAIL(qExifIFDStream.Read(iCount,m_qOECF))
                else
                    RETURN_ON_FAIL(pqValueStream->Read(iCount,m_qOECF));

                m_eOECFTagPresence = TPT_PRESENT;
            }
            else if ( usTagId == 36864 )
            {
                // Exif version
                CONDITIONAL_PROCEED_TAG_AS_INCORRECT(( eType == EXIFT_UNDEFINED && iCount == 4 ),
                    m_eExifVersionTagPresence,MarkerSkipping1);

                unsigned char aucByte[4];
                RETURN_ON_FAIL(qExifIFDStream.Read(4,aucByte));

                if ( aucByte[0] == '0' && aucByte[1] == '2' && aucByte[2] == '0' && aucByte[3] == '0' )
                    m_eExifVersion = EXIFVT_2_0;
                else if ( aucByte[0] == '0' && aucByte[1] == '2' && aucByte[2] == '1' && aucByte[3] == '0' )
                    m_eExifVersion = EXIFVT_2_1;
                else if ( aucByte[0] == '0' && aucByte[1] == '2' && aucByte[2] == '2' && aucByte[3] == '0' )
                    m_eExifVersion = EXIFVT_2_2;
                else
                    m_eExifVersion = EXIFVT_OTHER;

                m_eExifVersionTagPresence = TPT_PRESENT;
            }
            else if ( usTagId == 36867 )
            {
                // date and time of original data generation
                CONDITIONAL_PROCEED_TAG_AS_INCORRECT(( eType == EXIFT_ASCII && iCount == 20 ),
                    m_eDateTimeOriginalTagPresence,MarkerSkipping1);

                String qDateTimeOriginalStr;
                qDateTimeOriginalStr.SetLength(19);
                RETURN_ON_FAIL(pqValueStream->Read(20,qDateTimeOriginalStr));
                if ( ms_bTrimTexts )
                {
                    if ( qDateTimeOriginalStr.Trim() )
                    {
                        // when date and time are unknown, all character spaces except colons may be filled
                        // with blanks
                        if ( !qDateTimeOriginalStr.IsEmpty()
                        &&   qDateTimeOriginalStr[qDateTimeOriginalStr.GetLength()-1] == ':' )
                        {
                            qDateTimeOriginalStr.SetLength(qDateTimeOriginalStr.GetLength()+2);
                            qDateTimeOriginalStr[qDateTimeOriginalStr.GetLength()-2] = ' ';
                            qDateTimeOriginalStr[qDateTimeOriginalStr.GetLength()-1] = ' ';
                        }
                    }
                }
                m_qDateTimeOriginal = Date(qDateTimeOriginalStr);

                m_eDateTimeOriginalTagPresence = TPT_PRESENT;
            }
            else if ( usTagId == 36868 )
            {
                // date and time of digital data generation
                CONDITIONAL_PROCEED_TAG_AS_INCORRECT(( eType == EXIFT_ASCII && iCount == 20 ),
                    m_eDateTimeDigitizedTagPresence,MarkerSkipping1);

                String qDateTimeDigitizedStr;
                qDateTimeDigitizedStr.SetLength(19);
                RETURN_ON_FAIL(pqValueStream->Read(20,qDateTimeDigitizedStr));
                if ( ms_bTrimTexts )
                {
                    if ( qDateTimeDigitizedStr.Trim() )
                    {
                        // when date and time are unknown, all character spaces except colons may be filled
                        // with blanks
                        if ( !qDateTimeDigitizedStr.IsEmpty()
                        &&   qDateTimeDigitizedStr[qDateTimeDigitizedStr.GetLength()-1] == ':' )
                        {
                            qDateTimeDigitizedStr.SetLength(qDateTimeDigitizedStr.GetLength()+2);
                            qDateTimeDigitizedStr[qDateTimeDigitizedStr.GetLength()-2] = ' ';
                            qDateTimeDigitizedStr[qDateTimeDigitizedStr.GetLength()-1] = ' ';
                        }
                    }
                }
                m_qDateTimeDigitized = Date(qDateTimeDigitizedStr);

                m_eDateTimeDigitizedTagPresence = TPT_PRESENT;
            }
            else if ( usTagId == 37121 )
            {
                // meaning of each component
                CONDITIONAL_PROCEED_TAG_AS_INCORRECT(( eType == EXIFT_UNDEFINED && iCount == 4 ),
                    m_eComponentsConfigurationTagPresence,MarkerSkipping1);

                unsigned char aucByte[4];
                RETURN_ON_FAIL(qExifIFDStream.Read(4,aucByte));

                if ( aucByte[0] == 4 && aucByte[1] == 5 && aucByte[2] == 6 && aucByte[3] == 0 )
                    m_eComponentsConfiguration = CCT_RGB;
                else if ( aucByte[0] == 1 && aucByte[1] == 2 && aucByte[2] == 3 && aucByte[3] == 0 )
                    m_eComponentsConfiguration = CCT_YCC;
                else
                    PROCEED_TAG_AS_INCORRECT(m_eComponentsConfigurationTagPresence,MarkerSkipping1);

                m_eComponentsConfigurationTagPresence = TPT_PRESENT;
            }
            else if ( usTagId == 37122 )
            {
                // image compression mode
                CONDITIONAL_PROCEED_TAG_AS_INCORRECT(( eType == EXIFT_RATIONAL && iCount == 1 ),
                    m_eCompressedBitsPerPixelTagPresence,MarkerSkipping1);

                RETURN_ON_FAIL(pqValueStream->Read(iValue0));
                RETURN_ON_FAIL(pqValueStream->Read(iValue1));
                m_fCompressedBitsPerPixel = ((float)iValue0)/iValue1;

                m_eCompressedBitsPerPixelTagPresence = TPT_PRESENT;
            }
            else if ( usTagId == 37377 )
            {
                // shutter speed
                CONDITIONAL_PROCEED_TAG_AS_INCORRECT(( eType == EXIFT_SRATIONAL && iCount == 1 ),
                    m_eShutterSpeedValueTagPresence,MarkerSkipping1);

                RETURN_ON_FAIL(pqValueStream->Read(iValue0));
                RETURN_ON_FAIL(pqValueStream->Read(iValue1));
                m_fShutterSpeedValue = ((float)iValue0)/iValue1;

                m_eShutterSpeedValueTagPresence = TPT_PRESENT;
            }
            else if ( usTagId == 37378 )
            {
                // aperture
                CONDITIONAL_PROCEED_TAG_AS_INCORRECT(( eType == EXIFT_RATIONAL && iCount == 1 ),
                    m_eApertureValueTagPresence,MarkerSkipping1);

                RETURN_ON_FAIL(pqValueStream->Read(iValue0));
                RETURN_ON_FAIL(pqValueStream->Read(iValue1));
                m_fApertureValue = ((float)iValue0)/iValue1;

                m_eApertureValueTagPresence = TPT_PRESENT;
            }
            else if ( usTagId == 37379 )
            {
                // brightness
                CONDITIONAL_PROCEED_TAG_AS_INCORRECT(( eType == EXIFT_SRATIONAL && iCount == 1 ),
                    m_eBrightnessValueTagPresence,MarkerSkipping1);

                RETURN_ON_FAIL(pqValueStream->Read(iValue0));
                RETURN_ON_FAIL(pqValueStream->Read(iValue1));
                if ( iValue0 != 4294967295 )
                {
                    m_eBrightnessValueState = BVST_KNOWN;
                    m_fBrightnessValue = ((float)iValue0)/iValue1;
                }
                else
                {
                    m_eBrightnessValueState = BVST_UNKNOWN;
                }

                m_eBrightnessValueTagPresence = TPT_PRESENT;
            }
            else if ( usTagId == 37380 )
            {
                // exposure bias
                CONDITIONAL_PROCEED_TAG_AS_INCORRECT(( eType == EXIFT_SRATIONAL && iCount == 1 ),
                    m_eExposureBiasValueTagPresence,MarkerSkipping1);

                RETURN_ON_FAIL(pqValueStream->Read(iValue0));
                RETURN_ON_FAIL(pqValueStream->Read(iValue1));
                m_fExposureBiasValue = ((float)iValue0)/iValue1;

                m_eExposureBiasValueTagPresence = TPT_PRESENT;
            }
            else if ( usTagId == 37381 )
            {
                // maximum lens aperture
                CONDITIONAL_PROCEED_TAG_AS_INCORRECT(( eType == EXIFT_RATIONAL && iCount == 1 ),
                    m_eMaxApertureValueTagPresence,MarkerSkipping1);

                RETURN_ON_FAIL(pqValueStream->Read(iValue0));
                RETURN_ON_FAIL(pqValueStream->Read(iValue1));
                m_fMaxApertureValue = ((float)iValue0)/iValue1;

                m_eMaxApertureValueTagPresence = TPT_PRESENT;
            }
            else if ( usTagId == 37382 )
            {
                // subject distance
                CONDITIONAL_PROCEED_TAG_AS_INCORRECT(( eType == EXIFT_RATIONAL && iCount == 1 ),
                    m_eSubjectDistanceTagPresence,MarkerSkipping1);

                RETURN_ON_FAIL(pqValueStream->Read(iValue0));
                RETURN_ON_FAIL(pqValueStream->Read(iValue1));
                if ( iValue0 != 4294967295 && iValue0 != 0 )
                {
                    m_eSubjectDistanceState = SDST_KNOWN;
                    m_fSubjectDistance = ((float)iValue0)/iValue1;
                }
                else if ( iValue0 == 4294967295 )
                {
                    m_eSubjectDistanceState = SDST_INFINITY;
                }
                else  // iValue0 = 0
                {
                    m_eSubjectDistanceState = SDST_UNKNOWN;
                }

                m_eSubjectDistanceTagPresence = TPT_PRESENT;
            }
            else if ( usTagId == 37383 )
            {
                // metering mode
                CONDITIONAL_PROCEED_TAG_AS_INCORRECT(( eType == EXIFT_SHORT && iCount == 1 ),
                    m_eMeteringModeTagPresence,MarkerSkipping1);

                RETURN_ON_FAIL(qExifIFDStream.Read(usValue));
                switch ( usValue )
                {
                case 0:
                    m_eMeteringMode = MMT_UNKNOWN;
                    break;
                case 1:
                    m_eMeteringMode = MMT_AVERAGE;
                    break;
                case 2:
                    m_eMeteringMode = MMT_CENTER_WEIGHTED_AVERAGE;
                    break;
                case 3:
                    m_eMeteringMode = MMT_SPOT;
                    break;
                case 4:
                    m_eMeteringMode = MMT_MULTI_SPOT;
                    break;
                case 5:
                    m_eMeteringMode = MMT_PATTERN;
                    break;
                case 6:
                    m_eMeteringMode = MMT_PARTIAL;
                    break;
                case 255:
                    m_eMeteringMode = MMT_OTHER;
                    break;
                default:
                    PROCEED_TAG_AS_INCORRECT(m_eMeteringModeTagPresence,MarkerSkipping1);
                }

                m_eMeteringModeTagPresence = TPT_PRESENT;
            }
            else if ( usTagId == 37384 )
            {
                // light source
                CONDITIONAL_PROCEED_TAG_AS_INCORRECT(( eType == EXIFT_SHORT && iCount == 1 ),
                    m_eLightSourceTagPresence,MarkerSkipping1);

                RETURN_ON_FAIL(qExifIFDStream.Read(usValue));
                switch ( usValue )
                {
                case 0:
                    m_eLightSource = LST_UNKNOWN;
                    break;
                case 1:
                    m_eLightSource = LST_DAYLIGHT;
                    break;
                case 2:
                    m_eLightSource = LST_FLUORESCENT;
                    break;
                case 3:
                    m_eLightSource = LST_TUNGSTEN;
                    break;
                case 4:
                    m_eLightSource = LST_FLASH;
                    break;
                case 9:
                    m_eLightSource = LST_FINE_WEATHER;
                    break;
                case 10:
                    m_eLightSource = LST_CLOUDY_WEATHER;
                    break;
                case 11:
                    m_eLightSource = LST_SHADE;
                    break;
                case 12:
                    m_eLightSource = LST_DAYLIGHT_FLUORESCENT;
                    break;
                case 13:
                    m_eLightSource = LST_DAY_WHITE_FLUORESCENT;
                    break;
                case 14:
                    m_eLightSource = LST_COOL_WHITE_FLUORESCENT;
                    break;
                case 15:
                    m_eLightSource = LST_WHITE_FLUORESCENT;
                    break;
                case 17:
                    m_eLightSource = LST_STANDARD_LIGHT_A;
                    break;
                case 18:
                    m_eLightSource = LST_STANDARD_LIGHT_B;
                    break;
                case 19:
                    m_eLightSource = LST_STANDARD_LIGHT_C;
                    break;
                case 20:
                    m_eLightSource = LST_D55;
                    break;
                case 21:
                    m_eLightSource = LST_D65;
                    break;
                case 22:
                    m_eLightSource = LST_D75;
                    break;
                case 23:
                    m_eLightSource = LST_D50;
                    break;
                case 24:
                    m_eLightSource = LST_ISO_STUDIO_TUNGSTEN;
                    break;
                case 255:
                    m_eLightSource = LST_OTHER;
                    break;
                default:
                    PROCEED_TAG_AS_INCORRECT(m_eLightSourceTagPresence,MarkerSkipping1);
                }

                m_eLightSourceTagPresence = TPT_PRESENT;
            }
            else if ( usTagId == 37385 )
            {
                // flash
                CONDITIONAL_PROCEED_TAG_AS_INCORRECT(( eType == EXIFT_SHORT && iCount == 1 ),m_eFlashTagPresence,
                    MarkerSkipping1);

                RETURN_ON_FAIL(qExifIFDStream.Read(usValue));
                switch ( usValue )
                {
                case 0:
                    m_eFlash = FT_FLASH_DID_NOT_FIRE;
                    break;
                case 1:
                    m_eFlash = FT_FLASH_FIRED;
                    break;
                case 5:
                    m_eFlash = FT_STROBE_RETURN_LIGHT_NOT_DETECTED;
                    break;
                case 7:
                    m_eFlash = FT_STROBE_RETURN_LIGHT_DETECTED;
                    break;
                case 9:
                    m_eFlash = FT_FLASH_FIRED__COMPULSORY_FLASH_MODE;
                    break;
                case 13:
                    m_eFlash = FT_FLASH_FIRED__COMPULSORY_FLASH_MODE__RETURN_LIGHT_NOT_DETECTED;
                    break;
                case 15:
                    m_eFlash = FT_FLASH_FIRED__COMPULSORY_FLASH_MODE__RETURN_LIGHT_DETECTED;
                    break;
                case 16:
                    m_eFlash = FT_FLASH_DID_NOT_FIRE__COMPULSORY_FLASH_MODE;
                    break;
                case 24:
                    m_eFlash = FT_FLASH_DID_NOT_FIRE__AUTO_MODE;
                    break;
                case 25:
                    m_eFlash = FT_FLASH_FIRED__AUTO_MODE;
                    break;
                case 29:
                    m_eFlash = FT_FLASH_FIRED__AUTO_MODE__RETURN_LIGHT_NOT_DETECTED;
                    break;
                case 31:
                    m_eFlash = FT_FLASH_FIRED__AUTO_MODE__RETURN_LIGHT_DETECTED;
                    break;
                case 32:
                    m_eFlash = FT_NO_FLASH_FUNCTION;
                    break;
                case 65:
                    m_eFlash = FT_FLASH_FIRED__REDEYE_REDUCTION_MODE;
                    break;
                case 69:
                    m_eFlash = FT_FLASH_FIRED__REDEYE_REDUCTION_MODE__RETURN_LIGHT_NOT_DETECTED;
                    break;
                case 71:
                    m_eFlash = FT_FLASH_FIRED__REDEYE_REDUCTION_MODE__RETURN_LIGHT_DETECTED;
                    break;
                case 73:
                    m_eFlash = FT_FLASH_FIRED__COMPULSORY_FLASH_MODE__REDEYE_REDUCTION_MODE;
                    break;
                case 77:
                    m_eFlash =
                        FT_FLASH_FIRED__COMPULSORY_FLASH_MODE__REDEYE_REDUCTION_MODE__RETURN_LIGHT_NOT_DETECTED;
                    break;
                case 79:
                    m_eFlash = FT_FLASH_FIRED__COMPULSORY_FLASH_MODE__REDEYE_REDUCTION_MODE__RETURN_LIGHT_DETECTED;
                    break;
                case 89:
                    m_eFlash = FT_FLASH_FIRED__AUTO_MODE__REDEYE_REDUCTION_MODE;
                    break;
                case 93:
                    m_eFlash = FT_FLASH_FIRED__AUTO_MODE__RETURN_LIGHT_NOT_DETECTED__REDEYE_REDUCTION_MODE;
                    break;
                case 95:
                    m_eFlash = FT_FLASH_FIRED__AUTO_MODE__RETURN_LIGHT_DETECTED__REDEYE_REDUCTION_MODE;
                    break;
                default:
                    PROCEED_TAG_AS_INCORRECT(m_eFlashTagPresence,MarkerSkipping1);
                }

                m_eFlashTagPresence = TPT_PRESENT;
            }
            else if ( usTagId == 37386 )
            {
                // lens focal length
                CONDITIONAL_PROCEED_TAG_AS_INCORRECT(( eType == EXIFT_RATIONAL && iCount == 1 ),
                    m_eFocalLengthTagPresence,MarkerSkipping1);

                RETURN_ON_FAIL(pqValueStream->Read(iValue0));
                RETURN_ON_FAIL(pqValueStream->Read(iValue1));
                m_fFocalLength = ((float)iValue0)/iValue1;

                m_eFocalLengthTagPresence = TPT_PRESENT;
            }
            else if ( usTagId == 37396 )
            {
                // subject area
                CONDITIONAL_PROCEED_TAG_AS_INCORRECT(
                    ( eType == EXIFT_SHORT && (iCount == 2 || iCount == 3 || iCount == 4) ),
                    m_eSubjectAreaTagPresence,MarkerSkipping1);

                m_qSubjectAreaCoordinates.SetQuantity(iCount);
                if ( bValueFitsFourBytes )
                {
                    RETURN_ON_FAIL(qExifIFDStream.Read(usValue));
                    m_qSubjectAreaCoordinates[0] = usValue;
                    RETURN_ON_FAIL(qExifIFDStream.Read(usValue));
                    m_qSubjectAreaCoordinates[1] = usValue;
                }
                else
                {
                    for (int i1 = 0; i1 < iCount; i1++)
                    {
                        RETURN_ON_FAIL(pqValueStream->Read(usValue));
                        m_qSubjectAreaCoordinates[i1] = usValue;
                    }
                }

                m_eSubjectAreaTagPresence = TPT_PRESENT;
            }
            else if ( usTagId == 37500 )
            {
                // manufacturer notes
                CONDITIONAL_PROCEED_TAG_AS_INCORRECT(( eType == EXIFT_UNDEFINED && iCount >= 0 ),
                    m_eMakerNoteTagPresence,MarkerSkipping1);

                m_qMakerNote.SetQuantity(iCount);
                if ( bValueFitsFourBytes )
                    RETURN_ON_FAIL(qExifIFDStream.Read(iCount,m_qMakerNote))
                else
                    RETURN_ON_FAIL(pqValueStream->Read(iCount,m_qMakerNote));

                m_eMakerNoteTagPresence = TPT_PRESENT;
            }
            else if ( usTagId == 37510 )
            {
                // user comments
                CONDITIONAL_PROCEED_TAG_AS_INCORRECT(( eType == EXIFT_UNDEFINED && iCount >= 8 ),
                    m_eUserCommentTagPresence,MarkerSkipping1);

                unsigned char aucCharacterCode[8];
                RETURN_ON_FAIL(pqValueStream->Read(8,aucCharacterCode));
                if ( MemCmp(aucCharacterCode,gs_acCharacterCodeASCII,8) == 0 )
                    m_eUserCommentCharacterCode = UCCCT_ASCII;
                else if ( MemCmp(aucCharacterCode,gs_acCharacterCodeJIS,8) == 0 )
                    m_eUserCommentCharacterCode = UCCCT_JIS;
                else if ( MemCmp(aucCharacterCode,gs_acCharacterCodeUnicode,8) == 0 )
                    m_eUserCommentCharacterCode = UCCCT_UNICODE;
                else if ( MemCmp(aucCharacterCode,gs_acCharacterCodeUndefined,8) == 0 )
                    m_eUserCommentCharacterCode = UCCCT_UNDEFINED;
                else
                    PROCEED_TAG_AS_INCORRECT(m_eUserCommentTagPresence,MarkerSkipping1);
                m_qUserComment.SetQuantity(iCount-8);
                RETURN_ON_FAIL(pqValueStream->Read(m_qUserComment.GetQuantity(),m_qUserComment));

                m_eUserCommentTagPresence = TPT_PRESENT;
            }
            else if ( usTagId == 37520 )
            {
                // DateTime subseconds
                CONDITIONAL_PROCEED_TAG_AS_INCORRECT(( eType == EXIFT_ASCII && iCount >= 0 ),
                    m_eSubsecTimeTagPresence,MarkerSkipping1);

                m_qSubsecTime.SetLength(( iCount != 0 ? iCount - 1 : 0 ));
                if ( bValueFitsFourBytes )
                    RETURN_ON_FAIL(qExifIFDStream.Read(iCount,m_qSubsecTime))
                else
                    RETURN_ON_FAIL(pqValueStream->Read(iCount,m_qSubsecTime));

                if ( ms_bTrimTexts )
                    m_qSubsecTime.Trim();

                m_eSubsecTimeTagPresence = TPT_PRESENT;
            }
            else if ( usTagId == 37521 )
            {
                // DateTimeOriginal subseconds
                CONDITIONAL_PROCEED_TAG_AS_INCORRECT(( eType == EXIFT_ASCII && iCount >= 0 ),
                    m_eSubsecTimeOriginalTagPresence,MarkerSkipping1);

                m_qSubsecTimeOriginal.SetLength(( iCount != 0 ? iCount - 1 : 0 ));
                if ( bValueFitsFourBytes )
                    RETURN_ON_FAIL(qExifIFDStream.Read(iCount,m_qSubsecTimeOriginal))
                else
                    RETURN_ON_FAIL(pqValueStream->Read(iCount,m_qSubsecTimeOriginal));

                if ( ms_bTrimTexts )
                    m_qSubsecTimeOriginal.Trim();

                m_eSubsecTimeOriginalTagPresence = TPT_PRESENT;
            }
            else if ( usTagId == 37522 )
            {
                // DateTimeDigitized subseconds
                CONDITIONAL_PROCEED_TAG_AS_INCORRECT(( eType == EXIFT_ASCII && iCount >= 0 ),
                    m_eSubsecTimeDigitizedTagPresence,MarkerSkipping1);

                m_qSubsecTimeDigitized.SetLength(( iCount != 0 ? iCount - 1 : 0 ));
                if ( bValueFitsFourBytes )
                    RETURN_ON_FAIL(qExifIFDStream.Read(iCount,m_qSubsecTimeDigitized))
                else
                    RETURN_ON_FAIL(pqValueStream->Read(iCount,m_qSubsecTimeDigitized));

                if ( ms_bTrimTexts )
                    m_qSubsecTimeDigitized.Trim();

                m_eSubsecTimeDigitizedTagPresence = TPT_PRESENT;
            }
            else if ( usTagId == 40960 )
            {
                // supported Flashpix version
                CONDITIONAL_PROCEED_TAG_AS_INCORRECT(( eType == EXIFT_UNDEFINED && iCount == 4 ),
                    m_eFlashpixVersionTagPresence,MarkerSkipping1);

                unsigned char aucByte[4];
                RETURN_ON_FAIL(qExifIFDStream.Read(4,aucByte));

                if ( aucByte[0] == '0' && aucByte[1] == '1' && aucByte[2] == '0' && aucByte[3] == '0' )
                    m_eFlashpixVersion = FVT_1_0;
                else
                    m_eFlashpixVersion = FVT_OTHER;

                m_eFlashpixVersionTagPresence = TPT_PRESENT;
            }
            else if ( usTagId == 40961 )
            {
                // color space information
                CONDITIONAL_PROCEED_TAG_AS_INCORRECT(( eType == EXIFT_SHORT && iCount == 1 ),
                    m_eColorSpaceTagPresence,MarkerSkipping1);

                RETURN_ON_FAIL(qExifIFDStream.Read(usValue));
                switch ( usValue )
                {
                case 1:
                    m_eColorSpace = CSTT_sRGB;
                    break;
                case 65535:
                    m_eColorSpace = CSTT_UNCALIBRATED;
                    break;
                default:
                    PROCEED_TAG_AS_INCORRECT(m_eColorSpaceTagPresence,MarkerSkipping1);
                }

                m_eColorSpaceTagPresence = TPT_PRESENT;
            }
            else if ( usTagId == 40962 )
            {
                // valid image width
                CONDITIONAL_PROCEED_TAG_AS_INCORRECT(
                    ( (eType == EXIFT_SHORT || eType == EXIFT_LONG) && iCount == 1 ),m_ePixelXDimensionTagPresence,
                    MarkerSkipping1);

                if ( eType == EXIFT_SHORT )
                {
                    RETURN_ON_FAIL(qExifIFDStream.Read(usValue));
                    m_iPixelXDimension = usValue;
                }
                else  // eType = EXIFT_LONG
                {
                    RETURN_ON_FAIL(qExifIFDStream.Read(m_iPixelXDimension));
                }

                m_ePixelXDimensionTagPresence = TPT_PRESENT;
            }
            else if ( usTagId == 40963 )
            {
                // valid image height
                CONDITIONAL_PROCEED_TAG_AS_INCORRECT(
                    ( (eType == EXIFT_SHORT || eType == EXIFT_LONG) && iCount == 1 ),m_ePixelYDimensionTagPresence,
                    MarkerSkipping1);

                if ( eType == EXIFT_SHORT )
                {
                    RETURN_ON_FAIL(qExifIFDStream.Read(usValue));
                    m_iPixelYDimension = usValue;
                }
                else  // eType = EXIFT_LONG
                {
                    RETURN_ON_FAIL(qExifIFDStream.Read(m_iPixelYDimension));
                }

                m_ePixelYDimensionTagPresence = TPT_PRESENT;
            }
            else if ( usTagId == 40964 )
            {
                // related audio file
                CONDITIONAL_PROCEED_TAG_AS_INCORRECT(( eType == EXIFT_ASCII && iCount == 13 ),
                    m_eRelatedSoundFileTagPresence,MarkerSkipping1);

                m_qRelatedSoundFile.SetLength(12);
                RETURN_ON_FAIL(pqValueStream->Read(13,m_qRelatedSoundFile));
                if ( ms_bTrimTexts )
                    m_qRelatedSoundFile.Trim();

                m_eRelatedSoundFileTagPresence = TPT_PRESENT;
            }
            else if ( usTagId == 41483 )
            {
                // flash energy
                CONDITIONAL_PROCEED_TAG_AS_INCORRECT(( eType == EXIFT_RATIONAL && iCount == 1 ),
                    m_eFlashEnergyTagPresence,MarkerSkipping1);

                RETURN_ON_FAIL(pqValueStream->Read(iValue0));
                RETURN_ON_FAIL(pqValueStream->Read(iValue1));
                m_fFlashEnergy = ((float)iValue0)/iValue1;

                m_eFlashEnergyTagPresence = TPT_PRESENT;
            }
            else if ( usTagId == 41484 )
            {
                // spatial frequency response
                CONDITIONAL_PROCEED_TAG_AS_INCORRECT(( eType == EXIFT_UNDEFINED && iCount >= 0 ),
                    m_eSpatialFrequencyResponseTagPresence,MarkerSkipping1);

                m_qSpatialFrequencyResponse.SetQuantity(iCount);
                if ( bValueFitsFourBytes )
                    RETURN_ON_FAIL(qExifIFDStream.Read(iCount,m_qSpatialFrequencyResponse))
                else
                    RETURN_ON_FAIL(pqValueStream->Read(iCount,m_qSpatialFrequencyResponse));

                m_eSpatialFrequencyResponseTagPresence = TPT_PRESENT;
            }
            else if ( usTagId == 41486 )
            {
                // focal plane X resolution
                CONDITIONAL_PROCEED_TAG_AS_INCORRECT(( eType == EXIFT_RATIONAL && iCount == 1 ),
                    m_eFocalPlaneXResolutionTagPresence,MarkerSkipping1);

                RETURN_ON_FAIL(pqValueStream->Read(iValue0));
                RETURN_ON_FAIL(pqValueStream->Read(iValue1));
                m_fFocalPlaneXResolution = ((float)iValue0)/iValue1;

                m_eFocalPlaneXResolutionTagPresence = TPT_PRESENT;
            }
            else if ( usTagId == 41487 )
            {
                // focal plane Y resolution
                CONDITIONAL_PROCEED_TAG_AS_INCORRECT(( eType == EXIFT_RATIONAL && iCount == 1 ),
                    m_eFocalPlaneYResolutionTagPresence,MarkerSkipping1);

                RETURN_ON_FAIL(pqValueStream->Read(iValue0));
                RETURN_ON_FAIL(pqValueStream->Read(iValue1));
                m_fFocalPlaneYResolution = ((float)iValue0)/iValue1;

                m_eFocalPlaneYResolutionTagPresence = TPT_PRESENT;
            }
            else if ( usTagId == 41488 )
            {
                // focal plane resolution unit
                CONDITIONAL_PROCEED_TAG_AS_INCORRECT(( eType == EXIFT_SHORT && iCount == 1 ),
                    m_eFocalPlaneResolutionUnitTagPresence,MarkerSkipping1);

                RETURN_ON_FAIL(qExifIFDStream.Read(usValue));
                switch ( usValue )
                {
                case 2:
                    m_eFocalPlaneResolutionUnit = RUT_INCHES;
                    break;
                case 3:
                    m_eFocalPlaneResolutionUnit = RUT_CENTIMETERS;
                    break;
                default:
                    PROCEED_TAG_AS_INCORRECT(m_eFocalPlaneResolutionUnitTagPresence,MarkerSkipping1);
                }

                m_eFocalPlaneResolutionUnitTagPresence = TPT_PRESENT;
            }
            else if ( usTagId == 41492 )
            {
                // subject location
                CONDITIONAL_PROCEED_TAG_AS_INCORRECT(( eType == EXIFT_SHORT && iCount == 2 ),
                    m_eSubjectLocationTagPresence,MarkerSkipping1);

                RETURN_ON_FAIL(qExifIFDStream.Read(usValue));
                m_iSubjectLocationX = usValue;
                RETURN_ON_FAIL(qExifIFDStream.Read(usValue));
                m_iSubjectLocationY = usValue;

                m_eSubjectLocationTagPresence = TPT_PRESENT;
            }
            else if ( usTagId == 41493 )
            {
                // exposure index
                CONDITIONAL_PROCEED_TAG_AS_INCORRECT(( eType == EXIFT_RATIONAL && iCount == 1 ),
                    m_eExposureIndexTagPresence,MarkerSkipping1);

                RETURN_ON_FAIL(pqValueStream->Read(iValue0));
                RETURN_ON_FAIL(pqValueStream->Read(iValue1));
                m_fExposureIndex = ((float)iValue0)/iValue1;

                m_eExposureIndexTagPresence = TPT_PRESENT;
            }
            else if ( usTagId == 41495 )
            {
                // sensing method
                CONDITIONAL_PROCEED_TAG_AS_INCORRECT(( eType == EXIFT_SHORT && iCount == 1 ),
                    m_eSensingMethodTagPresence,MarkerSkipping1);

                RETURN_ON_FAIL(qExifIFDStream.Read(usValue));
                switch ( usValue )
                {
                case 1:
                    m_eSensingMethod = SMT_NOT_DEFINED;
                    break;
                case 2:
                    m_eSensingMethod = SMT_ONECHIP_COLOR_AREA_SENSOR;
                    break;
                case 3:
                    m_eSensingMethod = SMT_TWOCHIP_COLOR_AREA_SENSOR;
                    break;
                case 4:
                    m_eSensingMethod = SMT_THREECHIP_COLOR_AREA_SENSOR;
                    break;
                case 5:
                    m_eSensingMethod = SMT_COLOR_SEQUENTIAL_AREA_SENSOR;
                    break;
                case 7:
                    m_eSensingMethod = SMT_TRILINEAR_SENSOR;
                    break;
                case 8:
                    m_eSensingMethod = SMT_COLOR_SEQUENTIAL_LINEAR_SENSOR;
                    break;
                default:
                    PROCEED_TAG_AS_INCORRECT(m_eSensingMethodTagPresence,MarkerSkipping1);
                }

                m_eSensingMethodTagPresence = TPT_PRESENT;
            }
            else if ( usTagId == 41728 )
            {
                // file source
                CONDITIONAL_PROCEED_TAG_AS_INCORRECT(( eType == EXIFT_UNDEFINED && iCount == 1 ),
                    m_eFileSourceTagPresence,MarkerSkipping1);

                RETURN_ON_FAIL(qExifIFDStream.Read(ucValue));
                switch ( ucValue )
                {
                case 3:
                    m_eFileSource = FST_DSC;
                    break;
                default:
                    PROCEED_TAG_AS_INCORRECT(m_eFileSourceTagPresence,MarkerSkipping1);
                }

                m_eFileSourceTagPresence = TPT_PRESENT;
            }
            else if ( usTagId == 41729 )
            {
                // scene type
                CONDITIONAL_PROCEED_TAG_AS_INCORRECT(( eType == EXIFT_UNDEFINED && iCount == 1 ),
                    m_eSceneTypeTagPresence,MarkerSkipping1);

                RETURN_ON_FAIL(qExifIFDStream.Read(ucValue));
                switch ( ucValue )
                {
                case 1:
                    m_eSceneType = ST_A_DIRECTLY_PHOTOGRAPHED_IMAGE;
                    break;
                default:
                    PROCEED_TAG_AS_INCORRECT(m_eSceneTypeTagPresence,MarkerSkipping1);
                }

                m_eSceneTypeTagPresence = TPT_PRESENT;
            }
            else if ( usTagId == 41730 )
            {
                // CFA pattern
                CONDITIONAL_PROCEED_TAG_AS_INCORRECT(( eType == EXIFT_UNDEFINED && iCount >= 0 ),
                    m_eCFAPatternTagPresence,MarkerSkipping1);

                m_qCFAPattern.SetQuantity(iCount);
                if ( bValueFitsFourBytes )
                    RETURN_ON_FAIL(qExifIFDStream.Read(iCount,m_qCFAPattern))
                else
                    RETURN_ON_FAIL(pqValueStream->Read(iCount,m_qCFAPattern));

                m_eCFAPatternTagPresence = TPT_PRESENT;
            }
            else if ( usTagId == 41985 )
            {
                // custom image processing
                CONDITIONAL_PROCEED_TAG_AS_INCORRECT(( eType == EXIFT_SHORT && iCount == 1 ),
                    m_eCustomRenderedTagPresence,MarkerSkipping1);

                RETURN_ON_FAIL(qExifIFDStream.Read(usValue));
                switch ( usValue )
                {
                case 0:
                    m_eCustomRendered = CRT_NORMAL_PROCESS;
                    break;
                case 1:
                    m_eCustomRendered = CRT_CUSTOM_PROCESS;
                    break;
                default:
                    PROCEED_TAG_AS_INCORRECT(m_eCustomRenderedTagPresence,MarkerSkipping1);
                }

                m_eCustomRenderedTagPresence = TPT_PRESENT;
            }
            else if ( usTagId == 41986 )
            {
                // exposure mode
                CONDITIONAL_PROCEED_TAG_AS_INCORRECT(( eType == EXIFT_SHORT && iCount == 1 ),
                    m_eExposureModeTagPresence,MarkerSkipping1);

                RETURN_ON_FAIL(qExifIFDStream.Read(usValue));
                switch ( usValue )
                {
                case 0:
                    m_eExposureMode = EMT_AUTO_EXPOSURE;
                    break;
                case 1:
                    m_eExposureMode = EMT_MANUAL_EXPOSURE;
                    break;
                case 2:
                    m_eExposureMode = EMT_AUTO_BRACKET;
                    break;
                default:
                    PROCEED_TAG_AS_INCORRECT(m_eExposureModeTagPresence,MarkerSkipping1);
                }

                m_eExposureModeTagPresence = TPT_PRESENT;
            }
            else if ( usTagId == 41987 )
            {
                // white balance
                CONDITIONAL_PROCEED_TAG_AS_INCORRECT(( eType == EXIFT_SHORT && iCount == 1 ),
                    m_eWhiteBalanceTagPresence,MarkerSkipping1);

                RETURN_ON_FAIL(qExifIFDStream.Read(usValue));
                switch ( usValue )
                {
                case 0:
                    m_eWhiteBalance = WBT_AUTO_WHITE_BALANCE;
                    break;
                case 1:
                    m_eWhiteBalance = WBT_MANUAL_WHITE_BALANCE;
                    break;
                default:
                    PROCEED_TAG_AS_INCORRECT(m_eWhiteBalanceTagPresence,MarkerSkipping1);
                }

                m_eWhiteBalanceTagPresence = TPT_PRESENT;
            }
            else if ( usTagId == 41988 )
            {
                // digital zoom ratio
                CONDITIONAL_PROCEED_TAG_AS_INCORRECT(( eType == EXIFT_RATIONAL && iCount == 1 ),
                    m_eDigitalZoomRatioTagPresence,MarkerSkipping1);

                RETURN_ON_FAIL(pqValueStream->Read(iValue0));
                RETURN_ON_FAIL(pqValueStream->Read(iValue1));
                if ( iValue0 != 0 )
                {
                    m_eDigitalZoomState = DZST_USED;
                    m_fDigitalZoomRatio = ((float)iValue0)/iValue1;
                }
                else
                {
                    m_eDigitalZoomState = DZST_NOT_USED;
                }

                m_eDigitalZoomRatioTagPresence = TPT_PRESENT;
            }
            else if ( usTagId == 41989 )
            {
                // focal length in 35 mm film
                CONDITIONAL_PROCEED_TAG_AS_INCORRECT(( eType == EXIFT_SHORT && iCount == 1 ),
                    m_eFocalLengthIn35mmFilmTagPresence,MarkerSkipping1);

                RETURN_ON_FAIL(qExifIFDStream.Read(usValue));
                if ( usValue != 0 )
                {
                    m_eFocalLengthIn35mmFilmState = FLI35MMFST_KNOWN;
                    m_iFocalLengthIn35mmFilm = usValue;
                }
                else
                {
                    m_eFocalLengthIn35mmFilmState = FLI35MMFST_UNKNOWN;
                }

                m_eFocalLengthIn35mmFilmTagPresence = TPT_PRESENT;
            }
            else if ( usTagId == 41990 )
            {
                // scene capture type
                CONDITIONAL_PROCEED_TAG_AS_INCORRECT(( eType == EXIFT_SHORT && iCount == 1 ),
                    m_eSceneCaptureTypeTagPresence,MarkerSkipping1);

                RETURN_ON_FAIL(qExifIFDStream.Read(usValue));
                switch ( usValue )
                {
                case 0:
                    m_eSceneCaptureType = SCT_STANDARD;
                    break;
                case 1:
                    m_eSceneCaptureType = SCT_LANDSCAPE;
                    break;
                case 2:
                    m_eSceneCaptureType = SCT_PORTRAIT;
                    break;
                case 3:
                    m_eSceneCaptureType = SCT_NIGHT_SCENE;
                    break;
                default:
                    PROCEED_TAG_AS_INCORRECT(m_eSceneCaptureTypeTagPresence,MarkerSkipping1);
                }

                m_eSceneCaptureTypeTagPresence = TPT_PRESENT;
            }
            else if ( usTagId == 41991 )
            {
                // gain control
                CONDITIONAL_PROCEED_TAG_AS_INCORRECT(( eType == EXIFT_SHORT && iCount == 1 ),
                    m_eGainControlTagPresence,MarkerSkipping1);

                RETURN_ON_FAIL(qExifIFDStream.Read(usValue));
                switch ( usValue )
                {
                case 0:
                    m_eGainControl = GCT_NONE;
                    break;
                case 1:
                    m_eGainControl = GCT_LOW_GAIN_UP;
                    break;
                case 2:
                    m_eGainControl = GCT_HIGH_GAIN_UP;
                    break;
                case 3:
                    m_eGainControl = GCT_LOW_GAIN_DOWN;
                    break;
                case 4:
                    m_eGainControl = GCT_HIGH_GAIN_DOWN;
                    break;
                default:
                    PROCEED_TAG_AS_INCORRECT(m_eGainControlTagPresence,MarkerSkipping1);
                }

                m_eGainControlTagPresence = TPT_PRESENT;
            }
            else if ( usTagId == 41992 )
            {
                // contrast
                CONDITIONAL_PROCEED_TAG_AS_INCORRECT(( eType == EXIFT_SHORT && iCount == 1 ),
                    m_eContrastTagPresence,MarkerSkipping1);

                RETURN_ON_FAIL(qExifIFDStream.Read(usValue));
                switch ( usValue )
                {
                case 0:
                    m_eContrast = CT_NORMAL;
                    break;
                case 1:
                    m_eContrast = CT_SOFT;
                    break;
                case 2:
                    m_eContrast = CT_HARD;
                    break;
                default:
                    PROCEED_TAG_AS_INCORRECT(m_eContrastTagPresence,MarkerSkipping1);
                }

                m_eContrastTagPresence = TPT_PRESENT;
            }
            else if ( usTagId == 41993 )
            {
                // saturation
                CONDITIONAL_PROCEED_TAG_AS_INCORRECT(( eType == EXIFT_SHORT && iCount == 1 ),
                    m_eSaturationTagPresence,MarkerSkipping1);

                RETURN_ON_FAIL(qExifIFDStream.Read(usValue));
                switch ( usValue )
                {
                case 0:
                    m_eSaturation = STT_NORMAL;
                    break;
                case 1:
                    m_eSaturation = STT_LOW_SATURATION;
                    break;
                case 2:
                    m_eSaturation = STT_HIGH_SATURATION;
                    break;
                default:
                    PROCEED_TAG_AS_INCORRECT(m_eSaturationTagPresence,MarkerSkipping1);
                }

                m_eSaturationTagPresence = TPT_PRESENT;
            }
            else if ( usTagId == 41994 )
            {
                // sharpness
                CONDITIONAL_PROCEED_TAG_AS_INCORRECT(( eType == EXIFT_SHORT && iCount == 1 ),
                    m_eSharpnessTagPresence,MarkerSkipping1);

                RETURN_ON_FAIL(qExifIFDStream.Read(usValue));
                switch ( usValue )
                {
                case 0:
                    m_eSharpness = STTT_NORMAL;
                    break;
                case 1:
                    m_eSharpness = STTT_SOFT;
                    break;
                case 2:
                    m_eSharpness = STTT_HARD;
                    break;
                default:
                    PROCEED_TAG_AS_INCORRECT(m_eSharpnessTagPresence,MarkerSkipping1);
                }

                m_eSharpnessTagPresence = TPT_PRESENT;
            }
            else if ( usTagId == 41995 )
            {
                // device settings description
                CONDITIONAL_PROCEED_TAG_AS_INCORRECT(( eType == EXIFT_UNDEFINED && iCount >= 0 ),
                    m_eDeviceSettingDescriptionTagPresence,MarkerSkipping1);

                m_qDeviceSettingDescription.SetQuantity(iCount);
                if ( bValueFitsFourBytes )
                    RETURN_ON_FAIL(qExifIFDStream.Read(iCount,m_qDeviceSettingDescription))
                else
                    RETURN_ON_FAIL(pqValueStream->Read(iCount,m_qDeviceSettingDescription));

                m_eDeviceSettingDescriptionTagPresence = TPT_PRESENT;
            }
            else if ( usTagId == 41996 )
            {
                // subject distance range
                CONDITIONAL_PROCEED_TAG_AS_INCORRECT(( eType == EXIFT_SHORT && iCount == 1 ),
                    m_eSubjectDistanceRangeTagPresence,MarkerSkipping1);

                RETURN_ON_FAIL(qExifIFDStream.Read(usValue));
                switch ( usValue )
                {
                case 0:
                    m_eSubjectDistanceRange = SDRT_UNKNOWN;
                    break;
                case 1:
                    m_eSubjectDistanceRange = SDRT_MACRO;
                    break;
                case 2:
                    m_eSubjectDistanceRange = SDRT_CLOSE_VIEW;
                    break;
                case 3:
                    m_eSubjectDistanceRange = SDRT_DISTANT_VIEW;
                    break;
                default:
                    PROCEED_TAG_AS_INCORRECT(m_eSubjectDistanceRangeTagPresence,MarkerSkipping1);
                }

                m_eSubjectDistanceRangeTagPresence = TPT_PRESENT;
            }
            else if ( usTagId == 42016 )
            {
                // unique image ID
                CONDITIONAL_PROCEED_TAG_AS_INCORRECT(( eType == EXIFT_ASCII && iCount == 33 ),
                    m_eImageUniqueIdTagPresence,MarkerSkipping1);

                m_qImageUniqueId.SetLength(32);
                RETURN_ON_FAIL(pqValueStream->Read(33,m_qImageUniqueId));
                if ( ms_bTrimTexts )
                    m_qImageUniqueId.Trim();

                m_eImageUniqueIdTagPresence = TPT_PRESENT;
            }
            else
            {
MarkerSkipping1:
                if ( bValueFitsFourBytes )
                    qFirstIFDStream.SetBytePosition(iBytePosAfterCountTag+iLength);
            }

            AFTER_TAG_VALUE_READ(qExifIFDStream);
        }

        qExifIFDStream.Finalize();
    }

    return true;
}
//------------------------------------------------------------------------------------------------------------------



