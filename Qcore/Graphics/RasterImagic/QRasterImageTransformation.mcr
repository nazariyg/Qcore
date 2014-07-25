//------------------------------------------------------------------------------------------------------------------
#define SWITCH_FILTER(interpolation) \
{ \
    switch ( interpolation ) \
    { \
    case IT_BILINEAR: \
        fFilterRadius = gs_fBilinearFilterRadius; \
        oEvaluateFilter = BilinearFilter; \
        break; \
    case IT_BICUBIC: \
        fFilterRadius = gs_fBicubicFilterRadius; \
        oEvaluateFilter = BicubicFilter; \
        break; \
    case IT_CATMULL_ROM: \
        fFilterRadius = gs_fCatmullRomFilterRadius; \
        oEvaluateFilter = CatmullRomFilter; \
        break; \
    case IT_LANCZOS_3: \
        fFilterRadius = gs_fLanczos3FilterRadius; \
        oEvaluateFilter = Lanczos3Filter; \
        break; \
    case IT_B_SPLINE: \
        fFilterRadius = gs_fBSplineFilterRadius; \
        oEvaluateFilter = BSplineFilter; \
        break; \
    case IT_GAUSSIAN: \
        fFilterRadius = gs_fGaussianFilterRadius; \
        oEvaluateFilter = GaussianFilter; \
        break; \
    } \
}
//------------------------------------------------------------------------------------------------------------------
#define PREPARE_SKEW_INTERPOLATION_FOR_ROW \
{ \
    dShiftX0 = -(((double)fSkewFactor)*fDstPixCntY + fNormShiftX); \
    dShiftX1 = dShiftX0 + 0.5; \
    iInterpFromX = Mathd::RoundToInt(dShiftX1-fFilterRadius); \
    iInterpUpToX = iInterpFromX + iFilterDiam; \
    fContribSumM = fContribSumE = 0.0f; \
    for (int iSrcX = iInterpFromX, i = 0; iSrcX < iInterpUpToX; iSrcX++, i++) \
    { \
        qContribsMain[i] = oEvaluateFilter((float)dShiftX0-iSrcX); \
        qContribsEdge[i] = Mathf::Max(qContribsMain[i],0.0f); \
        fContribSumM += qContribsMain[i]; \
        fContribSumE += qContribsEdge[i]; \
    } \
    fInvContribSumM = 1.0f/fContribSumM; \
    fInvContribSumE = 1.0f/fContribSumE; \
    for (int i = 0; i < iFilterDiam; i++) \
    { \
        qContribsMain[i] *= fInvContribSumM; \
        qContribsEdge[i] *= fInvContribSumE; \
    } \
}
//------------------------------------------------------------------------------------------------------------------
#define PREPARE_SKEW_INTERPOLATION_FOR_COLUMN \
{ \
    dShiftY0 = -(((double)fSkewFactor)*fDstPixCntX + fNormShiftY); \
    dShiftY1 = dShiftY0 + 0.5; \
    iInterpFromY = Mathd::RoundToInt(dShiftY1-fFilterRadius); \
    iInterpUpToY = iInterpFromY + iFilterDiam; \
    fContribSumM = fContribSumE = 0.0f; \
    for (int iSrcY = iInterpFromY, i = 0; iSrcY < iInterpUpToY; iSrcY++, i++) \
    { \
        qContribsMain[i] = oEvaluateFilter((float)dShiftY0-iSrcY); \
        qContribsEdge[i] = Mathf::Max(qContribsMain[i],0.0f); \
        fContribSumM += qContribsMain[i]; \
        fContribSumE += qContribsEdge[i]; \
    } \
    fInvContribSumM = 1.0f/fContribSumM; \
    fInvContribSumE = 1.0f/fContribSumE; \
    for (int i = 0; i < iFilterDiam; i++) \
    { \
        qContribsMain[i] *= fInvContribSumM; \
        qContribsEdge[i] *= fInvContribSumE; \
    } \
}
//------------------------------------------------------------------------------------------------------------------
#define TRANSFORM_VISIBILITY_MASK_AS_WELL(transformationcall) \
{ \
    RasterImagePtr spqVmImage = new RasterImage(RasterImage::IT_GRAYSCALE_8,m_spqSrcImage->m_iWidth, \
        m_spqSrcImage->m_iHeight); \
 \
    unsigned char* pucSrcRow = (unsigned char*)m_spqSrcImage->m_qVisibilityMask.GetElements(); \
    unsigned char* pucSrcCell = pucSrcRow; \
    unsigned char* pucDstRow = (unsigned char*)spqVmImage->m_acImage; \
    unsigned char* pucDstCell = pucDstRow; \
    for (int iY = 0; iY < m_spqSrcImage->m_iHeight; iY++) \
    { \
        for (int iX = 0; iX < m_spqSrcImage->m_iWidth; iX++) \
            *pucDstCell++ = ( *pucSrcCell++ == 0 ? 0 : 255 ); \
 \
        pucSrcRow += m_spqSrcImage->m_iWidth; \
        pucSrcCell = pucSrcRow; \
        pucDstRow += spqVmImage->m_iRowStride; \
        pucDstCell = pucDstRow; \
    } \
 \
    RasterImageTransformation qRIT(spqVmImage); \
    spqVmImage = qRIT.transformationcall; \
    spqNewImage->m_qVisibilityMask.SetQuantity(spqNewImage->m_iArea); \
 \
    pucSrcRow = (unsigned char*)spqVmImage->m_acImage; \
    pucSrcCell = pucSrcRow; \
    pucDstRow = (unsigned char*)spqNewImage->m_qVisibilityMask.GetElements(); \
    pucDstCell = pucDstRow; \
    for (int iY = 0; iY < iNewHeight; iY++) \
    { \
        for (int iX = 0; iX < iNewWidth; iX++) \
            *pucDstCell++ = ( *pucSrcCell++ < 128 ? 0 : 1 ); \
 \
        pucSrcRow += spqVmImage->m_iRowStride; \
        pucSrcCell = pucSrcRow; \
        pucDstRow += iNewWidth; \
        pucDstCell = pucDstRow; \
    } \
}
//------------------------------------------------------------------------------------------------------------------



