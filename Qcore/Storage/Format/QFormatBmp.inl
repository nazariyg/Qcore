//------------------------------------------------------------------------------------------------------------------
inline void FormatBmp::SetOs (OsType eOs)
{
    m_eOs = eOs;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatBmp::SetCompression (CompressionType eCompression)
{
    m_eCompression = eCompression;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatBmp::SetBitMasks (unsigned int uiMaskR, unsigned int uiMaskG, unsigned int uiMaskB)
{
    m_uiRedMask = uiMaskR;
    m_uiGreenMask = uiMaskG;
    m_uiBlueMask = uiMaskB;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatBmp::SetPixelsPerMeterHorizontally (int iResolution)
{
    assert( iResolution > 0 );
    m_iPixelsPerMeterHorizontally = iResolution;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatBmp::SetPixelsPerMeterVertically (int iResolution)
{
    assert( iResolution > 0 );
    m_iPixelsPerMeterVertically = iResolution;
}
//------------------------------------------------------------------------------------------------------------------
inline int FormatBmp::GetWidth () const
{
    return m_iWidth;
}
//------------------------------------------------------------------------------------------------------------------
inline int FormatBmp::GetHeight () const
{
    return m_iHeight;
}
//------------------------------------------------------------------------------------------------------------------
inline int FormatBmp::GetBitsPerPixel () const
{
    return m_iBitsPerPixel;
}
//------------------------------------------------------------------------------------------------------------------
inline FormatBmp::OsType FormatBmp::GetOs () const
{
    return m_eOs;
}
//------------------------------------------------------------------------------------------------------------------
inline FormatBmp::ColorModelType FormatBmp::GetColorModel () const
{
    // derive from bpp
    return ( m_iBitsPerPixel == 1 || m_iBitsPerPixel == 4 || m_iBitsPerPixel == 8 ? CMT_PALETTED : CMT_RGB );
}
//------------------------------------------------------------------------------------------------------------------
inline FormatBmp::CompressionType FormatBmp::GetCompression () const
{
    return m_eCompression;
}
//------------------------------------------------------------------------------------------------------------------
inline unsigned int FormatBmp::GetRedMask () const
{
    assert( m_eCompression == CT_BITFIELDS );
    return m_uiRedMask;
}
//------------------------------------------------------------------------------------------------------------------
inline unsigned int FormatBmp::GetGreenMask () const
{
    assert( m_eCompression == CT_BITFIELDS );
    return m_uiGreenMask;
}
//------------------------------------------------------------------------------------------------------------------
inline unsigned int FormatBmp::GetBlueMask () const
{
    assert( m_eCompression == CT_BITFIELDS );
    return m_uiBlueMask;
}
//------------------------------------------------------------------------------------------------------------------
inline int FormatBmp::GetPixelsPerMeterHorizontally () const
{
    return m_iPixelsPerMeterHorizontally;
}
//------------------------------------------------------------------------------------------------------------------
inline int FormatBmp::GetPixelsPerMeterVertically () const
{
    return m_iPixelsPerMeterVertically;
}
//------------------------------------------------------------------------------------------------------------------
inline int FormatBmp::GetDevelopedRowQuantity () const
{
    assert( m_eStreaming == Stream::ST_READING_FROM_DYNAMIC_BYTESTREAM );
    return m_iDevelopedRowQuantity;
}
//------------------------------------------------------------------------------------------------------------------



