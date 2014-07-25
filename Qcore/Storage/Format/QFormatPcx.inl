//------------------------------------------------------------------------------------------------------------------
inline void FormatPcx::SetResolution (int iHorizontalResolution, int iVerticalResolution)
{
    assert( 0 < iHorizontalResolution && iHorizontalResolution < 65536
        &&  0 < iVerticalResolution && iVerticalResolution < 65536 );

    m_iHorizontalResolution = iHorizontalResolution;
    m_iVerticalResolution = iVerticalResolution;
}
//------------------------------------------------------------------------------------------------------------------
inline int FormatPcx::GetWidth () const
{
    return m_iWidth;
}
//------------------------------------------------------------------------------------------------------------------
inline int FormatPcx::GetHeight () const
{
    return m_iHeight;
}
//------------------------------------------------------------------------------------------------------------------
inline int FormatPcx::GetBitsPerPixel () const
{
    return m_iBitsPerPixel;
}
//------------------------------------------------------------------------------------------------------------------
inline FormatPcx::ColorModelType FormatPcx::GetColorModel () const
{
    return m_eColorModel;
}
//------------------------------------------------------------------------------------------------------------------
inline int FormatPcx::GetHorizontalResolution () const
{
    return m_iHorizontalResolution;
}
//------------------------------------------------------------------------------------------------------------------
inline int FormatPcx::GetVerticalResolution () const
{
    return m_iVerticalResolution;
}
//------------------------------------------------------------------------------------------------------------------



