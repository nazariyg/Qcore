//------------------------------------------------------------------------------------------------------------------
#define ALL_TAGS_ARE_ABSENT \
    m_eImageWidthTagPresence(TPT_ABSENT), \
    m_eImageLengthTagPresence(TPT_ABSENT), \
    m_eBitsPerSampleTagPresence(TPT_ABSENT), \
    m_eCompressionTagPresence(TPT_ABSENT), \
    m_ePhotometricInterpretationTagPresence(TPT_ABSENT), \
    m_eOrientationTagPresence(TPT_ABSENT), \
    m_eSamplesPerPixelTagPresence(TPT_ABSENT), \
    m_ePlanarConfigurationTagPresence(TPT_ABSENT), \
    m_eYCbCrSubSamplingTagPresence(TPT_ABSENT), \
    m_eYCbCrPositioningTagPresence(TPT_ABSENT), \
    m_eXResolutionTagPresence(TPT_ABSENT), \
    m_eYResolutionTagPresence(TPT_ABSENT), \
    m_eResolutionUnitTagPresence(TPT_ABSENT), \
    m_eStripOffsetsTagPresence(TPT_ABSENT), \
    m_eRowsPerStripTagPresence(TPT_ABSENT), \
    m_eStripByteCountsTagPresence(TPT_ABSENT), \
    m_eJPEGInterchangeFormatTagPresence(TPT_ABSENT), \
    m_eJPEGInterchangeFormatLengthTagPresence(TPT_ABSENT), \
    m_eTransferFunctionTagPresence(TPT_ABSENT), \
    m_eWhitePointTagPresence(TPT_ABSENT), \
    m_ePrimaryChromaticitiesTagPresence(TPT_ABSENT), \
    m_eYCbCrCoefficientsTagPresence(TPT_ABSENT), \
    m_eReferenceBlackWhiteTagPresence(TPT_ABSENT), \
    m_eDateTimeTagPresence(TPT_ABSENT), \
    m_eImageDescriptionTagPresence(TPT_ABSENT), \
    m_eMakeTagPresence(TPT_ABSENT), \
    m_eModelTagPresence(TPT_ABSENT), \
    m_eSoftwareTagPresence(TPT_ABSENT), \
    m_eArtistTagPresence(TPT_ABSENT), \
    m_eCopyrightTagPresence(TPT_ABSENT), \
    m_eExifIFDPointerTagPresence(TPT_ABSENT), \
    m_eExifVersionTagPresence(TPT_ABSENT), \
    m_eFlashpixVersionTagPresence(TPT_ABSENT), \
    m_eColorSpaceTagPresence(TPT_ABSENT), \
    m_ePixelXDimensionTagPresence(TPT_ABSENT), \
    m_ePixelYDimensionTagPresence(TPT_ABSENT), \
    m_eComponentsConfigurationTagPresence(TPT_ABSENT), \
    m_eCompressedBitsPerPixelTagPresence(TPT_ABSENT), \
    m_eMakerNoteTagPresence(TPT_ABSENT), \
    m_eUserCommentTagPresence(TPT_ABSENT), \
    m_eRelatedSoundFileTagPresence(TPT_ABSENT), \
    m_eDateTimeOriginalTagPresence(TPT_ABSENT), \
    m_eDateTimeDigitizedTagPresence(TPT_ABSENT), \
    m_eSubsecTimeTagPresence(TPT_ABSENT), \
    m_eSubsecTimeOriginalTagPresence(TPT_ABSENT), \
    m_eSubsecTimeDigitizedTagPresence(TPT_ABSENT), \
    m_eExposureTimeTagPresence(TPT_ABSENT), \
    m_eFNumberTagPresence(TPT_ABSENT), \
    m_eExposureProgramTagPresence(TPT_ABSENT), \
    m_eSpectralSensitivityTagPresence(TPT_ABSENT), \
    m_eISOSpeedRatingsTagPresence(TPT_ABSENT), \
    m_eOECFTagPresence(TPT_ABSENT), \
    m_eShutterSpeedValueTagPresence(TPT_ABSENT), \
    m_eApertureValueTagPresence(TPT_ABSENT), \
    m_eBrightnessValueTagPresence(TPT_ABSENT), \
    m_eExposureBiasValueTagPresence(TPT_ABSENT), \
    m_eMaxApertureValueTagPresence(TPT_ABSENT), \
    m_eSubjectDistanceTagPresence(TPT_ABSENT), \
    m_eMeteringModeTagPresence(TPT_ABSENT), \
    m_eLightSourceTagPresence(TPT_ABSENT), \
    m_eFlashTagPresence(TPT_ABSENT), \
    m_eSubjectAreaTagPresence(TPT_ABSENT), \
    m_eFocalLengthTagPresence(TPT_ABSENT), \
    m_eFlashEnergyTagPresence(TPT_ABSENT), \
    m_eSpatialFrequencyResponseTagPresence(TPT_ABSENT), \
    m_eFocalPlaneXResolutionTagPresence(TPT_ABSENT), \
    m_eFocalPlaneYResolutionTagPresence(TPT_ABSENT), \
    m_eFocalPlaneResolutionUnitTagPresence(TPT_ABSENT), \
    m_eSubjectLocationTagPresence(TPT_ABSENT), \
    m_eExposureIndexTagPresence(TPT_ABSENT), \
    m_eSensingMethodTagPresence(TPT_ABSENT), \
    m_eFileSourceTagPresence(TPT_ABSENT), \
    m_eSceneTypeTagPresence(TPT_ABSENT), \
    m_eCFAPatternTagPresence(TPT_ABSENT), \
    m_eCustomRenderedTagPresence(TPT_ABSENT), \
    m_eExposureModeTagPresence(TPT_ABSENT), \
    m_eWhiteBalanceTagPresence(TPT_ABSENT), \
    m_eDigitalZoomRatioTagPresence(TPT_ABSENT), \
    m_eFocalLengthIn35mmFilmTagPresence(TPT_ABSENT), \
    m_eSceneCaptureTypeTagPresence(TPT_ABSENT), \
    m_eGainControlTagPresence(TPT_ABSENT), \
    m_eContrastTagPresence(TPT_ABSENT), \
    m_eSaturationTagPresence(TPT_ABSENT), \
    m_eSharpnessTagPresence(TPT_ABSENT), \
    m_eDeviceSettingDescriptionTagPresence(TPT_ABSENT), \
    m_eSubjectDistanceRangeTagPresence(TPT_ABSENT), \
    m_eImageUniqueIdTagPresence(TPT_ABSENT)
