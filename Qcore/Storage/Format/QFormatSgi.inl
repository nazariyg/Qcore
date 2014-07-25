//------------------------------------------------------------------------------------------------------------------
inline void FormatSgi::SetCompression (CompressionType eCompression)
{
    m_eCompression = eCompression;
}
//------------------------------------------------------------------------------------------------------------------
inline int FormatSgi::GetWidth () const
{
    return m_iWidth;
}
//------------------------------------------------------------------------------------------------------------------
inline int FormatSgi::GetHeight () const
{
    return m_iHeight;
}
//------------------------------------------------------------------------------------------------------------------
inline int FormatSgi::GetBitsPerPixel () const
{
    return m_iBitsPerPixel;
}
//------------------------------------------------------------------------------------------------------------------
inline FormatSgi::ColorModelType FormatSgi::GetColorModel () const
{
    return m_eColorModel;
}
//------------------------------------------------------------------------------------------------------------------
inline FormatSgi::CompressionType FormatSgi::GetCompression () const
{
    return m_eCompression;
}
//------------------------------------------------------------------------------------------------------------------



