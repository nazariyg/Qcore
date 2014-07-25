//------------------------------------------------------------------------------------------------------------------
inline void FormatTiff::ImageFileDirectory::SetCompression (CompressionType eCompression)
{
    m_eCompression = eCompression;
    m_bCompressionFieldIsPresent = true;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatTiff::ImageFileDirectory::SetT42d (bool bT42d)
{
    m_bT42d = bT42d;
    m_bT4OptionsFieldIsPresent = true;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatTiff::ImageFileDirectory::SetJpegQuality (float fQuality)
{
    m_fJpegQuality = fQuality;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatTiff::ImageFileDirectory::SetOrientation (OrientationType eOrientation)
{
    m_eOrientation = eOrientation;
    m_bOrientationFieldIsPresent = true;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatTiff::ImageFileDirectory::SetIsReducedResolutionVersion (bool bItIs)
{
    m_bSubfileIsReducedResolutionVersion = bItIs;
    m_bNewSubfileTypeFieldIsPresent = true;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatTiff::ImageFileDirectory::SetIsSinglePageOfMultipageImage (bool bItIs)
{
    m_bSubfileIsSinglePageOfMultipageImage = bItIs;
    m_bNewSubfileTypeFieldIsPresent = true;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatTiff::ImageFileDirectory::SetXResolution (float fXResolution)
{
    m_fXResolution = fXResolution;
    m_bXResolutionFieldIsPresent = true;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatTiff::ImageFileDirectory::SetYResolution (float fYResolution)
{
    m_fYResolution = fYResolution;
    m_bYResolutionFieldIsPresent = true;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatTiff::ImageFileDirectory::SetResolutionUnit (ResolutionUnitType eResolutionUnit)
{
    m_eResolutionUnit = eResolutionUnit;
    m_bResolutionUnitFieldIsPresent = true;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatTiff::ImageFileDirectory::UnsetResolutionInfo ()
{
    m_bDoNotWriteResolutionInfo = true;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatTiff::ImageFileDirectory::SetXPosition (float fXPosition)
{
    m_fXPosition = fXPosition;
    m_bXPositionFieldIsPresent = true;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatTiff::ImageFileDirectory::SetYPosition (float fYPosition)
{
    m_fYPosition = fYPosition;
    m_bYPositionFieldIsPresent = true;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatTiff::ImageFileDirectory::SetTransferFunction (const StillArray<Short>& rqTransferFunction)
{
    m_qTransferFunction = rqTransferFunction;
    m_bTransferFunctionFieldIsPresent = true;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatTiff::ImageFileDirectory::SetTransferRange (
    Short TransferBlack0, Short TransferWhite0,
    Short TransferBlack1, Short TransferWhite1,
    Short TransferBlack2, Short TransferWhite2)
{
    m_TransferBlack0 = TransferBlack0;
    m_TransferWhite0 = TransferWhite0;
    m_TransferBlack1 = TransferBlack1;
    m_TransferWhite1 = TransferWhite1;
    m_TransferBlack2 = TransferBlack2;
    m_TransferWhite2 = TransferWhite2;
    m_bTransferRangeFieldIsPresent = true;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatTiff::ImageFileDirectory::SetWhitePoint (float fWhitePointX, float fWhitePointY)
{
    m_fWhitePointX = fWhitePointX;
    m_fWhitePointY = fWhitePointY;
    m_bWhitePointFieldIsPresent = true;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatTiff::ImageFileDirectory::SetPrimaryChromaticities (
    float fChromaticityRedX, float fChromaticityRedY,
    float fChromaticityGreenX, float fChromaticityGreenY,
    float fChromaticityBlueX, float fChromaticityBlueY)
{
    m_fChromaticityRedX = fChromaticityRedX;
    m_fChromaticityRedY = fChromaticityRedY;
    m_fChromaticityGreenX = fChromaticityGreenX;
    m_fChromaticityGreenY = fChromaticityGreenY;
    m_fChromaticityBlueX = fChromaticityBlueX;
    m_fChromaticityBlueY = fChromaticityBlueY;
    m_bPrimaryChromaticitiesFieldIsPresent = true;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatTiff::ImageFileDirectory::SetReferenceBlackWhite (
    float fReferenceBlack0, float fReferenceWhite0,
    float fReferenceBlack1, float fReferenceWhite1,
    float fReferenceBlack2, float fReferenceWhite2)
{
    m_fReferenceBlack0 = fReferenceBlack0;
    m_fReferenceWhite0 = fReferenceWhite0;
    m_fReferenceBlack1 = fReferenceBlack1;
    m_fReferenceWhite1 = fReferenceWhite1;
    m_fReferenceBlack2 = fReferenceBlack2;
    m_fReferenceWhite2 = fReferenceWhite2;
    m_bReferenceBlackWhiteFieldIsPresent = true;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatTiff::ImageFileDirectory::SetDateTime (const Date& rqDateTime)
{
    m_qDateTime.SetQuantity(1);
    rqDateTime.ToString(m_qDateTime[0]);
    m_bDateTimeFieldIsPresent = true;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatTiff::ImageFileDirectory::SetImageDescription (const StringArray& rqImageDescription)
{
    m_qImageDescription = rqImageDescription;
    m_bImageDescriptionFieldIsPresent = true;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatTiff::ImageFileDirectory::SetMake (const StringArray& rqMake)
{
    m_qMake = rqMake;
    m_bMakeFieldIsPresent = true;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatTiff::ImageFileDirectory::SetModel (const StringArray& rqModel)
{
    m_qModel = rqModel;
    m_bModelFieldIsPresent = true;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatTiff::ImageFileDirectory::SetSoftware (const StringArray& rqSoftware)
{
    m_qSoftware = rqSoftware;
    m_bSoftwareFieldIsPresent = true;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatTiff::ImageFileDirectory::SetArtist (const StringArray& rqArtist)
{
    m_qArtist = rqArtist;
    m_bArtistFieldIsPresent = true;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatTiff::ImageFileDirectory::SetCopyright (const StringArray& rqCopyright)
{
    m_qCopyright = rqCopyright;
    m_bCopyrightFieldIsPresent = true;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatTiff::ImageFileDirectory::SetHostComputer (const StringArray& rqHostComputer)
{
    m_qHostComputer = rqHostComputer;
    m_bHostComputerFieldIsPresent = true;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatTiff::ImageFileDirectory::SetDocumentName (const StringArray& rqDocumentName)
{
    m_qDocumentName = rqDocumentName;
    m_bDocumentNameFieldIsPresent = true;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatTiff::ImageFileDirectory::SetPageName (const StringArray& rqPageName)
{
    m_qPageName = rqPageName;
    m_bPageNameFieldIsPresent = true;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatTiff::ImageFileDirectory::SetPageNumber (Short PageNumber, Short TotalPageNumber)
{
    m_PageNumber = PageNumber;
    m_TotalPageNumber = TotalPageNumber;
    m_bPageNumberFieldIsPresent = true;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatTiff::ImageFileDirectory::SetTargetPrinter (const StringArray& rqTargetPrinter)
{
    m_qTargetPrinter = rqTargetPrinter;
    m_bTargetPrinterFieldIsPresent = true;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatTiff::ImageFileDirectory::SetDotRange (const StillArray<Link<Short,Short> >& rqDotRange)
{
    m_qDotRange = rqDotRange;
    m_bDotRangeFieldIsPresent = true;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatTiff::ImageFileDirectory::SetThreshholding (ThreshholdingType eThreshholding)
{
    m_eThreshholding = eThreshholding;
    m_bThreshholdingFieldIsPresent = true;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatTiff::ImageFileDirectory::SetCellWidth (Short CellWidth)
{
    m_CellWidth = CellWidth;
    m_bCellWidthFieldIsPresent = true;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatTiff::ImageFileDirectory::SetCellLength (Short CellLength)
{
    m_CellLength = CellLength;
    m_bCellLengthFieldIsPresent = true;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatTiff::ImageFileDirectory::SetGrayResponseUnit (GrayResponseUnitType eGrayResponseUnit)
{
    m_eGrayResponseUnit = eGrayResponseUnit;
    m_bGrayResponseUnitFieldIsPresent = true;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatTiff::ImageFileDirectory::SetGrayResponseCurve (const StillArray<Short>& rqGrayResponseCurve)
{
    m_qGrayResponseCurve = rqGrayResponseCurve;
    m_bGrayResponseCurveFieldIsPresent = true;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatTiff::ImageFileDirectory::SetHalftoneHints (Short HighlightGrayLevel, Short ShadowGrayLevel)
{
    m_HighlightGrayLevel = HighlightGrayLevel;
    m_ShadowGrayLevel = ShadowGrayLevel;
    m_bHalftoneHintsFieldIsPresent = true;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatTiff::ImageFileDirectory::SetMinSampleValue (const StillArray<Short>& rqMinSampleValue)
{
    m_qMinSampleValue = rqMinSampleValue;
    m_bMinSampleValueFieldIsPresent = true;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatTiff::ImageFileDirectory::SetMaxSampleValue (const StillArray<Short>& rqMaxSampleValue)
{
    m_qMaxSampleValue = rqMaxSampleValue;
    m_bMaxSampleValueFieldIsPresent = true;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatTiff::ImageFileDirectory::SetSMinSampleValueUI (const StillArray<Long>& rqSMinSampleValueUI)
{
    assert( m_spqImageSaveTo->GetPixelComponentType() == RasterImage::PCT_UNSIGNED_INTEGER );

    m_qSMinSampleValueUI = rqSMinSampleValueUI;
    m_bSMinSampleValueFieldIsPresent = true;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatTiff::ImageFileDirectory::SetSMinSampleValueFP (const StillArray<Float>& rqSMinSampleValueFP)
{
    assert( m_spqImageSaveTo->GetPixelComponentType() == RasterImage::PCT_FLOATING_POINT );

    m_qSMinSampleValueFP = rqSMinSampleValueFP;
    m_bSMinSampleValueFieldIsPresent = true;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatTiff::ImageFileDirectory::SetSMaxSampleValueUI (const StillArray<Long>& rqSMaxSampleValueUI)
{
    assert( m_spqImageSaveTo->GetPixelComponentType() == RasterImage::PCT_UNSIGNED_INTEGER );

    m_qSMaxSampleValueUI = rqSMaxSampleValueUI;
    m_bSMaxSampleValueFieldIsPresent = true;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatTiff::ImageFileDirectory::SetSMaxSampleValueFP (const StillArray<Float>& rqSMaxSampleValueFP)
{
    assert( m_spqImageSaveTo->GetPixelComponentType() == RasterImage::PCT_FLOATING_POINT );

    m_qSMaxSampleValueFP = rqSMaxSampleValueFP;
    m_bSMaxSampleValueFieldIsPresent = true;
}
//------------------------------------------------------------------------------------------------------------------
inline FormatTiff::Long FormatTiff::ImageFileDirectory::GetImageWidth () const
{
    return m_ImageWidth;
}
//------------------------------------------------------------------------------------------------------------------
inline FormatTiff::Long FormatTiff::ImageFileDirectory::GetImageLength () const
{
    return m_ImageLength;
}
//------------------------------------------------------------------------------------------------------------------
inline int FormatTiff::ImageFileDirectory::GetBitsPerPixel () const
{
    int iBpp = 0;
    for (int i = 0; i < m_qBitsPerSample.GetQuantity(); i++)
        iBpp += m_qBitsPerSample[i];
    return iBpp;
}
//------------------------------------------------------------------------------------------------------------------
inline const StillArray<FormatTiff::Short>& FormatTiff::ImageFileDirectory::GetBitsPerSample () const
{
    return m_qBitsPerSample;
}
//------------------------------------------------------------------------------------------------------------------
inline FormatTiff::ColorModelType FormatTiff::ImageFileDirectory::GetColorModel () const
{
    return m_eColorModel;
}
//------------------------------------------------------------------------------------------------------------------
inline FormatTiff::CompressionType FormatTiff::ImageFileDirectory::GetCompression () const
{
    return m_eCompression;
}
//------------------------------------------------------------------------------------------------------------------
inline FormatTiff::OrientationType FormatTiff::ImageFileDirectory::GetOrientation () const
{
    return m_eOrientation;
}
//------------------------------------------------------------------------------------------------------------------
inline bool FormatTiff::ImageFileDirectory::IsSubfileReducedResolutionVersion () const
{
    return m_bSubfileIsReducedResolutionVersion;
}
//------------------------------------------------------------------------------------------------------------------
inline bool FormatTiff::ImageFileDirectory::IsSubfileSinglePageOfMultipageImage () const
{
    return m_bSubfileIsSinglePageOfMultipageImage;
}
//------------------------------------------------------------------------------------------------------------------
inline bool FormatTiff::ImageFileDirectory::IsSubfileTransparencyMask () const
{
    return m_bSubfileIsTransparencyMask;
}
//------------------------------------------------------------------------------------------------------------------
inline bool FormatTiff::ImageFileDirectory::HasSubfileTypeField () const
{
    return m_bSubfileTypeFieldIsPresent;
}
//------------------------------------------------------------------------------------------------------------------
inline FormatTiff::SubfileType FormatTiff::ImageFileDirectory::GetSubfileType () const
{
    assert( m_bSubfileTypeFieldIsPresent );
    return m_eSubfileType;
}
//------------------------------------------------------------------------------------------------------------------
inline bool FormatTiff::ImageFileDirectory::HasXResolutionField () const
{
    return m_bXResolutionFieldIsPresent;
}
//------------------------------------------------------------------------------------------------------------------
inline float FormatTiff::ImageFileDirectory::GetXResolution () const
{
    assert( m_bXResolutionFieldIsPresent );
    return m_fXResolution;
}
//------------------------------------------------------------------------------------------------------------------
inline bool FormatTiff::ImageFileDirectory::HasYResolutionField () const
{
    return m_bYResolutionFieldIsPresent;
}
//------------------------------------------------------------------------------------------------------------------
inline float FormatTiff::ImageFileDirectory::GetYResolution () const
{
    assert( m_bYResolutionFieldIsPresent );
    return m_fYResolution;
}
//------------------------------------------------------------------------------------------------------------------
inline FormatTiff::ResolutionUnitType FormatTiff::ImageFileDirectory::GetResolutionUnit () const
{
    return m_eResolutionUnit;
}
//------------------------------------------------------------------------------------------------------------------
inline bool FormatTiff::ImageFileDirectory::HasXPositionField () const
{
    return m_bXPositionFieldIsPresent;
}
//------------------------------------------------------------------------------------------------------------------
inline float FormatTiff::ImageFileDirectory::GetXPosition () const
{
    assert( m_bXPositionFieldIsPresent );
    return m_fXPosition;
}
//------------------------------------------------------------------------------------------------------------------
inline bool FormatTiff::ImageFileDirectory::HasYPositionField () const
{
    return m_bYPositionFieldIsPresent;
}
//------------------------------------------------------------------------------------------------------------------
inline float FormatTiff::ImageFileDirectory::GetYPosition () const
{
    assert( m_bYPositionFieldIsPresent );
    return m_fYPosition;
}
//------------------------------------------------------------------------------------------------------------------
inline const StillArray<FormatTiff::Short>& FormatTiff::ImageFileDirectory::GetTransferFunction () const
{
    return m_qTransferFunction;
}
//------------------------------------------------------------------------------------------------------------------
inline FormatTiff::Short FormatTiff::ImageFileDirectory::GetTransferBlack0 () const
{
    return m_TransferBlack0;
}
//------------------------------------------------------------------------------------------------------------------
inline FormatTiff::Short FormatTiff::ImageFileDirectory::GetTransferWhite0 () const
{
    return m_TransferWhite0;
}
//------------------------------------------------------------------------------------------------------------------
inline FormatTiff::Short FormatTiff::ImageFileDirectory::GetTransferBlack1 () const
{
    return m_TransferBlack1;
}
//------------------------------------------------------------------------------------------------------------------
inline FormatTiff::Short FormatTiff::ImageFileDirectory::GetTransferWhite1 () const
{
    return m_TransferWhite1;
}
//------------------------------------------------------------------------------------------------------------------
inline FormatTiff::Short FormatTiff::ImageFileDirectory::GetTransferBlack2 () const
{
    return m_TransferBlack2;
}
//------------------------------------------------------------------------------------------------------------------
inline FormatTiff::Short FormatTiff::ImageFileDirectory::GetTransferWhite2 () const
{
    return m_TransferWhite2;
}
//------------------------------------------------------------------------------------------------------------------
inline bool FormatTiff::ImageFileDirectory::HasWhitePointField () const
{
    return m_bWhitePointFieldIsPresent;
}
//------------------------------------------------------------------------------------------------------------------
inline float FormatTiff::ImageFileDirectory::GetWhitePointX () const
{
    assert( m_bWhitePointFieldIsPresent );
    return m_fWhitePointX;
}
//------------------------------------------------------------------------------------------------------------------
inline float FormatTiff::ImageFileDirectory::GetWhitePointY () const
{
    assert( m_bWhitePointFieldIsPresent );
    return m_fWhitePointY;
}
//------------------------------------------------------------------------------------------------------------------
inline bool FormatTiff::ImageFileDirectory::HasPrimaryChromaticitiesField () const
{
    return m_bPrimaryChromaticitiesFieldIsPresent;
}
//------------------------------------------------------------------------------------------------------------------
inline float FormatTiff::ImageFileDirectory::GetChromaticityRedX () const
{
    assert( m_bPrimaryChromaticitiesFieldIsPresent );
    return m_fChromaticityRedX;
}
//------------------------------------------------------------------------------------------------------------------
inline float FormatTiff::ImageFileDirectory::GetChromaticityRedY () const
{
    assert( m_bPrimaryChromaticitiesFieldIsPresent );
    return m_fChromaticityRedY;
}
//------------------------------------------------------------------------------------------------------------------
inline float FormatTiff::ImageFileDirectory::GetChromaticityGreenX () const
{
    assert( m_bPrimaryChromaticitiesFieldIsPresent );
    return m_fChromaticityGreenX;
}
//------------------------------------------------------------------------------------------------------------------
inline float FormatTiff::ImageFileDirectory::GetChromaticityGreenY () const
{
    assert( m_bPrimaryChromaticitiesFieldIsPresent );
    return m_fChromaticityGreenY;
}
//------------------------------------------------------------------------------------------------------------------
inline float FormatTiff::ImageFileDirectory::GetChromaticityBlueX () const
{
    assert( m_bPrimaryChromaticitiesFieldIsPresent );
    return m_fChromaticityBlueX;
}
//------------------------------------------------------------------------------------------------------------------
inline float FormatTiff::ImageFileDirectory::GetChromaticityBlueY () const
{
    assert( m_bPrimaryChromaticitiesFieldIsPresent );
    return m_fChromaticityBlueY;
}
//------------------------------------------------------------------------------------------------------------------
inline float FormatTiff::ImageFileDirectory::GetReferenceBlack0 () const
{
    return m_fReferenceBlack0;
}
//------------------------------------------------------------------------------------------------------------------
inline float FormatTiff::ImageFileDirectory::GetReferenceWhite0 () const
{
    return m_fReferenceWhite0;
}
//------------------------------------------------------------------------------------------------------------------
inline float FormatTiff::ImageFileDirectory::GetReferenceBlack1 () const
{
    return m_fReferenceBlack1;
}
//------------------------------------------------------------------------------------------------------------------
inline float FormatTiff::ImageFileDirectory::GetReferenceWhite1 () const
{
    return m_fReferenceWhite1;
}
//------------------------------------------------------------------------------------------------------------------
inline float FormatTiff::ImageFileDirectory::GetReferenceBlack2 () const
{
    return m_fReferenceBlack2;
}
//------------------------------------------------------------------------------------------------------------------
inline float FormatTiff::ImageFileDirectory::GetReferenceWhite2 () const
{
    return m_fReferenceWhite2;
}
//------------------------------------------------------------------------------------------------------------------
inline bool FormatTiff::ImageFileDirectory::HasDateTimeField () const
{
    return m_bDateTimeFieldIsPresent;
}
//------------------------------------------------------------------------------------------------------------------
inline const FormatTiff::StringArray& FormatTiff::ImageFileDirectory::GetDateTime () const
{
    assert( m_bDateTimeFieldIsPresent );
    return m_qDateTime;
}
//------------------------------------------------------------------------------------------------------------------
inline bool FormatTiff::ImageFileDirectory::HasImageDescriptionField () const
{
    return m_bImageDescriptionFieldIsPresent;
}
//------------------------------------------------------------------------------------------------------------------
inline const FormatTiff::StringArray& FormatTiff::ImageFileDirectory::GetImageDescription () const
{
    assert( m_bImageDescriptionFieldIsPresent );
    return m_qImageDescription;
}
//------------------------------------------------------------------------------------------------------------------
inline bool FormatTiff::ImageFileDirectory::HasMakeField () const
{
    return m_bMakeFieldIsPresent;
}
//------------------------------------------------------------------------------------------------------------------
inline const FormatTiff::StringArray& FormatTiff::ImageFileDirectory::GetMake () const
{
    assert( m_bMakeFieldIsPresent );
    return m_qMake;
}
//------------------------------------------------------------------------------------------------------------------
inline bool FormatTiff::ImageFileDirectory::HasModelField () const
{
    return m_bModelFieldIsPresent;
}
//------------------------------------------------------------------------------------------------------------------
inline const FormatTiff::StringArray& FormatTiff::ImageFileDirectory::GetModel () const
{
    assert( m_bModelFieldIsPresent );
    return m_qModel;
}
//------------------------------------------------------------------------------------------------------------------
inline bool FormatTiff::ImageFileDirectory::HasSoftwareField () const
{
    return m_bSoftwareFieldIsPresent;
}
//------------------------------------------------------------------------------------------------------------------
inline const FormatTiff::StringArray& FormatTiff::ImageFileDirectory::GetSoftware () const
{
    assert( m_bSoftwareFieldIsPresent );
    return m_qSoftware;
}
//------------------------------------------------------------------------------------------------------------------
inline bool FormatTiff::ImageFileDirectory::HasArtistField () const
{
    return m_bArtistFieldIsPresent;
}
//------------------------------------------------------------------------------------------------------------------
inline const FormatTiff::StringArray& FormatTiff::ImageFileDirectory::GetArtist () const
{
    assert( m_bArtistFieldIsPresent );
    return m_qArtist;
}
//------------------------------------------------------------------------------------------------------------------
inline bool FormatTiff::ImageFileDirectory::HasCopyrightField () const
{
    return m_bCopyrightFieldIsPresent;
}
//------------------------------------------------------------------------------------------------------------------
inline const FormatTiff::StringArray& FormatTiff::ImageFileDirectory::GetCopyright () const
{
    assert( m_bCopyrightFieldIsPresent );
    return m_qCopyright;
}
//------------------------------------------------------------------------------------------------------------------
inline bool FormatTiff::ImageFileDirectory::HasHostComputerField () const
{
    return m_bHostComputerFieldIsPresent;
}
//------------------------------------------------------------------------------------------------------------------
inline const FormatTiff::StringArray& FormatTiff::ImageFileDirectory::GetHostComputer () const
{
    assert( m_bHostComputerFieldIsPresent );
    return m_qHostComputer;
}
//------------------------------------------------------------------------------------------------------------------
inline bool FormatTiff::ImageFileDirectory::HasDocumentNameField () const
{
    return m_bDocumentNameFieldIsPresent;
}
//------------------------------------------------------------------------------------------------------------------
inline const FormatTiff::StringArray& FormatTiff::ImageFileDirectory::GetDocumentName () const
{
    assert( m_bDocumentNameFieldIsPresent );
    return m_qDocumentName;
}
//------------------------------------------------------------------------------------------------------------------
inline bool FormatTiff::ImageFileDirectory::HasPageNameField () const
{
    return m_bPageNameFieldIsPresent;
}
//------------------------------------------------------------------------------------------------------------------
inline const FormatTiff::StringArray& FormatTiff::ImageFileDirectory::GetPageName () const
{
    assert( m_bPageNameFieldIsPresent );
    return m_qPageName;
}
//------------------------------------------------------------------------------------------------------------------
inline bool FormatTiff::ImageFileDirectory::HasPageNumberField () const
{
    return m_bPageNumberFieldIsPresent;
}
//------------------------------------------------------------------------------------------------------------------
inline FormatTiff::Short FormatTiff::ImageFileDirectory::GetPageNumber () const
{
    assert( m_bPageNumberFieldIsPresent );
    return m_PageNumber;
}
//------------------------------------------------------------------------------------------------------------------
inline FormatTiff::Short FormatTiff::ImageFileDirectory::GetTotalPageNumber () const
{
    assert( m_bPageNumberFieldIsPresent );
    return m_TotalPageNumber;
}
//------------------------------------------------------------------------------------------------------------------
inline bool FormatTiff::ImageFileDirectory::HasTargetPrinterField () const
{
    return m_bTargetPrinterFieldIsPresent;
}
//------------------------------------------------------------------------------------------------------------------
inline const FormatTiff::StringArray& FormatTiff::ImageFileDirectory::GetTargetPrinter () const
{
    assert( m_bTargetPrinterFieldIsPresent );
    return m_qTargetPrinter;
}
//------------------------------------------------------------------------------------------------------------------
inline const StillArray<Link<FormatTiff::Short,FormatTiff::Short> >&
FormatTiff::ImageFileDirectory::GetDotRange () const
{
    return m_qDotRange;
}
//------------------------------------------------------------------------------------------------------------------
inline FormatTiff::InkSetType FormatTiff::ImageFileDirectory::GetInkSet () const
{
    return m_eInkSet;
}
//------------------------------------------------------------------------------------------------------------------
inline FormatTiff::Short FormatTiff::ImageFileDirectory::GetNumberOfInks () const
{
    return m_NumberOfInks;
}
//------------------------------------------------------------------------------------------------------------------
inline bool FormatTiff::ImageFileDirectory::HasInkNamesField () const
{
    return m_bInkNamesFieldIsPresent;
}
//------------------------------------------------------------------------------------------------------------------
inline const FormatTiff::StringArray& FormatTiff::ImageFileDirectory::GetInkNames () const
{
    assert( m_bInkNamesFieldIsPresent );
    return m_qInkNames;
}
//------------------------------------------------------------------------------------------------------------------
inline FormatTiff::ThreshholdingType FormatTiff::ImageFileDirectory::GetThreshholding () const
{
    return m_eThreshholding;
}
//------------------------------------------------------------------------------------------------------------------
inline bool FormatTiff::ImageFileDirectory::HasCellWidthField () const
{
    return m_bCellWidthFieldIsPresent;
}
//------------------------------------------------------------------------------------------------------------------
inline FormatTiff::Short FormatTiff::ImageFileDirectory::GetCellWidth () const
{
    assert( m_bCellWidthFieldIsPresent );
    return m_CellWidth;
}
//------------------------------------------------------------------------------------------------------------------
inline bool FormatTiff::ImageFileDirectory::HasCellLengthField () const
{
    return m_bCellLengthFieldIsPresent;
}
//------------------------------------------------------------------------------------------------------------------
inline FormatTiff::Short FormatTiff::ImageFileDirectory::GetCellLength () const
{
    assert( m_bCellLengthFieldIsPresent );
    return m_CellLength;
}
//------------------------------------------------------------------------------------------------------------------
inline FormatTiff::GrayResponseUnitType FormatTiff::ImageFileDirectory::GetGrayResponseUnit () const
{
    return m_eGrayResponseUnit;
}
//------------------------------------------------------------------------------------------------------------------
inline bool FormatTiff::ImageFileDirectory::HasGrayResponseCurveField () const
{
    return m_bGrayResponseCurveFieldIsPresent;
}
//------------------------------------------------------------------------------------------------------------------
inline const StillArray<FormatTiff::Short>& FormatTiff::ImageFileDirectory::GetGrayResponseCurve () const
{
    assert( m_bGrayResponseCurveFieldIsPresent );
    return m_qGrayResponseCurve;
}
//------------------------------------------------------------------------------------------------------------------
inline bool FormatTiff::ImageFileDirectory::HasHalftoneHintsField () const
{
    return m_bHalftoneHintsFieldIsPresent;
}
//------------------------------------------------------------------------------------------------------------------
inline FormatTiff::Short FormatTiff::ImageFileDirectory::GetHighlightGrayLevel () const
{
    assert( m_bHalftoneHintsFieldIsPresent );
    return m_HighlightGrayLevel;
}
//------------------------------------------------------------------------------------------------------------------
inline FormatTiff::Short FormatTiff::ImageFileDirectory::GetShadowGrayLevel () const
{
    assert( m_bHalftoneHintsFieldIsPresent );
    return m_ShadowGrayLevel;
}
//------------------------------------------------------------------------------------------------------------------
inline bool FormatTiff::ImageFileDirectory::HasMinSampleValueField () const
{
    return m_bMinSampleValueFieldIsPresent;
}
//------------------------------------------------------------------------------------------------------------------
inline const StillArray<FormatTiff::Short>& FormatTiff::ImageFileDirectory::GetMinSampleValue () const
{
    assert( m_bMinSampleValueFieldIsPresent );
    return m_qMinSampleValue;
}
//------------------------------------------------------------------------------------------------------------------
inline bool FormatTiff::ImageFileDirectory::HasMaxSampleValueField () const
{
    return m_bMaxSampleValueFieldIsPresent;
}
//------------------------------------------------------------------------------------------------------------------
inline const StillArray<FormatTiff::Short>& FormatTiff::ImageFileDirectory::GetMaxSampleValue () const
{
    assert( m_bMaxSampleValueFieldIsPresent );
    return m_qMaxSampleValue;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatTiff::SetByteOrder (ByteOrderType eByteOrder)
{
    m_eByteOrder = eByteOrder;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatTiff::SetExifMetadata (const FormatExifPtr& rspqExifMetadata)
{
    m_spqExifMetadata = rspqExifMetadata;
    m_bHasExifMetadata = true;
}
//------------------------------------------------------------------------------------------------------------------
inline FormatTiff::ByteOrderType FormatTiff::GetByteOrder () const
{
    return m_eByteOrder;
}
//------------------------------------------------------------------------------------------------------------------
inline int FormatTiff::GetImageFileDirectoryQuantity () const
{
    return m_qImageFileDirectories.GetQuantity();
}
//------------------------------------------------------------------------------------------------------------------
inline bool FormatTiff::HasExifMetadata () const
{
    return m_bHasExifMetadata;
}
//------------------------------------------------------------------------------------------------------------------
inline const FormatExif& FormatTiff::GetExifMetadata () const
{
    assert( m_bHasExifMetadata );
    return *m_spqExifMetadata;
}
//------------------------------------------------------------------------------------------------------------------
inline const FormatTiff::ImageFileDirectory& FormatTiff::GetImageFileDirectory (int i) const
{
    return *m_qImageFileDirectories[i];
}
//------------------------------------------------------------------------------------------------------------------
inline FormatTiff::StringArray FormatTiff::WrapSoleString (const String& rqString)
{
    StringArray qStringArray(1);
    qStringArray.GetFirst() = rqString;
    return qStringArray;
}
//------------------------------------------------------------------------------------------------------------------
inline String FormatTiff::UnwrapSoleString (const StringArray& rqStringArray)
{
    assert( rqStringArray.GetQuantity() == 1 );
    return rqStringArray.GetFirst();
}
//------------------------------------------------------------------------------------------------------------------



