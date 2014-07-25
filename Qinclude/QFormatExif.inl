//------------------------------------------------------------------------------------------------------------------
inline void FormatExif::SetImageWidth (int iImageWidth)
{
    m_iImageWidth = iImageWidth;
    m_eImageWidthTagPresence = TPT_PRESENT;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExif::SetImageLength (int iImageLength)
{
    m_iImageLength = iImageLength;
    m_eImageLengthTagPresence = TPT_PRESENT;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExif::SetBitsPerSample (int iBitsPerSample0, int iBitsPerSample1, int iBitsPerSample2)
{
    m_iBitsPerSample0 = iBitsPerSample0;
    m_iBitsPerSample1 = iBitsPerSample1;
    m_iBitsPerSample2 = iBitsPerSample2;

    m_eBitsPerSampleTagPresence = TPT_PRESENT;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExif::SetCompression (CompressionSchemeType eCompression)
{
    m_eCompression = eCompression;
    m_eCompressionTagPresence = TPT_PRESENT;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExif::SetPhotometricInterpretation (PhotometricInterpretationType ePhotometricInterpretation)
{
    m_ePhotometricInterpretation = ePhotometricInterpretation;
    m_ePhotometricInterpretationTagPresence = TPT_PRESENT;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExif::SetOrientation (OrientationType eOrientation)
{
    m_eOrientation = eOrientation;
    m_eOrientationTagPresence = TPT_PRESENT;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExif::SetSamplesPerPixel (int iSamplesPerPixel)
{
    m_iSamplesPerPixel = iSamplesPerPixel;
    m_eSamplesPerPixelTagPresence = TPT_PRESENT;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExif::SetPlanarConfiguration (PlanarConfigurationType ePlanarConfiguration)
{
    m_ePlanarConfiguration = ePlanarConfiguration;
    m_ePlanarConfigurationTagPresence = TPT_PRESENT;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExif::SetYCbCrSubSampling (YCbCrSubSamplingType eYCbCrSubSampling)
{
    m_eYCbCrSubSampling = eYCbCrSubSampling;
    m_eYCbCrSubSamplingTagPresence = TPT_PRESENT;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExif::SetYCbCrPositioning (YCbCrPositioningType eYCbCrPositioning)
{
    m_eYCbCrPositioning = eYCbCrPositioning;
    m_eYCbCrPositioningTagPresence = TPT_PRESENT;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExif::SetXResolution (float fXResolution)
{
    m_fXResolution = fXResolution;
    m_eXResolutionTagPresence = TPT_PRESENT;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExif::SetYResolution (float fYResolution)
{
    m_fYResolution = fYResolution;
    m_eYResolutionTagPresence = TPT_PRESENT;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExif::SetResolutionUnit (ResolutionUnitType eResolutionUnit)
{
    m_eResolutionUnit = eResolutionUnit;
    m_eResolutionUnitTagPresence = TPT_PRESENT;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExif::SetStripOffsets (const StillArray<int>& rqStripOffsets)
{
    m_qStripOffsets = rqStripOffsets;
    m_eStripOffsetsTagPresence = TPT_PRESENT;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExif::SetRowsPerStrip (int iRowsPerStrip)
{
    m_iRowsPerStrip = iRowsPerStrip;
    m_eRowsPerStripTagPresence = TPT_PRESENT;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExif::SetStripByteCounts (const StillArray<int>& rqStripByteCounts)
{
    m_qStripByteCounts = rqStripByteCounts;
    m_eStripByteCountsTagPresence = TPT_PRESENT;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExif::SetJPEGInterchangeFormat (int iJPEGInterchangeFormat)
{
    m_iJPEGInterchangeFormat = iJPEGInterchangeFormat;
    m_eJPEGInterchangeFormatTagPresence = TPT_PRESENT;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExif::SetJPEGInterchangeFormatLength (int iJPEGInterchangeFormatLength)
{
    m_iJPEGInterchangeFormatLength = iJPEGInterchangeFormatLength;
    m_eJPEGInterchangeFormatLengthTagPresence = TPT_PRESENT;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExif::SetTransferFunction (const StillArray<int>& rqTransferFunction)
{
    assert( rqTransferFunction.GetQuantity() == 768 );

    m_qTransferFunction = rqTransferFunction;
    m_eTransferFunctionTagPresence = TPT_PRESENT;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExif::SetWhitePoint (float fWhitePointX, float fWhitePointY)
{
    m_fWhitePointX = fWhitePointX;
    m_fWhitePointY = fWhitePointY;

    m_eWhitePointTagPresence = TPT_PRESENT;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExif::SetPrimaryChromaticities (
    float fPrimaryChromaticityRedX, float fPrimaryChromaticityRedY,
    float fPrimaryChromaticityGreenX, float fPrimaryChromaticityGreenY,
    float fPrimaryChromaticityBlueX, float fPrimaryChromaticityBlueY)
{
    m_fPrimaryChromaticityRedX = fPrimaryChromaticityRedX;
    m_fPrimaryChromaticityRedY = fPrimaryChromaticityRedY;
    m_fPrimaryChromaticityGreenX = fPrimaryChromaticityGreenX;
    m_fPrimaryChromaticityGreenY = fPrimaryChromaticityGreenY;
    m_fPrimaryChromaticityBlueX = fPrimaryChromaticityBlueX;
    m_fPrimaryChromaticityBlueY = fPrimaryChromaticityBlueY;

    m_ePrimaryChromaticitiesTagPresence = TPT_PRESENT;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExif::SetYCbCrCoefficients (float fCoefficientY, float fCoefficientCb, float fCoefficientCr)
{
    m_fCoefficientY = fCoefficientY;
    m_fCoefficientCb = fCoefficientCb;
    m_fCoefficientCr = fCoefficientCr;

    m_eYCbCrCoefficientsTagPresence = TPT_PRESENT;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExif::SetReferenceBlackWhite (
    const Link<float,float>& rqReferenceBlackWhiteComponent0,
    const Link<float,float>& rqReferenceBlackWhiteComponent1,
    const Link<float,float>& rqReferenceBlackWhiteComponent2)
{
    m_qReferenceBlackWhiteComponent0 = rqReferenceBlackWhiteComponent0;
    m_qReferenceBlackWhiteComponent1 = rqReferenceBlackWhiteComponent1;
    m_qReferenceBlackWhiteComponent2 = rqReferenceBlackWhiteComponent2;

    m_eReferenceBlackWhiteTagPresence = TPT_PRESENT;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExif::SetDateTime (const Date& rqDateTime)
{
    m_qDateTime = rqDateTime;
    m_eDateTimeTagPresence = TPT_PRESENT;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExif::SetImageDescription (const String& rqImageDescription)
{
    m_qImageDescription = rqImageDescription;
    m_eImageDescriptionTagPresence = TPT_PRESENT;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExif::SetMake (const String& rqMake)
{
    m_qMake = rqMake;
    m_eMakeTagPresence = TPT_PRESENT;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExif::SetModel (const String& rqModel)
{
    m_qModel = rqModel;
    m_eModelTagPresence = TPT_PRESENT;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExif::SetSoftware (const String& rqSoftware)
{
    m_qSoftware = rqSoftware;
    m_eSoftwareTagPresence = TPT_PRESENT;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExif::SetArtist (const String& rqArtist)
{
    m_qArtist = rqArtist;
    m_eArtistTagPresence = TPT_PRESENT;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExif::SetCopyright (const String& rqPhotographerCopyright, const String& rqEditorCopyright)
{
    m_qPhotographerCopyright = rqPhotographerCopyright;
    m_qEditorCopyright = rqEditorCopyright;

    m_eCopyrightTagPresence = TPT_PRESENT;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExif::SetExifVersion (ExifVersionType eExifVersion)
{
    assert( eExifVersion != EXIFVT_OTHER );

    m_eExifVersion = eExifVersion;
    m_eExifVersionTagPresence = TPT_PRESENT;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExif::SetFlashpixVersion (FlashpixVersionType eFlashpixVersion)
{
    assert( eFlashpixVersion != FVT_OTHER );

    m_eFlashpixVersion = eFlashpixVersion;
    m_eFlashpixVersionTagPresence = TPT_PRESENT;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExif::SetColorSpace (ColorSpaceType eColorSpace)
{
    m_eColorSpace = eColorSpace;
    m_eColorSpaceTagPresence = TPT_PRESENT;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExif::SetPixelXDimension (int iPixelXDimension)
{
    m_iPixelXDimension = iPixelXDimension;
    m_ePixelXDimensionTagPresence = TPT_PRESENT;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExif::SetPixelYDimension (int iPixelYDimension)
{
    m_iPixelYDimension = iPixelYDimension;
    m_ePixelYDimensionTagPresence = TPT_PRESENT;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExif::SetComponentsConfiguration (ComponentsConfigurationType eComponentsConfiguration)
{
    m_eComponentsConfiguration = eComponentsConfiguration;
    m_eComponentsConfigurationTagPresence = TPT_PRESENT;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExif::SetCompressedBitsPerPixel (float fCompressedBitsPerPixel)
{
    m_fCompressedBitsPerPixel = fCompressedBitsPerPixel;
    m_eCompressedBitsPerPixelTagPresence = TPT_PRESENT;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExif::SetMakerNote (const StillArray<char>& rqMakerNote)
{
    m_qMakerNote = rqMakerNote;
    m_eMakerNoteTagPresence = TPT_PRESENT;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExif::SetUserComment (UserCommentCharacterCodeType eUserCommentCharacterCode,
    const StillArray<char>& rqUserComment)
{
    m_eUserCommentCharacterCode = eUserCommentCharacterCode;
    m_qUserComment = rqUserComment;

    m_eUserCommentTagPresence = TPT_PRESENT;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExif::SetRelatedSoundFile (const String& rqRelatedSoundFile)
{
    assert( rqRelatedSoundFile.GetLength() == 12 );

    m_qRelatedSoundFile = rqRelatedSoundFile;
    m_eRelatedSoundFileTagPresence = TPT_PRESENT;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExif::SetDateTimeOriginal (const Date& rqDateTimeOriginal)
{
    m_qDateTimeOriginal = rqDateTimeOriginal;
    m_eDateTimeOriginalTagPresence = TPT_PRESENT;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExif::SetDateTimeDigitized (const Date& rqDateTimeDigitized)
{
    m_qDateTimeDigitized = rqDateTimeDigitized;
    m_eDateTimeDigitizedTagPresence = TPT_PRESENT;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExif::SetSubsecTime (const String& rqSubsecTime)
{
    m_qSubsecTime = rqSubsecTime;
    m_eSubsecTimeTagPresence = TPT_PRESENT;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExif::SetSubsecTimeOriginal (const String& rqSubsecTimeOriginal)
{
    m_qSubsecTimeOriginal = rqSubsecTimeOriginal;
    m_eSubsecTimeOriginalTagPresence = TPT_PRESENT;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExif::SetSubsecTimeDigitized (const String& rqSubsecTimeDigitized)
{
    m_qSubsecTimeDigitized = rqSubsecTimeDigitized;
    m_eSubsecTimeDigitizedTagPresence = TPT_PRESENT;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExif::SetExposureTime (float fExposureTime)
{
    m_fExposureTime = fExposureTime;
    m_eExposureTimeTagPresence = TPT_PRESENT;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExif::SetFNumber (float fFNumber)
{
    m_fFNumber = fFNumber;
    m_eFNumberTagPresence = TPT_PRESENT;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExif::SetExposureProgram (ExposureProgramType eExposureProgram)
{
    m_eExposureProgram = eExposureProgram;
    m_eExposureProgramTagPresence = TPT_PRESENT;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExif::SetSpectralSensitivity (const String& rqSpectralSensitivity)
{
    m_qSpectralSensitivity = rqSpectralSensitivity;
    m_eSpectralSensitivityTagPresence = TPT_PRESENT;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExif::SetISOSpeedRatings (const StillArray<int>& rqISOSpeedRatings)
{
    m_qISOSpeedRatings = rqISOSpeedRatings;
    m_eISOSpeedRatingsTagPresence = TPT_PRESENT;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExif::SetOECF (const StillArray<char>& rqOECF)
{
    m_qOECF = rqOECF;
    m_eOECFTagPresence = TPT_PRESENT;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExif::SetShutterSpeedValue (float fShutterSpeedValue)
{
    m_fShutterSpeedValue = fShutterSpeedValue;
    m_eShutterSpeedValueTagPresence = TPT_PRESENT;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExif::SetApertureValue (float fApertureValue)
{
    m_fApertureValue = fApertureValue;
    m_eApertureValueTagPresence = TPT_PRESENT;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExif::SetBrightnessValue (BrightnessValueStateType eBrightnessValueState, float fBrightnessValue)
{
    m_eBrightnessValueState = eBrightnessValueState;
    m_fBrightnessValue = fBrightnessValue;

    m_eBrightnessValueTagPresence = TPT_PRESENT;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExif::SetExposureBiasValue (float fExposureBiasValue)
{
    m_fExposureBiasValue = fExposureBiasValue;
    m_eExposureBiasValueTagPresence = TPT_PRESENT;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExif::SetMaxApertureValue (float fMaxApertureValue)
{
    m_fMaxApertureValue = fMaxApertureValue;
    m_eMaxApertureValueTagPresence = TPT_PRESENT;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExif::SetSubjectDistance (SubjectDistanceStateType eSubjectDistanceState, float fSubjectDistance)
{
    m_eSubjectDistanceState = eSubjectDistanceState;
    m_fSubjectDistance = fSubjectDistance;

    m_eSubjectDistanceTagPresence = TPT_PRESENT;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExif::SetMeteringMode (MeteringModeType eMeteringMode)
{
    m_eMeteringMode = eMeteringMode;
    m_eMeteringModeTagPresence = TPT_PRESENT;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExif::SetLightSource (LightSourceType eLightSource)
{
    m_eLightSource = eLightSource;
    m_eLightSourceTagPresence = TPT_PRESENT;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExif::SetFlash (FlashType eFlash)
{
    m_eFlash = eFlash;
    m_eFlashTagPresence = TPT_PRESENT;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExif::SetSubjectArea (const StillArray<int>& rqSubjectAreaCoordinates)
{
    assert( 2 <= rqSubjectAreaCoordinates.GetQuantity() && rqSubjectAreaCoordinates.GetQuantity() <= 4 );

    m_qSubjectAreaCoordinates = rqSubjectAreaCoordinates;
    m_eSubjectAreaTagPresence = TPT_PRESENT;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExif::SetFocalLength (float fFocalLength)
{
    m_fFocalLength = fFocalLength;
    m_eFocalLengthTagPresence = TPT_PRESENT;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExif::SetFlashEnergy (float fFlashEnergy)
{
    m_fFlashEnergy = fFlashEnergy;
    m_eFlashEnergyTagPresence = TPT_PRESENT;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExif::SetSpatialFrequencyResponse (const StillArray<char>& rqSpatialFrequencyResponse)
{
    m_qSpatialFrequencyResponse = rqSpatialFrequencyResponse;
    m_eSpatialFrequencyResponseTagPresence = TPT_PRESENT;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExif::SetFocalPlaneXResolution (float fFocalPlaneXResolution)
{
    m_fFocalPlaneXResolution = fFocalPlaneXResolution;
    m_eFocalPlaneXResolutionTagPresence = TPT_PRESENT;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExif::SetFocalPlaneYResolution (float fFocalPlaneYResolution)
{
    m_fFocalPlaneYResolution = fFocalPlaneYResolution;
    m_eFocalPlaneYResolutionTagPresence = TPT_PRESENT;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExif::SetFocalPlaneResolutionUnit (ResolutionUnitType eFocalPlaneResolutionUnit)
{
    m_eFocalPlaneResolutionUnit = eFocalPlaneResolutionUnit;
    m_eFocalPlaneResolutionUnitTagPresence = TPT_PRESENT;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExif::SetSubjectLocation (int iSubjectLocationX, int iSubjectLocationY)
{
    m_iSubjectLocationX = iSubjectLocationX;
    m_iSubjectLocationY = iSubjectLocationY;

    m_eSubjectLocationTagPresence = TPT_PRESENT;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExif::SetExposureIndex (float fExposureIndex)
{
    m_fExposureIndex = fExposureIndex;
    m_eExposureIndexTagPresence = TPT_PRESENT;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExif::SetSensingMethod (SensingMethodType eSensingMethod)
{
    m_eSensingMethod = eSensingMethod;
    m_eSensingMethodTagPresence = TPT_PRESENT;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExif::SetFileSource (FileSourceType eFileSource)
{
    m_eFileSource = eFileSource;
    m_eFileSourceTagPresence = TPT_PRESENT;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExif::SetSceneType (SceneType eSceneType)
{
    m_eSceneType = eSceneType;
    m_eSceneTypeTagPresence = TPT_PRESENT;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExif::SetCFAPattern (const StillArray<char>& rqCFAPattern)
{
    m_qCFAPattern = rqCFAPattern;
    m_eCFAPatternTagPresence = TPT_PRESENT;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExif::SetCustomRendered (CustomRenderedType eCustomRendered)
{
    m_eCustomRendered = eCustomRendered;
    m_eCustomRenderedTagPresence = TPT_PRESENT;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExif::SetExposureMode (ExposureModeType eExposureMode)
{
    m_eExposureMode = eExposureMode;
    m_eExposureModeTagPresence = TPT_PRESENT;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExif::SetWhiteBalance (WhiteBalanceType eWhiteBalance)
{
    m_eWhiteBalance = eWhiteBalance;
    m_eWhiteBalanceTagPresence = TPT_PRESENT;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExif::SetDigitalZoomRatio (DigitalZoomStateType eDigitalZoomState, float fDigitalZoomRatio)
{
    m_eDigitalZoomState = eDigitalZoomState;
    m_fDigitalZoomRatio = fDigitalZoomRatio;

    m_eDigitalZoomRatioTagPresence = TPT_PRESENT;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExif::SetFocalLengthIn35mmFilm (FocalLengthIn35mmFilmStateType eFocalLengthIn35mmFilmState,
    int iFocalLengthIn35mmFilm)
{
    m_eFocalLengthIn35mmFilmState = eFocalLengthIn35mmFilmState;
    m_iFocalLengthIn35mmFilm = iFocalLengthIn35mmFilm;

    m_eFocalLengthIn35mmFilmTagPresence = TPT_PRESENT;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExif::SetSceneCaptureType (SceneCaptureType eSceneCaptureType)
{
    m_eSceneCaptureType = eSceneCaptureType;
    m_eSceneCaptureTypeTagPresence = TPT_PRESENT;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExif::SetGainControl (GainControlType eGainControl)
{
    m_eGainControl = eGainControl;
    m_eGainControlTagPresence = TPT_PRESENT;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExif::SetContrast (ContrastType eContrast)
{
    m_eContrast = eContrast;
    m_eContrastTagPresence = TPT_PRESENT;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExif::SetSaturation (SaturationType eSaturation)
{
    m_eSaturation = eSaturation;
    m_eSaturationTagPresence = TPT_PRESENT;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExif::SetSharpness (SharpnessType eSharpness)
{
    m_eSharpness = eSharpness;
    m_eSharpnessTagPresence = TPT_PRESENT;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExif::SetDeviceSettingDescription (const StillArray<char>& rqDeviceSettingDescription)
{
    m_qDeviceSettingDescription = rqDeviceSettingDescription;
    m_eDeviceSettingDescriptionTagPresence = TPT_PRESENT;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExif::SetSubjectDistanceRange (SubjectDistanceRangeType eSubjectDistanceRange)
{
    m_eSubjectDistanceRange = eSubjectDistanceRange;
    m_eSubjectDistanceRangeTagPresence = TPT_PRESENT;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExif::SetImageUniqueId (const String& rqImageUniqueId)
{
    assert( rqImageUniqueId.GetLength() == 32 );

    m_qImageUniqueId = rqImageUniqueId;
    m_eImageUniqueIdTagPresence = TPT_PRESENT;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExif::RemoveAllFirstSectionTags ()
{
    RemoveImageWidthTag();
    RemoveImageLengthTag();
    RemoveBitsPerSampleTag();
    RemoveCompressionTag();
    RemovePhotometricInterpretationTag();
    RemoveOrientationTag();
    RemoveSamplesPerPixelTag();
    RemovePlanarConfigurationTag();
    RemoveYCbCrSubSamplingTag();
    RemoveYCbCrPositioningTag();
    RemoveXResolutionTag();
    RemoveYResolutionTag();
    RemoveResolutionUnitTag();
    RemoveStripOffsetsTag();
    RemoveRowsPerStripTag();
    RemoveStripByteCountsTag();
    RemoveJPEGInterchangeFormatTag();
    RemoveJPEGInterchangeFormatLengthTag();
    RemoveTransferFunctionTag();
    RemoveWhitePointTag();
    RemovePrimaryChromaticitiesTag();
    RemoveYCbCrCoefficientsTag();
    RemoveReferenceBlackWhiteTag();
    RemoveDateTimeTag();
    RemoveImageDescriptionTag();
    RemoveMakeTag();
    RemoveModelTag();
    RemoveSoftwareTag();
    RemoveArtistTag();
    RemoveCopyrightTag();
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExif::RemoveImageWidthTag ()
{
    m_eImageWidthTagPresence = TPT_ABSENT;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExif::RemoveImageLengthTag ()
{
    m_eImageLengthTagPresence = TPT_ABSENT;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExif::RemoveBitsPerSampleTag ()
{
    m_eBitsPerSampleTagPresence = TPT_ABSENT;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExif::RemoveCompressionTag ()
{
    m_eCompressionTagPresence = TPT_ABSENT;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExif::RemovePhotometricInterpretationTag ()
{
    m_ePhotometricInterpretationTagPresence = TPT_ABSENT;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExif::RemoveOrientationTag ()
{
    m_eOrientationTagPresence = TPT_ABSENT;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExif::RemoveSamplesPerPixelTag ()
{
    m_eSamplesPerPixelTagPresence = TPT_ABSENT;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExif::RemovePlanarConfigurationTag ()
{
    m_ePlanarConfigurationTagPresence = TPT_ABSENT;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExif::RemoveYCbCrSubSamplingTag ()
{
    m_eYCbCrSubSamplingTagPresence = TPT_ABSENT;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExif::RemoveYCbCrPositioningTag ()
{
    m_eYCbCrPositioningTagPresence = TPT_ABSENT;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExif::RemoveXResolutionTag ()
{
    m_eXResolutionTagPresence = TPT_ABSENT;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExif::RemoveYResolutionTag ()
{
    m_eYResolutionTagPresence = TPT_ABSENT;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExif::RemoveResolutionUnitTag ()
{
    m_eResolutionUnitTagPresence = TPT_ABSENT;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExif::RemoveStripOffsetsTag ()
{
    m_eStripOffsetsTagPresence = TPT_ABSENT;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExif::RemoveRowsPerStripTag ()
{
    m_eRowsPerStripTagPresence = TPT_ABSENT;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExif::RemoveStripByteCountsTag ()
{
    m_eStripByteCountsTagPresence = TPT_ABSENT;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExif::RemoveJPEGInterchangeFormatTag ()
{
    m_eJPEGInterchangeFormatTagPresence = TPT_ABSENT;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExif::RemoveJPEGInterchangeFormatLengthTag ()
{
    m_eJPEGInterchangeFormatLengthTagPresence = TPT_ABSENT;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExif::RemoveTransferFunctionTag ()
{
    m_eTransferFunctionTagPresence = TPT_ABSENT;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExif::RemoveWhitePointTag ()
{
    m_eWhitePointTagPresence = TPT_ABSENT;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExif::RemovePrimaryChromaticitiesTag ()
{
    m_ePrimaryChromaticitiesTagPresence = TPT_ABSENT;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExif::RemoveYCbCrCoefficientsTag ()
{
    m_eYCbCrCoefficientsTagPresence = TPT_ABSENT;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExif::RemoveReferenceBlackWhiteTag ()
{
    m_eReferenceBlackWhiteTagPresence = TPT_ABSENT;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExif::RemoveDateTimeTag ()
{
    m_eDateTimeTagPresence = TPT_ABSENT;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExif::RemoveImageDescriptionTag ()
{
    m_eImageDescriptionTagPresence = TPT_ABSENT;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExif::RemoveMakeTag ()
{
    m_eMakeTagPresence = TPT_ABSENT;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExif::RemoveModelTag ()
{
    m_eModelTagPresence = TPT_ABSENT;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExif::RemoveSoftwareTag ()
{
    m_eSoftwareTagPresence = TPT_ABSENT;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExif::RemoveArtistTag ()
{
    m_eArtistTagPresence = TPT_ABSENT;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExif::RemoveCopyrightTag ()
{
    m_eCopyrightTagPresence = TPT_ABSENT;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExif::RemoveExifVersionTag ()
{
    m_eExifVersionTagPresence = TPT_ABSENT;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExif::RemoveFlashpixVersionTag ()
{
    m_eFlashpixVersionTagPresence = TPT_ABSENT;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExif::RemoveColorSpaceTag ()
{
    m_eColorSpaceTagPresence = TPT_ABSENT;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExif::RemovePixelXDimensionTag ()
{
    m_ePixelXDimensionTagPresence = TPT_ABSENT;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExif::RemovePixelYDimensionTag ()
{
    m_ePixelYDimensionTagPresence = TPT_ABSENT;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExif::RemoveComponentsConfigurationTag ()
{
    m_eComponentsConfigurationTagPresence = TPT_ABSENT;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExif::RemoveCompressedBitsPerPixelTag ()
{
    m_eCompressedBitsPerPixelTagPresence = TPT_ABSENT;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExif::RemoveMakerNoteTag ()
{
    m_eMakerNoteTagPresence = TPT_ABSENT;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExif::RemoveUserCommentTag ()
{
    m_eUserCommentTagPresence = TPT_ABSENT;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExif::RemoveRelatedSoundFileTag ()
{
    m_eRelatedSoundFileTagPresence = TPT_ABSENT;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExif::RemoveDateTimeOriginalTag ()
{
    m_eDateTimeOriginalTagPresence = TPT_ABSENT;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExif::RemoveDateTimeDigitizedTag ()
{
    m_eDateTimeDigitizedTagPresence = TPT_ABSENT;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExif::RemoveSubsecTimeTag ()
{
    m_eSubsecTimeTagPresence = TPT_ABSENT;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExif::RemoveSubsecTimeOriginalTag ()
{
    m_eSubsecTimeOriginalTagPresence = TPT_ABSENT;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExif::RemoveSubsecTimeDigitizedTag ()
{
    m_eSubsecTimeDigitizedTagPresence = TPT_ABSENT;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExif::RemoveExposureTimeTag ()
{
    m_eExposureTimeTagPresence = TPT_ABSENT;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExif::RemoveFNumberTag ()
{
    m_eFNumberTagPresence = TPT_ABSENT;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExif::RemoveExposureProgramTag ()
{
    m_eExposureProgramTagPresence = TPT_ABSENT;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExif::RemoveSpectralSensitivityTag ()
{
    m_eSpectralSensitivityTagPresence = TPT_ABSENT;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExif::RemoveISOSpeedRatingsTag ()
{
    m_eISOSpeedRatingsTagPresence = TPT_ABSENT;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExif::RemoveOECFTag ()
{
    m_eOECFTagPresence = TPT_ABSENT;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExif::RemoveShutterSpeedValueTag ()
{
    m_eShutterSpeedValueTagPresence = TPT_ABSENT;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExif::RemoveApertureValueTag ()
{
    m_eApertureValueTagPresence = TPT_ABSENT;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExif::RemoveBrightnessValueTag ()
{
    m_eBrightnessValueTagPresence = TPT_ABSENT;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExif::RemoveExposureBiasValueTag ()
{
    m_eExposureBiasValueTagPresence = TPT_ABSENT;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExif::RemoveMaxApertureValueTag ()
{
    m_eMaxApertureValueTagPresence = TPT_ABSENT;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExif::RemoveSubjectDistanceTag ()
{
    m_eSubjectDistanceTagPresence = TPT_ABSENT;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExif::RemoveMeteringModeTag ()
{
    m_eMeteringModeTagPresence = TPT_ABSENT;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExif::RemoveLightSourceTag ()
{
    m_eLightSourceTagPresence = TPT_ABSENT;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExif::RemoveFlashTag ()
{
    m_eFlashTagPresence = TPT_ABSENT;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExif::RemoveSubjectAreaTag ()
{
    m_eSubjectAreaTagPresence = TPT_ABSENT;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExif::RemoveFocalLengthTag ()
{
    m_eFocalLengthTagPresence = TPT_ABSENT;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExif::RemoveFlashEnergyTag ()
{
    m_eFlashEnergyTagPresence = TPT_ABSENT;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExif::RemoveSpatialFrequencyResponseTag ()
{
    m_eSpatialFrequencyResponseTagPresence = TPT_ABSENT;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExif::RemoveFocalPlaneXResolutionTag ()
{
    m_eFocalPlaneXResolutionTagPresence = TPT_ABSENT;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExif::RemoveFocalPlaneYResolutionTag ()
{
    m_eFocalPlaneYResolutionTagPresence = TPT_ABSENT;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExif::RemoveFocalPlaneResolutionUnitTag ()
{
    m_eFocalPlaneResolutionUnitTagPresence = TPT_ABSENT;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExif::RemoveSubjectLocationTag ()
{
    m_eSubjectLocationTagPresence = TPT_ABSENT;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExif::RemoveExposureIndexTag ()
{
    m_eExposureIndexTagPresence = TPT_ABSENT;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExif::RemoveSensingMethodTag ()
{
    m_eSensingMethodTagPresence = TPT_ABSENT;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExif::RemoveFileSourceTag ()
{
    m_eFileSourceTagPresence = TPT_ABSENT;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExif::RemoveSceneTypeTag ()
{
    m_eSceneTypeTagPresence = TPT_ABSENT;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExif::RemoveCFAPatternTag ()
{
    m_eCFAPatternTagPresence = TPT_ABSENT;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExif::RemoveCustomRenderedTag ()
{
    m_eCustomRenderedTagPresence = TPT_ABSENT;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExif::RemoveExposureModeTag ()
{
    m_eExposureModeTagPresence = TPT_ABSENT;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExif::RemoveWhiteBalanceTag ()
{
    m_eWhiteBalanceTagPresence = TPT_ABSENT;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExif::RemoveDigitalZoomRatioTag ()
{
    m_eDigitalZoomRatioTagPresence = TPT_ABSENT;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExif::RemoveFocalLengthIn35mmFilmTag ()
{
    m_eFocalLengthIn35mmFilmTagPresence = TPT_ABSENT;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExif::RemoveSceneCaptureTypeTag ()
{
    m_eSceneCaptureTypeTagPresence = TPT_ABSENT;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExif::RemoveGainControlTag ()
{
    m_eGainControlTagPresence = TPT_ABSENT;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExif::RemoveContrastTag ()
{
    m_eContrastTagPresence = TPT_ABSENT;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExif::RemoveSaturationTag ()
{
    m_eSaturationTagPresence = TPT_ABSENT;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExif::RemoveSharpnessTag ()
{
    m_eSharpnessTagPresence = TPT_ABSENT;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExif::RemoveDeviceSettingDescriptionTag ()
{
    m_eDeviceSettingDescriptionTagPresence = TPT_ABSENT;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExif::RemoveSubjectDistanceRangeTag ()
{
    m_eSubjectDistanceRangeTagPresence = TPT_ABSENT;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExif::RemoveImageUniqueIdTag ()
{
    m_eImageUniqueIdTagPresence = TPT_ABSENT;
}
//------------------------------------------------------------------------------------------------------------------
inline FormatExif::ByteOrderType FormatExif::GetByteOrder () const
{
    return m_eByteOrder;
}
//------------------------------------------------------------------------------------------------------------------
inline FormatExif::TagPresenceType FormatExif::GetImageWidthTagPresence () const
{
    return m_eImageWidthTagPresence;
}
//------------------------------------------------------------------------------------------------------------------
inline int FormatExif::GetImageWidth () const
{
    assert( m_eImageWidthTagPresence == TPT_PRESENT );
    return m_iImageWidth;
}
//------------------------------------------------------------------------------------------------------------------
inline FormatExif::TagPresenceType FormatExif::GetImageLengthTagPresence () const
{
    return m_eImageLengthTagPresence;
}
//------------------------------------------------------------------------------------------------------------------
inline int FormatExif::GetImageLength () const
{
    assert( m_eImageLengthTagPresence == TPT_PRESENT );
    return m_iImageLength;
}
//------------------------------------------------------------------------------------------------------------------
inline FormatExif::TagPresenceType FormatExif::GetBitsPerSampleTagPresence () const
{
    return m_eBitsPerSampleTagPresence;
}
//------------------------------------------------------------------------------------------------------------------
inline int FormatExif::GetBitsPerSample0 () const
{
    assert( m_eBitsPerSampleTagPresence == TPT_PRESENT );
    return m_iBitsPerSample0;
}
//------------------------------------------------------------------------------------------------------------------
inline int FormatExif::GetBitsPerSample1 () const
{
    assert( m_eBitsPerSampleTagPresence == TPT_PRESENT );
    return m_iBitsPerSample1;
}
//------------------------------------------------------------------------------------------------------------------
inline int FormatExif::GetBitsPerSample2 () const
{
    assert( m_eBitsPerSampleTagPresence == TPT_PRESENT );
    return m_iBitsPerSample2;
}
//------------------------------------------------------------------------------------------------------------------
inline FormatExif::TagPresenceType FormatExif::GetCompressionTagPresence () const
{
    return m_eCompressionTagPresence;
}
//------------------------------------------------------------------------------------------------------------------
inline FormatExif::CompressionSchemeType FormatExif::GetCompression () const
{
    assert( m_eCompressionTagPresence == TPT_PRESENT );
    return m_eCompression;
}
//------------------------------------------------------------------------------------------------------------------
inline FormatExif::TagPresenceType FormatExif::GetPhotometricInterpretationTagPresence () const
{
    return m_ePhotometricInterpretationTagPresence;
}
//------------------------------------------------------------------------------------------------------------------
inline FormatExif::PhotometricInterpretationType FormatExif::GetPhotometricInterpretation () const
{
    assert( m_ePhotometricInterpretationTagPresence == TPT_PRESENT );
    return m_ePhotometricInterpretation;
}
//------------------------------------------------------------------------------------------------------------------
inline FormatExif::TagPresenceType FormatExif::GetOrientationTagPresence () const
{
    return m_eOrientationTagPresence;
}
//------------------------------------------------------------------------------------------------------------------
inline FormatExif::OrientationType FormatExif::GetOrientation () const
{
    assert( m_eOrientationTagPresence == TPT_PRESENT );
    return m_eOrientation;
}
//------------------------------------------------------------------------------------------------------------------
inline FormatExif::TagPresenceType FormatExif::GetSamplesPerPixelTagPresence () const
{
    return m_eSamplesPerPixelTagPresence;
}
//------------------------------------------------------------------------------------------------------------------
inline int FormatExif::GetSamplesPerPixel () const
{
    assert( m_eSamplesPerPixelTagPresence == TPT_PRESENT );
    return m_iSamplesPerPixel;
}
//------------------------------------------------------------------------------------------------------------------
inline FormatExif::TagPresenceType FormatExif::GetPlanarConfigurationTagPresence () const
{
    return m_ePlanarConfigurationTagPresence;
}
//------------------------------------------------------------------------------------------------------------------
inline FormatExif::PlanarConfigurationType FormatExif::GetPlanarConfiguration () const
{
    assert( m_ePlanarConfigurationTagPresence == TPT_PRESENT );
    return m_ePlanarConfiguration;
}
//------------------------------------------------------------------------------------------------------------------
inline FormatExif::TagPresenceType FormatExif::GetYCbCrSubSamplingTagPresence () const
{
    return m_eYCbCrSubSamplingTagPresence;
}
//------------------------------------------------------------------------------------------------------------------
inline FormatExif::YCbCrSubSamplingType FormatExif::GetYCbCrSubSampling () const
{
    assert( m_eYCbCrSubSamplingTagPresence == TPT_PRESENT );
    return m_eYCbCrSubSampling;
}
//------------------------------------------------------------------------------------------------------------------
inline FormatExif::TagPresenceType FormatExif::GetYCbCrPositioningTagPresence () const
{
    return m_eYCbCrPositioningTagPresence;
}
//------------------------------------------------------------------------------------------------------------------
inline FormatExif::YCbCrPositioningType FormatExif::GetYCbCrPositioning () const
{
    assert( m_eYCbCrPositioningTagPresence == TPT_PRESENT );
    return m_eYCbCrPositioning;
}
//------------------------------------------------------------------------------------------------------------------
inline FormatExif::TagPresenceType FormatExif::GetXResolutionTagPresence () const
{
    return m_eXResolutionTagPresence;
}
//------------------------------------------------------------------------------------------------------------------
inline float FormatExif::GetXResolution () const
{
    assert( m_eXResolutionTagPresence == TPT_PRESENT );
    return m_fXResolution;
}
//------------------------------------------------------------------------------------------------------------------
inline FormatExif::TagPresenceType FormatExif::GetYResolutionTagPresence () const
{
    return m_eYResolutionTagPresence;
}
//------------------------------------------------------------------------------------------------------------------
inline float FormatExif::GetYResolution () const
{
    assert( m_eYResolutionTagPresence == TPT_PRESENT );
    return m_fYResolution;
}
//------------------------------------------------------------------------------------------------------------------
inline FormatExif::TagPresenceType FormatExif::GetResolutionUnitTagPresence () const
{
    return m_eResolutionUnitTagPresence;
}
//------------------------------------------------------------------------------------------------------------------
inline FormatExif::ResolutionUnitType FormatExif::GetResolutionUnit () const
{
    assert( m_eResolutionUnitTagPresence == TPT_PRESENT );
    return m_eResolutionUnit;
}
//------------------------------------------------------------------------------------------------------------------
inline FormatExif::TagPresenceType FormatExif::GetStripOffsetsTagPresence () const
{
    return m_eStripOffsetsTagPresence;
}
//------------------------------------------------------------------------------------------------------------------
inline const StillArray<int>& FormatExif::GetStripOffsets () const
{
    assert( m_eStripOffsetsTagPresence == TPT_PRESENT );
    return m_qStripOffsets;
}
//------------------------------------------------------------------------------------------------------------------
inline FormatExif::TagPresenceType FormatExif::GetRowsPerStripTagPresence () const
{
    return m_eRowsPerStripTagPresence;
}
//------------------------------------------------------------------------------------------------------------------
inline int FormatExif::GetRowsPerStrip () const
{
    assert( m_eRowsPerStripTagPresence == TPT_PRESENT );
    return m_iRowsPerStrip;
}
//------------------------------------------------------------------------------------------------------------------
inline FormatExif::TagPresenceType FormatExif::GetStripByteCountsTagPresence () const
{
    return m_eStripByteCountsTagPresence;
}
//------------------------------------------------------------------------------------------------------------------
inline const StillArray<int>& FormatExif::GetStripByteCounts () const
{
    assert( m_eStripByteCountsTagPresence == TPT_PRESENT );
    return m_qStripByteCounts;
}
//------------------------------------------------------------------------------------------------------------------
inline FormatExif::TagPresenceType FormatExif::GetJPEGInterchangeFormatTagPresence () const
{
    return m_eJPEGInterchangeFormatTagPresence;
}
//------------------------------------------------------------------------------------------------------------------
inline int FormatExif::GetJPEGInterchangeFormat () const
{
    assert( m_eJPEGInterchangeFormatTagPresence == TPT_PRESENT );
    return m_iJPEGInterchangeFormat;
}
//------------------------------------------------------------------------------------------------------------------
inline FormatExif::TagPresenceType FormatExif::GetJPEGInterchangeFormatLengthTagPresence () const
{
    return m_eJPEGInterchangeFormatLengthTagPresence;
}
//------------------------------------------------------------------------------------------------------------------
inline int FormatExif::GetJPEGInterchangeFormatLength () const
{
    assert( m_eJPEGInterchangeFormatLengthTagPresence == TPT_PRESENT );
    return m_iJPEGInterchangeFormatLength;
}
//------------------------------------------------------------------------------------------------------------------
inline FormatExif::TagPresenceType FormatExif::GetTransferFunctionTagPresence () const
{
    return m_eTransferFunctionTagPresence;
}
//------------------------------------------------------------------------------------------------------------------
inline const StillArray<int>& FormatExif::GetTransferFunction () const
{
    assert( m_eTransferFunctionTagPresence == TPT_PRESENT );
    return m_qTransferFunction;
}
//------------------------------------------------------------------------------------------------------------------
inline FormatExif::TagPresenceType FormatExif::GetWhitePointTagPresence () const
{
    return m_eWhitePointTagPresence;
}
//------------------------------------------------------------------------------------------------------------------
inline float FormatExif::GetWhitePointX () const
{
    assert( m_eWhitePointTagPresence == TPT_PRESENT );
    return m_fWhitePointX;
}
//------------------------------------------------------------------------------------------------------------------
inline float FormatExif::GetWhitePointY () const
{
    assert( m_eWhitePointTagPresence == TPT_PRESENT );
    return m_fWhitePointY;
}
//------------------------------------------------------------------------------------------------------------------
inline FormatExif::TagPresenceType FormatExif::GetPrimaryChromaticitiesTagPresence () const
{
    return m_ePrimaryChromaticitiesTagPresence;
}
//------------------------------------------------------------------------------------------------------------------
inline float FormatExif::GetPrimaryChromaticityRedX () const
{
    assert( m_ePrimaryChromaticitiesTagPresence == TPT_PRESENT );
    return m_fPrimaryChromaticityRedX;
}
//------------------------------------------------------------------------------------------------------------------
inline float FormatExif::GetPrimaryChromaticityRedY () const
{
    assert( m_ePrimaryChromaticitiesTagPresence == TPT_PRESENT );
    return m_fPrimaryChromaticityRedY;
}
//------------------------------------------------------------------------------------------------------------------
inline float FormatExif::GetPrimaryChromaticityGreenX () const
{
    assert( m_ePrimaryChromaticitiesTagPresence == TPT_PRESENT );
    return m_fPrimaryChromaticityGreenX;
}
//------------------------------------------------------------------------------------------------------------------
inline float FormatExif::GetPrimaryChromaticityGreenY () const
{
    assert( m_ePrimaryChromaticitiesTagPresence == TPT_PRESENT );
    return m_fPrimaryChromaticityGreenY;
}
//------------------------------------------------------------------------------------------------------------------
inline float FormatExif::GetPrimaryChromaticityBlueX () const
{
    assert( m_ePrimaryChromaticitiesTagPresence == TPT_PRESENT );
    return m_fPrimaryChromaticityBlueX;
}
//------------------------------------------------------------------------------------------------------------------
inline float FormatExif::GetPrimaryChromaticityBlueY () const
{
    assert( m_ePrimaryChromaticitiesTagPresence == TPT_PRESENT );
    return m_fPrimaryChromaticityBlueY;
}
//------------------------------------------------------------------------------------------------------------------
inline FormatExif::TagPresenceType FormatExif::GetYCbCrCoefficientsTagPresence () const
{
    return m_eYCbCrCoefficientsTagPresence;
}
//------------------------------------------------------------------------------------------------------------------
inline float FormatExif::GetCoefficientY () const
{
    assert( m_eYCbCrCoefficientsTagPresence == TPT_PRESENT );
    return m_fCoefficientY;
}
//------------------------------------------------------------------------------------------------------------------
inline float FormatExif::GetCoefficientCb () const
{
    assert( m_eYCbCrCoefficientsTagPresence == TPT_PRESENT );
    return m_fCoefficientCb;
}
//------------------------------------------------------------------------------------------------------------------
inline float FormatExif::GetCoefficientCr () const
{
    assert( m_eYCbCrCoefficientsTagPresence == TPT_PRESENT );
    return m_fCoefficientCr;
}
//------------------------------------------------------------------------------------------------------------------
inline FormatExif::TagPresenceType FormatExif::GetReferenceBlackWhiteTagPresence () const
{
    return m_eReferenceBlackWhiteTagPresence;
}
//------------------------------------------------------------------------------------------------------------------
inline const Link<float,float>& FormatExif::GetReferenceBlackWhiteComponent0 () const
{
    assert( m_eReferenceBlackWhiteTagPresence == TPT_PRESENT );
    return m_qReferenceBlackWhiteComponent0;
}
//------------------------------------------------------------------------------------------------------------------
inline const Link<float,float>& FormatExif::GetReferenceBlackWhiteComponent1 () const
{
    assert( m_eReferenceBlackWhiteTagPresence == TPT_PRESENT );
    return m_qReferenceBlackWhiteComponent1;
}
//------------------------------------------------------------------------------------------------------------------
inline const Link<float,float>& FormatExif::GetReferenceBlackWhiteComponent2 () const
{
    assert( m_eReferenceBlackWhiteTagPresence == TPT_PRESENT );
    return m_qReferenceBlackWhiteComponent2;
}
//------------------------------------------------------------------------------------------------------------------
inline FormatExif::TagPresenceType FormatExif::GetDateTimeTagPresence () const
{
    return m_eDateTimeTagPresence;
}
//------------------------------------------------------------------------------------------------------------------
inline const Date& FormatExif::GetDateTime () const
{
    assert( m_eDateTimeTagPresence == TPT_PRESENT );
    return m_qDateTime;
}
//------------------------------------------------------------------------------------------------------------------
inline FormatExif::TagPresenceType FormatExif::GetImageDescriptionTagPresence () const
{
    return m_eImageDescriptionTagPresence;
}
//------------------------------------------------------------------------------------------------------------------
inline const String& FormatExif::GetImageDescription () const
{
    assert( m_eImageDescriptionTagPresence == TPT_PRESENT );
    return m_qImageDescription;
}
//------------------------------------------------------------------------------------------------------------------
inline FormatExif::TagPresenceType FormatExif::GetMakeTagPresence () const
{
    return m_eMakeTagPresence;
}
//------------------------------------------------------------------------------------------------------------------
inline const String& FormatExif::GetMake () const
{
    assert( m_eMakeTagPresence == TPT_PRESENT );
    return m_qMake;
}
//------------------------------------------------------------------------------------------------------------------
inline FormatExif::TagPresenceType FormatExif::GetModelTagPresence () const
{
    return m_eModelTagPresence;
}
//------------------------------------------------------------------------------------------------------------------
inline const String& FormatExif::GetModel () const
{
    assert( m_eModelTagPresence == TPT_PRESENT );
    return m_qModel;
}
//------------------------------------------------------------------------------------------------------------------
inline FormatExif::TagPresenceType FormatExif::GetSoftwareTagPresence () const
{
    return m_eSoftwareTagPresence;
}
//------------------------------------------------------------------------------------------------------------------
inline const String& FormatExif::GetSoftware () const
{
    assert( m_eSoftwareTagPresence == TPT_PRESENT );
    return m_qSoftware;
}
//------------------------------------------------------------------------------------------------------------------
inline FormatExif::TagPresenceType FormatExif::GetArtistTagPresence () const
{
    return m_eArtistTagPresence;
}
//------------------------------------------------------------------------------------------------------------------
inline const String& FormatExif::GetArtist () const
{
    assert( m_eArtistTagPresence == TPT_PRESENT );
    return m_qArtist;
}
//------------------------------------------------------------------------------------------------------------------
inline FormatExif::TagPresenceType FormatExif::GetCopyrightTagPresence () const
{
    return m_eCopyrightTagPresence;
}
//------------------------------------------------------------------------------------------------------------------
inline bool FormatExif::HasPhotographerCopyright () const
{
    assert( m_eCopyrightTagPresence == TPT_PRESENT );
    return m_bPhotographerCopyrightIsPresent;
}
//------------------------------------------------------------------------------------------------------------------
inline bool FormatExif::HasEditorCopyright () const
{
    assert( m_eCopyrightTagPresence == TPT_PRESENT );
    return m_bEditorCopyrightIsPresent;
}
//------------------------------------------------------------------------------------------------------------------
inline const String& FormatExif::GetPhotographerCopyright () const
{
    assert( m_eCopyrightTagPresence == TPT_PRESENT );
    assert( m_bPhotographerCopyrightIsPresent );

    return m_qPhotographerCopyright;
}
//------------------------------------------------------------------------------------------------------------------
inline const String& FormatExif::GetEditorCopyright () const
{
    assert( m_eCopyrightTagPresence == TPT_PRESENT );
    assert( m_bEditorCopyrightIsPresent );

    return m_qEditorCopyright;
}
//------------------------------------------------------------------------------------------------------------------
inline FormatExif::TagPresenceType FormatExif::GetExifIFDPointerTagPresence () const
{
    return m_eExifIFDPointerTagPresence;
}
//------------------------------------------------------------------------------------------------------------------
inline FormatExif::TagPresenceType FormatExif::GetExifVersionTagPresence () const
{
    assert( m_eExifIFDPointerTagPresence == TPT_PRESENT );
    return m_eExifVersionTagPresence;
}
//------------------------------------------------------------------------------------------------------------------
inline FormatExif::ExifVersionType FormatExif::GetExifVersion () const
{
    assert( m_eExifIFDPointerTagPresence == TPT_PRESENT );
    assert( m_eExifVersionTagPresence == TPT_PRESENT );

    return m_eExifVersion;
}
//------------------------------------------------------------------------------------------------------------------
inline FormatExif::TagPresenceType FormatExif::GetFlashpixVersionTagPresence () const
{
    assert( m_eExifIFDPointerTagPresence == TPT_PRESENT );
    return m_eFlashpixVersionTagPresence;
}
//------------------------------------------------------------------------------------------------------------------
inline FormatExif::FlashpixVersionType FormatExif::GetFlashpixVersion () const
{
    assert( m_eExifIFDPointerTagPresence == TPT_PRESENT );
    assert( m_eFlashpixVersionTagPresence == TPT_PRESENT );

    return m_eFlashpixVersion;
}
//------------------------------------------------------------------------------------------------------------------
inline FormatExif::TagPresenceType FormatExif::GetColorSpaceTagPresence () const
{
    assert( m_eExifIFDPointerTagPresence == TPT_PRESENT );
    return m_eColorSpaceTagPresence;
}
//------------------------------------------------------------------------------------------------------------------
inline FormatExif::ColorSpaceType FormatExif::GetColorSpace () const
{
    assert( m_eExifIFDPointerTagPresence == TPT_PRESENT );
    assert( m_eColorSpaceTagPresence == TPT_PRESENT );

    return m_eColorSpace;
}
//------------------------------------------------------------------------------------------------------------------
inline FormatExif::TagPresenceType FormatExif::GetPixelXDimensionTagPresence () const
{
    assert( m_eExifIFDPointerTagPresence == TPT_PRESENT );
    return m_ePixelXDimensionTagPresence;
}
//------------------------------------------------------------------------------------------------------------------
inline int FormatExif::GetPixelXDimension () const
{
    assert( m_eExifIFDPointerTagPresence == TPT_PRESENT );
    assert( m_ePixelXDimensionTagPresence == TPT_PRESENT );

    return m_iPixelXDimension;
}
//------------------------------------------------------------------------------------------------------------------
inline FormatExif::TagPresenceType FormatExif::GetPixelYDimensionTagPresence () const
{
    assert( m_eExifIFDPointerTagPresence == TPT_PRESENT );
    return m_ePixelYDimensionTagPresence;
}
//------------------------------------------------------------------------------------------------------------------
inline int FormatExif::GetPixelYDimension () const
{
    assert( m_eExifIFDPointerTagPresence == TPT_PRESENT );
    assert( m_ePixelYDimensionTagPresence == TPT_PRESENT );

    return m_iPixelYDimension;
}
//------------------------------------------------------------------------------------------------------------------
inline FormatExif::TagPresenceType FormatExif::GetComponentsConfigurationTagPresence () const
{
    assert( m_eExifIFDPointerTagPresence == TPT_PRESENT );
    return m_eComponentsConfigurationTagPresence;
}
//------------------------------------------------------------------------------------------------------------------
inline FormatExif::ComponentsConfigurationType FormatExif::GetComponentsConfiguration () const
{
    assert( m_eExifIFDPointerTagPresence == TPT_PRESENT );
    assert( m_eComponentsConfigurationTagPresence == TPT_PRESENT );

    return m_eComponentsConfiguration;
}
//------------------------------------------------------------------------------------------------------------------
inline FormatExif::TagPresenceType FormatExif::GetCompressedBitsPerPixelTagPresence () const
{
    assert( m_eExifIFDPointerTagPresence == TPT_PRESENT );
    return m_eCompressedBitsPerPixelTagPresence;
}
//------------------------------------------------------------------------------------------------------------------
inline float FormatExif::GetCompressedBitsPerPixel () const
{
    assert( m_eExifIFDPointerTagPresence == TPT_PRESENT );
    assert( m_eCompressedBitsPerPixelTagPresence == TPT_PRESENT );

    return m_fCompressedBitsPerPixel;
}
//------------------------------------------------------------------------------------------------------------------
inline FormatExif::TagPresenceType FormatExif::GetMakerNoteTagPresence () const
{
    assert( m_eExifIFDPointerTagPresence == TPT_PRESENT );
    return m_eMakerNoteTagPresence;
}
//------------------------------------------------------------------------------------------------------------------
inline const StillArray<char>& FormatExif::GetMakerNote () const
{
    assert( m_eExifIFDPointerTagPresence == TPT_PRESENT );
    assert( m_eMakerNoteTagPresence == TPT_PRESENT );

    return m_qMakerNote;
}
//------------------------------------------------------------------------------------------------------------------
inline FormatExif::TagPresenceType FormatExif::GetUserCommentTagPresence () const
{
    assert( m_eExifIFDPointerTagPresence == TPT_PRESENT );
    return m_eUserCommentTagPresence;
}
//------------------------------------------------------------------------------------------------------------------
inline FormatExif::UserCommentCharacterCodeType FormatExif::GetUserCommentCharacterCode () const
{
    assert( m_eExifIFDPointerTagPresence == TPT_PRESENT );
    assert( m_eUserCommentTagPresence == TPT_PRESENT );

    return m_eUserCommentCharacterCode;
}
//------------------------------------------------------------------------------------------------------------------
inline const StillArray<char>& FormatExif::GetUserComment () const
{
    assert( m_eExifIFDPointerTagPresence == TPT_PRESENT );
    assert( m_eUserCommentTagPresence == TPT_PRESENT );

    return m_qUserComment;
}
//------------------------------------------------------------------------------------------------------------------
inline FormatExif::TagPresenceType FormatExif::GetRelatedSoundFileTagPresence () const
{
    assert( m_eExifIFDPointerTagPresence == TPT_PRESENT );
    return m_eRelatedSoundFileTagPresence;
}
//------------------------------------------------------------------------------------------------------------------
inline const String& FormatExif::GetRelatedSoundFile () const
{
    assert( m_eExifIFDPointerTagPresence == TPT_PRESENT );
    assert( m_eRelatedSoundFileTagPresence == TPT_PRESENT );

    return m_qRelatedSoundFile;
}
//------------------------------------------------------------------------------------------------------------------
inline FormatExif::TagPresenceType FormatExif::GetDateTimeOriginalTagPresence () const
{
    assert( m_eExifIFDPointerTagPresence == TPT_PRESENT );
    return m_eDateTimeOriginalTagPresence;
}
//------------------------------------------------------------------------------------------------------------------
inline const Date& FormatExif::GetDateTimeOriginal () const
{
    assert( m_eExifIFDPointerTagPresence == TPT_PRESENT );
    assert( m_eDateTimeOriginalTagPresence == TPT_PRESENT );

    return m_qDateTimeOriginal;
}
//------------------------------------------------------------------------------------------------------------------
inline FormatExif::TagPresenceType FormatExif::GetDateTimeDigitizedTagPresence () const
{
    assert( m_eExifIFDPointerTagPresence == TPT_PRESENT );
    return m_eDateTimeDigitizedTagPresence;
}
//------------------------------------------------------------------------------------------------------------------
inline const Date& FormatExif::GetDateTimeDigitized () const
{
    assert( m_eExifIFDPointerTagPresence == TPT_PRESENT );
    assert( m_eDateTimeDigitizedTagPresence == TPT_PRESENT );

    return m_qDateTimeDigitized;
}
//------------------------------------------------------------------------------------------------------------------
inline FormatExif::TagPresenceType FormatExif::GetSubsecTimeTagPresence () const
{
    assert( m_eExifIFDPointerTagPresence == TPT_PRESENT );
    return m_eSubsecTimeTagPresence;
}
//------------------------------------------------------------------------------------------------------------------
inline const String& FormatExif::GetSubsecTime () const
{
    assert( m_eExifIFDPointerTagPresence == TPT_PRESENT );
    assert( m_eSubsecTimeTagPresence == TPT_PRESENT );

    return m_qSubsecTime;
}
//------------------------------------------------------------------------------------------------------------------
inline FormatExif::TagPresenceType FormatExif::GetSubsecTimeOriginalTagPresence () const
{
    assert( m_eExifIFDPointerTagPresence == TPT_PRESENT );
    return m_eSubsecTimeOriginalTagPresence;
}
//------------------------------------------------------------------------------------------------------------------
inline const String& FormatExif::GetSubsecTimeOriginal () const
{
    assert( m_eExifIFDPointerTagPresence == TPT_PRESENT );
    assert( m_eSubsecTimeOriginalTagPresence == TPT_PRESENT );

    return m_qSubsecTimeOriginal;
}
//------------------------------------------------------------------------------------------------------------------
inline FormatExif::TagPresenceType FormatExif::GetSubsecTimeDigitizedTagPresence () const
{
    assert( m_eExifIFDPointerTagPresence == TPT_PRESENT );
    return m_eSubsecTimeDigitizedTagPresence;
}
//------------------------------------------------------------------------------------------------------------------
inline const String& FormatExif::GetSubsecTimeDigitized () const
{
    assert( m_eExifIFDPointerTagPresence == TPT_PRESENT );
    assert( m_eSubsecTimeDigitizedTagPresence == TPT_PRESENT );

    return m_qSubsecTimeDigitized;
}
//------------------------------------------------------------------------------------------------------------------
inline FormatExif::TagPresenceType FormatExif::GetExposureTimeTagPresence () const
{
    assert( m_eExifIFDPointerTagPresence == TPT_PRESENT );
    return m_eExposureTimeTagPresence;
}
//------------------------------------------------------------------------------------------------------------------
inline float FormatExif::GetExposureTime () const
{
    assert( m_eExifIFDPointerTagPresence == TPT_PRESENT );
    assert( m_eExposureTimeTagPresence == TPT_PRESENT );

    return m_fExposureTime;
}
//------------------------------------------------------------------------------------------------------------------
inline FormatExif::TagPresenceType FormatExif::GetFNumberTagPresence () const
{
    assert( m_eExifIFDPointerTagPresence == TPT_PRESENT );
    return m_eFNumberTagPresence;
}
//------------------------------------------------------------------------------------------------------------------
inline float FormatExif::GetFNumber () const
{
    assert( m_eExifIFDPointerTagPresence == TPT_PRESENT );
    assert( m_eFNumberTagPresence == TPT_PRESENT );

    return m_fFNumber;
}
//------------------------------------------------------------------------------------------------------------------
inline FormatExif::TagPresenceType FormatExif::GetExposureProgramTagPresence () const
{
    assert( m_eExifIFDPointerTagPresence == TPT_PRESENT );
    return m_eExposureProgramTagPresence;
}
//------------------------------------------------------------------------------------------------------------------
inline FormatExif::ExposureProgramType FormatExif::GetExposureProgram () const
{
    assert( m_eExifIFDPointerTagPresence == TPT_PRESENT );
    assert( m_eExposureProgramTagPresence == TPT_PRESENT );

    return m_eExposureProgram;
}
//------------------------------------------------------------------------------------------------------------------
inline FormatExif::TagPresenceType FormatExif::GetSpectralSensitivityTagPresence () const
{
    assert( m_eExifIFDPointerTagPresence == TPT_PRESENT );
    return m_eSpectralSensitivityTagPresence;
}
//------------------------------------------------------------------------------------------------------------------
inline const String& FormatExif::GetSpectralSensitivity () const
{
    assert( m_eExifIFDPointerTagPresence == TPT_PRESENT );
    assert( m_eSpectralSensitivityTagPresence == TPT_PRESENT );

    return m_qSpectralSensitivity;
}
//------------------------------------------------------------------------------------------------------------------
inline FormatExif::TagPresenceType FormatExif::GetISOSpeedRatingsTagPresence () const
{
    assert( m_eExifIFDPointerTagPresence == TPT_PRESENT );
    return m_eISOSpeedRatingsTagPresence;
}
//------------------------------------------------------------------------------------------------------------------
inline const StillArray<int>& FormatExif::GetISOSpeedRatings () const
{
    assert( m_eExifIFDPointerTagPresence == TPT_PRESENT );
    assert( m_eISOSpeedRatingsTagPresence == TPT_PRESENT );

    return m_qISOSpeedRatings;
}
//------------------------------------------------------------------------------------------------------------------
inline FormatExif::TagPresenceType FormatExif::GetOECFTagPresence () const
{
    assert( m_eExifIFDPointerTagPresence == TPT_PRESENT );
    return m_eOECFTagPresence;
}
//------------------------------------------------------------------------------------------------------------------
inline const StillArray<char>& FormatExif::GetOECF () const
{
    assert( m_eExifIFDPointerTagPresence == TPT_PRESENT );
    assert( m_eOECFTagPresence == TPT_PRESENT );

    return m_qOECF;
}
//------------------------------------------------------------------------------------------------------------------
inline FormatExif::TagPresenceType FormatExif::GetShutterSpeedValueTagPresence () const
{
    assert( m_eExifIFDPointerTagPresence == TPT_PRESENT );
    return m_eShutterSpeedValueTagPresence;
}
//------------------------------------------------------------------------------------------------------------------
inline float FormatExif::GetShutterSpeedValue () const
{
    assert( m_eExifIFDPointerTagPresence == TPT_PRESENT );
    assert( m_eShutterSpeedValueTagPresence == TPT_PRESENT );

    return m_fShutterSpeedValue;
}
//------------------------------------------------------------------------------------------------------------------
inline FormatExif::TagPresenceType FormatExif::GetApertureValueTagPresence () const
{
    assert( m_eExifIFDPointerTagPresence == TPT_PRESENT );
    return m_eApertureValueTagPresence;
}
//------------------------------------------------------------------------------------------------------------------
inline float FormatExif::GetApertureValue () const
{
    assert( m_eExifIFDPointerTagPresence == TPT_PRESENT );
    assert( m_eApertureValueTagPresence == TPT_PRESENT );

    return m_fApertureValue;
}
//------------------------------------------------------------------------------------------------------------------
inline FormatExif::TagPresenceType FormatExif::GetBrightnessValueTagPresence () const
{
    assert( m_eExifIFDPointerTagPresence == TPT_PRESENT );
    return m_eBrightnessValueTagPresence;
}
//------------------------------------------------------------------------------------------------------------------
inline FormatExif::BrightnessValueStateType FormatExif::GetBrightnessValueState () const
{
    assert( m_eExifIFDPointerTagPresence == TPT_PRESENT );
    assert( m_eBrightnessValueTagPresence == TPT_PRESENT );

    return m_eBrightnessValueState;
}
//------------------------------------------------------------------------------------------------------------------
inline float FormatExif::GetBrightnessValue () const
{
    assert( m_eExifIFDPointerTagPresence == TPT_PRESENT );
    assert( m_eBrightnessValueTagPresence == TPT_PRESENT );

    return m_fBrightnessValue;
}
//------------------------------------------------------------------------------------------------------------------
inline FormatExif::TagPresenceType FormatExif::GetExposureBiasValueTagPresence () const
{
    assert( m_eExifIFDPointerTagPresence == TPT_PRESENT );
    return m_eExposureBiasValueTagPresence;
}
//------------------------------------------------------------------------------------------------------------------
inline float FormatExif::GetExposureBiasValue () const
{
    assert( m_eExifIFDPointerTagPresence == TPT_PRESENT );
    assert( m_eExposureBiasValueTagPresence == TPT_PRESENT );

    return m_fExposureBiasValue;
}
//------------------------------------------------------------------------------------------------------------------
inline FormatExif::TagPresenceType FormatExif::GetMaxApertureValueTagPresence () const
{
    assert( m_eExifIFDPointerTagPresence == TPT_PRESENT );
    return m_eMaxApertureValueTagPresence;
}
//------------------------------------------------------------------------------------------------------------------
inline float FormatExif::GetMaxApertureValue () const
{
    assert( m_eExifIFDPointerTagPresence == TPT_PRESENT );
    assert( m_eMaxApertureValueTagPresence == TPT_PRESENT );

    return m_fMaxApertureValue;
}
//------------------------------------------------------------------------------------------------------------------
inline FormatExif::TagPresenceType FormatExif::GetSubjectDistanceTagPresence () const
{
    assert( m_eExifIFDPointerTagPresence == TPT_PRESENT );
    return m_eSubjectDistanceTagPresence;
}
//------------------------------------------------------------------------------------------------------------------
inline FormatExif::SubjectDistanceStateType FormatExif::GetSubjectDistanceState () const
{
    assert( m_eExifIFDPointerTagPresence == TPT_PRESENT );
    assert( m_eSubjectDistanceTagPresence == TPT_PRESENT );

    return m_eSubjectDistanceState;
}
//------------------------------------------------------------------------------------------------------------------
inline float FormatExif::GetSubjectDistance () const
{
    assert( m_eExifIFDPointerTagPresence == TPT_PRESENT );
    assert( m_eSubjectDistanceTagPresence == TPT_PRESENT );

    return m_fSubjectDistance;
}
//------------------------------------------------------------------------------------------------------------------
inline FormatExif::TagPresenceType FormatExif::GetMeteringModeTagPresence () const
{
    assert( m_eExifIFDPointerTagPresence == TPT_PRESENT );
    return m_eMeteringModeTagPresence;
}
//------------------------------------------------------------------------------------------------------------------
inline FormatExif::MeteringModeType FormatExif::GetMeteringMode () const
{
    assert( m_eExifIFDPointerTagPresence == TPT_PRESENT );
    assert( m_eMeteringModeTagPresence == TPT_PRESENT );

    return m_eMeteringMode;
}
//------------------------------------------------------------------------------------------------------------------
inline FormatExif::TagPresenceType FormatExif::GetLightSourceTagPresence () const
{
    assert( m_eExifIFDPointerTagPresence == TPT_PRESENT );
    return m_eLightSourceTagPresence;
}
//------------------------------------------------------------------------------------------------------------------
inline FormatExif::LightSourceType FormatExif::GetLightSource () const
{
    assert( m_eExifIFDPointerTagPresence == TPT_PRESENT );
    assert( m_eLightSourceTagPresence == TPT_PRESENT );

    return m_eLightSource;
}
//------------------------------------------------------------------------------------------------------------------
inline FormatExif::TagPresenceType FormatExif::GetFlashTagPresence () const
{
    assert( m_eExifIFDPointerTagPresence == TPT_PRESENT );
    return m_eFlashTagPresence;
}
//------------------------------------------------------------------------------------------------------------------
inline FormatExif::FlashType FormatExif::GetFlash () const
{
    assert( m_eExifIFDPointerTagPresence == TPT_PRESENT );
    assert( m_eFlashTagPresence == TPT_PRESENT );

    return m_eFlash;
}
//------------------------------------------------------------------------------------------------------------------
inline FormatExif::TagPresenceType FormatExif::GetSubjectAreaTagPresence () const
{
    assert( m_eExifIFDPointerTagPresence == TPT_PRESENT );
    return m_eSubjectAreaTagPresence;
}
//------------------------------------------------------------------------------------------------------------------
inline const StillArray<int>& FormatExif::GetSubjectAreaCoordinates () const
{
    assert( m_eExifIFDPointerTagPresence == TPT_PRESENT );
    assert( m_eSubjectAreaTagPresence == TPT_PRESENT );

    return m_qSubjectAreaCoordinates;
}
//------------------------------------------------------------------------------------------------------------------
inline FormatExif::TagPresenceType FormatExif::GetFocalLengthTagPresence () const
{
    assert( m_eExifIFDPointerTagPresence == TPT_PRESENT );
    return m_eFocalLengthTagPresence;
}
//------------------------------------------------------------------------------------------------------------------
inline float FormatExif::GetFocalLength () const
{
    assert( m_eExifIFDPointerTagPresence == TPT_PRESENT );
    assert( m_eFocalLengthTagPresence == TPT_PRESENT );

    return m_fFocalLength;
}
//------------------------------------------------------------------------------------------------------------------
inline FormatExif::TagPresenceType FormatExif::GetFlashEnergyTagPresence () const
{
    assert( m_eExifIFDPointerTagPresence == TPT_PRESENT );
    return m_eFlashEnergyTagPresence;
}
//------------------------------------------------------------------------------------------------------------------
inline float FormatExif::GetFlashEnergy () const
{
    assert( m_eExifIFDPointerTagPresence == TPT_PRESENT );
    assert( m_eFlashEnergyTagPresence == TPT_PRESENT );

    return m_fFlashEnergy;
}
//------------------------------------------------------------------------------------------------------------------
inline FormatExif::TagPresenceType FormatExif::GetSpatialFrequencyResponseTagPresence () const
{
    assert( m_eExifIFDPointerTagPresence == TPT_PRESENT );
    return m_eSpatialFrequencyResponseTagPresence;
}
//------------------------------------------------------------------------------------------------------------------
inline const StillArray<char>& FormatExif::GetSpatialFrequencyResponse () const
{
    assert( m_eExifIFDPointerTagPresence == TPT_PRESENT );
    assert( m_eSpatialFrequencyResponseTagPresence == TPT_PRESENT );

    return m_qSpatialFrequencyResponse;
}
//------------------------------------------------------------------------------------------------------------------
inline FormatExif::TagPresenceType FormatExif::GetFocalPlaneXResolutionTagPresence () const
{
    assert( m_eExifIFDPointerTagPresence == TPT_PRESENT );
    return m_eFocalPlaneXResolutionTagPresence;
}
//------------------------------------------------------------------------------------------------------------------
inline float FormatExif::GetFocalPlaneXResolution () const
{
    assert( m_eExifIFDPointerTagPresence == TPT_PRESENT );
    assert( m_eFocalPlaneXResolutionTagPresence == TPT_PRESENT );

    return m_fFocalPlaneXResolution;
}
//------------------------------------------------------------------------------------------------------------------
inline FormatExif::TagPresenceType FormatExif::GetFocalPlaneYResolutionTagPresence () const
{
    assert( m_eExifIFDPointerTagPresence == TPT_PRESENT );
    return m_eFocalPlaneYResolutionTagPresence;
}
//------------------------------------------------------------------------------------------------------------------
inline float FormatExif::GetFocalPlaneYResolution () const
{
    assert( m_eExifIFDPointerTagPresence == TPT_PRESENT );
    assert( m_eFocalPlaneYResolutionTagPresence == TPT_PRESENT );

    return m_fFocalPlaneYResolution;
}
//------------------------------------------------------------------------------------------------------------------
inline FormatExif::TagPresenceType FormatExif::GetFocalPlaneResolutionUnitTagPresence () const
{
    assert( m_eExifIFDPointerTagPresence == TPT_PRESENT );
    return m_eFocalPlaneResolutionUnitTagPresence;
}
//------------------------------------------------------------------------------------------------------------------
inline FormatExif::ResolutionUnitType FormatExif::GetFocalPlaneResolutionUnit () const
{
    assert( m_eExifIFDPointerTagPresence == TPT_PRESENT );
    assert( m_eFocalPlaneResolutionUnitTagPresence == TPT_PRESENT );

    return m_eFocalPlaneResolutionUnit;
}
//------------------------------------------------------------------------------------------------------------------
inline FormatExif::TagPresenceType FormatExif::GetSubjectLocationTagPresence () const
{
    assert( m_eExifIFDPointerTagPresence == TPT_PRESENT );
    return m_eSubjectLocationTagPresence;
}
//------------------------------------------------------------------------------------------------------------------
inline int FormatExif::GetSubjectLocationX () const
{
    assert( m_eExifIFDPointerTagPresence == TPT_PRESENT );
    assert( m_eSubjectLocationTagPresence == TPT_PRESENT );

    return m_iSubjectLocationX;
}
//------------------------------------------------------------------------------------------------------------------
inline int FormatExif::GetSubjectLocationY () const
{
    assert( m_eExifIFDPointerTagPresence == TPT_PRESENT );
    assert( m_eSubjectLocationTagPresence == TPT_PRESENT );

    return m_iSubjectLocationY;
}
//------------------------------------------------------------------------------------------------------------------
inline FormatExif::TagPresenceType FormatExif::GetExposureIndexTagPresence () const
{
    assert( m_eExifIFDPointerTagPresence == TPT_PRESENT );
    return m_eExposureIndexTagPresence;
}
//------------------------------------------------------------------------------------------------------------------
inline float FormatExif::GetExposureIndex () const
{
    assert( m_eExifIFDPointerTagPresence == TPT_PRESENT );
    assert( m_eExposureIndexTagPresence == TPT_PRESENT );

    return m_fExposureIndex;
}
//------------------------------------------------------------------------------------------------------------------
inline FormatExif::TagPresenceType FormatExif::GetSensingMethodTagPresence () const
{
    assert( m_eExifIFDPointerTagPresence == TPT_PRESENT );
    return m_eSensingMethodTagPresence;
}
//------------------------------------------------------------------------------------------------------------------
inline FormatExif::SensingMethodType FormatExif::GetSensingMethod () const
{
    assert( m_eExifIFDPointerTagPresence == TPT_PRESENT );
    assert( m_eSensingMethodTagPresence == TPT_PRESENT );

    return m_eSensingMethod;
}
//------------------------------------------------------------------------------------------------------------------
inline FormatExif::TagPresenceType FormatExif::GetFileSourceTagPresence () const
{
    assert( m_eExifIFDPointerTagPresence == TPT_PRESENT );
    return m_eFileSourceTagPresence;
}
//------------------------------------------------------------------------------------------------------------------
inline FormatExif::FileSourceType FormatExif::GetFileSource () const
{
    assert( m_eExifIFDPointerTagPresence == TPT_PRESENT );
    assert( m_eFileSourceTagPresence == TPT_PRESENT );

    return m_eFileSource;
}
//------------------------------------------------------------------------------------------------------------------
inline FormatExif::TagPresenceType FormatExif::GetSceneTypeTagPresence () const
{
    assert( m_eExifIFDPointerTagPresence == TPT_PRESENT );
    return m_eSceneTypeTagPresence;
}
//------------------------------------------------------------------------------------------------------------------
inline FormatExif::SceneType FormatExif::GetSceneType () const
{
    assert( m_eExifIFDPointerTagPresence == TPT_PRESENT );
    assert( m_eSceneTypeTagPresence == TPT_PRESENT );

    return m_eSceneType;
}
//------------------------------------------------------------------------------------------------------------------
inline FormatExif::TagPresenceType FormatExif::GetCFAPatternTagPresence () const
{
    assert( m_eExifIFDPointerTagPresence == TPT_PRESENT );
    return m_eCFAPatternTagPresence;
}
//------------------------------------------------------------------------------------------------------------------
inline const StillArray<char>& FormatExif::GetCFAPattern () const
{
    assert( m_eExifIFDPointerTagPresence == TPT_PRESENT );
    assert( m_eCFAPatternTagPresence == TPT_PRESENT );

    return m_qCFAPattern;
}
//------------------------------------------------------------------------------------------------------------------
inline FormatExif::TagPresenceType FormatExif::GetCustomRenderedTagPresence () const
{
    assert( m_eExifIFDPointerTagPresence == TPT_PRESENT );
    return m_eCustomRenderedTagPresence;
}
//------------------------------------------------------------------------------------------------------------------
inline FormatExif::CustomRenderedType FormatExif::GetCustomRendered () const
{
    assert( m_eExifIFDPointerTagPresence == TPT_PRESENT );
    assert( m_eCustomRenderedTagPresence == TPT_PRESENT );

    return m_eCustomRendered;
}
//------------------------------------------------------------------------------------------------------------------
inline FormatExif::TagPresenceType FormatExif::GetExposureModeTagPresence () const
{
    assert( m_eExifIFDPointerTagPresence == TPT_PRESENT );
    return m_eExposureModeTagPresence;
}
//------------------------------------------------------------------------------------------------------------------
inline FormatExif::ExposureModeType FormatExif::GetExposureMode () const
{
    assert( m_eExifIFDPointerTagPresence == TPT_PRESENT );
    assert( m_eExposureModeTagPresence == TPT_PRESENT );

    return m_eExposureMode;
}
//------------------------------------------------------------------------------------------------------------------
inline FormatExif::TagPresenceType FormatExif::GetWhiteBalanceTagPresence () const
{
    assert( m_eExifIFDPointerTagPresence == TPT_PRESENT );
    return m_eWhiteBalanceTagPresence;
}
//------------------------------------------------------------------------------------------------------------------
inline FormatExif::WhiteBalanceType FormatExif::GetWhiteBalance () const
{
    assert( m_eExifIFDPointerTagPresence == TPT_PRESENT );
    assert( m_eWhiteBalanceTagPresence == TPT_PRESENT );

    return m_eWhiteBalance;
}
//------------------------------------------------------------------------------------------------------------------
inline FormatExif::TagPresenceType FormatExif::GetDigitalZoomRatioTagPresence () const
{
    assert( m_eExifIFDPointerTagPresence == TPT_PRESENT );
    return m_eDigitalZoomRatioTagPresence;
}
//------------------------------------------------------------------------------------------------------------------
inline FormatExif::DigitalZoomStateType FormatExif::GetDigitalZoomState () const
{
    assert( m_eExifIFDPointerTagPresence == TPT_PRESENT );
    assert( m_eDigitalZoomRatioTagPresence == TPT_PRESENT );

    return m_eDigitalZoomState;
}
//------------------------------------------------------------------------------------------------------------------
inline float FormatExif::GetDigitalZoomRatio () const
{
    assert( m_eExifIFDPointerTagPresence == TPT_PRESENT );
    assert( m_eDigitalZoomRatioTagPresence == TPT_PRESENT );

    return m_fDigitalZoomRatio;
}
//------------------------------------------------------------------------------------------------------------------
inline FormatExif::TagPresenceType FormatExif::GetFocalLengthIn35mmFilmTagPresence () const
{
    assert( m_eExifIFDPointerTagPresence == TPT_PRESENT );
    return m_eFocalLengthIn35mmFilmTagPresence;
}
//------------------------------------------------------------------------------------------------------------------
inline FormatExif::FocalLengthIn35mmFilmStateType FormatExif::GetFocalLengthIn35mmFilmState () const
{
    assert( m_eExifIFDPointerTagPresence == TPT_PRESENT );
    assert( m_eFocalLengthIn35mmFilmTagPresence == TPT_PRESENT );

    return m_eFocalLengthIn35mmFilmState;
}
//------------------------------------------------------------------------------------------------------------------
inline int FormatExif::GetFocalLengthIn35mmFilm () const
{
    assert( m_eExifIFDPointerTagPresence == TPT_PRESENT );
    assert( m_eFocalLengthIn35mmFilmTagPresence == TPT_PRESENT );

    return m_iFocalLengthIn35mmFilm;
}
//------------------------------------------------------------------------------------------------------------------
inline FormatExif::TagPresenceType FormatExif::GetSceneCaptureTypeTagPresence () const
{
    assert( m_eExifIFDPointerTagPresence == TPT_PRESENT );
    return m_eSceneCaptureTypeTagPresence;
}
//------------------------------------------------------------------------------------------------------------------
inline FormatExif::SceneCaptureType FormatExif::GetSceneCaptureType () const
{
    assert( m_eExifIFDPointerTagPresence == TPT_PRESENT );
    assert( m_eSceneCaptureTypeTagPresence == TPT_PRESENT );

    return m_eSceneCaptureType;
}
//------------------------------------------------------------------------------------------------------------------
inline FormatExif::TagPresenceType FormatExif::GetGainControlTagPresence () const
{
    assert( m_eExifIFDPointerTagPresence == TPT_PRESENT );
    return m_eGainControlTagPresence;
}
//------------------------------------------------------------------------------------------------------------------
inline FormatExif::GainControlType FormatExif::GetGainControl () const
{
    assert( m_eExifIFDPointerTagPresence == TPT_PRESENT );
    assert( m_eGainControlTagPresence == TPT_PRESENT );

    return m_eGainControl;
}
//------------------------------------------------------------------------------------------------------------------
inline FormatExif::TagPresenceType FormatExif::GetContrastTagPresence () const
{
    assert( m_eExifIFDPointerTagPresence == TPT_PRESENT );
    return m_eContrastTagPresence;
}
//------------------------------------------------------------------------------------------------------------------
inline FormatExif::ContrastType FormatExif::GetContrast () const
{
    assert( m_eExifIFDPointerTagPresence == TPT_PRESENT );
    assert( m_eContrastTagPresence == TPT_PRESENT );

    return m_eContrast;
}
//------------------------------------------------------------------------------------------------------------------
inline FormatExif::TagPresenceType FormatExif::GetSaturationTagPresence () const
{
    assert( m_eExifIFDPointerTagPresence == TPT_PRESENT );
    return m_eSaturationTagPresence;
}
//------------------------------------------------------------------------------------------------------------------
inline FormatExif::SaturationType FormatExif::GetSaturation () const
{
    assert( m_eExifIFDPointerTagPresence == TPT_PRESENT );
    assert( m_eSaturationTagPresence == TPT_PRESENT );

    return m_eSaturation;
}
//------------------------------------------------------------------------------------------------------------------
inline FormatExif::TagPresenceType FormatExif::GetSharpnessTagPresence () const
{
    assert( m_eExifIFDPointerTagPresence == TPT_PRESENT );
    return m_eSharpnessTagPresence;
}
//------------------------------------------------------------------------------------------------------------------
inline FormatExif::SharpnessType FormatExif::GetSharpness () const
{
    assert( m_eExifIFDPointerTagPresence == TPT_PRESENT );
    assert( m_eSharpnessTagPresence == TPT_PRESENT );

    return m_eSharpness;
}
//------------------------------------------------------------------------------------------------------------------
inline FormatExif::TagPresenceType FormatExif::GetDeviceSettingDescriptionTagPresence () const
{
    assert( m_eExifIFDPointerTagPresence == TPT_PRESENT );
    return m_eDeviceSettingDescriptionTagPresence;
}
//------------------------------------------------------------------------------------------------------------------
inline const StillArray<char>& FormatExif::GetDeviceSettingDescription () const
{
    assert( m_eExifIFDPointerTagPresence == TPT_PRESENT );
    assert( m_eDeviceSettingDescriptionTagPresence == TPT_PRESENT );

    return m_qDeviceSettingDescription;
}
//------------------------------------------------------------------------------------------------------------------
inline FormatExif::TagPresenceType FormatExif::GetSubjectDistanceRangeTagPresence () const
{
    assert( m_eExifIFDPointerTagPresence == TPT_PRESENT );
    return m_eSubjectDistanceRangeTagPresence;
}
//------------------------------------------------------------------------------------------------------------------
inline FormatExif::SubjectDistanceRangeType FormatExif::GetSubjectDistanceRange () const
{
    assert( m_eExifIFDPointerTagPresence == TPT_PRESENT );
    assert( m_eSubjectDistanceRangeTagPresence == TPT_PRESENT );

    return m_eSubjectDistanceRange;
}
//------------------------------------------------------------------------------------------------------------------
inline FormatExif::TagPresenceType FormatExif::GetImageUniqueIdTagPresence () const
{
    assert( m_eExifIFDPointerTagPresence == TPT_PRESENT );
    return m_eImageUniqueIdTagPresence;
}
//------------------------------------------------------------------------------------------------------------------
inline const String& FormatExif::GetImageUniqueId () const
{
    assert( m_eExifIFDPointerTagPresence == TPT_PRESENT );
    assert( m_eImageUniqueIdTagPresence == TPT_PRESENT );

    return m_qImageUniqueId;
}
//------------------------------------------------------------------------------------------------------------------



