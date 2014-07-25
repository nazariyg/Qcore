//------------------------------------------------------------------------------------------------------------------
inline void RasterImageConversion::SetMatte (const Matte& rqMatte)
{
#ifdef _DEBUG
    RasterImage::ColorModelType eSourceColorModel = m_spqSrcImage->GetColorModel();
    RasterImage::ColorModelType eTargetColorModel = RasterImage::GetColorModelFromType(m_eTargetType);
    assert( (eSourceColorModel == RasterImage::CMT_PALETTED_WITH_ALPHA
        ||  eSourceColorModel == RasterImage::CMT_GRAYSCALE_WITH_ALPHA
        ||  eSourceColorModel == RasterImage::CMT_RGBA
        ||  eSourceColorModel == RasterImage::CMT_CMYK_WITH_ALPHA
        ||  eSourceColorModel == RasterImage::CMT_LAB_WITH_ALPHA)
        &&  (eTargetColorModel == RasterImage::CMT_PALETTED
        ||  eTargetColorModel == RasterImage::CMT_GRAYSCALE
        ||  eTargetColorModel == RasterImage::CMT_RGB
        ||  eTargetColorModel == RasterImage::CMT_CMYK
        ||  eTargetColorModel == RasterImage::CMT_LAB) );
#endif
    m_qMatte = rqMatte;
}
//------------------------------------------------------------------------------------------------------------------
inline void RasterImageConversion::SetTryToHoldTransparency (bool bSet)
{
#ifdef _DEBUG
    RasterImage::ColorModelType eSourceColorModel = m_spqSrcImage->GetColorModel();
    RasterImage::ColorModelType eTargetColorModel = RasterImage::GetColorModelFromType(m_eTargetType);
    assert( ((eSourceColorModel == RasterImage::CMT_PALETTED_WITH_ALPHA
        ||  eSourceColorModel == RasterImage::CMT_GRAYSCALE_WITH_ALPHA
        ||  eSourceColorModel == RasterImage::CMT_RGBA
        ||  eSourceColorModel == RasterImage::CMT_CMYK_WITH_ALPHA
        ||  eSourceColorModel == RasterImage::CMT_LAB_WITH_ALPHA)
        &&  (eTargetColorModel == RasterImage::CMT_PALETTED
        ||  eTargetColorModel == RasterImage::CMT_GRAYSCALE
        ||  eTargetColorModel == RasterImage::CMT_RGB
        ||  eTargetColorModel == RasterImage::CMT_CMYK
        ||  eTargetColorModel == RasterImage::CMT_LAB))
        ||  (m_spqSrcImage->HasVisibilityMask()
        &&  (eTargetColorModel == RasterImage::CMT_PALETTED_WITH_ALPHA
        ||  eTargetColorModel == RasterImage::CMT_GRAYSCALE_WITH_ALPHA
        ||  eTargetColorModel == RasterImage::CMT_RGBA
        ||  eTargetColorModel == RasterImage::CMT_CMYK_WITH_ALPHA
        ||  eTargetColorModel == RasterImage::CMT_LAB_WITH_ALPHA)) );
#endif
    m_bTryToHoldTransparency = bSet;
}
//------------------------------------------------------------------------------------------------------------------



