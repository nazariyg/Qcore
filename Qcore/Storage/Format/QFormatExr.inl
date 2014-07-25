//------------------------------------------------------------------------------------------------------------------
inline void FormatExr::SetImageDisplayWindow (const Bound2i& rqWindow)
{
    m_qImageDisplayWindow = rqWindow;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExr::SetImageDataWindow (const Bound2i& rqWindow)
{
    assert( rqWindow.MaxX - rqWindow.MinX + 1 == m_iImageWidth
        &&  rqWindow.MaxY - rqWindow.MinY + 1 == m_iImageHeight );

    m_qImageDataWindow = rqWindow;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExr::SetColorModel (ColorModelType eColorModel)
{
    m_eColorModel = eColorModel;
    m_bColorModelPrecised = true;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExr::SetCompression (CompressionType eCompression)
{
    m_eCompression = eCompression;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExr::SetPixelAspectRatio (float fPixelAspectRatio)
{
    assert( fPixelAspectRatio > 0.0f );
    m_fPixelAspectRatio = fPixelAspectRatio;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExr::SetScreenWindowCenter (float fX, float fY)
{
    m_fScreenWindowCenterX = fX;
    m_fScreenWindowCenterY = fY;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExr::SetScreenWindowWidth (float fWidth)
{
    assert( fWidth > 0.0f );
    m_fScreenWindowWidth = fWidth;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExr::SetLineOrder (LineOrderType eLineOrder)
{
    m_eLineOrder = eLineOrder;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExr::SetTileDescription (LevelModeType eLevelMode, LevelRoundingModeType eLevelRoundingMode,
    int iTileWidth, int iTileHeight)
{
    m_bIsTiled = true;
    m_eLevelMode = eLevelMode;
    m_eLevelRoundingMode = eLevelRoundingMode;
    m_iTileWidth = iTileWidth;
    m_iTileHeight = iTileHeight;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExr::SetPreviewImage (int iWidth, int iHeight, const StillArray<UcRgba>& rqPixels)
{
    assert( rqPixels.GetQuantity() == iWidth*iHeight );

    m_bHasPreviewImage = true;
    m_iPreviewImageWidth = iWidth;
    m_iPreviewImageHeight = iHeight;
    m_qPreviewImagePixels = rqPixels;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExr::SetChromaticities (
    float fRedX, float fRedY,
    float fGreenX, float fGreenY,
    float fBlueX, float fBlueY,
    float fWhitePointX, float fWhitePointY)
{
    m_bHasChromaticities = true;
    m_fChromaticityRedX = fRedX;
    m_fChromaticityRedY = fRedY;
    m_fChromaticityGreenX = fGreenX;
    m_fChromaticityGreenY = fGreenY;
    m_fChromaticityBlueX = fBlueX;
    m_fChromaticityBlueY = fBlueY;
    m_fWhitePointX = fWhitePointX;
    m_fWhitePointY = fWhitePointY;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExr::SetWhiteLuminance (float fWhiteLuminance)
{
    m_bHasWhiteLuminance = true;
    m_fWhiteLuminance = fWhiteLuminance;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExr::SetHorizontalDensity (float fDensity)
{
    assert( fDensity > 0.0f );

    m_bHasHorizontalDensity = true;
    m_fHorizontalDensity = fDensity;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExr::SetOwner (const String& rqOwner)
{
    m_bHasOwner = true;
    m_qOwner = rqOwner;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExr::SetComments (const String& rqComments)
{
    m_bHasComments = true;
    m_qComments = rqComments;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExr::SetCaptureDate (const Date& rqDate)
{
    m_bHasCaptureDate = true;
    m_qCaptureDate = rqDate;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExr::SetUtcOffset (float fUtcOffset)
{
    m_bHasUtcOffset = true;
    m_fUtcOffset = fUtcOffset;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExr::SetLongitude (float fLongitude)
{
    m_bHasLongitude = true;
    m_fLongitude = fLongitude;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExr::SetLatitude (float fLatitude)
{
    m_bHasLatitude = true;
    m_fLatitude = fLatitude;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExr::SetAltitude (float fAltitude)
{
    m_bHasAltitude = true;
    m_fAltitude = fAltitude;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExr::SetFocus (float fFocus)
{
    m_bHasFocus = true;
    m_fFocus = fFocus;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExr::SetExposureTime (float fExposureTime)
{
    m_bHasExposureTime = true;
    m_fExposureTime = fExposureTime;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExr::SetAperture (float fAperture)
{
    m_bHasAperture = true;
    m_fAperture = fAperture;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExr::SetIsoSpeed (float fIsoSpeed)
{
    m_bHasIsoSpeed = true;
    m_fIsoSpeed = fIsoSpeed;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExr::SetEnvironmentMapType (EnvironmentMapType eType)
{
    m_bIsEnvironmentMap = true;
    m_eEnvironmentMapType = eType;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExr::SetKeyCode (const KeyCode& rqKeyCode)
{
    m_bHasKeyCode = true;
    m_qKeyCode = rqKeyCode;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExr::SetTimeCode (const TimeCode& rqTimeCode)
{
    m_bHasTimeCode = true;
    m_qTimeCode = rqTimeCode;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExr::SetWrapmodes (const String& rqWrapmodes)
{
    m_bHasWrapmodes = true;
    m_qWrapmodes = rqWrapmodes;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExr::SetAlphaAssociationHandling (bool bMultiplyAlpha)
{
    m_bMultiplyAlpha = bMultiplyAlpha;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExr::SetScreenModeOff (float fExposure, float fGamma)
{
    m_bOffScreenMode = true;
    m_fOffScreenExposure = fExposure;
    m_fOffScreenGamma = fGamma;

    // precompute the helping value
    m_fOffScreenExposureCoeff = 1.0f/Mathf::Pow(2.0f,fExposure);
}
//------------------------------------------------------------------------------------------------------------------
inline int FormatExr::GetVersion () const
{
    return m_iVersion;
}
//------------------------------------------------------------------------------------------------------------------
inline int FormatExr::GetImageWidth () const
{
    return m_iImageWidth;
}
//------------------------------------------------------------------------------------------------------------------
inline int FormatExr::GetImageHeight () const
{
    return m_iImageHeight;
}
//------------------------------------------------------------------------------------------------------------------
inline const Bound2i& FormatExr::GetImageDisplayWindow () const
{
    return m_qImageDisplayWindow;
}
//------------------------------------------------------------------------------------------------------------------
inline const Bound2i& FormatExr::GetImageDataWindow () const
{
    return m_qImageDataWindow;
}
//------------------------------------------------------------------------------------------------------------------
inline int FormatExr::GetBitsPerPixel () const
{
    int iBpp = 0;
    for (int i = 0; i < m_qChannelRecords.GetQuantity(); i++)
    {
        switch ( m_qChannelRecords[i].DataType )
        {
        case CDT_UINT:
            iBpp += 32;
            break;
        case CDT_HALF:
            iBpp += 16;
            break;
        case CDT_FLOAT:
            iBpp += 32;
            break;
        }
    }
    return iBpp;
}
//------------------------------------------------------------------------------------------------------------------
inline FormatExr::ColorModelType FormatExr::GetColorModel () const
{
    return m_eColorModel;
}
//------------------------------------------------------------------------------------------------------------------
inline FormatExr::CompressionType FormatExr::GetCompression () const
{
    return m_eCompression;
}
//------------------------------------------------------------------------------------------------------------------
inline float FormatExr::GetPixelAspectRatio () const
{
    return m_fPixelAspectRatio;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExr::GetScreenWindowCenter (float& rfX, float& rfY) const
{
    rfX = m_fScreenWindowCenterX;
    rfY = m_fScreenWindowCenterY;
}
//------------------------------------------------------------------------------------------------------------------
inline float FormatExr::GetScreenWindowWidth () const
{
    return m_fScreenWindowWidth;
}
//------------------------------------------------------------------------------------------------------------------
inline int FormatExr::GetChannelQuantity () const
{
    return m_iChannelQuantity;
}
//------------------------------------------------------------------------------------------------------------------
inline const FormatExr::ChannelRecord& FormatExr::GetChannelRecord (int i) const
{
    return m_qChannelRecords[i];
}
//------------------------------------------------------------------------------------------------------------------
inline FormatExr::LineOrderType FormatExr::GetLineOrder () const
{
    return m_eLineOrder;
}
//------------------------------------------------------------------------------------------------------------------
inline bool FormatExr::IsTiled () const
{
    return m_bIsTiled;
}
//------------------------------------------------------------------------------------------------------------------
inline int FormatExr::GetTileWidth () const
{
    assert( m_bIsTiled );
    return m_iTileWidth;
}
//------------------------------------------------------------------------------------------------------------------
inline int FormatExr::GetTileHeight () const
{
    assert( m_bIsTiled );
    return m_iTileHeight;
}
//------------------------------------------------------------------------------------------------------------------
inline FormatExr::LevelModeType FormatExr::GetLevelMode () const
{
    assert( m_bIsTiled );
    return m_eLevelMode;
}
//------------------------------------------------------------------------------------------------------------------
inline FormatExr::LevelRoundingModeType FormatExr::GetLevelRoundingMode () const
{
    assert( m_bIsTiled );
    return m_eLevelRoundingMode;
}
//------------------------------------------------------------------------------------------------------------------
inline int FormatExr::GetMipmapLevelQuantity () const
{
    assert( m_bIsTiled );
    assert( m_eLevelMode == LMT_MIPMAP_LEVELS );

    return m_iMipmapLevelQuantity;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExr::GetRipmapLevelQuantities (int& riQuantityX, int& riQuantityY) const
{
    assert( m_bIsTiled );
    assert( m_eLevelMode == LMT_RIPMAP_LEVELS );

    riQuantityX = m_iRipmapLevelQuantityX;
    riQuantityY = m_iRipmapLevelQuantityY;
}
//------------------------------------------------------------------------------------------------------------------
inline bool FormatExr::HasPreviewImage () const
{
    return m_bHasPreviewImage;
}
//------------------------------------------------------------------------------------------------------------------
inline int FormatExr::GetPreviewImageWidth () const
{
    assert( m_bHasPreviewImage );
    return m_iPreviewImageWidth;
}
//------------------------------------------------------------------------------------------------------------------
inline int FormatExr::GetPreviewImageHeight () const
{
    assert( m_bHasPreviewImage );
    return m_iPreviewImageHeight;
}
//------------------------------------------------------------------------------------------------------------------
inline const StillArray<FormatExr::UcRgba>& FormatExr::GetPreviewImagePixels () const
{
    assert( m_bHasPreviewImage );
    return m_qPreviewImagePixels;
}
//------------------------------------------------------------------------------------------------------------------
inline bool FormatExr::HasChromaticities () const
{
    return m_bHasChromaticities;
}
//------------------------------------------------------------------------------------------------------------------
inline float FormatExr::GetChromaticityRedX () const
{
    assert( m_bHasChromaticities );
    return m_fChromaticityRedX;
}
//------------------------------------------------------------------------------------------------------------------
inline float FormatExr::GetChromaticityRedY () const
{
    assert( m_bHasChromaticities );
    return m_fChromaticityRedY;
}
//------------------------------------------------------------------------------------------------------------------
inline float FormatExr::GetChromaticityGreenX () const
{
    assert( m_bHasChromaticities );
    return m_fChromaticityGreenX;
}
//------------------------------------------------------------------------------------------------------------------
inline float FormatExr::GetChromaticityGreenY () const
{
    assert( m_bHasChromaticities );
    return m_fChromaticityGreenY;
}
//------------------------------------------------------------------------------------------------------------------
inline float FormatExr::GetChromaticityBlueX () const
{
    assert( m_bHasChromaticities );
    return m_fChromaticityBlueX;
}
//------------------------------------------------------------------------------------------------------------------
inline float FormatExr::GetChromaticityBlueY () const
{
    assert( m_bHasChromaticities );
    return m_fChromaticityBlueY;
}
//------------------------------------------------------------------------------------------------------------------
inline float FormatExr::GetWhitePointX () const
{
    assert( m_bHasChromaticities );
    return m_fWhitePointX;
}
//------------------------------------------------------------------------------------------------------------------
inline float FormatExr::GetWhitePointY () const
{
    assert( m_bHasChromaticities );
    return m_fWhitePointY;
}
//------------------------------------------------------------------------------------------------------------------
inline bool FormatExr::HasWhiteLuminance () const
{
    return m_bHasWhiteLuminance;
}
//------------------------------------------------------------------------------------------------------------------
inline float FormatExr::GetWhiteLuminance () const
{
    assert( m_bHasWhiteLuminance );
    return m_fWhiteLuminance;
}
//------------------------------------------------------------------------------------------------------------------
inline bool FormatExr::HasHorizontalDensity () const
{
    return m_bHasHorizontalDensity;
}
//------------------------------------------------------------------------------------------------------------------
inline float FormatExr::GetHorizontalDensity () const
{
    assert( m_bHasHorizontalDensity );
    return m_fHorizontalDensity;
}
//------------------------------------------------------------------------------------------------------------------
inline bool FormatExr::HasOwner () const
{
    return m_bHasOwner;
}
//------------------------------------------------------------------------------------------------------------------
inline const String& FormatExr::GetOwner () const
{
    assert( m_bHasOwner );
    return m_qOwner;
}
//------------------------------------------------------------------------------------------------------------------
inline bool FormatExr::HasComments () const
{
    return m_bHasComments;
}
//------------------------------------------------------------------------------------------------------------------
inline const String& FormatExr::GetComments () const
{
    assert( m_bHasComments );
    return m_qComments;
}
//------------------------------------------------------------------------------------------------------------------
inline bool FormatExr::HasCaptureDate () const
{
    return m_bHasCaptureDate;
}
//------------------------------------------------------------------------------------------------------------------
inline const Date& FormatExr::GetCaptureDate () const
{
    assert( m_bHasCaptureDate );
    return m_qCaptureDate;
}
//------------------------------------------------------------------------------------------------------------------
inline bool FormatExr::HasUtcOffset () const
{
    return m_bHasUtcOffset;
}
//------------------------------------------------------------------------------------------------------------------
inline float FormatExr::GetUtcOffset () const
{
    assert( m_bHasUtcOffset );
    return m_fUtcOffset;
}
//------------------------------------------------------------------------------------------------------------------
inline bool FormatExr::HasLongitude () const
{
    return m_bHasLongitude;
}
//------------------------------------------------------------------------------------------------------------------
inline float FormatExr::GetLongitude () const
{
    assert( m_bHasLongitude );
    return m_fLongitude;
}
//------------------------------------------------------------------------------------------------------------------
inline bool FormatExr::HasLatitude () const
{
    return m_bHasLatitude;
}
//------------------------------------------------------------------------------------------------------------------
inline float FormatExr::GetLatitude () const
{
    assert( m_bHasLatitude );
    return m_fLatitude;
}
//------------------------------------------------------------------------------------------------------------------
inline bool FormatExr::HasAltitude () const
{
    return m_bHasAltitude;
}
//------------------------------------------------------------------------------------------------------------------
inline float FormatExr::GetAltitude () const
{
    assert( m_bHasAltitude );
    return m_fAltitude;
}
//------------------------------------------------------------------------------------------------------------------
inline bool FormatExr::HasFocus () const
{
    return m_bHasFocus;
}
//------------------------------------------------------------------------------------------------------------------
inline float FormatExr::GetFocus () const
{
    assert( m_bHasFocus );
    return m_fFocus;
}
//------------------------------------------------------------------------------------------------------------------
inline bool FormatExr::HasExposureTime () const
{
    return m_bHasExposureTime;
}
//------------------------------------------------------------------------------------------------------------------
inline float FormatExr::GetExposureTime () const
{
    assert( m_bHasExposureTime );
    return m_fExposureTime;
}
//------------------------------------------------------------------------------------------------------------------
inline bool FormatExr::HasAperture () const
{
    return m_bHasAperture;
}
//------------------------------------------------------------------------------------------------------------------
inline float FormatExr::GetAperture () const
{
    assert( m_bHasAperture );
    return m_fAperture;
}
//------------------------------------------------------------------------------------------------------------------
inline bool FormatExr::HasIsoSpeed () const
{
    return m_bHasIsoSpeed;
}
//------------------------------------------------------------------------------------------------------------------
inline float FormatExr::GetIsoSpeed () const
{
    assert( m_bHasIsoSpeed );
    return m_fIsoSpeed;
}
//------------------------------------------------------------------------------------------------------------------
inline bool FormatExr::IsEnvironmentMap () const
{
    return m_bIsEnvironmentMap;
}
//------------------------------------------------------------------------------------------------------------------
inline FormatExr::EnvironmentMapType FormatExr::GetEnvironmentMapType () const
{
    assert( m_bIsEnvironmentMap );
    return m_eEnvironmentMapType;
}
//------------------------------------------------------------------------------------------------------------------
inline bool FormatExr::HasKeyCode () const
{
    return m_bHasKeyCode;
}
//------------------------------------------------------------------------------------------------------------------
inline const FormatExr::KeyCode& FormatExr::GetKeyCode () const
{
    assert( m_bHasKeyCode );
    return m_qKeyCode;
}
//------------------------------------------------------------------------------------------------------------------
inline bool FormatExr::HasTimeCode () const
{
    return m_bHasTimeCode;
}
//------------------------------------------------------------------------------------------------------------------
inline const FormatExr::TimeCode& FormatExr::GetTimeCode () const
{
    assert( m_bHasTimeCode );
    return m_qTimeCode;
}
//------------------------------------------------------------------------------------------------------------------
inline bool FormatExr::HasWrapmodes () const
{
    return m_bHasWrapmodes;
}
//------------------------------------------------------------------------------------------------------------------
inline const String& FormatExr::GetWrapmodes () const
{
    assert( m_bHasWrapmodes );
    return m_qWrapmodes;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExr::SetAlphaUnassociationHandling (bool bUnmultiplyAlpha)
{
    m_bUnmultiplyAlpha = bUnmultiplyAlpha;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatExr::SetScreenModeOn (float fExposure, float fGamma)
{
    m_bOnScreenMode = true;
    m_fOnScreenExposure = fExposure;
    m_fOnScreenGamma = fGamma;

    // precompute some helping values
    m_fOnScreenExposureCoeff = Mathf::Pow(2.0f,fExposure);
    if ( fGamma != 0.0f )
        m_fOnScreenInvGamma = 1.0f/fGamma;
}
//------------------------------------------------------------------------------------------------------------------



