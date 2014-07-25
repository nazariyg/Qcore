//------------------------------------------------------------------------------------------------------------------
inline void FormatHdr::SetCompression (CompressionType eCompression)
{
    m_eCompression = eCompression;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatHdr::SetExposure (float fExposure)
{
    m_fExposure = fExposure;
    m_bHasExposureInfo = true;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatHdr::SetColorCorrection (float fCorrR, float fCorrG, float fCorrB)
{
    m_afColorCorr[0] = fCorrR;
    m_afColorCorr[1] = fCorrG;
    m_afColorCorr[2] = fCorrB;
    m_bHasColorCorrectionInfo = true;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatHdr::SetSoftware (const String& rqSoftware)
{
    m_qSoftware = rqSoftware;
    m_bHasSoftwareInfo = true;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatHdr::SetPixelAspectRatio (float fPixelAspectRatio)
{
    m_fPixelAspectRatio = fPixelAspectRatio;
    m_bHasPixelAspectRatioInfo = true;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatHdr::SetGamma (float fGamma)
{
    m_fGamma = fGamma;
    m_bHasGammaInfo = true;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatHdr::SetOrientation (OrientationType eOrientation)
{
    m_eOrientation = eOrientation;
}
//------------------------------------------------------------------------------------------------------------------
inline int FormatHdr::GetWidth () const
{
    return m_iWidth;
}
//------------------------------------------------------------------------------------------------------------------
inline int FormatHdr::GetHeight () const
{
    return m_iHeight;
}
//------------------------------------------------------------------------------------------------------------------
inline bool FormatHdr::HasSoftwareInfo () const
{
    return m_bHasSoftwareInfo;
}
//------------------------------------------------------------------------------------------------------------------
inline const String& FormatHdr::GetSoftware () const
{
    assert( m_bHasSoftwareInfo );
    return m_qSoftware;
}
//------------------------------------------------------------------------------------------------------------------
inline bool FormatHdr::HasPixelAspectRatioInfo () const
{
    return m_bHasPixelAspectRatioInfo;
}
//------------------------------------------------------------------------------------------------------------------
inline float FormatHdr::GetPixelAspectRatio () const
{
    assert( m_bHasPixelAspectRatioInfo );
    return m_fPixelAspectRatio;
}
//------------------------------------------------------------------------------------------------------------------
inline bool FormatHdr::HasGammaInfo () const
{
    return m_bHasGammaInfo;
}
//------------------------------------------------------------------------------------------------------------------
inline float FormatHdr::GetGamma () const
{
    assert( m_bHasGammaInfo );
    return m_fGamma;
}
//------------------------------------------------------------------------------------------------------------------
inline FormatHdr::OrientationType FormatHdr::GetOrientation () const
{
    return m_eOrientation;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatHdr::SetDoRelyOnExposureAndColorCorrection (bool bSet)
{
    m_bDoRelyOnExposureAndColorCorrection = bSet;
}
//------------------------------------------------------------------------------------------------------------------



