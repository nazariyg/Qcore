//------------------------------------------------------------------------------------------------------------------
inline void FormatPsd::SetCompression (CompressionType eCompression)
{
    m_eCompression = eCompression;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatPsd::SetResolutionInfo (int iHorizontalResolution, int iVerticalResolution)
{
    assert( iHorizontalResolution > 0 && iVerticalResolution > 0 );

    m_iHorizontalResolution = iHorizontalResolution;
    m_iVerticalResolution = iVerticalResolution;
    m_bHasResolutionInfo = true;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatPsd::UnsetResolutionInfo ()
{
    m_bHasResolutionInfo = false;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatPsd::SetICCProfile (const StillArray<char>& rqICCProfile)
{
    m_qICCProfile = rqICCProfile;
    m_bHasICCProfile = true;
}
//------------------------------------------------------------------------------------------------------------------
inline int FormatPsd::GetCompositeImageWidth () const
{
    return m_iCIWidth;
}
//------------------------------------------------------------------------------------------------------------------
inline int FormatPsd::GetCompositeImageHeight () const
{
    return m_iCIHeight;
}
//------------------------------------------------------------------------------------------------------------------
inline int FormatPsd::GetBitsPerPixel () const
{
    return m_iChannelQuantity*m_iBitsPerChannel;
}
//------------------------------------------------------------------------------------------------------------------
inline FormatPsd::ColorModelType FormatPsd::GetColorModel () const
{
    return m_eColorModel;
}
//------------------------------------------------------------------------------------------------------------------
inline int FormatPsd::GetLayerQuantity () const
{
    return m_iLayerQuantity;
}
//------------------------------------------------------------------------------------------------------------------
inline bool FormatPsd::HasResolutionInfo () const
{
    return m_bHasResolutionInfo;
}
//------------------------------------------------------------------------------------------------------------------
inline int FormatPsd::GetHorizontalResolution () const
{
    assert( m_bHasResolutionInfo );
    return m_iHorizontalResolution;
}
//------------------------------------------------------------------------------------------------------------------
inline int FormatPsd::GetVerticalResolution () const
{
    assert( m_bHasResolutionInfo );
    return m_iVerticalResolution;
}
//------------------------------------------------------------------------------------------------------------------
inline bool FormatPsd::HasICCProfile () const
{
    return m_bHasICCProfile;
}
//------------------------------------------------------------------------------------------------------------------
inline const StillArray<char>& FormatPsd::GetICCProfile () const
{
    assert( m_bHasICCProfile );
    return m_qICCProfile;
}
//------------------------------------------------------------------------------------------------------------------