//------------------------------------------------------------------------------------------------------------------
#define BEFORE_TAG_VALUE_WRITE(tagid,type,count) \
{ \
	usTagId = tagid; \
	iCount = count; \
    switch ( type ) \
    { \
    case EXIFT_BYTE: \
        usType = 1; \
        iLength = iCount; \
        break; \
    case EXIFT_ASCII: \
        usType = 2; \
        iLength = iCount; \
        break; \
    case EXIFT_SHORT: \
        usType = 3; \
        iLength = 2*iCount; \
        break; \
    case EXIFT_LONG: \
        usType = 4; \
        iLength = 4*iCount; \
        break; \
    case EXIFT_RATIONAL: \
        usType = 5; \
        iLength = 8*iCount; \
        break; \
    case EXIFT_UNDEFINED: \
        usType = 7; \
        iLength = iCount; \
        break; \
    case EXIFT_SLONG: \
        usType = 9; \
        iLength = 4*iCount; \
        break; \
    case EXIFT_SRATIONAL: \
        usType = 10; \
        iLength = 8*iCount; \
        break; \
    } \
    bValueFitsFourBytes = ( iLength <= 4 ); \
	qSaveStream.Write(usTagId); \
	qSaveStream.Write(usType); \
	qSaveStream.Write(iCount); \
    if ( !bValueFitsFourBytes ) \
    { \
        iBytePosF = qSaveStream.GetBytePosition(); \
        qSaveStream.SetBytePosition(iBytePosV); \
    } \
}
//------------------------------------------------------------------------------------------------------------------
#define AFTER_TAG_VALUE_WRITE \
{ \
    if ( bValueFitsFourBytes ) \
    { \
		qSaveStream.OffsetBytePosition(4-iLength); \
    } \
    else \
    { \
        iOldBytePosV = iBytePosV; \
        iBytePosV = qSaveStream.GetBytePosition(); \
        qSaveStream.SetBytePosition(iBytePosF); \
        if ( eExifDataDestination == EDDT_TIFF ) \
			iOldBytePosV += iTiffPreOffset; \
        qSaveStream.Write(iOldBytePosV); \
    } \
}
//------------------------------------------------------------------------------------------------------------------
#define WRITE_SHORT(value) \
{ \
	usValue = value; \
	qSaveStream.Write(usValue); \
}
//------------------------------------------------------------------------------------------------------------------
#define WRITE_RATIONAL(value) \
{ \
    iNumerator = Mathf::RoundToInt(value*ms_iRationalNumbersScaler); \
    qSaveStream.Write(iNumerator); \
    qSaveStream.Write(ms_iRationalNumbersScaler); \
}
//------------------------------------------------------------------------------------------------------------------
#define PROCEED_TAG_AS_INCORRECT(variable,label) \
{ \
    variable = TPT_INCORRECT; \
    goto label; \
}
//------------------------------------------------------------------------------------------------------------------
#define CONDITIONAL_PROCEED_TAG_AS_INCORRECT(expression,variable,label) \
{ \
    if ( !expression ) \
        PROCEED_TAG_AS_INCORRECT(variable,label); \
}
//------------------------------------------------------------------------------------------------------------------
#define BEFORE_TAG_VALUE_READ(stream,label) \
{ \
    RETURN_ON_FAIL(stream.Read(usTagId)); \
    RETURN_ON_FAIL(stream.Read(usType)); \
    RETURN_ON_FAIL(stream.Read(iCount)); \
    iBytePosAfterCountTag = stream.GetBytePosition(); \
    switch ( usType ) \
    { \
    case 1: \
        eType = EXIFT_BYTE; \
        iLength = iCount; \
        break; \
    case 2: \
        eType = EXIFT_ASCII; \
        iLength = iCount; \
        break; \
    case 3: \
        eType = EXIFT_SHORT; \
        iLength = 2*iCount; \
        break; \
    case 4: \
        eType = EXIFT_LONG; \
        iLength = 4*iCount; \
        break; \
    case 5: \
        eType = EXIFT_RATIONAL; \
        iLength = 8*iCount; \
        break; \
    case 7: \
        eType = EXIFT_UNDEFINED; \
        iLength = iCount; \
        break; \
    case 9: \
        eType = EXIFT_SLONG; \
        iLength = 4*iCount; \
        break; \
    case 10: \
        eType = EXIFT_SRATIONAL; \
        iLength = 8*iCount; \
        break; \
    default: \
        iLength = 4; \
        bValueFitsFourBytes = true; \
        goto label; \
    } \
    bValueFitsFourBytes = ( iLength <= 4 ); \
    if ( !bValueFitsFourBytes ) \
    { \
        RETURN_ON_FAIL(stream.Read(iValueOffset)); \
        pqValueStream = new Stream(m_eStreaming,m_acStreamSrcParam,iValueOffset,iLength,bLittleEndian,bSuccess, \
            iBytestreamSize); \
        RETURN_ON_FAIL(bSuccess); \
    } \
}
//------------------------------------------------------------------------------------------------------------------
#define AFTER_TAG_VALUE_READ(stream) \
{ \
    if ( bValueFitsFourBytes ) \
    { \
        stream.OffsetBytePosition(4-iLength); \
    } \
    else \
    { \
        pqValueStream->Finalize(); \
        delete pqValueStream; \
        pqValueStream = 0; \
    } \
}
//------------------------------------------------------------------------------------------------------------------



