//------------------------------------------------------------------------------------------------------------------
#define DEFAULT_INIT_LIST \
    m_fJpegQuality(0.75f), \
    m_bDoNotWriteResolutionInfo(false), \
    m_bNewSubfileTypeFieldIsPresent(false), \
    m_bSubfileIsReducedResolutionVersion(false), \
    m_bSubfileIsSinglePageOfMultipageImage(false), \
    m_bSubfileIsTransparencyMask(false), \
    m_bSubfileTypeFieldIsPresent(false), \
    m_bImageWidthFieldIsPresent(false), \
    m_bImageLengthFieldIsPresent(false), \
    m_bCompressionFieldIsPresent(false), \
    m_eCompression(CT_NONE), \
    m_bPhotometricInterpretationFieldIsPresent(false), \
    m_bThreshholdingFieldIsPresent(false), \
    m_eThreshholding(TT_NO_DITHERING_OR_HALFTONING_APPLIED), \
    m_bCellWidthFieldIsPresent(false), \
    m_bCellLengthFieldIsPresent(false), \
    m_bFillOrderFieldIsPresent(false), \
    m_eFillOrder(FOT_HIGH_ORDER_BIT_FIRST), \
    m_bDocumentNameFieldIsPresent(false), \
    m_bImageDescriptionFieldIsPresent(false), \
    m_bMakeFieldIsPresent(false), \
    m_bModelFieldIsPresent(false), \
    m_bStripOffsetsFieldIsPresent(false), \
    m_bOrientationFieldIsPresent(false), \
    m_eOrientation(OT_TOP_LEFT), \
    m_bSamplesPerPixelFieldIsPresent(false), \
    m_SamplesPerPixel(1), \
    m_bRowsPerStripFieldIsPresent(false), \
    m_RowsPerStrip(4294967295), \
    m_bStripByteCountsFieldIsPresent(false), \
    m_bMinSampleValueFieldIsPresent(false), \
    m_bMaxSampleValueFieldIsPresent(false), \
    m_bXResolutionFieldIsPresent(false), \
    m_bYResolutionFieldIsPresent(false), \
    m_bPlanarConfigurationFieldIsPresent(false), \
    m_ePlanarConfiguration(PCT_CHUNKY_FORMAT), \
    m_bPageNameFieldIsPresent(false), \
    m_bXPositionFieldIsPresent(false), \
    m_bYPositionFieldIsPresent(false), \
    m_bFreeOffsetsFieldIsPresent(false), \
    m_bFreeByteCountsFieldIsPresent(false), \
    m_bGrayResponseUnitFieldIsPresent(false), \
    m_eGrayResponseUnit(GRUT_HUNDREDTHS_OF_UNIT), \
    m_bGrayResponseCurveFieldIsPresent(false), \
    m_bT4OptionsFieldIsPresent(false), \
    m_bT42d(false), \
    m_bT4UncompressedModeUsed(false), \
    m_bT4FillBitsBeforeEolUsed(false), \
    m_bT6OptionsFieldIsPresent(false), \
    m_bT6UncompressedModeAllowed(false), \
    m_bResolutionUnitFieldIsPresent(false), \
    m_eResolutionUnit(RUT_INCH), \
    m_bPageNumberFieldIsPresent(false), \
    m_bTransferFunctionFieldIsPresent(false), \
    m_bSoftwareFieldIsPresent(false), \
    m_bDateTimeFieldIsPresent(false), \
    m_bArtistFieldIsPresent(false), \
    m_bHostComputerFieldIsPresent(false), \
    m_bPredictorFieldIsPresent(false), \
    m_ePredictor(PT_NO_PREDICTION), \
    m_bWhitePointFieldIsPresent(false), \
    m_bPrimaryChromaticitiesFieldIsPresent(false), \
    m_bColorMapFieldIsPresent(false), \
    m_bHalftoneHintsFieldIsPresent(false), \
    m_bTileWidthFieldIsPresent(false), \
    m_bTileLengthFieldIsPresent(false), \
    m_bTileOffsetsFieldIsPresent(false), \
    m_bTileByteCountsFieldIsPresent(false), \
    m_bInkSetFieldIsPresent(false), \
    m_eInkSet(IST_CMYK), \
    m_bInkNamesFieldIsPresent(false), \
    m_bNumberOfInksFieldIsPresent(false), \
    m_NumberOfInks(4), \
    m_bDotRangeFieldIsPresent(false), \
    m_bTargetPrinterFieldIsPresent(false), \
    m_bExtraSamplesFieldIsPresent(false), \
    m_bSampleFormatFieldIsPresent(false), \
    m_bSMinSampleValueFieldIsPresent(false), \
    m_bSMaxSampleValueFieldIsPresent(false), \
    m_bTransferRangeFieldIsPresent(false), \
    m_bJPEGTablesFieldIsPresent(false), \
    m_bYCbCrCoefficientsFieldIsPresent(false), \
    m_fYCbCrLumaRed(0.299f), \
    m_fYCbCrLumaGreen(0.587f), \
    m_fYCbCrLumaBlue(0.114f), \
    m_bYCbCrSubSamplingFieldIsPresent(false), \
    m_eYCbCrSubSamplingHoriz(YCCSST_ONE_TO_TWO), \
    m_eYCbCrSubSamplingVert(YCCSST_ONE_TO_TWO), \
    m_bYCbCrPositioningFieldIsPresent(false), \
    m_eYCbCrPositioning(YCCPT_CENTERED), \
    m_bReferenceBlackWhiteFieldIsPresent(false), \
    m_bCopyrightFieldIsPresent(false)
