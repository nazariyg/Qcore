//------------------------------------------------------------------------------------------------------------------
inline void FormatIff::SetCompression (CompressionType eCompression)
{
    m_eCompression = eCompression;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatIff::UnsetDpi ()
{
    m_bHasDPIChunk = false;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatIff::SetDpi (int iHorizontalDpi, int iVerticalDpi)
{
    assert( iHorizontalDpi > 0 && iVerticalDpi > 0 );

    m_iHorizontalDpi = iHorizontalDpi;
    m_iVerticalDpi = iVerticalDpi;
    m_bHasDPIChunk = true;
}
//------------------------------------------------------------------------------------------------------------------
inline int FormatIff::GetWidth () const
{
    return m_iWidth;
}
//------------------------------------------------------------------------------------------------------------------
inline int FormatIff::GetHeight () const
{
    return m_iHeight;
}
//------------------------------------------------------------------------------------------------------------------
inline int FormatIff::GetBitsPerPixel () const
{
    return m_iBitsPerPixel;
}
//------------------------------------------------------------------------------------------------------------------
inline FormatIff::ColorModelType FormatIff::GetColorModel () const
{
    return m_eColorModel;
}
//------------------------------------------------------------------------------------------------------------------
inline FormatIff::CompressionType FormatIff::GetCompression () const
{
    return m_eCompression;
}
//------------------------------------------------------------------------------------------------------------------
inline bool FormatIff::HasDPIChunk () const
{
    return m_bHasDPIChunk;
}
//------------------------------------------------------------------------------------------------------------------
inline int FormatIff::GetHorizontalDpi () const
{
    assert( m_bHasDPIChunk );
    return m_iHorizontalDpi;
}
//------------------------------------------------------------------------------------------------------------------
inline int FormatIff::GetVerticalDpi () const
{
    assert( m_bHasDPIChunk );
    return m_iVerticalDpi;
}
//------------------------------------------------------------------------------------------------------------------
inline FormatIff::PalettedImageSpecificType FormatIff::GetPalettedImageSpecific () const
{
    assert( m_eColorModel == CMT_PALETTED );
    return m_ePalettedImageSpecific;
}
//------------------------------------------------------------------------------------------------------------------



