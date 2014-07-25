//------------------------------------------------------------------------------------------------------------------
inline void RasterImageTransformation::SetOuterColor (const OuterColor& rqOuterColor)
{
    assert( !((m_spqSrcImage->m_eColorModel == RasterImage::CMT_PALETTED
        ||  m_spqSrcImage->m_eColorModel == RasterImage::CMT_PALETTED_WITH_ALPHA)
        &&  !(0 <= rqOuterColor.Index && rqOuterColor.Index < m_spqSrcImage->m_qPalette.GetQuantity())) );

    m_qOuterColor = rqOuterColor;
}
//------------------------------------------------------------------------------------------------------------------



