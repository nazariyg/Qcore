//------------------------------------------------------------------------------------------------------------------
inline RasterImage::ImageType RasterImage::GetType () const
{
    return m_eType;
}
//------------------------------------------------------------------------------------------------------------------
inline RasterImage::ColorModelType RasterImage::GetColorModel () const
{
    return m_eColorModel;
}
//------------------------------------------------------------------------------------------------------------------
inline bool RasterImage::IsHighDynamicRange () const
{
    return m_bIsHighDynamicRange;
}
//------------------------------------------------------------------------------------------------------------------
inline int RasterImage::GetWidth () const
{
    return m_iWidth;
}
//------------------------------------------------------------------------------------------------------------------
inline int RasterImage::GetHeight () const
{
    return m_iHeight;
}
//------------------------------------------------------------------------------------------------------------------
inline int RasterImage::GetRowStride () const
{
    return m_iRowStride;
}
//------------------------------------------------------------------------------------------------------------------
inline int RasterImage::GetArea () const
{
    return m_iArea;
}
//------------------------------------------------------------------------------------------------------------------
inline RasterImage::PixelComponentType RasterImage::GetPixelComponentType () const
{
    return m_ePixelComponentType;
}
//------------------------------------------------------------------------------------------------------------------
inline int RasterImage::GetComponentsPerPixel () const
{
    return m_iComponentsPerPixel;
}
//------------------------------------------------------------------------------------------------------------------
inline bool RasterImage::HasGradIntSampleUniformity () const
{
    return m_bHasGradIntSampleUniformity;
}
//------------------------------------------------------------------------------------------------------------------
inline int RasterImage::GetBitsPerIntegerSample () const
{
    assert( m_bHasGradIntSampleUniformity );
    return m_iBitsPerIntegerSample;
}
//------------------------------------------------------------------------------------------------------------------
inline int RasterImage::GetBitsPerPixel () const
{
    return m_iBitsPerPixel;
}
//------------------------------------------------------------------------------------------------------------------
inline int RasterImage::GetBytesPerPixel () const
{
    return m_iBytesPerPixel;
}
//------------------------------------------------------------------------------------------------------------------
inline bool RasterImage::HasAlpha () const
{
    return m_bHasAlpha;
}
//------------------------------------------------------------------------------------------------------------------
inline char* RasterImage::GetRow (int iRow)
{
    assert( 0 <= iRow && iRow < m_iHeight );
    return m_acImage + iRow*m_iRowStride;
}
//------------------------------------------------------------------------------------------------------------------
inline const char* RasterImage::GetRow (int iRow) const
{
    assert( 0 <= iRow && iRow < m_iHeight );
    return m_acImage + iRow*m_iRowStride;
}
//------------------------------------------------------------------------------------------------------------------
inline char* RasterImage::GetImage ()
{
    return m_acImage;
}
//------------------------------------------------------------------------------------------------------------------
inline const char* RasterImage::GetImage () const
{
    return m_acImage;
}
//------------------------------------------------------------------------------------------------------------------
inline RasterImage::operator char* ()
{
    return m_acImage;
}
//------------------------------------------------------------------------------------------------------------------
inline RasterImage::operator const char* () const
{
    return m_acImage;
}
//------------------------------------------------------------------------------------------------------------------
inline void RasterImage::SetPalette (const Palette& rqPalette)
{
    assert( m_eColorModel == CMT_PALETTED
        ||  m_eColorModel == CMT_PALETTED_WITH_ALPHA );

    m_qPalette = rqPalette;
}
//------------------------------------------------------------------------------------------------------------------
inline RasterImage::Palette& RasterImage::GetPalette ()
{
    assert( m_eColorModel == CMT_PALETTED
        ||  m_eColorModel == CMT_PALETTED_WITH_ALPHA );

    return m_qPalette;
}
//------------------------------------------------------------------------------------------------------------------
inline const RasterImage::Palette& RasterImage::GetPalette () const
{
    assert( m_eColorModel == CMT_PALETTED
        ||  m_eColorModel == CMT_PALETTED_WITH_ALPHA );

    return m_qPalette;
}
//------------------------------------------------------------------------------------------------------------------
inline int RasterImage::GetPaletteSize () const
{
    assert( m_eColorModel == CMT_PALETTED
        ||  m_eColorModel == CMT_PALETTED_WITH_ALPHA );

    return m_qPalette.GetQuantity();
}
//------------------------------------------------------------------------------------------------------------------
inline PaletteColor& RasterImage::GetPaletteColor (int i)
{
    assert( m_eColorModel == CMT_PALETTED
        ||  m_eColorModel == CMT_PALETTED_WITH_ALPHA );

    return m_qPalette[i];
}
//------------------------------------------------------------------------------------------------------------------
inline const PaletteColor& RasterImage::GetPaletteColor (int i) const
{
    assert( m_eColorModel == CMT_PALETTED
        ||  m_eColorModel == CMT_PALETTED_WITH_ALPHA );

    return m_qPalette[i];
}
//------------------------------------------------------------------------------------------------------------------
inline bool RasterImage::HasVisibilityMask () const
{
    return !m_qVisibilityMask.IsEmpty();
}
//------------------------------------------------------------------------------------------------------------------
inline bool RasterImage::IsPixelVisible (int iX, int iY) const
{
    assert( !m_qVisibilityMask.IsEmpty() );
    assert( 0 <= iX && iX < m_iWidth
        &&  0 <= iY && iY < m_iHeight );

    return ( m_qVisibilityMask[iY*m_iWidth+iX] != 0 );
}
//------------------------------------------------------------------------------------------------------------------
inline void RasterImage::SetVisibilityMask (const VisibilityMask& rqVisibilityMask)
{
    assert( m_eColorModel == CMT_PALETTED
        ||  m_eColorModel == CMT_GRAYSCALE
        ||  m_eColorModel == CMT_RGB
        ||  m_eColorModel == CMT_CMYK
        ||  m_eColorModel == CMT_LAB );
    assert( rqVisibilityMask.GetQuantity() == m_iArea );

    m_qVisibilityMask = rqVisibilityMask;
}
//------------------------------------------------------------------------------------------------------------------
inline RasterImage::VisibilityMask& RasterImage::GetVisibilityMask ()
{
    assert( m_eColorModel == CMT_PALETTED
        ||  m_eColorModel == CMT_GRAYSCALE
        ||  m_eColorModel == CMT_RGB
        ||  m_eColorModel == CMT_CMYK
        ||  m_eColorModel == CMT_LAB );

    return m_qVisibilityMask;
}
//------------------------------------------------------------------------------------------------------------------
inline const RasterImage::VisibilityMask& RasterImage::GetVisibilityMask () const
{
    assert( m_eColorModel == CMT_PALETTED
        ||  m_eColorModel == CMT_GRAYSCALE
        ||  m_eColorModel == CMT_RGB
        ||  m_eColorModel == CMT_CMYK
        ||  m_eColorModel == CMT_LAB );

    return m_qVisibilityMask;
}
//------------------------------------------------------------------------------------------------------------------
inline void RasterImage::RemoveVisibilityMask ()
{
    assert( m_eColorModel == CMT_PALETTED
        ||  m_eColorModel == CMT_GRAYSCALE
        ||  m_eColorModel == CMT_RGB
        ||  m_eColorModel == CMT_CMYK
        ||  m_eColorModel == CMT_LAB );

    m_qVisibilityMask.RemoveAll();
}
//------------------------------------------------------------------------------------------------------------------



