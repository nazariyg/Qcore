//------------------------------------------------------------------------------------------------------------------
inline void FormatPng::SetFilterSelectionMethod (FilterSelectionMethodType eFilterSelectionMethod)
{
    m_eFilterSelectionMethod = eFilterSelectionMethod;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatPng::SetInterlaceMethod (InterlaceMethodType eInterlaceMethod)
{
    m_eInterlaceMethod = eInterlaceMethod;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatPng::SetDataChunkSplitting (DataChunkSplittingType eDataChunkSplitting)
{
    m_eDataChunkSplitting = eDataChunkSplitting;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatPng::UnsetPhysicalPixelDimensions ()
{
    m_bHas_pHYs_Chunk = false;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatPng::SetPhysicalPixelDimensions (int iPixelsPerUnitX, int iPixelsPerUnitY,
    PhysicalPixelDimensionsUnitType ePhysicalPixelDimensionsUnit)
{
    assert( iPixelsPerUnitX > 0 && iPixelsPerUnitY > 0 );

    m_iPixelsPerUnitX = iPixelsPerUnitX;
    m_iPixelsPerUnitY = iPixelsPerUnitY;
    m_ePhysicalPixelDimensionsUnit = ePhysicalPixelDimensionsUnit;

    m_bHas_pHYs_Chunk = true;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatPng::SetChromaticities (
    float fWhitePointX, float fWhitePointY,
    float fRedX, float fRedY,
    float fGreenX, float fGreenY,
    float fBlueX, float fBlueY)
{
    m_fChromaticityWhitePointX = fWhitePointX;
    m_fChromaticityWhitePointY = fWhitePointY;
    m_fChromaticityRedX = fRedX;
    m_fChromaticityRedY = fRedY;
    m_fChromaticityGreenX = fGreenX;
    m_fChromaticityGreenY = fGreenY;
    m_fChromaticityBlueX = fBlueX;
    m_fChromaticityBlueY = fBlueY;

    m_bHas_cHRM_Chunk = true;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatPng::SetGamma (float fGamma)
{
    m_fGamma = fGamma;
    m_bHas_gAMA_Chunk = true;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatPng::SetICCProfile (const String& rqICCProfileName, const StillArray<char>& rqICCProfile)
{
    m_qICCProfileName = rqICCProfileName;
    m_qICCProfile = rqICCProfile;

    m_bHas_iCCP_Chunk = true;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatPng::SetStandardRgbColorSpaceRenderingIntent (
    StandardRgbColorSpaceRenderingIntentType eStandardRgbColorSpaceRenderingIntent)
{
    assert( m_spqImageSaveTo->GetColorModel() == RasterImage::CMT_RGB
        ||  m_spqImageSaveTo->GetColorModel() == RasterImage::CMT_RGBA );

    m_eStandardRgbColorSpaceRenderingIntent = eStandardRgbColorSpaceRenderingIntent;
    m_bHas_sRGB_Chunk = true;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatPng::SetBackgroundGrayscale (int iGrayscale)
{
    assert( m_spqImageSaveTo->GetColorModel() == RasterImage::CMT_GRAYSCALE
        ||  m_spqImageSaveTo->GetColorModel() == RasterImage::CMT_GRAYSCALE_WITH_ALPHA );

    m_iBackgroundGrayscale = iGrayscale;
    m_bHas_bKGD_Chunk = true;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatPng::SetBackgroundRgb (int iRed, int iGreen, int iBlue)
{
    assert( m_spqImageSaveTo->GetColorModel() == RasterImage::CMT_RGB
        ||  m_spqImageSaveTo->GetColorModel() == RasterImage::CMT_RGBA );

    m_iBackgroundRed = iRed;
    m_iBackgroundGreen = iGreen;
    m_iBackgroundBlue = iBlue;

    m_bHas_bKGD_Chunk = true;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatPng::SetBackgroundPaletteIndex (int iPaletteIndex)
{
    assert( m_spqImageSaveTo->GetColorModel() == RasterImage::CMT_PALETTED
        ||  m_spqImageSaveTo->GetColorModel() == RasterImage::CMT_PALETTED_WITH_ALPHA );

    m_iBackgroundPaletteIndex = iPaletteIndex;
    m_bHas_bKGD_Chunk = true;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatPng::SetHistogramFrequencies (const StillArray<int>& rqHistogramFrequencies)
{
    assert( m_spqImageSaveTo->GetColorModel() == RasterImage::CMT_PALETTED
        ||  m_spqImageSaveTo->GetColorModel() == RasterImage::CMT_PALETTED_WITH_ALPHA );
    assert( rqHistogramFrequencies.GetQuantity() == m_spqImageSaveTo->GetPaletteSize() );

    m_qHistogramFrequencies = rqHistogramFrequencies;
    m_bHas_hIST_Chunk = true;
}
//------------------------------------------------------------------------------------------------------------------
inline int FormatPng::AddSuggestedPalette (const SuggestedPalette& rqSuggestedPalette)
{
    return m_qSuggestedPalettes.Push(rqSuggestedPalette);
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatPng::SetLastModificationTime (const Date& rqLastModificationTime)
{
    m_qLastModificationTime = rqLastModificationTime;
    m_bHas_tIME_Chunk = true;
}
//------------------------------------------------------------------------------------------------------------------
inline int FormatPng::AddTextualData (TextualData& rqTextualData, bool bOnlyAs_tEXt)
{
    rqTextualData.m_bOnlyAs_tEXt = bOnlyAs_tEXt;
    return m_qTextualDataArray.Push(rqTextualData);
}
//------------------------------------------------------------------------------------------------------------------
inline int FormatPng::GetWidth () const
{
    return m_iWidth;
}
//------------------------------------------------------------------------------------------------------------------
inline int FormatPng::GetHeight () const
{
    return m_iHeight;
}
//------------------------------------------------------------------------------------------------------------------
inline int FormatPng::GetBitsPerPixel () const
{
    return m_iBitsPerPixel;
}
//------------------------------------------------------------------------------------------------------------------
inline FormatPng::ColorModelType FormatPng::GetColorModel () const
{
    return m_eColorModel;
}
//------------------------------------------------------------------------------------------------------------------
inline FormatPng::InterlaceMethodType FormatPng::GetInterlaceMethod () const
{
    return m_eInterlaceMethod;
}
//------------------------------------------------------------------------------------------------------------------
inline FormatPng::CompressionType FormatPng::GetCompression () const
{
    return m_eCompression;
}
//------------------------------------------------------------------------------------------------------------------
inline FormatPng::FilterMethodType FormatPng::GetFilterMethod () const
{
    return m_eFilterMethod;
}
//------------------------------------------------------------------------------------------------------------------
inline int FormatPng::GetBitsPerSample () const
{
    return m_iBitDepth;
}
//------------------------------------------------------------------------------------------------------------------
inline bool FormatPng::Has_pHYs_Chunk () const
{
    return m_bHas_pHYs_Chunk;
}
//------------------------------------------------------------------------------------------------------------------
inline int FormatPng::GetPixelsPerUnitX () const
{
    assert( m_bHas_pHYs_Chunk );
    return m_iPixelsPerUnitX;
}
//------------------------------------------------------------------------------------------------------------------
inline int FormatPng::GetPixelsPerUnitY () const
{
    assert( m_bHas_pHYs_Chunk );
    return m_iPixelsPerUnitY;
}
//------------------------------------------------------------------------------------------------------------------
inline FormatPng::PhysicalPixelDimensionsUnitType FormatPng::GetPhysicalPixelDimensionsUnit () const
{
    assert( m_bHas_pHYs_Chunk );
    return m_ePhysicalPixelDimensionsUnit;
}
//------------------------------------------------------------------------------------------------------------------
inline bool FormatPng::Has_cHRM_Chunk () const
{
    return m_bHas_cHRM_Chunk;
}
//------------------------------------------------------------------------------------------------------------------
inline float FormatPng::GetChromaticityWhitePointX () const
{
    assert( m_bHas_cHRM_Chunk );
    return m_fChromaticityWhitePointX;
}
//------------------------------------------------------------------------------------------------------------------
inline float FormatPng::GetChromaticityWhitePointY () const
{
    assert( m_bHas_cHRM_Chunk );
    return m_fChromaticityWhitePointY;
}
//------------------------------------------------------------------------------------------------------------------
inline float FormatPng::GetChromaticityRedX () const
{
    assert( m_bHas_cHRM_Chunk );
    return m_fChromaticityRedX;
}
//------------------------------------------------------------------------------------------------------------------
inline float FormatPng::GetChromaticityRedY () const
{
    assert( m_bHas_cHRM_Chunk );
    return m_fChromaticityRedY;
}
//------------------------------------------------------------------------------------------------------------------
inline float FormatPng::GetChromaticityGreenX () const
{
    assert( m_bHas_cHRM_Chunk );
    return m_fChromaticityGreenX;
}
//------------------------------------------------------------------------------------------------------------------
inline float FormatPng::GetChromaticityGreenY () const
{
    assert( m_bHas_cHRM_Chunk );
    return m_fChromaticityGreenY;
}
//------------------------------------------------------------------------------------------------------------------
inline float FormatPng::GetChromaticityBlueX () const
{
    assert( m_bHas_cHRM_Chunk );
    return m_fChromaticityBlueX;
}
//------------------------------------------------------------------------------------------------------------------
inline float FormatPng::GetChromaticityBlueY () const
{
    assert( m_bHas_cHRM_Chunk );
    return m_fChromaticityBlueY;
}
//------------------------------------------------------------------------------------------------------------------
inline bool FormatPng::Has_gAMA_Chunk () const
{
    return m_bHas_gAMA_Chunk;
}
//------------------------------------------------------------------------------------------------------------------
inline float FormatPng::GetGamma () const
{
    assert( m_bHas_gAMA_Chunk );
    return m_fGamma;
}
//------------------------------------------------------------------------------------------------------------------
inline bool FormatPng::Has_iCCP_Chunk () const
{
    return m_bHas_iCCP_Chunk;
}
//------------------------------------------------------------------------------------------------------------------
inline const String& FormatPng::GetICCProfileName () const
{
    assert( m_bHas_iCCP_Chunk );
    return m_qICCProfileName;
}
//------------------------------------------------------------------------------------------------------------------
inline const StillArray<char>& FormatPng::GetICCProfile () const
{
    assert( m_bHas_iCCP_Chunk );
    return m_qICCProfile;
}
//------------------------------------------------------------------------------------------------------------------
inline bool FormatPng::Has_sRGB_Chunk () const
{
    return m_bHas_sRGB_Chunk;
}
//------------------------------------------------------------------------------------------------------------------
inline FormatPng::StandardRgbColorSpaceRenderingIntentType
FormatPng::GetStandardRgbColorSpaceRenderingIntent () const
{
    assert( m_bHas_sRGB_Chunk );
    return m_eStandardRgbColorSpaceRenderingIntent;
}
//------------------------------------------------------------------------------------------------------------------
inline bool FormatPng::Has_bKGD_Chunk () const
{
    return m_bHas_bKGD_Chunk;
}
//------------------------------------------------------------------------------------------------------------------
inline int FormatPng::GetBackgroundGrayscale () const
{
    assert( m_bHas_bKGD_Chunk );
    assert( m_eColorModel == CMT_GRAYSCALE || m_eColorModel == CMT_GRAYSCALE_WITH_ALPHA );

    return m_iBackgroundGrayscale;
}
//------------------------------------------------------------------------------------------------------------------
inline int FormatPng::GetBackgroundRed () const
{
    assert( m_bHas_bKGD_Chunk );
    assert( m_eColorModel == CMT_RGB || m_eColorModel == CMT_RGBA );

    return m_iBackgroundRed;
}
//------------------------------------------------------------------------------------------------------------------
inline int FormatPng::GetBackgroundGreen () const
{
    assert( m_bHas_bKGD_Chunk );
    assert( m_eColorModel == CMT_RGB || m_eColorModel == CMT_RGBA );

    return m_iBackgroundGreen;
}
//------------------------------------------------------------------------------------------------------------------
inline int FormatPng::GetBackgroundBlue () const
{
    assert( m_bHas_bKGD_Chunk );
    assert( m_eColorModel == CMT_RGB || m_eColorModel == CMT_RGBA );

    return m_iBackgroundBlue;
}
//------------------------------------------------------------------------------------------------------------------
inline int FormatPng::GetBackgroundPaletteIndex () const
{
    assert( m_bHas_bKGD_Chunk );
    assert( m_eColorModel == CMT_INDEXED );

    return m_iBackgroundPaletteIndex;
}
//------------------------------------------------------------------------------------------------------------------
inline bool FormatPng::Has_hIST_Chunk () const
{
    return m_bHas_hIST_Chunk;
}
//------------------------------------------------------------------------------------------------------------------
inline const StillArray<int>& FormatPng::GetHistogramFrequencies () const
{
    assert( m_bHas_hIST_Chunk );
    return m_qHistogramFrequencies;
}
//------------------------------------------------------------------------------------------------------------------
inline int FormatPng::GetSuggestedPaletteQuantity () const
{
    return m_qSuggestedPalettes.GetQuantity();
}
//------------------------------------------------------------------------------------------------------------------
inline const FormatPng::SuggestedPalette& FormatPng::GetSuggestedPalette (int i) const
{
    return m_qSuggestedPalettes[i];
}
//------------------------------------------------------------------------------------------------------------------
inline bool FormatPng::Has_tIME_Chunk () const
{
    return m_bHas_tIME_Chunk;
}
//------------------------------------------------------------------------------------------------------------------
inline const Date& FormatPng::GetLastModificationTime () const
{
    assert( m_bHas_tIME_Chunk );
    return m_qLastModificationTime;
}
//------------------------------------------------------------------------------------------------------------------
inline int FormatPng::GetTextualDataQuantity () const
{
    return m_qTextualDataArray.GetQuantity();
}
//------------------------------------------------------------------------------------------------------------------
inline const FormatPng::TextualData& FormatPng::GetTextualData (int i) const
{
    return m_qTextualDataArray[i];
}
//------------------------------------------------------------------------------------------------------------------
inline int FormatPng::GetDevelopedRowQuantity () const
{
    assert( m_eStreaming == Stream::ST_READING_FROM_DYNAMIC_BYTESTREAM );
    return m_iDevelopedRowQuantity;
}
//------------------------------------------------------------------------------------------------------------------
inline int FormatPng::GetCurrentPassIndex () const
{
    assert( m_eStreaming == Stream::ST_READING_FROM_DYNAMIC_BYTESTREAM );
    return m_iCurrentPassIndex;
}
//------------------------------------------------------------------------------------------------------------------
inline int FormatPng::PredictPaeth (int iA, int iB, int iC)
{
    int iP = iA + iB - iC;
    int iPA = Mathi::Abs(iP-iA);
    int iPB = Mathi::Abs(iP-iB);
    int iPC = Mathi::Abs(iP-iC);
    if ( iPA <= iPB && iPA <= iPC )
        return iA;
    else if ( iPB <= iPC )
        return iB;
    else
        return iC;
}
//------------------------------------------------------------------------------------------------------------------



