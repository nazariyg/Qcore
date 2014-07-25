#include "QRasterImageTransformation.h"
#include "QRasterImageTransformation.mcr"
#include "QProgressProbe.h"
using namespace Q;

// support for bilinear interpolation
static const float gs_fBilinearFilterRadius(1.0f);
float BilinearFilter (float fDist)
{
    fDist = Mathf::FAbs(fDist);
    return ( fDist < gs_fBilinearFilterRadius ? gs_fBilinearFilterRadius - fDist : 0.0f );
}

// support for bicubic interpolation
static const float gs_fBicubicFilterRadius(2.0f);
float BicubicFilter (float fDist)
{
    static const float s_fB = 0.0f;
    static const float s_fC = 0.75f;

    static const float s_fP0 = (6.0f - 2.0f*s_fB)/6.0f;
    static const float s_fP2 = (-18.0f + 12.0f*s_fB + 6.0f*s_fC)/6.0f;
    static const float s_fP3 = (12.0f - 9.0f*s_fB - 6.0f*s_fC)/6.0f;
    static const float s_fQ0 = (8.0f*s_fB + 24.0f*s_fC)/6.0f;
    static const float s_fQ1 = (-12.0f*s_fB - 48.0f*s_fC)/6.0f;
    static const float s_fQ2 = (6.0f*s_fB + 30.0f*s_fC)/6.0f;
    static const float s_fQ3 = (-s_fB - 6.0f*s_fC)/6.0f;

    fDist = Mathf::FAbs(fDist);
    return ( fDist < 1.0f ? s_fP0 + fDist*fDist*(s_fP2 + fDist*s_fP3) :
        ( fDist < 2.0f ? s_fQ0 + fDist*(s_fQ1 + fDist*(s_fQ2 + fDist*s_fQ3)) : 0.0f ) );
}

// support for Catmull-Rom interpolation
static const float gs_fCatmullRomFilterRadius(2.0f);
float CatmullRomFilter (float fDist)
{
    if ( fDist < -2.0f )
        return 0.0f;
    if ( fDist < -1.0f )
        return 0.5f*(4.0f + fDist*(8.0f + fDist*(5.0f + fDist)));
    if ( fDist < 0.0f )
        return 0.5f*(2.0f + fDist*fDist*(-5.0f - 3.0f*fDist));
    if ( fDist < 1.0f )
        return 0.5f*(2.0f + fDist*fDist*(-5.0f + 3.0f*fDist));
    if ( fDist < 2.0f )
        return 0.5f*(4.0f + fDist*(-8.0f + fDist*(5.0f - fDist)));
    return 0.0f;
}

// support for "Lanczos 3" interpolation
static const float gs_fLanczos3FilterRadius(3.0f);
float Lanczos3Sinc (float fValue)
{
    if ( fValue != 0.0f )
    {
        fValue *= Mathf::PI;
        return Mathf::Sin(fValue)/fValue;
    }
    return 1.0f;
}
float Lanczos3Filter (float fDist)
{
    fDist = Mathf::FAbs(fDist);
    if ( fDist < gs_fLanczos3FilterRadius )
    {
        static const float s_fInvFilterRadius = 1.0f/gs_fLanczos3FilterRadius;
        return Lanczos3Sinc(fDist)*Lanczos3Sinc(fDist*s_fInvFilterRadius);
    }
    return 0.0f;
}

// support for B-spline interpolation
static const float gs_fBSplineFilterRadius(2.0f);
float BSplineFilter (float fDist)
{
    fDist = Mathf::FAbs(fDist);
    if ( fDist < 1.0f )
        return (4.0f + fDist*fDist*(-6.0f + 3.0f*fDist))*0.166666f;
    if ( fDist < 2.0f )
    {
        float fT = 2.0f - fDist;
        return fT*fT*fT*0.166666f;
    }
    return 0.0f;
}

// support for Gaussian interpolation
static const float gs_fGaussianFilterRadius(3.0f);
float GaussianFilter (float fDist)
{
    if ( Mathf::FAbs(fDist) > gs_fGaussianFilterRadius )
        return 0.0f;
    static const float s_fInvSqrtTwoPi = 1.0f/Mathf::Sqrt(Mathf::TWO_PI);
    return Mathf::Exp(-fDist*fDist*0.5f)*s_fInvSqrtTwoPi;
}

