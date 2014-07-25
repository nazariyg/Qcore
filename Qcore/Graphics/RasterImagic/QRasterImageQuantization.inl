//------------------------------------------------------------------------------------------------------------------
inline void RasterImageQuantization::SetDithering (DitheringType eDithering, float fAmount)
{
    assert( fAmount >= 0.0f );

    m_eDithering = eDithering;
    m_fDitheringAmount = fAmount;
}
//------------------------------------------------------------------------------------------------------------------
inline void RasterImageQuantization::SetPreservePaletteColorForTransparency (bool bSet)
{
    assert( m_spqImage->GetType() == RasterImage::IT_RGBA_8_8_8_8 );
    m_bPreservePaletteColorForTransparency = bSet;
}
//------------------------------------------------------------------------------------------------------------------



