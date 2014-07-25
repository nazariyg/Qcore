#include "QRasterImageConversion.h"
#include "QProgressProbe.h"
#include "QColorModelConversion.h"
using namespace Q;

//------------------------------------------------------------------------------------------------------------------
RasterImageConversion::RasterImageConversion (const RasterImagePtr& rspqImage, RasterImage::ImageType eTargetType)
    :
    m_spqSrcImage(rspqImage),
    m_eTargetType(eTargetType),
    m_iWidth(rspqImage->GetWidth()),
    m_iHeight(rspqImage->GetHeight()),
    m_bTryToHoldTransparency(false)
{
}
//------------------------------------------------------------------------------------------------------------------
RasterImagePtr RasterImageConversion::GetConverted ()
{
    assert( !(m_spqSrcImage->GetType() != RasterImage::IT_PALETTED
        &&  m_spqSrcImage->GetType() != RasterImage::IT_PALETTED_WITH_ALPHA
        &&  (m_eTargetType == RasterImage::IT_PALETTED
        ||  m_eTargetType == RasterImage::IT_PALETTED_WITH_ALPHA)) );

    // for a progress bar
    ProgressProbe qProgress(this);

    if ( m_spqSrcImage->GetType() == m_eTargetType )
    {
        RasterImagePtr spqDuplicate = m_spqSrcImage->GetDuplicate();
        qProgress.SetDone(1.0f);
        return spqDuplicate;
    }

    RasterImagePtr spqNewImage;
    if ( (m_spqSrcImage->GetType() == RasterImage::IT_PALETTED
    ||   m_spqSrcImage->GetType() == RasterImage::IT_PALETTED_WITH_ALPHA)
    &&   (m_eTargetType == RasterImage::IT_PALETTED
    ||   m_eTargetType == RasterImage::IT_PALETTED_WITH_ALPHA) )
    {
        // converting from paletted to paletted
        spqNewImage = new RasterImage(m_eTargetType,m_iWidth,m_iHeight,m_spqSrcImage->GetPalette());
        MemCpy(spqNewImage->GetImage(),m_spqSrcImage->GetImage(),m_iHeight*m_spqSrcImage->GetRowStride());
        if ( m_eTargetType == RasterImage::IT_PALETTED_WITH_ALPHA )
        {
            // reset alpha values to 255
            for (int i = 0; i < spqNewImage->GetPaletteSize(); i++)
                spqNewImage->GetPaletteColor(i).A() = 255;
        }
        qProgress.SetDone(1.0f);

        goto ConversionEnd;
    }
    else
    {
        RasterImage qTestImage(m_eTargetType,1,1);
        if ( qTestImage.GetPixelComponentType() != RasterImage::PCT_FLOATING_POINT )
            spqNewImage = new RasterImage(m_eTargetType,m_iWidth,m_iHeight,false);
        else
            spqNewImage = new RasterImage(m_eTargetType,m_iWidth,m_iHeight,m_spqSrcImage->IsHighDynamicRange());
    }

    // for paletted-related conversions
    static const float s_fInv255 = 1.0f/255;

    // RGB weighting factors
    static const float s_afPerception[3] = {0.3086f, 0.6094f, 0.0820f};

    RasterImage::ColorModelType eTargetColorModel = spqNewImage->GetColorModel();
    bool bDoClamp = !spqNewImage->IsHighDynamicRange();

    // do the conversion
    qProgress.SetAdvance(1.0f/m_iHeight);
    if ( m_spqSrcImage->GetColorModel() == RasterImage::CMT_PALETTED )
    {
        float afFlt[256];
        for (int i = 0; i < 256; i++)
            afFlt[i] = s_fInv255*i;

        if ( eTargetColorModel == RasterImage::CMT_GRAYSCALE )
        {
            float aafMul[3][256];
            for (int i0 = 0; i0 < 3; i0++)
            {
                for (int i1 = 0; i1 < 256; i1++)
                    aafMul[i0][i1] = afFlt[i1]*s_afPerception[i0];
            }

            for (int iY = 0; iY < m_iHeight; iY++)
            {
                for (int iX = 0; iX < m_iWidth; iX++)
                {
                    const PaletteColor& rqPalC = m_spqSrcImage->GetPaletteColor(
                        m_spqSrcImage->GetPixelIndex(iX,iY));
                    spqNewImage->SetPixelIntensity(
                        aafMul[0][rqPalC.R()]+
                        aafMul[1][rqPalC.G()]+
                        aafMul[2][rqPalC.B()],iX,iY);
                }

                qProgress.MakeAdvance();
            }
        }
        else if ( eTargetColorModel == RasterImage::CMT_GRAYSCALE_WITH_ALPHA )
        {
            float aafMul[3][256];
            for (int i0 = 0; i0 < 3; i0++)
            {
                for (int i1 = 0; i1 < 256; i1++)
                    aafMul[i0][i1] = afFlt[i1]*s_afPerception[i0];
            }

            for (int iY = 0; iY < m_iHeight; iY++)
            {
                for (int iX = 0; iX < m_iWidth; iX++)
                {
                    const PaletteColor& rqPalC = m_spqSrcImage->GetPaletteColor(
                        m_spqSrcImage->GetPixelIndex(iX,iY));
                    spqNewImage->SetPixelIntensityWithAlpha(Link<float,float>(
                        aafMul[0][rqPalC.R()]+
                        aafMul[1][rqPalC.G()]+
                        aafMul[2][rqPalC.B()],
                        1.0f),iX,iY);
                }

                qProgress.MakeAdvance();
            }
        }
        else if ( eTargetColorModel == RasterImage::CMT_RGB )
        {
            for (int iY = 0; iY < m_iHeight; iY++)
            {
                for (int iX = 0; iX < m_iWidth; iX++)
                {
                    const PaletteColor& rqPalC = m_spqSrcImage->GetPaletteColor(
                        m_spqSrcImage->GetPixelIndex(iX,iY));
                    spqNewImage->SetPixelRgb(ColorRgb(
                        afFlt[rqPalC.R()],
                        afFlt[rqPalC.G()],
                        afFlt[rqPalC.B()]),iX,iY);
                }

                qProgress.MakeAdvance();
            }
        }
        else if ( eTargetColorModel == RasterImage::CMT_RGBA )
        {
            for (int iY = 0; iY < m_iHeight; iY++)
            {
                for (int iX = 0; iX < m_iWidth; iX++)
                {
                    const PaletteColor& rqPalC = m_spqSrcImage->GetPaletteColor(
                        m_spqSrcImage->GetPixelIndex(iX,iY));
                    spqNewImage->SetPixelRgba(ColorRgba(
                        afFlt[rqPalC.R()],
                        afFlt[rqPalC.G()],
                        afFlt[rqPalC.B()],
                        1.0f),iX,iY);
                }

                qProgress.MakeAdvance();
            }
        }
        else if ( eTargetColorModel == RasterImage::CMT_CMYK )
        {
            for (int iY = 0; iY < m_iHeight; iY++)
            {
                for (int iX = 0; iX < m_iWidth; iX++)
                {
                    const PaletteColor& rqPalC = m_spqSrcImage->GetPaletteColor(
                        m_spqSrcImage->GetPixelIndex(iX,iY));
                    spqNewImage->SetPixelCmyk(ColorModelConversion::GetConvertedToCmyk(ColorRgb(
                        afFlt[rqPalC.R()],
                        afFlt[rqPalC.G()],
                        afFlt[rqPalC.B()]),bDoClamp),iX,iY);
                }

                qProgress.MakeAdvance();
            }
        }
        else if ( eTargetColorModel == RasterImage::CMT_CMYK_WITH_ALPHA )
        {
            ColorCmyk qCmyk;
            for (int iY = 0; iY < m_iHeight; iY++)
            {
                for (int iX = 0; iX < m_iWidth; iX++)
                {
                    const PaletteColor& rqPalC = m_spqSrcImage->GetPaletteColor(
                        m_spqSrcImage->GetPixelIndex(iX,iY));
                    qCmyk = ColorModelConversion::GetConvertedToCmyk(ColorRgb(
                        afFlt[rqPalC.R()],
                        afFlt[rqPalC.G()],
                        afFlt[rqPalC.B()]),bDoClamp);
                    spqNewImage->SetPixelCmyka(ColorCmyka(
                        qCmyk.C(),
                        qCmyk.M(),
                        qCmyk.Y(),
                        qCmyk.K(),
                        1.0f),iX,iY);
                }

                qProgress.MakeAdvance();
            }
        }
        else if ( eTargetColorModel == RasterImage::CMT_LAB )
        {
            for (int iY = 0; iY < m_iHeight; iY++)
            {
                for (int iX = 0; iX < m_iWidth; iX++)
                {
                    const PaletteColor& rqPalC = m_spqSrcImage->GetPaletteColor(
                        m_spqSrcImage->GetPixelIndex(iX,iY));
                    spqNewImage->SetPixelLab(ColorModelConversion::GetConvertedToLab(ColorRgb(
                        afFlt[rqPalC.R()],
                        afFlt[rqPalC.G()],
                        afFlt[rqPalC.B()]),bDoClamp),iX,iY);
                }

                qProgress.MakeAdvance();
            }
        }
        else  // eTargetColorModel = RasterImage::CMT_LAB_WITH_ALPHA
        {
            ColorLab qLab;
            for (int iY = 0; iY < m_iHeight; iY++)
            {
                for (int iX = 0; iX < m_iWidth; iX++)
                {
                    const PaletteColor& rqPalC = m_spqSrcImage->GetPaletteColor(
                        m_spqSrcImage->GetPixelIndex(iX,iY));
                    qLab = ColorModelConversion::GetConvertedToLab(ColorRgb(
                        afFlt[rqPalC.R()],
                        afFlt[rqPalC.G()],
                        afFlt[rqPalC.B()]),bDoClamp);
                    spqNewImage->SetPixelLaba(ColorLaba(
                        qLab.L(),
                        qLab.A(),
                        qLab.B(),
                        1.0f),iX,iY);
                }

                qProgress.MakeAdvance();
            }
        }
    }
    else if ( m_spqSrcImage->GetColorModel() == RasterImage::CMT_PALETTED_WITH_ALPHA )
    {
        float afFlt[256];
        for (int i = 0; i < 256; i++)
            afFlt[i] = s_fInv255*i;

        if ( eTargetColorModel == RasterImage::CMT_GRAYSCALE )
        {
            float aafMul[3][256];
            for (int i0 = 0; i0 < 3; i0++)
            {
                for (int i1 = 0; i1 < 256; i1++)
                    aafMul[i0][i1] = afFlt[i1]*s_afPerception[i0];
            }

            if ( !m_qMatte.Specified )
            {
                for (int iY = 0; iY < m_iHeight; iY++)
                {
                    for (int iX = 0; iX < m_iWidth; iX++)
                    {
                        const PaletteColor& rqPalC = m_spqSrcImage->GetPaletteColor(
                            m_spqSrcImage->GetPixelIndex(iX,iY));
                        spqNewImage->SetPixelIntensity(
                            aafMul[0][rqPalC.R()]+
                            aafMul[1][rqPalC.G()]+
                            aafMul[2][rqPalC.B()],iX,iY);
                    }

                    qProgress.MakeAdvance();
                }
            }
            else
            {
                for (int iY = 0; iY < m_iHeight; iY++)
                {
                    for (int iX = 0; iX < m_iWidth; iX++)
                    {
                        const PaletteColor& rqPalC = m_spqSrcImage->GetPaletteColor(
                            m_spqSrcImage->GetPixelIndex(iX,iY));
                        spqNewImage->SetPixelIntensity((
                            aafMul[0][rqPalC.R()]+
                            aafMul[1][rqPalC.G()]+
                            aafMul[2][rqPalC.B()])*afFlt[rqPalC.A()]+m_qMatte.Intensity*(1.0f-afFlt[rqPalC.A()]),
                            iX,iY);
                    }

                    qProgress.MakeAdvance();
                }
            }
        }
        else if ( eTargetColorModel == RasterImage::CMT_GRAYSCALE_WITH_ALPHA )
        {
            float aafMul[3][256];
            for (int i0 = 0; i0 < 3; i0++)
            {
                for (int i1 = 0; i1 < 256; i1++)
                    aafMul[i0][i1] = afFlt[i1]*s_afPerception[i0];
            }

            for (int iY = 0; iY < m_iHeight; iY++)
            {
                for (int iX = 0; iX < m_iWidth; iX++)
                {
                    const PaletteColor& rqPalC = m_spqSrcImage->GetPaletteColor(
                        m_spqSrcImage->GetPixelIndex(iX,iY));
                    spqNewImage->SetPixelIntensityWithAlpha(Link<float,float>(
                        aafMul[0][rqPalC.R()]+
                        aafMul[1][rqPalC.G()]+
                        aafMul[2][rqPalC.B()],
                        afFlt[rqPalC.A()]),iX,iY);
                }

                qProgress.MakeAdvance();
            }
        }
        else if ( eTargetColorModel == RasterImage::CMT_RGB )
        {
            if ( !m_qMatte.Specified )
            {
                for (int iY = 0; iY < m_iHeight; iY++)
                {
                    for (int iX = 0; iX < m_iWidth; iX++)
                    {
                        const PaletteColor& rqPalC = m_spqSrcImage->GetPaletteColor(
                            m_spqSrcImage->GetPixelIndex(iX,iY));
                        spqNewImage->SetPixelRgb(ColorRgb(
                            afFlt[rqPalC.R()],
                            afFlt[rqPalC.G()],
                            afFlt[rqPalC.B()]),iX,iY);
                    }

                    qProgress.MakeAdvance();
                }
            }
            else
            {
                for (int iY = 0; iY < m_iHeight; iY++)
                {
                    for (int iX = 0; iX < m_iWidth; iX++)
                    {
                        const PaletteColor& rqPalC = m_spqSrcImage->GetPaletteColor(
                            m_spqSrcImage->GetPixelIndex(iX,iY));
                        spqNewImage->SetPixelRgb(ColorRgb(
                            afFlt[rqPalC.R()],
                            afFlt[rqPalC.G()],
                            afFlt[rqPalC.B()])*afFlt[rqPalC.A()]+m_qMatte.RgbColor*(1.0f-afFlt[rqPalC.A()]),iX,iY);
                    }

                    qProgress.MakeAdvance();
                }
            }
        }
        else if ( eTargetColorModel == RasterImage::CMT_RGBA )
        {
            for (int iY = 0; iY < m_iHeight; iY++)
            {
                for (int iX = 0; iX < m_iWidth; iX++)
                {
                    const PaletteColor& rqPalC = m_spqSrcImage->GetPaletteColor(
                        m_spqSrcImage->GetPixelIndex(iX,iY));
                    spqNewImage->SetPixelRgba(ColorRgba(
                        afFlt[rqPalC.R()],
                        afFlt[rqPalC.G()],
                        afFlt[rqPalC.B()],
                        afFlt[rqPalC.A()]),iX,iY);
                }

                qProgress.MakeAdvance();
            }
        }
        else if ( eTargetColorModel == RasterImage::CMT_CMYK )
        {
            if ( !m_qMatte.Specified )
            {
                for (int iY = 0; iY < m_iHeight; iY++)
                {
                    for (int iX = 0; iX < m_iWidth; iX++)
                    {
                        const PaletteColor& rqPalC = m_spqSrcImage->GetPaletteColor(
                            m_spqSrcImage->GetPixelIndex(iX,iY));
                        spqNewImage->SetPixelCmyk(ColorModelConversion::GetConvertedToCmyk(ColorRgb(
                            afFlt[rqPalC.R()],
                            afFlt[rqPalC.G()],
                            afFlt[rqPalC.B()]),bDoClamp),iX,iY);
                    }

                    qProgress.MakeAdvance();
                }
            }
            else
            {
                for (int iY = 0; iY < m_iHeight; iY++)
                {
                    for (int iX = 0; iX < m_iWidth; iX++)
                    {
                        const PaletteColor& rqPalC = m_spqSrcImage->GetPaletteColor(
                            m_spqSrcImage->GetPixelIndex(iX,iY));
                        spqNewImage->SetPixelCmyk(ColorModelConversion::GetConvertedToCmyk(ColorRgb(
                            afFlt[rqPalC.R()],
                            afFlt[rqPalC.G()],
                            afFlt[rqPalC.B()]),bDoClamp)*afFlt[rqPalC.A()]+m_qMatte.CmykColor*
                            (1.0f-afFlt[rqPalC.A()]),iX,iY);
                    }

                    qProgress.MakeAdvance();
                }
            }
        }
        else if ( eTargetColorModel == RasterImage::CMT_CMYK_WITH_ALPHA )
        {
            ColorCmyk qCmyk;
            for (int iY = 0; iY < m_iHeight; iY++)
            {
                for (int iX = 0; iX < m_iWidth; iX++)
                {
                    const PaletteColor& rqPalC = m_spqSrcImage->GetPaletteColor(
                        m_spqSrcImage->GetPixelIndex(iX,iY));
                    qCmyk = ColorModelConversion::GetConvertedToCmyk(ColorRgb(
                        afFlt[rqPalC.R()],
                        afFlt[rqPalC.G()],
                        afFlt[rqPalC.B()]),bDoClamp);
                    spqNewImage->SetPixelCmyka(ColorCmyka(
                        qCmyk.C(),
                        qCmyk.M(),
                        qCmyk.Y(),
                        qCmyk.K(),
                        afFlt[rqPalC.A()]),iX,iY);
                }

                qProgress.MakeAdvance();
            }
        }
        else if ( eTargetColorModel == RasterImage::CMT_LAB )
        {
            if ( !m_qMatte.Specified )
            {
                for (int iY = 0; iY < m_iHeight; iY++)
                {
                    for (int iX = 0; iX < m_iWidth; iX++)
                    {
                        const PaletteColor& rqPalC = m_spqSrcImage->GetPaletteColor(
                            m_spqSrcImage->GetPixelIndex(iX,iY));
                        spqNewImage->SetPixelLab(ColorModelConversion::GetConvertedToLab(ColorRgb(
                            afFlt[rqPalC.R()],
                            afFlt[rqPalC.G()],
                            afFlt[rqPalC.B()]),bDoClamp),iX,iY);
                    }

                    qProgress.MakeAdvance();
                }
            }
            else
            {
                for (int iY = 0; iY < m_iHeight; iY++)
                {
                    for (int iX = 0; iX < m_iWidth; iX++)
                    {
                        const PaletteColor& rqPalC = m_spqSrcImage->GetPaletteColor(
                            m_spqSrcImage->GetPixelIndex(iX,iY));
                        spqNewImage->SetPixelLab(ColorModelConversion::GetConvertedToLab(ColorRgb(
                            afFlt[rqPalC.R()],
                            afFlt[rqPalC.G()],
                            afFlt[rqPalC.B()]),bDoClamp)*afFlt[rqPalC.A()]+m_qMatte.LabColor*
                            (1.0f-afFlt[rqPalC.A()]),iX,iY);
                    }

                    qProgress.MakeAdvance();
                }
            }
        }
        else  // eTargetColorModel = RasterImage::CMT_LAB_WITH_ALPHA
        {
            ColorLab qLab;
            for (int iY = 0; iY < m_iHeight; iY++)
            {
                for (int iX = 0; iX < m_iWidth; iX++)
                {
                    const PaletteColor& rqPalC = m_spqSrcImage->GetPaletteColor(
                        m_spqSrcImage->GetPixelIndex(iX,iY));
                    qLab = ColorModelConversion::GetConvertedToLab(ColorRgb(
                        afFlt[rqPalC.R()],
                        afFlt[rqPalC.G()],
                        afFlt[rqPalC.B()]),bDoClamp);
                    spqNewImage->SetPixelLaba(ColorLaba(
                        qLab.L(),
                        qLab.A(),
                        qLab.B(),
                        afFlt[rqPalC.A()]),iX,iY);
                }

                qProgress.MakeAdvance();
            }
        }
    }
    else if ( m_spqSrcImage->GetColorModel() == RasterImage::CMT_GRAYSCALE )
    {
        float fIntensity;
        if ( eTargetColorModel == RasterImage::CMT_GRAYSCALE )
        {
            for (int iY = 0; iY < m_iHeight; iY++)
            {
                for (int iX = 0; iX < m_iWidth; iX++)
                {
                    fIntensity = m_spqSrcImage->GetPixelIntensity(iX,iY);
                    if ( bDoClamp )
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
        else if ( eTargetColorModel == RasterImage::CMT_GRAYSCALE_WITH_ALPHA )
        {
            for (int iY = 0; iY < m_iHeight; iY++)
            {
                for (int iX = 0; iX < m_iWidth; iX++)
                {
                    fIntensity = m_spqSrcImage->GetPixelIntensity(iX,iY);
                    if ( bDoClamp )
                    {
                        if ( fIntensity < 0.0f )
                            fIntensity = 0.0f;
                        else if ( fIntensity > 1.0f )
                            fIntensity = 1.0f;
                    }
                    spqNewImage->SetPixelIntensityWithAlpha(Link<float,float>(
                        fIntensity,
                        1.0f),iX,iY);
                }

                qProgress.MakeAdvance();
            }
        }
        else if ( eTargetColorModel == RasterImage::CMT_RGB )
        {
            for (int iY = 0; iY < m_iHeight; iY++)
            {
                for (int iX = 0; iX < m_iWidth; iX++)
                {
                    fIntensity = m_spqSrcImage->GetPixelIntensity(iX,iY);
                    if ( bDoClamp )
                    {
                        if ( fIntensity < 0.0f )
                            fIntensity = 0.0f;
                        else if ( fIntensity > 1.0f )
                            fIntensity = 1.0f;
                    }
                    spqNewImage->SetPixelRgb(ColorRgb(
                        fIntensity,
                        fIntensity,
                        fIntensity),iX,iY);
                }

                qProgress.MakeAdvance();
            }
        }
        else if ( eTargetColorModel == RasterImage::CMT_RGBA )
        {
            for (int iY = 0; iY < m_iHeight; iY++)
            {
                for (int iX = 0; iX < m_iWidth; iX++)
                {
                    fIntensity = m_spqSrcImage->GetPixelIntensity(iX,iY);
                    if ( bDoClamp )
                    {
                        if ( fIntensity < 0.0f )
                            fIntensity = 0.0f;
                        else if ( fIntensity > 1.0f )
                            fIntensity = 1.0f;
                    }
                    spqNewImage->SetPixelRgba(ColorRgba(
                        fIntensity,
                        fIntensity,
                        fIntensity,
                        1.0f),iX,iY);
                }

                qProgress.MakeAdvance();
            }
        }
        else if ( eTargetColorModel == RasterImage::CMT_CMYK )
        {
            for (int iY = 0; iY < m_iHeight; iY++)
            {
                for (int iX = 0; iX < m_iWidth; iX++)
                {
                    fIntensity = m_spqSrcImage->GetPixelIntensity(iX,iY);
                    spqNewImage->SetPixelCmyk(ColorModelConversion::GetConvertedToCmyk(ColorRgb(
                        fIntensity,
                        fIntensity,
                        fIntensity),bDoClamp),iX,iY);
                }

                qProgress.MakeAdvance();
            }
        }
        else if ( eTargetColorModel == RasterImage::CMT_CMYK_WITH_ALPHA )
        {
            ColorCmyk qCmyk;
            for (int iY = 0; iY < m_iHeight; iY++)
            {
                for (int iX = 0; iX < m_iWidth; iX++)
                {
                    fIntensity = m_spqSrcImage->GetPixelIntensity(iX,iY);
                    qCmyk = ColorModelConversion::GetConvertedToCmyk(ColorRgb(
                        fIntensity,
                        fIntensity,
                        fIntensity),bDoClamp);
                    spqNewImage->SetPixelCmyka(ColorCmyka(
                        qCmyk.C(),
                        qCmyk.M(),
                        qCmyk.Y(),
                        qCmyk.K(),
                        1.0f),iX,iY);
                }

                qProgress.MakeAdvance();
            }
        }
        else if ( eTargetColorModel == RasterImage::CMT_LAB )
        {
            for (int iY = 0; iY < m_iHeight; iY++)
            {
                for (int iX = 0; iX < m_iWidth; iX++)
                {
                    fIntensity = m_spqSrcImage->GetPixelIntensity(iX,iY);
                    spqNewImage->SetPixelLab(ColorModelConversion::GetConvertedToLab(ColorRgb(
                        fIntensity,
                        fIntensity,
                        fIntensity),bDoClamp),iX,iY);
                }

                qProgress.MakeAdvance();
            }
        }
        else  // eTargetColorModel = RasterImage::CMT_LAB_WITH_ALPHA
        {
            ColorLab qLab;
            for (int iY = 0; iY < m_iHeight; iY++)
            {
                for (int iX = 0; iX < m_iWidth; iX++)
                {
                    fIntensity = m_spqSrcImage->GetPixelIntensity(iX,iY);
                    qLab = ColorModelConversion::GetConvertedToLab(ColorRgb(
                        fIntensity,
                        fIntensity,
                        fIntensity),bDoClamp);
                    spqNewImage->SetPixelLaba(ColorLaba(
                        qLab.L(),
                        qLab.A(),
                        qLab.B(),
                        1.0f),iX,iY);
                }

                qProgress.MakeAdvance();
            }
        }
    }
    else if ( m_spqSrcImage->GetColorModel() == RasterImage::CMT_GRAYSCALE_WITH_ALPHA )
    {
        Link<float,float> qIA;
        if ( eTargetColorModel == RasterImage::CMT_GRAYSCALE )
        {
            if ( !m_qMatte.Specified )
            {
                for (int iY = 0; iY < m_iHeight; iY++)
                {
                    for (int iX = 0; iX < m_iWidth; iX++)
                    {
                        qIA = m_spqSrcImage->GetPixelIntensityWithAlpha(iX,iY);
                        if ( bDoClamp )
                        {
                            if ( qIA.First < 0.0f )
                                qIA.First = 0.0f;
                            else if ( qIA.First > 1.0f )
                                qIA.First = 1.0f;
                        }
                        spqNewImage->SetPixelIntensity(
                            qIA.First,iX,iY);
                    }

                    qProgress.MakeAdvance();
                }
            }
            else
            {
                for (int iY = 0; iY < m_iHeight; iY++)
                {
                    for (int iX = 0; iX < m_iWidth; iX++)
                    {
                        qIA = m_spqSrcImage->GetPixelIntensityWithAlpha(iX,iY);
                        if ( bDoClamp )
                        {
                            if ( qIA.First < 0.0f )
                                qIA.First = 0.0f;
                            else if ( qIA.First > 1.0f )
                                qIA.First = 1.0f;
                        }
                        spqNewImage->SetPixelIntensity(
                            qIA.First*qIA.Second+m_qMatte.Intensity*(1.0f-qIA.Second),iX,iY);
                    }

                    qProgress.MakeAdvance();
                }
            }
        }
        else if ( eTargetColorModel == RasterImage::CMT_GRAYSCALE_WITH_ALPHA )
        {
            for (int iY = 0; iY < m_iHeight; iY++)
            {
                for (int iX = 0; iX < m_iWidth; iX++)
                {
                    qIA = m_spqSrcImage->GetPixelIntensityWithAlpha(iX,iY);
                    if ( bDoClamp )
                    {
                        if ( qIA.First < 0.0f )
                            qIA.First = 0.0f;
                        else if ( qIA.First > 1.0f )
                            qIA.First = 1.0f;
                    }
                    spqNewImage->SetPixelIntensityWithAlpha(qIA,iX,iY);
                }

                qProgress.MakeAdvance();
            }
        }
        else if ( eTargetColorModel == RasterImage::CMT_RGB )
        {
            if ( !m_qMatte.Specified )
            {
                for (int iY = 0; iY < m_iHeight; iY++)
                {
                    for (int iX = 0; iX < m_iWidth; iX++)
                    {
                        qIA = m_spqSrcImage->GetPixelIntensityWithAlpha(iX,iY);
                        if ( bDoClamp )
                        {
                            if ( qIA.First < 0.0f )
                                qIA.First = 0.0f;
                            else if ( qIA.First > 1.0f )
                                qIA.First = 1.0f;
                        }
                        spqNewImage->SetPixelRgb(ColorRgb(
                            qIA.First,
                            qIA.First,
                            qIA.First),iX,iY);
                    }

                    qProgress.MakeAdvance();
                }
            }
            else
            {
                for (int iY = 0; iY < m_iHeight; iY++)
                {
                    for (int iX = 0; iX < m_iWidth; iX++)
                    {
                        qIA = m_spqSrcImage->GetPixelIntensityWithAlpha(iX,iY);
                        if ( bDoClamp )
                        {
                            if ( qIA.First < 0.0f )
                                qIA.First = 0.0f;
                            else if ( qIA.First > 1.0f )
                                qIA.First = 1.0f;
                        }
                        spqNewImage->SetPixelRgb(ColorRgb(
                            qIA.First,
                            qIA.First,
                            qIA.First)*qIA.Second+m_qMatte.RgbColor*(1.0f-qIA.Second),iX,iY);
                    }

                    qProgress.MakeAdvance();
                }
            }
        }
        else if ( eTargetColorModel == RasterImage::CMT_RGBA )
        {
            for (int iY = 0; iY < m_iHeight; iY++)
            {
                for (int iX = 0; iX < m_iWidth; iX++)
                {
                    qIA = m_spqSrcImage->GetPixelIntensityWithAlpha(iX,iY);
                    if ( bDoClamp )
                    {
                        if ( qIA.First < 0.0f )
                            qIA.First = 0.0f;
                        else if ( qIA.First > 1.0f )
                            qIA.First = 1.0f;
                    }
                    spqNewImage->SetPixelRgba(ColorRgba(
                        qIA.First,
                        qIA.First,
                        qIA.First,
                        qIA.Second),iX,iY);
                }

                qProgress.MakeAdvance();
            }
        }
        else if ( eTargetColorModel == RasterImage::CMT_CMYK )
        {
            if ( !m_qMatte.Specified )
            {
                for (int iY = 0; iY < m_iHeight; iY++)
                {
                    for (int iX = 0; iX < m_iWidth; iX++)
                    {
                        qIA = m_spqSrcImage->GetPixelIntensityWithAlpha(iX,iY);
                        spqNewImage->SetPixelCmyk(ColorModelConversion::GetConvertedToCmyk(ColorRgb(
                            qIA.First,
                            qIA.First,
                            qIA.First),bDoClamp),iX,iY);
                    }

                    qProgress.MakeAdvance();
                }
            }
            else
            {
                for (int iY = 0; iY < m_iHeight; iY++)
                {
                    for (int iX = 0; iX < m_iWidth; iX++)
                    {
                        qIA = m_spqSrcImage->GetPixelIntensityWithAlpha(iX,iY);
                        spqNewImage->SetPixelCmyk(ColorModelConversion::GetConvertedToCmyk(ColorRgb(
                            qIA.First,
                            qIA.First,
                            qIA.First),bDoClamp)*qIA.Second+m_qMatte.CmykColor*(1.0f-qIA.Second),iX,iY);
                    }

                    qProgress.MakeAdvance();
                }
            }
        }
        else if ( eTargetColorModel == RasterImage::CMT_CMYK_WITH_ALPHA )
        {
            ColorCmyk qCmyk;
            for (int iY = 0; iY < m_iHeight; iY++)
            {
                for (int iX = 0; iX < m_iWidth; iX++)
                {
                    qIA = m_spqSrcImage->GetPixelIntensityWithAlpha(iX,iY);
                    qCmyk = ColorModelConversion::GetConvertedToCmyk(ColorRgb(
                        qIA.First,
                        qIA.First,
                        qIA.First),bDoClamp);
                    spqNewImage->SetPixelCmyka(ColorCmyka(
                        qCmyk.C(),
                        qCmyk.M(),
                        qCmyk.Y(),
                        qCmyk.K(),
                        qIA.Second),iX,iY);
                }

                qProgress.MakeAdvance();
            }
        }
        else if ( eTargetColorModel == RasterImage::CMT_LAB )
        {
            if ( !m_qMatte.Specified )
            {
                for (int iY = 0; iY < m_iHeight; iY++)
                {
                    for (int iX = 0; iX < m_iWidth; iX++)
                    {
                        qIA = m_spqSrcImage->GetPixelIntensityWithAlpha(iX,iY);
                        spqNewImage->SetPixelLab(ColorModelConversion::GetConvertedToLab(ColorRgb(
                            qIA.First,
                            qIA.First,
                            qIA.First),bDoClamp),iX,iY);
                    }

                    qProgress.MakeAdvance();
                }
            }
            else
            {
                for (int iY = 0; iY < m_iHeight; iY++)
                {
                    for (int iX = 0; iX < m_iWidth; iX++)
                    {
                        qIA = m_spqSrcImage->GetPixelIntensityWithAlpha(iX,iY);
                        spqNewImage->SetPixelLab(ColorModelConversion::GetConvertedToLab(ColorRgb(
                            qIA.First,
                            qIA.First,
                            qIA.First),bDoClamp)*qIA.Second+m_qMatte.LabColor*(1.0f-qIA.Second),iX,iY);
                    }

                    qProgress.MakeAdvance();
                }
            }
        }
        else  // eTargetColorModel = RasterImage::CMT_LAB_WITH_ALPHA
        {
            ColorLab qLab;
            for (int iY = 0; iY < m_iHeight; iY++)
            {
                for (int iX = 0; iX < m_iWidth; iX++)
                {
                    qIA = m_spqSrcImage->GetPixelIntensityWithAlpha(iX,iY);
                    qLab = ColorModelConversion::GetConvertedToLab(ColorRgb(
                        qIA.First,
                        qIA.First,
                        qIA.First),bDoClamp);
                    spqNewImage->SetPixelLaba(ColorLaba(
                        qLab.L(),
                        qLab.A(),
                        qLab.B(),
                        qIA.Second),iX,iY);
                }

                qProgress.MakeAdvance();
            }
        }
    }
    else if ( m_spqSrcImage->GetColorModel() == RasterImage::CMT_RGB )
    {
        ColorRgb qRgb;
        if ( eTargetColorModel == RasterImage::CMT_GRAYSCALE )
        {
            for (int iY = 0; iY < m_iHeight; iY++)
            {
                for (int iX = 0; iX < m_iWidth; iX++)
                {
                    qRgb = m_spqSrcImage->GetPixelRgb(iX,iY);
                    if ( bDoClamp )
                        qRgb.Clamp();
                    spqNewImage->SetPixelIntensity(
                        qRgb.R()*s_afPerception[0]+
                        qRgb.G()*s_afPerception[1]+
                        qRgb.B()*s_afPerception[2],iX,iY);
                }

                qProgress.MakeAdvance();
            }
        }
        else if ( eTargetColorModel == RasterImage::CMT_GRAYSCALE_WITH_ALPHA )
        {
            for (int iY = 0; iY < m_iHeight; iY++)
            {
                for (int iX = 0; iX < m_iWidth; iX++)
                {
                    qRgb = m_spqSrcImage->GetPixelRgb(iX,iY);
                    if ( bDoClamp )
                        qRgb.Clamp();
                    spqNewImage->SetPixelIntensityWithAlpha(Link<float,float>(
                        qRgb.R()*s_afPerception[0]+
                        qRgb.G()*s_afPerception[1]+
                        qRgb.B()*s_afPerception[2],
                        1.0f),iX,iY);
                }

                qProgress.MakeAdvance();
            }
        }
        else if ( eTargetColorModel == RasterImage::CMT_RGB )
        {
            for (int iY = 0; iY < m_iHeight; iY++)
            {
                for (int iX = 0; iX < m_iWidth; iX++)
                {
                    qRgb = m_spqSrcImage->GetPixelRgb(iX,iY);
                    if ( bDoClamp )
                        qRgb.Clamp();
                    spqNewImage->SetPixelRgb(qRgb,iX,iY);
                }

                qProgress.MakeAdvance();
            }
        }
        else if ( eTargetColorModel == RasterImage::CMT_RGBA )
        {
            for (int iY = 0; iY < m_iHeight; iY++)
            {
                for (int iX = 0; iX < m_iWidth; iX++)
                {
                    qRgb = m_spqSrcImage->GetPixelRgb(iX,iY);
                    if ( bDoClamp )
                        qRgb.Clamp();
                    spqNewImage->SetPixelRgba(ColorRgba(
                        qRgb.R(),
                        qRgb.G(),
                        qRgb.B(),
                        1.0f),iX,iY);
                }

                qProgress.MakeAdvance();
            }
        }
        else if ( eTargetColorModel == RasterImage::CMT_CMYK )
        {
            for (int iY = 0; iY < m_iHeight; iY++)
            {
                for (int iX = 0; iX < m_iWidth; iX++)
                {
                    qRgb = m_spqSrcImage->GetPixelRgb(iX,iY);
                    spqNewImage->SetPixelCmyk(ColorModelConversion::GetConvertedToCmyk(qRgb,bDoClamp),iX,iY);
                }

                qProgress.MakeAdvance();
            }
        }
        else if ( eTargetColorModel == RasterImage::CMT_CMYK_WITH_ALPHA )
        {
            ColorCmyk qCmyk;
            for (int iY = 0; iY < m_iHeight; iY++)
            {
                for (int iX = 0; iX < m_iWidth; iX++)
                {
                    qRgb = m_spqSrcImage->GetPixelRgb(iX,iY);
                    qCmyk = ColorModelConversion::GetConvertedToCmyk(qRgb,bDoClamp);
                    spqNewImage->SetPixelCmyka(ColorCmyka(
                        qCmyk.C(),
                        qCmyk.M(),
                        qCmyk.Y(),
                        qCmyk.K(),
                        1.0f),iX,iY);
                }

                qProgress.MakeAdvance();
            }
        }
        else if ( eTargetColorModel == RasterImage::CMT_LAB )
        {
            for (int iY = 0; iY < m_iHeight; iY++)
            {
                for (int iX = 0; iX < m_iWidth; iX++)
                {
                    qRgb = m_spqSrcImage->GetPixelRgb(iX,iY);
                    spqNewImage->SetPixelLab(ColorModelConversion::GetConvertedToLab(qRgb,bDoClamp),iX,iY);
                }

                qProgress.MakeAdvance();
            }
        }
        else  // eTargetColorModel = RasterImage::CMT_LAB_WITH_ALPHA
        {
            ColorLab qLab;
            for (int iY = 0; iY < m_iHeight; iY++)
            {
                for (int iX = 0; iX < m_iWidth; iX++)
                {
                    qRgb = m_spqSrcImage->GetPixelRgb(iX,iY);
                    qLab = ColorModelConversion::GetConvertedToLab(qRgb,bDoClamp);
                    spqNewImage->SetPixelLaba(ColorLaba(
                        qLab.L(),
                        qLab.A(),
                        qLab.B(),
                        1.0f),iX,iY);
                }

                qProgress.MakeAdvance();
            }
        }
    }
    else if ( m_spqSrcImage->GetColorModel() == RasterImage::CMT_RGBA )
    {
        ColorRgba qRgba;
        if ( eTargetColorModel == RasterImage::CMT_GRAYSCALE )
        {
            if ( !m_qMatte.Specified )
            {
                for (int iY = 0; iY < m_iHeight; iY++)
                {
                    for (int iX = 0; iX < m_iWidth; iX++)
                    {
                        qRgba = m_spqSrcImage->GetPixelRgba(iX,iY);
                        if ( bDoClamp )
                            qRgba.Clamp();
                        spqNewImage->SetPixelIntensity(
                            qRgba.R()*s_afPerception[0]+
                            qRgba.G()*s_afPerception[1]+
                            qRgba.B()*s_afPerception[2],iX,iY);
                    }

                    qProgress.MakeAdvance();
                }
            }
            else
            {
                for (int iY = 0; iY < m_iHeight; iY++)
                {
                    for (int iX = 0; iX < m_iWidth; iX++)
                    {
                        qRgba = m_spqSrcImage->GetPixelRgba(iX,iY);
                        if ( bDoClamp )
                            qRgba.Clamp();
                        spqNewImage->SetPixelIntensity((
                            qRgba.R()*s_afPerception[0]+
                            qRgba.G()*s_afPerception[1]+
                            qRgba.B()*s_afPerception[2])*qRgba.A()+m_qMatte.Intensity*(1.0f-qRgba.A()),iX,iY);
                    }

                    qProgress.MakeAdvance();
                }
            }
        }
        else if ( eTargetColorModel == RasterImage::CMT_GRAYSCALE_WITH_ALPHA )
        {
            for (int iY = 0; iY < m_iHeight; iY++)
            {
                for (int iX = 0; iX < m_iWidth; iX++)
                {
                    qRgba = m_spqSrcImage->GetPixelRgba(iX,iY);
                    if ( bDoClamp )
                        qRgba.Clamp();
                    spqNewImage->SetPixelIntensityWithAlpha(Link<float,float>(
                        qRgba.R()*s_afPerception[0]+
                        qRgba.G()*s_afPerception[1]+
                        qRgba.B()*s_afPerception[2],
                        qRgba.A()),iX,iY);
                }

                qProgress.MakeAdvance();
            }
        }
        else if ( eTargetColorModel == RasterImage::CMT_RGB )
        {
            if ( !m_qMatte.Specified )
            {
                for (int iY = 0; iY < m_iHeight; iY++)
                {
                    for (int iX = 0; iX < m_iWidth; iX++)
                    {
                        qRgba = m_spqSrcImage->GetPixelRgba(iX,iY);
                        if ( bDoClamp )
                            qRgba.Clamp();
                        spqNewImage->SetPixelRgb(ColorRgb(
                            qRgba.R(),
                            qRgba.G(),
                            qRgba.B()),iX,iY);
                    }

                    qProgress.MakeAdvance();
                }
            }
            else
            {
                for (int iY = 0; iY < m_iHeight; iY++)
                {
                    for (int iX = 0; iX < m_iWidth; iX++)
                    {
                        qRgba = m_spqSrcImage->GetPixelRgba(iX,iY);
                        if ( bDoClamp )
                            qRgba.Clamp();
                        spqNewImage->SetPixelRgb(ColorRgb(
                            qRgba.R(),
                            qRgba.G(),
                            qRgba.B())*qRgba.A()+m_qMatte.RgbColor*(1.0f-qRgba.A()),iX,iY);
                    }

                    qProgress.MakeAdvance();
                }
            }
        }
        else if ( eTargetColorModel == RasterImage::CMT_RGBA )
        {
            for (int iY = 0; iY < m_iHeight; iY++)
            {
                for (int iX = 0; iX < m_iWidth; iX++)
                {
                    qRgba = m_spqSrcImage->GetPixelRgba(iX,iY);
                    if ( bDoClamp )
                        qRgba.Clamp();
                    spqNewImage->SetPixelRgba(qRgba,iX,iY);
                }

                qProgress.MakeAdvance();
            }
        }
        else if ( eTargetColorModel == RasterImage::CMT_CMYK )
        {
            if ( !m_qMatte.Specified )
            {
                for (int iY = 0; iY < m_iHeight; iY++)
                {
                    for (int iX = 0; iX < m_iWidth; iX++)
                    {
                        qRgba = m_spqSrcImage->GetPixelRgba(iX,iY);
                        spqNewImage->SetPixelCmyk(ColorModelConversion::GetConvertedToCmyk(ColorRgb(
                            qRgba.R(),
                            qRgba.G(),
                            qRgba.B()),bDoClamp),iX,iY);
                    }

                    qProgress.MakeAdvance();
                }
            }
            else
            {
                for (int iY = 0; iY < m_iHeight; iY++)
                {
                    for (int iX = 0; iX < m_iWidth; iX++)
                    {
                        qRgba = m_spqSrcImage->GetPixelRgba(iX,iY);
                        spqNewImage->SetPixelCmyk(ColorModelConversion::GetConvertedToCmyk(ColorRgb(
                            qRgba.R(),
                            qRgba.G(),
                            qRgba.B()),bDoClamp)*qRgba.A()+m_qMatte.CmykColor*(1.0f-qRgba.A()),iX,iY);
                    }

                    qProgress.MakeAdvance();
                }
            }
        }
        else if ( eTargetColorModel == RasterImage::CMT_CMYK_WITH_ALPHA )
        {
            ColorCmyk qCmyk;
            for (int iY = 0; iY < m_iHeight; iY++)
            {
                for (int iX = 0; iX < m_iWidth; iX++)
                {
                    qRgba = m_spqSrcImage->GetPixelRgba(iX,iY);
                    qCmyk = ColorModelConversion::GetConvertedToCmyk(ColorRgb(
                        qRgba.R(),
                        qRgba.G(),
                        qRgba.B()),bDoClamp);
                    spqNewImage->SetPixelCmyka(ColorCmyka(
                        qCmyk.C(),
                        qCmyk.M(),
                        qCmyk.Y(),
                        qCmyk.K(),
                        qRgba.A()),iX,iY);
                }

                qProgress.MakeAdvance();
            }
        }
        else if ( eTargetColorModel == RasterImage::CMT_LAB )
        {
            if ( !m_qMatte.Specified )
            {
                for (int iY = 0; iY < m_iHeight; iY++)
                {
                    for (int iX = 0; iX < m_iWidth; iX++)
                    {
                        qRgba = m_spqSrcImage->GetPixelRgba(iX,iY);
                        spqNewImage->SetPixelLab(ColorModelConversion::GetConvertedToLab(ColorRgb(
                            qRgba.R(),
                            qRgba.G(),
                            qRgba.B()),bDoClamp),iX,iY);
                    }

                    qProgress.MakeAdvance();
                }
            }
            else
            {
                for (int iY = 0; iY < m_iHeight; iY++)
                {
                    for (int iX = 0; iX < m_iWidth; iX++)
                    {
                        qRgba = m_spqSrcImage->GetPixelRgba(iX,iY);
                        spqNewImage->SetPixelLab(ColorModelConversion::GetConvertedToLab(ColorRgb(
                            qRgba.R(),
                            qRgba.G(),
                            qRgba.B()),bDoClamp)*qRgba.A()+m_qMatte.LabColor*(1.0f-qRgba.A()),iX,iY);
                    }

                    qProgress.MakeAdvance();
                }
            }
        }
        else  // eTargetColorModel = RasterImage::CMT_LAB_WITH_ALPHA
        {
            ColorLab qLab;
            for (int iY = 0; iY < m_iHeight; iY++)
            {
                for (int iX = 0; iX < m_iWidth; iX++)
                {
                    qRgba = m_spqSrcImage->GetPixelRgba(iX,iY);
                    qLab = ColorModelConversion::GetConvertedToLab(ColorRgb(
                        qRgba.R(),
                        qRgba.G(),
                        qRgba.B()),bDoClamp);
                    spqNewImage->SetPixelLaba(ColorLaba(
                        qLab.L(),
                        qLab.A(),
                        qLab.B(),
                        qRgba.A()),iX,iY);
                }

                qProgress.MakeAdvance();
            }
        }
    }
    else if ( m_spqSrcImage->GetColorModel() == RasterImage::CMT_CMYK )
    {
        ColorCmyk qCmyk;
        if ( eTargetColorModel == RasterImage::CMT_GRAYSCALE )
        {
            ColorRgb qRgb;
            for (int iY = 0; iY < m_iHeight; iY++)
            {
                for (int iX = 0; iX < m_iWidth; iX++)
                {
                    qCmyk = m_spqSrcImage->GetPixelCmyk(iX,iY);
                    qRgb = ColorModelConversion::GetConvertedToRgb(qCmyk,bDoClamp);
                    spqNewImage->SetPixelIntensity(
                        qRgb.R()*s_afPerception[0]+
                        qRgb.G()*s_afPerception[1]+
                        qRgb.B()*s_afPerception[2],iX,iY);
                }

                qProgress.MakeAdvance();
            }
        }
        else if ( eTargetColorModel == RasterImage::CMT_GRAYSCALE_WITH_ALPHA )
        {
            ColorRgb qRgb;
            for (int iY = 0; iY < m_iHeight; iY++)
            {
                for (int iX = 0; iX < m_iWidth; iX++)
                {
                    qCmyk = m_spqSrcImage->GetPixelCmyk(iX,iY);
                    qRgb = ColorModelConversion::GetConvertedToRgb(qCmyk,bDoClamp);
                    spqNewImage->SetPixelIntensityWithAlpha(Link<float,float>(
                        qRgb.R()*s_afPerception[0]+
                        qRgb.G()*s_afPerception[1]+
                        qRgb.B()*s_afPerception[2],
                        1.0f),iX,iY);
                }

                qProgress.MakeAdvance();
            }
        }
        else if ( eTargetColorModel == RasterImage::CMT_RGB )
        {
            for (int iY = 0; iY < m_iHeight; iY++)
            {
                for (int iX = 0; iX < m_iWidth; iX++)
                {
                    qCmyk = m_spqSrcImage->GetPixelCmyk(iX,iY);
                    spqNewImage->SetPixelRgb(ColorModelConversion::GetConvertedToRgb(qCmyk,bDoClamp),iX,iY);
                }

                qProgress.MakeAdvance();
            }
        }
        else if ( eTargetColorModel == RasterImage::CMT_RGBA )
        {
            ColorRgb qRgb;
            for (int iY = 0; iY < m_iHeight; iY++)
            {
                for (int iX = 0; iX < m_iWidth; iX++)
                {
                    qCmyk = m_spqSrcImage->GetPixelCmyk(iX,iY);
                    qRgb = ColorModelConversion::GetConvertedToRgb(qCmyk,bDoClamp);
                    spqNewImage->SetPixelRgba(ColorRgba(
                        qRgb.R(),
                        qRgb.G(),
                        qRgb.B(),
                        1.0f),iX,iY);
                }

                qProgress.MakeAdvance();
            }
        }
        else if ( eTargetColorModel == RasterImage::CMT_CMYK )
        {
            for (int iY = 0; iY < m_iHeight; iY++)
            {
                for (int iX = 0; iX < m_iWidth; iX++)
                {
                    qCmyk = m_spqSrcImage->GetPixelCmyk(iX,iY);
                    if ( bDoClamp )
                        qCmyk.Clamp();
                    spqNewImage->SetPixelCmyk(qCmyk,iX,iY);
                }

                qProgress.MakeAdvance();
            }
        }
        else if ( eTargetColorModel == RasterImage::CMT_CMYK_WITH_ALPHA )
        {
            for (int iY = 0; iY < m_iHeight; iY++)
            {
                for (int iX = 0; iX < m_iWidth; iX++)
                {
                    qCmyk = m_spqSrcImage->GetPixelCmyk(iX,iY);
                    if ( bDoClamp )
                        qCmyk.Clamp();
                    spqNewImage->SetPixelCmyka(ColorCmyka(
                        qCmyk.C(),
                        qCmyk.M(),
                        qCmyk.Y(),
                        qCmyk.K(),
                        1.0f),iX,iY);
                }

                qProgress.MakeAdvance();
            }
        }
        else if ( eTargetColorModel == RasterImage::CMT_LAB )
        {
            for (int iY = 0; iY < m_iHeight; iY++)
            {
                for (int iX = 0; iX < m_iWidth; iX++)
                {
                    qCmyk = m_spqSrcImage->GetPixelCmyk(iX,iY);
                    spqNewImage->SetPixelLab(ColorModelConversion::GetConvertedToLab(
                        ColorModelConversion::GetConvertedToRgb(qCmyk,bDoClamp),bDoClamp),iX,iY);
                }

                qProgress.MakeAdvance();
            }
        }
        else  // eTargetColorModel = RasterImage::CMT_LAB_WITH_ALPHA
        {
            ColorLab qLab;
            for (int iY = 0; iY < m_iHeight; iY++)
            {
                for (int iX = 0; iX < m_iWidth; iX++)
                {
                    qCmyk = m_spqSrcImage->GetPixelCmyk(iX,iY);
                    qLab = ColorModelConversion::GetConvertedToLab(
                        ColorModelConversion::GetConvertedToRgb(qCmyk,bDoClamp),bDoClamp);
                    spqNewImage->SetPixelLaba(ColorLaba(
                        qLab.L(),
                        qLab.A(),
                        qLab.B(),
                        1.0f),iX,iY);
                }

                qProgress.MakeAdvance();
            }
        }
    }
    else if ( m_spqSrcImage->GetColorModel() == RasterImage::CMT_CMYK_WITH_ALPHA )
    {
        ColorCmyka qCmyka;
        if ( eTargetColorModel == RasterImage::CMT_GRAYSCALE )
        {
            if ( !m_qMatte.Specified )
            {
                ColorRgb qRgb;
                for (int iY = 0; iY < m_iHeight; iY++)
                {
                    for (int iX = 0; iX < m_iWidth; iX++)
                    {
                        qCmyka = m_spqSrcImage->GetPixelCmyka(iX,iY);
                        qRgb = ColorModelConversion::GetConvertedToRgb(ColorCmyk(
                            qCmyka.C(),
                            qCmyka.M(),
                            qCmyka.Y(),
                            qCmyka.K()),bDoClamp);
                        spqNewImage->SetPixelIntensity(
                            qRgb.R()*s_afPerception[0]+
                            qRgb.G()*s_afPerception[1]+
                            qRgb.B()*s_afPerception[2],iX,iY);
                    }

                    qProgress.MakeAdvance();
                }
            }
            else
            {
                ColorRgb qRgb;
                for (int iY = 0; iY < m_iHeight; iY++)
                {
                    for (int iX = 0; iX < m_iWidth; iX++)
                    {
                        qCmyka = m_spqSrcImage->GetPixelCmyka(iX,iY);
                        qRgb = ColorModelConversion::GetConvertedToRgb(ColorCmyk(
                            qCmyka.C(),
                            qCmyka.M(),
                            qCmyka.Y(),
                            qCmyka.K()),bDoClamp);
                        spqNewImage->SetPixelIntensity((
                            qRgb.R()*s_afPerception[0]+
                            qRgb.G()*s_afPerception[1]+
                            qRgb.B()*s_afPerception[2])*qCmyka.Alpha()+m_qMatte.Intensity*(1.0f-qCmyka.Alpha()),
                            iX,iY);
                    }

                    qProgress.MakeAdvance();
                }
            }
        }
        else if ( eTargetColorModel == RasterImage::CMT_GRAYSCALE_WITH_ALPHA )
        {
            ColorRgb qRgb;
            for (int iY = 0; iY < m_iHeight; iY++)
            {
                for (int iX = 0; iX < m_iWidth; iX++)
                {
                    qCmyka = m_spqSrcImage->GetPixelCmyka(iX,iY);
                    qRgb = ColorModelConversion::GetConvertedToRgb(ColorCmyk(
                        qCmyka.C(),
                        qCmyka.M(),
                        qCmyka.Y(),
                        qCmyka.K()),bDoClamp);
                    spqNewImage->SetPixelIntensityWithAlpha(Link<float,float>(
                        qRgb.R()*s_afPerception[0]+
                        qRgb.G()*s_afPerception[1]+
                        qRgb.B()*s_afPerception[2],
                        qCmyka.Alpha()),iX,iY);
                }

                qProgress.MakeAdvance();
            }
        }
        else if ( eTargetColorModel == RasterImage::CMT_RGB )
        {
            if ( !m_qMatte.Specified )
            {
                for (int iY = 0; iY < m_iHeight; iY++)
                {
                    for (int iX = 0; iX < m_iWidth; iX++)
                    {
                        qCmyka = m_spqSrcImage->GetPixelCmyka(iX,iY);
                        spqNewImage->SetPixelRgb(ColorModelConversion::GetConvertedToRgb(ColorCmyk(
                            qCmyka.C(),
                            qCmyka.M(),
                            qCmyka.Y(),
                            qCmyka.K()),bDoClamp),iX,iY);
                    }

                    qProgress.MakeAdvance();
                }
            }
            else
            {
                for (int iY = 0; iY < m_iHeight; iY++)
                {
                    for (int iX = 0; iX < m_iWidth; iX++)
                    {
                        qCmyka = m_spqSrcImage->GetPixelCmyka(iX,iY);
                        spqNewImage->SetPixelRgb(ColorModelConversion::GetConvertedToRgb(ColorCmyk(
                            qCmyka.C(),
                            qCmyka.M(),
                            qCmyka.Y(),
                            qCmyka.K()),bDoClamp)*qCmyka.Alpha()+m_qMatte.RgbColor*(1.0f-qCmyka.Alpha()),iX,iY);
                    }

                    qProgress.MakeAdvance();
                }
            }
        }
        else if ( eTargetColorModel == RasterImage::CMT_RGBA )
        {
            ColorRgb qRgb;
            for (int iY = 0; iY < m_iHeight; iY++)
            {
                for (int iX = 0; iX < m_iWidth; iX++)
                {
                    qCmyka = m_spqSrcImage->GetPixelCmyka(iX,iY);
                    qRgb = ColorModelConversion::GetConvertedToRgb(ColorCmyk(
                        qCmyka.C(),
                        qCmyka.M(),
                        qCmyka.Y(),
                        qCmyka.K()),bDoClamp);
                    spqNewImage->SetPixelRgba(ColorRgba(
                        qRgb.R(),
                        qRgb.G(),
                        qRgb.B(),
                        qCmyka.Alpha()),iX,iY);
                }

                qProgress.MakeAdvance();
            }
        }
        else if ( eTargetColorModel == RasterImage::CMT_CMYK )
        {
            if ( !m_qMatte.Specified )
            {
                for (int iY = 0; iY < m_iHeight; iY++)
                {
                    for (int iX = 0; iX < m_iWidth; iX++)
                    {
                        qCmyka = m_spqSrcImage->GetPixelCmyka(iX,iY);
                        if ( bDoClamp )
                            qCmyka.Clamp();
                        spqNewImage->SetPixelCmyk(ColorCmyk(
                            qCmyka.C(),
                            qCmyka.M(),
                            qCmyka.Y(),
                            qCmyka.K()),iX,iY);
                    }

                    qProgress.MakeAdvance();
                }
            }
            else
            {
                for (int iY = 0; iY < m_iHeight; iY++)
                {
                    for (int iX = 0; iX < m_iWidth; iX++)
                    {
                        qCmyka = m_spqSrcImage->GetPixelCmyka(iX,iY);
                        if ( bDoClamp )
                            qCmyka.Clamp();
                        spqNewImage->SetPixelCmyk(ColorCmyk(
                            qCmyka.C(),
                            qCmyka.M(),
                            qCmyka.Y(),
                            qCmyka.K())*qCmyka.Alpha()+m_qMatte.CmykColor*(1.0f-qCmyka.Alpha()),iX,iY);
                    }

                    qProgress.MakeAdvance();
                }
            }
        }
        else if ( eTargetColorModel == RasterImage::CMT_CMYK_WITH_ALPHA )
        {
            for (int iY = 0; iY < m_iHeight; iY++)
            {
                for (int iX = 0; iX < m_iWidth; iX++)
                {
                    qCmyka = m_spqSrcImage->GetPixelCmyka(iX,iY);
                    if ( bDoClamp )
                        qCmyka.Clamp();
                    spqNewImage->SetPixelCmyka(qCmyka,iX,iY);
                }

                qProgress.MakeAdvance();
            }
        }
        else if ( eTargetColorModel == RasterImage::CMT_LAB )
        {
            if ( !m_qMatte.Specified )
            {
                for (int iY = 0; iY < m_iHeight; iY++)
                {
                    for (int iX = 0; iX < m_iWidth; iX++)
                    {
                        qCmyka = m_spqSrcImage->GetPixelCmyka(iX,iY);
                        spqNewImage->SetPixelLab(ColorModelConversion::GetConvertedToLab(
                            ColorModelConversion::GetConvertedToRgb(ColorCmyk(
                            qCmyka.C(),
                            qCmyka.M(),
                            qCmyka.Y(),
                            qCmyka.K()),bDoClamp),bDoClamp),iX,iY);
                    }

                    qProgress.MakeAdvance();
                }
            }
            else
            {
                for (int iY = 0; iY < m_iHeight; iY++)
                {
                    for (int iX = 0; iX < m_iWidth; iX++)
                    {
                        qCmyka = m_spqSrcImage->GetPixelCmyka(iX,iY);
                        spqNewImage->SetPixelLab(ColorModelConversion::GetConvertedToLab(
                            ColorModelConversion::GetConvertedToRgb(ColorCmyk(
                            qCmyka.C(),
                            qCmyka.M(),
                            qCmyka.Y(),
                            qCmyka.K()),bDoClamp),bDoClamp)*qCmyka.Alpha()+m_qMatte.LabColor*(1.0f-qCmyka.Alpha()),
                            iX,iY);
                    }

                    qProgress.MakeAdvance();
                }
            }
        }
        else  // eTargetColorModel = RasterImage::CMT_LAB_WITH_ALPHA
        {
            ColorLab qLab;
            for (int iY = 0; iY < m_iHeight; iY++)
            {
                for (int iX = 0; iX < m_iWidth; iX++)
                {
                    qCmyka = m_spqSrcImage->GetPixelCmyka(iX,iY);
                    qLab = ColorModelConversion::GetConvertedToLab(
                        ColorModelConversion::GetConvertedToRgb(ColorCmyk(
                        qCmyka.C(),
                        qCmyka.M(),
                        qCmyka.Y(),
                        qCmyka.K()),bDoClamp),bDoClamp);
                    spqNewImage->SetPixelLaba(ColorLaba(
                        qLab.L(),
                        qLab.A(),
                        qLab.B(),
                        qCmyka.Alpha()),iX,iY);
                }

                qProgress.MakeAdvance();
            }
        }
    }
    else if ( m_spqSrcImage->GetColorModel() == RasterImage::CMT_LAB )
    {
        ColorLab qLab;
        if ( eTargetColorModel == RasterImage::CMT_GRAYSCALE )
        {
            ColorRgb qRgb;
            for (int iY = 0; iY < m_iHeight; iY++)
            {
                for (int iX = 0; iX < m_iWidth; iX++)
                {
                    qLab = m_spqSrcImage->GetPixelLab(iX,iY);
                    qRgb = ColorModelConversion::GetConvertedToRgb(qLab,bDoClamp);
                    spqNewImage->SetPixelIntensity(
                        qRgb.R()*s_afPerception[0]+
                        qRgb.G()*s_afPerception[1]+
                        qRgb.B()*s_afPerception[2],iX,iY);
                }

                qProgress.MakeAdvance();
            }
        }
        else if ( eTargetColorModel == RasterImage::CMT_GRAYSCALE_WITH_ALPHA )
        {
            ColorRgb qRgb;
            for (int iY = 0; iY < m_iHeight; iY++)
            {
                for (int iX = 0; iX < m_iWidth; iX++)
                {
                    qLab = m_spqSrcImage->GetPixelLab(iX,iY);
                    qRgb = ColorModelConversion::GetConvertedToRgb(qLab,bDoClamp);
                    spqNewImage->SetPixelIntensityWithAlpha(Link<float,float>(
                        qRgb.R()*s_afPerception[0]+
                        qRgb.G()*s_afPerception[1]+
                        qRgb.B()*s_afPerception[2],
                        1.0f),iX,iY);
                }

                qProgress.MakeAdvance();
            }
        }
        else if ( eTargetColorModel == RasterImage::CMT_RGB )
        {
            for (int iY = 0; iY < m_iHeight; iY++)
            {
                for (int iX = 0; iX < m_iWidth; iX++)
                {
                    qLab = m_spqSrcImage->GetPixelLab(iX,iY);
                    spqNewImage->SetPixelRgb(ColorModelConversion::GetConvertedToRgb(qLab,bDoClamp),iX,iY);
                }

                qProgress.MakeAdvance();
            }
        }
        else if ( eTargetColorModel == RasterImage::CMT_RGBA )
        {
            ColorRgb qRgb;
            for (int iY = 0; iY < m_iHeight; iY++)
            {
                for (int iX = 0; iX < m_iWidth; iX++)
                {
                    qLab = m_spqSrcImage->GetPixelLab(iX,iY);
                    qRgb = ColorModelConversion::GetConvertedToRgb(qLab,bDoClamp);
                    spqNewImage->SetPixelRgba(ColorRgba(
                        qRgb.R(),
                        qRgb.G(),
                        qRgb.B(),
                        1.0f),iX,iY);
                }

                qProgress.MakeAdvance();
            }
        }
        else if ( eTargetColorModel == RasterImage::CMT_CMYK )
        {
            for (int iY = 0; iY < m_iHeight; iY++)
            {
                for (int iX = 0; iX < m_iWidth; iX++)
                {
                    qLab = m_spqSrcImage->GetPixelLab(iX,iY);
                    spqNewImage->SetPixelCmyk(ColorModelConversion::GetConvertedToCmyk(
                        ColorModelConversion::GetConvertedToRgb(qLab,bDoClamp),bDoClamp),iX,iY);
                }

                qProgress.MakeAdvance();
            }
        }
        else if ( eTargetColorModel == RasterImage::CMT_CMYK_WITH_ALPHA )
        {
            ColorCmyk qCmyk;
            for (int iY = 0; iY < m_iHeight; iY++)
            {
                for (int iX = 0; iX < m_iWidth; iX++)
                {
                    qLab = m_spqSrcImage->GetPixelLab(iX,iY);
                    qCmyk = ColorModelConversion::GetConvertedToCmyk(
                        ColorModelConversion::GetConvertedToRgb(qLab,bDoClamp),bDoClamp);
                    spqNewImage->SetPixelCmyka(ColorCmyka(
                        qCmyk.C(),
                        qCmyk.M(),
                        qCmyk.Y(),
                        qCmyk.K(),
                        1.0f),iX,iY);
                }

                qProgress.MakeAdvance();
            }
        }
        else if ( eTargetColorModel == RasterImage::CMT_LAB )
        {
            for (int iY = 0; iY < m_iHeight; iY++)
            {
                for (int iX = 0; iX < m_iWidth; iX++)
                {
                    qLab = m_spqSrcImage->GetPixelLab(iX,iY);
                    if ( bDoClamp )
                        qLab.Clamp();
                    spqNewImage->SetPixelLab(qLab,iX,iY);
                }

                qProgress.MakeAdvance();
            }
        }
        else  // eTargetColorModel = RasterImage::CMT_LAB_WITH_ALPHA
        {
            for (int iY = 0; iY < m_iHeight; iY++)
            {
                for (int iX = 0; iX < m_iWidth; iX++)
                {
                    qLab = m_spqSrcImage->GetPixelLab(iX,iY);
                    if ( bDoClamp )
                        qLab.Clamp();
                    spqNewImage->SetPixelLaba(ColorLaba(
                        qLab.L(),
                        qLab.A(),
                        qLab.B(),
                        1.0f),iX,iY);
                }

                qProgress.MakeAdvance();
            }
        }
    }
    else  // m_spqSrcImage->GetColorModel() = RasterImage::CMT_LAB_WITH_ALPHA
    {
        ColorLaba qLaba;
        if ( eTargetColorModel == RasterImage::CMT_GRAYSCALE )
        {
            ColorRgb qRgb;
            if ( !m_qMatte.Specified )
            {
                for (int iY = 0; iY < m_iHeight; iY++)
                {
                    for (int iX = 0; iX < m_iWidth; iX++)
                    {
                        qLaba = m_spqSrcImage->GetPixelLaba(iX,iY);
                        qRgb = ColorModelConversion::GetConvertedToRgb(ColorLab(
                            qLaba.L(),
                            qLaba.A(),
                            qLaba.B()),bDoClamp);
                        spqNewImage->SetPixelIntensity(
                            qRgb.R()*s_afPerception[0]+
                            qRgb.G()*s_afPerception[1]+
                            qRgb.B()*s_afPerception[2],iX,iY);
                    }

                    qProgress.MakeAdvance();
                }
            }
            else
            {
                for (int iY = 0; iY < m_iHeight; iY++)
                {
                    for (int iX = 0; iX < m_iWidth; iX++)
                    {
                        qLaba = m_spqSrcImage->GetPixelLaba(iX,iY);
                        qRgb = ColorModelConversion::GetConvertedToRgb(ColorLab(
                            qLaba.L(),
                            qLaba.A(),
                            qLaba.B()),bDoClamp);
                        spqNewImage->SetPixelIntensity((
                            qRgb.R()*s_afPerception[0]+
                            qRgb.G()*s_afPerception[1]+
                            qRgb.B()*s_afPerception[2])*qLaba.Alpha()+m_qMatte.Intensity*(1.0f-qLaba.Alpha()),
                            iX,iY);
                    }

                    qProgress.MakeAdvance();
                }
            }
        }
        else if ( eTargetColorModel == RasterImage::CMT_GRAYSCALE_WITH_ALPHA )
        {
            ColorRgb qRgb;
            for (int iY = 0; iY < m_iHeight; iY++)
            {
                for (int iX = 0; iX < m_iWidth; iX++)
                {
                    qLaba = m_spqSrcImage->GetPixelLaba(iX,iY);
                    qRgb = ColorModelConversion::GetConvertedToRgb(ColorLab(
                        qLaba.L(),
                        qLaba.A(),
                        qLaba.B()),bDoClamp);
                    spqNewImage->SetPixelIntensityWithAlpha(Link<float,float>(
                        qRgb.R()*s_afPerception[0]+
                        qRgb.G()*s_afPerception[1]+
                        qRgb.B()*s_afPerception[2],
                        qLaba.Alpha()),iX,iY);
                }

                qProgress.MakeAdvance();
            }
        }
        else if ( eTargetColorModel == RasterImage::CMT_RGB )
        {
            if ( !m_qMatte.Specified )
            {
                for (int iY = 0; iY < m_iHeight; iY++)
                {
                    for (int iX = 0; iX < m_iWidth; iX++)
                    {
                        qLaba = m_spqSrcImage->GetPixelLaba(iX,iY);
                        spqNewImage->SetPixelRgb(ColorModelConversion::GetConvertedToRgb(ColorLab(
                            qLaba.L(),
                            qLaba.A(),
                            qLaba.B()),bDoClamp),iX,iY);
                    }

                    qProgress.MakeAdvance();
                }
            }
            else
            {
                for (int iY = 0; iY < m_iHeight; iY++)
                {
                    for (int iX = 0; iX < m_iWidth; iX++)
                    {
                        qLaba = m_spqSrcImage->GetPixelLaba(iX,iY);
                        spqNewImage->SetPixelRgb(ColorModelConversion::GetConvertedToRgb(ColorLab(
                            qLaba.L(),
                            qLaba.A(),
                            qLaba.B()),bDoClamp)*qLaba.Alpha()+m_qMatte.RgbColor*(1.0f-qLaba.Alpha()),iX,iY);
                    }

                    qProgress.MakeAdvance();
                }
            }
        }
        else if ( eTargetColorModel == RasterImage::CMT_RGBA )
        {
            ColorRgb qRgb;
            for (int iY = 0; iY < m_iHeight; iY++)
            {
                for (int iX = 0; iX < m_iWidth; iX++)
                {
                    qLaba = m_spqSrcImage->GetPixelLaba(iX,iY);
                    qRgb = ColorModelConversion::GetConvertedToRgb(ColorLab(
                        qLaba.L(),
                        qLaba.A(),
                        qLaba.B()),bDoClamp);
                    spqNewImage->SetPixelRgba(ColorRgba(
                        qRgb.R(),
                        qRgb.G(),
                        qRgb.B(),
                        qLaba.Alpha()),iX,iY);
                }

                qProgress.MakeAdvance();
            }
        }
        else if ( eTargetColorModel == RasterImage::CMT_CMYK )
        {
            if ( !m_qMatte.Specified )
            {
                for (int iY = 0; iY < m_iHeight; iY++)
                {
                    for (int iX = 0; iX < m_iWidth; iX++)
                    {
                        qLaba = m_spqSrcImage->GetPixelLaba(iX,iY);
                        spqNewImage->SetPixelCmyk(ColorModelConversion::GetConvertedToCmyk(
                            ColorModelConversion::GetConvertedToRgb(ColorLab(
                            qLaba.L(),
                            qLaba.A(),
                            qLaba.B()),bDoClamp),bDoClamp),iX,iY);
                    }

                    qProgress.MakeAdvance();
                }
            }
            else
            {
                for (int iY = 0; iY < m_iHeight; iY++)
                {
                    for (int iX = 0; iX < m_iWidth; iX++)
                    {
                        qLaba = m_spqSrcImage->GetPixelLaba(iX,iY);
                        spqNewImage->SetPixelCmyk(ColorModelConversion::GetConvertedToCmyk(
                            ColorModelConversion::GetConvertedToRgb(ColorLab(
                            qLaba.L(),
                            qLaba.A(),
                            qLaba.B()),bDoClamp),bDoClamp)*qLaba.Alpha()+m_qMatte.CmykColor*(1.0f-qLaba.Alpha()),
                            iX,iY);
                    }

                    qProgress.MakeAdvance();
                }
            }
        }
        else if ( eTargetColorModel == RasterImage::CMT_CMYK_WITH_ALPHA )
        {
            ColorCmyk qCmyk;
            for (int iY = 0; iY < m_iHeight; iY++)
            {
                for (int iX = 0; iX < m_iWidth; iX++)
                {
                    qLaba = m_spqSrcImage->GetPixelLaba(iX,iY);
                    qCmyk = ColorModelConversion::GetConvertedToCmyk(
                        ColorModelConversion::GetConvertedToRgb(ColorLab(
                        qLaba.L(),
                        qLaba.A(),
                        qLaba.B()),bDoClamp),bDoClamp);
                    spqNewImage->SetPixelCmyka(ColorCmyka(
                        qCmyk.C(),
                        qCmyk.M(),
                        qCmyk.Y(),
                        qCmyk.K(),
                        qLaba.Alpha()),iX,iY);
                }

                qProgress.MakeAdvance();
            }
        }
        else if ( eTargetColorModel == RasterImage::CMT_LAB )
        {
            if ( !m_qMatte.Specified )
            {
                for (int iY = 0; iY < m_iHeight; iY++)
                {
                    for (int iX = 0; iX < m_iWidth; iX++)
                    {
                        qLaba = m_spqSrcImage->GetPixelLaba(iX,iY);
                        if ( bDoClamp )
                            qLaba.Clamp();
                        spqNewImage->SetPixelLab(ColorLab(
                            qLaba.L(),
                            qLaba.A(),
                            qLaba.B()),iX,iY);
                    }

                    qProgress.MakeAdvance();
                }
            }
            else
            {
                for (int iY = 0; iY < m_iHeight; iY++)
                {
                    for (int iX = 0; iX < m_iWidth; iX++)
                    {
                        qLaba = m_spqSrcImage->GetPixelLaba(iX,iY);
                        if ( bDoClamp )
                            qLaba.Clamp();
                        spqNewImage->SetPixelLab(ColorLab(
                            qLaba.L(),
                            qLaba.A(),
                            qLaba.B())*qLaba.Alpha()+m_qMatte.LabColor*(1.0f-qLaba.Alpha()),iX,iY);
                    }

                    qProgress.MakeAdvance();
                }
            }
        }
        else  // eTargetColorModel = RasterImage::CMT_LAB_WITH_ALPHA
        {
            for (int iY = 0; iY < m_iHeight; iY++)
            {
                for (int iX = 0; iX < m_iWidth; iX++)
                {
                    qLaba = m_spqSrcImage->GetPixelLaba(iX,iY);
                    if ( bDoClamp )
                        qLaba.Clamp();
                    spqNewImage->SetPixelLaba(qLaba,iX,iY);
                }

                qProgress.MakeAdvance();
            }
        }
    }

ConversionEnd:

    // process visibility/transparency
    if ( m_spqSrcImage->GetColorModel() == RasterImage::CMT_PALETTED_WITH_ALPHA
    ||   m_spqSrcImage->GetColorModel() == RasterImage::CMT_GRAYSCALE_WITH_ALPHA
    ||   m_spqSrcImage->GetColorModel() == RasterImage::CMT_RGBA
    ||   m_spqSrcImage->GetColorModel() == RasterImage::CMT_CMYK_WITH_ALPHA
    ||   m_spqSrcImage->GetColorModel() == RasterImage::CMT_LAB_WITH_ALPHA )
    {
        // conversion from an image with transparency
        if ( m_bTryToHoldTransparency
        &&   (spqNewImage->GetColorModel() == RasterImage::CMT_PALETTED
        ||   spqNewImage->GetColorModel() == RasterImage::CMT_GRAYSCALE
        ||   spqNewImage->GetColorModel() == RasterImage::CMT_RGB
        ||   spqNewImage->GetColorModel() == RasterImage::CMT_CMYK
        ||   spqNewImage->GetColorModel() == RasterImage::CMT_LAB) )
        {
            // the target color model has no alpha; make the visibility mask from the source image's
            // transparency
            RasterImage::VisibilityMask qVisibilityMask(m_spqSrcImage->GetArea());
            if ( m_spqSrcImage->GetColorModel() == RasterImage::CMT_PALETTED_WITH_ALPHA )
            {
                for (int iX = 0; iX < m_iWidth; iX++)
                {
                    for (int iY = 0; iY < m_iHeight; iY++)
                    {
                        qVisibilityMask[iY*m_iWidth+iX] = ( m_spqSrcImage->GetPaletteColor(
                            m_spqSrcImage->GetPixelIndex(iX,iY)).A() != 0 ? 1 : 0 );
                    }
                }
            }
            else if ( m_spqSrcImage->GetColorModel() == RasterImage::CMT_GRAYSCALE_WITH_ALPHA )
            {
                for (int iX = 0; iX < m_iWidth; iX++)
                {
                    for (int iY = 0; iY < m_iHeight; iY++)
                    {
                        qVisibilityMask[iY*m_iWidth+iX] =
                            ( m_spqSrcImage->GetPixelIntensityWithAlpha(iX,iY).Second != 0.0f ? 1 : 0 );
                    }
                }
            }
            else if ( m_spqSrcImage->GetColorModel() == RasterImage::CMT_RGBA )
            {
                for (int iX = 0; iX < m_iWidth; iX++)
                {
                    for (int iY = 0; iY < m_iHeight; iY++)
                    {
                        qVisibilityMask[iY*m_iWidth+iX] =
                            ( m_spqSrcImage->GetPixelRgba(iX,iY).A() != 0.0f ? 1 : 0 );
                    }
                }
            }
            else if ( m_spqSrcImage->GetColorModel() == RasterImage::CMT_CMYK_WITH_ALPHA )
            {
                for (int iX = 0; iX < m_iWidth; iX++)
                {
                    for (int iY = 0; iY < m_iHeight; iY++)
                    {
                        qVisibilityMask[iY*m_iWidth+iX] =
                            ( m_spqSrcImage->GetPixelCmyka(iX,iY).Alpha() != 0.0f ? 1 : 0 );
                    }
                }
            }
            else  // m_spqSrcImage->GetColorModel() = RasterImage::CMT_LAB_WITH_ALPHA
            {
                for (int iX = 0; iX < m_iWidth; iX++)
                {
                    for (int iY = 0; iY < m_iHeight; iY++)
                    {
                        qVisibilityMask[iY*m_iWidth+iX] =
                            ( m_spqSrcImage->GetPixelLaba(iX,iY).Alpha() != 0.0f ? 1 : 0 );
                    }
                }
            }
            spqNewImage->SetVisibilityMask(qVisibilityMask);
        }
    }
    else if ( m_spqSrcImage->HasVisibilityMask() )
    {
        // conversion from an image without transparency having a visibility mask
        if ( spqNewImage->GetColorModel() == RasterImage::CMT_PALETTED
        ||   spqNewImage->GetColorModel() == RasterImage::CMT_GRAYSCALE
        ||   spqNewImage->GetColorModel() == RasterImage::CMT_RGB
        ||   spqNewImage->GetColorModel() == RasterImage::CMT_CMYK
        ||   spqNewImage->GetColorModel() == RasterImage::CMT_LAB )
        {
            // simply transmit the visibility mask
            spqNewImage->SetVisibilityMask(m_spqSrcImage->GetVisibilityMask());
        }
        else if ( m_bTryToHoldTransparency
             &&   spqNewImage->GetColorModel() != RasterImage::CMT_PALETTED_WITH_ALPHA )
        {
            // the target color model has alpha; make transparency from the source image's visibility mask
            if ( spqNewImage->GetColorModel() == RasterImage::CMT_GRAYSCALE_WITH_ALPHA )
            {
                Link<float,float> qIA;
                for (int iX = 0; iX < m_iWidth; iX++)
                {
                    for (int iY = 0; iY < m_iHeight; iY++)
                    {
                        if ( !m_spqSrcImage->IsPixelVisible(iX,iY) )
                        {
                            qIA = spqNewImage->GetPixelIntensityWithAlpha(iX,iY);
                            qIA.Second = 0.0f;
                            spqNewImage->SetPixelIntensityWithAlpha(qIA,iX,iY);
                        }
                    }
                }
            }
            else if ( spqNewImage->GetColorModel() == RasterImage::CMT_RGBA )
            {
                ColorRgba qColorRgba;
                for (int iX = 0; iX < m_iWidth; iX++)
                {
                    for (int iY = 0; iY < m_iHeight; iY++)
                    {
                        if ( !m_spqSrcImage->IsPixelVisible(iX,iY) )
                        {
                            qColorRgba = spqNewImage->GetPixelRgba(iX,iY);
                            qColorRgba.A() = 0.0f;
                            spqNewImage->SetPixelRgba(qColorRgba,iX,iY);
                        }
                    }
                }
            }
            else if ( spqNewImage->GetColorModel() == RasterImage::CMT_CMYK_WITH_ALPHA )
            {
                ColorCmyka qColorCmyka;
                for (int iX = 0; iX < m_iWidth; iX++)
                {
                    for (int iY = 0; iY < m_iHeight; iY++)
                    {
                        if ( !m_spqSrcImage->IsPixelVisible(iX,iY) )
                        {
                            qColorCmyka = spqNewImage->GetPixelCmyka(iX,iY);
                            qColorCmyka.Alpha() = 0.0f;
                            spqNewImage->SetPixelCmyka(qColorCmyka,iX,iY);
                        }
                    }
                }
            }
            else  // spqNewImage->GetColorModel() = RasterImage::CMT_LAB_WITH_ALPHA
            {
                ColorLaba qColorLaba;
                for (int iX = 0; iX < m_iWidth; iX++)
                {
                    for (int iY = 0; iY < m_iHeight; iY++)
                    {
                        if ( !m_spqSrcImage->IsPixelVisible(iX,iY) )
                        {
                            qColorLaba = spqNewImage->GetPixelLaba(iX,iY);
                            qColorLaba.Alpha() = 0.0f;
                            spqNewImage->SetPixelLaba(qColorLaba,iX,iY);
                        }
                    }
                }
            }
        }
    }

    return spqNewImage;
}
//------------------------------------------------------------------------------------------------------------------