//------------------------------------------------------------------------------------------------------------------
RasterImageTransformation::RasterImageTransformation (const RasterImagePtr& rspqImage)
    :
    m_spqSrcImage(rspqImage)
{
}
//------------------------------------------------------------------------------------------------------------------
RasterImagePtr RasterImageTransformation::GetFlippedHorizontally () const
{
    // allocate destination image
    RasterImagePtr spqNewImage = m_spqSrcImage->GetDuplicate(false,false);

    // flip
    char* pcSrcRow = m_spqSrcImage->m_acImage;
    char* pcDstRow = spqNewImage->m_acImage + (spqNewImage->m_iWidth - 1)*spqNewImage->m_iBytesPerPixel;
    char* acSrcPix;
    char* acDstPix;
    for (int iY = 0; iY < m_spqSrcImage->m_iHeight; iY++)
    {
        acSrcPix = pcSrcRow;
        acDstPix = pcDstRow;
        for (int iX = 0; iX < m_spqSrcImage->m_iWidth; iX++)
        {
            MemCpy(acDstPix,acSrcPix,m_spqSrcImage->m_iBytesPerPixel);

            acSrcPix += m_spqSrcImage->m_iBytesPerPixel;
            acDstPix -= spqNewImage->m_iBytesPerPixel;
        }

        pcSrcRow += m_spqSrcImage->m_iRowStride;
        pcDstRow += spqNewImage->m_iRowStride;
    }

    if ( m_spqSrcImage->HasVisibilityMask() )
    {
        // also flip the visibility mask
        pcSrcRow = m_spqSrcImage->m_qVisibilityMask;
        pcDstRow = spqNewImage->m_qVisibilityMask + spqNewImage->m_iWidth - 1;
        for (int iY = 0; iY < m_spqSrcImage->m_iHeight; iY++)
        {
            acSrcPix = pcSrcRow;
            acDstPix = pcDstRow;
            for (int iX = 0; iX < m_spqSrcImage->m_iWidth; iX++)
                *acDstPix-- = *acSrcPix++;

            pcSrcRow += m_spqSrcImage->m_iWidth;
            pcDstRow += spqNewImage->m_iWidth;
        }
    }

    return spqNewImage;
}
//------------------------------------------------------------------------------------------------------------------
RasterImagePtr RasterImageTransformation::GetFlippedVertically () const
{
    // allocate destination image
    RasterImagePtr spqNewImage = m_spqSrcImage->GetDuplicate(false,false);

    // flip
    char* pcSrcRow = m_spqSrcImage->m_acImage;
    char* pcDstRow = spqNewImage->m_acImage + (spqNewImage->m_iHeight - 1)*spqNewImage->m_iRowStride;
    for (int iY = 0; iY < m_spqSrcImage->m_iHeight; iY++)
    {
        MemCpy(pcDstRow,pcSrcRow,m_spqSrcImage->m_iRowStride);

        pcSrcRow += m_spqSrcImage->m_iRowStride;
        pcDstRow -= spqNewImage->m_iRowStride;
    }

    if ( m_spqSrcImage->HasVisibilityMask() )
    {
        // also flip the visibility mask
        char* acSrcPix;
        char* acDstPix;
        pcSrcRow = m_spqSrcImage->m_qVisibilityMask;
        pcDstRow = spqNewImage->m_qVisibilityMask + (spqNewImage->m_iHeight - 1)*spqNewImage->m_iWidth;
        for (int iY = 0; iY < m_spqSrcImage->m_iHeight; iY++)
        {
            acSrcPix = pcSrcRow;
            acDstPix = pcDstRow;
            for (int iX = 0; iX < m_spqSrcImage->m_iWidth; iX++)
                *acDstPix++ = *acSrcPix++;

            pcSrcRow += m_spqSrcImage->m_iWidth;
            pcDstRow -= spqNewImage->m_iWidth;
        }
    }

    return spqNewImage;
}
//------------------------------------------------------------------------------------------------------------------
RasterImagePtr RasterImageTransformation::GetResampledHorizontally (int iNewWidth, InterpolationType eInterpolation)
{
    assert( iNewWidth >= 0 );
    assert( !((m_spqSrcImage->m_eColorModel == RasterImage::CMT_PALETTED
        ||  m_spqSrcImage->m_eColorModel == RasterImage::CMT_PALETTED_WITH_ALPHA)
        &&  eInterpolation != IT_NEAREST_NEIGHBOR) );

    int iOldWidth = m_spqSrcImage->m_iWidth;
    int iOldWidthMinusOne = iOldWidth - 1;
    int iNewHeight = m_spqSrcImage->m_iHeight;

    if ( iNewWidth == iOldWidth )
    {
        // nothing to do
        return m_spqSrcImage->GetDuplicate();
    }
    if ( m_spqSrcImage->m_iWidth == 0
    ||   m_spqSrcImage->m_iHeight == 0 )
    {
        // nothing to do
        return m_spqSrcImage->GetDuplicate();
    }

    // allocate destination image
    RasterImagePtr spqNewImage;
    if ( m_spqSrcImage->m_eColorModel == RasterImage::CMT_PALETTED
    ||   m_spqSrcImage->m_eColorModel == RasterImage::CMT_PALETTED_WITH_ALPHA )
    {
        spqNewImage = new RasterImage(m_spqSrcImage->m_eType,iNewWidth,iNewHeight,
            m_spqSrcImage->m_qPalette);
    }
    else
    {
        spqNewImage = new RasterImage(m_spqSrcImage->m_eType,iNewWidth,iNewHeight,
            m_spqSrcImage->m_bIsHighDynamicRange);
    }

    // for a progress bar
    ProgressProbe qProgress(this);
    qProgress.SetAdvance(1.0f/iNewHeight);

    if ( iNewWidth == 0 )
    {
        // skip resampling
        goto End;
    }

    // do actual resampling
    float fFactor = ((float)iOldWidth)/iNewWidth;
    if ( eInterpolation == IT_NEAREST_NEIGHBOR )
    {
        // without interpolation
        StillArray<int> qSrcRowShiftsX(iNewWidth);
        int iSrcX;
        for (int iDstX = 0; iDstX < iNewWidth; iDstX++)
        {
            iSrcX = (int)(((float)iDstX + 0.5f)*fFactor);
            if ( iSrcX < 0 )
                iSrcX = 0;
            else if ( iSrcX > iOldWidthMinusOne )
                iSrcX = iOldWidthMinusOne;
            qSrcRowShiftsX[iDstX] = iSrcX*m_spqSrcImage->m_iBytesPerPixel;
        }

        char* pcDstRow = spqNewImage->m_acImage;
        char* acDstPix = pcDstRow;
        char* pcSrcRow = m_spqSrcImage->m_acImage;
        char* acSrcPix;
        for (int iY = 0; iY < iNewHeight; iY++)
        {
            for (int iDstX = 0; iDstX < iNewWidth; iDstX++)
            {
                acSrcPix = pcSrcRow + qSrcRowShiftsX[iDstX];
                MemCpy(acDstPix,acSrcPix,m_spqSrcImage->m_iBytesPerPixel);

                acDstPix += spqNewImage->m_iBytesPerPixel;
            }

            pcDstRow += spqNewImage->m_iRowStride;
            acDstPix = pcDstRow;
            pcSrcRow += m_spqSrcImage->m_iRowStride;

            qProgress.MakeAdvance();
        }
    }
    else
    {
        // prepare interpolation for all rows
        float fFilterRadius, fUseFilterRadius, fDistCoeff;
        float (*oEvaluateFilter)(float);
        SWITCH_FILTER(eInterpolation);
        if ( iNewWidth < iOldWidth )
        {
            // minification
            fUseFilterRadius = fFilterRadius*fFactor;
            fDistCoeff = 1.0f/fFactor;
        }
        else
        {
            // magnification
            fUseFilterRadius = fFilterRadius;
            fDistCoeff = 1.0f;
        }
        int iFilterDiam = (int)(2.0f*fUseFilterRadius);
        StillArray<Link<float,Link<int,int> > > qRowPrecomp(iNewWidth);
        StillArray<float> qRowContribs(iNewWidth*iFilterDiam);
        float fSrcPtX, fContrib, fContribSum, fInvContribSum;
        int iInterpFromX, iInterpUpToX, iC = 0;
        for (int iDstX = 0; iDstX < iNewWidth; iDstX++)
        {
            fSrcPtX = ((float)iDstX + 0.5f)*fFactor;
            iInterpFromX = Mathf::RoundToInt(fSrcPtX-fUseFilterRadius);
            iInterpUpToX = iInterpFromX + iFilterDiam;

            Link<float,Link<int,int> >& rqPrecomp = qRowPrecomp[iDstX];
            rqPrecomp.First = fSrcPtX;
            rqPrecomp.Second.First = iInterpFromX;
            rqPrecomp.Second.Second = iInterpUpToX;

            fContribSum = 0.0f;
            for (int iSrcX = iInterpFromX, i = 0; iSrcX < iInterpUpToX; iSrcX++, i++)
            {
                fContrib = oEvaluateFilter((fSrcPtX-((float)iSrcX+0.5f))*fDistCoeff);
                qRowContribs[iC+i] = fContrib;
                fContribSum += fContrib;
            }
            fInvContribSum = 1.0f/fContribSum;
            for (int i = 0; i < iFilterDiam; i++)
                qRowContribs[iC++] *= fInvContribSum;
        }

        if ( m_spqSrcImage->HasGradIntSampleUniformity()
        &&   (m_spqSrcImage->m_iBitsPerIntegerSample == 8 || m_spqSrcImage->m_iBitsPerIntegerSample == 16) )
        {
            int iValue, iSpp = m_spqSrcImage->m_iComponentsPerPixel;
            StillArray<float> qCompAccums(iSpp);

            unsigned char* pucDstRow = (unsigned char*)spqNewImage->m_acImage;
            unsigned char* aucDstPix = pucDstRow;
            unsigned char* pucSrcRow = (unsigned char*)m_spqSrcImage->m_acImage;
            unsigned char* pucSrcComp;
            unsigned char* aucSrcPix;
            if ( m_spqSrcImage->m_iBitsPerIntegerSample == 8 )
            {
                for (int iY = 0; iY < iNewHeight; iY++)
                {
                    // interpolate
                    iC = 0;
                    for (int iDstX = 0; iDstX < iNewWidth; iDstX++)
                    {
                        const Link<float,Link<int,int> >& rqPrecomp = qRowPrecomp[iDstX];
                        fSrcPtX = rqPrecomp.First;
                        iInterpFromX = rqPrecomp.Second.First;
                        iInterpUpToX = rqPrecomp.Second.Second;

                        for (int i = 0; i < iSpp; i++)
                            qCompAccums[i] = 0.0f;
                        if ( iInterpFromX >= 0 && iInterpUpToX <= iOldWidth )
                        {
                            // in
                            pucSrcComp = pucSrcRow + iInterpFromX*m_spqSrcImage->m_iBytesPerPixel;
                            for (int i0 = 0; i0 < iFilterDiam; i0++)
                            {
                                fContrib = qRowContribs[iC++];
                                for (int i1 = 0; i1 < iSpp; i1++)
                                    qCompAccums[i1] += fContrib*(*pucSrcComp++);
                            }
                        }
                        else
                        {
                            // on an edge
                            for (int iSrcX = iInterpFromX; iSrcX < iInterpUpToX; iSrcX++)
                            {
                                fContrib = qRowContribs[iC++];
                                if ( 0 <= iSrcX && iSrcX < iOldWidth )
                                    aucSrcPix = pucSrcRow + iSrcX*m_spqSrcImage->m_iBytesPerPixel;
                                else if ( iSrcX < 0 )
                                    aucSrcPix = pucSrcRow;
                                else
                                    aucSrcPix = pucSrcRow + iOldWidthMinusOne*m_spqSrcImage->m_iBytesPerPixel;
                                for (int i = 0; i < iSpp; i++)
                                    qCompAccums[i] += fContrib*aucSrcPix[i];
                            }
                        }

                        // assign
                        for (int i = 0; i < iSpp; i++)
                        {
                            iValue = Mathf::RoundToInt(qCompAccums[i]);
                            if ( iValue < 0 )
                                iValue = 0;
                            else if ( iValue > 255 )
                                iValue = 255;
                            aucDstPix[i] = iValue;
                        }

                        aucDstPix += spqNewImage->m_iBytesPerPixel;
                    }

                    pucDstRow += spqNewImage->m_iRowStride;
                    aucDstPix = pucDstRow;
                    pucSrcRow += m_spqSrcImage->m_iRowStride;

                    qProgress.MakeAdvance();
                }
            }
            else  // m_spqSrcImage->m_iBitsPerIntegerSample = 16
            {
                for (int iY = 0; iY < iNewHeight; iY++)
                {
                    // interpolate
                    iC = 0;
                    for (int iDstX = 0; iDstX < iNewWidth; iDstX++)
                    {
                        const Link<float,Link<int,int> >& rqPrecomp = qRowPrecomp[iDstX];
                        fSrcPtX = rqPrecomp.First;
                        iInterpFromX = rqPrecomp.Second.First;
                        iInterpUpToX = rqPrecomp.Second.Second;

                        for (int i = 0; i < iSpp; i++)
                            qCompAccums[i] = 0.0f;
                        if ( iInterpFromX >= 0 && iInterpUpToX <= iOldWidth )
                        {
                            // in
                            pucSrcComp = pucSrcRow + iInterpFromX*m_spqSrcImage->m_iBytesPerPixel;
                            for (int i0 = 0; i0 < iFilterDiam; i0++)
                            {
                                fContrib = qRowContribs[iC++];
                                for (int i1 = 0; i1 < iSpp; i1++)
                                {
                                    qCompAccums[i1] += fContrib*(*((unsigned short*)pucSrcComp));
                                    pucSrcComp += 2;
                                }
                            }
                        }
                        else
                        {
                            // on an edge
                            for (int iSrcX = iInterpFromX; iSrcX < iInterpUpToX; iSrcX++)
                            {
                                fContrib = qRowContribs[iC++];
                                if ( 0 <= iSrcX && iSrcX < iOldWidth )
                                    aucSrcPix = pucSrcRow + iSrcX*m_spqSrcImage->m_iBytesPerPixel;
                                else if ( iSrcX < 0 )
                                    aucSrcPix = pucSrcRow;
                                else
                                    aucSrcPix = pucSrcRow + iOldWidthMinusOne*m_spqSrcImage->m_iBytesPerPixel;
                                for (int i = 0; i < iSpp; i++)
                                    qCompAccums[i] += fContrib*(*((unsigned short*)(aucSrcPix + i*2)));
                            }
                        }

                        // assign
                        for (int i = 0; i < iSpp; i++)
                        {
                            iValue = Mathf::RoundToInt(qCompAccums[i]);
                            if ( iValue < 0 )
                                iValue = 0;
                            else if ( iValue > 65535 )
                                iValue = 65535;
                            *((unsigned short*)(aucDstPix + i*2)) = iValue;
                        }

                        aucDstPix += spqNewImage->m_iBytesPerPixel;
                    }

                    pucDstRow += spqNewImage->m_iRowStride;
                    aucDstPix = pucDstRow;
                    pucSrcRow += m_spqSrcImage->m_iRowStride;

                    qProgress.MakeAdvance();
                }
            }
        }
        else
        {
            int iUseSrcX;
            bool bDoColorClamping = !m_spqSrcImage->m_bIsHighDynamicRange;
            if ( m_spqSrcImage->m_eColorModel == RasterImage::CMT_GRAYSCALE )
            {
                float fIntensityAccum, fIntensity;
                for (int iY = 0; iY < iNewHeight; iY++)
                {
                    // interpolate
                    iC = 0;
                    for (int iDstX = 0; iDstX < iNewWidth; iDstX++)
                    {
                        const Link<float,Link<int,int> >& rqPrecomp = qRowPrecomp[iDstX];
                        fSrcPtX = rqPrecomp.First;
                        iInterpFromX = rqPrecomp.Second.First;
                        iInterpUpToX = rqPrecomp.Second.Second;

                        fIntensityAccum = 0.0f;
                        if ( iInterpFromX >= 0 && iInterpUpToX <= iOldWidth )
                        {
                            // in
                            for (int iSrcX = iInterpFromX; iSrcX < iInterpUpToX; iSrcX++)
                            {
                                fContrib = qRowContribs[iC++];
                                fIntensity = m_spqSrcImage->GetPixelIntensity(iSrcX,iY);
                                fIntensityAccum += fIntensity*fContrib;
                            }
                        }
                        else
                        {
                            // on an edge
                            for (int iSrcX = iInterpFromX; iSrcX < iInterpUpToX; iSrcX++)
                            {
                                fContrib = qRowContribs[iC++];
                                iUseSrcX = iSrcX;
                                if ( iUseSrcX < 0 )
                                    iUseSrcX = 0;
                                else if ( iUseSrcX > iOldWidthMinusOne )
                                    iUseSrcX = iOldWidthMinusOne;
                                fIntensity = m_spqSrcImage->GetPixelIntensity(iUseSrcX,iY);
                                fIntensityAccum += fIntensity*fContrib;
                            }
                        }

                        // assign
                        if ( bDoColorClamping )
                        {
                            if ( fIntensityAccum < 0.0f )
                                fIntensityAccum = 0.0f;
                            else if ( fIntensityAccum > 1.0f )
                                fIntensityAccum = 1.0f;
                        }
                        spqNewImage->SetPixelIntensity(fIntensityAccum,iDstX,iY);
                    }

                    qProgress.MakeAdvance();
                }
            }
            else if ( m_spqSrcImage->m_eColorModel == RasterImage::CMT_GRAYSCALE_WITH_ALPHA )
            {
                Link<float,float> qIntensityWithAlphaAccum, qIntensityWithAlpha;
                for (int iY = 0; iY < iNewHeight; iY++)
                {
                    // interpolate
                    iC = 0;
                    for (int iDstX = 0; iDstX < iNewWidth; iDstX++)
                    {
                        const Link<float,Link<int,int> >& rqPrecomp = qRowPrecomp[iDstX];
                        fSrcPtX = rqPrecomp.First;
                        iInterpFromX = rqPrecomp.Second.First;
                        iInterpUpToX = rqPrecomp.Second.Second;

                        qIntensityWithAlphaAccum.First = 0.0f;
                        qIntensityWithAlphaAccum.Second = 0.0f;
                        if ( iInterpFromX >= 0 && iInterpUpToX <= iOldWidth )
                        {
                            // in
                            for (int iSrcX = iInterpFromX; iSrcX < iInterpUpToX; iSrcX++)
                            {
                                fContrib = qRowContribs[iC++];
                                qIntensityWithAlpha = m_spqSrcImage->GetPixelIntensityWithAlpha(iSrcX,iY);
                                qIntensityWithAlphaAccum.First += qIntensityWithAlpha.First*fContrib;
                                qIntensityWithAlphaAccum.Second += qIntensityWithAlpha.Second*fContrib;
                            }
                        }
                        else
                        {
                            // on an edge
                            for (int iSrcX = iInterpFromX; iSrcX < iInterpUpToX; iSrcX++)
                            {
                                fContrib = qRowContribs[iC++];
                                iUseSrcX = iSrcX;
                                if ( iUseSrcX < 0 )
                                    iUseSrcX = 0;
                                else if ( iUseSrcX > iOldWidthMinusOne )
                                    iUseSrcX = iOldWidthMinusOne;
                                qIntensityWithAlpha = m_spqSrcImage->GetPixelIntensityWithAlpha(iUseSrcX,iY);
                                qIntensityWithAlphaAccum.First += qIntensityWithAlpha.First*fContrib;
                                qIntensityWithAlphaAccum.Second += qIntensityWithAlpha.Second*fContrib;
                            }
                        }

                        // assign
                        if ( bDoColorClamping )
                        {
                            if ( qIntensityWithAlphaAccum.First < 0.0f )
                                qIntensityWithAlphaAccum.First = 0.0f;
                            else if ( qIntensityWithAlphaAccum.First > 1.0f )
                                qIntensityWithAlphaAccum.First = 1.0f;
                        }
                        if ( qIntensityWithAlphaAccum.Second < 0.0f )
                            qIntensityWithAlphaAccum.Second = 0.0f;
                        else if ( qIntensityWithAlphaAccum.Second > 1.0f )
                            qIntensityWithAlphaAccum.Second = 1.0f;
                        spqNewImage->SetPixelIntensityWithAlpha(qIntensityWithAlphaAccum,iDstX,iY);
                    }

                    qProgress.MakeAdvance();
                }
            }
            else if ( m_spqSrcImage->m_eColorModel == RasterImage::CMT_RGB )
            {
                ColorRgb qRgbAccum, qRgb;
                ColorRgb qRgbZero(0.0f,0.0f,0.0f);
                for (int iY = 0; iY < iNewHeight; iY++)
                {
                    // interpolate
                    iC = 0;
                    for (int iDstX = 0; iDstX < iNewWidth; iDstX++)
                    {
                        const Link<float,Link<int,int> >& rqPrecomp = qRowPrecomp[iDstX];
                        fSrcPtX = rqPrecomp.First;
                        iInterpFromX = rqPrecomp.Second.First;
                        iInterpUpToX = rqPrecomp.Second.Second;

                        qRgbAccum = qRgbZero;
                        if ( iInterpFromX >= 0 && iInterpUpToX <= iOldWidth )
                        {
                            // in
                            for (int iSrcX = iInterpFromX; iSrcX < iInterpUpToX; iSrcX++)
                            {
                                fContrib = qRowContribs[iC++];
                                qRgb = m_spqSrcImage->GetPixelRgb(iSrcX,iY);
                                qRgbAccum += qRgb*fContrib;
                            }
                        }
                        else
                        {
                            // on an edge
                            for (int iSrcX = iInterpFromX; iSrcX < iInterpUpToX; iSrcX++)
                            {
                                fContrib = qRowContribs[iC++];
                                iUseSrcX = iSrcX;
                                if ( iUseSrcX < 0 )
                                    iUseSrcX = 0;
                                else if ( iUseSrcX > iOldWidthMinusOne )
                                    iUseSrcX = iOldWidthMinusOne;
                                qRgb = m_spqSrcImage->GetPixelRgb(iUseSrcX,iY);
                                qRgbAccum += qRgb*fContrib;
                            }
                        }

                        // assign
                        if ( bDoColorClamping )
                        {
                            for (int i = 0; i < 3; i++)
                            {
                                if ( qRgbAccum[i] < 0.0f )
                                    qRgbAccum[i] = 0.0f;
                                else if ( qRgbAccum[i] > 1.0f )
                                    qRgbAccum[i] = 1.0f;
                            }
                        }
                        spqNewImage->SetPixelRgb(qRgbAccum,iDstX,iY);
                    }

                    qProgress.MakeAdvance();
                }
            }
            else if ( m_spqSrcImage->m_eColorModel == RasterImage::CMT_RGBA )
            {
                ColorRgba qRgbaAccum, qRgba;
                ColorRgba qRgbaZero(0.0f,0.0f,0.0f,0.0f);
                for (int iY = 0; iY < iNewHeight; iY++)
                {
                    // interpolate
                    iC = 0;
                    for (int iDstX = 0; iDstX < iNewWidth; iDstX++)
                    {
                        const Link<float,Link<int,int> >& rqPrecomp = qRowPrecomp[iDstX];
                        fSrcPtX = rqPrecomp.First;
                        iInterpFromX = rqPrecomp.Second.First;
                        iInterpUpToX = rqPrecomp.Second.Second;

                        qRgbaAccum = qRgbaZero;
                        if ( iInterpFromX >= 0 && iInterpUpToX <= iOldWidth )
                        {
                            // in
                            for (int iSrcX = iInterpFromX; iSrcX < iInterpUpToX; iSrcX++)
                            {
                                fContrib = qRowContribs[iC++];
                                qRgba = m_spqSrcImage->GetPixelRgba(iSrcX,iY);
                                qRgbaAccum += qRgba*fContrib;
                            }
                        }
                        else
                        {
                            // on an edge
                            for (int iSrcX = iInterpFromX; iSrcX < iInterpUpToX; iSrcX++)
                            {
                                fContrib = qRowContribs[iC++];
                                iUseSrcX = iSrcX;
                                if ( iUseSrcX < 0 )
                                    iUseSrcX = 0;
                                else if ( iUseSrcX > iOldWidthMinusOne )
                                    iUseSrcX = iOldWidthMinusOne;
                                qRgba = m_spqSrcImage->GetPixelRgba(iUseSrcX,iY);
                                qRgbaAccum += qRgba*fContrib;
                            }
                        }

                        // assign
                        if ( bDoColorClamping )
                        {
                            for (int i = 0; i < 3; i++)
                            {
                                if ( qRgbaAccum[i] < 0.0f )
                                    qRgbaAccum[i] = 0.0f;
                                else if ( qRgbaAccum[i] > 1.0f )
                                    qRgbaAccum[i] = 1.0f;
                            }
                        }
                        if ( qRgbaAccum[3] < 0.0f )
                            qRgbaAccum[3] = 0.0f;
                        else if ( qRgbaAccum[3] > 1.0f )
                            qRgbaAccum[3] = 1.0f;
                        spqNewImage->SetPixelRgba(qRgbaAccum,iDstX,iY);
                    }

                    qProgress.MakeAdvance();
                }
            }
            else if ( m_spqSrcImage->m_eColorModel == RasterImage::CMT_CMYK )
            {
                ColorCmyk qCmykAccum, qCmyk;
                ColorCmyk qCmykZero(0.0f,0.0f,0.0f,0.0f);
                for (int iY = 0; iY < iNewHeight; iY++)
                {
                    // interpolate
                    iC = 0;
                    for (int iDstX = 0; iDstX < iNewWidth; iDstX++)
                    {
                        const Link<float,Link<int,int> >& rqPrecomp = qRowPrecomp[iDstX];
                        fSrcPtX = rqPrecomp.First;
                        iInterpFromX = rqPrecomp.Second.First;
                        iInterpUpToX = rqPrecomp.Second.Second;

                        qCmykAccum = qCmykZero;
                        if ( iInterpFromX >= 0 && iInterpUpToX <= iOldWidth )
                        {
                            // in
                            for (int iSrcX = iInterpFromX; iSrcX < iInterpUpToX; iSrcX++)
                            {
                                fContrib = qRowContribs[iC++];
                                qCmyk = m_spqSrcImage->GetPixelCmyk(iSrcX,iY);
                                qCmykAccum += qCmyk*fContrib;
                            }
                        }
                        else
                        {
                            // on an edge
                            for (int iSrcX = iInterpFromX; iSrcX < iInterpUpToX; iSrcX++)
                            {
                                fContrib = qRowContribs[iC++];
                                iUseSrcX = iSrcX;
                                if ( iUseSrcX < 0 )
                                    iUseSrcX = 0;
                                else if ( iUseSrcX > iOldWidthMinusOne )
                                    iUseSrcX = iOldWidthMinusOne;
                                qCmyk = m_spqSrcImage->GetPixelCmyk(iUseSrcX,iY);
                                qCmykAccum += qCmyk*fContrib;
                            }
                        }

                        // assign
                        if ( bDoColorClamping )
                        {
                            for (int i = 0; i < 4; i++)
                            {
                                if ( qCmykAccum[i] < 0.0f )
                                    qCmykAccum[i] = 0.0f;
                                else if ( qCmykAccum[i] > 1.0f )
                                    qCmykAccum[i] = 1.0f;
                            }
                        }
                        spqNewImage->SetPixelCmyk(qCmykAccum,iDstX,iY);
                    }

                    qProgress.MakeAdvance();
                }
            }
            else if ( m_spqSrcImage->m_eColorModel == RasterImage::CMT_CMYK_WITH_ALPHA )
            {
                ColorCmyka qCmykaAccum, qCmyka;
                ColorCmyka qCmykaZero(0.0f,0.0f,0.0f,0.0f,0.0f);
                for (int iY = 0; iY < iNewHeight; iY++)
                {
                    // interpolate
                    iC = 0;
                    for (int iDstX = 0; iDstX < iNewWidth; iDstX++)
                    {
                        const Link<float,Link<int,int> >& rqPrecomp = qRowPrecomp[iDstX];
                        fSrcPtX = rqPrecomp.First;
                        iInterpFromX = rqPrecomp.Second.First;
                        iInterpUpToX = rqPrecomp.Second.Second;

                        qCmykaAccum = qCmykaZero;
                        if ( iInterpFromX >= 0 && iInterpUpToX <= iOldWidth )
                        {
                            // in
                            for (int iSrcX = iInterpFromX; iSrcX < iInterpUpToX; iSrcX++)
                            {
                                fContrib = qRowContribs[iC++];
                                qCmyka = m_spqSrcImage->GetPixelCmyka(iSrcX,iY);
                                qCmykaAccum += qCmyka*fContrib;
                            }
                        }
                        else
                        {
                            // on an edge
                            for (int iSrcX = iInterpFromX; iSrcX < iInterpUpToX; iSrcX++)
                            {
                                fContrib = qRowContribs[iC++];
                                iUseSrcX = iSrcX;
                                if ( iUseSrcX < 0 )
                                    iUseSrcX = 0;
                                else if ( iUseSrcX > iOldWidthMinusOne )
                                    iUseSrcX = iOldWidthMinusOne;
                                qCmyka = m_spqSrcImage->GetPixelCmyka(iUseSrcX,iY);
                                qCmykaAccum += qCmyka*fContrib;
                            }
                        }

                        // assign
                        if ( bDoColorClamping )
                        {
                            for (int i = 0; i < 4; i++)
                            {
                                if ( qCmykaAccum[i] < 0.0f )
                                    qCmykaAccum[i] = 0.0f;
                                else if ( qCmykaAccum[i] > 1.0f )
                                    qCmykaAccum[i] = 1.0f;
                            }
                        }
                        if ( qCmykaAccum[4] < 0.0f )
                            qCmykaAccum[4] = 0.0f;
                        else if ( qCmykaAccum[4] > 1.0f )
                            qCmykaAccum[4] = 1.0f;
                        spqNewImage->SetPixelCmyka(qCmykaAccum,iDstX,iY);
                    }

                    qProgress.MakeAdvance();
                }
            }
            else if ( m_spqSrcImage->m_eColorModel == RasterImage::CMT_LAB )
            {
                ColorLab qLabAccum, qLab;
                ColorLab qLabZero(0.0f,0.0f,0.0f);
                for (int iY = 0; iY < iNewHeight; iY++)
                {
                    // interpolate
                    iC = 0;
                    for (int iDstX = 0; iDstX < iNewWidth; iDstX++)
                    {
                        const Link<float,Link<int,int> >& rqPrecomp = qRowPrecomp[iDstX];
                        fSrcPtX = rqPrecomp.First;
                        iInterpFromX = rqPrecomp.Second.First;
                        iInterpUpToX = rqPrecomp.Second.Second;

                        qLabAccum = qLabZero;
                        if ( iInterpFromX >= 0 && iInterpUpToX <= iOldWidth )
                        {
                            // in
                            for (int iSrcX = iInterpFromX; iSrcX < iInterpUpToX; iSrcX++)
                            {
                                fContrib = qRowContribs[iC++];
                                qLab = m_spqSrcImage->GetPixelLab(iSrcX,iY);
                                qLabAccum += qLab*fContrib;
                            }
                        }
                        else
                        {
                            // on an edge
                            for (int iSrcX = iInterpFromX; iSrcX < iInterpUpToX; iSrcX++)
                            {
                                fContrib = qRowContribs[iC++];
                                iUseSrcX = iSrcX;
                                if ( iUseSrcX < 0 )
                                    iUseSrcX = 0;
                                else if ( iUseSrcX > iOldWidthMinusOne )
                                    iUseSrcX = iOldWidthMinusOne;
                                qLab = m_spqSrcImage->GetPixelLab(iUseSrcX,iY);
                                qLabAccum += qLab*fContrib;
                            }
                        }

                        // assign
                        if ( bDoColorClamping )
                        {
                            if ( qLabAccum[0] < 0.0f )
                                qLabAccum[0] = 0.0f;
                            else if ( qLabAccum[0] > 100.0f )
                                qLabAccum[0] = 100.0f;
                        }
                        spqNewImage->SetPixelLab(qLabAccum,iDstX,iY);
                    }

                    qProgress.MakeAdvance();
                }
            }
            else  // m_spqSrcImage->m_eColorModel = RasterImage::CMT_LAB_WITH_ALPHA
            {
                ColorLaba qLabaAccum, qLaba;
                ColorLaba qLabaZero(0.0f,0.0f,0.0f,0.0f);
                for (int iY = 0; iY < iNewHeight; iY++)
                {
                    // interpolate
                    iC = 0;
                    for (int iDstX = 0; iDstX < iNewWidth; iDstX++)
                    {
                        const Link<float,Link<int,int> >& rqPrecomp = qRowPrecomp[iDstX];
                        fSrcPtX = rqPrecomp.First;
                        iInterpFromX = rqPrecomp.Second.First;
                        iInterpUpToX = rqPrecomp.Second.Second;

                        qLabaAccum = qLabaZero;
                        if ( iInterpFromX >= 0 && iInterpUpToX <= iOldWidth )
                        {
                            // in
                            for (int iSrcX = iInterpFromX; iSrcX < iInterpUpToX; iSrcX++)
                            {
                                fContrib = qRowContribs[iC++];
                                qLaba = m_spqSrcImage->GetPixelLaba(iSrcX,iY);
                                qLabaAccum += qLaba*fContrib;
                            }
                        }
                        else
                        {
                            // on an edge
                            for (int iSrcX = iInterpFromX; iSrcX < iInterpUpToX; iSrcX++)
                            {
                                fContrib = qRowContribs[iC++];
                                iUseSrcX = iSrcX;
                                if ( iUseSrcX < 0 )
                                    iUseSrcX = 0;
                                else if ( iUseSrcX > iOldWidthMinusOne )
                                    iUseSrcX = iOldWidthMinusOne;
                                qLaba = m_spqSrcImage->GetPixelLaba(iUseSrcX,iY);
                                qLabaAccum += qLaba*fContrib;
                            }
                        }

                        // assign
                        if ( bDoColorClamping )
                        {
                            if ( qLabaAccum[0] < 0.0f )
                                qLabaAccum[0] = 0.0f;
                            else if ( qLabaAccum[0] > 100.0f )
                                qLabaAccum[0] = 100.0f;
                        }
                        if ( qLabaAccum[3] < 0.0f )
                            qLabaAccum[3] = 0.0f;
                        else if ( qLabaAccum[3] > 1.0f )
                            qLabaAccum[3] = 1.0f;
                        spqNewImage->SetPixelLaba(qLabaAccum,iDstX,iY);
                    }

                    qProgress.MakeAdvance();
                }
            }
        }
    }

End:
    if ( m_spqSrcImage->HasVisibilityMask() )
    {
        // treat the visibility mask as a grayscale image to be transformed
        TRANSFORM_VISIBILITY_MASK_AS_WELL(GetResampledHorizontally(iNewWidth,eInterpolation));
    }

    return spqNewImage;
}
//------------------------------------------------------------------------------------------------------------------
RasterImagePtr RasterImageTransformation::GetResampledVertically (int iNewHeight, InterpolationType eInterpolation)
{
    assert( iNewHeight >= 0 );
    assert( !((m_spqSrcImage->m_eColorModel == RasterImage::CMT_PALETTED
        ||  m_spqSrcImage->m_eColorModel == RasterImage::CMT_PALETTED_WITH_ALPHA)
        &&  eInterpolation != IT_NEAREST_NEIGHBOR) );

    int iOldHeight = m_spqSrcImage->m_iHeight;
    int iOldHeightMinusOne = iOldHeight - 1;
    int iNewWidth = m_spqSrcImage->m_iWidth;

    if ( iNewHeight == iOldHeight )
    {
        // nothing to do
        return m_spqSrcImage->GetDuplicate();
    }
    if ( m_spqSrcImage->m_iWidth == 0
    ||   m_spqSrcImage->m_iHeight == 0 )
    {
        // nothing to do
        return m_spqSrcImage->GetDuplicate();
    }

    // allocate destination image
    RasterImagePtr spqNewImage;
    if ( m_spqSrcImage->m_eColorModel == RasterImage::CMT_PALETTED
    ||   m_spqSrcImage->m_eColorModel == RasterImage::CMT_PALETTED_WITH_ALPHA )
    {
        spqNewImage = new RasterImage(m_spqSrcImage->m_eType,iNewWidth,iNewHeight,
            m_spqSrcImage->m_qPalette);
    }
    else
    {
        spqNewImage = new RasterImage(m_spqSrcImage->m_eType,iNewWidth,iNewHeight,
            m_spqSrcImage->m_bIsHighDynamicRange);
    }

    // for a progress bar
    ProgressProbe qProgress(this);
    qProgress.SetAdvance(1.0f/iNewWidth);

    if ( iNewHeight == 0 )
    {
        // skip resampling
        goto End;
    }

    // do actual resampling
    float fFactor = ((float)iOldHeight)/iNewHeight;
    if ( eInterpolation == IT_NEAREST_NEIGHBOR )
    {
        // without interpolation
        StillArray<int> qSrcColumnShiftsY(iNewHeight);
        int iSrcY;
        for (int iDstY = 0; iDstY < iNewHeight; iDstY++)
        {
            iSrcY = (int)(((float)iDstY + 0.5f)*fFactor);
            if ( iSrcY < 0 )
                iSrcY = 0;
            else if ( iSrcY > iOldHeightMinusOne )
                iSrcY = iOldHeightMinusOne;
            qSrcColumnShiftsY[iDstY] = iSrcY*m_spqSrcImage->m_iRowStride;
        }

        char* pcDstBase;
        char* pcSrcBase;
        char* acDstPix;
        char* acSrcPix;
        for (int iX = 0; iX < iNewWidth; iX++)
        {
            pcDstBase = spqNewImage->m_acImage + iX*spqNewImage->m_iBytesPerPixel;
            pcSrcBase = m_spqSrcImage->m_acImage + iX*m_spqSrcImage->m_iBytesPerPixel;
            for (int iDstY = 0; iDstY < iNewHeight; iDstY++)
            {
                acDstPix = pcDstBase + iDstY*spqNewImage->m_iRowStride;
                acSrcPix = pcSrcBase + qSrcColumnShiftsY[iDstY];
                MemCpy(acDstPix,acSrcPix,m_spqSrcImage->m_iBytesPerPixel);
            }

            qProgress.MakeAdvance();
        }
    }
    else
    {
        // prepare interpolation for all columns
        float fFilterRadius, fUseFilterRadius, fDistCoeff;
        float (*oEvaluateFilter)(float);
        SWITCH_FILTER(eInterpolation);
        if ( iNewHeight < iOldHeight )
        {
            // minification
            fUseFilterRadius = fFilterRadius*fFactor;
            fDistCoeff = 1.0f/fFactor;
        }
        else
        {
            // magnification
            fUseFilterRadius = fFilterRadius;
            fDistCoeff = 1.0f;
        }
        int iFilterDiam = (int)(2.0f*fUseFilterRadius);
        StillArray<Link<float,Link<int,int> > > qColumnPrecomp(iNewHeight);
        StillArray<float> qColumnContribs(iNewHeight*iFilterDiam);
        float fSrcPtY, fContrib, fContribSum, fInvContribSum;
        int iInterpFromY, iInterpUpToY, iC = 0;
        for (int iDstY = 0; iDstY < iNewHeight; iDstY++)
        {
            fSrcPtY = ((float)iDstY + 0.5f)*fFactor;
            iInterpFromY = Mathf::RoundToInt(fSrcPtY-fUseFilterRadius);
            iInterpUpToY = iInterpFromY + iFilterDiam;

            Link<float,Link<int,int> >& rqPrecomp = qColumnPrecomp[iDstY];
            rqPrecomp.First = fSrcPtY;
            rqPrecomp.Second.First = iInterpFromY;
            rqPrecomp.Second.Second = iInterpUpToY;

            fContribSum = 0.0f;
            for (int iSrcY = iInterpFromY, i = 0; iSrcY < iInterpUpToY; iSrcY++, i++)
            {
                fContrib = oEvaluateFilter((fSrcPtY-((float)iSrcY+0.5f))*fDistCoeff);
                qColumnContribs[iC+i] = fContrib;
                fContribSum += fContrib;
            }
            fInvContribSum = 1.0f/fContribSum;
            for (int i = 0; i < iFilterDiam; i++)
                qColumnContribs[iC++] *= fInvContribSum;
        }

        if ( m_spqSrcImage->HasGradIntSampleUniformity()
        &&   (m_spqSrcImage->m_iBitsPerIntegerSample == 8 || m_spqSrcImage->m_iBitsPerIntegerSample == 16) )
        {
            int iValue, iSpp = m_spqSrcImage->m_iComponentsPerPixel;
            StillArray<float> qCompAccums(iSpp);

            unsigned char* pucSrcBase;
            unsigned char* pucDstBase;
            unsigned char* aucSrcPix;
            unsigned char* aucDstPix;
            if ( m_spqSrcImage->m_iBitsPerIntegerSample == 8 )
            {
                for (int iX = 0; iX < iNewWidth; iX++)
                {
                    // interpolate
                    iC = 0;
                    pucSrcBase = (unsigned char*)(m_spqSrcImage->m_acImage + iX*m_spqSrcImage->m_iBytesPerPixel);
                    pucDstBase = (unsigned char*)(spqNewImage->m_acImage + iX*spqNewImage->m_iBytesPerPixel);
                    for (int iDstY = 0; iDstY < iNewHeight; iDstY++)
                    {
                        const Link<float,Link<int,int> >& rqPrecomp = qColumnPrecomp[iDstY];
                        fSrcPtY = rqPrecomp.First;
                        iInterpFromY = rqPrecomp.Second.First;
                        iInterpUpToY = rqPrecomp.Second.Second;

                        for (int i = 0; i < iSpp; i++)
                            qCompAccums[i] = 0.0f;
                        if ( iInterpFromY >= 0 && iInterpUpToY <= iOldHeight )
                        {
                            // in
                            for (int iSrcY = iInterpFromY; iSrcY < iInterpUpToY; iSrcY++)
                            {
                                fContrib = qColumnContribs[iC++];
                                aucSrcPix = pucSrcBase + iSrcY*m_spqSrcImage->m_iRowStride;
                                for (int i1 = 0; i1 < iSpp; i1++)
                                    qCompAccums[i1] += fContrib*aucSrcPix[i1];
                            }
                        }
                        else
                        {
                            // on an edge
                            for (int iSrcY = iInterpFromY; iSrcY < iInterpUpToY; iSrcY++)
                            {
                                fContrib = qColumnContribs[iC++];
                                if ( 0 <= iSrcY && iSrcY < iOldHeight )
                                    aucSrcPix = pucSrcBase + iSrcY*m_spqSrcImage->m_iRowStride;
                                else if ( iSrcY < 0 )
                                    aucSrcPix = pucSrcBase;
                                else
                                    aucSrcPix = pucSrcBase + iOldHeightMinusOne*m_spqSrcImage->m_iRowStride;
                                for (int i = 0; i < iSpp; i++)
                                    qCompAccums[i] += fContrib*aucSrcPix[i];
                            }
                        }

                        // assign
                        aucDstPix = pucDstBase + iDstY*spqNewImage->m_iRowStride;
                        for (int i = 0; i < iSpp; i++)
                        {
                            iValue = Mathf::RoundToInt(qCompAccums[i]);
                            if ( iValue < 0 )
                                iValue = 0;
                            else if ( iValue > 255 )
                                iValue = 255;
                            aucDstPix[i] = iValue;
                        }
                    }

                    qProgress.MakeAdvance();
                }
            }
            else  // m_spqSrcImage->m_iBitsPerIntegerSample = 16
            {
                for (int iX = 0; iX < iNewWidth; iX++)
                {
                    // interpolate
                    iC = 0;
                    pucSrcBase = (unsigned char*)(m_spqSrcImage->m_acImage + iX*m_spqSrcImage->m_iBytesPerPixel);
                    pucDstBase = (unsigned char*)(spqNewImage->m_acImage + iX*spqNewImage->m_iBytesPerPixel);
                    for (int iDstY = 0; iDstY < iNewHeight; iDstY++)
                    {
                        const Link<float,Link<int,int> >& rqPrecomp = qColumnPrecomp[iDstY];
                        fSrcPtY = rqPrecomp.First;
                        iInterpFromY = rqPrecomp.Second.First;
                        iInterpUpToY = rqPrecomp.Second.Second;

                        for (int i = 0; i < iSpp; i++)
                            qCompAccums[i] = 0.0f;
                        if ( iInterpFromY >= 0 && iInterpUpToY <= iOldHeight )
                        {
                            // in
                            for (int iSrcY = iInterpFromY; iSrcY < iInterpUpToY; iSrcY++)
                            {
                                fContrib = qColumnContribs[iC++];
                                aucSrcPix = pucSrcBase + iSrcY*m_spqSrcImage->m_iRowStride;
                                for (int i1 = 0; i1 < iSpp; i1++)
                                    qCompAccums[i1] += fContrib*(*((unsigned short*)(aucSrcPix + i1*2)));
                            }
                        }
                        else
                        {
                            // on an edge
                            for (int iSrcY = iInterpFromY; iSrcY < iInterpUpToY; iSrcY++)
                            {
                                fContrib = qColumnContribs[iC++];
                                if ( 0 <= iSrcY && iSrcY < iOldHeight )
                                    aucSrcPix = pucSrcBase + iSrcY*m_spqSrcImage->m_iRowStride;
                                else if ( iSrcY < 0 )
                                    aucSrcPix = pucSrcBase;
                                else
                                    aucSrcPix = pucSrcBase + iOldHeightMinusOne*m_spqSrcImage->m_iRowStride;
                                for (int i = 0; i < iSpp; i++)
                                    qCompAccums[i] += fContrib*(*((unsigned short*)(aucSrcPix + i*2)));
                            }
                        }

                        // assign
                        aucDstPix = pucDstBase + iDstY*spqNewImage->m_iRowStride;
                        for (int i = 0; i < iSpp; i++)
                        {
                            iValue = Mathf::RoundToInt(qCompAccums[i]);
                            if ( iValue < 0 )
                                iValue = 0;
                            else if ( iValue > 65535 )
                                iValue = 65535;
                            *((unsigned short*)(aucDstPix + i*2)) = iValue;
                        }
                    }

                    qProgress.MakeAdvance();
                }
            }
        }
        else
        {
            int iUseSrcY;
            bool bDoColorClamping = !m_spqSrcImage->m_bIsHighDynamicRange;
            if ( m_spqSrcImage->m_eColorModel == RasterImage::CMT_GRAYSCALE )
            {
                float fIntensityAccum, fIntensity;
                for (int iX = 0; iX < iNewWidth; iX++)
                {
                    // interpolate
                    iC = 0;
                    for (int iDstY = 0; iDstY < iNewHeight; iDstY++)
                    {
                        const Link<float,Link<int,int> >& rqPrecomp = qColumnPrecomp[iDstY];
                        fSrcPtY = rqPrecomp.First;
                        iInterpFromY = rqPrecomp.Second.First;
                        iInterpUpToY = rqPrecomp.Second.Second;

                        fIntensityAccum = 0.0f;
                        if ( iInterpFromY >= 0 && iInterpUpToY <= iOldHeight )
                        {
                            // in
                            for (int iSrcY = iInterpFromY; iSrcY < iInterpUpToY; iSrcY++)
                            {
                                fContrib = qColumnContribs[iC++];
                                fIntensity = m_spqSrcImage->GetPixelIntensity(iX,iSrcY);
                                fIntensityAccum += fIntensity*fContrib;
                            }
                        }
                        else
                        {
                            // on an edge
                            for (int iSrcY = iInterpFromY; iSrcY < iInterpUpToY; iSrcY++)
                            {
                                fContrib = qColumnContribs[iC++];
                                iUseSrcY = iSrcY;
                                if ( iUseSrcY < 0 )
                                    iUseSrcY = 0;
                                else if ( iUseSrcY > iOldHeightMinusOne )
                                    iUseSrcY = iOldHeightMinusOne;
                                fIntensity = m_spqSrcImage->GetPixelIntensity(iX,iUseSrcY);
                                fIntensityAccum += fIntensity*fContrib;
                            }
                        }

                        // assign
                        if ( bDoColorClamping )
                        {
                            if ( fIntensityAccum < 0.0f )
                                fIntensityAccum = 0.0f;
                            else if ( fIntensityAccum > 1.0f )
                                fIntensityAccum = 1.0f;
                        }
                        spqNewImage->SetPixelIntensity(fIntensityAccum,iX,iDstY);
                    }

                    qProgress.MakeAdvance();
                }
            }
            else if ( m_spqSrcImage->m_eColorModel == RasterImage::CMT_GRAYSCALE_WITH_ALPHA )
            {
                Link<float,float> qIntensityWithAlphaAccum, qIntensityWithAlpha;
                for (int iX = 0; iX < iNewWidth; iX++)
                {
                    // interpolate
                    iC = 0;
                    for (int iDstY = 0; iDstY < iNewHeight; iDstY++)
                    {
                        const Link<float,Link<int,int> >& rqPrecomp = qColumnPrecomp[iDstY];
                        fSrcPtY = rqPrecomp.First;
                        iInterpFromY = rqPrecomp.Second.First;
                        iInterpUpToY = rqPrecomp.Second.Second;

                        qIntensityWithAlphaAccum.First = 0.0f;
                        qIntensityWithAlphaAccum.Second = 0.0f;
                        if ( iInterpFromY >= 0 && iInterpUpToY <= iOldHeight )
                        {
                            // in
                            for (int iSrcY = iInterpFromY; iSrcY < iInterpUpToY; iSrcY++)
                            {
                                fContrib = qColumnContribs[iC++];
                                qIntensityWithAlpha = m_spqSrcImage->GetPixelIntensityWithAlpha(iX,iSrcY);
                                qIntensityWithAlphaAccum.First += qIntensityWithAlpha.First*fContrib;
                                qIntensityWithAlphaAccum.Second += qIntensityWithAlpha.Second*fContrib;
                            }
                        }
                        else
                        {
                            // on an edge
                            for (int iSrcY = iInterpFromY; iSrcY < iInterpUpToY; iSrcY++)
                            {
                                fContrib = qColumnContribs[iC++];
                                iUseSrcY = iSrcY;
                                if ( iUseSrcY < 0 )
                                    iUseSrcY = 0;
                                else if ( iUseSrcY > iOldHeightMinusOne )
                                    iUseSrcY = iOldHeightMinusOne;
                                qIntensityWithAlpha = m_spqSrcImage->GetPixelIntensityWithAlpha(iX,iUseSrcY);
                                qIntensityWithAlphaAccum.First += qIntensityWithAlpha.First*fContrib;
                                qIntensityWithAlphaAccum.Second += qIntensityWithAlpha.Second*fContrib;
                            }
                        }

                        // assign
                        if ( bDoColorClamping )
                        {
                            if ( qIntensityWithAlphaAccum.First < 0.0f )
                                qIntensityWithAlphaAccum.First = 0.0f;
                            else if ( qIntensityWithAlphaAccum.First > 1.0f )
                                qIntensityWithAlphaAccum.First = 1.0f;
                        }
                        if ( qIntensityWithAlphaAccum.Second < 0.0f )
                            qIntensityWithAlphaAccum.Second = 0.0f;
                        else if ( qIntensityWithAlphaAccum.Second > 1.0f )
                            qIntensityWithAlphaAccum.Second = 1.0f;
                        spqNewImage->SetPixelIntensityWithAlpha(qIntensityWithAlphaAccum,iX,iDstY);
                    }

                    qProgress.MakeAdvance();
                }
            }
            else if ( m_spqSrcImage->m_eColorModel == RasterImage::CMT_RGB )
            {
                ColorRgb qRgbAccum, qRgb;
                ColorRgb qRgbZero(0.0f,0.0f,0.0f);
                for (int iX = 0; iX < iNewWidth; iX++)
                {
                    // interpolate
                    iC = 0;
                    for (int iDstY = 0; iDstY < iNewHeight; iDstY++)
                    {
                        const Link<float,Link<int,int> >& rqPrecomp = qColumnPrecomp[iDstY];
                        fSrcPtY = rqPrecomp.First;
                        iInterpFromY = rqPrecomp.Second.First;
                        iInterpUpToY = rqPrecomp.Second.Second;

                        qRgbAccum = qRgbZero;
                        if ( iInterpFromY >= 0 && iInterpUpToY <= iOldHeight )
                        {
                            // in
                            for (int iSrcY = iInterpFromY; iSrcY < iInterpUpToY; iSrcY++)
                            {
                                fContrib = qColumnContribs[iC++];
                                qRgb = m_spqSrcImage->GetPixelRgb(iX,iSrcY);
                                qRgbAccum += qRgb*fContrib;
                            }
                        }
                        else
                        {
                            // on an edge
                            for (int iSrcY = iInterpFromY; iSrcY < iInterpUpToY; iSrcY++)
                            {
                                fContrib = qColumnContribs[iC++];
                                iUseSrcY = iSrcY;
                                if ( iUseSrcY < 0 )
                                    iUseSrcY = 0;
                                else if ( iUseSrcY > iOldHeightMinusOne )
                                    iUseSrcY = iOldHeightMinusOne;
                                qRgb = m_spqSrcImage->GetPixelRgb(iX,iUseSrcY);
                                qRgbAccum += qRgb*fContrib;
                            }
                        }

                        // assign
                        if ( bDoColorClamping )
                        {
                            for (int i = 0; i < 3; i++)
                            {
                                if ( qRgbAccum[i] < 0.0f )
                                    qRgbAccum[i] = 0.0f;
                                else if ( qRgbAccum[i] > 1.0f )
                                    qRgbAccum[i] = 1.0f;
                            }
                        }
                        spqNewImage->SetPixelRgb(qRgbAccum,iX,iDstY);
                    }

                    qProgress.MakeAdvance();
                }
            }
            else if ( m_spqSrcImage->m_eColorModel == RasterImage::CMT_RGBA )
            {
                ColorRgba qRgbaAccum, qRgba;
                ColorRgba qRgbaZero(0.0f,0.0f,0.0f,0.0f);
                for (int iX = 0; iX < iNewWidth; iX++)
                {
                    // interpolate
                    iC = 0;
                    for (int iDstY = 0; iDstY < iNewHeight; iDstY++)
                    {
                        const Link<float,Link<int,int> >& rqPrecomp = qColumnPrecomp[iDstY];
                        fSrcPtY = rqPrecomp.First;
                        iInterpFromY = rqPrecomp.Second.First;
                        iInterpUpToY = rqPrecomp.Second.Second;

                        qRgbaAccum = qRgbaZero;
                        if ( iInterpFromY >= 0 && iInterpUpToY <= iOldHeight )
                        {
                            // in
                            for (int iSrcY = iInterpFromY; iSrcY < iInterpUpToY; iSrcY++)
                            {
                                fContrib = qColumnContribs[iC++];
                                qRgba = m_spqSrcImage->GetPixelRgba(iX,iSrcY);
                                qRgbaAccum += qRgba*fContrib;
                            }
                        }
                        else
                        {
                            // on an edge
                            for (int iSrcY = iInterpFromY; iSrcY < iInterpUpToY; iSrcY++)
                            {
                                fContrib = qColumnContribs[iC++];
                                iUseSrcY = iSrcY;
                                if ( iUseSrcY < 0 )
                                    iUseSrcY = 0;
                                else if ( iUseSrcY > iOldHeightMinusOne )
                                    iUseSrcY = iOldHeightMinusOne;
                                qRgba = m_spqSrcImage->GetPixelRgba(iX,iUseSrcY);
                                qRgbaAccum += qRgba*fContrib;
                            }
                        }

                        // assign
                        if ( bDoColorClamping )
                        {
                            for (int i = 0; i < 3; i++)
                            {
                                if ( qRgbaAccum[i] < 0.0f )
                                    qRgbaAccum[i] = 0.0f;
                                else if ( qRgbaAccum[i] > 1.0f )
                                    qRgbaAccum[i] = 1.0f;
                            }
                        }
                        if ( qRgbaAccum[3] < 0.0f )
                            qRgbaAccum[3] = 0.0f;
                        else if ( qRgbaAccum[3] > 1.0f )
                            qRgbaAccum[3] = 1.0f;
                        spqNewImage->SetPixelRgba(qRgbaAccum,iX,iDstY);
                    }

                    qProgress.MakeAdvance();
                }
            }
            else if ( m_spqSrcImage->m_eColorModel == RasterImage::CMT_CMYK )
            {
                ColorCmyk qCmykAccum, qCmyk;
                ColorCmyk qCmykZero(0.0f,0.0f,0.0f,0.0f);
                for (int iX = 0; iX < iNewWidth; iX++)
                {
                    // interpolate
                    iC = 0;
                    for (int iDstY = 0; iDstY < iNewHeight; iDstY++)
                    {
                        const Link<float,Link<int,int> >& rqPrecomp = qColumnPrecomp[iDstY];
                        fSrcPtY = rqPrecomp.First;
                        iInterpFromY = rqPrecomp.Second.First;
                        iInterpUpToY = rqPrecomp.Second.Second;

                        qCmykAccum = qCmykZero;
                        if ( iInterpFromY >= 0 && iInterpUpToY <= iOldHeight )
                        {
                            // in
                            for (int iSrcY = iInterpFromY; iSrcY < iInterpUpToY; iSrcY++)
                            {
                                fContrib = qColumnContribs[iC++];
                                qCmyk = m_spqSrcImage->GetPixelCmyk(iX,iSrcY);
                                qCmykAccum += qCmyk*fContrib;
                            }
                        }
                        else
                        {
                            // on an edge
                            for (int iSrcY = iInterpFromY; iSrcY < iInterpUpToY; iSrcY++)
                            {
                                fContrib = qColumnContribs[iC++];
                                iUseSrcY = iSrcY;
                                if ( iUseSrcY < 0 )
                                    iUseSrcY = 0;
                                else if ( iUseSrcY > iOldHeightMinusOne )
                                    iUseSrcY = iOldHeightMinusOne;
                                qCmyk = m_spqSrcImage->GetPixelCmyk(iX,iUseSrcY);
                                qCmykAccum += qCmyk*fContrib;
                            }
                        }

                        // assign
                        if ( bDoColorClamping )
                        {
                            for (int i = 0; i < 4; i++)
                            {
                                if ( qCmykAccum[i] < 0.0f )
                                    qCmykAccum[i] = 0.0f;
                                else if ( qCmykAccum[i] > 1.0f )
                                    qCmykAccum[i] = 1.0f;
                            }
                        }
                        spqNewImage->SetPixelCmyk(qCmykAccum,iX,iDstY);
                    }

                    qProgress.MakeAdvance();
                }
            }
            else if ( m_spqSrcImage->m_eColorModel == RasterImage::CMT_CMYK_WITH_ALPHA )
            {
                ColorCmyka qCmykaAccum, qCmyka;
                ColorCmyka qCmykaZero(0.0f,0.0f,0.0f,0.0f,0.0f);
                for (int iX = 0; iX < iNewWidth; iX++)
                {
                    // interpolate
                    iC = 0;
                    for (int iDstY = 0; iDstY < iNewHeight; iDstY++)
                    {
                        const Link<float,Link<int,int> >& rqPrecomp = qColumnPrecomp[iDstY];
                        fSrcPtY = rqPrecomp.First;
                        iInterpFromY = rqPrecomp.Second.First;
                        iInterpUpToY = rqPrecomp.Second.Second;

                        qCmykaAccum = qCmykaZero;
                        if ( iInterpFromY >= 0 && iInterpUpToY <= iOldHeight )
                        {
                            // in
                            for (int iSrcY = iInterpFromY; iSrcY < iInterpUpToY; iSrcY++)
                            {
                                fContrib = qColumnContribs[iC++];
                                qCmyka = m_spqSrcImage->GetPixelCmyka(iX,iSrcY);
                                qCmykaAccum += qCmyka*fContrib;
                            }
                        }
                        else
                        {
                            // on an edge
                            for (int iSrcY = iInterpFromY; iSrcY < iInterpUpToY; iSrcY++)
                            {
                                fContrib = qColumnContribs[iC++];
                                iUseSrcY = iSrcY;
                                if ( iUseSrcY < 0 )
                                    iUseSrcY = 0;
                                else if ( iUseSrcY > iOldHeightMinusOne )
                                    iUseSrcY = iOldHeightMinusOne;
                                qCmyka = m_spqSrcImage->GetPixelCmyka(iX,iUseSrcY);
                                qCmykaAccum += qCmyka*fContrib;
                            }
                        }

                        // assign
                        if ( bDoColorClamping )
                        {
                            for (int i = 0; i < 4; i++)
                            {
                                if ( qCmykaAccum[i] < 0.0f )
                                    qCmykaAccum[i] = 0.0f;
                                else if ( qCmykaAccum[i] > 1.0f )
                                    qCmykaAccum[i] = 1.0f;
                            }
                        }
                        if ( qCmykaAccum[4] < 0.0f )
                            qCmykaAccum[4] = 0.0f;
                        else if ( qCmykaAccum[4] > 1.0f )
                            qCmykaAccum[4] = 1.0f;
                        spqNewImage->SetPixelCmyka(qCmykaAccum,iX,iDstY);
                    }

                    qProgress.MakeAdvance();
                }
            }
            else if ( m_spqSrcImage->m_eColorModel == RasterImage::CMT_LAB )
            {
                ColorLab qLabAccum, qLab;
                ColorLab qLabZero(0.0f,0.0f,0.0f);
                for (int iX = 0; iX < iNewWidth; iX++)
                {
                    // interpolate
                    iC = 0;
                    for (int iDstY = 0; iDstY < iNewHeight; iDstY++)
                    {
                        const Link<float,Link<int,int> >& rqPrecomp = qColumnPrecomp[iDstY];
                        fSrcPtY = rqPrecomp.First;
                        iInterpFromY = rqPrecomp.Second.First;
                        iInterpUpToY = rqPrecomp.Second.Second;

                        qLabAccum = qLabZero;
                        if ( iInterpFromY >= 0 && iInterpUpToY <= iOldHeight )
                        {
                            // in
                            for (int iSrcY = iInterpFromY; iSrcY < iInterpUpToY; iSrcY++)
                            {
                                fContrib = qColumnContribs[iC++];
                                qLab = m_spqSrcImage->GetPixelLab(iX,iSrcY);
                                qLabAccum += qLab*fContrib;
                            }
                        }
                        else
                        {
                            // on an edge
                            for (int iSrcY = iInterpFromY; iSrcY < iInterpUpToY; iSrcY++)
                            {
                                fContrib = qColumnContribs[iC++];
                                iUseSrcY = iSrcY;
                                if ( iUseSrcY < 0 )
                                    iUseSrcY = 0;
                                else if ( iUseSrcY > iOldHeightMinusOne )
                                    iUseSrcY = iOldHeightMinusOne;
                                qLab = m_spqSrcImage->GetPixelLab(iX,iUseSrcY);
                                qLabAccum += qLab*fContrib;
                            }
                        }

                        // assign
                        if ( bDoColorClamping )
                        {
                            if ( qLabAccum[0] < 0.0f )
                                qLabAccum[0] = 0.0f;
                            else if ( qLabAccum[0] > 100.0f )
                                qLabAccum[0] = 100.0f;
                        }
                        spqNewImage->SetPixelLab(qLabAccum,iX,iDstY);
                    }

                    qProgress.MakeAdvance();
                }
            }
            else  // m_spqSrcImage->m_eColorModel = RasterImage::CMT_LAB_WITH_ALPHA
            {
                ColorLaba qLabaAccum, qLaba;
                ColorLaba qLabaZero(0.0f,0.0f,0.0f,0.0f);
                for (int iX = 0; iX < iNewWidth; iX++)
                {
                    // interpolate
                    iC = 0;
                    for (int iDstY = 0; iDstY < iNewHeight; iDstY++)
                    {
                        const Link<float,Link<int,int> >& rqPrecomp = qColumnPrecomp[iDstY];
                        fSrcPtY = rqPrecomp.First;
                        iInterpFromY = rqPrecomp.Second.First;
                        iInterpUpToY = rqPrecomp.Second.Second;

                        qLabaAccum = qLabaZero;
                        if ( iInterpFromY >= 0 && iInterpUpToY <= iOldHeight )
                        {
                            // in
                            for (int iSrcY = iInterpFromY; iSrcY < iInterpUpToY; iSrcY++)
                            {
                                fContrib = qColumnContribs[iC++];
                                qLaba = m_spqSrcImage->GetPixelLaba(iX,iSrcY);
                                qLabaAccum += qLaba*fContrib;
                            }
                        }
                        else
                        {
                            // on an edge
                            for (int iSrcY = iInterpFromY; iSrcY < iInterpUpToY; iSrcY++)
                            {
                                fContrib = qColumnContribs[iC++];
                                iUseSrcY = iSrcY;
                                if ( iUseSrcY < 0 )
                                    iUseSrcY = 0;
                                else if ( iUseSrcY > iOldHeightMinusOne )
                                    iUseSrcY = iOldHeightMinusOne;
                                qLaba = m_spqSrcImage->GetPixelLaba(iX,iUseSrcY);
                                qLabaAccum += qLaba*fContrib;
                            }
                        }

                        // assign
                        if ( bDoColorClamping )
                        {
                            if ( qLabaAccum[0] < 0.0f )
                                qLabaAccum[0] = 0.0f;
                            else if ( qLabaAccum[0] > 100.0f )
                                qLabaAccum[0] = 100.0f;
                        }
                        if ( qLabaAccum[3] < 0.0f )
                            qLabaAccum[3] = 0.0f;
                        else if ( qLabaAccum[3] > 1.0f )
                            qLabaAccum[3] = 1.0f;
                        spqNewImage->SetPixelLaba(qLabaAccum,iX,iDstY);
                    }

                    qProgress.MakeAdvance();
                }
            }
        }
    }

End:
    if ( m_spqSrcImage->HasVisibilityMask() )
    {
        // treat the visibility mask as a grayscale image to be transformed
        TRANSFORM_VISIBILITY_MASK_AS_WELL(GetResampledVertically(iNewHeight,eInterpolation));
    }

    return spqNewImage;
}
//------------------------------------------------------------------------------------------------------------------
RasterImagePtr RasterImageTransformation::GetResampled (int iNewWidth, int iNewHeight,
    InterpolationType eInterpolation)
{
    assert( iNewWidth >= 0 && iNewHeight >= 0 );
    assert( !((m_spqSrcImage->m_eColorModel == RasterImage::CMT_PALETTED
        ||  m_spqSrcImage->m_eColorModel == RasterImage::CMT_PALETTED_WITH_ALPHA)
        &&  eInterpolation != IT_NEAREST_NEIGHBOR) );

    if ( iNewWidth == m_spqSrcImage->m_iWidth
    &&   iNewHeight == m_spqSrcImage->m_iHeight )
    {
        // nothing to do
        return m_spqSrcImage->GetDuplicate();
    }
    if ( m_spqSrcImage->m_iWidth == 0
    ||   m_spqSrcImage->m_iHeight == 0 )
    {
        // nothing to do
        return m_spqSrcImage->GetDuplicate();
    }

    // for a progress bar
    ProgressProbe qProgress(this);

    // resample
    RasterImagePtr spqNewImage;
    if ( eInterpolation == IT_NEAREST_NEIGHBOR )
    {
        // without interpolation
        qProgress.SetAdvance(1.0f/iNewHeight);

        if ( m_spqSrcImage->m_eColorModel == RasterImage::CMT_PALETTED
        ||   m_spqSrcImage->m_eColorModel == RasterImage::CMT_PALETTED_WITH_ALPHA )
        {
            spqNewImage = new RasterImage(m_spqSrcImage->m_eType,iNewWidth,iNewHeight,
                m_spqSrcImage->m_qPalette);
        }
        else
        {
            spqNewImage = new RasterImage(m_spqSrcImage->m_eType,iNewWidth,iNewHeight,
                m_spqSrcImage->m_bIsHighDynamicRange);
        }
        if ( iNewWidth == 0
        ||   iNewHeight == 0 )
        {
            // skip resampling
            goto End;
        }

        // prepare
        int iOldWidth = m_spqSrcImage->m_iWidth;
        int iOldWidthMinusOne = iOldWidth - 1;
        int iOldHeight = m_spqSrcImage->m_iHeight;
        int iOldHeightMinusOne = iOldHeight - 1;
        float fFactorH = ((float)iOldWidth)/iNewWidth;
        float fFactorV = ((float)iOldHeight)/iNewHeight;
        StillArray<int> qSrcRowShiftsX(iNewWidth);
        StillArray<int> qSrcColumnShiftsY(iNewHeight);
        int iSrcX, iSrcY;
        for (int iDstX = 0; iDstX < iNewWidth; iDstX++)
        {
            iSrcX = (int)(((float)iDstX + 0.5f)*fFactorH);
            if ( iSrcX < 0 )
                iSrcX = 0;
            else if ( iSrcX > iOldWidthMinusOne )
                iSrcX = iOldWidthMinusOne;
            qSrcRowShiftsX[iDstX] = iSrcX*m_spqSrcImage->m_iBytesPerPixel;
        }
        for (int iDstY = 0; iDstY < iNewHeight; iDstY++)
        {
            iSrcY = (int)(((float)iDstY + 0.5f)*fFactorV);
            if ( iSrcY < 0 )
                iSrcY = 0;
            else if ( iSrcY > iOldHeightMinusOne )
                iSrcY = iOldHeightMinusOne;
            qSrcColumnShiftsY[iDstY] = iSrcY*m_spqSrcImage->m_iRowStride;
        }

        // resample
        char* pcDstRow = spqNewImage->m_acImage;
        char* acDstPix = pcDstRow;
        char* pcSrcBase;
        char* acSrcPix;
        for (int iDstY = 0; iDstY < iNewHeight; iDstY++)
        {
            pcSrcBase = m_spqSrcImage->m_acImage + qSrcColumnShiftsY[iDstY];
            for (int iDstX = 0; iDstX < iNewWidth; iDstX++)
            {
                acSrcPix = pcSrcBase + qSrcRowShiftsX[iDstX];
                MemCpy(acDstPix,acSrcPix,m_spqSrcImage->m_iBytesPerPixel);

                acDstPix += spqNewImage->m_iBytesPerPixel;
            }

            pcDstRow += spqNewImage->m_iRowStride;
            acDstPix = pcDstRow;

            qProgress.MakeAdvance();
        }
    }
    else  // using interpolation
    {
        // first resample horizontally
        RasterImageTransformation qTrfm0(m_spqSrcImage);
        qTrfm0.RedirectProgressTo(this,qProgress,0.5f);
        spqNewImage = qTrfm0.GetResampledHorizontally(iNewWidth,eInterpolation);

        // then resample vertically
        RasterImageTransformation qTrfm1(spqNewImage);
        qTrfm1.RedirectProgressTo(this,qProgress,0.5f);
        spqNewImage = qTrfm1.GetResampledVertically(iNewHeight,eInterpolation);
    }

End:
    if ( m_spqSrcImage->HasVisibilityMask() )
    {
        // treat the visibility mask as a grayscale image to be transformed
        TRANSFORM_VISIBILITY_MASK_AS_WELL(GetResampled(iNewWidth,iNewHeight,eInterpolation));
    }

    return spqNewImage;
}
//------------------------------------------------------------------------------------------------------------------
RasterImagePtr RasterImageTransformation::GetRotated90Ccw () const
{
    // allocate destination image
    RasterImagePtr spqNewImage;
    if ( m_spqSrcImage->m_eColorModel == RasterImage::CMT_PALETTED
    ||   m_spqSrcImage->m_eColorModel == RasterImage::CMT_PALETTED_WITH_ALPHA )
    {
        spqNewImage = new RasterImage(m_spqSrcImage->m_eType,m_spqSrcImage->m_iHeight,
            m_spqSrcImage->m_iWidth,m_spqSrcImage->m_qPalette);
    }
    else
    {
        spqNewImage = new RasterImage(m_spqSrcImage->m_eType,m_spqSrcImage->m_iHeight,
            m_spqSrcImage->m_iWidth,m_spqSrcImage->m_bIsHighDynamicRange);
    }

    // rotate
    char* pcSrcRow = m_spqSrcImage->m_acImage;
    char* acSrcPix = pcSrcRow;
    char* pcDstRow = spqNewImage->m_acImage + (spqNewImage->m_iHeight - 1)*spqNewImage->m_iRowStride;
    char* acDstPix;
    for (int iY = 0; iY < m_spqSrcImage->m_iHeight; iY++)
    {
        acDstPix = pcDstRow + iY*spqNewImage->m_iBytesPerPixel;
        for (int iX = 0; iX < m_spqSrcImage->m_iWidth; iX++)
        {
            MemCpy(acDstPix,acSrcPix,m_spqSrcImage->m_iBytesPerPixel);

            acSrcPix += m_spqSrcImage->m_iBytesPerPixel;
            acDstPix -= spqNewImage->m_iRowStride;
        }

        pcSrcRow += m_spqSrcImage->m_iRowStride;
        acSrcPix = pcSrcRow;
    }

    if ( m_spqSrcImage->HasVisibilityMask() )
    {
        // also rotate the visibility mask
        spqNewImage->m_qVisibilityMask.SetQuantity(m_spqSrcImage->m_qVisibilityMask.GetQuantity());
        int iNIHMO = spqNewImage->m_iHeight - 1;
        for (int iY = 0; iY < m_spqSrcImage->m_iHeight; iY++)
        {
            for (int iX = 0; iX < m_spqSrcImage->m_iWidth; iX++)
            {
                spqNewImage->m_qVisibilityMask[(iNIHMO-iX)*spqNewImage->m_iWidth+iY] =
                    m_spqSrcImage->m_qVisibilityMask[iY*m_spqSrcImage->m_iWidth+iX];
            }
        }
    }

    return spqNewImage;
}
//------------------------------------------------------------------------------------------------------------------
RasterImagePtr RasterImageTransformation::GetRotated90Cw () const
{
    // allocate destination image
    RasterImagePtr spqNewImage;
    if ( m_spqSrcImage->m_eColorModel == RasterImage::CMT_PALETTED
    ||   m_spqSrcImage->m_eColorModel == RasterImage::CMT_PALETTED_WITH_ALPHA )
    {
        spqNewImage = new RasterImage(m_spqSrcImage->m_eType,m_spqSrcImage->m_iHeight,
            m_spqSrcImage->m_iWidth,m_spqSrcImage->m_qPalette);
    }
    else
    {
        spqNewImage = new RasterImage(m_spqSrcImage->m_eType,m_spqSrcImage->m_iHeight,
            m_spqSrcImage->m_iWidth,m_spqSrcImage->m_bIsHighDynamicRange);
    }

    // rotate
    char* pcSrcRow = m_spqSrcImage->m_acImage;
    char* acSrcPix = pcSrcRow;
    char* pcDstRow = spqNewImage->m_acImage;
    char* acDstPix;
    int iNIWMO = spqNewImage->m_iWidth - 1;
    for (int iY = 0; iY < m_spqSrcImage->m_iHeight; iY++)
    {
        acDstPix = pcDstRow + (iNIWMO - iY)*spqNewImage->m_iBytesPerPixel;
        for (int iX = 0; iX < m_spqSrcImage->m_iWidth; iX++)
        {
            MemCpy(acDstPix,acSrcPix,m_spqSrcImage->m_iBytesPerPixel);

            acSrcPix += m_spqSrcImage->m_iBytesPerPixel;
            acDstPix += spqNewImage->m_iRowStride;
        }

        pcSrcRow += m_spqSrcImage->m_iRowStride;
        acSrcPix = pcSrcRow;
    }

    if ( m_spqSrcImage->HasVisibilityMask() )
    {
        // also rotate the visibility mask
        spqNewImage->m_qVisibilityMask.SetQuantity(m_spqSrcImage->m_qVisibilityMask.GetQuantity());
        for (int iY = 0; iY < m_spqSrcImage->m_iHeight; iY++)
        {
            for (int iX = 0; iX < m_spqSrcImage->m_iWidth; iX++)
            {
                spqNewImage->m_qVisibilityMask[iX*spqNewImage->m_iWidth+iNIWMO-iY] =
                    m_spqSrcImage->m_qVisibilityMask[iY*m_spqSrcImage->m_iWidth+iX];
            }
        }
    }

    return spqNewImage;
}
//------------------------------------------------------------------------------------------------------------------
RasterImagePtr RasterImageTransformation::GetRotated180 () const
{
    // allocate destination image
    RasterImagePtr spqNewImage = m_spqSrcImage->GetDuplicate(false,false);

    // rotate
    char* pcSrcRow = m_spqSrcImage->m_acImage;
    char* pcDstRow = spqNewImage->m_acImage +
        (spqNewImage->m_iHeight - 1)*spqNewImage->m_iRowStride +
        (spqNewImage->m_iWidth - 1)*spqNewImage->m_iBytesPerPixel;
    char* acSrcPix;
    char* acDstPix;
    for (int iY = 0; iY < m_spqSrcImage->m_iHeight; iY++)
    {
        acSrcPix = pcSrcRow;
        acDstPix = pcDstRow;
        for (int iX = 0; iX < m_spqSrcImage->m_iWidth; iX++)
        {
            MemCpy(acDstPix,acSrcPix,m_spqSrcImage->m_iBytesPerPixel);

            acSrcPix += m_spqSrcImage->m_iBytesPerPixel;
            acDstPix -= spqNewImage->m_iBytesPerPixel;
        }

        pcSrcRow += m_spqSrcImage->m_iRowStride;
        pcDstRow -= spqNewImage->m_iRowStride;
    }

    if ( m_spqSrcImage->HasVisibilityMask() )
    {
        // also rotate the visibility mask
        acSrcPix = m_spqSrcImage->m_qVisibilityMask;
        acDstPix = spqNewImage->m_qVisibilityMask + spqNewImage->m_iArea - 1;
        for (int i = 0; i < m_spqSrcImage->m_iArea; i++)
            *acDstPix-- = *acSrcPix++;
    }

    return spqNewImage;
}
//------------------------------------------------------------------------------------------------------------------
RasterImagePtr RasterImageTransformation::GetRotated (float fAngle, InterpolationType eInterpolation)
{
    assert( -180.0f <= fAngle && fAngle <= 180.0f );
    assert( !((m_spqSrcImage->m_eColorModel == RasterImage::CMT_PALETTED
        ||  m_spqSrcImage->m_eColorModel == RasterImage::CMT_PALETTED_WITH_ALPHA)
        &&  eInterpolation != IT_NEAREST_NEIGHBOR) );

    if ( m_spqSrcImage->m_iWidth == 0
    ||   m_spqSrcImage->m_iHeight == 0 )
    {
        // nothing to do
        return m_spqSrcImage->GetDuplicate();
    }

    // for a progress bar
    ProgressProbe qProgress(this);

    // do actual rotation
    int iNewWidth, iNewHeight;
    RasterImagePtr spqNewImage;
    if ( eInterpolation == IT_NEAREST_NEIGHBOR )
    {
        // without interpolation
        qProgress.SetAdvance(1.0f/m_spqSrcImage->m_iHeight);

        // work the angle up
        float fAngleRad = fAngle*Mathf::DEG_TO_RAD;
        float fCos = Mathf::Cos(fAngleRad);
        float fSin = Mathf::Sin(fAngleRad);
        Matrix2f qRotMat(fCos,-fSin,fSin,fCos);

        // find out new dimensions
        Vector2f qRotShift;
        int iNormShiftX, iNormShiftY;
        qRotShift = Vector2f(fCos,fSin);
        Vector2f aqCornPt[4];
        aqCornPt[0] = Vector2f(0.5f,0.5f);
        aqCornPt[1] = aqCornPt[0];
        for (int i = 0; i < m_spqSrcImage->m_iWidth; i++)
            aqCornPt[1] += qRotShift;
        aqCornPt[2] = qRotMat*Vector2f(0.5f,(float)(m_spqSrcImage->m_iHeight-1)+0.5f);
        aqCornPt[3] = aqCornPt[2];
        for (int i = 0; i < m_spqSrcImage->m_iWidth; i++)
            aqCornPt[3] += qRotShift;
        float fRotImgMinX = aqCornPt[0].X();
        float fRotImgMaxX = aqCornPt[0].X();
        float fRotImgMinY = aqCornPt[0].Y();
        float fRotImgMaxY = aqCornPt[0].Y();
        for (int i = 1; i < 4; i++)
        {
            const Vector2f& rqPt = aqCornPt[i];

            if ( rqPt.X() < fRotImgMinX )
                fRotImgMinX = rqPt.X();
            else if ( rqPt.X() > fRotImgMaxX )
                fRotImgMaxX = rqPt.X();
            if ( rqPt.Y() < fRotImgMinY )
                fRotImgMinY = rqPt.Y();
            else if ( rqPt.Y() > fRotImgMaxY )
                fRotImgMaxY = rqPt.Y();
        }
        iNormShiftX = -(int)Mathf::Floor(fRotImgMinX);
        iNormShiftY = -(int)Mathf::Floor(fRotImgMinY);
        iNewWidth = (int)Mathf::Ceil(fRotImgMaxX) + iNormShiftX;
        iNewHeight = (int)Mathf::Ceil(fRotImgMaxY) + iNormShiftY;

        // allocate destination image
        if ( m_spqSrcImage->m_eColorModel == RasterImage::CMT_PALETTED
        ||   m_spqSrcImage->m_eColorModel == RasterImage::CMT_PALETTED_WITH_ALPHA )
        {
            spqNewImage = new RasterImage(m_spqSrcImage->m_eType,iNewWidth,iNewHeight,
                m_spqSrcImage->m_qPalette);
        }
        else
        {
            spqNewImage = new RasterImage(m_spqSrcImage->m_eType,iNewWidth,iNewHeight,
                m_spqSrcImage->m_bIsHighDynamicRange);
        }

        // image-mask is needed to track and fix pixel holes
        StillArray<char> qImgMaskChr;
        qImgMaskChr.SetQuantity(spqNewImage->m_iArea);
        for (int i = 0; i < spqNewImage->m_iArea; i++)
            qImgMaskChr[i] = 0;

        StillArray<unsigned char> qOCBytes;
        if ( !m_qOuterColor.Specified )
            MemSet(spqNewImage->m_acImage,0,iNewHeight*spqNewImage->m_iRowStride);
        else
            GetOuterColorAsBytes(qOCBytes);

        // rotate
        Vector2f qPixCntPnt;
        char* pcRow = m_spqSrcImage->m_acImage;
        char* pcPix = pcRow;
        int iRotX, iRotY;
        StillArray<int> qInRowMaxX(iNewHeight);
        for (int i = 0; i < iNewHeight; i++)
            qInRowMaxX[i] = Mathi::MIN_INT;
        for (int iY = 0; iY < m_spqSrcImage->m_iHeight; iY++)
        {
            qPixCntPnt = qRotMat*Vector2f(0.5f,(float)iY+0.5f);
            for (int iX = 0; iX < m_spqSrcImage->m_iWidth; iX++)
            {
                iRotX = (int)Mathf::Floor(qPixCntPnt.X()) + iNormShiftX;
                iRotY = (int)Mathf::Floor(qPixCntPnt.Y()) + iNormShiftY;
                MemCpy(spqNewImage->m_acImage+iRotY*spqNewImage->m_iRowStride+iRotX*m_spqSrcImage->m_iBytesPerPixel,
                    pcPix,m_spqSrcImage->m_iBytesPerPixel);
                qImgMaskChr[iRotY*iNewWidth+iRotX] = 1;
                if ( iRotX > qInRowMaxX[iRotY] )
                    qInRowMaxX[iRotY] = iRotX;
                qPixCntPnt += qRotShift;

                pcPix += m_spqSrcImage->m_iBytesPerPixel;
            }

            pcRow += m_spqSrcImage->m_iRowStride;
            pcPix = pcRow;

            qProgress.MakeAdvance();
        }

        // fix every pixel hole by replicating the first valid pixel from the left
        char* pcIMCell;
        char* pcLeftValidPix;
        pcRow = spqNewImage->m_acImage;
        for (iRotY = 0; iRotY < iNewHeight; iRotY++)
        {
            pcIMCell = qImgMaskChr + iRotY*iNewWidth;
            pcLeftValidPix = 0;
            for (iRotX = 0; iRotX < qInRowMaxX[iRotY]; iRotX++)
            {
                if ( *pcIMCell == 0 )
                {
                    if ( pcLeftValidPix )
                    {
                        MemCpy(pcRow+iRotX*spqNewImage->m_iBytesPerPixel,pcLeftValidPix,
                            spqNewImage->m_iBytesPerPixel);
                        *pcIMCell = 1;
                    }
                }
                else
                {
                    pcLeftValidPix = pcRow + iRotX*spqNewImage->m_iBytesPerPixel;
                }

                pcIMCell++;
            }

            pcRow += spqNewImage->m_iRowStride;
        }

        if ( m_qOuterColor.Specified )
        {
            // paint empty areas with the outer color
            pcIMCell = qImgMaskChr;
            pcRow = spqNewImage->m_acImage;
            for (iRotY = 0; iRotY < iNewHeight; iRotY++)
            {
                for (iRotX = 0; iRotX < iNewWidth; iRotX++)
                {
                    if ( *pcIMCell++ != 1 )
                        MemCpy(pcRow+iRotX*spqNewImage->m_iBytesPerPixel,qOCBytes,spqNewImage->m_iBytesPerPixel);
                }

                pcRow += spqNewImage->m_iRowStride;
            }
        }
    }
    else
    {
        // as skewing sub-operations to follow are based on the tangent function that gives too large numbers
        // (i.e. image dimensions) when module of the argument is close to 90, transform the angle to only
        // [-45, 45] and, if needed, correspondingly pre-rotate the source image
        float fUseAngle = fAngle;
        if ( fUseAngle < -45.0f )
        {
            if ( fUseAngle > -135.0f )
            {
                fUseAngle += 90.0f;
                RasterImageTransformation qTrfm(m_spqSrcImage);
                m_spqSrcImage = qTrfm.GetRotated90Ccw();
            }
            else
            {
                fUseAngle += 180.0f;
                RasterImageTransformation qTrfm(m_spqSrcImage);
                m_spqSrcImage = qTrfm.GetRotated180();
            }
        }
        else if ( fUseAngle > 45.0f )
        {
            if ( fUseAngle < 135.0f )
            {
                fUseAngle -= 90.0f;
                RasterImageTransformation qTrfm(m_spqSrcImage);
                m_spqSrcImage = qTrfm.GetRotated90Cw();
            }
            else
            {
                fUseAngle -= 180.0f;
                RasterImageTransformation qTrfm(m_spqSrcImage);
                m_spqSrcImage = qTrfm.GetRotated180();
            }
        }

        // rotate source image vertices in order to know necessary and unnecessary resulting image areas
        float fHalfWidth = 0.5f*m_spqSrcImage->m_iWidth;
        float fHalfHeight = 0.5f*m_spqSrcImage->m_iHeight;
        Vector2f aqImgVert[4] = {
            Vector2f(-fHalfWidth,-fHalfHeight),
            Vector2f(+fHalfWidth,-fHalfHeight),
            Vector2f(-fHalfWidth,+fHalfHeight),
            Vector2f(+fHalfWidth,+fHalfHeight)};
        Matrix2f qRotMat;
        qRotMat.FromAngle(fUseAngle*Mathf::DEG_TO_RAD);
        for (int i = 0; i < 4; i++)
            aqImgVert[i] = qRotMat*aqImgVert[i];
        float aafXYMinMax[2][2] = {
            {+Mathf::MAX_REAL, -Mathf::MAX_REAL},
            {+Mathf::MAX_REAL, -Mathf::MAX_REAL}};
        for (int i0 = 0; i0 < 4; i0++)
        {
            const Vector2f& rqVert = aqImgVert[i0];
            for (int i1 = 0; i1 < 2; i1++)
            {
                if ( rqVert[i1] < aafXYMinMax[i1][0] )
                    aafXYMinMax[i1][0] = rqVert[i1];
                if ( rqVert[i1] > aafXYMinMax[i1][1] )
                    aafXYMinMax[i1][1] = rqVert[i1];
            }
        }
        float fNewWidth = aafXYMinMax[0][1] - aafXYMinMax[0][0];
        float fNewHeight = aafXYMinMax[1][1] - aafXYMinMax[1][0];

        // skew-angles to be used
        float fSkewAngleH = -fUseAngle*0.5f;
        float fSkewAngleV = Mathf::ATan(Mathf::Sin(fUseAngle*Mathf::DEG_TO_RAD))*Mathf::RAD_TO_DEG;

        // skew horizontally for the first time
        RasterImageTransformation qTrfm0(m_spqSrcImage);
        qTrfm0.m_qOuterColor = m_qOuterColor;
        qTrfm0.RedirectProgressTo(this,qProgress,0.33f);
        spqNewImage = qTrfm0.GetSkewedHorizontally(fSkewAngleH,eInterpolation);

        // skew vertically
        RasterImageTransformation qTrfm1(spqNewImage);
        qTrfm1.m_qOuterColor = m_qOuterColor;
        qTrfm1.RedirectProgressTo(this,qProgress,0.33f);
        spqNewImage = qTrfm1.GetSkewedVertically(fSkewAngleV,eInterpolation);
        qTrfm1.m_spqSrcImage->Delete();
        float fCurrHeight = (float)spqNewImage->m_iHeight;
        int iFromY = Mathi::Max((int)Mathf::Floor((fCurrHeight-fNewHeight)*0.5f),0);
        int iUpToY = Mathi::Min((int)Mathf::Ceil((fCurrHeight+fNewHeight)*0.5f),spqNewImage->m_iHeight);
        spqNewImage = spqNewImage->GetSubimage(0,iFromY,spqNewImage->m_iWidth,iUpToY-iFromY);

        // and skew horizontally for the second time
        RasterImageTransformation qTrfm2(spqNewImage);
        qTrfm2.m_qOuterColor = m_qOuterColor;
        qTrfm2.RedirectProgressTo(this,qProgress,0.33f);
        spqNewImage = qTrfm2.GetSkewedHorizontally(fSkewAngleH,eInterpolation);
        qTrfm2.m_spqSrcImage->Delete();
        float fCurrWidth = (float)spqNewImage->m_iWidth;
        int iFromX = Mathi::Max((int)Mathf::Floor((fCurrWidth-fNewWidth)*0.5f),0);
        int iUpToX = Mathi::Min((int)Mathf::Ceil((fCurrWidth+fNewWidth)*0.5f),spqNewImage->m_iWidth);
        spqNewImage = spqNewImage->GetSubimage(iFromX,0,iUpToX-iFromX,spqNewImage->m_iHeight);
    }

    if ( m_spqSrcImage->HasVisibilityMask() )
    {
        // treat the visibility mask as a grayscale image to be transformed
        TRANSFORM_VISIBILITY_MASK_AS_WELL(GetRotated(fAngle,eInterpolation));
    }

    return spqNewImage;
}
//------------------------------------------------------------------------------------------------------------------
RasterImagePtr RasterImageTransformation::GetSkewedHorizontally (float fAngle, InterpolationType eInterpolation)
{
    assert( -180.0f <= fAngle && fAngle <= 180.0f );
    assert( !((m_spqSrcImage->m_eColorModel == RasterImage::CMT_PALETTED
        ||  m_spqSrcImage->m_eColorModel == RasterImage::CMT_PALETTED_WITH_ALPHA)
        &&  eInterpolation != IT_NEAREST_NEIGHBOR) );

    if ( m_spqSrcImage->m_iWidth == 0
    ||   m_spqSrcImage->m_iHeight == 0 )
    {
        // nothing to do
        return m_spqSrcImage->GetDuplicate();
    }

    // work the angle up
    static const float s_fMaxAbsAngle = 89.0f;
    static const int s_iMaxResWidth = 30000;
    float fUseAngle = fAngle;
    if ( fUseAngle < -90.0f )
        fUseAngle += 180.0f;
    else if ( fUseAngle > 90.0f )
        fUseAngle -= 180.0f;
    if ( fUseAngle < -s_fMaxAbsAngle )
        fUseAngle = -s_fMaxAbsAngle;
    else if ( fUseAngle > s_fMaxAbsAngle )
        fUseAngle = s_fMaxAbsAngle;
    float fSkewFactor = Mathf::Tan(fUseAngle*Mathf::DEG_TO_RAD);
    if ( Mathf::Ceil(Mathf::FAbs(fSkewFactor)*m_spqSrcImage->m_iHeight+m_spqSrcImage->m_iWidth) > s_iMaxResWidth )
    {
        // resulting image width seems to be too large, so replace the angle by the closest possible
        fUseAngle =
            Mathf::Sign(fUseAngle)*
            Mathf::ATan(((float)(s_iMaxResWidth-1-m_spqSrcImage->m_iWidth))/m_spqSrcImage->m_iHeight)*
            Mathf::RAD_TO_DEG;
        fSkewFactor = Mathf::Tan(fUseAngle*Mathf::DEG_TO_RAD);
    }

    // find out new width
    int iNewWidth;
    StillArray<int> qSkewedFrontX;  // to accompany IT_NEAREST_NEIGHBOR
    float fNormShiftX;              // to accompany interpolation filters
    if ( eInterpolation == IT_NEAREST_NEIGHBOR )
    {
        qSkewedFrontX.SetQuantity(m_spqSrcImage->m_iHeight);
        for (int iY = 0; iY < m_spqSrcImage->m_iHeight; iY++)
            qSkewedFrontX[iY] = Mathf::RoundToInt(fSkewFactor*(0.5f+iY));
        int iShift;
        if ( qSkewedFrontX.GetFirst() > qSkewedFrontX.GetLast() )
        {
            iShift = -qSkewedFrontX.GetLast();
            for (int i = 0; i < m_spqSrcImage->m_iHeight; i++)
                qSkewedFrontX[i] += iShift;
            iNewWidth = qSkewedFrontX.GetFirst() + m_spqSrcImage->m_iWidth;
        }
        else
        {
            iShift = -qSkewedFrontX.GetFirst();
            for (int i = 0; i < m_spqSrcImage->m_iHeight; i++)
                qSkewedFrontX[i] += iShift;
            iNewWidth = qSkewedFrontX.GetLast() + m_spqSrcImage->m_iWidth;
        }
    }
    else
    {
        float fDev = fSkewFactor*m_spqSrcImage->m_iHeight;
        float fNewWidth, fNWCeil;
        if ( fDev < 0.0f )
        {
            fNewWidth = -fDev + m_spqSrcImage->m_iWidth;
            fNWCeil = Mathf::Ceil(fNewWidth);
            fNormShiftX = -fDev;
        }
        else
        {
            fNewWidth = fDev + m_spqSrcImage->m_iWidth;
            fNWCeil = Mathf::Ceil(fNewWidth);
            fNormShiftX = 0.0f;
        }
        iNewWidth = (int)fNWCeil;
        fNormShiftX += (fNWCeil - fNewWidth)*0.5f;  // to make resulting image centered
    }
    int iNewHeight = m_spqSrcImage->m_iHeight;

    // allocate destination image
    RasterImagePtr spqNewImage;
    if ( m_spqSrcImage->m_eColorModel == RasterImage::CMT_PALETTED
    ||   m_spqSrcImage->m_eColorModel == RasterImage::CMT_PALETTED_WITH_ALPHA )
    {
        spqNewImage = new RasterImage(m_spqSrcImage->m_eType,iNewWidth,iNewHeight,
            m_spqSrcImage->m_qPalette);
    }
    else
    {
        spqNewImage = new RasterImage(m_spqSrcImage->m_eType,iNewWidth,iNewHeight,
            m_spqSrcImage->m_bIsHighDynamicRange);
    }

    // for a progress bar
    ProgressProbe qProgress(this);

    // do actual skewing
    if ( eInterpolation == IT_NEAREST_NEIGHBOR )
    {
        // without interpolation
        qProgress.SetAdvance(1.0f/m_spqSrcImage->m_iHeight);
        StillArray<char> qRowMask;  // to distinguish occupied row-areas from empty
        StillArray<unsigned char> qOCBytes;
        if ( !m_qOuterColor.Specified )
        {
            MemSet(spqNewImage->m_acImage,0,iNewHeight*spqNewImage->m_iRowStride);
        }
        else
        {
            qRowMask.SetQuantity(iNewWidth);
            GetOuterColorAsBytes(qOCBytes);
        }

        char* pcRow0 = m_spqSrcImage->m_acImage;
        char* pcPix0 = pcRow0;
        char* pcShRow1;
        char* pcPix1;
        int iPosX;
        for (int iY = 0; iY < m_spqSrcImage->m_iHeight; iY++)
        {
            if ( m_qOuterColor.Specified )
            {
                for (int i = 0; i < iNewWidth; i++)
                    qRowMask[i] = 0;
            }

            pcShRow1 = spqNewImage->m_acImage + iY*spqNewImage->m_iRowStride +
                qSkewedFrontX[iY]*spqNewImage->m_iBytesPerPixel;
            pcPix1 = pcShRow1;
            for (int iX = 0; iX < m_spqSrcImage->m_iWidth; iX++)
            {
                iPosX = qSkewedFrontX[iY] + iX;
                if ( 0 <= iPosX && iPosX < iNewWidth )
                {
                    MemCpy(pcPix1,pcPix0,m_spqSrcImage->m_iBytesPerPixel);
                    if ( m_qOuterColor.Specified )
                        qRowMask[iPosX] = 1;
                }

                pcPix0 += m_spqSrcImage->m_iBytesPerPixel;
                pcPix1 += spqNewImage->m_iBytesPerPixel;
            }

            pcRow0 += m_spqSrcImage->m_iRowStride;
            pcPix0 = pcRow0;

            if ( m_qOuterColor.Specified )
            {
                // paint empty areas with the outer color
                pcShRow1 = spqNewImage->m_acImage + iY*spqNewImage->m_iRowStride;
                for (int iX = 0; iX < iNewWidth; iX++)
                {
                    if ( qRowMask[iX] != 1 )
                        MemCpy(pcShRow1+iX*spqNewImage->m_iBytesPerPixel,qOCBytes,spqNewImage->m_iBytesPerPixel);
                }
            }

            qProgress.MakeAdvance();
        }
    }
    else
    {
        // using interpolation
        qProgress.SetAdvance(1.0f/iNewHeight);
        float fFilterRadius;
        float (*oEvaluateFilter)(float);
        SWITCH_FILTER(eInterpolation);
        int iFilterDiam = (int)(2.0f*fFilterRadius);

        float fDstPixCntY, fContribSumM, fContribSumE, fInvContribSumM, fInvContribSumE, fContrib, fAlphaPM,
            fOneMinusAlphaPM, fInvAlphaPM;
        double dShiftX0, dShiftX1, dSrcPtX;
        int iInterpFromX, iInterpUpToX;
        StillArray<float> qContribsMain(iFilterDiam);
        StillArray<float> qContribsEdge(iFilterDiam);
        if ( m_spqSrcImage->m_bHasGradIntSampleUniformity
        &&   (m_spqSrcImage->m_iBitsPerIntegerSample == 8 || m_spqSrcImage->m_iBitsPerIntegerSample == 16) )
        {
            int iValue, iSppMinusOne, iSpp = m_spqSrcImage->m_iComponentsPerPixel;
            if ( m_spqSrcImage->m_bHasAlpha )
                iSppMinusOne = iSpp - 1;
            StillArray<float> qCompAccums(iSpp);
            StillArray<unsigned char> qOCBytes;
            if ( m_qOuterColor.Specified )
                GetOuterColorAsBytes(qOCBytes);

            bool bGrayRgbCase = (
                m_spqSrcImage->m_eColorModel == RasterImage::CMT_GRAYSCALE
            ||  m_spqSrcImage->m_eColorModel == RasterImage::CMT_GRAYSCALE_WITH_ALPHA
            ||  m_spqSrcImage->m_eColorModel == RasterImage::CMT_RGB
            ||  m_spqSrcImage->m_eColorModel == RasterImage::CMT_RGBA );
            bool bCmykCase = (
                m_spqSrcImage->m_eColorModel == RasterImage::CMT_CMYK
            ||  m_spqSrcImage->m_eColorModel == RasterImage::CMT_CMYK_WITH_ALPHA );

            unsigned char* pucDstRow = (unsigned char*)spqNewImage->m_acImage;
            unsigned char* aucDstPix = pucDstRow;
            unsigned char* pucSrcRow = (unsigned char*)m_spqSrcImage->m_acImage;
            unsigned char* pucSrcComp;
            unsigned char* aucSrcPix;
            if ( m_spqSrcImage->m_iBitsPerIntegerSample == 8 )
            {
                if ( bGrayRgbCase )
                {
                    for (int iY = 0; iY < iNewHeight; iY++)
                    {
                        fDstPixCntY = (float)iY + 0.5f;

                        // prepare interpolation for the row
                        PREPARE_SKEW_INTERPOLATION_FOR_ROW;

                        // interpolate
                        for (int iDstX = 0; iDstX < iNewWidth; iDstX++)
                        {
                            dSrcPtX = dShiftX1 + iDstX;

                            iInterpFromX = Mathd::RoundToInt(dSrcPtX-fFilterRadius);
                            iInterpUpToX = iInterpFromX + iFilterDiam;
                            if ( iInterpFromX >= 0 && iInterpUpToX <= m_spqSrcImage->m_iWidth )
                            {
                                // in
                                for (int i = 0; i < iSpp; i++)
                                    qCompAccums[i] = 0.0f;
                                pucSrcComp = pucSrcRow + iInterpFromX*m_spqSrcImage->m_iBytesPerPixel;
                                for (int i0 = 0; i0 < iFilterDiam; i0++)
                                {
                                    fContrib = qContribsMain[i0];
                                    for (int i1 = 0; i1 < iSpp; i1++)
                                        qCompAccums[i1] += fContrib*(*pucSrcComp++);
                                }
                            }
                            else if ( iInterpUpToX <= 0 || iInterpFromX >= m_spqSrcImage->m_iWidth )
                            {
                                // out
                                if ( !m_qOuterColor.Specified )
                                {
                                    for (int i = 0; i < iSpp; i++)
                                        qCompAccums[i] = 0.0f;
                                }
                                else
                                {
                                    for (int i = 0; i < iSpp; i++)
                                        qCompAccums[i] = (float)qOCBytes[i];
                                }
                            }
                            else
                            {
                                // on an edge
                                for (int i = 0; i < iSpp; i++)
                                    qCompAccums[i] = 0.0f;
                                fAlphaPM = 0.0f;
                                for (int iSrcX = iInterpFromX, i0 = 0; iSrcX < iInterpUpToX; iSrcX++, i0++)
                                {
                                    if ( 0 <= iSrcX && iSrcX < m_spqSrcImage->m_iWidth )
                                    {
                                        fContrib = qContribsEdge[i0];
                                        aucSrcPix = pucSrcRow + iSrcX*m_spqSrcImage->m_iBytesPerPixel;
                                        for (int i1 = 0; i1 < iSpp; i1++)
                                            qCompAccums[i1] += fContrib*aucSrcPix[i1];

                                        fAlphaPM += fContrib;
                                    }
                                }

                                if ( !m_spqSrcImage->m_bHasAlpha )
                                {
                                    if ( m_qOuterColor.Specified )
                                    {
                                        fOneMinusAlphaPM = 1.0f - fAlphaPM;
                                        for (int i = 0; i < iSpp; i++)
                                            qCompAccums[i] += fOneMinusAlphaPM*qOCBytes[i];
                                    }
                                }
                                else
                                {
                                    if ( fAlphaPM > Mathf::ZERO_TOLERANCE )
                                    {
                                        fInvAlphaPM = 1.0f/fAlphaPM;
                                        for (int i = 0; i < iSppMinusOne; i++)
                                            qCompAccums[i] *= fInvAlphaPM;
                                    }
                                    else
                                    {
                                        for (int i = 0; i < iSppMinusOne; i++)
                                            qCompAccums[i] = (float)255;
                                    }
                                }
                            }

                            // assign
                            for (int i = 0; i < iSpp; i++)
                            {
                                iValue = Mathf::RoundToInt(qCompAccums[i]);
                                if ( iValue < 0 )
                                    iValue = 0;
                                else if ( iValue > 255 )
                                    iValue = 255;
                                aucDstPix[i] = iValue;
                            }

                            aucDstPix += spqNewImage->m_iBytesPerPixel;
                        }

                        pucDstRow += spqNewImage->m_iRowStride;
                        aucDstPix = pucDstRow;
                        pucSrcRow += m_spqSrcImage->m_iRowStride;

                        qProgress.MakeAdvance();
                    }
                }
                else if ( bCmykCase )
                {
                    for (int iY = 0; iY < iNewHeight; iY++)
                    {
                        fDstPixCntY = (float)iY + 0.5f;

                        // prepare interpolation for the row
                        PREPARE_SKEW_INTERPOLATION_FOR_ROW;

                        // interpolate
                        for (int iDstX = 0; iDstX < iNewWidth; iDstX++)
                        {
                            dSrcPtX = dShiftX1 + iDstX;

                            iInterpFromX = Mathd::RoundToInt(dSrcPtX-fFilterRadius);
                            iInterpUpToX = iInterpFromX + iFilterDiam;
                            if ( iInterpFromX >= 0 && iInterpUpToX <= m_spqSrcImage->m_iWidth )
                            {
                                // in
                                for (int i = 0; i < iSpp; i++)
                                    qCompAccums[i] = 0.0f;
                                pucSrcComp = pucSrcRow + iInterpFromX*m_spqSrcImage->m_iBytesPerPixel;
                                for (int i0 = 0; i0 < iFilterDiam; i0++)
                                {
                                    fContrib = qContribsMain[i0];
                                    for (int i1 = 0; i1 < iSpp; i1++)
                                        qCompAccums[i1] += fContrib*(*pucSrcComp++);
                                }
                            }
                            else if ( iInterpUpToX <= 0 || iInterpFromX >= m_spqSrcImage->m_iWidth )
                            {
                                // out
                                if ( !m_qOuterColor.Specified )
                                {
                                    for (int i = 0; i < iSpp; i++)
                                        qCompAccums[i] = 0.0f;
                                }
                                else
                                {
                                    for (int i = 0; i < iSpp; i++)
                                        qCompAccums[i] = (float)qOCBytes[i];
                                }
                            }
                            else
                            {
                                // on an edge
                                for (int i = 0; i < iSpp; i++)
                                    qCompAccums[i] = 0.0f;
                                fAlphaPM = 0.0f;
                                for (int iSrcX = iInterpFromX, i0 = 0; iSrcX < iInterpUpToX; iSrcX++, i0++)
                                {
                                    if ( 0 <= iSrcX && iSrcX < m_spqSrcImage->m_iWidth )
                                    {
                                        fContrib = qContribsEdge[i0];
                                        aucSrcPix = pucSrcRow + iSrcX*m_spqSrcImage->m_iBytesPerPixel;
                                        for (int i1 = 0; i1 < iSpp; i1++)
                                            qCompAccums[i1] += fContrib*aucSrcPix[i1];

                                        fAlphaPM += fContrib;
                                    }
                                }

                                if ( !m_spqSrcImage->m_bHasAlpha )
                                {
                                    if ( m_qOuterColor.Specified )
                                    {
                                        fOneMinusAlphaPM = 1.0f - fAlphaPM;
                                        for (int i = 0; i < iSpp; i++)
                                            qCompAccums[i] += fOneMinusAlphaPM*qOCBytes[i];
                                    }
                                }
                                else
                                {
                                    if ( fAlphaPM > Mathf::ZERO_TOLERANCE )
                                    {
                                        fInvAlphaPM = 1.0f/fAlphaPM;
                                        for (int i = 0; i < iSppMinusOne; i++)
                                            qCompAccums[i] *= fInvAlphaPM;
                                    }
                                    else
                                    {
                                        for (int i = 0; i < iSppMinusOne; i++)
                                            qCompAccums[i] = (float)0;
                                    }
                                }
                            }

                            // assign
                            for (int i = 0; i < iSpp; i++)
                            {
                                iValue = Mathf::RoundToInt(qCompAccums[i]);
                                if ( iValue < 0 )
                                    iValue = 0;
                                else if ( iValue > 255 )
                                    iValue = 255;
                                aucDstPix[i] = iValue;
                            }

                            aucDstPix += spqNewImage->m_iBytesPerPixel;
                        }

                        pucDstRow += spqNewImage->m_iRowStride;
                        aucDstPix = pucDstRow;
                        pucSrcRow += m_spqSrcImage->m_iRowStride;

                        qProgress.MakeAdvance();
                    }
                }
                else  // L*a*b* case
                {
                    for (int iY = 0; iY < iNewHeight; iY++)
                    {
                        fDstPixCntY = (float)iY + 0.5f;

                        // prepare interpolation for the row
                        PREPARE_SKEW_INTERPOLATION_FOR_ROW;

                        // interpolate
                        for (int iDstX = 0; iDstX < iNewWidth; iDstX++)
                        {
                            dSrcPtX = dShiftX1 + iDstX;

                            iInterpFromX = Mathd::RoundToInt(dSrcPtX-fFilterRadius);
                            iInterpUpToX = iInterpFromX + iFilterDiam;
                            if ( iInterpFromX >= 0 && iInterpUpToX <= m_spqSrcImage->m_iWidth )
                            {
                                // in
                                for (int i = 0; i < iSpp; i++)
                                    qCompAccums[i] = 0.0f;
                                pucSrcComp = pucSrcRow + iInterpFromX*m_spqSrcImage->m_iBytesPerPixel;
                                for (int i0 = 0; i0 < iFilterDiam; i0++)
                                {
                                    fContrib = qContribsMain[i0];
                                    for (int i1 = 0; i1 < iSpp; i1++)
                                        qCompAccums[i1] += fContrib*(*pucSrcComp++);
                                }
                            }
                            else if ( iInterpUpToX <= 0 || iInterpFromX >= m_spqSrcImage->m_iWidth )
                            {
                                // out
                                if ( !m_qOuterColor.Specified )
                                {
                                    for (int i = 0; i < iSpp; i++)
                                        qCompAccums[i] = ( i != 1 && i != 2 ? 0.0f : (float)128 );
                                }
                                else
                                {
                                    for (int i = 0; i < iSpp; i++)
                                        qCompAccums[i] = (float)qOCBytes[i];
                                }
                            }
                            else
                            {
                                // on an edge
                                for (int i = 0; i < iSpp; i++)
                                    qCompAccums[i] = 0.0f;
                                fAlphaPM = 0.0f;
                                for (int iSrcX = iInterpFromX, i0 = 0; iSrcX < iInterpUpToX; iSrcX++, i0++)
                                {
                                    if ( 0 <= iSrcX && iSrcX < m_spqSrcImage->m_iWidth )
                                    {
                                        fContrib = qContribsEdge[i0];
                                        aucSrcPix = pucSrcRow + iSrcX*m_spqSrcImage->m_iBytesPerPixel;
                                        for (int i1 = 0; i1 < iSpp; i1++)
                                        {
                                            qCompAccums[i1] += fContrib*
                                                ( i1 != 1 && i1 != 2 ? aucSrcPix[i1] : aucSrcPix[i1] - 128 );
                                        }

                                        fAlphaPM += fContrib;
                                    }
                                }

                                if ( !m_spqSrcImage->m_bHasAlpha )
                                {
                                    if ( m_qOuterColor.Specified )
                                    {
                                        fOneMinusAlphaPM = 1.0f - fAlphaPM;
                                        for (int i = 0; i < iSpp; i++)
                                        {
                                            qCompAccums[i] += fOneMinusAlphaPM*
                                                ( i != 1 && i != 2 ? qOCBytes[i] : qOCBytes[i] - 128 );
                                        }
                                    }
                                }
                                else
                                {
                                    if ( fAlphaPM > Mathf::ZERO_TOLERANCE )
                                    {
                                        fInvAlphaPM = 1.0f/fAlphaPM;
                                        for (int i = 0; i < iSppMinusOne; i++)
                                            qCompAccums[i] *= fInvAlphaPM;
                                    }
                                    else
                                    {
                                        qCompAccums[0] = (float)255;
                                        for (int i = 1; i < iSppMinusOne; i++)
                                            qCompAccums[i] = Mathf::Sign(qCompAccums[i])*128;
                                    }
                                }

                                for (int i = 1; i < 3; i++)
                                    qCompAccums[i] += 128;
                            }

                            // assign
                            for (int i = 0; i < iSpp; i++)
                            {
                                iValue = Mathf::RoundToInt(qCompAccums[i]);
                                if ( iValue < 0 )
                                    iValue = 0;
                                else if ( iValue > 255 )
                                    iValue = 255;
                                aucDstPix[i] = iValue;
                            }

                            aucDstPix += spqNewImage->m_iBytesPerPixel;
                        }

                        pucDstRow += spqNewImage->m_iRowStride;
                        aucDstPix = pucDstRow;
                        pucSrcRow += m_spqSrcImage->m_iRowStride;

                        qProgress.MakeAdvance();
                    }
                }
            }
            else  // m_spqSrcImage->m_iBitsPerIntegerSample = 16
            {
                if ( bGrayRgbCase )
                {
                    for (int iY = 0; iY < iNewHeight; iY++)
                    {
                        fDstPixCntY = (float)iY + 0.5f;

                        // prepare interpolation for the row
                        PREPARE_SKEW_INTERPOLATION_FOR_ROW;

                        // interpolate
                        for (int iDstX = 0; iDstX < iNewWidth; iDstX++)
                        {
                            dSrcPtX = dShiftX1 + iDstX;

                            iInterpFromX = Mathd::RoundToInt(dSrcPtX-fFilterRadius);
                            iInterpUpToX = iInterpFromX + iFilterDiam;
                            if ( iInterpFromX >= 0 && iInterpUpToX <= m_spqSrcImage->m_iWidth )
                            {
                                // in
                                for (int i = 0; i < iSpp; i++)
                                    qCompAccums[i] = 0.0f;
                                pucSrcComp = pucSrcRow + iInterpFromX*m_spqSrcImage->m_iBytesPerPixel;
                                for (int i0 = 0; i0 < iFilterDiam; i0++)
                                {
                                    fContrib = qContribsMain[i0];
                                    for (int i1 = 0; i1 < iSpp; i1++)
                                    {
                                        qCompAccums[i1] += fContrib*(*((unsigned short*)pucSrcComp));
                                        pucSrcComp += 2;
                                    }
                                }
                            }
                            else if ( iInterpUpToX <= 0 || iInterpFromX >= m_spqSrcImage->m_iWidth )
                            {
                                // out
                                if ( !m_qOuterColor.Specified )
                                {
                                    for (int i = 0; i < iSpp; i++)
                                        qCompAccums[i] = 0.0f;
                                }
                                else
                                {
                                    for (int i = 0; i < iSpp; i++)
                                        qCompAccums[i] = (float)(*((unsigned short*)(qOCBytes + i*2)));
                                }
                            }
                            else
                            {
                                // on an edge
                                for (int i = 0; i < iSpp; i++)
                                    qCompAccums[i] = 0.0f;
                                fAlphaPM = 0.0f;
                                for (int iSrcX = iInterpFromX, i0 = 0; iSrcX < iInterpUpToX; iSrcX++, i0++)
                                {
                                    if ( 0 <= iSrcX && iSrcX < m_spqSrcImage->m_iWidth )
                                    {
                                        fContrib = qContribsEdge[i0];
                                        aucSrcPix = pucSrcRow + iSrcX*m_spqSrcImage->m_iBytesPerPixel;
                                        for (int i1 = 0; i1 < iSpp; i1++)
                                            qCompAccums[i1] += fContrib*(*((unsigned short*)(aucSrcPix + i1*2)));

                                        fAlphaPM += fContrib;
                                    }
                                }

                                if ( !m_spqSrcImage->m_bHasAlpha )
                                {
                                    if ( m_qOuterColor.Specified )
                                    {
                                        fOneMinusAlphaPM = 1.0f - fAlphaPM;
                                        for (int i = 0; i < iSpp; i++)
                                        {
                                            qCompAccums[i] += fOneMinusAlphaPM*
                                                (*((unsigned short*)(qOCBytes + i*2)));
                                        }
                                    }
                                }
                                else
                                {
                                    if ( fAlphaPM > Mathf::ZERO_TOLERANCE )
                                    {
                                        fInvAlphaPM = 1.0f/fAlphaPM;
                                        for (int i = 0; i < iSppMinusOne; i++)
                                            qCompAccums[i] *= fInvAlphaPM;
                                    }
                                    else
                                    {
                                        for (int i = 0; i < iSppMinusOne; i++)
                                            qCompAccums[i] = (float)65535;
                                    }
                                }
                            }

                            // assign
                            for (int i = 0; i < iSpp; i++)
                            {
                                iValue = Mathf::RoundToInt(qCompAccums[i]);
                                if ( iValue < 0 )
                                    iValue = 0;
                                else if ( iValue > 65535 )
                                    iValue = 65535;
                                *((unsigned short*)(aucDstPix + i*2)) = iValue;
                            }

                            aucDstPix += spqNewImage->m_iBytesPerPixel;
                        }

                        pucDstRow += spqNewImage->m_iRowStride;
                        aucDstPix = pucDstRow;
                        pucSrcRow += m_spqSrcImage->m_iRowStride;

                        qProgress.MakeAdvance();
                    }
                }
                else if ( bCmykCase )
                {
                    for (int iY = 0; iY < iNewHeight; iY++)
                    {
                        fDstPixCntY = (float)iY + 0.5f;

                        // prepare interpolation for the row
                        PREPARE_SKEW_INTERPOLATION_FOR_ROW;

                        // interpolate
                        for (int iDstX = 0; iDstX < iNewWidth; iDstX++)
                        {
                            dSrcPtX = dShiftX1 + iDstX;

                            iInterpFromX = Mathd::RoundToInt(dSrcPtX-fFilterRadius);
                            iInterpUpToX = iInterpFromX + iFilterDiam;
                            if ( iInterpFromX >= 0 && iInterpUpToX <= m_spqSrcImage->m_iWidth )
                            {
                                // in
                                for (int i = 0; i < iSpp; i++)
                                    qCompAccums[i] = 0.0f;
                                pucSrcComp = pucSrcRow + iInterpFromX*m_spqSrcImage->m_iBytesPerPixel;
                                for (int i0 = 0; i0 < iFilterDiam; i0++)
                                {
                                    fContrib = qContribsMain[i0];
                                    for (int i1 = 0; i1 < iSpp; i1++)
                                    {
                                        qCompAccums[i1] += fContrib*(*((unsigned short*)pucSrcComp));
                                        pucSrcComp += 2;
                                    }
                                }
                            }
                            else if ( iInterpUpToX <= 0 || iInterpFromX >= m_spqSrcImage->m_iWidth )
                            {
                                // out
                                if ( !m_qOuterColor.Specified )
                                {
                                    for (int i = 0; i < iSpp; i++)
                                        qCompAccums[i] = 0.0f;
                                }
                                else
                                {
                                    for (int i = 0; i < iSpp; i++)
                                        qCompAccums[i] = (float)(*((unsigned short*)(qOCBytes + i*2)));
                                }
                            }
                            else
                            {
                                // on an edge
                                for (int i = 0; i < iSpp; i++)
                                    qCompAccums[i] = 0.0f;
                                fAlphaPM = 0.0f;
                                for (int iSrcX = iInterpFromX, i0 = 0; iSrcX < iInterpUpToX; iSrcX++, i0++)
                                {
                                    if ( 0 <= iSrcX && iSrcX < m_spqSrcImage->m_iWidth )
                                    {
                                        fContrib = qContribsEdge[i0];
                                        aucSrcPix = pucSrcRow + iSrcX*m_spqSrcImage->m_iBytesPerPixel;
                                        for (int i1 = 0; i1 < iSpp; i1++)
                                            qCompAccums[i1] += fContrib*(*((unsigned short*)(aucSrcPix + i1*2)));

                                        fAlphaPM += fContrib;
                                    }
                                }

                                if ( !m_spqSrcImage->m_bHasAlpha )
                                {
                                    if ( m_qOuterColor.Specified )
                                    {
                                        fOneMinusAlphaPM = 1.0f - fAlphaPM;
                                        for (int i = 0; i < iSpp; i++)
                                        {
                                            qCompAccums[i] += fOneMinusAlphaPM*
                                                (*((unsigned short*)(qOCBytes + i*2)));
                                        }
                                    }
                                }
                                else
                                {
                                    if ( fAlphaPM > Mathf::ZERO_TOLERANCE )
                                    {
                                        fInvAlphaPM = 1.0f/fAlphaPM;
                                        for (int i = 0; i < iSppMinusOne; i++)
                                            qCompAccums[i] *= fInvAlphaPM;
                                    }
                                    else
                                    {
                                        for (int i = 0; i < iSppMinusOne; i++)
                                            qCompAccums[i] = (float)0;
                                    }
                                }
                            }

                            // assign
                            for (int i = 0; i < iSpp; i++)
                            {
                                iValue = Mathf::RoundToInt(qCompAccums[i]);
                                if ( iValue < 0 )
                                    iValue = 0;
                                else if ( iValue > 65535 )
                                    iValue = 65535;
                                *((unsigned short*)(aucDstPix + i*2)) = iValue;
                            }

                            aucDstPix += spqNewImage->m_iBytesPerPixel;
                        }

                        pucDstRow += spqNewImage->m_iRowStride;
                        aucDstPix = pucDstRow;
                        pucSrcRow += m_spqSrcImage->m_iRowStride;

                        qProgress.MakeAdvance();
                    }
                }
                else  // L*a*b* case
                {
                    unsigned short usValue;
                    for (int iY = 0; iY < iNewHeight; iY++)
                    {
                        fDstPixCntY = (float)iY + 0.5f;

                        // prepare interpolation for the row
                        PREPARE_SKEW_INTERPOLATION_FOR_ROW;

                        // interpolate
                        for (int iDstX = 0; iDstX < iNewWidth; iDstX++)
                        {
                            dSrcPtX = dShiftX1 + iDstX;

                            iInterpFromX = Mathd::RoundToInt(dSrcPtX-fFilterRadius);
                            iInterpUpToX = iInterpFromX + iFilterDiam;
                            if ( iInterpFromX >= 0 && iInterpUpToX <= m_spqSrcImage->m_iWidth )
                            {
                                // in
                                for (int i = 0; i < iSpp; i++)
                                    qCompAccums[i] = 0.0f;
                                pucSrcComp = pucSrcRow + iInterpFromX*m_spqSrcImage->m_iBytesPerPixel;
                                for (int i0 = 0; i0 < iFilterDiam; i0++)
                                {
                                    fContrib = qContribsMain[i0];
                                    for (int i1 = 0; i1 < iSpp; i1++)
                                    {
                                        qCompAccums[i1] += fContrib*(*((unsigned short*)pucSrcComp));
                                        pucSrcComp += 2;
                                    }
                                }
                            }
                            else if ( iInterpUpToX <= 0 || iInterpFromX >= m_spqSrcImage->m_iWidth )
                            {
                                // out
                                if ( !m_qOuterColor.Specified )
                                {
                                    for (int i = 0; i < iSpp; i++)
                                        qCompAccums[i] = ( i != 1 && i != 2 ? 0.0f : (float)32768 );
                                }
                                else
                                {
                                    for (int i = 0; i < iSpp; i++)
                                        qCompAccums[i] = (float)(*((unsigned short*)(qOCBytes + i*2)));
                                }
                            }
                            else
                            {
                                // on an edge
                                for (int i = 0; i < iSpp; i++)
                                    qCompAccums[i] = 0.0f;
                                fAlphaPM = 0.0f;
                                for (int iSrcX = iInterpFromX, i0 = 0; iSrcX < iInterpUpToX; iSrcX++, i0++)
                                {
                                    if ( 0 <= iSrcX && iSrcX < m_spqSrcImage->m_iWidth )
                                    {
                                        fContrib = qContribsEdge[i0];
                                        aucSrcPix = pucSrcRow + iSrcX*m_spqSrcImage->m_iBytesPerPixel;
                                        for (int i1 = 0; i1 < iSpp; i1++)
                                        {
                                            usValue = *((unsigned short*)(aucSrcPix + i1*2));
                                            qCompAccums[i1] += fContrib*
                                                ( i1 != 1 && i1 != 2 ? usValue : usValue - 32768 );
                                        }

                                        fAlphaPM += fContrib;
                                    }
                                }

                                if ( !m_spqSrcImage->m_bHasAlpha )
                                {
                                    if ( m_qOuterColor.Specified )
                                    {
                                        fOneMinusAlphaPM = 1.0f - fAlphaPM;
                                        for (int i = 0; i < iSpp; i++)
                                        {
                                            usValue = *((unsigned short*)(qOCBytes + i*2));
                                            qCompAccums[i] += fOneMinusAlphaPM*
                                                ( i != 1 && i != 2 ? usValue : usValue - 32768 );
                                        }
                                    }
                                }
                                else
                                {
                                    if ( fAlphaPM > Mathf::ZERO_TOLERANCE )
                                    {
                                        fInvAlphaPM = 1.0f/fAlphaPM;
                                        for (int i = 0; i < iSppMinusOne; i++)
                                            qCompAccums[i] *= fInvAlphaPM;
                                    }
                                    else
                                    {
                                        qCompAccums[0] = (float)65535;
                                        for (int i = 1; i < iSppMinusOne; i++)
                                            qCompAccums[i] = Mathf::Sign(qCompAccums[i])*32768;
                                    }
                                }

                                for (int i = 1; i < 3; i++)
                                    qCompAccums[i] += 32768;
                            }

                            // assign
                            for (int i = 0; i < iSpp; i++)
                            {
                                iValue = Mathf::RoundToInt(qCompAccums[i]);
                                if ( iValue < 0 )
                                    iValue = 0;
                                else if ( iValue > 65535 )
                                    iValue = 65535;
                                *((unsigned short*)(aucDstPix + i*2)) = iValue;
                            }

                            aucDstPix += spqNewImage->m_iBytesPerPixel;
                        }

                        pucDstRow += spqNewImage->m_iRowStride;
                        aucDstPix = pucDstRow;
                        pucSrcRow += m_spqSrcImage->m_iRowStride;

                        qProgress.MakeAdvance();
                    }
                }
            }
        }
        else
        {
            bool bDoColorClamping = !m_spqSrcImage->m_bIsHighDynamicRange;
            if ( m_spqSrcImage->m_eColorModel == RasterImage::CMT_GRAYSCALE )
            {
                float fIntensityAccum, fIntensity;
                for (int iY = 0; iY < iNewHeight; iY++)
                {
                    fDstPixCntY = (float)iY + 0.5f;

                    // prepare interpolation for the row
                    PREPARE_SKEW_INTERPOLATION_FOR_ROW;

                    // interpolate
                    for (int iDstX = 0; iDstX < iNewWidth; iDstX++)
                    {
                        dSrcPtX = dShiftX1 + iDstX;

                        iInterpFromX = Mathd::RoundToInt(dSrcPtX-fFilterRadius);
                        iInterpUpToX = iInterpFromX + iFilterDiam;
                        if ( iInterpFromX >= 0 && iInterpUpToX <= m_spqSrcImage->m_iWidth )
                        {
                            // in
                            fIntensityAccum = 0.0f;
                            for (int iSrcX = iInterpFromX, i = 0; iSrcX < iInterpUpToX; iSrcX++, i++)
                            {
                                fContrib = qContribsMain[i];
                                fIntensity = m_spqSrcImage->GetPixelIntensity(iSrcX,iY);
                                fIntensityAccum += fIntensity*fContrib;
                            }
                        }
                        else if ( iInterpUpToX <= 0 || iInterpFromX >= m_spqSrcImage->m_iWidth )
                        {
                            // out
                            fIntensityAccum = ( !m_qOuterColor.Specified ? 0.0f : m_qOuterColor.Intensity );
                        }
                        else
                        {
                            // on an edge
                            fIntensityAccum = 0.0f;
                            fAlphaPM = 0.0f;
                            for (int iSrcX = iInterpFromX, i = 0; iSrcX < iInterpUpToX; iSrcX++, i++)
                            {
                                if ( 0 <= iSrcX && iSrcX < m_spqSrcImage->m_iWidth )
                                {
                                    fContrib = qContribsEdge[i];
                                    fIntensity = m_spqSrcImage->GetPixelIntensity(iSrcX,iY);
                                    fIntensityAccum += fIntensity*fContrib;

                                    fAlphaPM += fContrib;
                                }
                            }

                            if ( m_qOuterColor.Specified )
                            {
                                fOneMinusAlphaPM = 1.0f - fAlphaPM;
                                fIntensityAccum += m_qOuterColor.Intensity*fOneMinusAlphaPM;
                            }
                        }

                        // assign
                        if ( bDoColorClamping )
                        {
                            if ( fIntensityAccum < 0.0f )
                                fIntensityAccum = 0.0f;
                            else if ( fIntensityAccum > 1.0f )
                                fIntensityAccum = 1.0f;
                        }
                        spqNewImage->SetPixelIntensity(fIntensityAccum,iDstX,iY);
                    }

                    qProgress.MakeAdvance();
                }
            }
            else if ( m_spqSrcImage->m_eColorModel == RasterImage::CMT_GRAYSCALE_WITH_ALPHA )
            {
                Link<float,float> qIntensityWithAlphaAccum, qIntensityWithAlpha;
                for (int iY = 0; iY < iNewHeight; iY++)
                {
                    fDstPixCntY = (float)iY + 0.5f;

                    // prepare interpolation for the row
                    PREPARE_SKEW_INTERPOLATION_FOR_ROW;

                    // interpolate
                    for (int iDstX = 0; iDstX < iNewWidth; iDstX++)
                    {
                        dSrcPtX = dShiftX1 + iDstX;

                        iInterpFromX = Mathd::RoundToInt(dSrcPtX-fFilterRadius);
                        iInterpUpToX = iInterpFromX + iFilterDiam;
                        if ( iInterpFromX >= 0 && iInterpUpToX <= m_spqSrcImage->m_iWidth )
                        {
                            // in
                            qIntensityWithAlphaAccum.First = 0.0f;
                            qIntensityWithAlphaAccum.Second = 0.0f;
                            for (int iSrcX = iInterpFromX, i = 0; iSrcX < iInterpUpToX; iSrcX++, i++)
                            {
                                fContrib = qContribsMain[i];
                                qIntensityWithAlpha = m_spqSrcImage->GetPixelIntensityWithAlpha(iSrcX,iY);
                                qIntensityWithAlphaAccum.First += qIntensityWithAlpha.First*fContrib;
                                qIntensityWithAlphaAccum.Second += qIntensityWithAlpha.Second*fContrib;
                            }
                        }
                        else if ( iInterpUpToX <= 0 || iInterpFromX >= m_spqSrcImage->m_iWidth )
                        {
                            // out
                            qIntensityWithAlphaAccum.First =
                                ( !m_qOuterColor.Specified ? 0.0f : m_qOuterColor.Intensity );
                            qIntensityWithAlphaAccum.Second = 0.0f;
                        }
                        else
                        {
                            // on an edge
                            qIntensityWithAlphaAccum.First = 0.0f;
                            qIntensityWithAlphaAccum.Second = 0.0f;
                            fAlphaPM = 0.0f;
                            for (int iSrcX = iInterpFromX, i = 0; iSrcX < iInterpUpToX; iSrcX++, i++)
                            {
                                if ( 0 <= iSrcX && iSrcX < m_spqSrcImage->m_iWidth )
                                {
                                    fContrib = qContribsEdge[i];
                                    qIntensityWithAlpha = m_spqSrcImage->GetPixelIntensityWithAlpha(iSrcX,iY);
                                    qIntensityWithAlphaAccum.First += qIntensityWithAlpha.First*fContrib;
                                    qIntensityWithAlphaAccum.Second += qIntensityWithAlpha.Second*fContrib;

                                    fAlphaPM += fContrib;
                                }
                            }

                            if ( fAlphaPM > Mathf::ZERO_TOLERANCE )
                            {
                                fInvAlphaPM = 1.0f/fAlphaPM;
                                qIntensityWithAlphaAccum.First *= fInvAlphaPM;
                            }
                            else
                            {
                                qIntensityWithAlphaAccum.First = 1.0f;
                            }
                        }

                        // assign
                        if ( bDoColorClamping )
                        {
                            if ( qIntensityWithAlphaAccum.First < 0.0f )
                                qIntensityWithAlphaAccum.First = 0.0f;
                            else if ( qIntensityWithAlphaAccum.First > 1.0f )
                                qIntensityWithAlphaAccum.First = 1.0f;
                        }
                        if ( qIntensityWithAlphaAccum.Second < 0.0f )
                            qIntensityWithAlphaAccum.Second = 0.0f;
                        else if ( qIntensityWithAlphaAccum.Second > 1.0f )
                            qIntensityWithAlphaAccum.Second = 1.0f;
                        spqNewImage->SetPixelIntensityWithAlpha(qIntensityWithAlphaAccum,iDstX,iY);
                    }

                    qProgress.MakeAdvance();
                }
            }
            else if ( m_spqSrcImage->m_eColorModel == RasterImage::CMT_RGB )
            {
                ColorRgb qRgbAccum, qRgb;
                ColorRgb qRgbZero(0.0f,0.0f,0.0f);
                for (int iY = 0; iY < iNewHeight; iY++)
                {
                    fDstPixCntY = (float)iY + 0.5f;

                    // prepare interpolation for the row
                    PREPARE_SKEW_INTERPOLATION_FOR_ROW;

                    // interpolate
                    for (int iDstX = 0; iDstX < iNewWidth; iDstX++)
                    {
                        dSrcPtX = dShiftX1 + iDstX;

                        iInterpFromX = Mathd::RoundToInt(dSrcPtX-fFilterRadius);
                        iInterpUpToX = iInterpFromX + iFilterDiam;
                        if ( iInterpFromX >= 0 && iInterpUpToX <= m_spqSrcImage->m_iWidth )
                        {
                            // in
                            qRgbAccum = qRgbZero;
                            for (int iSrcX = iInterpFromX, i = 0; iSrcX < iInterpUpToX; iSrcX++, i++)
                            {
                                fContrib = qContribsMain[i];
                                qRgb = m_spqSrcImage->GetPixelRgb(iSrcX,iY);
                                qRgbAccum += qRgb*fContrib;
                            }
                        }
                        else if ( iInterpUpToX <= 0 || iInterpFromX >= m_spqSrcImage->m_iWidth )
                        {
                            // out
                            qRgbAccum = ( !m_qOuterColor.Specified ? qRgbZero : m_qOuterColor.RgbColor );
                        }
                        else
                        {
                            // on an edge
                            qRgbAccum = qRgbZero;
                            fAlphaPM = 0.0f;
                            for (int iSrcX = iInterpFromX, i = 0; iSrcX < iInterpUpToX; iSrcX++, i++)
                            {
                                if ( 0 <= iSrcX && iSrcX < m_spqSrcImage->m_iWidth )
                                {
                                    fContrib = qContribsEdge[i];
                                    qRgb = m_spqSrcImage->GetPixelRgb(iSrcX,iY);
                                    qRgbAccum += qRgb*fContrib;

                                    fAlphaPM += fContrib;
                                }
                            }

                            if ( m_qOuterColor.Specified )
                            {
                                fOneMinusAlphaPM = 1.0f - fAlphaPM;
                                qRgbAccum += m_qOuterColor.RgbColor*fOneMinusAlphaPM;
                            }
                        }

                        // assign
                        if ( bDoColorClamping )
                        {
                            for (int i = 0; i < 3; i++)
                            {
                                if ( qRgbAccum[i] < 0.0f )
                                    qRgbAccum[i] = 0.0f;
                                else if ( qRgbAccum[i] > 1.0f )
                                    qRgbAccum[i] = 1.0f;
                            }
                        }
                        spqNewImage->SetPixelRgb(qRgbAccum,iDstX,iY);
                    }

                    qProgress.MakeAdvance();
                }
            }
            else if ( m_spqSrcImage->m_eColorModel == RasterImage::CMT_RGBA )
            {
                ColorRgba qRgbaAccum, qRgba;
                ColorRgba qRgbaZero(0.0f,0.0f,0.0f,0.0f);
                for (int iY = 0; iY < iNewHeight; iY++)
                {
                    fDstPixCntY = (float)iY + 0.5f;

                    // prepare interpolation for the row
                    PREPARE_SKEW_INTERPOLATION_FOR_ROW;

                    // interpolate
                    for (int iDstX = 0; iDstX < iNewWidth; iDstX++)
                    {
                        dSrcPtX = dShiftX1 + iDstX;

                        iInterpFromX = Mathd::RoundToInt(dSrcPtX-fFilterRadius);
                        iInterpUpToX = iInterpFromX + iFilterDiam;
                        if ( iInterpFromX >= 0 && iInterpUpToX <= m_spqSrcImage->m_iWidth )
                        {
                            // in
                            qRgbaAccum = qRgbaZero;
                            for (int iSrcX = iInterpFromX, i = 0; iSrcX < iInterpUpToX; iSrcX++, i++)
                            {
                                fContrib = qContribsMain[i];
                                qRgba = m_spqSrcImage->GetPixelRgba(iSrcX,iY);
                                qRgbaAccum += qRgba*fContrib;
                            }
                        }
                        else if ( iInterpUpToX <= 0 || iInterpFromX >= m_spqSrcImage->m_iWidth )
                        {
                            // out
                            if ( !m_qOuterColor.Specified )
                            {
                                qRgbaAccum = qRgbaZero;
                            }
                            else
                            {
                                for (int i = 0; i < 3; i++)
                                    qRgbaAccum[i] = m_qOuterColor.RgbColor[i];
                                qRgbaAccum[3] = 0.0f;
                            }
                        }
                        else
                        {
                            // on an edge
                            qRgbaAccum = qRgbaZero;
                            fAlphaPM = 0.0f;
                            for (int iSrcX = iInterpFromX, i = 0; iSrcX < iInterpUpToX; iSrcX++, i++)
                            {
                                if ( 0 <= iSrcX && iSrcX < m_spqSrcImage->m_iWidth )
                                {
                                    fContrib = qContribsEdge[i];
                                    qRgba = m_spqSrcImage->GetPixelRgba(iSrcX,iY);
                                    qRgbaAccum += qRgba*fContrib;

                                    fAlphaPM += fContrib;
                                }
                            }

                            if ( fAlphaPM > Mathf::ZERO_TOLERANCE )
                            {
                                fInvAlphaPM = 1.0f/fAlphaPM;
                                for (int i = 0; i < 3; i++)
                                    qRgbaAccum[i] *= fInvAlphaPM;
                            }
                            else
                            {
                                for (int i = 0; i < 3; i++)
                                    qRgbaAccum[i] = 1.0f;
                            }
                        }

                        // assign
                        if ( bDoColorClamping )
                        {
                            for (int i = 0; i < 3; i++)
                            {
                                if ( qRgbaAccum[i] < 0.0f )
                                    qRgbaAccum[i] = 0.0f;
                                else if ( qRgbaAccum[i] > 1.0f )
                                    qRgbaAccum[i] = 1.0f;
                            }
                        }
                        if ( qRgbaAccum[3] < 0.0f )
                            qRgbaAccum[3] = 0.0f;
                        else if ( qRgbaAccum[3] > 1.0f )
                            qRgbaAccum[3] = 1.0f;
                        spqNewImage->SetPixelRgba(qRgbaAccum,iDstX,iY);
                    }

                    qProgress.MakeAdvance();
                }
            }
            else if ( m_spqSrcImage->m_eColorModel == RasterImage::CMT_CMYK )
            {
                ColorCmyk qCmykAccum, qCmyk;
                ColorCmyk qCmykZero(0.0f,0.0f,0.0f,0.0f);
                for (int iY = 0; iY < iNewHeight; iY++)
                {
                    fDstPixCntY = (float)iY + 0.5f;

                    // prepare interpolation for the row
                    PREPARE_SKEW_INTERPOLATION_FOR_ROW;

                    // interpolate
                    for (int iDstX = 0; iDstX < iNewWidth; iDstX++)
                    {
                        dSrcPtX = dShiftX1 + iDstX;

                        iInterpFromX = Mathd::RoundToInt(dSrcPtX-fFilterRadius);
                        iInterpUpToX = iInterpFromX + iFilterDiam;
                        if ( iInterpFromX >= 0 && iInterpUpToX <= m_spqSrcImage->m_iWidth )
                        {
                            // in
                            qCmykAccum = qCmykZero;
                            for (int iSrcX = iInterpFromX, i = 0; iSrcX < iInterpUpToX; iSrcX++, i++)
                            {
                                fContrib = qContribsMain[i];
                                qCmyk = m_spqSrcImage->GetPixelCmyk(iSrcX,iY);
                                qCmykAccum += qCmyk*fContrib;
                            }
                        }
                        else if ( iInterpUpToX <= 0 || iInterpFromX >= m_spqSrcImage->m_iWidth )
                        {
                            // out
                            qCmykAccum = ( !m_qOuterColor.Specified ? qCmykZero : m_qOuterColor.CmykColor );
                        }
                        else
                        {
                            // on an edge
                            qCmykAccum = qCmykZero;
                            fAlphaPM = 0.0f;
                            for (int iSrcX = iInterpFromX, i = 0; iSrcX < iInterpUpToX; iSrcX++, i++)
                            {
                                if ( 0 <= iSrcX && iSrcX < m_spqSrcImage->m_iWidth )
                                {
                                    fContrib = qContribsEdge[i];
                                    qCmyk = m_spqSrcImage->GetPixelCmyk(iSrcX,iY);
                                    qCmykAccum += qCmyk*fContrib;

                                    fAlphaPM += fContrib;
                                }
                            }

                            if ( m_qOuterColor.Specified )
                            {
                                fOneMinusAlphaPM = 1.0f - fAlphaPM;
                                qCmykAccum += m_qOuterColor.CmykColor*fOneMinusAlphaPM;
                            }
                        }

                        // assign
                        if ( bDoColorClamping )
                        {
                            for (int i = 0; i < 4; i++)
                            {
                                if ( qCmykAccum[i] < 0.0f )
                                    qCmykAccum[i] = 0.0f;
                                else if ( qCmykAccum[i] > 1.0f )
                                    qCmykAccum[i] = 1.0f;
                            }
                        }
                        spqNewImage->SetPixelCmyk(qCmykAccum,iDstX,iY);
                    }

                    qProgress.MakeAdvance();
                }
            }
            else if ( m_spqSrcImage->m_eColorModel == RasterImage::CMT_CMYK_WITH_ALPHA )
            {
                ColorCmyka qCmykaAccum, qCmyka;
                ColorCmyka qCmykaZero(0.0f,0.0f,0.0f,0.0f,0.0f);
                for (int iY = 0; iY < iNewHeight; iY++)
                {
                    fDstPixCntY = (float)iY + 0.5f;

                    // prepare interpolation for the row
                    PREPARE_SKEW_INTERPOLATION_FOR_ROW;

                    // interpolate
                    for (int iDstX = 0; iDstX < iNewWidth; iDstX++)
                    {
                        dSrcPtX = dShiftX1 + iDstX;

                        iInterpFromX = Mathd::RoundToInt(dSrcPtX-fFilterRadius);
                        iInterpUpToX = iInterpFromX + iFilterDiam;
                        if ( iInterpFromX >= 0 && iInterpUpToX <= m_spqSrcImage->m_iWidth )
                        {
                            // in
                            qCmykaAccum = qCmykaZero;
                            for (int iSrcX = iInterpFromX, i = 0; iSrcX < iInterpUpToX; iSrcX++, i++)
                            {
                                fContrib = qContribsMain[i];
                                qCmyka = m_spqSrcImage->GetPixelCmyka(iSrcX,iY);
                                qCmykaAccum += qCmyka*fContrib;
                            }
                        }
                        else if ( iInterpUpToX <= 0 || iInterpFromX >= m_spqSrcImage->m_iWidth )
                        {
                            // out
                            if ( !m_qOuterColor.Specified )
                            {
                                qCmykaAccum = qCmykaZero;
                            }
                            else
                            {
                                for (int i = 0; i < 4; i++)
                                    qCmykaAccum[i] = m_qOuterColor.CmykColor[i];
                                qCmykaAccum[4] = 0.0f;
                            }
                        }
                        else
                        {
                            // on an edge
                            qCmykaAccum = qCmykaZero;
                            fAlphaPM = 0.0f;
                            for (int iSrcX = iInterpFromX, i = 0; iSrcX < iInterpUpToX; iSrcX++, i++)
                            {
                                if ( 0 <= iSrcX && iSrcX < m_spqSrcImage->m_iWidth )
                                {
                                    fContrib = qContribsEdge[i];
                                    qCmyka = m_spqSrcImage->GetPixelCmyka(iSrcX,iY);
                                    qCmykaAccum += qCmyka*fContrib;

                                    fAlphaPM += fContrib;
                                }
                            }

                            if ( fAlphaPM > Mathf::ZERO_TOLERANCE )
                            {
                                fInvAlphaPM = 1.0f/fAlphaPM;
                                for (int i = 0; i < 4; i++)
                                    qCmykaAccum[i] *= fInvAlphaPM;
                            }
                            else
                            {
                                for (int i = 0; i < 4; i++)
                                    qCmykaAccum[i] = 0.0f;
                            }
                        }

                        // assign
                        if ( bDoColorClamping )
                        {
                            for (int i = 0; i < 4; i++)
                            {
                                if ( qCmykaAccum[i] < 0.0f )
                                    qCmykaAccum[i] = 0.0f;
                                else if ( qCmykaAccum[i] > 1.0f )
                                    qCmykaAccum[i] = 1.0f;
                            }
                        }
                        if ( qCmykaAccum[4] < 0.0f )
                            qCmykaAccum[4] = 0.0f;
                        else if ( qCmykaAccum[4] > 1.0f )
                            qCmykaAccum[4] = 1.0f;
                        spqNewImage->SetPixelCmyka(qCmykaAccum,iDstX,iY);
                    }

                    qProgress.MakeAdvance();
                }
            }
            else if ( m_spqSrcImage->m_eColorModel == RasterImage::CMT_LAB )
            {
                ColorLab qLabAccum, qLab;
                ColorLab qLabZero(0.0f,0.0f,0.0f);
                for (int iY = 0; iY < iNewHeight; iY++)
                {
                    fDstPixCntY = (float)iY + 0.5f;

                    // prepare interpolation for the row
                    PREPARE_SKEW_INTERPOLATION_FOR_ROW;

                    // interpolate
                    for (int iDstX = 0; iDstX < iNewWidth; iDstX++)
                    {
                        dSrcPtX = dShiftX1 + iDstX;

                        iInterpFromX = Mathd::RoundToInt(dSrcPtX-fFilterRadius);
                        iInterpUpToX = iInterpFromX + iFilterDiam;
                        if ( iInterpFromX >= 0 && iInterpUpToX <= m_spqSrcImage->m_iWidth )
                        {
                            // in
                            qLabAccum = qLabZero;
                            for (int iSrcX = iInterpFromX, i = 0; iSrcX < iInterpUpToX; iSrcX++, i++)
                            {
                                fContrib = qContribsMain[i];
                                qLab = m_spqSrcImage->GetPixelLab(iSrcX,iY);
                                qLabAccum += qLab*fContrib;
                            }
                        }
                        else if ( iInterpUpToX <= 0 || iInterpFromX >= m_spqSrcImage->m_iWidth )
                        {
                            // out
                            qLabAccum = ( !m_qOuterColor.Specified ? qLabZero : m_qOuterColor.LabColor );
                        }
                        else
                        {
                            // on an edge
                            qLabAccum = qLabZero;
                            fAlphaPM = 0.0f;
                            for (int iSrcX = iInterpFromX, i = 0; iSrcX < iInterpUpToX; iSrcX++, i++)
                            {
                                if ( 0 <= iSrcX && iSrcX < m_spqSrcImage->m_iWidth )
                                {
                                    fContrib = qContribsEdge[i];
                                    qLab = m_spqSrcImage->GetPixelLab(iSrcX,iY);
                                    qLabAccum += qLab*fContrib;

                                    fAlphaPM += fContrib;
                                }
                            }

                            if ( m_qOuterColor.Specified )
                            {
                                fOneMinusAlphaPM = 1.0f - fAlphaPM;
                                qLabAccum += m_qOuterColor.LabColor*fOneMinusAlphaPM;
                            }
                        }

                        // assign
                        if ( bDoColorClamping )
                        {
                            if ( qLabAccum[0] < 0.0f )
                                qLabAccum[0] = 0.0f;
                            else if ( qLabAccum[0] > 100.0f )
                                qLabAccum[0] = 100.0f;
                        }
                        spqNewImage->SetPixelLab(qLabAccum,iDstX,iY);
                    }

                    qProgress.MakeAdvance();
                }
            }
            else  // m_spqSrcImage->m_eColorModel = RasterImage::CMT_LAB_WITH_ALPHA
            {
                ColorLaba qLabaAccum, qLaba;
                ColorLaba qLabaZero(0.0f,0.0f,0.0f,0.0f);
                for (int iY = 0; iY < iNewHeight; iY++)
                {
                    fDstPixCntY = (float)iY + 0.5f;

                    // prepare interpolation for the row
                    PREPARE_SKEW_INTERPOLATION_FOR_ROW;

                    // interpolate
                    for (int iDstX = 0; iDstX < iNewWidth; iDstX++)
                    {
                        dSrcPtX = dShiftX1 + iDstX;

                        iInterpFromX = Mathd::RoundToInt(dSrcPtX-fFilterRadius);
                        iInterpUpToX = iInterpFromX + iFilterDiam;
                        if ( iInterpFromX >= 0 && iInterpUpToX <= m_spqSrcImage->m_iWidth )
                        {
                            // in
                            qLabaAccum = qLabaZero;
                            for (int iSrcX = iInterpFromX, i = 0; iSrcX < iInterpUpToX; iSrcX++, i++)
                            {
                                fContrib = qContribsMain[i];
                                qLaba = m_spqSrcImage->GetPixelLaba(iSrcX,iY);
                                qLabaAccum += qLaba*fContrib;
                            }
                        }
                        else if ( iInterpUpToX <= 0 || iInterpFromX >= m_spqSrcImage->m_iWidth )
                        {
                            // out
                            if ( !m_qOuterColor.Specified )
                            {
                                qLabaAccum = qLabaZero;
                            }
                            else
                            {
                                for (int i = 0; i < 3; i++)
                                    qLabaAccum[i] = m_qOuterColor.LabColor[i];
                                qLabaAccum[3] = 0.0f;
                            }
                        }
                        else
                        {
                            // on an edge
                            qLabaAccum = qLabaZero;
                            fAlphaPM = 0.0f;
                            for (int iSrcX = iInterpFromX, i = 0; iSrcX < iInterpUpToX; iSrcX++, i++)
                            {
                                if ( 0 <= iSrcX && iSrcX < m_spqSrcImage->m_iWidth )
                                {
                                    fContrib = qContribsEdge[i];
                                    qLaba = m_spqSrcImage->GetPixelLaba(iSrcX,iY);
                                    qLabaAccum += qLaba*fContrib;

                                    fAlphaPM += fContrib;
                                }
                            }

                            if ( fAlphaPM > Mathf::ZERO_TOLERANCE )
                            {
                                fInvAlphaPM = 1.0f/fAlphaPM;
                                for (int i = 0; i < 3; i++)
                                    qLabaAccum[i] *= fInvAlphaPM;
                            }
                            else
                            {
                                qLabaAccum[0] = 100.0f;
                                for (int i = 1; i < 3; i++)
                                    qLabaAccum[i] = Mathf::Sign(qLabaAccum[i])*128.0f;
                            }
                        }

                        // assign
                        if ( bDoColorClamping )
                        {
                            if ( qLabaAccum[0] < 0.0f )
                                qLabaAccum[0] = 0.0f;
                            else if ( qLabaAccum[0] > 100.0f )
                                qLabaAccum[0] = 100.0f;
                        }
                        if ( qLabaAccum[3] < 0.0f )
                            qLabaAccum[3] = 0.0f;
                        else if ( qLabaAccum[3] > 1.0f )
                            qLabaAccum[3] = 1.0f;
                        spqNewImage->SetPixelLaba(qLabaAccum,iDstX,iY);
                    }

                    qProgress.MakeAdvance();
                }
            }
        }
    }

    if ( m_spqSrcImage->HasVisibilityMask() )
    {
        // treat the visibility mask as a grayscale image to be transformed
        TRANSFORM_VISIBILITY_MASK_AS_WELL(GetSkewedHorizontally(fAngle,eInterpolation));
    }

    return spqNewImage;
}
//------------------------------------------------------------------------------------------------------------------
RasterImagePtr RasterImageTransformation::GetSkewedVertically (float fAngle, InterpolationType eInterpolation)
{
    assert( -180.0f <= fAngle && fAngle <= 180.0f );
    assert( !((m_spqSrcImage->m_eColorModel == RasterImage::CMT_PALETTED
        ||  m_spqSrcImage->m_eColorModel == RasterImage::CMT_PALETTED_WITH_ALPHA)
        &&  eInterpolation != IT_NEAREST_NEIGHBOR) );

    if ( m_spqSrcImage->m_iWidth == 0
    ||   m_spqSrcImage->m_iHeight == 0 )
    {
        // nothing to do
        return m_spqSrcImage->GetDuplicate();
    }

    // work the angle up
    static const float s_fMaxAbsAngle = 89.0f;
    static const int s_iMaxResHeight = 30000;
    float fUseAngle = fAngle;
    if ( fUseAngle < -90.0f )
        fUseAngle += 180.0f;
    else if ( fUseAngle > 90.0f )
        fUseAngle -= 180.0f;
    if ( fUseAngle < -s_fMaxAbsAngle )
        fUseAngle = -s_fMaxAbsAngle;
    else if ( fUseAngle > s_fMaxAbsAngle )
        fUseAngle = s_fMaxAbsAngle;
    float fSkewFactor = Mathf::Tan(fUseAngle*Mathf::DEG_TO_RAD);
    if ( Mathf::Ceil(Mathf::FAbs(fSkewFactor)*m_spqSrcImage->m_iWidth+m_spqSrcImage->m_iHeight) > s_iMaxResHeight )
    {
        // resulting image height seems to be too large, so replace the angle by the closest possible
        fUseAngle =
            Mathf::Sign(fUseAngle)*
            Mathf::ATan(((float)(s_iMaxResHeight-1-m_spqSrcImage->m_iHeight))/m_spqSrcImage->m_iWidth)*
            Mathf::RAD_TO_DEG;
        fSkewFactor = Mathf::Tan(fUseAngle*Mathf::DEG_TO_RAD);
    }

    // find out new height
    int iNewHeight;
    StillArray<int> qSkewedFrontY;  // to accompany IT_NEAREST_NEIGHBOR
    float fNormShiftY;              // to accompany interpolation filters
    if ( eInterpolation == IT_NEAREST_NEIGHBOR )
    {
        qSkewedFrontY.SetQuantity(m_spqSrcImage->m_iWidth);
        for (int iX = 0; iX < m_spqSrcImage->m_iWidth; iX++)
            qSkewedFrontY[iX] = Mathf::RoundToInt(fSkewFactor*(0.5f+iX));
        int iShift;
        if ( qSkewedFrontY.GetFirst() > qSkewedFrontY.GetLast() )
        {
            iShift = -qSkewedFrontY.GetLast();
            for (int i = 0; i < m_spqSrcImage->m_iWidth; i++)
                qSkewedFrontY[i] += iShift;
            iNewHeight = qSkewedFrontY.GetFirst() + m_spqSrcImage->m_iHeight;
        }
        else
        {
            iShift = -qSkewedFrontY.GetFirst();
            for (int i = 0; i < m_spqSrcImage->m_iWidth; i++)
                qSkewedFrontY[i] += iShift;
            iNewHeight = qSkewedFrontY.GetLast() + m_spqSrcImage->m_iHeight;
        }
    }
    else
    {
        float fDev = fSkewFactor*m_spqSrcImage->m_iWidth;
        float fNewHeight, fNHCeil;
        if ( fDev < 0.0f )
        {
            fNewHeight = -fDev + m_spqSrcImage->m_iHeight;
            fNHCeil = Mathf::Ceil(fNewHeight);
            fNormShiftY = -fDev;
        }
        else
        {
            fNewHeight = fDev + m_spqSrcImage->m_iHeight;
            fNHCeil = Mathf::Ceil(fNewHeight);
            fNormShiftY = 0.0f;
        }
        iNewHeight = (int)fNHCeil;
        fNormShiftY += (fNHCeil - fNewHeight)*0.5f;  // to make resulting image centered
    }
    int iNewWidth = m_spqSrcImage->m_iWidth;

    // allocate destination image
    RasterImagePtr spqNewImage;
    if ( m_spqSrcImage->m_eColorModel == RasterImage::CMT_PALETTED
    ||   m_spqSrcImage->m_eColorModel == RasterImage::CMT_PALETTED_WITH_ALPHA )
    {
        spqNewImage = new RasterImage(m_spqSrcImage->m_eType,iNewWidth,iNewHeight,
            m_spqSrcImage->m_qPalette);
    }
    else
    {
        spqNewImage = new RasterImage(m_spqSrcImage->m_eType,iNewWidth,iNewHeight,
            m_spqSrcImage->m_bIsHighDynamicRange);
    }

    // for a progress bar
    ProgressProbe qProgress(this);

    // do actual skewing
    if ( eInterpolation == IT_NEAREST_NEIGHBOR )
    {
        // without interpolation
        qProgress.SetAdvance(1.0f/m_spqSrcImage->m_iWidth);
        StillArray<char> qColumnMask;  // to distinguish occupied column-areas from empty
        StillArray<unsigned char> qOCBytes;
        if ( !m_qOuterColor.Specified )
        {
            MemSet(spqNewImage->m_acImage,0,iNewHeight*spqNewImage->m_iRowStride);
        }
        else
        {
            qColumnMask.SetQuantity(iNewHeight);
            GetOuterColorAsBytes(qOCBytes);
        }

        char* pcBase0;
        char* pcBase1;
        char* pcPix0;
        char* pcPix1;
        int iPosY;
        for (int iX = 0; iX < m_spqSrcImage->m_iWidth; iX++)
        {
            if ( m_qOuterColor.Specified )
            {
                for (int i = 0; i < iNewHeight; i++)
                    qColumnMask[i] = 0;
            }

            pcBase0 = m_spqSrcImage->m_acImage + iX*m_spqSrcImage->m_iBytesPerPixel;
            pcBase1 = spqNewImage->m_acImage + iX*spqNewImage->m_iBytesPerPixel;
            for (int iY = 0; iY < m_spqSrcImage->m_iHeight; iY++)
            {
                iPosY = qSkewedFrontY[iX] + iY;
                if ( 0 <= iPosY && iPosY < iNewHeight )
                {
                    pcPix0 = pcBase0 + iY*m_spqSrcImage->m_iRowStride;
                    pcPix1 = pcBase1 + iPosY*spqNewImage->m_iRowStride;
                    MemCpy(pcPix1,pcPix0,m_spqSrcImage->m_iBytesPerPixel);
                    if ( m_qOuterColor.Specified )
                        qColumnMask[iPosY] = 1;
                }
            }

            if ( m_qOuterColor.Specified )
            {
                // paint empty areas with the outer color
                for (int iY = 0; iY < iNewHeight; iY++)
                {
                    if ( qColumnMask[iY] != 1 )
                        MemCpy(pcBase1+iY*spqNewImage->m_iRowStride,qOCBytes,spqNewImage->m_iBytesPerPixel);
                }
            }

            qProgress.MakeAdvance();
        }
    }
    else
    {
        // using interpolation
        qProgress.SetAdvance(1.0f/iNewWidth);
        float fFilterRadius;
        float (*oEvaluateFilter)(float);
        SWITCH_FILTER(eInterpolation);
        int iFilterDiam = (int)(2.0f*fFilterRadius);

        float fDstPixCntX, fContribSumM, fContribSumE, fInvContribSumM, fInvContribSumE, fContrib, fAlphaPM,
            fOneMinusAlphaPM, fInvAlphaPM;
        double dShiftY0, dShiftY1, dSrcPtY;
        int iInterpFromY, iInterpUpToY;
        StillArray<float> qContribsMain(iFilterDiam);
        StillArray<float> qContribsEdge(iFilterDiam);
        if ( m_spqSrcImage->m_bHasGradIntSampleUniformity
        &&   (m_spqSrcImage->m_iBitsPerIntegerSample == 8 || m_spqSrcImage->m_iBitsPerIntegerSample == 16) )
        {
            int iValue, iSppMinusOne, iSpp = m_spqSrcImage->m_iComponentsPerPixel;
            if ( m_spqSrcImage->m_bHasAlpha )
                iSppMinusOne = iSpp - 1;
            StillArray<float> qCompAccums(iSpp);
            StillArray<unsigned char> qOCBytes;
            if ( m_qOuterColor.Specified )
                GetOuterColorAsBytes(qOCBytes);

            bool bGrayRgbCase = (
                m_spqSrcImage->m_eColorModel == RasterImage::CMT_GRAYSCALE
            ||  m_spqSrcImage->m_eColorModel == RasterImage::CMT_GRAYSCALE_WITH_ALPHA
            ||  m_spqSrcImage->m_eColorModel == RasterImage::CMT_RGB
            ||  m_spqSrcImage->m_eColorModel == RasterImage::CMT_RGBA );
            bool bCmykCase = (
                m_spqSrcImage->m_eColorModel == RasterImage::CMT_CMYK
            ||  m_spqSrcImage->m_eColorModel == RasterImage::CMT_CMYK_WITH_ALPHA );

            unsigned char* pucSrcBase;
            unsigned char* pucDstBase;
            unsigned char* aucSrcPix;
            unsigned char* aucDstPix;
            if ( m_spqSrcImage->m_iBitsPerIntegerSample == 8 )
            {
                if ( bGrayRgbCase )
                {
                    for (int iX = 0; iX < iNewWidth; iX++)
                    {
                        fDstPixCntX = (float)iX + 0.5f;

                        // prepare interpolation for the column
                        PREPARE_SKEW_INTERPOLATION_FOR_COLUMN;

                        // interpolate
                        pucSrcBase = (unsigned char*)(m_spqSrcImage->m_acImage +
                            iX*m_spqSrcImage->m_iBytesPerPixel);
                        pucDstBase = (unsigned char*)(spqNewImage->m_acImage + iX*spqNewImage->m_iBytesPerPixel);
                        for (int iDstY = 0; iDstY < iNewHeight; iDstY++)
                        {
                            dSrcPtY = dShiftY1 + iDstY;

                            iInterpFromY = Mathd::RoundToInt(dSrcPtY-fFilterRadius);
                            iInterpUpToY = iInterpFromY + iFilterDiam;
                            if ( iInterpFromY >= 0 && iInterpUpToY <= m_spqSrcImage->m_iHeight )
                            {
                                // in
                                for (int i = 0; i < iSpp; i++)
                                    qCompAccums[i] = 0.0f;
                                for (int iSrcY = iInterpFromY, i0 = 0; iSrcY < iInterpUpToY; iSrcY++, i0++)
                                {
                                    fContrib = qContribsMain[i0];
                                    aucSrcPix = pucSrcBase + iSrcY*m_spqSrcImage->m_iRowStride;
                                    for (int i1 = 0; i1 < iSpp; i1++)
                                        qCompAccums[i1] += fContrib*aucSrcPix[i1];
                                }
                            }
                            else if ( iInterpUpToY <= 0 || iInterpFromY >= m_spqSrcImage->m_iHeight )
                            {
                                // out
                                if ( !m_qOuterColor.Specified )
                                {
                                    for (int i = 0; i < iSpp; i++)
                                        qCompAccums[i] = 0.0f;
                                }
                                else
                                {
                                    for (int i = 0; i < iSpp; i++)
                                        qCompAccums[i] = (float)qOCBytes[i];
                                }
                            }
                            else
                            {
                                // on an edge
                                for (int i = 0; i < iSpp; i++)
                                    qCompAccums[i] = 0.0f;
                                fAlphaPM = 0.0f;
                                for (int iSrcY = iInterpFromY, i0 = 0; iSrcY < iInterpUpToY; iSrcY++, i0++)
                                {
                                    if ( 0 <= iSrcY && iSrcY < m_spqSrcImage->m_iHeight )
                                    {
                                        fContrib = qContribsEdge[i0];
                                        aucSrcPix = pucSrcBase + iSrcY*m_spqSrcImage->m_iRowStride;
                                        for (int i1 = 0; i1 < iSpp; i1++)
                                            qCompAccums[i1] += fContrib*aucSrcPix[i1];

                                        fAlphaPM += fContrib;
                                    }
                                }

                                if ( !m_spqSrcImage->m_bHasAlpha )
                                {
                                    if ( m_qOuterColor.Specified )
                                    {
                                        fOneMinusAlphaPM = 1.0f - fAlphaPM;
                                        for (int i = 0; i < iSpp; i++)
                                            qCompAccums[i] += fOneMinusAlphaPM*qOCBytes[i];
                                    }
                                }
                                else
                                {
                                    if ( fAlphaPM > Mathf::ZERO_TOLERANCE )
                                    {
                                        fInvAlphaPM = 1.0f/fAlphaPM;
                                        for (int i = 0; i < iSppMinusOne; i++)
                                            qCompAccums[i] *= fInvAlphaPM;
                                    }
                                    else
                                    {
                                        for (int i = 0; i < iSppMinusOne; i++)
                                            qCompAccums[i] = (float)255;
                                    }
                                }
                            }

                            // assign
                            aucDstPix = pucDstBase + iDstY*spqNewImage->m_iRowStride;
                            for (int i = 0; i < iSpp; i++)
                            {
                                iValue = Mathf::RoundToInt(qCompAccums[i]);
                                if ( iValue < 0 )
                                    iValue = 0;
                                else if ( iValue > 255 )
                                    iValue = 255;
                                aucDstPix[i] = iValue;
                            }
                        }

                        qProgress.MakeAdvance();
                    }
                }
                else if ( bCmykCase )
                {
                    for (int iX = 0; iX < iNewWidth; iX++)
                    {
                        fDstPixCntX = (float)iX + 0.5f;

                        // prepare interpolation for the column
                        PREPARE_SKEW_INTERPOLATION_FOR_COLUMN;

                        // interpolate
                        pucSrcBase = (unsigned char*)(m_spqSrcImage->m_acImage +
                            iX*m_spqSrcImage->m_iBytesPerPixel);
                        pucDstBase = (unsigned char*)(spqNewImage->m_acImage + iX*spqNewImage->m_iBytesPerPixel);
                        for (int iDstY = 0; iDstY < iNewHeight; iDstY++)
                        {
                            dSrcPtY = dShiftY1 + iDstY;

                            iInterpFromY = Mathd::RoundToInt(dSrcPtY-fFilterRadius);
                            iInterpUpToY = iInterpFromY + iFilterDiam;
                            if ( iInterpFromY >= 0 && iInterpUpToY <= m_spqSrcImage->m_iHeight )
                            {
                                // in
                                for (int i = 0; i < iSpp; i++)
                                    qCompAccums[i] = 0.0f;
                                for (int iSrcY = iInterpFromY, i0 = 0; iSrcY < iInterpUpToY; iSrcY++, i0++)
                                {
                                    fContrib = qContribsMain[i0];
                                    aucSrcPix = pucSrcBase + iSrcY*m_spqSrcImage->m_iRowStride;
                                    for (int i1 = 0; i1 < iSpp; i1++)
                                        qCompAccums[i1] += fContrib*aucSrcPix[i1];
                                }
                            }
                            else if ( iInterpUpToY <= 0 || iInterpFromY >= m_spqSrcImage->m_iHeight )
                            {
                                // out
                                if ( !m_qOuterColor.Specified )
                                {
                                    for (int i = 0; i < iSpp; i++)
                                        qCompAccums[i] = 0.0f;
                                }
                                else
                                {
                                    for (int i = 0; i < iSpp; i++)
                                        qCompAccums[i] = (float)qOCBytes[i];
                                }
                            }
                            else
                            {
                                // on an edge
                                for (int i = 0; i < iSpp; i++)
                                    qCompAccums[i] = 0.0f;
                                fAlphaPM = 0.0f;
                                for (int iSrcY = iInterpFromY, i0 = 0; iSrcY < iInterpUpToY; iSrcY++, i0++)
                                {
                                    if ( 0 <= iSrcY && iSrcY < m_spqSrcImage->m_iHeight )
                                    {
                                        fContrib = qContribsEdge[i0];
                                        aucSrcPix = pucSrcBase + iSrcY*m_spqSrcImage->m_iRowStride;
                                        for (int i1 = 0; i1 < iSpp; i1++)
                                            qCompAccums[i1] += fContrib*aucSrcPix[i1];

                                        fAlphaPM += fContrib;
                                    }
                                }

                                if ( !m_spqSrcImage->m_bHasAlpha )
                                {
                                    if ( m_qOuterColor.Specified )
                                    {
                                        fOneMinusAlphaPM = 1.0f - fAlphaPM;
                                        for (int i = 0; i < iSpp; i++)
                                            qCompAccums[i] += fOneMinusAlphaPM*qOCBytes[i];
                                    }
                                }
                                else
                                {
                                    if ( fAlphaPM > Mathf::ZERO_TOLERANCE )
                                    {
                                        fInvAlphaPM = 1.0f/fAlphaPM;
                                        for (int i = 0; i < iSppMinusOne; i++)
                                            qCompAccums[i] *= fInvAlphaPM;
                                    }
                                    else
                                    {
                                        for (int i = 0; i < iSppMinusOne; i++)
                                            qCompAccums[i] = (float)0;
                                    }
                                }
                            }

                            // assign
                            aucDstPix = pucDstBase + iDstY*spqNewImage->m_iRowStride;
                            for (int i = 0; i < iSpp; i++)
                            {
                                iValue = Mathf::RoundToInt(qCompAccums[i]);
                                if ( iValue < 0 )
                                    iValue = 0;
                                else if ( iValue > 255 )
                                    iValue = 255;
                                aucDstPix[i] = iValue;
                            }
                        }

                        qProgress.MakeAdvance();
                    }
                }
                else  // L*a*b* case
                {
                    for (int iX = 0; iX < iNewWidth; iX++)
                    {
                        fDstPixCntX = (float)iX + 0.5f;

                        // prepare interpolation for the column
                        PREPARE_SKEW_INTERPOLATION_FOR_COLUMN;

                        // interpolate
                        pucSrcBase = (unsigned char*)(m_spqSrcImage->m_acImage +
                            iX*m_spqSrcImage->m_iBytesPerPixel);
                        pucDstBase = (unsigned char*)(spqNewImage->m_acImage + iX*spqNewImage->m_iBytesPerPixel);
                        for (int iDstY = 0; iDstY < iNewHeight; iDstY++)
                        {
                            dSrcPtY = dShiftY1 + iDstY;

                            iInterpFromY = Mathd::RoundToInt(dSrcPtY-fFilterRadius);
                            iInterpUpToY = iInterpFromY + iFilterDiam;
                            if ( iInterpFromY >= 0 && iInterpUpToY <= m_spqSrcImage->m_iHeight )
                            {
                                // in
                                for (int i = 0; i < iSpp; i++)
                                    qCompAccums[i] = 0.0f;
                                for (int iSrcY = iInterpFromY, i0 = 0; iSrcY < iInterpUpToY; iSrcY++, i0++)
                                {
                                    fContrib = qContribsMain[i0];
                                    aucSrcPix = pucSrcBase + iSrcY*m_spqSrcImage->m_iRowStride;
                                    for (int i1 = 0; i1 < iSpp; i1++)
                                        qCompAccums[i1] += fContrib*aucSrcPix[i1];
                                }
                            }
                            else if ( iInterpUpToY <= 0 || iInterpFromY >= m_spqSrcImage->m_iHeight )
                            {
                                // out
                                if ( !m_qOuterColor.Specified )
                                {
                                    for (int i = 0; i < iSpp; i++)
                                        qCompAccums[i] = ( i != 1 && i != 2 ? 0.0f : (float)128 );
                                }
                                else
                                {
                                    for (int i = 0; i < iSpp; i++)
                                        qCompAccums[i] = (float)qOCBytes[i];
                                }
                            }
                            else
                            {
                                // on an edge
                                for (int i = 0; i < iSpp; i++)
                                    qCompAccums[i] = 0.0f;
                                fAlphaPM = 0.0f;
                                for (int iSrcY = iInterpFromY, i0 = 0; iSrcY < iInterpUpToY; iSrcY++, i0++)
                                {
                                    if ( 0 <= iSrcY && iSrcY < m_spqSrcImage->m_iHeight )
                                    {
                                        fContrib = qContribsEdge[i0];
                                        aucSrcPix = pucSrcBase + iSrcY*m_spqSrcImage->m_iRowStride;
                                        for (int i1 = 0; i1 < iSpp; i1++)
                                        {
                                            qCompAccums[i1] += fContrib*
                                                ( i1 != 1 && i1 != 2 ? aucSrcPix[i1] : aucSrcPix[i1] - 128 );
                                        }

                                        fAlphaPM += fContrib;
                                    }
                                }

                                if ( !m_spqSrcImage->m_bHasAlpha )
                                {
                                    if ( m_qOuterColor.Specified )
                                    {
                                        fOneMinusAlphaPM = 1.0f - fAlphaPM;
                                        for (int i = 0; i < iSpp; i++)
                                        {
                                            qCompAccums[i] += fOneMinusAlphaPM*
                                                ( i != 1 && i != 2 ? qOCBytes[i] : qOCBytes[i] - 128 );
                                        }
                                    }
                                }
                                else
                                {
                                    if ( fAlphaPM > Mathf::ZERO_TOLERANCE )
                                    {
                                        fInvAlphaPM = 1.0f/fAlphaPM;
                                        for (int i = 0; i < iSppMinusOne; i++)
                                            qCompAccums[i] *= fInvAlphaPM;
                                    }
                                    else
                                    {
                                        qCompAccums[0] = (float)255;
                                        for (int i = 1; i < iSppMinusOne; i++)
                                            qCompAccums[i] = Mathf::Sign(qCompAccums[i])*128;
                                    }
                                }

                                for (int i = 1; i < 3; i++)
                                    qCompAccums[i] += 128;
                            }

                            // assign
                            aucDstPix = pucDstBase + iDstY*spqNewImage->m_iRowStride;
                            for (int i = 0; i < iSpp; i++)
                            {
                                iValue = Mathf::RoundToInt(qCompAccums[i]);
                                if ( iValue < 0 )
                                    iValue = 0;
                                else if ( iValue > 255 )
                                    iValue = 255;
                                aucDstPix[i] = iValue;
                            }
                        }

                        qProgress.MakeAdvance();
                    }
                }
            }
            else  // m_spqSrcImage->m_iBitsPerIntegerSample = 16
            {
                if ( bGrayRgbCase )
                {
                    for (int iX = 0; iX < iNewWidth; iX++)
                    {
                        fDstPixCntX = (float)iX + 0.5f;

                        // prepare interpolation for the column
                        PREPARE_SKEW_INTERPOLATION_FOR_COLUMN;

                        // interpolate
                        pucSrcBase = (unsigned char*)(m_spqSrcImage->m_acImage +
                            iX*m_spqSrcImage->m_iBytesPerPixel);
                        pucDstBase = (unsigned char*)(spqNewImage->m_acImage + iX*spqNewImage->m_iBytesPerPixel);
                        for (int iDstY = 0; iDstY < iNewHeight; iDstY++)
                        {
                            dSrcPtY = dShiftY1 + iDstY;

                            iInterpFromY = Mathd::RoundToInt(dSrcPtY-fFilterRadius);
                            iInterpUpToY = iInterpFromY + iFilterDiam;
                            if ( iInterpFromY >= 0 && iInterpUpToY <= m_spqSrcImage->m_iHeight )
                            {
                                // in
                                for (int i = 0; i < iSpp; i++)
                                    qCompAccums[i] = 0.0f;
                                for (int iSrcY = iInterpFromY, i0 = 0; iSrcY < iInterpUpToY; iSrcY++, i0++)
                                {
                                    fContrib = qContribsMain[i0];
                                    aucSrcPix = pucSrcBase + iSrcY*m_spqSrcImage->m_iRowStride;
                                    for (int i1 = 0; i1 < iSpp; i1++)
                                        qCompAccums[i1] += fContrib*(*((unsigned short*)(aucSrcPix + i1*2)));
                                }
                            }
                            else if ( iInterpUpToY <= 0 || iInterpFromY >= m_spqSrcImage->m_iHeight )
                            {
                                // out
                                if ( !m_qOuterColor.Specified )
                                {
                                    for (int i = 0; i < iSpp; i++)
                                        qCompAccums[i] = 0.0f;
                                }
                                else
                                {
                                    for (int i = 0; i < iSpp; i++)
                                        qCompAccums[i] = (float)(*((unsigned short*)(qOCBytes + i*2)));
                                }
                            }
                            else
                            {
                                // on an edge
                                for (int i = 0; i < iSpp; i++)
                                    qCompAccums[i] = 0.0f;
                                fAlphaPM = 0.0f;
                                for (int iSrcY = iInterpFromY, i0 = 0; iSrcY < iInterpUpToY; iSrcY++, i0++)
                                {
                                    if ( 0 <= iSrcY && iSrcY < m_spqSrcImage->m_iHeight )
                                    {
                                        fContrib = qContribsEdge[i0];
                                        aucSrcPix = pucSrcBase + iSrcY*m_spqSrcImage->m_iRowStride;
                                        for (int i1 = 0; i1 < iSpp; i1++)
                                            qCompAccums[i1] += fContrib*(*((unsigned short*)(aucSrcPix + i1*2)));

                                        fAlphaPM += fContrib;
                                    }
                                }

                                if ( !m_spqSrcImage->m_bHasAlpha )
                                {
                                    if ( m_qOuterColor.Specified )
                                    {
                                        fOneMinusAlphaPM = 1.0f - fAlphaPM;
                                        for (int i = 0; i < iSpp; i++)
                                        {
                                            qCompAccums[i] += fOneMinusAlphaPM*
                                                (*((unsigned short*)(qOCBytes + i*2)));
                                        }
                                    }
                                }
                                else
                                {
                                    if ( fAlphaPM > Mathf::ZERO_TOLERANCE )
                                    {
                                        fInvAlphaPM = 1.0f/fAlphaPM;
                                        for (int i = 0; i < iSppMinusOne; i++)
                                            qCompAccums[i] *= fInvAlphaPM;
                                    }
                                    else
                                    {
                                        for (int i = 0; i < iSppMinusOne; i++)
                                            qCompAccums[i] = (float)65535;
                                    }
                                }
                            }

                            // assign
                            aucDstPix = pucDstBase + iDstY*spqNewImage->m_iRowStride;
                            for (int i = 0; i < iSpp; i++)
                            {
                                iValue = Mathf::RoundToInt(qCompAccums[i]);
                                if ( iValue < 0 )
                                    iValue = 0;
                                else if ( iValue > 65535 )
                                    iValue = 65535;
                                *((unsigned short*)(aucDstPix + i*2)) = iValue;
                            }
                        }

                        qProgress.MakeAdvance();
                    }
                }
                else if ( bCmykCase )
                {
                    for (int iX = 0; iX < iNewWidth; iX++)
                    {
                        fDstPixCntX = (float)iX + 0.5f;

                        // prepare interpolation for the column
                        PREPARE_SKEW_INTERPOLATION_FOR_COLUMN;

                        // interpolate
                        pucSrcBase = (unsigned char*)(m_spqSrcImage->m_acImage +
                            iX*m_spqSrcImage->m_iBytesPerPixel);
                        pucDstBase = (unsigned char*)(spqNewImage->m_acImage + iX*spqNewImage->m_iBytesPerPixel);
                        for (int iDstY = 0; iDstY < iNewHeight; iDstY++)
                        {
                            dSrcPtY = dShiftY1 + iDstY;

                            iInterpFromY = Mathd::RoundToInt(dSrcPtY-fFilterRadius);
                            iInterpUpToY = iInterpFromY + iFilterDiam;
                            if ( iInterpFromY >= 0 && iInterpUpToY <= m_spqSrcImage->m_iHeight )
                            {
                                // in
                                for (int i = 0; i < iSpp; i++)
                                    qCompAccums[i] = 0.0f;
                                for (int iSrcY = iInterpFromY, i0 = 0; iSrcY < iInterpUpToY; iSrcY++, i0++)
                                {
                                    fContrib = qContribsMain[i0];
                                    aucSrcPix = pucSrcBase + iSrcY*m_spqSrcImage->m_iRowStride;
                                    for (int i1 = 0; i1 < iSpp; i1++)
                                        qCompAccums[i1] += fContrib*(*((unsigned short*)(aucSrcPix + i1*2)));
                                }
                            }
                            else if ( iInterpUpToY <= 0 || iInterpFromY >= m_spqSrcImage->m_iHeight )
                            {
                                // out
                                if ( !m_qOuterColor.Specified )
                                {
                                    for (int i = 0; i < iSpp; i++)
                                        qCompAccums[i] = 0.0f;
                                }
                                else
                                {
                                    for (int i = 0; i < iSpp; i++)
                                        qCompAccums[i] = (float)(*((unsigned short*)(qOCBytes + i*2)));
                                }
                            }
                            else
                            {
                                // on an edge
                                for (int i = 0; i < iSpp; i++)
                                    qCompAccums[i] = 0.0f;
                                fAlphaPM = 0.0f;
                                for (int iSrcY = iInterpFromY, i0 = 0; iSrcY < iInterpUpToY; iSrcY++, i0++)
                                {
                                    if ( 0 <= iSrcY && iSrcY < m_spqSrcImage->m_iHeight )
                                    {
                                        fContrib = qContribsEdge[i0];
                                        aucSrcPix = pucSrcBase + iSrcY*m_spqSrcImage->m_iRowStride;
                                        for (int i1 = 0; i1 < iSpp; i1++)
                                            qCompAccums[i1] += fContrib*(*((unsigned short*)(aucSrcPix + i1*2)));

                                        fAlphaPM += fContrib;
                                    }
                                }

                                if ( !m_spqSrcImage->m_bHasAlpha )
                                {
                                    if ( m_qOuterColor.Specified )
                                    {
                                        fOneMinusAlphaPM = 1.0f - fAlphaPM;
                                        for (int i = 0; i < iSpp; i++)
                                        {
                                            qCompAccums[i] += fOneMinusAlphaPM*
                                                (*((unsigned short*)(qOCBytes + i*2)));
                                        }
                                    }
                                }
                                else
                                {
                                    if ( fAlphaPM > Mathf::ZERO_TOLERANCE )
                                    {
                                        fInvAlphaPM = 1.0f/fAlphaPM;
                                        for (int i = 0; i < iSppMinusOne; i++)
                                            qCompAccums[i] *= fInvAlphaPM;
                                    }
                                    else
                                    {
                                        for (int i = 0; i < iSppMinusOne; i++)
                                            qCompAccums[i] = (float)0;
                                    }
                                }
                            }

                            // assign
                            aucDstPix = pucDstBase + iDstY*spqNewImage->m_iRowStride;
                            for (int i = 0; i < iSpp; i++)
                            {
                                iValue = Mathf::RoundToInt(qCompAccums[i]);
                                if ( iValue < 0 )
                                    iValue = 0;
                                else if ( iValue > 65535 )
                                    iValue = 65535;
                                *((unsigned short*)(aucDstPix + i*2)) = iValue;
                            }
                        }

                        qProgress.MakeAdvance();
                    }
                }
                else  // L*a*b* case
                {
                    unsigned short usValue;
                    for (int iX = 0; iX < iNewWidth; iX++)
                    {
                        fDstPixCntX = (float)iX + 0.5f;

                        // prepare interpolation for the column
                        PREPARE_SKEW_INTERPOLATION_FOR_COLUMN;

                        // interpolate
                        pucSrcBase = (unsigned char*)(m_spqSrcImage->m_acImage +
                            iX*m_spqSrcImage->m_iBytesPerPixel);
                        pucDstBase = (unsigned char*)(spqNewImage->m_acImage + iX*spqNewImage->m_iBytesPerPixel);
                        for (int iDstY = 0; iDstY < iNewHeight; iDstY++)
                        {
                            dSrcPtY = dShiftY1 + iDstY;

                            iInterpFromY = Mathd::RoundToInt(dSrcPtY-fFilterRadius);
                            iInterpUpToY = iInterpFromY + iFilterDiam;
                            if ( iInterpFromY >= 0 && iInterpUpToY <= m_spqSrcImage->m_iHeight )
                            {
                                // in
                                for (int i = 0; i < iSpp; i++)
                                    qCompAccums[i] = 0.0f;
                                for (int iSrcY = iInterpFromY, i0 = 0; iSrcY < iInterpUpToY; iSrcY++, i0++)
                                {
                                    fContrib = qContribsMain[i0];
                                    aucSrcPix = pucSrcBase + iSrcY*m_spqSrcImage->m_iRowStride;
                                    for (int i1 = 0; i1 < iSpp; i1++)
                                        qCompAccums[i1] += fContrib*(*((unsigned short*)(aucSrcPix + i1*2)));
                                }
                            }
                            else if ( iInterpUpToY <= 0 || iInterpFromY >= m_spqSrcImage->m_iHeight )
                            {
                                // out
                                if ( !m_qOuterColor.Specified )
                                {
                                    for (int i = 0; i < iSpp; i++)
                                        qCompAccums[i] = ( i != 1 && i != 2 ? 0.0f : (float)32768 );
                                }
                                else
                                {
                                    for (int i = 0; i < iSpp; i++)
                                        qCompAccums[i] = (float)(*((unsigned short*)(qOCBytes + i*2)));
                                }
                            }
                            else
                            {
                                // on an edge
                                for (int i = 0; i < iSpp; i++)
                                    qCompAccums[i] = 0.0f;
                                fAlphaPM = 0.0f;
                                for (int iSrcY = iInterpFromY, i0 = 0; iSrcY < iInterpUpToY; iSrcY++, i0++)
                                {
                                    if ( 0 <= iSrcY && iSrcY < m_spqSrcImage->m_iHeight )
                                    {
                                        fContrib = qContribsEdge[i0];
                                        aucSrcPix = pucSrcBase + iSrcY*m_spqSrcImage->m_iRowStride;
                                        for (int i1 = 0; i1 < iSpp; i1++)
                                        {
                                            usValue = *((unsigned short*)(aucSrcPix + i1*2));
                                            qCompAccums[i1] += fContrib*
                                                ( i1 != 1 && i1 != 2 ? usValue : usValue - 32768 );
                                        }

                                        fAlphaPM += fContrib;
                                    }
                                }

                                if ( !m_spqSrcImage->m_bHasAlpha )
                                {
                                    if ( m_qOuterColor.Specified )
                                    {
                                        fOneMinusAlphaPM = 1.0f - fAlphaPM;
                                        for (int i = 0; i < iSpp; i++)
                                        {
                                            usValue = *((unsigned short*)(qOCBytes + i*2));
                                            qCompAccums[i] += fOneMinusAlphaPM*
                                                ( i != 1 && i != 2 ? usValue : usValue - 32768 );
                                        }
                                    }
                                }
                                else
                                {
                                    if ( fAlphaPM > Mathf::ZERO_TOLERANCE )
                                    {
                                        fInvAlphaPM = 1.0f/fAlphaPM;
                                        for (int i = 0; i < iSppMinusOne; i++)
                                            qCompAccums[i] *= fInvAlphaPM;
                                    }
                                    else
                                    {
                                        qCompAccums[0] = (float)65535;
                                        for (int i = 1; i < iSppMinusOne; i++)
                                            qCompAccums[i] = Mathf::Sign(qCompAccums[i])*32768;
                                    }
                                }

                                for (int i = 1; i < 3; i++)
                                    qCompAccums[i] += 32768;
                            }

                            // assign
                            aucDstPix = pucDstBase + iDstY*spqNewImage->m_iRowStride;
                            for (int i = 0; i < iSpp; i++)
                            {
                                iValue = Mathf::RoundToInt(qCompAccums[i]);
                                if ( iValue < 0 )
                                    iValue = 0;
                                else if ( iValue > 65535 )
                                    iValue = 65535;
                                *((unsigned short*)(aucDstPix + i*2)) = iValue;
                            }
                        }

                        qProgress.MakeAdvance();
                    }
                }
            }
        }
        else
        {
            bool bDoColorClamping = !m_spqSrcImage->m_bIsHighDynamicRange;
            if ( m_spqSrcImage->m_eColorModel == RasterImage::CMT_GRAYSCALE )
            {
                float fIntensityAccum, fIntensity;
                for (int iX = 0; iX < iNewWidth; iX++)
                {
                    fDstPixCntX = (float)iX + 0.5f;

                    // prepare interpolation for the column
                    PREPARE_SKEW_INTERPOLATION_FOR_COLUMN;

                    // interpolate
                    for (int iDstY = 0; iDstY < iNewHeight; iDstY++)
                    {
                        dSrcPtY = dShiftY1 + iDstY;

                        iInterpFromY = Mathd::RoundToInt(dSrcPtY-fFilterRadius);
                        iInterpUpToY = iInterpFromY + iFilterDiam;
                        if ( iInterpFromY >= 0 && iInterpUpToY <= m_spqSrcImage->m_iHeight )
                        {
                            // in
                            fIntensityAccum = 0.0f;
                            for (int iSrcY = iInterpFromY, i = 0; iSrcY < iInterpUpToY; iSrcY++, i++)
                            {
                                fContrib = qContribsMain[i];
                                fIntensity = m_spqSrcImage->GetPixelIntensity(iX,iSrcY);
                                fIntensityAccum += fIntensity*fContrib;
                            }
                        }
                        else if ( iInterpUpToY <= 0 || iInterpFromY >= m_spqSrcImage->m_iHeight )
                        {
                            // out
                            fIntensityAccum = ( !m_qOuterColor.Specified ? 0.0f : m_qOuterColor.Intensity );
                        }
                        else
                        {
                            // on an edge
                            fIntensityAccum = 0.0f;
                            fAlphaPM = 0.0f;
                            for (int iSrcY = iInterpFromY, i = 0; iSrcY < iInterpUpToY; iSrcY++, i++)
                            {
                                if ( 0 <= iSrcY && iSrcY < m_spqSrcImage->m_iHeight )
                                {
                                    fContrib = qContribsEdge[i];
                                    fIntensity = m_spqSrcImage->GetPixelIntensity(iX,iSrcY);
                                    fIntensityAccum += fIntensity*fContrib;

                                    fAlphaPM += fContrib;
                                }
                            }

                            if ( m_qOuterColor.Specified )
                            {
                                fOneMinusAlphaPM = 1.0f - fAlphaPM;
                                fIntensityAccum += m_qOuterColor.Intensity*fOneMinusAlphaPM;
                            }
                        }

                        // assign
                        if ( bDoColorClamping )
                        {
                            if ( fIntensityAccum < 0.0f )
                                fIntensityAccum = 0.0f;
                            else if ( fIntensityAccum > 1.0f )
                                fIntensityAccum = 1.0f;
                        }
                        spqNewImage->SetPixelIntensity(fIntensityAccum,iX,iDstY);
                    }

                    qProgress.MakeAdvance();
                }
            }
            else if ( m_spqSrcImage->m_eColorModel == RasterImage::CMT_GRAYSCALE_WITH_ALPHA )
            {
                Link<float,float> qIntensityWithAlphaAccum, qIntensityWithAlpha;
                for (int iX = 0; iX < iNewWidth; iX++)
                {
                    fDstPixCntX = (float)iX + 0.5f;

                    // prepare interpolation for the column
                    PREPARE_SKEW_INTERPOLATION_FOR_COLUMN;

                    // interpolate
                    for (int iDstY = 0; iDstY < iNewHeight; iDstY++)
                    {
                        dSrcPtY = dShiftY1 + iDstY;

                        iInterpFromY = Mathd::RoundToInt(dSrcPtY-fFilterRadius);
                        iInterpUpToY = iInterpFromY + iFilterDiam;
                        if ( iInterpFromY >= 0 && iInterpUpToY <= m_spqSrcImage->m_iHeight )
                        {
                            // in
                            qIntensityWithAlphaAccum.First = 0.0f;
                            qIntensityWithAlphaAccum.Second = 0.0f;
                            for (int iSrcY = iInterpFromY, i = 0; iSrcY < iInterpUpToY; iSrcY++, i++)
                            {
                                fContrib = qContribsMain[i];
                                qIntensityWithAlpha = m_spqSrcImage->GetPixelIntensityWithAlpha(iX,iSrcY);
                                qIntensityWithAlphaAccum.First += qIntensityWithAlpha.First*fContrib;
                                qIntensityWithAlphaAccum.Second += qIntensityWithAlpha.Second*fContrib;
                            }
                        }
                        else if ( iInterpUpToY <= 0 || iInterpFromY >= m_spqSrcImage->m_iHeight )
                        {
                            // out
                            qIntensityWithAlphaAccum.First =
                                ( !m_qOuterColor.Specified ? 0.0f : m_qOuterColor.Intensity );
                            qIntensityWithAlphaAccum.Second = 0.0f;
                        }
                        else
                        {
                            // on an edge
                            qIntensityWithAlphaAccum.First = 0.0f;
                            qIntensityWithAlphaAccum.Second = 0.0f;
                            fAlphaPM = 0.0f;
                            for (int iSrcY = iInterpFromY, i = 0; iSrcY < iInterpUpToY; iSrcY++, i++)
                            {
                                if ( 0 <= iSrcY && iSrcY < m_spqSrcImage->m_iHeight )
                                {
                                    fContrib = qContribsEdge[i];
                                    qIntensityWithAlpha = m_spqSrcImage->GetPixelIntensityWithAlpha(iX,iSrcY);
                                    qIntensityWithAlphaAccum.First += qIntensityWithAlpha.First*fContrib;
                                    qIntensityWithAlphaAccum.Second += qIntensityWithAlpha.Second*fContrib;

                                    fAlphaPM += fContrib;
                                }
                            }

                            if ( fAlphaPM > Mathf::ZERO_TOLERANCE )
                            {
                                fInvAlphaPM = 1.0f/fAlphaPM;
                                qIntensityWithAlphaAccum.First *= fInvAlphaPM;
                            }
                            else
                            {
                                qIntensityWithAlphaAccum.First = 1.0f;
                            }
                        }

                        // assign
                        if ( bDoColorClamping )
                        {
                            if ( qIntensityWithAlphaAccum.First < 0.0f )
                                qIntensityWithAlphaAccum.First = 0.0f;
                            else if ( qIntensityWithAlphaAccum.First > 1.0f )
                                qIntensityWithAlphaAccum.First = 1.0f;
                        }
                        if ( qIntensityWithAlphaAccum.Second < 0.0f )
                            qIntensityWithAlphaAccum.Second = 0.0f;
                        else if ( qIntensityWithAlphaAccum.Second > 1.0f )
                            qIntensityWithAlphaAccum.Second = 1.0f;
                        spqNewImage->SetPixelIntensityWithAlpha(qIntensityWithAlphaAccum,iX,iDstY);
                    }

                    qProgress.MakeAdvance();
                }
            }
            else if ( m_spqSrcImage->m_eColorModel == RasterImage::CMT_RGB )
            {
                ColorRgb qRgbAccum, qRgb;
                ColorRgb qRgbZero(0.0f,0.0f,0.0f);
                for (int iX = 0; iX < iNewWidth; iX++)
                {
                    fDstPixCntX = (float)iX + 0.5f;

                    // prepare interpolation for the column
                    PREPARE_SKEW_INTERPOLATION_FOR_COLUMN;

                    // interpolate
                    for (int iDstY = 0; iDstY < iNewHeight; iDstY++)
                    {
                        dSrcPtY = dShiftY1 + iDstY;

                        iInterpFromY = Mathd::RoundToInt(dSrcPtY-fFilterRadius);
                        iInterpUpToY = iInterpFromY + iFilterDiam;
                        if ( iInterpFromY >= 0 && iInterpUpToY <= m_spqSrcImage->m_iHeight )
                        {
                            // in
                            qRgbAccum = qRgbZero;
                            for (int iSrcY = iInterpFromY, i = 0; iSrcY < iInterpUpToY; iSrcY++, i++)
                            {
                                fContrib = qContribsMain[i];
                                qRgb = m_spqSrcImage->GetPixelRgb(iX,iSrcY);
                                qRgbAccum += qRgb*fContrib;
                            }
                        }
                        else if ( iInterpUpToY <= 0 || iInterpFromY >= m_spqSrcImage->m_iHeight )
                        {
                            // out
                            qRgbAccum = ( !m_qOuterColor.Specified ? qRgbZero : m_qOuterColor.RgbColor );
                        }
                        else
                        {
                            // on an edge
                            qRgbAccum = qRgbZero;
                            fAlphaPM = 0.0f;
                            for (int iSrcY = iInterpFromY, i = 0; iSrcY < iInterpUpToY; iSrcY++, i++)
                            {
                                if ( 0 <= iSrcY && iSrcY < m_spqSrcImage->m_iHeight )
                                {
                                    fContrib = qContribsEdge[i];
                                    qRgb = m_spqSrcImage->GetPixelRgb(iX,iSrcY);
                                    qRgbAccum += qRgb*fContrib;

                                    fAlphaPM += fContrib;
                                }
                            }

                            if ( m_qOuterColor.Specified )
                            {
                                fOneMinusAlphaPM = 1.0f - fAlphaPM;
                                qRgbAccum += m_qOuterColor.RgbColor*fOneMinusAlphaPM;
                            }
                        }

                        // assign
                        if ( bDoColorClamping )
                        {
                            for (int i = 0; i < 3; i++)
                            {
                                if ( qRgbAccum[i] < 0.0f )
                                    qRgbAccum[i] = 0.0f;
                                else if ( qRgbAccum[i] > 1.0f )
                                    qRgbAccum[i] = 1.0f;
                            }
                        }
                        spqNewImage->SetPixelRgb(qRgbAccum,iX,iDstY);
                    }

                    qProgress.MakeAdvance();
                }
            }
            else if ( m_spqSrcImage->m_eColorModel == RasterImage::CMT_RGBA )
            {
                ColorRgba qRgbaAccum, qRgba;
                ColorRgba qRgbaZero(0.0f,0.0f,0.0f,0.0f);
                for (int iX = 0; iX < iNewWidth; iX++)
                {
                    fDstPixCntX = (float)iX + 0.5f;

                    // prepare interpolation for the column
                    PREPARE_SKEW_INTERPOLATION_FOR_COLUMN;

                    // interpolate
                    for (int iDstY = 0; iDstY < iNewHeight; iDstY++)
                    {
                        dSrcPtY = dShiftY1 + iDstY;

                        iInterpFromY = Mathd::RoundToInt(dSrcPtY-fFilterRadius);
                        iInterpUpToY = iInterpFromY + iFilterDiam;
                        if ( iInterpFromY >= 0 && iInterpUpToY <= m_spqSrcImage->m_iHeight )
                        {
                            // in
                            qRgbaAccum = qRgbaZero;
                            for (int iSrcY = iInterpFromY, i = 0; iSrcY < iInterpUpToY; iSrcY++, i++)
                            {
                                fContrib = qContribsMain[i];
                                qRgba = m_spqSrcImage->GetPixelRgba(iX,iSrcY);
                                qRgbaAccum += qRgba*fContrib;
                            }
                        }
                        else if ( iInterpUpToY <= 0 || iInterpFromY >= m_spqSrcImage->m_iHeight )
                        {
                            // out
                            if ( !m_qOuterColor.Specified )
                            {
                                qRgbaAccum = qRgbaZero;
                            }
                            else
                            {
                                for (int i = 0; i < 3; i++)
                                    qRgbaAccum[i] = m_qOuterColor.RgbColor[i];
                                qRgbaAccum[3] = 0.0f;
                            }
                        }
                        else
                        {
                            // on an edge
                            qRgbaAccum = qRgbaZero;
                            fAlphaPM = 0.0f;
                            for (int iSrcY = iInterpFromY, i = 0; iSrcY < iInterpUpToY; iSrcY++, i++)
                            {
                                if ( 0 <= iSrcY && iSrcY < m_spqSrcImage->m_iHeight )
                                {
                                    fContrib = qContribsEdge[i];
                                    qRgba = m_spqSrcImage->GetPixelRgba(iX,iSrcY);
                                    qRgbaAccum += qRgba*fContrib;

                                    fAlphaPM += fContrib;
                                }
                            }

                            if ( fAlphaPM > Mathf::ZERO_TOLERANCE )
                            {
                                fInvAlphaPM = 1.0f/fAlphaPM;
                                for (int i = 0; i < 3; i++)
                                    qRgbaAccum[i] *= fInvAlphaPM;
                            }
                            else
                            {
                                for (int i = 0; i < 3; i++)
                                    qRgbaAccum[i] = 1.0f;
                            }
                        }

                        // assign
                        if ( bDoColorClamping )
                        {
                            for (int i = 0; i < 3; i++)
                            {
                                if ( qRgbaAccum[i] < 0.0f )
                                    qRgbaAccum[i] = 0.0f;
                                else if ( qRgbaAccum[i] > 1.0f )
                                    qRgbaAccum[i] = 1.0f;
                            }
                        }
                        if ( qRgbaAccum[3] < 0.0f )
                            qRgbaAccum[3] = 0.0f;
                        else if ( qRgbaAccum[3] > 1.0f )
                            qRgbaAccum[3] = 1.0f;
                        spqNewImage->SetPixelRgba(qRgbaAccum,iX,iDstY);
                    }

                    qProgress.MakeAdvance();
                }
            }
            else if ( m_spqSrcImage->m_eColorModel == RasterImage::CMT_CMYK )
            {
                ColorCmyk qCmykAccum, qCmyk;
                ColorCmyk qCmykZero(0.0f,0.0f,0.0f,0.0f);
                for (int iX = 0; iX < iNewWidth; iX++)
                {
                    fDstPixCntX = (float)iX + 0.5f;

                    // prepare interpolation for the column
                    PREPARE_SKEW_INTERPOLATION_FOR_COLUMN;

                    // interpolate
                    for (int iDstY = 0; iDstY < iNewHeight; iDstY++)
                    {
                        dSrcPtY = dShiftY1 + iDstY;

                        iInterpFromY = Mathd::RoundToInt(dSrcPtY-fFilterRadius);
                        iInterpUpToY = iInterpFromY + iFilterDiam;
                        if ( iInterpFromY >= 0 && iInterpUpToY <= m_spqSrcImage->m_iHeight )
                        {
                            // in
                            qCmykAccum = qCmykZero;
                            for (int iSrcY = iInterpFromY, i = 0; iSrcY < iInterpUpToY; iSrcY++, i++)
                            {
                                fContrib = qContribsMain[i];
                                qCmyk = m_spqSrcImage->GetPixelCmyk(iX,iSrcY);
                                qCmykAccum += qCmyk*fContrib;
                            }
                        }
                        else if ( iInterpUpToY <= 0 || iInterpFromY >= m_spqSrcImage->m_iHeight )
                        {
                            // out
                            qCmykAccum = ( !m_qOuterColor.Specified ? qCmykZero : m_qOuterColor.CmykColor );
                        }
                        else
                        {
                            // on an edge
                            qCmykAccum = qCmykZero;
                            fAlphaPM = 0.0f;
                            for (int iSrcY = iInterpFromY, i = 0; iSrcY < iInterpUpToY; iSrcY++, i++)
                            {
                                if ( 0 <= iSrcY && iSrcY < m_spqSrcImage->m_iHeight )
                                {
                                    fContrib = qContribsEdge[i];
                                    qCmyk = m_spqSrcImage->GetPixelCmyk(iX,iSrcY);
                                    qCmykAccum += qCmyk*fContrib;

                                    fAlphaPM += fContrib;
                                }
                            }

                            if ( m_qOuterColor.Specified )
                            {
                                fOneMinusAlphaPM = 1.0f - fAlphaPM;
                                qCmykAccum += m_qOuterColor.CmykColor*fOneMinusAlphaPM;
                            }
                        }

                        // assign
                        if ( bDoColorClamping )
                        {
                            for (int i = 0; i < 4; i++)
                            {
                                if ( qCmykAccum[i] < 0.0f )
                                    qCmykAccum[i] = 0.0f;
                                else if ( qCmykAccum[i] > 1.0f )
                                    qCmykAccum[i] = 1.0f;
                            }
                        }
                        spqNewImage->SetPixelCmyk(qCmykAccum,iX,iDstY);
                    }

                    qProgress.MakeAdvance();
                }
            }
            else if ( m_spqSrcImage->m_eColorModel == RasterImage::CMT_CMYK_WITH_ALPHA )
            {
                ColorCmyka qCmykaAccum, qCmyka;
                ColorCmyka qCmykaZero(0.0f,0.0f,0.0f,0.0f,0.0f);
                for (int iX = 0; iX < iNewWidth; iX++)
                {
                    fDstPixCntX = (float)iX + 0.5f;

                    // prepare interpolation for the column
                    PREPARE_SKEW_INTERPOLATION_FOR_COLUMN;

                    // interpolate
                    for (int iDstY = 0; iDstY < iNewHeight; iDstY++)
                    {
                        dSrcPtY = dShiftY1 + iDstY;

                        iInterpFromY = Mathd::RoundToInt(dSrcPtY-fFilterRadius);
                        iInterpUpToY = iInterpFromY + iFilterDiam;
                        if ( iInterpFromY >= 0 && iInterpUpToY <= m_spqSrcImage->m_iHeight )
                        {
                            // in
                            qCmykaAccum = qCmykaZero;
                            for (int iSrcY = iInterpFromY, i = 0; iSrcY < iInterpUpToY; iSrcY++, i++)
                            {
                                fContrib = qContribsMain[i];
                                qCmyka = m_spqSrcImage->GetPixelCmyka(iX,iSrcY);
                                qCmykaAccum += qCmyka*fContrib;
                            }
                        }
                        else if ( iInterpUpToY <= 0 || iInterpFromY >= m_spqSrcImage->m_iHeight )
                        {
                            // out
                            if ( !m_qOuterColor.Specified )
                            {
                                qCmykaAccum = qCmykaZero;
                            }
                            else
                            {
                                for (int i = 0; i < 4; i++)
                                    qCmykaAccum[i] = m_qOuterColor.CmykColor[i];
                                qCmykaAccum[4] = 0.0f;
                            }
                        }
                        else
                        {
                            // on an edge
                            qCmykaAccum = qCmykaZero;
                            fAlphaPM = 0.0f;
                            for (int iSrcY = iInterpFromY, i = 0; iSrcY < iInterpUpToY; iSrcY++, i++)
                            {
                                if ( 0 <= iSrcY && iSrcY < m_spqSrcImage->m_iHeight )
                                {
                                    fContrib = qContribsEdge[i];
                                    qCmyka = m_spqSrcImage->GetPixelCmyka(iX,iSrcY);
                                    qCmykaAccum += qCmyka*fContrib;

                                    fAlphaPM += fContrib;
                                }
                            }

                            if ( fAlphaPM > Mathf::ZERO_TOLERANCE )
                            {
                                fInvAlphaPM = 1.0f/fAlphaPM;
                                for (int i = 0; i < 4; i++)
                                    qCmykaAccum[i] *= fInvAlphaPM;
                            }
                            else
                            {
                                for (int i = 0; i < 4; i++)
                                    qCmykaAccum[i] = 0.0f;
                            }
                        }

                        // assign
                        if ( bDoColorClamping )
                        {
                            for (int i = 0; i < 4; i++)
                            {
                                if ( qCmykaAccum[i] < 0.0f )
                                    qCmykaAccum[i] = 0.0f;
                                else if ( qCmykaAccum[i] > 1.0f )
                                    qCmykaAccum[i] = 1.0f;
                            }
                        }
                        if ( qCmykaAccum[4] < 0.0f )
                            qCmykaAccum[4] = 0.0f;
                        else if ( qCmykaAccum[4] > 1.0f )
                            qCmykaAccum[4] = 1.0f;
                        spqNewImage->SetPixelCmyka(qCmykaAccum,iX,iDstY);
                    }

                    qProgress.MakeAdvance();
                }
            }
            else if ( m_spqSrcImage->m_eColorModel == RasterImage::CMT_LAB )
            {
                ColorLab qLabAccum, qLab;
                ColorLab qLabZero(0.0f,0.0f,0.0f);
                for (int iX = 0; iX < iNewWidth; iX++)
                {
                    fDstPixCntX = (float)iX + 0.5f;

                    // prepare interpolation for the column
                    PREPARE_SKEW_INTERPOLATION_FOR_COLUMN;

                    // interpolate
                    for (int iDstY = 0; iDstY < iNewHeight; iDstY++)
                    {
                        dSrcPtY = dShiftY1 + iDstY;

                        iInterpFromY = Mathd::RoundToInt(dSrcPtY-fFilterRadius);
                        iInterpUpToY = iInterpFromY + iFilterDiam;
                        if ( iInterpFromY >= 0 && iInterpUpToY <= m_spqSrcImage->m_iHeight )
                        {
                            // in
                            qLabAccum = qLabZero;
                            for (int iSrcY = iInterpFromY, i = 0; iSrcY < iInterpUpToY; iSrcY++, i++)
                            {
                                fContrib = qContribsMain[i];
                                qLab = m_spqSrcImage->GetPixelLab(iX,iSrcY);
                                qLabAccum += qLab*fContrib;
                            }
                        }
                        else if ( iInterpUpToY <= 0 || iInterpFromY >= m_spqSrcImage->m_iHeight )
                        {
                            // out
                            qLabAccum = ( !m_qOuterColor.Specified ? qLabZero : m_qOuterColor.LabColor );
                        }
                        else
                        {
                            // on an edge
                            qLabAccum = qLabZero;
                            fAlphaPM = 0.0f;
                            for (int iSrcY = iInterpFromY, i = 0; iSrcY < iInterpUpToY; iSrcY++, i++)
                            {
                                if ( 0 <= iSrcY && iSrcY < m_spqSrcImage->m_iHeight )
                                {
                                    fContrib = qContribsEdge[i];
                                    qLab = m_spqSrcImage->GetPixelLab(iX,iSrcY);
                                    qLabAccum += qLab*fContrib;

                                    fAlphaPM += fContrib;
                                }
                            }

                            if ( m_qOuterColor.Specified )
                            {
                                fOneMinusAlphaPM = 1.0f - fAlphaPM;
                                qLabAccum += m_qOuterColor.LabColor*fOneMinusAlphaPM;
                            }
                        }

                        // assign
                        if ( bDoColorClamping )
                        {
                            if ( qLabAccum[0] < 0.0f )
                                qLabAccum[0] = 0.0f;
                            else if ( qLabAccum[0] > 100.0f )
                                qLabAccum[0] = 100.0f;
                        }
                        spqNewImage->SetPixelLab(qLabAccum,iX,iDstY);
                    }

                    qProgress.MakeAdvance();
                }
            }
            else  // m_spqSrcImage->m_eColorModel = RasterImage::CMT_LAB_WITH_ALPHA
            {
                ColorLaba qLabaAccum, qLaba;
                ColorLaba qLabaZero(0.0f,0.0f,0.0f,0.0f);
                for (int iX = 0; iX < iNewWidth; iX++)
                {
                    fDstPixCntX = (float)iX + 0.5f;

                    // prepare interpolation for the column
                    PREPARE_SKEW_INTERPOLATION_FOR_COLUMN;

                    // interpolate
                    for (int iDstY = 0; iDstY < iNewHeight; iDstY++)
                    {
                        dSrcPtY = dShiftY1 + iDstY;

                        iInterpFromY = Mathd::RoundToInt(dSrcPtY-fFilterRadius);
                        iInterpUpToY = iInterpFromY + iFilterDiam;
                        if ( iInterpFromY >= 0 && iInterpUpToY <= m_spqSrcImage->m_iHeight )
                        {
                            // in
                            qLabaAccum = qLabaZero;
                            for (int iSrcY = iInterpFromY, i = 0; iSrcY < iInterpUpToY; iSrcY++, i++)
                            {
                                fContrib = qContribsMain[i];
                                qLaba = m_spqSrcImage->GetPixelLaba(iX,iSrcY);
                                qLabaAccum += qLaba*fContrib;
                            }
                        }
                        else if ( iInterpUpToY <= 0 || iInterpFromY >= m_spqSrcImage->m_iHeight )
                        {
                            // out
                            if ( !m_qOuterColor.Specified )
                            {
                                qLabaAccum = qLabaZero;
                            }
                            else
                            {
                                for (int i = 0; i < 3; i++)
                                    qLabaAccum[i] = m_qOuterColor.LabColor[i];
                                qLabaAccum[3] = 0.0f;
                            }
                        }
                        else
                        {
                            // on an edge
                            qLabaAccum = qLabaZero;
                            fAlphaPM = 0.0f;
                            for (int iSrcY = iInterpFromY, i = 0; iSrcY < iInterpUpToY; iSrcY++, i++)
                            {
                                if ( 0 <= iSrcY && iSrcY < m_spqSrcImage->m_iHeight )
                                {
                                    fContrib = qContribsEdge[i];
                                    qLaba = m_spqSrcImage->GetPixelLaba(iX,iSrcY);
                                    qLabaAccum += qLaba*fContrib;

                                    fAlphaPM += fContrib;
                                }
                            }

                            if ( fAlphaPM > Mathf::ZERO_TOLERANCE )
                            {
                                fInvAlphaPM = 1.0f/fAlphaPM;
                                for (int i = 0; i < 3; i++)
                                    qLabaAccum[i] *= fInvAlphaPM;
                            }
                            else
                            {
                                qLabaAccum[0] = 100.0f;
                                for (int i = 1; i < 3; i++)
                                    qLabaAccum[i] = Mathf::Sign(qLabaAccum[i])*128.0f;
                            }
                        }

                        // assign
                        if ( bDoColorClamping )
                        {
                            if ( qLabaAccum[0] < 0.0f )
                                qLabaAccum[0] = 0.0f;
                            else if ( qLabaAccum[0] > 100.0f )
                                qLabaAccum[0] = 100.0f;
                        }
                        if ( qLabaAccum[3] < 0.0f )
                            qLabaAccum[3] = 0.0f;
                        else if ( qLabaAccum[3] > 1.0f )
                            qLabaAccum[3] = 1.0f;
                        spqNewImage->SetPixelLaba(qLabaAccum,iX,iDstY);
                    }

                    qProgress.MakeAdvance();
                }
            }
        }
    }

    if ( m_spqSrcImage->HasVisibilityMask() )
    {
        // treat the visibility mask as a grayscale image to be transformed
        TRANSFORM_VISIBILITY_MASK_AS_WELL(GetSkewedVertically(fAngle,eInterpolation));
    }

    return spqNewImage;
}
//------------------------------------------------------------------------------------------------------------------
void RasterImageTransformation::GetOuterColorAsBytes (StillArray<unsigned char>& rqOCBytes) const
{
    RasterImagePtr spqDot;
    if ( m_spqSrcImage->m_eColorModel == RasterImage::CMT_PALETTED
    ||   m_spqSrcImage->m_eColorModel == RasterImage::CMT_PALETTED_WITH_ALPHA )
    {
        spqDot = new RasterImage(m_spqSrcImage->m_eType,1,1,m_spqSrcImage->m_qPalette);
    }
    else
    {
        spqDot = new RasterImage(m_spqSrcImage->m_eType,1,1,m_spqSrcImage->m_bIsHighDynamicRange);
    }
    if ( spqDot->m_eColorModel == RasterImage::CMT_PALETTED
    ||   spqDot->m_eColorModel == RasterImage::CMT_PALETTED_WITH_ALPHA )
    {
        spqDot->SetPixelIndex(m_qOuterColor.Index,0,0);
    }
    else if ( spqDot->m_eColorModel == RasterImage::CMT_GRAYSCALE )
    {
        spqDot->SetPixelIntensity(m_qOuterColor.Intensity,0,0);
    }
    else if ( spqDot->m_eColorModel == RasterImage::CMT_GRAYSCALE_WITH_ALPHA )
    {
        spqDot->SetPixelIntensityWithAlpha(MakeLink(
            m_qOuterColor.Intensity,
            0.0f),0,0);
    }
    else if ( spqDot->m_eColorModel == RasterImage::CMT_RGB )
    {
        spqDot->SetPixelRgb(m_qOuterColor.RgbColor,0,0);
    }
    else if ( spqDot->m_eColorModel == RasterImage::CMT_RGBA )
    {
        spqDot->SetPixelRgba(ColorRgba(
            m_qOuterColor.RgbColor.R(),
            m_qOuterColor.RgbColor.G(),
            m_qOuterColor.RgbColor.B(),
            0.0f),0,0);
    }
    else if ( spqDot->m_eColorModel == RasterImage::CMT_CMYK )
    {
        spqDot->SetPixelCmyk(m_qOuterColor.CmykColor,0,0);
    }
    else if ( spqDot->m_eColorModel == RasterImage::CMT_CMYK_WITH_ALPHA )
    {
        spqDot->SetPixelCmyka(ColorCmyka(
            m_qOuterColor.CmykColor.C(),
            m_qOuterColor.CmykColor.M(),
            m_qOuterColor.CmykColor.Y(),
            m_qOuterColor.CmykColor.K(),
            0.0f),0,0);
    }
    else if ( spqDot->m_eColorModel == RasterImage::CMT_LAB )
    {
        spqDot->SetPixelLab(m_qOuterColor.LabColor,0,0);
    }
    else  // spqDot->m_eColorModel = RasterImage::CMT_LAB_WITH_ALPHA
    {
        spqDot->SetPixelLaba(ColorLaba(
            m_qOuterColor.LabColor.L(),
            m_qOuterColor.LabColor.A(),
            m_qOuterColor.LabColor.B(),
            0.0f),0,0);
    }
    rqOCBytes.SetQuantity(m_spqSrcImage->m_iBytesPerPixel);
    MemCpy(rqOCBytes,spqDot->m_acImage,m_spqSrcImage->m_iBytesPerPixel);
}
//------------------------------------------------------------------------------------------------------------------



