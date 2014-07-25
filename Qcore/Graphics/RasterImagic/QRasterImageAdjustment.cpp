#include "QRasterImageAdjustment.h"
#include "QProgressProbe.h"
using namespace Q;

//------------------------------------------------------------------------------------------------------------------
RasterImageAdjustment::RasterImageAdjustment (const RasterImagePtr& rspqImage)
    :
    m_spqSrcImage(rspqImage)
{
}
//------------------------------------------------------------------------------------------------------------------
RasterImagePtr RasterImageAdjustment::GetWithBrightness (float fBrightness) const
{
    assert( -1.0f <= fBrightness && fBrightness <= 1.0f );

    if ( fBrightness == 0.0f )
    {
        // nothing to do
        return m_spqSrcImage->GetDuplicate();
    }

    // allocate output image
    RasterImagePtr spqNewImage = m_spqSrcImage->GetDuplicate();

    // mapper in the range [0.0, 1.0]; 100/256 is used by Adobe Photoshop
    static const float s_fMap = ((float)100)/256;

    // adjust brightness value
    float fBr = fBrightness*s_fMap;
    if ( spqNewImage->m_eColorModel == RasterImage::CMT_CMYK
    ||   spqNewImage->m_eColorModel == RasterImage::CMT_CMYK_WITH_ALPHA )
    {
        fBr = -fBr;
    }

    // decide if to generate a lookup table
    bool bLut = false;
    int iPrec;
    if ( spqNewImage->m_eColorModel == RasterImage::CMT_PALETTED
    ||   spqNewImage->m_eColorModel == RasterImage::CMT_PALETTED_WITH_ALPHA )
    {
        bLut = true;
        iPrec = 8;
    }
    else if ( spqNewImage->m_bHasGradIntSampleUniformity
         &&   (spqNewImage->m_iBitsPerIntegerSample == 8 || spqNewImage->m_iBitsPerIntegerSample == 16) )
    {
        bLut = true;
        iPrec = spqNewImage->m_iBitsPerIntegerSample;
    }

    StillArray<unsigned char> qLutUc;
    StillArray<unsigned short> qLutUs;
    if ( bLut )
    {
        // generate LUT
        int iShift, iShVal;
        if ( iPrec == 8 )
        {
            qLutUc.SetQuantity(256);

            iShift = Mathf::RoundToInt(fBr*256);
            for (int i = 0; i < 256; i++)
            {
                iShVal = i + iShift;
                if ( iShVal < 0 )
                    iShVal = 0;
                else if ( iShVal > 255 )
                    iShVal = 255;
                qLutUc[i] = iShVal;
            }
        }
        else  // iPrec = 16
        {
            qLutUs.SetQuantity(65536);

            iShift = Mathf::RoundToInt(fBr*65536);
            for (int i = 0; i < 65536; i++)
            {
                iShVal = i + iShift;
                if ( iShVal < 0 )
                    iShVal = 0;
                else if ( iShVal > 65535 )
                    iShVal = 65535;
                qLutUs[i] = iShVal;
            }
        }
    }

    // do the adjustment
    if ( spqNewImage->m_eColorModel == RasterImage::CMT_PALETTED
    ||   spqNewImage->m_eColorModel == RasterImage::CMT_PALETTED_WITH_ALPHA )
    {
        // paletted is the special case with LUT
        for (int i0 = 0; i0 < spqNewImage->m_qPalette.GetQuantity(); i0++)
        {
            PaletteColor& rqPalC = spqNewImage->m_qPalette[i0];
            for (int i1 = 0; i1 < 3; i1++)
                rqPalC.Components[i1] = qLutUc[(int)rqPalC.Components[i1]];
        }
    }
    else
    {
        if ( bLut )
        {
            // using LUT
            unsigned char* pucRow = (unsigned char*)spqNewImage->m_acImage;
            unsigned char* aucPix = pucRow;
            if ( spqNewImage->m_eColorModel != RasterImage::CMT_LAB
            &&   spqNewImage->m_eColorModel != RasterImage::CMT_LAB_WITH_ALPHA )
            {
                int iCSQuantity = spqNewImage->m_iComponentsPerPixel;
                if ( spqNewImage->m_bHasAlpha )
                    iCSQuantity--;

                if ( iPrec == 8 )
                {
                    for (int iY = 0; iY < spqNewImage->m_iHeight; iY++)
                    {
                        for (int iX = 0; iX < spqNewImage->m_iWidth; iX++)
                        {
                            for (int i = 0; i < iCSQuantity; i++)
                                *(aucPix + i) = qLutUc[(int)*(aucPix+i)];
                            aucPix += spqNewImage->m_iComponentsPerPixel;
                        }

                        pucRow += spqNewImage->m_iRowStride;
                        aucPix = pucRow;
                    }
                }
                else  // iPrec = 16
                {
                    int iSPPM2 = spqNewImage->m_iComponentsPerPixel*2;
                    for (int iY = 0; iY < spqNewImage->m_iHeight; iY++)
                    {
                        for (int iX = 0; iX < spqNewImage->m_iWidth; iX++)
                        {
                            for (int i = 0; i < iCSQuantity; i++)
                                MemCpy(aucPix+i*2,&qLutUs[(int)*((unsigned short*)(aucPix+i*2))],2);
                            aucPix += iSPPM2;
                        }

                        pucRow += spqNewImage->m_iRowStride;
                        aucPix = pucRow;
                    }
                }
            }
            else
            {
                // L*a*b* is the special case
                if ( spqNewImage->m_eColorModel == RasterImage::CMT_LAB )
                {
                    if ( spqNewImage->m_eType == RasterImage::IT_LAB_8_8_8 )
                    {
                        for (int iY = 0; iY < spqNewImage->m_iHeight; iY++)
                        {
                            for (int iX = 0; iX < spqNewImage->m_iWidth; iX++)
                            {
                                *aucPix = qLutUc[(int)*aucPix];
                                aucPix += 3;
                            }

                            pucRow += spqNewImage->m_iRowStride;
                            aucPix = pucRow;
                        }
                    }
                    else  // spqNewImage->m_eType = RasterImage::IT_LAB_16_16_16
                    {
                        for (int iY = 0; iY < spqNewImage->m_iHeight; iY++)
                        {
                            for (int iX = 0; iX < spqNewImage->m_iWidth; iX++)
                            {
                                MemCpy(aucPix,&qLutUs[(int)*((unsigned short*)aucPix)],2);
                                aucPix += 6;
                            }

                            pucRow += spqNewImage->m_iRowStride;
                            aucPix = pucRow;
                        }
                    }
                }
                else  // spqNewImage->m_eColorModel = RasterImage::CMT_LAB_WITH_ALPHA
                {
                    if ( spqNewImage->m_eType == RasterImage::IT_LAB_WITH_ALPHA_8_8_8_8 )
                    {
                        for (int iY = 0; iY < spqNewImage->m_iHeight; iY++)
                        {
                            for (int iX = 0; iX < spqNewImage->m_iWidth; iX++)
                            {
                                *aucPix = qLutUc[(int)*aucPix];
                                aucPix += 4;
                            }

                            pucRow += spqNewImage->m_iRowStride;
                            aucPix = pucRow;
                        }
                    }
                    else  // spqNewImage->m_eType = RasterImage::IT_LAB_WITH_ALPHA_16_16_16_16
                    {
                        for (int iY = 0; iY < spqNewImage->m_iHeight; iY++)
                        {
                            for (int iX = 0; iX < spqNewImage->m_iWidth; iX++)
                            {
                                MemCpy(aucPix,&qLutUs[(int)*((unsigned short*)aucPix)],2);
                                aucPix += 8;
                            }

                            pucRow += spqNewImage->m_iRowStride;
                            aucPix = pucRow;
                        }
                    }
                }
            }
        }
        else
        {
            // without LUT
            if ( spqNewImage->m_eColorModel == RasterImage::CMT_GRAYSCALE )
            {
                float fIntensity;
                for (int iY = 0; iY < spqNewImage->m_iHeight; iY++)
                {
                    for (int iX = 0; iX < spqNewImage->m_iWidth; iX++)
                    {
                        fIntensity = spqNewImage->GetPixelIntensity(iX,iY) + fBr;
                        if ( !spqNewImage->m_bIsHighDynamicRange )
                        {
                            if ( fIntensity < 0.0f )
                                fIntensity = 0.0f;
                            else if ( fIntensity > 1.0f )
                                fIntensity = 1.0f;
                        }
                        spqNewImage->SetPixelIntensity(fIntensity,iX,iY);
                    }
                }
            }
            else if ( spqNewImage->m_eColorModel == RasterImage::CMT_GRAYSCALE_WITH_ALPHA )
            {
                Link<float,float> qIA;
                for (int iY = 0; iY < spqNewImage->m_iHeight; iY++)
                {
                    for (int iX = 0; iX < spqNewImage->m_iWidth; iX++)
                    {
                        qIA = spqNewImage->GetPixelIntensityWithAlpha(iX,iY);
                        qIA.First += fBr;
                        if ( !spqNewImage->m_bIsHighDynamicRange )
                        {
                            if ( qIA.First < 0.0f )
                                qIA.First = 0.0f;
                            else if ( qIA.First > 1.0f )
                                qIA.First = 1.0f;
                        }
                        spqNewImage->SetPixelIntensityWithAlpha(qIA,iX,iY);
                    }
                }
            }
            else if ( spqNewImage->m_eColorModel == RasterImage::CMT_RGB )
            {
                ColorRgb qRgb;
                for (int iY = 0; iY < spqNewImage->m_iHeight; iY++)
                {
                    for (int iX = 0; iX < spqNewImage->m_iWidth; iX++)
                    {
                        qRgb = spqNewImage->GetPixelRgb(iX,iY);
                        for (int i = 0; i < 3; i++)
                            qRgb[i] += fBr;
                        if ( !spqNewImage->m_bIsHighDynamicRange )
                            qRgb.Clamp();
                        spqNewImage->SetPixelRgb(qRgb,iX,iY);
                    }
                }
            }
            else if ( spqNewImage->m_eColorModel == RasterImage::CMT_RGBA )
            {
                ColorRgba qRgba;
                for (int iY = 0; iY < spqNewImage->m_iHeight; iY++)
                {
                    for (int iX = 0; iX < spqNewImage->m_iWidth; iX++)
                    {
                        qRgba = spqNewImage->GetPixelRgba(iX,iY);
                        for (int i = 0; i < 3; i++)
                            qRgba[i] += fBr;
                        if ( !spqNewImage->m_bIsHighDynamicRange )
                            qRgba.Clamp();
                        spqNewImage->SetPixelRgba(qRgba,iX,iY);
                    }
                }
            }
            else if ( spqNewImage->m_eColorModel == RasterImage::CMT_CMYK )
            {
                ColorCmyk qCmyk;
                for (int iY = 0; iY < spqNewImage->m_iHeight; iY++)
                {
                    for (int iX = 0; iX < spqNewImage->m_iWidth; iX++)
                    {
                        qCmyk = spqNewImage->GetPixelCmyk(iX,iY);
                        for (int i = 0; i < 4; i++)
                            qCmyk[i] += fBr;
                        if ( !spqNewImage->m_bIsHighDynamicRange )
                            qCmyk.Clamp();
                        spqNewImage->SetPixelCmyk(qCmyk,iX,iY);
                    }
                }
            }
            else if ( spqNewImage->m_eColorModel == RasterImage::CMT_CMYK_WITH_ALPHA )
            {
                ColorCmyka qCmyka;
                for (int iY = 0; iY < spqNewImage->m_iHeight; iY++)
                {
                    for (int iX = 0; iX < spqNewImage->m_iWidth; iX++)
                    {
                        qCmyka = spqNewImage->GetPixelCmyka(iX,iY);
                        for (int i = 0; i < 4; i++)
                            qCmyka[i] += fBr;
                        if ( !spqNewImage->m_bIsHighDynamicRange )
                            qCmyka.Clamp();
                        spqNewImage->SetPixelCmyka(qCmyka,iX,iY);
                    }
                }
            }
            else if ( spqNewImage->m_eColorModel == RasterImage::CMT_LAB )
            {
                fBr *= 100.0f;
                ColorLab qLab;
                for (int iY = 0; iY < spqNewImage->m_iHeight; iY++)
                {
                    for (int iX = 0; iX < spqNewImage->m_iWidth; iX++)
                    {
                        qLab = spqNewImage->GetPixelLab(iX,iY);
                        qLab.L() += fBr;
                        if ( !spqNewImage->m_bIsHighDynamicRange )
                        {
                            if ( qLab.L() < 0.0f )
                                qLab.L() = 0.0f;
                            else if ( qLab.L() > 100.0f )
                                qLab.L() = 100.0f;
                        }
                        spqNewImage->SetPixelLab(qLab,iX,iY);
                    }
                }
            }
            else  // spqNewImage->m_eColorModel = RasterImage::CMT_LAB_WITH_ALPHA
            {
                fBr *= 100.0f;
                ColorLaba qLaba;
                for (int iY = 0; iY < spqNewImage->m_iHeight; iY++)
                {
                    for (int iX = 0; iX < spqNewImage->m_iWidth; iX++)
                    {
                        qLaba = spqNewImage->GetPixelLaba(iX,iY);
                        qLaba.L() += fBr;
                        if ( !spqNewImage->m_bIsHighDynamicRange )
                        {
                            if ( qLaba.L() < 0.0f )
                                qLaba.L() = 0.0f;
                            else if ( qLaba.L() > 100.0f )
                                qLaba.L() = 100.0f;
                        }
                        spqNewImage->SetPixelLaba(qLaba,iX,iY);
                    }
                }
            }
        }
    }

    return spqNewImage;
}
//------------------------------------------------------------------------------------------------------------------
RasterImagePtr RasterImageAdjustment::GetWithContrast (float fContrast) const
{
    assert( -1.0f <= fContrast && fContrast <= 1.0f );

    if ( fContrast == 0.0f )
    {
        // nothing to do
        return m_spqSrcImage->GetDuplicate();
    }

    // allocate output image
    RasterImagePtr spqNewImage = m_spqSrcImage->GetDuplicate();

    // to avoid infinity in further computations
    static const float s_fMaxContrast = 0.999f;
    if ( fContrast > s_fMaxContrast )
        fContrast = s_fMaxContrast;

    // transform contrast value
    float fCon = 1.0f + ( fContrast < 0.0f ? fContrast : fContrast/(1.0f - fContrast) );

    static const int s_iMaxCS = 4;  // CMYK currently does the maximal number of color samples
    static const float s_fMul8 = 1.0f/255;
    static const float s_fMul16 = 1.0f/65535;
    float afAv[s_iMaxCS] = {0.0f, 0.0f, 0.0f, 0.0f};
    int iVisArea = 0;
    bool bLut = false;
    int iPrec, iCSQuantity;
    unsigned char* pucRow;
    unsigned char* aucPix;

    if ( spqNewImage->m_eColorModel != RasterImage::CMT_PALETTED
    &&   spqNewImage->m_eColorModel != RasterImage::CMT_PALETTED_WITH_ALPHA )
    {
        iCSQuantity = spqNewImage->m_iComponentsPerPixel;
        if ( spqNewImage->m_bHasAlpha )
            iCSQuantity--;
    }

    // find average color over the image and decide if to generate a lookup table
    if ( spqNewImage->m_eColorModel == RasterImage::CMT_PALETTED
    ||   spqNewImage->m_eColorModel == RasterImage::CMT_PALETTED_WITH_ALPHA )
    {
        pucRow = (unsigned char*)spqNewImage->m_acImage;
        aucPix = pucRow;
        for (int iY = 0; iY < spqNewImage->m_iHeight; iY++)
        {
            for (int iX = 0; iX < spqNewImage->m_iWidth; iX++)
            {
                if ( spqNewImage->m_qVisibilityMask.IsEmpty() || spqNewImage->IsPixelVisible(iX,iY) )
                {
                    const PaletteColor& rqPalC = spqNewImage->m_qPalette[(int)*aucPix];
                    aucPix++;
                    for (int i = 0; i < 3; i++)
                        afAv[i] += s_fMul8*rqPalC.Components[i];

                    iVisArea++;
                }
            }

            pucRow += spqNewImage->m_iRowStride;
            aucPix = pucRow;
        }

        bLut = true;
        iPrec = 8;
    }
    else if ( spqNewImage->m_bHasGradIntSampleUniformity
         &&   (spqNewImage->m_iBitsPerIntegerSample == 8 || spqNewImage->m_iBitsPerIntegerSample == 16) )
    {
        pucRow = (unsigned char*)spqNewImage->m_acImage;
        aucPix = pucRow;
        if ( spqNewImage->m_iBitsPerIntegerSample == 8 )
        {
            for (int iY = 0; iY < spqNewImage->m_iHeight; iY++)
            {
                for (int iX = 0; iX < spqNewImage->m_iWidth; iX++)
                {
                    if ( spqNewImage->m_qVisibilityMask.IsEmpty() || spqNewImage->IsPixelVisible(iX,iY) )
                    {
                        for (int i = 0; i < iCSQuantity; i++)
                            afAv[i] += s_fMul8*(*(aucPix + i));
                        aucPix += spqNewImage->m_iComponentsPerPixel;

                        iVisArea++;
                    }
                }

                pucRow += spqNewImage->m_iRowStride;
                aucPix = pucRow;
            }
        }
        else  // spqNewImage->m_iBitsPerIntegerSample = 16
        {
            int iSPPM2 = spqNewImage->m_iComponentsPerPixel*2;
            for (int iY = 0; iY < spqNewImage->m_iHeight; iY++)
            {
                for (int iX = 0; iX < spqNewImage->m_iWidth; iX++)
                {
                    if ( spqNewImage->m_qVisibilityMask.IsEmpty() || spqNewImage->IsPixelVisible(iX,iY) )
                    {
                        for (int i = 0; i < iCSQuantity; i++)
                            afAv[i] += s_fMul16*(*((unsigned short*)(aucPix + i*2)));
                        aucPix += iSPPM2;

                        iVisArea++;
                    }
                }

                pucRow += spqNewImage->m_iRowStride;
                aucPix = pucRow;
            }
        }

        bLut = true;
        iPrec = spqNewImage->m_iBitsPerIntegerSample;
    }
    else
    {
        if ( spqNewImage->m_eColorModel == RasterImage::CMT_GRAYSCALE )
        {
            float fIntensity;
            for (int iY = 0; iY < spqNewImage->m_iHeight; iY++)
            {
                for (int iX = 0; iX < spqNewImage->m_iWidth; iX++)
                {
                    if ( spqNewImage->m_qVisibilityMask.IsEmpty() || spqNewImage->IsPixelVisible(iX,iY) )
                    {
                        fIntensity = spqNewImage->GetPixelIntensity(iX,iY);
                        afAv[0] += fIntensity;

                        iVisArea++;
                    }
                }
            }
        }
        else if ( spqNewImage->m_eColorModel == RasterImage::CMT_GRAYSCALE_WITH_ALPHA )
        {
            Link<float,float> qIA;
            for (int iY = 0; iY < spqNewImage->m_iHeight; iY++)
            {
                for (int iX = 0; iX < spqNewImage->m_iWidth; iX++)
                {
                    if ( spqNewImage->m_qVisibilityMask.IsEmpty() || spqNewImage->IsPixelVisible(iX,iY) )
                    {
                        qIA = spqNewImage->GetPixelIntensityWithAlpha(iX,iY);
                        afAv[0] += qIA.First;

                        iVisArea++;
                    }
                }
            }
        }
        else if ( spqNewImage->m_eColorModel == RasterImage::CMT_RGB )
        {
            ColorRgb qRgb;
            for (int iY = 0; iY < spqNewImage->m_iHeight; iY++)
            {
                for (int iX = 0; iX < spqNewImage->m_iWidth; iX++)
                {
                    if ( spqNewImage->m_qVisibilityMask.IsEmpty() || spqNewImage->IsPixelVisible(iX,iY) )
                    {
                        qRgb = spqNewImage->GetPixelRgb(iX,iY);
                        for (int i = 0; i < 3; i++)
                            afAv[i] += qRgb[i];

                        iVisArea++;
                    }
                }
            }
        }
        else if ( spqNewImage->m_eColorModel == RasterImage::CMT_RGBA )
        {
            ColorRgba qRgba;
            for (int iY = 0; iY < spqNewImage->m_iHeight; iY++)
            {
                for (int iX = 0; iX < spqNewImage->m_iWidth; iX++)
                {
                    if ( spqNewImage->m_qVisibilityMask.IsEmpty() || spqNewImage->IsPixelVisible(iX,iY) )
                    {
                        qRgba = spqNewImage->GetPixelRgba(iX,iY);
                        for (int i = 0; i < 3; i++)
                            afAv[i] += qRgba[i];

                        iVisArea++;
                    }
                }
            }
        }
        else if ( spqNewImage->m_eColorModel == RasterImage::CMT_CMYK )
        {
            ColorCmyk qCmyk;
            for (int iY = 0; iY < spqNewImage->m_iHeight; iY++)
            {
                for (int iX = 0; iX < spqNewImage->m_iWidth; iX++)
                {
                    if ( spqNewImage->m_qVisibilityMask.IsEmpty() || spqNewImage->IsPixelVisible(iX,iY) )
                    {
                        qCmyk = spqNewImage->GetPixelCmyk(iX,iY);
                        for (int i = 0; i < 4; i++)
                            afAv[i] += qCmyk[i];

                        iVisArea++;
                    }
                }
            }
        }
        else if ( spqNewImage->m_eColorModel == RasterImage::CMT_CMYK_WITH_ALPHA )
        {
            ColorCmyka qCmyka;
            for (int iY = 0; iY < spqNewImage->m_iHeight; iY++)
            {
                for (int iX = 0; iX < spqNewImage->m_iWidth; iX++)
                {
                    if ( spqNewImage->m_qVisibilityMask.IsEmpty() || spqNewImage->IsPixelVisible(iX,iY) )
                    {
                        qCmyka = spqNewImage->GetPixelCmyka(iX,iY);
                        for (int i = 0; i < 4; i++)
                            afAv[i] += qCmyka[i];

                        iVisArea++;
                    }
                }
            }
        }
        else if ( spqNewImage->m_eColorModel == RasterImage::CMT_LAB )
        {
            ColorLab qLab;
            for (int iY = 0; iY < spqNewImage->m_iHeight; iY++)
            {
                for (int iX = 0; iX < spqNewImage->m_iWidth; iX++)
                {
                    if ( spqNewImage->m_qVisibilityMask.IsEmpty() || spqNewImage->IsPixelVisible(iX,iY) )
                    {
                        qLab = spqNewImage->GetPixelLab(iX,iY);
                        for (int i = 0; i < 3; i++)
                            afAv[i] += qLab[i];

                        iVisArea++;
                    }
                }
            }
        }
        else  // spqNewImage->m_eColorModel = RasterImage::CMT_LAB_WITH_ALPHA
        {
            ColorLaba qLaba;
            for (int iY = 0; iY < spqNewImage->m_iHeight; iY++)
            {
                for (int iX = 0; iX < spqNewImage->m_iWidth; iX++)
                {
                    if ( spqNewImage->m_qVisibilityMask.IsEmpty() || spqNewImage->IsPixelVisible(iX,iY) )
                    {
                        qLaba = spqNewImage->GetPixelLaba(iX,iY);
                        for (int i = 0; i < 3; i++)
                            afAv[i] += qLaba[i];

                        iVisArea++;
                    }
                }
            }
        }
    }
    if ( spqNewImage->m_eColorModel == RasterImage::CMT_PALETTED
    ||   spqNewImage->m_eColorModel == RasterImage::CMT_PALETTED_WITH_ALPHA )
    {
        float fSum = 0.0f;
        for (int i = 0; i < 3; i++)
            fSum += afAv[i]/iVisArea;
        float fCAv = fSum/3;
        for (int i = 0; i < 3; i++)
            afAv[i] = fCAv;
    }
    else  // non-paletted
    {
        if ( spqNewImage->m_eColorModel != RasterImage::CMT_LAB
        &&   spqNewImage->m_eColorModel != RasterImage::CMT_LAB_WITH_ALPHA )
        {
            float fSum = 0.0f;
            for (int i = 0; i < iCSQuantity; i++)
                fSum += afAv[i]/iVisArea;
            float fCAv = fSum/iCSQuantity;
            for (int i = 0; i < iCSQuantity; i++)
                afAv[i] = fCAv;
        }
        else
        {
            // L*a*b* gray point
            afAv[0] /= iVisArea;
            afAv[1] = 0.0f;
            afAv[2] = 0.0f;
        }
    }

    StillArray<unsigned char> aqLutUc[s_iMaxCS];
    StillArray<unsigned short> aqLutUs[s_iMaxCS];
    int iShVal;
    if ( bLut )
    {
        // generate LUT
        if ( iPrec == 8 )
        {
            if ( spqNewImage->m_eColorModel == RasterImage::CMT_PALETTED
            ||   spqNewImage->m_eColorModel == RasterImage::CMT_PALETTED_WITH_ALPHA )
            {
                for (int i0 = 0; i0 < 3; i0++)
                {
                    aqLutUc[i0].SetQuantity(256);

                    for (int i1 = 0; i1 < 256; i1++)
                    {
                        iShVal = Mathf::RoundToInt((afAv[i0]+(s_fMul8*i1-afAv[i0])*fCon)*255);
                        if ( iShVal < 0 )
                            iShVal = 0;
                        else if ( iShVal > 255 )
                            iShVal = 255;
                        aqLutUc[i0][i1] = iShVal;
                    }
                }
            }
            else  // non-paletted
            {
                for (int i0 = 0; i0 < iCSQuantity; i0++)
                {
                    aqLutUc[i0].SetQuantity(256);

                    if ( !((spqNewImage->m_eColorModel == RasterImage::CMT_LAB
                    ||   spqNewImage->m_eColorModel == RasterImage::CMT_LAB_WITH_ALPHA) && i0 != 0) )
                    {
                        for (int i1 = 0; i1 < 256; i1++)
                        {
                            iShVal = Mathf::RoundToInt((afAv[i0]+(s_fMul8*i1-afAv[i0])*fCon)*255);
                            if ( iShVal < 0 )
                                iShVal = 0;
                            else if ( iShVal > 255 )
                                iShVal = 255;
                            aqLutUc[i0][i1] = iShVal;
                        }
                    }
                    else
                    {
                        // specially for a* and b*
                        for (int i1 = 0; i1 < 256; i1++)
                        {
                            iShVal = Mathf::RoundToInt((afAv[i0]+(s_fMul8*(i1-128)-afAv[i0])*fCon)*255+128);
                            if ( iShVal < 0 )
                                iShVal = 0;
                            else if ( iShVal > 255 )
                                iShVal = 255;
                            aqLutUc[i0][i1] = iShVal;
                        }
                    }
                }
            }
        }
        else  // iPrec = 16
        {
            for (int i0 = 0; i0 < iCSQuantity; i0++)
            {
                aqLutUs[i0].SetQuantity(65536);

                if ( !((spqNewImage->m_eColorModel == RasterImage::CMT_LAB
                ||   spqNewImage->m_eColorModel == RasterImage::CMT_LAB_WITH_ALPHA) && i0 != 0) )
                {
                    for (int i1 = 0; i1 < 65536; i1++)
                    {
                        iShVal = Mathf::RoundToInt((afAv[i0]+(s_fMul16*i1-afAv[i0])*fCon)*65535);
                        if ( iShVal < 0 )
                            iShVal = 0;
                        else if ( iShVal > 65535 )
                            iShVal = 65535;
                        aqLutUs[i0][i1] = iShVal;
                    }
                }
                else
                {
                    // specially for a* and b*
                    for (int i1 = 0; i1 < 65536; i1++)
                    {
                        iShVal = Mathf::RoundToInt((afAv[i0]+(s_fMul16*(i1-32768)-afAv[i0])*fCon)*65535+32768);
                        if ( iShVal < 0 )
                            iShVal = 0;
                        else if ( iShVal > 65535 )
                            iShVal = 65535;
                        aqLutUs[i0][i1] = iShVal;
                    }
                }
            }
        }
    }

    // do the adjustment
    if ( spqNewImage->m_eColorModel == RasterImage::CMT_PALETTED
    ||   spqNewImage->m_eColorModel == RasterImage::CMT_PALETTED_WITH_ALPHA )
    {
        // paletted is the special case with LUT
        for (int i0 = 0; i0 < spqNewImage->m_qPalette.GetQuantity(); i0++)
        {
            PaletteColor& rqPalC = spqNewImage->m_qPalette[i0];
            for (int i1 = 0; i1 < 3; i1++)
                rqPalC.Components[i1] = aqLutUc[i1][(int)rqPalC.Components[i1]];
        }
    }
    else
    {
        if ( bLut )
        {
            // using LUT
            pucRow = (unsigned char*)spqNewImage->m_acImage;
            aucPix = pucRow;
            if ( iPrec == 8 )
            {
                for (int iY = 0; iY < spqNewImage->m_iHeight; iY++)
                {
                    for (int iX = 0; iX < spqNewImage->m_iWidth; iX++)
                    {
                        if ( spqNewImage->m_qVisibilityMask.IsEmpty() || spqNewImage->IsPixelVisible(iX,iY) )
                        {
                            for (int i = 0; i < iCSQuantity; i++)
                                *(aucPix + i) = aqLutUc[i][(int)*(aucPix+i)];
                            aucPix += spqNewImage->m_iComponentsPerPixel;
                        }
                    }

                    pucRow += spqNewImage->m_iRowStride;
                    aucPix = pucRow;
                }
            }
            else  // iPrec = 16
            {
                int iSPPM2 = spqNewImage->m_iComponentsPerPixel*2;
                for (int iY = 0; iY < spqNewImage->m_iHeight; iY++)
                {
                    for (int iX = 0; iX < spqNewImage->m_iWidth; iX++)
                    {
                        if ( spqNewImage->m_qVisibilityMask.IsEmpty() || spqNewImage->IsPixelVisible(iX,iY) )
                        {
                            for (int i = 0; i < iCSQuantity; i++)
                                MemCpy(aucPix+i*2,&aqLutUs[i][(int)*((unsigned short*)(aucPix+i*2))],2);
                            aucPix += iSPPM2;
                        }
                    }

                    pucRow += spqNewImage->m_iRowStride;
                    aucPix = pucRow;
                }
            }
        }
        else
        {
            // without LUT
            if ( spqNewImage->m_eColorModel == RasterImage::CMT_GRAYSCALE )
            {
                float fIntensity;
                for (int iY = 0; iY < spqNewImage->m_iHeight; iY++)
                {
                    for (int iX = 0; iX < spqNewImage->m_iWidth; iX++)
                    {
                        if ( spqNewImage->m_qVisibilityMask.IsEmpty() || spqNewImage->IsPixelVisible(iX,iY) )
                        {
                            fIntensity = spqNewImage->GetPixelIntensity(iX,iY);
                            fIntensity = afAv[0] + (fIntensity - afAv[0])*fCon;
                            if ( !spqNewImage->m_bIsHighDynamicRange )
                            {
                                if ( fIntensity < 0.0f )
                                    fIntensity = 0.0f;
                                else if ( fIntensity > 1.0f )
                                    fIntensity = 1.0f;
                            }
                            spqNewImage->SetPixelIntensity(fIntensity,iX,iY);
                        }
                    }
                }
            }
            else if ( spqNewImage->m_eColorModel == RasterImage::CMT_GRAYSCALE_WITH_ALPHA )
            {
                Link<float,float> qIA;
                for (int iY = 0; iY < spqNewImage->m_iHeight; iY++)
                {
                    for (int iX = 0; iX < spqNewImage->m_iWidth; iX++)
                    {
                        if ( spqNewImage->m_qVisibilityMask.IsEmpty() || spqNewImage->IsPixelVisible(iX,iY) )
                        {
                            qIA = spqNewImage->GetPixelIntensityWithAlpha(iX,iY);
                            qIA.First = afAv[0] + (qIA.First - afAv[0])*fCon;
                            if ( !spqNewImage->m_bIsHighDynamicRange )
                            {
                                if ( qIA.First < 0.0f )
                                    qIA.First = 0.0f;
                                else if ( qIA.First > 1.0f )
                                    qIA.First = 1.0f;
                            }
                            spqNewImage->SetPixelIntensityWithAlpha(qIA,iX,iY);
                        }
                    }
                }
            }
            else if ( spqNewImage->m_eColorModel == RasterImage::CMT_RGB )
            {
                ColorRgb qRgb;
                for (int iY = 0; iY < spqNewImage->m_iHeight; iY++)
                {
                    for (int iX = 0; iX < spqNewImage->m_iWidth; iX++)
                    {
                        if ( spqNewImage->m_qVisibilityMask.IsEmpty() || spqNewImage->IsPixelVisible(iX,iY) )
                        {
                            qRgb = spqNewImage->GetPixelRgb(iX,iY);
                            for (int i = 0; i < 3; i++)
                                qRgb[i] = afAv[i] + (qRgb[i] - afAv[i])*fCon;
                            if ( !spqNewImage->m_bIsHighDynamicRange )
                                qRgb.Clamp();
                            spqNewImage->SetPixelRgb(qRgb,iX,iY);
                        }
                    }
                }
            }
            else if ( spqNewImage->m_eColorModel == RasterImage::CMT_RGBA )
            {
                ColorRgba qRgba;
                for (int iY = 0; iY < spqNewImage->m_iHeight; iY++)
                {
                    for (int iX = 0; iX < spqNewImage->m_iWidth; iX++)
                    {
                        if ( spqNewImage->m_qVisibilityMask.IsEmpty() || spqNewImage->IsPixelVisible(iX,iY) )
                        {
                            qRgba = spqNewImage->GetPixelRgba(iX,iY);
                            for (int i = 0; i < 3; i++)
                                qRgba[i] = afAv[i] + (qRgba[i] - afAv[i])*fCon;
                            if ( !spqNewImage->m_bIsHighDynamicRange )
                                qRgba.Clamp();
                            spqNewImage->SetPixelRgba(qRgba,iX,iY);
                        }
                    }
                }
            }
            else if ( spqNewImage->m_eColorModel == RasterImage::CMT_CMYK )
            {
                ColorCmyk qCmyk;
                for (int iY = 0; iY < spqNewImage->m_iHeight; iY++)
                {
                    for (int iX = 0; iX < spqNewImage->m_iWidth; iX++)
                    {
                        if ( spqNewImage->m_qVisibilityMask.IsEmpty() || spqNewImage->IsPixelVisible(iX,iY) )
                        {
                            qCmyk = spqNewImage->GetPixelCmyk(iX,iY);
                            for (int i = 0; i < 4; i++)
                                qCmyk[i] = afAv[i] + (qCmyk[i] - afAv[i])*fCon;
                            if ( !spqNewImage->m_bIsHighDynamicRange )
                                qCmyk.Clamp();
                            spqNewImage->SetPixelCmyk(qCmyk,iX,iY);
                        }
                    }
                }
            }
            else if ( spqNewImage->m_eColorModel == RasterImage::CMT_CMYK_WITH_ALPHA )
            {
                ColorCmyka qCmyka;
                for (int iY = 0; iY < spqNewImage->m_iHeight; iY++)
                {
                    for (int iX = 0; iX < spqNewImage->m_iWidth; iX++)
                    {
                        if ( spqNewImage->m_qVisibilityMask.IsEmpty() || spqNewImage->IsPixelVisible(iX,iY) )
                        {
                            qCmyka = spqNewImage->GetPixelCmyka(iX,iY);
                            for (int i = 0; i < 4; i++)
                                qCmyka[i] = afAv[i] + (qCmyka[i] - afAv[i])*fCon;
                            if ( !spqNewImage->m_bIsHighDynamicRange )
                                qCmyka.Clamp();
                            spqNewImage->SetPixelCmyka(qCmyka,iX,iY);
                        }
                    }
                }
            }
            else if ( spqNewImage->m_eColorModel == RasterImage::CMT_LAB )
            {
                ColorLab qLab;
                for (int iY = 0; iY < spqNewImage->m_iHeight; iY++)
                {
                    for (int iX = 0; iX < spqNewImage->m_iWidth; iX++)
                    {
                        if ( spqNewImage->m_qVisibilityMask.IsEmpty() || spqNewImage->IsPixelVisible(iX,iY) )
                        {
                            qLab = spqNewImage->GetPixelLab(iX,iY);
                            for (int i = 0; i < 3; i++)
                                qLab[i] = afAv[i] + (qLab[i] - afAv[i])*fCon;
                            if ( !spqNewImage->m_bIsHighDynamicRange )
                                qLab.Clamp();
                            spqNewImage->SetPixelLab(qLab,iX,iY);
                        }
                    }
                }
            }
            else  // spqNewImage->m_eColorModel = RasterImage::CMT_LAB_WITH_ALPHA
            {
                ColorLaba qLaba;
                for (int iY = 0; iY < spqNewImage->m_iHeight; iY++)
                {
                    for (int iX = 0; iX < spqNewImage->m_iWidth; iX++)
                    {
                        if ( spqNewImage->m_qVisibilityMask.IsEmpty() || spqNewImage->IsPixelVisible(iX,iY) )
                        {
                            qLaba = spqNewImage->GetPixelLaba(iX,iY);
                            for (int i = 0; i < 3; i++)
                                qLaba[i] = afAv[i] + (qLaba[i] - afAv[i])*fCon;
                            if ( !spqNewImage->m_bIsHighDynamicRange )
                                qLaba.Clamp();
                            spqNewImage->SetPixelLaba(qLaba,iX,iY);
                        }
                    }
                }
            }
        }
    }

    return spqNewImage;
}
//------------------------------------------------------------------------------------------------------------------
RasterImagePtr RasterImageAdjustment::GetWithHue (float fHue) const
{
    assert( -1.0f <= fHue && fHue <= 1.0f );

    if ( fHue == 0.0f )
    {
        // nothing to do
        return m_spqSrcImage->GetDuplicate();
    }
    if ( m_spqSrcImage->m_eColorModel == RasterImage::CMT_GRAYSCALE
    ||   m_spqSrcImage->m_eColorModel == RasterImage::CMT_GRAYSCALE_WITH_ALPHA )
    {
        // nothing to do
        return m_spqSrcImage->GetDuplicate();
    }

    // allocate output image
    RasterImagePtr spqNewImage = m_spqSrcImage->GetDuplicate();

    static const int s_iMaxRollsForTri = 5;
    static const int s_iMaxRollsForDuo = 3;
    float fH;
    int iSn, iDir, iSl, iRoll;
    int aiMod[3];
    if ( spqNewImage->m_eColorModel == RasterImage::CMT_PALETTED
    ||   spqNewImage->m_eColorModel == RasterImage::CMT_PALETTED_WITH_ALPHA
    ||   spqNewImage->m_eColorModel == RasterImage::CMT_RGB
    ||   spqNewImage->m_eColorModel == RasterImage::CMT_RGBA
    ||   spqNewImage->m_eColorModel == RasterImage::CMT_CMYK
    ||   spqNewImage->m_eColorModel == RasterImage::CMT_CMYK_WITH_ALPHA )
    {
        fH = Mathf::FAbs(fHue)*3;
        iSn = ( fHue < 0.0f ? 1 : 2 );
        for (int i = 0; i < 3; i++)
            aiMod[i] = (i + iSn) % 3;
    }
    else  // spqNewImage->m_eColorModel = CMT_LAB or spqNewImage->m_eColorModel = CMT_LAB_WITH_ALPHA
    {
        fH = Mathf::FAbs(fHue)*2;
    }

    // do the adjustment
    if ( spqNewImage->m_eColorModel == RasterImage::CMT_PALETTED
    ||   spqNewImage->m_eColorModel == RasterImage::CMT_PALETTED_WITH_ALPHA )
    {
        int iMin, iMax, iResHue, iSub, iReSl;
        for (int i = 0; i < spqNewImage->m_qPalette.GetQuantity(); i++)
        {
            PaletteColor& rqPalC = spqNewImage->m_qPalette[i];

            Mathi::MinMax3(rqPalC.Components[2],rqPalC.Components[1],rqPalC.Components[0],iMin,iMax);
            if ( rqPalC.Components[2-iSn] <= rqPalC.Components[2] )
            {
                iDir = -1;
                iSl = ( rqPalC.Components[2] != iMax ? 0 : iSn );
            }
            else
            {
                iDir = 1;
                iSl = ( rqPalC.Components[2] != iMin ? 0 : iSn );
            }

            for (iResHue = Mathf::RoundToInt(fH*(iMax-iMin)), iRoll = 0;
                 iResHue != 0, iRoll < s_iMaxRollsForTri;
                 iResHue -= iSub, iRoll++, iDir = -iDir, iSl = aiMod[iSl])
            {
                iReSl = 2 - iSl;
                if ( iDir == -1 )
                {
                    iSub = Mathi::Min(iResHue,rqPalC.Components[iReSl]-iMin);
                    rqPalC.Components[iReSl] -= iSub;
                }
                else  // iDir = 1
                {
                    iSub = Mathi::Min(iResHue,iMax-rqPalC.Components[iReSl]);
                    rqPalC.Components[iReSl] += iSub;
                }
            }
        }
    }
    else if ( spqNewImage->m_bHasGradIntSampleUniformity
         &&   (spqNewImage->m_iBitsPerIntegerSample == 8 || spqNewImage->m_iBitsPerIntegerSample == 16) )
    {
        unsigned char* pucRow;
        unsigned char* aucPix;
        int iMin, iMax, iResHue, iSub;
        if ( spqNewImage->m_iBitsPerIntegerSample == 8 )
        {
            if ( spqNewImage->m_eColorModel == RasterImage::CMT_RGB
            ||   spqNewImage->m_eColorModel == RasterImage::CMT_RGBA
            ||   spqNewImage->m_eColorModel == RasterImage::CMT_CMYK
            ||   spqNewImage->m_eColorModel == RasterImage::CMT_CMYK_WITH_ALPHA )
            {
                pucRow = (unsigned char*)spqNewImage->m_acImage;
                aucPix = pucRow;
                for (int iY = 0; iY < spqNewImage->m_iHeight; iY++)
                {
                    for (int iX = 0; iX < spqNewImage->m_iWidth; iX++)
                    {
                        Mathi::MinMax3(aucPix[0],aucPix[1],aucPix[2],iMin,iMax);
                        if ( aucPix[iSn] <= aucPix[0] )
                        {
                            iDir = -1;
                            iSl = ( aucPix[0] != iMax ? 0 : iSn );
                        }
                        else
                        {
                            iDir = 1;
                            iSl = ( aucPix[0] != iMin ? 0 : iSn );
                        }

                        for (iResHue = Mathf::RoundToInt(fH*(iMax-iMin)), iRoll = 0;
                             iResHue != 0, iRoll < s_iMaxRollsForTri;
                             iResHue -= iSub, iRoll++, iDir = -iDir, iSl = aiMod[iSl])
                        {
                            if ( iDir == -1 )
                            {
                                iSub = Mathi::Min(iResHue,aucPix[iSl]-iMin);
                                aucPix[iSl] -= iSub;
                            }
                            else  // iDir = 1
                            {
                                iSub = Mathi::Min(iResHue,iMax-aucPix[iSl]);
                                aucPix[iSl] += iSub;
                            }
                        }

                        aucPix += spqNewImage->m_iBytesPerPixel;
                    }

                    pucRow += spqNewImage->m_iRowStride;
                    aucPix = pucRow;
                }
            }
            else  // spqNewImage->m_eColorModel = CMT_LAB or spqNewImage->m_eColorModel = CMT_LAB_WITH_ALPHA
            {
                int aiComp[2];
                pucRow = (unsigned char*)spqNewImage->m_acImage;
                aucPix = pucRow;
                for (int iY = 0; iY < spqNewImage->m_iHeight; iY++)
                {
                    for (int iX = 0; iX < spqNewImage->m_iWidth; iX++)
                    {
                        aiComp[0] = aucPix[1] - 128;
                        aiComp[1] = aucPix[2] - 128;

                        iMax = Mathi::Max(Mathi::Abs(aiComp[0]),Mathi::Abs(aiComp[1]));
                        iMin = -iMax;
                        if ( aiComp[1] <= aiComp[0] )
                        {
                            iDir = -1;
                            iSl = ( aiComp[0] != iMax ? 0 : 1 );
                        }
                        else
                        {
                            iDir = 1;
                            iSl = ( aiComp[0] != iMin ? 0 : 1 );
                        }
                        if ( fHue > 0.0f )
                            iDir = -iDir;

                        for (iResHue = Mathf::RoundToInt(fH*(iMax-iMin)), iRoll = 0;
                             iResHue != 0, iRoll < s_iMaxRollsForDuo;
                             iResHue -= iSub, iRoll++)
                        {
                            if ( iDir == -1 )
                            {
                                iSub = Mathi::Min(iResHue,aiComp[iSl]-iMin);
                                aiComp[iSl] -= iSub;
                                iSl ^= 1;
                                iDir = ( aiComp[iSl] != iMax ? 1 : -1 );
                            }
                            else  // iDir = 1
                            {
                                iSub = Mathi::Min(iResHue,iMax-aiComp[iSl]);
                                aiComp[iSl] += iSub;
                                iSl ^= 1;
                                iDir = ( aiComp[iSl] != iMin ? -1 : 1 );
                            }
                        }

                        aucPix[1] = aiComp[0] + 128;
                        aucPix[2] = aiComp[1] + 128;
                        aucPix += spqNewImage->m_iBytesPerPixel;
                    }

                    pucRow += spqNewImage->m_iRowStride;
                    aucPix = pucRow;
                }
            }
        }
        else  // spqNewImage->m_iBitsPerIntegerSample = 16
        {
            if ( spqNewImage->m_eColorModel == RasterImage::CMT_RGB
            ||   spqNewImage->m_eColorModel == RasterImage::CMT_RGBA
            ||   spqNewImage->m_eColorModel == RasterImage::CMT_CMYK
            ||   spqNewImage->m_eColorModel == RasterImage::CMT_CMYK_WITH_ALPHA )
            {
                unsigned short ausComp[3];
                pucRow = (unsigned char*)spqNewImage->m_acImage;
                aucPix = pucRow;
                for (int iY = 0; iY < spqNewImage->m_iHeight; iY++)
                {
                    for (int iX = 0; iX < spqNewImage->m_iWidth; iX++)
                    {
                        MemCpy(ausComp,aucPix,6);

                        Mathi::MinMax3(ausComp[0],ausComp[1],ausComp[2],iMin,iMax);
                        if ( ausComp[iSn] <= ausComp[0] )
                        {
                            iDir = -1;
                            iSl = ( ausComp[0] != iMax ? 0 : iSn );
                        }
                        else
                        {
                            iDir = 1;
                            iSl = ( ausComp[0] != iMin ? 0 : iSn );
                        }

                        for (iResHue = Mathf::RoundToInt(fH*(iMax-iMin)), iRoll = 0;
                             iResHue != 0, iRoll < s_iMaxRollsForTri;
                             iResHue -= iSub, iRoll++, iDir = -iDir, iSl = aiMod[iSl])
                        {
                            if ( iDir == -1 )
                            {
                                iSub = Mathi::Min(iResHue,ausComp[iSl]-iMin);
                                ausComp[iSl] -= iSub;
                            }
                            else  // iDir = 1
                            {
                                iSub = Mathi::Min(iResHue,iMax-ausComp[iSl]);
                                ausComp[iSl] += iSub;
                            }
                        }

                        MemCpy(aucPix,ausComp,6);
                        aucPix += spqNewImage->m_iBytesPerPixel;
                    }

                    pucRow += spqNewImage->m_iRowStride;
                    aucPix = pucRow;
                }
            }
            else  // spqNewImage->m_eColorModel = CMT_LAB or spqNewImage->m_eColorModel = CMT_LAB_WITH_ALPHA
            {
                unsigned short ausComp[2];
                int aiComp[2];
                pucRow = (unsigned char*)spqNewImage->m_acImage;
                aucPix = pucRow;
                for (int iY = 0; iY < spqNewImage->m_iHeight; iY++)
                {
                    for (int iX = 0; iX < spqNewImage->m_iWidth; iX++)
                    {
                        MemCpy(ausComp,aucPix+2,4);
                        aiComp[0] = ausComp[0] - 32768;
                        aiComp[1] = ausComp[1] - 32768;

                        iMax = Mathi::Max(Mathi::Abs(aiComp[0]),Mathi::Abs(aiComp[1]));
                        iMin = -iMax;
                        if ( aiComp[1] <= aiComp[0] )
                        {
                            iDir = -1;
                            iSl = ( aiComp[0] != iMax ? 0 : 1 );
                        }
                        else
                        {
                            iDir = 1;
                            iSl = ( aiComp[0] != iMin ? 0 : 1 );
                        }
                        if ( fHue > 0.0f )
                            iDir = -iDir;

                        for (iResHue = Mathf::RoundToInt(fH*(iMax-iMin)), iRoll = 0;
                             iResHue != 0, iRoll < s_iMaxRollsForDuo;
                             iResHue -= iSub, iRoll++)
                        {
                            if ( iDir == -1 )
                            {
                                iSub = Mathi::Min(iResHue,aiComp[iSl]-iMin);
                                aiComp[iSl] -= iSub;
                                iSl ^= 1;
                                iDir = ( aiComp[iSl] != iMax ? 1 : -1 );
                            }
                            else  // iDir = 1
                            {
                                iSub = Mathi::Min(iResHue,iMax-aiComp[iSl]);
                                aiComp[iSl] += iSub;
                                iSl ^= 1;
                                iDir = ( aiComp[iSl] != iMin ? -1 : 1 );
                            }
                        }

                        ausComp[0] = aiComp[0] + 32768;
                        ausComp[1] = aiComp[1] + 32768;
                        MemCpy(aucPix+2,ausComp,4);
                        aucPix += spqNewImage->m_iBytesPerPixel;
                    }

                    pucRow += spqNewImage->m_iRowStride;
                    aucPix = pucRow;
                }
            }
        }
    }
    else
    {
        float fMin, fMax, fResHue, fSub;
        if ( spqNewImage->m_eColorModel == RasterImage::CMT_RGB )
        {
            ColorRgb qRgb;
            for (int iY = 0; iY < spqNewImage->m_iHeight; iY++)
            {
                for (int iX = 0; iX < spqNewImage->m_iWidth; iX++)
                {
                    qRgb = spqNewImage->GetPixelRgb(iX,iY);

                    Mathf::MinMax3(qRgb[0],qRgb[1],qRgb[2],fMin,fMax);
                    if ( qRgb[iSn] <= qRgb[0] )
                    {
                        iDir = -1;
                        iSl = ( qRgb[0] != fMax ? 0 : iSn );
                    }
                    else
                    {
                        iDir = 1;
                        iSl = ( qRgb[0] != fMin ? 0 : iSn );
                    }

                    for (fResHue = fH*(fMax-fMin), iRoll = 0;
                         fResHue != 0.0f, iRoll < s_iMaxRollsForTri;
                         fResHue -= fSub, iRoll++, iDir = -iDir, iSl = aiMod[iSl])
                    {
                        if ( iDir == -1 )
                        {
                            fSub = Mathf::Min(fResHue,qRgb[iSl]-fMin);
                            qRgb[iSl] -= fSub;
                        }
                        else  // iDir = 1
                        {
                            fSub = Mathf::Min(fResHue,fMax-qRgb[iSl]);
                            qRgb[iSl] += fSub;
                        }
                    }

                    spqNewImage->SetPixelRgb(qRgb,iX,iY);
                }
            }
        }
        else if ( spqNewImage->m_eColorModel == RasterImage::CMT_RGBA )
        {
            ColorRgba qRgba;
            for (int iY = 0; iY < spqNewImage->m_iHeight; iY++)
            {
                for (int iX = 0; iX < spqNewImage->m_iWidth; iX++)
                {
                    qRgba = spqNewImage->GetPixelRgba(iX,iY);

                    Mathf::MinMax3(qRgba[0],qRgba[1],qRgba[2],fMin,fMax);
                    if ( qRgba[iSn] <= qRgba[0] )
                    {
                        iDir = -1;
                        iSl = ( qRgba[0] != fMax ? 0 : iSn );
                    }
                    else
                    {
                        iDir = 1;
                        iSl = ( qRgba[0] != fMin ? 0 : iSn );
                    }

                    for (fResHue = fH*(fMax-fMin), iRoll = 0;
                         fResHue != 0.0f, iRoll < s_iMaxRollsForTri;
                         fResHue -= fSub, iRoll++, iDir = -iDir, iSl = aiMod[iSl])
                    {
                        if ( iDir == -1 )
                        {
                            fSub = Mathf::Min(fResHue,qRgba[iSl]-fMin);
                            qRgba[iSl] -= fSub;
                        }
                        else  // iDir = 1
                        {
                            fSub = Mathf::Min(fResHue,fMax-qRgba[iSl]);
                            qRgba[iSl] += fSub;
                        }
                    }

                    spqNewImage->SetPixelRgba(qRgba,iX,iY);
                }
            }
        }
        else if ( spqNewImage->m_eColorModel == RasterImage::CMT_CMYK )
        {
            ColorCmyk qCmyk;
            for (int iY = 0; iY < spqNewImage->m_iHeight; iY++)
            {
                for (int iX = 0; iX < spqNewImage->m_iWidth; iX++)
                {
                    qCmyk = spqNewImage->GetPixelCmyk(iX,iY);

                    Mathf::MinMax3(qCmyk[0],qCmyk[1],qCmyk[2],fMin,fMax);
                    if ( qCmyk[iSn] <= qCmyk[0] )
                    {
                        iDir = -1;
                        iSl = ( qCmyk[0] != fMax ? 0 : iSn );
                    }
                    else
                    {
                        iDir = 1;
                        iSl = ( qCmyk[0] != fMin ? 0 : iSn );
                    }

                    for (fResHue = fH*(fMax-fMin), iRoll = 0;
                         fResHue != 0.0f, iRoll < s_iMaxRollsForTri;
                         fResHue -= fSub, iRoll++, iDir = -iDir, iSl = aiMod[iSl])
                    {
                        if ( iDir == -1 )
                        {
                            fSub = Mathf::Min(fResHue,qCmyk[iSl]-fMin);
                            qCmyk[iSl] -= fSub;
                        }
                        else  // iDir = 1
                        {
                            fSub = Mathf::Min(fResHue,fMax-qCmyk[iSl]);
                            qCmyk[iSl] += fSub;
                        }
                    }

                    spqNewImage->SetPixelCmyk(qCmyk,iX,iY);
                }
            }
        }
        else if ( spqNewImage->m_eColorModel == RasterImage::CMT_CMYK_WITH_ALPHA )
        {
            ColorCmyka qCmyka;
            for (int iY = 0; iY < spqNewImage->m_iHeight; iY++)
            {
                for (int iX = 0; iX < spqNewImage->m_iWidth; iX++)
                {
                    qCmyka = spqNewImage->GetPixelCmyka(iX,iY);

                    Mathf::MinMax3(qCmyka[0],qCmyka[1],qCmyka[2],fMin,fMax);
                    if ( qCmyka[iSn] <= qCmyka[0] )
                    {
                        iDir = -1;
                        iSl = ( qCmyka[0] != fMax ? 0 : iSn );
                    }
                    else
                    {
                        iDir = 1;
                        iSl = ( qCmyka[0] != fMin ? 0 : iSn );
                    }

                    for (fResHue = fH*(fMax-fMin), iRoll = 0;
                         fResHue != 0.0f, iRoll < s_iMaxRollsForTri;
                         fResHue -= fSub, iRoll++, iDir = -iDir, iSl = aiMod[iSl])
                    {
                        if ( iDir == -1 )
                        {
                            fSub = Mathf::Min(fResHue,qCmyka[iSl]-fMin);
                            qCmyka[iSl] -= fSub;
                        }
                        else  // iDir = 1
                        {
                            fSub = Mathf::Min(fResHue,fMax-qCmyka[iSl]);
                            qCmyka[iSl] += fSub;
                        }
                    }

                    spqNewImage->SetPixelCmyka(qCmyka,iX,iY);
                }
            }
        }
        else if ( spqNewImage->m_eColorModel == RasterImage::CMT_LAB )
        {
            ColorLab qLab;
            for (int iY = 0; iY < spqNewImage->m_iHeight; iY++)
            {
                for (int iX = 0; iX < spqNewImage->m_iWidth; iX++)
                {
                    qLab = spqNewImage->GetPixelLab(iX,iY);

                    fMax = Mathf::Max(Mathf::FAbs(qLab[1]),Mathf::FAbs(qLab[2]));
                    fMin = -fMax;
                    if ( qLab[2] <= qLab[1] )
                    {
                        iDir = -1;
                        iSl = ( qLab[1] != fMax ? 1 : 2 );
                    }
                    else
                    {
                        iDir = 1;
                        iSl = ( qLab[1] != fMin ? 1 : 2 );
                    }
                    if ( fHue > 0.0f )
                        iDir = -iDir;

                    for (fResHue = fH*(fMax-fMin), iRoll = 0;
                         fResHue != 0.0f, iRoll < s_iMaxRollsForDuo;
                         fResHue -= fSub, iRoll++)
                    {
                        if ( iDir == -1 )
                        {
                            fSub = Mathf::Min(fResHue,qLab[iSl]-fMin);
                            qLab[iSl] -= fSub;
                            iSl = ((iSl - 1) ^ 1) + 1;
                            iDir = ( qLab[iSl] != fMax ? 1 : -1 );
                        }
                        else  // iDir = 1
                        {
                            fSub = Mathf::Min(fResHue,fMax-qLab[iSl]);
                            qLab[iSl] += fSub;
                            iSl = ((iSl - 1) ^ 1) + 1;
                            iDir = ( qLab[iSl] != fMin ? -1 : 1 );
                        }
                    }

                    spqNewImage->SetPixelLab(qLab,iX,iY);
                }
            }
        }
        else  // spqNewImage->m_eColorModel = RasterImage::CMT_LAB_WITH_ALPHA
        {
            ColorLaba qLaba;
            for (int iY = 0; iY < spqNewImage->m_iHeight; iY++)
            {
                for (int iX = 0; iX < spqNewImage->m_iWidth; iX++)
                {
                    qLaba = spqNewImage->GetPixelLaba(iX,iY);

                    fMax = Mathf::Max(Mathf::FAbs(qLaba[1]),Mathf::FAbs(qLaba[2]));
                    fMin = -fMax;
                    if ( qLaba[2] <= qLaba[1] )
                    {
                        iDir = -1;
                        iSl = ( qLaba[1] != fMax ? 1 : 2 );
                    }
                    else
                    {
                        iDir = 1;
                        iSl = ( qLaba[1] != fMin ? 1 : 2 );
                    }
                    if ( fHue > 0.0f )
                        iDir = -iDir;

                    for (fResHue = fH*(fMax-fMin), iRoll = 0;
                         fResHue != 0.0f, iRoll < s_iMaxRollsForDuo;
                         fResHue -= fSub, iRoll++)
                    {
                        if ( iDir == -1 )
                        {
                            fSub = Mathf::Min(fResHue,qLaba[iSl]-fMin);
                            qLaba[iSl] -= fSub;
                            iSl = ((iSl - 1) ^ 1) + 1;
                            iDir = ( qLaba[iSl] != fMax ? 1 : -1 );
                        }
                        else  // iDir = 1
                        {
                            fSub = Mathf::Min(fResHue,fMax-qLaba[iSl]);
                            qLaba[iSl] += fSub;
                            iSl = ((iSl - 1) ^ 1) + 1;
                            iDir = ( qLaba[iSl] != fMin ? -1 : 1 );
                        }
                    }

                    spqNewImage->SetPixelLaba(qLaba,iX,iY);
                }
            }
        }
    }

    return spqNewImage;
}
//------------------------------------------------------------------------------------------------------------------
RasterImagePtr RasterImageAdjustment::GetWithSaturation (float fSaturation) const
{
    assert( -1.0f <= fSaturation && fSaturation <= 1.0f );

    if ( fSaturation == 0.0f )
    {
        // nothing to do
        return m_spqSrcImage->GetDuplicate();
    }
    if ( m_spqSrcImage->m_eColorModel == RasterImage::CMT_GRAYSCALE
    ||   m_spqSrcImage->m_eColorModel == RasterImage::CMT_GRAYSCALE_WITH_ALPHA )
    {
        // nothing to do
        return m_spqSrcImage->GetDuplicate();
    }

    // allocate output image
    RasterImagePtr spqNewImage = m_spqSrcImage->GetDuplicate();

    // to avoid infinity in further computations
    static const float s_fMaxSaturation = 0.999f;
    if ( fSaturation > s_fMaxSaturation )
        fSaturation = s_fMaxSaturation;

    // transform saturation value
    float fSat = 1.0f + ( fSaturation < 0.0f ? fSaturation : fSaturation/(1.0f - fSaturation) );

    // RGB and CMY weighting factors
    static const float s_afRgbPerc[3] = {0.3086f, 0.6094f, 0.0820f};
    static const float s_afCmyPerc[3] = {0.3333f, 0.3333f, 0.3333f};

    // do the adjustment
    if ( spqNewImage->m_eColorModel == RasterImage::CMT_PALETTED
    ||   spqNewImage->m_eColorModel == RasterImage::CMT_PALETTED_WITH_ALPHA )
    {
        static const float s_fMul = 1.0f/255;
        ColorRgb qRgb;
        float fDeSat;
        int iShVal;
        for (int i0 = 0; i0 < spqNewImage->m_qPalette.GetQuantity(); i0++)
        {
            PaletteColor& rqPalC = spqNewImage->m_qPalette[i0];
            qRgb = rqPalC.GetColorRgb();
            fDeSat =
                s_afRgbPerc[0]*qRgb.R() +
                s_afRgbPerc[1]*qRgb.G() +
                s_afRgbPerc[2]*qRgb.B();
            for (int i1 = 0; i1 < 3; i1++)
            {
                iShVal = Mathf::RoundToInt((fDeSat+(qRgb[i1]-fDeSat)*fSat)*255);
                if ( iShVal < 0 )
                    iShVal = 0;
                else if ( iShVal > 255 )
                    iShVal = 255;
                rqPalC.Components[2-i1] = iShVal;
            }
        }
    }
    else if ( spqNewImage->m_eColorModel == RasterImage::CMT_RGB )
    {
        ColorRgb qRgb;
        float fDeSat;
        for (int iY = 0; iY < spqNewImage->m_iHeight; iY++)
        {
            for (int iX = 0; iX < spqNewImage->m_iWidth; iX++)
            {
                qRgb = spqNewImage->GetPixelRgb(iX,iY);
                fDeSat =
                    s_afRgbPerc[0]*qRgb[0] +
                    s_afRgbPerc[1]*qRgb[1] +
                    s_afRgbPerc[2]*qRgb[2];
                for (int i = 0; i < 3; i++)
                    qRgb[i] = fDeSat + (qRgb[i] - fDeSat)*fSat;
                if ( !spqNewImage->m_bIsHighDynamicRange )
                    qRgb.Clamp();
                spqNewImage->SetPixelRgb(qRgb,iX,iY);
            }
        }
    }
    else if ( spqNewImage->m_eColorModel == RasterImage::CMT_RGBA )
    {
        ColorRgba qRgba;
        float fDeSat;
        for (int iY = 0; iY < spqNewImage->m_iHeight; iY++)
        {
            for (int iX = 0; iX < spqNewImage->m_iWidth; iX++)
            {
                qRgba = spqNewImage->GetPixelRgba(iX,iY);
                fDeSat =
                    s_afRgbPerc[0]*qRgba[0] +
                    s_afRgbPerc[1]*qRgba[1] +
                    s_afRgbPerc[2]*qRgba[2];
                for (int i = 0; i < 3; i++)
                    qRgba[i] = fDeSat + (qRgba[i] - fDeSat)*fSat;
                if ( !spqNewImage->m_bIsHighDynamicRange )
                    qRgba.Clamp();
                spqNewImage->SetPixelRgba(qRgba,iX,iY);
            }
        }
    }
    else if ( spqNewImage->m_eColorModel == RasterImage::CMT_CMYK )
    {
        ColorCmyk qCmyk;
        float fDeSat;
        for (int iY = 0; iY < spqNewImage->m_iHeight; iY++)
        {
            for (int iX = 0; iX < spqNewImage->m_iWidth; iX++)
            {
                qCmyk = spqNewImage->GetPixelCmyk(iX,iY);
                fDeSat =
                    s_afCmyPerc[0]*qCmyk[0] +
                    s_afCmyPerc[1]*qCmyk[1] +
                    s_afCmyPerc[2]*qCmyk[2];
                for (int i = 0; i < 3; i++)
                    qCmyk[i] = fDeSat + (qCmyk[i] - fDeSat)*fSat;
                if ( !spqNewImage->m_bIsHighDynamicRange )
                    qCmyk.Clamp();
                spqNewImage->SetPixelCmyk(qCmyk,iX,iY);
            }
        }
    }
    else if ( spqNewImage->m_eColorModel == RasterImage::CMT_CMYK_WITH_ALPHA )
    {
        ColorCmyka qCmyka;
        float fDeSat;
        for (int iY = 0; iY < spqNewImage->m_iHeight; iY++)
        {
            for (int iX = 0; iX < spqNewImage->m_iWidth; iX++)
            {
                qCmyka = spqNewImage->GetPixelCmyka(iX,iY);
                fDeSat =
                    s_afCmyPerc[0]*qCmyka[0] +
                    s_afCmyPerc[1]*qCmyka[1] +
                    s_afCmyPerc[2]*qCmyka[2];
                for (int i = 0; i < 3; i++)
                    qCmyka[i] = fDeSat + (qCmyka[i] - fDeSat)*fSat;
                if ( !spqNewImage->m_bIsHighDynamicRange )
                    qCmyka.Clamp();
                spqNewImage->SetPixelCmyka(qCmyka,iX,iY);
            }
        }
    }
    else if ( spqNewImage->m_eColorModel == RasterImage::CMT_LAB )
    {
        ColorLab qLab;
        for (int iY = 0; iY < spqNewImage->m_iHeight; iY++)
        {
            for (int iX = 0; iX < spqNewImage->m_iWidth; iX++)
            {
                qLab = spqNewImage->GetPixelLab(iX,iY);
                for (int i = 1; i < 3; i++)
                    qLab[i] *= fSat;
                if ( !spqNewImage->m_bIsHighDynamicRange )
                    qLab.Clamp();
                spqNewImage->SetPixelLab(qLab,iX,iY);
            }
        }
    }
    else  // spqNewImage->m_eColorModel = RasterImage::CMT_LAB_WITH_ALPHA
    {
        ColorLaba qLaba;
        for (int iY = 0; iY < spqNewImage->m_iHeight; iY++)
        {
            for (int iX = 0; iX < spqNewImage->m_iWidth; iX++)
            {
                qLaba = spqNewImage->GetPixelLaba(iX,iY);
                for (int i = 1; i < 3; i++)
                    qLaba[i] *= fSat;
                if ( !spqNewImage->m_bIsHighDynamicRange )
                    qLaba.Clamp();
                spqNewImage->SetPixelLaba(qLaba,iX,iY);
            }
        }
    }

    return spqNewImage;
}
//------------------------------------------------------------------------------------------------------------------
RasterImagePtr RasterImageAdjustment::GetWithLightness (float fLightness) const
{
    assert( -1.0f <= fLightness && fLightness <= 1.0f );

    if ( fLightness == 0.0f )
    {
        // nothing to do
        return m_spqSrcImage->GetDuplicate();
    }

    // allocate output image
    RasterImagePtr spqNewImage = m_spqSrcImage->GetDuplicate();

    float fLi = Mathf::FAbs(fLightness);
    float fRef = ( fLightness < 0.0f ? 0.0f : 1.0f );
    if ( spqNewImage->m_eColorModel == RasterImage::CMT_CMYK
    ||   spqNewImage->m_eColorModel == RasterImage::CMT_CMYK_WITH_ALPHA )
    {
        fRef = 1.0f - fRef;
    }
    else if ( spqNewImage->m_eColorModel == RasterImage::CMT_LAB
         ||   spqNewImage->m_eColorModel == RasterImage::CMT_LAB_WITH_ALPHA )
    {
        if ( fRef == 1.0f )
            fRef = 100.0f;
    }

    // decide if to generate a lookup table
    bool bLut = false;
    int iPrec;
    if ( spqNewImage->m_eColorModel == RasterImage::CMT_PALETTED
    ||   spqNewImage->m_eColorModel == RasterImage::CMT_PALETTED_WITH_ALPHA )
    {
        bLut = true;
        iPrec = 8;
    }
    else if ( spqNewImage->m_bHasGradIntSampleUniformity
         &&   (spqNewImage->m_iBitsPerIntegerSample == 8 || spqNewImage->m_iBitsPerIntegerSample == 16) )
    {
        bLut = true;
        iPrec = spqNewImage->m_iBitsPerIntegerSample;
    }

    StillArray<unsigned char> qLutUc;
    StillArray<unsigned short> qLutUs;
    float fUniI;
    if ( bLut )
    {
        // generate LUT
        if ( iPrec == 8 )
        {
            qLutUc.SetQuantity(256);

            static const float s_fInv8b = 1.0f/255;
            for (int i = 0; i < 256; i++)
            {
                fUniI = s_fInv8b*i;
                qLutUc[i] = Mathf::RoundToInt(((fRef-fUniI)*fLi+fUniI)*255);
            }
        }
        else  // iPrec = 16
        {
            qLutUs.SetQuantity(65536);

            static const float s_fInv16b = 1.0f/65535;
            for (int i = 0; i < 65536; i++)
            {
                fUniI = s_fInv16b*i;
                qLutUs[i] = Mathf::RoundToInt(((fRef-fUniI)*fLi+fUniI)*65535);
            }
        }
    }

    // do the adjustment
    if ( spqNewImage->m_eColorModel == RasterImage::CMT_PALETTED
    ||   spqNewImage->m_eColorModel == RasterImage::CMT_PALETTED_WITH_ALPHA )
    {
        // paletted is the special case with LUT
        for (int i0 = 0; i0 < spqNewImage->m_qPalette.GetQuantity(); i0++)
        {
            PaletteColor& rqPalC = spqNewImage->m_qPalette[i0];
            for (int i1 = 0; i1 < 3; i1++)
                rqPalC.Components[i1] = qLutUc[(int)rqPalC.Components[i1]];
        }
    }
    else
    {
        if ( bLut )
        {
            // using LUT
            unsigned char* pucRow = (unsigned char*)spqNewImage->m_acImage;
            unsigned char* aucPix = pucRow;
            if ( spqNewImage->m_eColorModel != RasterImage::CMT_LAB
            &&   spqNewImage->m_eColorModel != RasterImage::CMT_LAB_WITH_ALPHA )
            {
                int iCSQuantity = spqNewImage->m_iComponentsPerPixel;
                if ( spqNewImage->m_bHasAlpha )
                    iCSQuantity--;
                if ( spqNewImage->m_eColorModel == RasterImage::CMT_CMYK
                ||   spqNewImage->m_eColorModel == RasterImage::CMT_CMYK_WITH_ALPHA )
                {
                    // K does not change
                    iCSQuantity--;
                }

                if ( iPrec == 8 )
                {
                    for (int iY = 0; iY < spqNewImage->m_iHeight; iY++)
                    {
                        for (int iX = 0; iX < spqNewImage->m_iWidth; iX++)
                        {
                            for (int i = 0; i < iCSQuantity; i++)
                                *(aucPix + i) = qLutUc[(int)*(aucPix+i)];
                            aucPix += spqNewImage->m_iComponentsPerPixel;
                        }

                        pucRow += spqNewImage->m_iRowStride;
                        aucPix = pucRow;
                    }
                }
                else  // iPrec = 16
                {
                    int iSPPM2 = spqNewImage->m_iComponentsPerPixel*2;
                    for (int iY = 0; iY < spqNewImage->m_iHeight; iY++)
                    {
                        for (int iX = 0; iX < spqNewImage->m_iWidth; iX++)
                        {
                            for (int i = 0; i < iCSQuantity; i++)
                                MemCpy(aucPix+i*2,&qLutUs[(int)*((unsigned short*)(aucPix+i*2))],2);
                            aucPix += iSPPM2;
                        }

                        pucRow += spqNewImage->m_iRowStride;
                        aucPix = pucRow;
                    }
                }
            }
            else
            {
                // L*a*b* is the special case
                if ( spqNewImage->m_eColorModel == RasterImage::CMT_LAB )
                {
                    if ( spqNewImage->m_eType == RasterImage::IT_LAB_8_8_8 )
                    {
                        for (int iY = 0; iY < spqNewImage->m_iHeight; iY++)
                        {
                            for (int iX = 0; iX < spqNewImage->m_iWidth; iX++)
                            {
                                *aucPix = qLutUc[(int)*aucPix];
                                aucPix += 3;
                            }

                            pucRow += spqNewImage->m_iRowStride;
                            aucPix = pucRow;
                        }
                    }
                    else  // spqNewImage->m_eType = RasterImage::IT_LAB_16_16_16
                    {
                        for (int iY = 0; iY < spqNewImage->m_iHeight; iY++)
                        {
                            for (int iX = 0; iX < spqNewImage->m_iWidth; iX++)
                            {
                                MemCpy(aucPix,&qLutUs[(int)*((unsigned short*)aucPix)],2);
                                aucPix += 6;
                            }

                            pucRow += spqNewImage->m_iRowStride;
                            aucPix = pucRow;
                        }
                    }
                }
                else  // spqNewImage->m_eColorModel = RasterImage::CMT_LAB_WITH_ALPHA
                {
                    if ( spqNewImage->m_eType == RasterImage::IT_LAB_WITH_ALPHA_8_8_8_8 )
                    {
                        for (int iY = 0; iY < spqNewImage->m_iHeight; iY++)
                        {
                            for (int iX = 0; iX < spqNewImage->m_iWidth; iX++)
                            {
                                *aucPix = qLutUc[(int)*aucPix];
                                aucPix += 4;
                            }

                            pucRow += spqNewImage->m_iRowStride;
                            aucPix = pucRow;
                        }
                    }
                    else  // spqNewImage->m_eType = RasterImage::IT_LAB_WITH_ALPHA_16_16_16_16
                    {
                        for (int iY = 0; iY < spqNewImage->m_iHeight; iY++)
                        {
                            for (int iX = 0; iX < spqNewImage->m_iWidth; iX++)
                            {
                                MemCpy(aucPix,&qLutUs[(int)*((unsigned short*)aucPix)],2);
                                aucPix += 8;
                            }

                            pucRow += spqNewImage->m_iRowStride;
                            aucPix = pucRow;
                        }
                    }
                }
            }
        }
        else
        {
            // without LUT
            if ( spqNewImage->m_eColorModel == RasterImage::CMT_GRAYSCALE )
            {
                float fIntensity;
                for (int iY = 0; iY < spqNewImage->m_iHeight; iY++)
                {
                    for (int iX = 0; iX < spqNewImage->m_iWidth; iX++)
                    {
                        fIntensity = spqNewImage->GetPixelIntensity(iX,iY);
                        fIntensity = (fRef - fIntensity)*fLi + fIntensity;
                        if ( !spqNewImage->m_bIsHighDynamicRange )
                        {
                            if ( fIntensity < 0.0f )
                                fIntensity = 0.0f;
                            else if ( fIntensity > 1.0f )
                                fIntensity = 1.0f;
                        }
                        spqNewImage->SetPixelIntensity(fIntensity,iX,iY);
                    }
                }
            }
            else if ( spqNewImage->m_eColorModel == RasterImage::CMT_GRAYSCALE_WITH_ALPHA )
            {
                Link<float,float> qIA;
                for (int iY = 0; iY < spqNewImage->m_iHeight; iY++)
                {
                    for (int iX = 0; iX < spqNewImage->m_iWidth; iX++)
                    {
                        qIA = spqNewImage->GetPixelIntensityWithAlpha(iX,iY);
                        qIA.First = (fRef - qIA.First)*fLi + qIA.First;
                        if ( !spqNewImage->m_bIsHighDynamicRange )
                        {
                            if ( qIA.First < 0.0f )
                                qIA.First = 0.0f;
                            else if ( qIA.First > 1.0f )
                                qIA.First = 1.0f;
                        }
                        spqNewImage->SetPixelIntensityWithAlpha(qIA,iX,iY);
                    }
                }
            }
            else if ( spqNewImage->m_eColorModel == RasterImage::CMT_RGB )
            {
                ColorRgb qRgb;
                for (int iY = 0; iY < spqNewImage->m_iHeight; iY++)
                {
                    for (int iX = 0; iX < spqNewImage->m_iWidth; iX++)
                    {
                        qRgb = spqNewImage->GetPixelRgb(iX,iY);
                        for (int i = 0; i < 3; i++)
                            qRgb[i] = (fRef - qRgb[i])*fLi + qRgb[i];
                        if ( !spqNewImage->m_bIsHighDynamicRange )
                            qRgb.Clamp();
                        spqNewImage->SetPixelRgb(qRgb,iX,iY);
                    }
                }
            }
            else if ( spqNewImage->m_eColorModel == RasterImage::CMT_RGBA )
            {
                ColorRgba qRgba;
                for (int iY = 0; iY < spqNewImage->m_iHeight; iY++)
                {
                    for (int iX = 0; iX < spqNewImage->m_iWidth; iX++)
                    {
                        qRgba = spqNewImage->GetPixelRgba(iX,iY);
                        for (int i = 0; i < 3; i++)
                            qRgba[i] = (fRef - qRgba[i])*fLi + qRgba[i];
                        if ( !spqNewImage->m_bIsHighDynamicRange )
                            qRgba.Clamp();
                        spqNewImage->SetPixelRgba(qRgba,iX,iY);
                    }
                }
            }
            else if ( spqNewImage->m_eColorModel == RasterImage::CMT_CMYK )
            {
                ColorCmyk qCmyk;
                for (int iY = 0; iY < spqNewImage->m_iHeight; iY++)
                {
                    for (int iX = 0; iX < spqNewImage->m_iWidth; iX++)
                    {
                        qCmyk = spqNewImage->GetPixelCmyk(iX,iY);
                        for (int i = 0; i < 3; i++)
                            qCmyk[i] = (fRef - qCmyk[i])*fLi + qCmyk[i];
                        if ( !spqNewImage->m_bIsHighDynamicRange )
                            qCmyk.Clamp();
                        spqNewImage->SetPixelCmyk(qCmyk,iX,iY);
                    }
                }
            }
            else if ( spqNewImage->m_eColorModel == RasterImage::CMT_CMYK_WITH_ALPHA )
            {
                ColorCmyka qCmyka;
                for (int iY = 0; iY < spqNewImage->m_iHeight; iY++)
                {
                    for (int iX = 0; iX < spqNewImage->m_iWidth; iX++)
                    {
                        qCmyka = spqNewImage->GetPixelCmyka(iX,iY);
                        for (int i = 0; i < 3; i++)
                            qCmyka[i] = (fRef - qCmyka[i])*fLi + qCmyka[i];
                        if ( !spqNewImage->m_bIsHighDynamicRange )
                            qCmyka.Clamp();
                        spqNewImage->SetPixelCmyka(qCmyka,iX,iY);
                    }
                }
            }
            else if ( spqNewImage->m_eColorModel == RasterImage::CMT_LAB )
            {
                ColorLab qLab;
                for (int iY = 0; iY < spqNewImage->m_iHeight; iY++)
                {
                    for (int iX = 0; iX < spqNewImage->m_iWidth; iX++)
                    {
                        qLab = spqNewImage->GetPixelLab(iX,iY);
                        qLab[0] = (fRef - qLab[0])*fLi + qLab[0];
                        if ( !spqNewImage->m_bIsHighDynamicRange )
                            qLab.Clamp();
                        spqNewImage->SetPixelLab(qLab,iX,iY);
                    }
                }
            }
            else  // spqNewImage->m_eColorModel = RasterImage::CMT_LAB_WITH_ALPHA
            {
                ColorLaba qLaba;
                for (int iY = 0; iY < spqNewImage->m_iHeight; iY++)
                {
                    for (int iX = 0; iX < spqNewImage->m_iWidth; iX++)
                    {
                        qLaba = spqNewImage->GetPixelLaba(iX,iY);
                        qLaba[0] = (fRef - qLaba[0])*fLi + qLaba[0];
                        if ( !spqNewImage->m_bIsHighDynamicRange )
                            qLaba.Clamp();
                        spqNewImage->SetPixelLaba(qLaba,iX,iY);
                    }
                }
            }
        }
    }

    return spqNewImage;
}
//------------------------------------------------------------------------------------------------------------------
RasterImagePtr RasterImageAdjustment::GetWithGamma (float fGamma) const
{
    assert( fGamma >= 0.0f );

    if ( fGamma == 1.0f )
    {
        // nothing to do
        return m_spqSrcImage->GetDuplicate();
    }

    // allocate output image
    RasterImagePtr spqNewImage = m_spqSrcImage->GetDuplicate();

    // decide if to generate a lookup table
    bool bLut = false;
    int iPrec;
    if ( spqNewImage->m_eColorModel == RasterImage::CMT_PALETTED
    ||   spqNewImage->m_eColorModel == RasterImage::CMT_PALETTED_WITH_ALPHA )
    {
        bLut = true;
        iPrec = 8;
    }
    else if ( spqNewImage->m_bHasGradIntSampleUniformity
         &&   (spqNewImage->m_iBitsPerIntegerSample == 8 || spqNewImage->m_iBitsPerIntegerSample == 16) )
    {
        bLut = true;
        iPrec = spqNewImage->m_iBitsPerIntegerSample;
    }

    StillArray<unsigned char> qLutUc;
    StillArray<unsigned short> qLutUs;
    StillArray<unsigned char> aqLutLabUc[2];
    StillArray<unsigned short> aqLutLabUs[2];
    if ( bLut )
    {
        // generate LUT
        static const float s_fMul8 = 1.0f/255;
        static const float s_fMul16 = 1.0f/65535;
        if ( spqNewImage->m_eColorModel != RasterImage::CMT_LAB
        &&   spqNewImage->m_eColorModel != RasterImage::CMT_LAB_WITH_ALPHA )
        {
            if ( spqNewImage->m_eColorModel != RasterImage::CMT_CMYK
            &&   spqNewImage->m_eColorModel != RasterImage::CMT_CMYK_WITH_ALPHA )
            {
                if ( iPrec == 8 )
                {
                    qLutUc.SetQuantity(256);
                    for (int i = 0; i < 256; i++)
                        qLutUc[i] = Mathf::RoundToInt(Mathf::Pow(s_fMul8*i,fGamma)*255);
                }
                else  // iPrec = 16
                {
                    qLutUs.SetQuantity(65536);
                    for (int i = 0; i < 65536; i++)
                        qLutUs[i] = Mathf::RoundToInt(Mathf::Pow(s_fMul16*i,fGamma)*65535);
                }
            }
            else
            {
                // CMYK is the special case
                if ( iPrec == 8 )
                {
                    qLutUc.SetQuantity(256);
                    for (int i = 0; i < 256; i++)
                        qLutUc[i] = 255 - Mathf::RoundToInt(Mathf::Pow(s_fMul8*(255-i),fGamma)*255);
                }
                else  // iPrec = 16
                {
                    qLutUs.SetQuantity(65536);
                    for (int i = 0; i < 65536; i++)
                        qLutUs[i] = 65535 - Mathf::RoundToInt(Mathf::Pow(s_fMul16*(65535-i),fGamma)*65535);
                }
            }
        }
        else
        {
            // L*a*b* is the special case
            float fValue, fSign;
            int iShVal;
            if ( iPrec == 8 )
            {
                static const float s_fMulAb = 1.0f/128;

                for (int i = 0; i < 2; i++)
                    aqLutLabUc[i].SetQuantity(256);

                // L*
                for (int i = 0; i < 256; i++)
                    aqLutLabUc[0][i] = Mathf::RoundToInt(Mathf::Pow(s_fMul8*i,fGamma)*255);

                // a* and b*
                for (int i = 0; i < 256; i++)
                {
                    fValue = (float)i - 128;
                    fSign = Mathf::Sign(fValue);
                    fValue = fSign*(1.0f - Mathf::Pow(1.0f-Mathf::FAbs(fValue)*s_fMulAb,fGamma))*128;
                    iShVal = Mathf::RoundToInt(fValue+128);
                    if ( iShVal > 255 )
                        iShVal = 255;
                    aqLutLabUc[1][i] = iShVal;
                }
            }
            else  // iPrec = 16
            {
                static const float s_fMulAb = 1.0f/32768;

                for (int i = 0; i < 2; i++)
                    aqLutLabUs[i].SetQuantity(65536);

                // L*
                for (int i = 0; i < 65536; i++)
                    aqLutLabUs[0][i] = Mathf::RoundToInt(Mathf::Pow(s_fMul16*i,fGamma)*65535);

                // a* and b*
                for (int i = 0; i < 65536; i++)
                {
                    fValue = (float)i - 32768;
                    fSign = Mathf::Sign(fValue);
                    fValue = fSign*(1.0f - Mathf::Pow(1.0f-Mathf::FAbs(fValue)*s_fMulAb,fGamma))*32768;
                    iShVal = Mathf::RoundToInt(fValue+32768);
                    if ( iShVal > 65535 )
                        iShVal = 65535;
                    aqLutLabUs[1][i] = iShVal;
                }
            }
        }
    }

    // do the adjustment
    if ( spqNewImage->m_eColorModel == RasterImage::CMT_PALETTED
    ||   spqNewImage->m_eColorModel == RasterImage::CMT_PALETTED_WITH_ALPHA )
    {
        // paletted is the special case with LUT
        for (int i0 = 0; i0 < spqNewImage->m_qPalette.GetQuantity(); i0++)
        {
            PaletteColor& rqPalC = spqNewImage->m_qPalette[i0];
            for (int i1 = 0; i1 < 3; i1++)
                rqPalC.Components[i1] = qLutUc[(int)rqPalC.Components[i1]];
        }
    }
    else
    {
        if ( bLut )
        {
            // using LUT
            int iCSQuantity = spqNewImage->m_iComponentsPerPixel;
            if ( spqNewImage->m_bHasAlpha )
                iCSQuantity--;

            unsigned char* pucRow = (unsigned char*)spqNewImage->m_acImage;
            unsigned char* aucPix = pucRow;
            if ( spqNewImage->m_eColorModel != RasterImage::CMT_LAB
            &&   spqNewImage->m_eColorModel != RasterImage::CMT_LAB_WITH_ALPHA )
            {
                if ( iPrec == 8 )
                {
                    for (int iY = 0; iY < spqNewImage->m_iHeight; iY++)
                    {
                        for (int iX = 0; iX < spqNewImage->m_iWidth; iX++)
                        {
                            for (int i = 0; i < iCSQuantity; i++)
                                *(aucPix + i) = qLutUc[(int)*(aucPix+i)];
                            aucPix += spqNewImage->m_iComponentsPerPixel;
                        }

                        pucRow += spqNewImage->m_iRowStride;
                        aucPix = pucRow;
                    }
                }
                else  // iPrec = 16
                {
                    int iSPPM2 = spqNewImage->m_iComponentsPerPixel*2;
                    for (int iY = 0; iY < spqNewImage->m_iHeight; iY++)
                    {
                        for (int iX = 0; iX < spqNewImage->m_iWidth; iX++)
                        {
                            for (int i = 0; i < iCSQuantity; i++)
                                MemCpy(aucPix+i*2,&qLutUs[(int)*((unsigned short*)(aucPix+i*2))],2);
                            aucPix += iSPPM2;
                        }

                        pucRow += spqNewImage->m_iRowStride;
                        aucPix = pucRow;
                    }
                }
            }
            else
            {
                // L*a*b* is the special case
                if ( iPrec == 8 )
                {
                    for (int iY = 0; iY < spqNewImage->m_iHeight; iY++)
                    {
                        for (int iX = 0; iX < spqNewImage->m_iWidth; iX++)
                        {
                            *(aucPix + 0) = aqLutLabUc[0][(int)*(aucPix+0)];
                            *(aucPix + 1) = aqLutLabUc[1][(int)*(aucPix+1)];
                            *(aucPix + 2) = aqLutLabUc[1][(int)*(aucPix+2)];

                            aucPix += spqNewImage->m_iComponentsPerPixel;
                        }

                        pucRow += spqNewImage->m_iRowStride;
                        aucPix = pucRow;
                    }
                }
                else  // iPrec = 16
                {
                    int iSPPM2 = spqNewImage->m_iComponentsPerPixel*2;
                    for (int iY = 0; iY < spqNewImage->m_iHeight; iY++)
                    {
                        for (int iX = 0; iX < spqNewImage->m_iWidth; iX++)
                        {
                            MemCpy(aucPix+0,&aqLutLabUs[0][(int)*((unsigned short*)(aucPix+0))],2);
                            MemCpy(aucPix+2,&aqLutLabUs[1][(int)*((unsigned short*)(aucPix+2))],2);
                            MemCpy(aucPix+4,&aqLutLabUs[1][(int)*((unsigned short*)(aucPix+4))],2);

                            aucPix += iSPPM2;
                        }

                        pucRow += spqNewImage->m_iRowStride;
                        aucPix = pucRow;
                    }
                }
            }
        }
        else
        {
            // without LUT
            if ( spqNewImage->m_eColorModel == RasterImage::CMT_GRAYSCALE )
            {
                float fIntensity;
                for (int iY = 0; iY < spqNewImage->m_iHeight; iY++)
                {
                    for (int iX = 0; iX < spqNewImage->m_iWidth; iX++)
                    {
                        fIntensity = spqNewImage->GetPixelIntensity(iX,iY);
                        fIntensity = Mathf::Pow(fIntensity,fGamma);
                        spqNewImage->SetPixelIntensity(fIntensity,iX,iY);
                    }
                }
            }
            else if ( spqNewImage->m_eColorModel == RasterImage::CMT_GRAYSCALE_WITH_ALPHA )
            {
                Link<float,float> qIA;
                for (int iY = 0; iY < spqNewImage->m_iHeight; iY++)
                {
                    for (int iX = 0; iX < spqNewImage->m_iWidth; iX++)
                    {
                        qIA = spqNewImage->GetPixelIntensityWithAlpha(iX,iY);
                        qIA.First = Mathf::Pow(qIA.First,fGamma);
                        spqNewImage->SetPixelIntensityWithAlpha(qIA,iX,iY);
                    }
                }
            }
            else if ( spqNewImage->m_eColorModel == RasterImage::CMT_RGB )
            {
                ColorRgb qRgb;
                for (int iY = 0; iY < spqNewImage->m_iHeight; iY++)
                {
                    for (int iX = 0; iX < spqNewImage->m_iWidth; iX++)
                    {
                        qRgb = spqNewImage->GetPixelRgb(iX,iY);
                        for (int i = 0; i < 3; i++)
                            qRgb[i] = Mathf::Pow(qRgb[i],fGamma);
                        spqNewImage->SetPixelRgb(qRgb,iX,iY);
                    }
                }
            }
            else if ( spqNewImage->m_eColorModel == RasterImage::CMT_RGBA )
            {
                ColorRgba qRgba;
                for (int iY = 0; iY < spqNewImage->m_iHeight; iY++)
                {
                    for (int iX = 0; iX < spqNewImage->m_iWidth; iX++)
                    {
                        qRgba = spqNewImage->GetPixelRgba(iX,iY);
                        for (int i = 0; i < 3; i++)
                            qRgba[i] = Mathf::Pow(qRgba[i],fGamma);
                        spqNewImage->SetPixelRgba(qRgba,iX,iY);
                    }
                }
            }
            else if ( spqNewImage->m_eColorModel == RasterImage::CMT_CMYK )
            {
                ColorCmyk qCmyk;
                for (int iY = 0; iY < spqNewImage->m_iHeight; iY++)
                {
                    for (int iX = 0; iX < spqNewImage->m_iWidth; iX++)
                    {
                        qCmyk = spqNewImage->GetPixelCmyk(iX,iY);
                        for (int i = 0; i < 4; i++)
                            qCmyk[i] = 1.0f - Mathf::Pow(1.0f-qCmyk[i],fGamma);
                        spqNewImage->SetPixelCmyk(qCmyk,iX,iY);
                    }
                }
            }
            else if ( spqNewImage->m_eColorModel == RasterImage::CMT_CMYK_WITH_ALPHA )
            {
                ColorCmyka qCmyka;
                for (int iY = 0; iY < spqNewImage->m_iHeight; iY++)
                {
                    for (int iX = 0; iX < spqNewImage->m_iWidth; iX++)
                    {
                        qCmyka = spqNewImage->GetPixelCmyka(iX,iY);
                        for (int i = 0; i < 4; i++)
                            qCmyka[i] = 1.0f - Mathf::Pow(1.0f-qCmyka[i],fGamma);
                        spqNewImage->SetPixelCmyka(qCmyka,iX,iY);
                    }
                }
            }
            else if ( spqNewImage->m_eColorModel == RasterImage::CMT_LAB )
            {
                ColorLab qLab;
                float fSign;
                static const float s_fMul0 = 1.0f/100;
                static const float s_fMul1 = 1.0f/128;
                for (int iY = 0; iY < spqNewImage->m_iHeight; iY++)
                {
                    for (int iX = 0; iX < spqNewImage->m_iWidth; iX++)
                    {
                        qLab = spqNewImage->GetPixelLab(iX,iY);
                        qLab[0] = Mathf::Pow(qLab[0]*s_fMul0,fGamma)*100;
                        for (int i = 1; i < 3; i++)
                        {
                            fSign = Mathf::Sign(qLab[i]);
                            qLab[i] = fSign*(1.0f - Mathf::Pow(1.0f-Mathf::FAbs(qLab[i])*s_fMul1,fGamma))*128;
                        }
                        spqNewImage->SetPixelLab(qLab,iX,iY);
                    }
                }
            }
            else  // spqNewImage->m_eColorModel = RasterImage::CMT_LAB_WITH_ALPHA
            {
                ColorLaba qLaba;
                float fSign;
                static const float s_fMul0 = 1.0f/100;
                static const float s_fMul1 = 1.0f/128;
                for (int iY = 0; iY < spqNewImage->m_iHeight; iY++)
                {
                    for (int iX = 0; iX < spqNewImage->m_iWidth; iX++)
                    {
                        qLaba = spqNewImage->GetPixelLaba(iX,iY);
                        qLaba[0] = Mathf::Pow(qLaba[0]*s_fMul0,fGamma)*100;
                        for (int i = 1; i < 3; i++)
                        {
                            fSign = Mathf::Sign(qLaba[i]);
                            qLaba[i] = fSign*(1.0f - Mathf::Pow(1.0f-Mathf::FAbs(qLaba[i])*s_fMul1,fGamma))*128;
                        }
                        spqNewImage->SetPixelLaba(qLaba,iX,iY);
                    }
                }
            }
        }
    }

    return spqNewImage;
}
//------------------------------------------------------------------------------------------------------------------
RasterImagePtr RasterImageAdjustment::GetBlurred ()
{
    int aiMatrix[9] = {
         1,  1,  1,
         1, 10,  1,
         1,  1,  1};
    return GetCustomFiltered(aiMatrix,3);
}
//------------------------------------------------------------------------------------------------------------------
RasterImagePtr RasterImageAdjustment::GetBlurredMore ()
{
    int aiMatrix[9] = {
        1, 1, 1,
        1, 3, 1,
        1, 1, 1};
    return GetCustomFiltered(aiMatrix,3);
}
//------------------------------------------------------------------------------------------------------------------
RasterImagePtr RasterImageAdjustment::GetSharpened ()
{
    int aiMatrix[9] = {
        -1, -1, -1,
        -1, 18, -1,
        -1, -1, -1};
    return GetCustomFiltered(aiMatrix,3);
}
//------------------------------------------------------------------------------------------------------------------
RasterImagePtr RasterImageAdjustment::GetSharpenedMore ()
{
    int aiMatrix[9] = {
        -1, -1, -1,
        -1, 12, -1,
        -1, -1, -1};
    return GetCustomFiltered(aiMatrix,3);
}
//------------------------------------------------------------------------------------------------------------------
RasterImagePtr RasterImageAdjustment::GetCustomFiltered (int* aiMatrix, int iN, int iWeight)
{
    assert( m_spqSrcImage->m_eColorModel != RasterImage::CMT_PALETTED
        &&  m_spqSrcImage->m_eColorModel != RasterImage::CMT_PALETTED_WITH_ALPHA );
    assert( iN >= 1 && (iN & 1) != 0 );

    // allocate output image
    RasterImagePtr spqNewImage = m_spqSrcImage->GetDuplicate(false,true);

    static const int s_iMaxSam = 5;  // CMYKA currently does the maximal number of samples
    int iCornX, iCornY, iCell, iShX, iShY, iValidCellSum;
    float fWCoeff;
    int iHalfN = iN/2;
    int iSqN = iN*iN;
    int iCellSum = 0;
    for (int i = 0; i < iSqN; i++)
        iCellSum += aiMatrix[i];
    if ( iWeight == 0 )
        iWeight = iCellSum;
    float fInvWeight = 1.0f/iWeight;
    float fInvCellSumMulW = ((float)iWeight)/iCellSum;

    // for a progress bar
    ProgressProbe qProgress(this);
    qProgress.SetAdvance(1.0f/m_spqSrcImage->m_iHeight);

    // do filtering
    if ( m_spqSrcImage->m_bHasGradIntSampleUniformity
    &&   (m_spqSrcImage->m_iBitsPerIntegerSample == 8 || m_spqSrcImage->m_iBitsPerIntegerSample == 16) )
    {
        int aiSum[s_iMaxSam];
        unsigned char* aucImage0 = (unsigned char*)m_spqSrcImage->m_acImage;
        unsigned char* aucRow0;
        unsigned char* pucPix0;
        unsigned char* pucPixC;
        unsigned char* aucRow1 = (unsigned char*)spqNewImage->m_acImage;
        unsigned char* pucPix1 = aucRow1;
        int iShVal;

        if ( m_spqSrcImage->m_iBitsPerIntegerSample == 8 )
        {
            for (int iY = 0; iY < m_spqSrcImage->m_iHeight; iY++)
            {
                for (int iX = 0; iX < m_spqSrcImage->m_iWidth; iX++)
                {
                    iCornX = iX - iHalfN;
                    iCornY = iY - iHalfN;
                    aucRow0 = aucImage0 + iCornY*m_spqSrcImage->m_iRowStride +
                        iCornX*m_spqSrcImage->m_iBytesPerPixel;
                    pucPix0 = aucRow0;
                    pucPixC = aucImage0 + iY*m_spqSrcImage->m_iRowStride + iX*m_spqSrcImage->m_iBytesPerPixel;
                    iCell = 0;
                    for (int i = 0; i < m_spqSrcImage->m_iComponentsPerPixel; i++)
                        aiSum[i] = 0;

                    // lay the matrix on
                    iValidCellSum = 0;
                    for (int i0 = 0; i0 < iN; i0++)
                    {
                        iShY = iCornY + i0;
                        for (int i1 = 0; i1 < iN; i1++)
                        {
                            iShX = iCornX + i1;

                            if ( 0 <= iShX && iShX < m_spqSrcImage->m_iWidth
                            &&   0 <= iShY && iShY < m_spqSrcImage->m_iHeight )
                            {
                                for (int i2 = 0; i2 < m_spqSrcImage->m_iComponentsPerPixel; i2++)
                                    aiSum[i2] += aiMatrix[iCell]*pucPix0[i2];
                                iValidCellSum += aiMatrix[iCell];
                            }

                            pucPix0 += m_spqSrcImage->m_iBytesPerPixel;
                            iCell++;
                        }

                        aucRow0 += m_spqSrcImage->m_iRowStride;
                        pucPix0 = aucRow0;
                    }
                    fWCoeff = ( iValidCellSum == iCellSum ? fInvWeight : 1.0f/(fInvCellSumMulW*iValidCellSum) );

                    // assign weighted values
                    for (int i = 0; i < m_spqSrcImage->m_iComponentsPerPixel; i++)
                    {
                        iShVal = Mathf::RoundToInt(fWCoeff*aiSum[i]);
                        if ( iShVal < 0 )
                            iShVal = 0;
                        else if ( iShVal > 255 )
                            iShVal = 255;
                        pucPix1[i] = iShVal;
                    }

                    pucPix1 += spqNewImage->m_iBytesPerPixel;
                }

                aucRow1 += spqNewImage->m_iRowStride;
                pucPix1 = aucRow1;

                qProgress.MakeAdvance();
            }
        }
        else  // m_spqSrcImage->m_iBitsPerIntegerSample = 16
        {
            for (int iY = 0; iY < m_spqSrcImage->m_iHeight; iY++)
            {
                for (int iX = 0; iX < m_spqSrcImage->m_iWidth; iX++)
                {
                    iCornX = iX - iHalfN;
                    iCornY = iY - iHalfN;
                    aucRow0 = aucImage0 + iCornY*m_spqSrcImage->m_iRowStride +
                        iCornX*m_spqSrcImage->m_iBytesPerPixel;
                    pucPix0 = aucRow0;
                    pucPixC = aucImage0 + iY*m_spqSrcImage->m_iRowStride + iX*m_spqSrcImage->m_iBytesPerPixel;
                    iCell = 0;
                    for (int i = 0; i < m_spqSrcImage->m_iComponentsPerPixel; i++)
                        aiSum[i] = 0;

                    // lay the matrix on
                    iValidCellSum = 0;
                    for (int i0 = 0; i0 < iN; i0++)
                    {
                        iShY = iCornY + i0;
                        for (int i1 = 0; i1 < iN; i1++)
                        {
                            iShX = iCornX + i1;

                            if ( 0 <= iShX && iShX < m_spqSrcImage->m_iWidth
                            &&   0 <= iShY && iShY < m_spqSrcImage->m_iHeight )
                            {
                                for (int i2 = 0; i2 < m_spqSrcImage->m_iComponentsPerPixel; i2++)
                                    aiSum[i2] += aiMatrix[iCell]*(*((unsigned short*)(pucPix0 + i2*2)));
                                iValidCellSum += aiMatrix[iCell];
                            }

                            pucPix0 += m_spqSrcImage->m_iBytesPerPixel;
                            iCell++;
                        }

                        aucRow0 += m_spqSrcImage->m_iRowStride;
                        pucPix0 = aucRow0;
                    }
                    fWCoeff = ( iValidCellSum == iCellSum ? fInvWeight : 1.0f/(fInvCellSumMulW*iValidCellSum) );

                    // assign weighted values
                    for (int i = 0; i < m_spqSrcImage->m_iComponentsPerPixel; i++)
                    {
                        iShVal = Mathf::RoundToInt(fWCoeff*aiSum[i]);
                        if ( iShVal < 0 )
                            iShVal = 0;
                        else if ( iShVal > 65535 )
                            iShVal = 65535;
                        *((unsigned short*)(pucPix1 + i*2)) = iShVal;
                    }

                    pucPix1 += spqNewImage->m_iBytesPerPixel;
                }

                aucRow1 += spqNewImage->m_iRowStride;
                pucPix1 = aucRow1;

                qProgress.MakeAdvance();
            }
        }
    }
    else
    {
        if ( m_spqSrcImage->m_eColorModel == RasterImage::CMT_GRAYSCALE )
        {
            float fSum, fIntensity;
            for (int iY = 0; iY < m_spqSrcImage->m_iHeight; iY++)
            {
                for (int iX = 0; iX < m_spqSrcImage->m_iWidth; iX++)
                {
                    iCornX = iX - iHalfN;
                    iCornY = iY - iHalfN;
                    iCell = 0;
                    fSum = 0.0f;

                    // lay the matrix on
                    iValidCellSum = 0;
                    for (int i0 = 0; i0 < iN; i0++)
                    {
                        iShY = iCornY + i0;
                        for (int i1 = 0; i1 < iN; i1++)
                        {
                            iShX = iCornX + i1;

                            if ( 0 <= iShX && iShX < m_spqSrcImage->m_iWidth
                            &&   0 <= iShY && iShY < m_spqSrcImage->m_iHeight )
                            {
                                fIntensity = m_spqSrcImage->GetPixelIntensity(iShX,iShY);
                                fSum += fIntensity*aiMatrix[iCell];
                                iValidCellSum += aiMatrix[iCell];
                            }

                            iCell++;
                        }
                    }
                    fWCoeff = ( iValidCellSum == iCellSum ? fInvWeight : 1.0f/(fInvCellSumMulW*iValidCellSum) );

                    // assign weighted value
                    fIntensity = fSum*fWCoeff;
                    if ( !m_spqSrcImage->m_bIsHighDynamicRange )
                    {
                        if ( fIntensity < 0.0f )
                            fIntensity = 0.0f;
                        else if ( fIntensity > 1.0f )
                            fIntensity = 1.0f;
                    }
                    spqNewImage->SetPixelIntensity(fIntensity,iX,iY);
                }

                qProgress.MakeAdvance();
            }
        }
        else if ( m_spqSrcImage->m_eColorModel == RasterImage::CMT_GRAYSCALE_WITH_ALPHA )
        {
            Link<float,float> qSum, qIA;
            for (int iY = 0; iY < m_spqSrcImage->m_iHeight; iY++)
            {
                for (int iX = 0; iX < m_spqSrcImage->m_iWidth; iX++)
                {
                    iCornX = iX - iHalfN;
                    iCornY = iY - iHalfN;
                    iCell = 0;
                    qSum.First = 0.0f;
                    qSum.Second = 0.0f;

                    // lay the matrix on
                    iValidCellSum = 0;
                    for (int i0 = 0; i0 < iN; i0++)
                    {
                        iShY = iCornY + i0;
                        for (int i1 = 0; i1 < iN; i1++)
                        {
                            iShX = iCornX + i1;

                            if ( 0 <= iShX && iShX < m_spqSrcImage->m_iWidth
                            &&   0 <= iShY && iShY < m_spqSrcImage->m_iHeight )
                            {
                                qIA = m_spqSrcImage->GetPixelIntensityWithAlpha(iShX,iShY);
                                qSum.First += qIA.First*aiMatrix[iCell];
                                qSum.Second += qIA.Second*aiMatrix[iCell];
                                iValidCellSum += aiMatrix[iCell];
                            }

                            iCell++;
                        }
                    }
                    fWCoeff = ( iValidCellSum == iCellSum ? fInvWeight : 1.0f/(fInvCellSumMulW*iValidCellSum) );

                    // assign weighted values
                    qIA.First = qSum.First*fWCoeff;
                    qIA.Second = qSum.Second*fWCoeff;
                    if ( !m_spqSrcImage->m_bIsHighDynamicRange )
                    {
                        if ( qIA.First < 0.0f )
                            qIA.First = 0.0f;
                        else if ( qIA.First > 1.0f )
                            qIA.First = 1.0f;
                        if ( qIA.Second < 0.0f )
                            qIA.Second = 0.0f;
                        else if ( qIA.Second > 1.0f )
                            qIA.Second = 1.0f;
                    }
                    spqNewImage->SetPixelIntensityWithAlpha(qIA,iX,iY);
                }

                qProgress.MakeAdvance();
            }
        }
        else if ( m_spqSrcImage->m_eColorModel == RasterImage::CMT_RGB )
        {
            ColorRgb qSum, qRgb;
            for (int iY = 0; iY < m_spqSrcImage->m_iHeight; iY++)
            {
                for (int iX = 0; iX < m_spqSrcImage->m_iWidth; iX++)
                {
                    iCornX = iX - iHalfN;
                    iCornY = iY - iHalfN;
                    iCell = 0;
                    for (int i = 0; i < 3; i++)
                        qSum[i] = 0.0f;

                    // lay the matrix on
                    iValidCellSum = 0;
                    for (int i0 = 0; i0 < iN; i0++)
                    {
                        iShY = iCornY + i0;
                        for (int i1 = 0; i1 < iN; i1++)
                        {
                            iShX = iCornX + i1;

                            if ( 0 <= iShX && iShX < m_spqSrcImage->m_iWidth
                            &&   0 <= iShY && iShY < m_spqSrcImage->m_iHeight )
                            {
                                qRgb = m_spqSrcImage->GetPixelRgb(iShX,iShY);
                                qSum += qRgb*((float)aiMatrix[iCell]);
                                iValidCellSum += aiMatrix[iCell];
                            }

                            iCell++;
                        }
                    }
                    fWCoeff = ( iValidCellSum == iCellSum ? fInvWeight : 1.0f/(fInvCellSumMulW*iValidCellSum) );

                    // assign weighted values
                    qRgb = qSum*fWCoeff;
                    if ( !m_spqSrcImage->m_bIsHighDynamicRange )
                        qRgb.Clamp();
                    spqNewImage->SetPixelRgb(qRgb,iX,iY);
                }

                qProgress.MakeAdvance();
            }
        }
        else if ( m_spqSrcImage->m_eColorModel == RasterImage::CMT_RGBA )
        {
            ColorRgba qSum, qRgba;
            for (int iY = 0; iY < m_spqSrcImage->m_iHeight; iY++)
            {
                for (int iX = 0; iX < m_spqSrcImage->m_iWidth; iX++)
                {
                    iCornX = iX - iHalfN;
                    iCornY = iY - iHalfN;
                    iCell = 0;
                    for (int i = 0; i < 4; i++)
                        qSum[i] = 0.0f;

                    // lay the matrix on
                    iValidCellSum = 0;
                    for (int i0 = 0; i0 < iN; i0++)
                    {
                        iShY = iCornY + i0;
                        for (int i1 = 0; i1 < iN; i1++)
                        {
                            iShX = iCornX + i1;

                            if ( 0 <= iShX && iShX < m_spqSrcImage->m_iWidth
                            &&   0 <= iShY && iShY < m_spqSrcImage->m_iHeight )
                            {
                                qRgba = m_spqSrcImage->GetPixelRgba(iShX,iShY);
                                qSum += qRgba*((float)aiMatrix[iCell]);
                                iValidCellSum += aiMatrix[iCell];
                            }

                            iCell++;
                        }
                    }
                    fWCoeff = ( iValidCellSum == iCellSum ? fInvWeight : 1.0f/(fInvCellSumMulW*iValidCellSum) );

                    // assign weighted values
                    qRgba = qSum*fWCoeff;
                    if ( !m_spqSrcImage->m_bIsHighDynamicRange )
                        qRgba.Clamp();
                    spqNewImage->SetPixelRgba(qRgba,iX,iY);
                }

                qProgress.MakeAdvance();
            }
        }
        else if ( m_spqSrcImage->m_eColorModel == RasterImage::CMT_CMYK )
        {
            ColorCmyk qSum, qCmyk;
            for (int iY = 0; iY < m_spqSrcImage->m_iHeight; iY++)
            {
                for (int iX = 0; iX < m_spqSrcImage->m_iWidth; iX++)
                {
                    iCornX = iX - iHalfN;
                    iCornY = iY - iHalfN;
                    iCell = 0;
                    for (int i = 0; i < 4; i++)
                        qSum[i] = 0.0f;

                    // lay the matrix on
                    iValidCellSum = 0;
                    for (int i0 = 0; i0 < iN; i0++)
                    {
                        iShY = iCornY + i0;
                        for (int i1 = 0; i1 < iN; i1++)
                        {
                            iShX = iCornX + i1;

                            if ( 0 <= iShX && iShX < m_spqSrcImage->m_iWidth
                            &&   0 <= iShY && iShY < m_spqSrcImage->m_iHeight )
                            {
                                qCmyk = m_spqSrcImage->GetPixelCmyk(iShX,iShY);
                                qSum += qCmyk*((float)aiMatrix[iCell]);
                                iValidCellSum += aiMatrix[iCell];
                            }

                            iCell++;
                        }
                    }
                    fWCoeff = ( iValidCellSum == iCellSum ? fInvWeight : 1.0f/(fInvCellSumMulW*iValidCellSum) );

                    // assign weighted values
                    qCmyk = qSum*fWCoeff;
                    if ( !m_spqSrcImage->m_bIsHighDynamicRange )
                        qCmyk.Clamp();
                    spqNewImage->SetPixelCmyk(qCmyk,iX,iY);
                }

                qProgress.MakeAdvance();
            }
        }
        else if ( m_spqSrcImage->m_eColorModel == RasterImage::CMT_CMYK_WITH_ALPHA )
        {
            ColorCmyka qSum, qCmyka;
            for (int iY = 0; iY < m_spqSrcImage->m_iHeight; iY++)
            {
                for (int iX = 0; iX < m_spqSrcImage->m_iWidth; iX++)
                {
                    iCornX = iX - iHalfN;
                    iCornY = iY - iHalfN;
                    iCell = 0;
                    for (int i = 0; i < 5; i++)
                        qSum[i] = 0.0f;

                    // lay the matrix on
                    iValidCellSum = 0;
                    for (int i0 = 0; i0 < iN; i0++)
                    {
                        iShY = iCornY + i0;
                        for (int i1 = 0; i1 < iN; i1++)
                        {
                            iShX = iCornX + i1;

                            if ( 0 <= iShX && iShX < m_spqSrcImage->m_iWidth
                            &&   0 <= iShY && iShY < m_spqSrcImage->m_iHeight )
                            {
                                qCmyka = m_spqSrcImage->GetPixelCmyka(iShX,iShY);
                                qSum += qCmyka*((float)aiMatrix[iCell]);
                                iValidCellSum += aiMatrix[iCell];
                            }

                            iCell++;
                        }
                    }
                    fWCoeff = ( iValidCellSum == iCellSum ? fInvWeight : 1.0f/(fInvCellSumMulW*iValidCellSum) );

                    // assign weighted values
                    qCmyka = qSum*fWCoeff;
                    if ( !m_spqSrcImage->m_bIsHighDynamicRange )
                        qCmyka.Clamp();
                    spqNewImage->SetPixelCmyka(qCmyka,iX,iY);
                }

                qProgress.MakeAdvance();
            }
        }
        else if ( m_spqSrcImage->m_eColorModel == RasterImage::CMT_LAB )
        {
            ColorLab qSum, qLab;
            for (int iY = 0; iY < m_spqSrcImage->m_iHeight; iY++)
            {
                for (int iX = 0; iX < m_spqSrcImage->m_iWidth; iX++)
                {
                    iCornX = iX - iHalfN;
                    iCornY = iY - iHalfN;
                    iCell = 0;
                    for (int i = 0; i < 3; i++)
                        qSum[i] = 0.0f;

                    // lay the matrix on
                    iValidCellSum = 0;
                    for (int i0 = 0; i0 < iN; i0++)
                    {
                        iShY = iCornY + i0;
                        for (int i1 = 0; i1 < iN; i1++)
                        {
                            iShX = iCornX + i1;

                            if ( 0 <= iShX && iShX < m_spqSrcImage->m_iWidth
                            &&   0 <= iShY && iShY < m_spqSrcImage->m_iHeight )
                            {
                                qLab = m_spqSrcImage->GetPixelLab(iShX,iShY);
                                qSum += qLab*((float)aiMatrix[iCell]);
                                iValidCellSum += aiMatrix[iCell];
                            }

                            iCell++;
                        }
                    }
                    fWCoeff = ( iValidCellSum == iCellSum ? fInvWeight : 1.0f/(fInvCellSumMulW*iValidCellSum) );

                    // assign weighted values
                    qLab = qSum*fWCoeff;
                    if ( !m_spqSrcImage->m_bIsHighDynamicRange )
                        qLab.Clamp();
                    spqNewImage->SetPixelLab(qLab,iX,iY);
                }

                qProgress.MakeAdvance();
            }
        }
        else  // m_spqSrcImage->m_eColorModel = RasterImage::CMT_LAB_WITH_ALPHA
        {
            ColorLaba qSum, qLaba;
            for (int iY = 0; iY < m_spqSrcImage->m_iHeight; iY++)
            {
                for (int iX = 0; iX < m_spqSrcImage->m_iWidth; iX++)
                {
                    iCornX = iX - iHalfN;
                    iCornY = iY - iHalfN;
                    iCell = 0;
                    for (int i = 0; i < 4; i++)
                        qSum[i] = 0.0f;

                    // lay the matrix on
                    iValidCellSum = 0;
                    for (int i0 = 0; i0 < iN; i0++)
                    {
                        iShY = iCornY + i0;
                        for (int i1 = 0; i1 < iN; i1++)
                        {
                            iShX = iCornX + i1;

                            if ( 0 <= iShX && iShX < m_spqSrcImage->m_iWidth
                            &&   0 <= iShY && iShY < m_spqSrcImage->m_iHeight )
                            {
                                qLaba = m_spqSrcImage->GetPixelLaba(iShX,iShY);
                                qSum += qLaba*((float)aiMatrix[iCell]);
                                iValidCellSum += aiMatrix[iCell];
                            }

                            iCell++;
                        }
                    }
                    fWCoeff = ( iValidCellSum == iCellSum ? fInvWeight : 1.0f/(fInvCellSumMulW*iValidCellSum) );

                    // assign weighted values
                    qLaba = qSum*fWCoeff;
                    if ( !m_spqSrcImage->m_bIsHighDynamicRange )
                        qLaba.Clamp();
                    spqNewImage->SetPixelLaba(qLaba,iX,iY);
                }

                qProgress.MakeAdvance();
            }
        }
    }

    return spqNewImage;
}
//------------------------------------------------------------------------------------------------------------------