//------------------------------------------------------------------------------------------------------------------
#define BEFORE_FIELD_VALUE_WRITE \
{ \
    rqSaveStream.Write(usTag); \
    switch ( eType ) \
    { \
    case FT_BYTE: \
        usType = 1; \
        iValueLength = iCount; \
        break; \
    case FT_ASCII: \
        usType = 2; \
        iValueLength = iCount; \
        break; \
    case FT_SHORT: \
        usType = 3; \
        iValueLength = 2*iCount; \
        break; \
    case FT_LONG: \
        usType = 4; \
        iValueLength = 4*iCount; \
        break; \
    case FT_RATIONAL: \
        usType = 5; \
        iValueLength = 8*iCount; \
        break; \
    case FT_SBYTE: \
        usType = 6; \
        iValueLength = iCount; \
        break; \
    case FT_UNDEFINED: \
        usType = 7; \
        iValueLength = iCount; \
        break; \
    case FT_SSHORT: \
        usType = 8; \
        iValueLength = 2*iCount; \
        break; \
    case FT_SLONG: \
        usType = 9; \
        iValueLength = 4*iCount; \
        break; \
    case FT_SRATIONAL: \
        usType = 10; \
        iValueLength = 8*iCount; \
        break; \
    case FT_FLOAT: \
        usType = 11; \
        iValueLength = 4*iCount; \
        break; \
    case FT_DOUBLE: \
        usType = 12; \
        iValueLength = 8*iCount; \
        break; \
    } \
    rqSaveStream.Write(usType); \
    rqSaveStream.Write(iCount); \
    bValueFitsFourBytes = ( iValueLength <= 4 ); \
    if ( !bValueFitsFourBytes ) \
    { \
        rqSaveStream.Write(riSeparatedValuesBytePos); \
        rqSaveStream.PushPositionState(); \
        rqSaveStream.SetBytePosition(riSeparatedValuesBytePos); \
    } \
}
//------------------------------------------------------------------------------------------------------------------
#define AFTER_FIELD_VALUE_WRITE \
{ \
    if ( bValueFitsFourBytes ) \
    { \
        if ( iValueLength != 4 ) \
            rqSaveStream.OffsetBytePosition(4-iValueLength); \
    } \
    else \
    { \
        if ( (rqSaveStream.GetBytePosition() & 1) != 0 ) \
            rqSaveStream.Write(cNull); \
        riSeparatedValuesBytePos = rqSaveStream.GetBytePosition(); \
        rqSaveStream.PopPositionState(); \
    } \
}
//------------------------------------------------------------------------------------------------------------------
#define SKIP_FIELD_WITH_THE_WARNING \
{ \
    THROW_WARNING(WT_ONE_OR_MORE_IFD_FIELDS_SKIPPED); \
    goto MarkerSkipping; \
}
//------------------------------------------------------------------------------------------------------------------
#define CONDITIONAL_SKIP_FIELD_WITH_THE_WARNING(expression) \
{ \
    if ( !bFieldIsOk || !expression ) \
        SKIP_FIELD_WITH_THE_WARNING; \
}
//------------------------------------------------------------------------------------------------------------------
#define SKIP_FIELD_ON_FAIL(expression) \
{ \
    if ( !expression ) \
        SKIP_FIELD_WITH_THE_WARNING; \
}
//------------------------------------------------------------------------------------------------------------------
#define ESCAPE_IFD \
{ \
    delete pqSeparatedValueStream; \
    return false; \
}
//------------------------------------------------------------------------------------------------------------------
#define ESCAPE_IFD_WITH_THE_WARNING \
{ \
    THROW_WARNING(WT_ONE_OR_MORE_SUBFILES_FAILED_TO_LOAD); \
    ESCAPE_IFD; \
}
//------------------------------------------------------------------------------------------------------------------
#define CONDITIONAL_ESCAPE_IFD_WITH_THE_WARNING(expression) \
{ \
    if ( !bFieldIsOk || !expression ) \
        ESCAPE_IFD_WITH_THE_WARNING; \
}
//------------------------------------------------------------------------------------------------------------------
#define ESCAPE_IFD_WITH_WARNING(warning) \
{ \
    THROW_WARNING(warning); \
    ESCAPE_IFD; \
}
//------------------------------------------------------------------------------------------------------------------
#define CONDITIONAL_ESCAPE_IFD_WITH_WARNING(expression,warning) \
{ \
    if ( !expression ) \
        ESCAPE_IFD_WITH_WARNING(warning); \
}
//------------------------------------------------------------------------------------------------------------------
#define ESCAPE_IFD_ON_FAIL(expression) \
{ \
    if ( !expression ) \
        ESCAPE_IFD_WITH_THE_WARNING; \
}
//------------------------------------------------------------------------------------------------------------------
#define CRITICAL_TEST(expression) \
{ \
    if ( !expression ) \
    { \
        THROW_WARNING(WT_ONE_OR_MORE_SUBFILES_FAILED_TO_LOAD); \
        return false; \
    } \
}
//------------------------------------------------------------------------------------------------------------------
#define NON_CRITICAL_TEST(expression,fieldpresence) \
{ \
    if ( !expression ) \
    { \
        THROW_WARNING(WT_ONE_OR_MORE_IFD_FIELDS_SKIPPED); \
        fieldpresence = false; \
    } \
}
//------------------------------------------------------------------------------------------------------------------
#define ESCAPE_ON_FAIL(expression) \
{ \
    if ( !expression ) \
    { \
        assert( false ); \
        return false; \
    } \
}
//------------------------------------------------------------------------------------------------------------------



