//------------------------------------------------------------------------------------------------------------------
inline void FormatTga::SetCompression (CompressionType eCompression)
{
    m_eCompression = eCompression;
}
//------------------------------------------------------------------------------------------------------------------
inline int FormatTga::GetWidth () const
{
    return m_iWidth;
}
//------------------------------------------------------------------------------------------------------------------
inline int FormatTga::GetHeight () const
{
    return m_iHeight;
}
//------------------------------------------------------------------------------------------------------------------
inline int FormatTga::GetBitsPerPixel () const
{
    return m_iBitsPerPixel;
}
//------------------------------------------------------------------------------------------------------------------
inline FormatTga::ColorModelType FormatTga::GetColorModel () const
{
    switch ( m_eImage )
    {
    case IT_PALETTED:
        return CMT_PALETTED;
    case IT_CHANNELED:
        return ( m_iBitsPerPixel != 32 ? CMT_RGB : CMT_RGBA );
    default:  // IT_GRAYSCALE
        return CMT_GRAYSCALE;
    }
}
//------------------------------------------------------------------------------------------------------------------
inline FormatTga::CompressionType FormatTga::GetCompression () const
{
    return m_eCompression;
}
//------------------------------------------------------------------------------------------------------------------



