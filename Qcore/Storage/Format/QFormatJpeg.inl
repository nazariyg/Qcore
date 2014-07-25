//------------------------------------------------------------------------------------------------------------------
inline void FormatJpeg::SetColorModel (ColorModelType eColorModel)
{
    m_eColorModel = eColorModel;
    m_bColorModelPrecised = true;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatJpeg::SetQuality (float fQuality)
{
    assert( 0.0f <= fQuality && fQuality <= 1.0f );
    m_fQuality = fQuality;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatJpeg::SetYCbCrChromaSampling (YCbCrChromaSamplingType eYCbCrChromaSampling)
{
    m_eYCbCrChromaSampling = eYCbCrChromaSampling;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatJpeg::SetDataOrganization (
    DataOrganizationType eDataOrganization)
{
    m_eDataOrganization = eDataOrganization;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatJpeg::SetProgressiveScanQuantity (int iQuantity)
{
    assert( 3 <= iQuantity && iQuantity <= 5 );
    m_iProgressiveScanQuantity = iQuantity;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatJpeg::SetJfifMarker (JfifDotDensityMeaningType eJfifDotDensityMeaning,
    int iJfifDotDensityHorizontally, int iJfifDotDensityVertically)
{
    assert( iJfifDotDensityHorizontally >= 0 && iJfifDotDensityVertically >= 0 );

    m_eJfifDotDensityMeaning = eJfifDotDensityMeaning;
    m_iJfifDotDensityHorizontally = iJfifDotDensityHorizontally;
    m_iJfifDotDensityVertically = iJfifDotDensityVertically;
    m_bHasJfifMarker = true;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatJpeg::UnsetJfifMarker ()
{
    assert( m_bHasJfifMarker );
    m_bHasJfifMarker = false;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatJpeg::UnsetAdobeMarker ()
{
    m_bHasAdobeMarker = false;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatJpeg::SetExifMetadata (const FormatExifPtr& rspqExifMetadata)
{
    m_spqExifMetadata = rspqExifMetadata;
    m_bHasExifMetadata = true;
}
//------------------------------------------------------------------------------------------------------------------
inline int FormatJpeg::GetWidth () const
{
    return m_iWidth;
}
//------------------------------------------------------------------------------------------------------------------
inline int FormatJpeg::GetHeight () const
{
    return m_iHeight;
}
//------------------------------------------------------------------------------------------------------------------
inline int FormatJpeg::GetBitsPerPixel () const
{
    return m_iBitsPerPixel;
}
//------------------------------------------------------------------------------------------------------------------
inline FormatJpeg::ColorModelType FormatJpeg::GetColorModel () const
{
    return m_eColorModel;
}
//------------------------------------------------------------------------------------------------------------------
inline FormatJpeg::DataOrganizationType FormatJpeg::GetDataOrganization () const
{
    switch ( m_eProcess )
    {
    case PT_BASELINE_DCT:
        return DOT_PLANE;
    case PT_EXTENDED_DCT:
        return DOT_PLANE;
    default:  // PT_PROGRESSIVE_DCT
        return DOT_PROGRESSIVE;
    }
}
//------------------------------------------------------------------------------------------------------------------
inline bool FormatJpeg::HasJfifMarker () const
{
    return m_bHasJfifMarker;
}
//------------------------------------------------------------------------------------------------------------------
inline FormatJpeg::JfifDotDensityMeaningType FormatJpeg::GetJfifDotDensityMeaning () const
{
    return m_eJfifDotDensityMeaning;
}
//------------------------------------------------------------------------------------------------------------------
inline int FormatJpeg::GetJfifDotDensityHorizontally () const
{
    return m_iJfifDotDensityHorizontally;
}
//------------------------------------------------------------------------------------------------------------------
inline int FormatJpeg::GetJfifDotDensityVertically () const
{
    return m_iJfifDotDensityVertically;
}
//------------------------------------------------------------------------------------------------------------------
inline bool FormatJpeg::HasExifMetadata () const
{
    return m_bHasExifMetadata;
}
//------------------------------------------------------------------------------------------------------------------
inline const FormatExif& FormatJpeg::GetExifMetadata () const
{
    assert( m_bHasExifMetadata );
    return *m_spqExifMetadata;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatJpeg::SetUpsamplingFilter (UpsamplingFilterType eUpsamplingFilter)
{
    m_eUpsamplingFilter = eUpsamplingFilter;
}
//------------------------------------------------------------------------------------------------------------------
inline int FormatJpeg::GetDevelopedRowQuantity () const
{
    assert( m_eStreaming == Stream::ST_READING_FROM_DYNAMIC_BYTESTREAM );
    return m_iDevelopedRowQuantity;
}
//------------------------------------------------------------------------------------------------------------------
inline int FormatJpeg::GetCurrentPassIndex () const
{
    assert( m_eStreaming == Stream::ST_READING_FROM_DYNAMIC_BYTESTREAM );
    return m_iCurrentPassIndex;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatJpeg::DoExtendProc (int& riV, int iV, int iT)
{
    riV = ( iV < 1 << (iT - 1) ? iV + (-1 << iT) + 1 : iV );
}
//------------------------------------------------------------------------------------------------------------------



