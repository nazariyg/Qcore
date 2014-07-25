#include "QCompressionDxt.h"
#include "QCompressionDxt.mcr"
#include "QProgressProbe.h"
#include "QStorage.h"
#include "QPrincipalComponentsAnalysis.h"
#include "QSortUtils.h"
using namespace Q;

//------------------------------------------------------------------------------------------------------------------
CompressionDxt::CompressionDxt (CompressionType eCompression, CompressionQualityType eCompressionQuality)
    :
    m_eCompression(eCompression),
    m_eCompressionQuality(eCompressionQuality),
    m_iDxt1aThreshold(128)
{
    OnWeightingFactorsUpdate(0.3086f,0.6094f,0.082f);

    // LUTs for further compression
    for (int i = 0; i < 32; i++)
        m_ai5To8[i] = (i << 3) | (i >> 2);
    for (int i = 0; i < 64; i++)
        m_ai6To8[i] = (i << 2) | (i >> 4);
}
//------------------------------------------------------------------------------------------------------------------
void CompressionDxt::SetWeightingFactors (float fFcrR, float fFcrG, float fFcrB)
{
    assert( 0.01f <= fFcrR && fFcrR <= 1.0f
        &&  0.01f <= fFcrG && fFcrG <= 1.0f
        &&  0.01f <= fFcrB && fFcrB <= 1.0f );

    OnWeightingFactorsUpdate(fFcrR,fFcrG,fFcrB);
}
//------------------------------------------------------------------------------------------------------------------
void CompressionDxt::Compress (const RasterImagePtr& rspqRasterImage, StillArray<char>& rqBytes)
{
    rqBytes.RemoveAll();

    m_iWidth = rspqRasterImage->GetWidth();
    m_iHeight = rspqRasterImage->GetHeight();

    assert( rspqRasterImage->GetType() == RasterImage::IT_RGB_8_8_8
        ||  rspqRasterImage->GetType() == RasterImage::IT_RGBA_8_8_8_8 );
    assert( !(rspqRasterImage->GetType() == RasterImage::IT_RGB_8_8_8
        &&  (m_eCompression == CT_DXT_2
        ||  m_eCompression == CT_DXT_3
        ||  m_eCompression == CT_DXT_4
        ||  m_eCompression == CT_DXT_5)) );
    assert( (m_iWidth & 3) == 0 && (m_iHeight & 3) == 0 );

    int iBlocksPerWidth = m_iWidth/4;
    int iBlocksPerHeight = m_iHeight/4;

    // allocate space for compressed data
    int iOccupancy = m_iWidth*m_iHeight;
    if ( m_eCompression == CT_DXT_1 )
        iOccupancy /= 2;
    rqBytes.SetQuantity(iOccupancy,false);

    // for a progress bar
    ProgressProbe qProgress(this);
    qProgress.SetAdvance(1.0f/iBlocksPerHeight);

    // encode image data
    const unsigned char* pucBasePix;
    const unsigned char* aucBlockRow;
    const unsigned char* aucPix;
    int aiAlphaForAssoc[16];
    int iRowStride = rspqRasterImage->GetRowStride();
    int iRowStrideM4 = 4*iRowStride;
    int iBytesPerPixel = rspqRasterImage->GetBytesPerPixel();
    int iBlockStride = 4*iBytesPerPixel;
    bool bOpaque = ( rspqRasterImage->GetType() == RasterImage::IT_RGB_8_8_8 );
    int iBytePos = 0;
    int iBitRatio = 0;
    if ( m_eCompression == CT_DXT_1 )
    {
        for (int i0 = 0; i0 < iBlocksPerHeight; i0++)
        {
            pucBasePix = (const unsigned char*)(rspqRasterImage->GetImage() + i0*iRowStrideM4);
            for (int i1 = 0; i1 < iBlocksPerWidth; i1++)
            {
                // write color components and indices
                WriteColorData(pucBasePix,iBytePos,iBitRatio,bOpaque,iBytesPerPixel,iRowStride,aiAlphaForAssoc,
                    rqBytes);

                pucBasePix += iBlockStride;
            }

            qProgress.MakeAdvance();
        }
    }
    else if ( m_eCompression == CT_DXT_2 || m_eCompression == CT_DXT_3 )
    {
        unsigned short usValue;
        int iI, iA;
        static const float s_f15Div255 = ((float)15)/255;
        for (int i0 = 0; i0 < iBlocksPerHeight; i0++)
        {
            pucBasePix = (const unsigned char*)(rspqRasterImage->GetImage() + i0*iRowStrideM4);
            for (int i1 = 0; i1 < iBlocksPerWidth; i1++)
            {
                // generate and write explicit alpha values
                aucBlockRow = pucBasePix;
                for (int i2 = 0; i2 < 4; i2++)
                {
                    aucPix = aucBlockRow + 12;

                    usValue = 0;
                    iI = i2*4 + 3;
                    for (int i3 = 0; i3 < 4; i3++)
                    {
                        iA = Mathf::RoundToInt(s_f15Div255*aucPix[3]);
                        usValue |= iA;
                        if ( i3 != 3 )
                            usValue <<= 4;

                        if ( m_eCompression == CT_DXT_2 )
                        {
                            // construct the alpha value to be involved into association
                            aiAlphaForAssoc[iI--] = (iA << 4) | (iA >> 4);
                        }

                        aucPix -= 4;
                    }
                    iBytePos += Storage::Write2le(rqBytes+iBytePos,1,&usValue);

                    aucBlockRow += iRowStride;
                }

                // write color components and indices
                WriteColorData(pucBasePix,iBytePos,iBitRatio,bOpaque,4,iRowStride,aiAlphaForAssoc,rqBytes);

                pucBasePix += iBlockStride;
            }

            qProgress.MakeAdvance();
        }
    }
    else  // m_eCompression = CT_DXT_4 or m_eCompression = CT_DXT_5
    {
        unsigned char aucAlphaBlock[16], aucAlpha[2], ucSave, aucSelAlpha[8];
        int iI, aiIndex[16];
        unsigned int uiValue;
        for (int i0 = 0; i0 < iBlocksPerHeight; i0++)
        {
            pucBasePix = (const unsigned char*)(rspqRasterImage->GetImage() + i0*iRowStrideM4);
            for (int i1 = 0; i1 < iBlocksPerWidth; i1++)
            {
                // read alphas from the image
                iI = 0;
                aucBlockRow = pucBasePix;
                for (int i2 = 0; i2 < 4; i2++)
                {
                    aucPix = aucBlockRow;
                    for (int i3 = 0; i3 < 4; i3++)
                    {
                        aucAlphaBlock[iI++] = aucPix[3];
                        aucPix += 4;
                    }

                    aucBlockRow += iRowStride;
                }

                // get to know if the block contains fully transparent or fully opaque pixels
                bool bHas0Alpha = false;
                bool bHas255Alpha = false;
                bool bHasInBetweenAlpha = false;
                for (int i2 = 0; i2 < 16; i2++)
                {
                    if ( aucAlphaBlock[i2] == 0 )
                        bHas0Alpha = true;
                    else if ( aucAlphaBlock[i2] == 255 )
                        bHas255Alpha = true;
                    else
                        bHasInBetweenAlpha = true;
                }
                bool bHasOutermostAlpha = ( bHas0Alpha || bHas255Alpha );

                if ( !bHasInBetweenAlpha && bHas0Alpha != bHas255Alpha )
                {
                    if ( bHas0Alpha )
                    {
                        // the block is fully transparent
                        for (int i2 = 0; i2 < 2; i2++)
                            aucAlpha[i2] = 0;
                    }
                    else  // bHas255Alpha = true
                    {
                        // the block is fully opaque
                        for (int i2 = 0; i2 < 2; i2++)
                            aucAlpha[i2] = 255;
                    }
                }
                else
                {
                    // get the extreme alpha values to represent the range of interpolated alphas
                    FindExtremeAlphasForBlock(aucAlphaBlock,bHasOutermostAlpha,aucAlpha);
                }

                if ( bHasOutermostAlpha != ( aucAlpha[0] < aucAlpha[1] ) )
                {
                    // perform swapping
                    ucSave = aucAlpha[0];
                    aucAlpha[0] = aucAlpha[1];
                    aucAlpha[1] = ucSave;
                }

                // get alpha indices for pixels in block
                FindAlphaIndicesForBlock(aucAlphaBlock,aucAlpha,aiIndex);

                // write extreme alpha values
                for (int i2 = 0; i2 < 2; i2++)
                    rqBytes[iBytePos++] = aucAlpha[i2];

                // write alpha indices
                for (int i2 = 0; i2 < 2; i2++)
                {
                    uiValue = 0;
                    for (int i3 = 0; i3 < 2; i3++)
                    {
                        iI = 8*i2 - 4*i3 + 7;
                        for (int i4 = 0; i4 < 4; i4++)
                        {
                            uiValue |= aiIndex[iI--];
                            if ( !(i3 == 1 && i4 == 3) )
                                uiValue <<= 3;
                        }
                    }

                    for (int i3 = 0; i3 < 3; i3++)
                    {
                        rqBytes[iBytePos++] = uiValue & 255;
                        uiValue >>= 8;
                    }
                }

                if ( m_eCompression == CT_DXT_4 )
                {
                    // construct alpha values to be involved into association
                    for (int i2 = 0; i2 < 2; i2++)
                        aucSelAlpha[i2] = aucAlpha[i2];
                    if ( aucSelAlpha[0] > aucSelAlpha[1] )
                    {
                        for (int i2 = 2; i2 < 8; i2++)
                            aucSelAlpha[i2] = ((8 - i2)*aucSelAlpha[0] + (i2 - 1)*aucSelAlpha[1] + 3)/7;
                    }
                    else  // aucSelAlpha[0] <= aucSelAlpha[1]
                    {
                        for (int i2 = 2; i2 < 6; i2++)
                            aucSelAlpha[i2] = ((6 - i2)*aucSelAlpha[0] + (i2 - 1)*aucSelAlpha[1] + 2)/5;
                        aucSelAlpha[6] = 0;
                        aucSelAlpha[7] = 255;
                    }
                    for (int i2 = 0; i2 < 16; i2++)
                        aiAlphaForAssoc[i2] = aucSelAlpha[aiIndex[i2]];
                }

                // write color components and indices
                WriteColorData(pucBasePix,iBytePos,iBitRatio,bOpaque,4,iRowStride,aiAlphaForAssoc,rqBytes);

                pucBasePix += iBlockStride;
            }

            qProgress.MakeAdvance();
        }
    }
}
//------------------------------------------------------------------------------------------------------------------
CompressionDxt::CompressionDxt (CompressionType eCompression, int iWidth, int iHeight)
    :
    m_eCompression(eCompression),
    m_iWidth(iWidth),
    m_iHeight(iHeight)
{
    assert( iWidth > 0 && iHeight > 0
        &&  (iWidth & 3) == 0 && (iHeight & 3) == 0 );
}
//------------------------------------------------------------------------------------------------------------------
void CompressionDxt::Decompress (const char* acByte, RasterImagePtr& rspqRasterImage)
{
    // allocate the output image
    rspqRasterImage = new RasterImage(RasterImage::IT_RGBA_8_8_8_8,m_iWidth,m_iHeight);

    int iBlocksPerWidth = m_iWidth/4;
    int iBlocksPerHeight = m_iHeight/4;

    // for a progress bar
    ProgressProbe qProgress(this);
    qProgress.SetAdvance(1.0f/iBlocksPerHeight);

    // decode image data
    int iBlockLTX, iBlockLTY = 0;
    int iBytePos = 0;
    int iBitRatio = 0;
    unsigned char* pucRow;
    unsigned char* aucPix;
    unsigned short usValue;
    unsigned int uiValue;
    if ( m_eCompression == CT_DXT_1 )
    {
        for (int i0 = 0; i0 < iBlocksPerHeight; i0++)
        {
            iBlockLTX = 0;
            for (int i1 = 0; i1 < iBlocksPerWidth; i1++)
            {
                // read and assign color components accordingly to indices
                ReadColorData(acByte,iBytePos,iBitRatio,rspqRasterImage,iBlockLTX,iBlockLTY);

                iBlockLTX += 4;
            }

            iBlockLTY += 4;

            qProgress.MakeAdvance();
        }
    }
    else if ( m_eCompression == CT_DXT_2 || m_eCompression == CT_DXT_3 )
    {
        unsigned int uiMaskRes;
        for (int i0 = 0; i0 < iBlocksPerHeight; i0++)
        {
            iBlockLTX = 0;
            for (int i1 = 0; i1 < iBlocksPerWidth; i1++)
            {
                // read and assign explicit alpha values
                pucRow = (unsigned char*)(
                    rspqRasterImage->GetImage() + iBlockLTY*rspqRasterImage->GetRowStride() + iBlockLTX*4);
                for (int i2 = 0; i2 < 4; i2++)
                {
                    aucPix = pucRow;
                    iBytePos += Storage::Read2le(acByte+iBytePos,1,&usValue);
                    for (int i3 = 0; i3 < 4; i3++)
                    {
                        uiMaskRes = usValue & 15;
                        aucPix[3] = (uiMaskRes << 4) | uiMaskRes;
                        usValue >>= 4;

                        aucPix += 4;
                    }

                    pucRow += rspqRasterImage->GetRowStride();
                }

                // read and assign color components accordingly to indices
                ReadColorData(acByte,iBytePos,iBitRatio,rspqRasterImage,iBlockLTX,iBlockLTY);

                iBlockLTX += 4;
            }

            iBlockLTY += 4;

            qProgress.MakeAdvance();
        }
    }
    else  // m_eCompression = CT_DXT_4 or m_eCompression = CT_DXT_5
    {
        unsigned char aucAlpha[8];
        unsigned char* aucAlphaByte;
        for (int i0 = 0; i0 < iBlocksPerHeight; i0++)
        {
            iBlockLTX = 0;
            for (int i1 = 0; i1 < iBlocksPerWidth; i1++)
            {
                for (int i2 = 0; i2 < 2; i2++)
                    aucAlpha[i2] = (unsigned char)acByte[iBytePos++];

                // find intermediate alphas
                if ( aucAlpha[0] > aucAlpha[1] )
                {
                    for (int i2 = 2; i2 < 8; i2++)
                        aucAlpha[i2] = ((8 - i2)*aucAlpha[0] + (i2 - 1)*aucAlpha[1] + 3)/7;
                }
                else  // aucAlpha[0] <= aucAlpha[1]
                {
                    for (int i2 = 2; i2 < 6; i2++)
                        aucAlpha[i2] = ((6 - i2)*aucAlpha[0] + (i2 - 1)*aucAlpha[1] + 2)/5;
                    aucAlpha[6] = 0;
                    aucAlpha[7] = 255;
                }

                // read and assign interpolated alpha values
                aucAlphaByte = (unsigned char*)acByte;
                pucRow = (unsigned char*)(
                    rspqRasterImage->GetImage() + iBlockLTY*rspqRasterImage->GetRowStride() + iBlockLTX*4);
                for (int i2 = 0; i2 < 2; i2++)
                {
                    uiValue = 0;
                    for (int i3 = 0; i3 < 24; i3 += 8)
                        uiValue |= aucAlphaByte[iBytePos++] << i3;
                    for (int i3 = 0; i3 < 2; i3++)
                    {
                        aucPix = pucRow;
                        for (int i4 = 0; i4 < 4; i4++)
                        {
                            aucPix[3] = aucAlpha[uiValue & 7];
                            uiValue >>= 3;

                            aucPix += 4;
                        }

                        pucRow += rspqRasterImage->GetRowStride();
                    }
                }

                // read and assign color components accordingly to indices
                ReadColorData(acByte,iBytePos,iBitRatio,rspqRasterImage,iBlockLTX,iBlockLTY);

                iBlockLTX += 4;
            }

            iBlockLTY += 4;

            qProgress.MakeAdvance();
        }
    }
}
//------------------------------------------------------------------------------------------------------------------
void CompressionDxt::OnWeightingFactorsUpdate (float fFcrR, float fFcrG, float fFcrB)
{
    // normalize weighting factors
    float fInvSum = 1.0f/(fFcrR + fFcrG + fFcrB);
    m_afWeightingFactor[0] = fFcrR*fInvSum;
    m_afWeightingFactor[1] = fFcrG*fInvSum;
    m_afWeightingFactor[2] = fFcrB*fInvSum;

    // convert them into integer form and do squaring (squared distances need squared coefficients)
    static const int s_iScale = 1 << 5;
    for (int i = 0; i < 3; i++)
        m_aiWeightingSqMult[i] = Mathi::Sqr(Mathf::RoundToInt(m_afWeightingFactor[i]*s_iScale));
}
//------------------------------------------------------------------------------------------------------------------
void CompressionDxt::FindExtremeColorsForBlock (const unsigned char aaucBlock[16][4], bool bDxt1a,
    unsigned short ausColor[2]) const
{
    if ( m_eCompressionQuality == CQT_LOW )
    {
        // The algorithm for this compression level chooses two colors from the provided block which are
        // furthest apart in 3D color space, taking into account weighting factors for each channel.  To
        // improve quality, the resulting segment is shrunken from both ends on the amount obtained by division
        // of segment's length by s_iInsetDiv value.
        static const int s_iInsetDiv = 12;

        // find colors which are furthest from each other
        const unsigned char* aucBColor0;
        const unsigned char* aucBColor1;
        int iMostBC0, iMostBC1, iSqDist, iMaxSqDist = Mathi::MIN_INT;
        for (int i0 = 0; i0 < 16; i0++)
        {
            aucBColor0 = aaucBlock[i0];
            for (int i1 = i0+1; i1 < 16; i1++)
            {
                aucBColor1 = aaucBlock[i1];

                iSqDist =
                    (aucBColor0[0] - aucBColor1[0])*(aucBColor0[0] - aucBColor1[0])*m_aiWeightingSqMult[0] +
                    (aucBColor0[1] - aucBColor1[1])*(aucBColor0[1] - aucBColor1[1])*m_aiWeightingSqMult[1] +
                    (aucBColor0[2] - aucBColor1[2])*(aucBColor0[2] - aucBColor1[2])*m_aiWeightingSqMult[2];
                if ( iSqDist > iMaxSqDist )
                {
                    iMaxSqDist = iSqDist;
                    iMostBC0 = i0;
                    iMostBC1 = i1;
                }
            }
        }

        // shrink
        unsigned char aaucColor[2][3];
        unsigned char* pucMin;
        unsigned char* pucMax;
        int iInsLen, iInsMin, iInsMax;
        for (int i = 0; i < 3; i++)
        {
            aaucColor[0][i] = aaucBlock[iMostBC0][i];
            aaucColor[1][i] = aaucBlock[iMostBC1][i];
        }
        for (int i = 0; i < 3; i++)
        {
            if ( aaucColor[0][i] < aaucColor[1][i] )
            {
                pucMin = &aaucColor[0][i];
                pucMax = &aaucColor[1][i];
            }
            else
            {
                pucMin = &aaucColor[1][i];
                pucMax = &aaucColor[0][i];
            }
            iInsLen = (*pucMax - *pucMin)/s_iInsetDiv;
            iInsMin = *pucMax - iInsLen;
            iInsMax = *pucMin + iInsLen;
            if ( iInsMin < 0 )
                iInsMin = 0;
            if ( iInsMax > 255 )
                iInsMax = 255;

            // it is possible with integer arithmetics that "min" will actually be greater than "max", but
            // there is no swapping
            *pucMin = iInsMin;
            *pucMax = iInsMax;
        }

        for (int i = 0; i < 2; i++)
        {
            ausColor[i] =
                ((aaucColor[i][0] >> 3) << 11) |
                ((aaucColor[i][1] >> 2) <<  5) |
                ((aaucColor[i][2] >> 3) <<  0);
        }
    }
    else  // m_eCompressionQuality = CQT_MEDIUM or m_eCompressionQuality = CQT_HIGH
    {
        // To produce acceptable quality of compression the multi-stage process is involved.  First, a provided
        // block of colors is examined to find out required level of diligence to be used in further
        // searchings.  Second, using Principal Components Analysis, the direction in 3D space along which
        // colors from the block vary the most is found, two colors are selected which have furthest apart
        // projections on this principal axis and after that they are moved to each other slightly, some number
        // of pairs of endpoints lying on the axis near these ones are stated and the most matching pair, that
        // is which causes the smallest error accordingly to weighting factors, is chosen.  Third, using this
        // starting points, several methods are mixed up to improve starting points for each next one.
        //
        // Next variables are shared between quality presettings.

        float fInsetFactor;         // e.g. value 0.95 shrinks "early" segment on 10%
        float fSearchRadToInsLen;   // radius of search is: this_value*segment_length
        int iRadTestPointDensity;   // number of test points per unit length

        bool bDoCrawl;
        int iCrawlMaxRepeats;

        // compiler helping constant
        static const int s_iMaxStageQuantity = 1;

        bool bDoLightNearbySearch;
        int iLightNearbySearchMaxRepeats;
        int iLightNearbySearchStages;
        float aafLightNearbySearchDev[s_iMaxStageQuantity][3];
        float afLightNearbySearchMaxDev[3];

        bool bDoHeavyNearbySearch;
        int iHeavyNearbySearchMaxRepeats;
        int iHeavyNearbySearchStages;
        float aafHeavyNearbySearchDev[s_iMaxStageQuantity][3];
        float afHeavyNearbySearchMaxDev[3];

        static const int s_ai565MaxValue[3] = {31, 63, 31};

        // Presettings.  The most significant ones are nearby search deviations (radiuses) which can be
        // controlled on per-component basis.  In contrast to maximal deviations, these constants do not
        // indicate values which will be actually used but rather origins to be affected by weighting factors
        // and some other parameters, thus floating-point representation is chosen.
        if ( m_eCompressionQuality == CQT_MEDIUM )
        {
            static const float s_fInsetFactor = 0.95f;
            static const float s_fSearchRadToInsLen = 0.05f;
            static const int s_iRadTestPointDensity = 3;

            static const bool s_bDoCrawl = false;
            static const int s_iCrawlMaxRepeats = 1 << 5;

            static const bool s_bDoLightNearbySearch = false;
            static const int s_iLightNearbySearchMaxRepeats = 1 << 2;
            static const int s_iLightNearbySearchStages = 1;
            static const float s_aafLightNearbySearchDev[s_iLightNearbySearchStages][3] = {
                {4.0f, 8.0f, 4.0f}};
            static const float s_afLightNearbySearchMaxDev[3] =
                {8.0f, 16.0f, 8.0f};

            static const bool s_bDoHeavyNearbySearch = false;
            static const int s_iHeavyNearbySearchMaxRepeats = 1 << 0;
            static const int s_iHeavyNearbySearchStages = 1;
            static const float s_aafHeavyNearbySearchDev[s_iHeavyNearbySearchStages][3] = {
                {0.5f, 1.0f, 0.5f}};
            static const float s_afHeavyNearbySearchMaxDev[3] =
                {1.0f, 2.0f, 1.0f};

            ASSIGN_CONTROL_VARIABLES;
        }
        else  // m_eCompressionQuality = CQT_HIGH
        {
            static const float s_fInsetFactor = 0.95f;
            static const float s_fSearchRadToInsLen = 0.05f;
            static const int s_iRadTestPointDensity = 3;

            static const bool s_bDoCrawl = true;
            static const int s_iCrawlMaxRepeats = 1 << 5;

            static const bool s_bDoLightNearbySearch = true;
            static const int s_iLightNearbySearchMaxRepeats = 1 << 2;
            static const int s_iLightNearbySearchStages = 1;
            static const float s_aafLightNearbySearchDev[s_iLightNearbySearchStages][3] = {
                {4.0f, 8.0f, 4.0f}};
            static const float s_afLightNearbySearchMaxDev[3] =
                {8.0f, 16.0f, 8.0f};

            static const bool s_bDoHeavyNearbySearch = true;
            static const int s_iHeavyNearbySearchMaxRepeats = 1 << 0;
            static const int s_iHeavyNearbySearchStages = 1;
            static const float s_aafHeavyNearbySearchDev[s_iHeavyNearbySearchStages][3] = {
                {0.5f, 1.0f, 0.5f}};
            static const float s_afHeavyNearbySearchMaxDev[3] =
                {1.0f, 2.0f, 1.0f};

            ASSIGN_CONTROL_VARIABLES;
        }

        // change deviations accordingly to weighting factors
        for (int i0 = 0; i0 < iLightNearbySearchStages; i0++)
        {
            for (int i1 = 0; i1 < 3; i1++)
                aafLightNearbySearchDev[i0][i1] *= m_afWeightingFactor[i1];
        }
        for (int i0 = 0; i0 < iHeavyNearbySearchStages; i0++)
        {
            for (int i1 = 0; i1 < 3; i1++)
                aafHeavyNearbySearchDev[i0][i1] *= m_afWeightingFactor[i1];
        }

        // Tune deviations knowing characteristics of the color block to be compressed.  The current technique
        // to estimate block's compressibility is based on calculation of softness of transitions within each
        // color component domain: the more rough transitions are, the higher deviation for this component
        // domain is required to a find good pair of endpoints.
        unsigned char aaucBlockD[3][15];
        unsigned char aucSrtComp[16];
        for (int i0 = 0; i0 < 3; i0++)
        {
            for (int i1 = 0; i1 < 16; i1++)
                aucSrtComp[i1] = aaucBlock[i1][i0];
            SortUtils<unsigned char>::Sort(16,aucSrtComp);
            for (int i1 = 0, i2 = 1; i2 < 16; i1 = i2++)
                aaucBlockD[i0][i1] = aucSrtComp[i2] - aucSrtComp[i1];
        }
        float afMean[3];
        static const float s_fInv15 = 1.0f/15;
        int iSum;
        for (int i0 = 0; i0 < 3; i0++)
        {
            iSum = 0;
            for (int i1 = 0; i1 < 15; i1++)
                iSum += aaucBlockD[i0][i1];
            afMean[i0] = ((float)iSum)*s_fInv15;
        }
        float afStdDev[3], fStdDev, fSub;
        for (int i0 = 0; i0 < 3; i0++)
        {
            fStdDev = 0.0f;
            for (int i1 = 0; i1 < 15; i1++)
            {
                fSub = (float)aaucBlockD[i0][i1] - afMean[i0];
                fStdDev += fSub*fSub;
            }

            afStdDev[i0] = Mathf::Sqrt(fStdDev*s_fInv15);
        }
        for (int i0 = 0; i0 < iLightNearbySearchStages; i0++)
        {
            for (int i1 = 0; i1 < 3; i1++)
                aafLightNearbySearchDev[i0][i1] *= afStdDev[i1];
        }
        for (int i0 = 0; i0 < iHeavyNearbySearchStages; i0++)
        {
            for (int i1 = 0; i1 < 3; i1++)
                aafHeavyNearbySearchDev[i0][i1] *= afStdDev[i1];
        }

        // cut off deviations if they exceed the stated limits
        for (int i0 = 0; i0 < iLightNearbySearchStages; i0++)
        {
            for (int i1 = 0; i1 < 3; i1++)
            {
                if ( aafLightNearbySearchDev[i0][i1] > afLightNearbySearchMaxDev[i1] )
                    aafLightNearbySearchDev[i0][i1] = afLightNearbySearchMaxDev[i1];
            }
        }
        for (int i0 = 0; i0 < iHeavyNearbySearchStages; i0++)
        {
            for (int i1 = 0; i1 < 3; i1++)
            {
                if ( aafHeavyNearbySearchDev[i0][i1] > afHeavyNearbySearchMaxDev[i1] )
                    aafHeavyNearbySearchDev[i0][i1] = afHeavyNearbySearchMaxDev[i1];
            }
        }

        // get deviations in integer form to be used by the loops
        int aaiLightNearbySearchDev[s_iMaxStageQuantity][3];
        int aaiHeavyNearbySearchDev[s_iMaxStageQuantity][3];
        for (int i0 = 0; i0 < iLightNearbySearchStages; i0++)
        {
            for (int i1 = 0; i1 < 3; i1++)
                aaiLightNearbySearchDev[i0][i1] = Mathf::RoundToInt(aafLightNearbySearchDev[i0][i1]);
        }
        for (int i0 = 0; i0 < iHeavyNearbySearchStages; i0++)
        {
            for (int i1 = 0; i1 < 3; i1++)
                aaiHeavyNearbySearchDev[i0][i1] = Mathf::RoundToInt(aafHeavyNearbySearchDev[i0][i1]);
        }

        // scan the color block for strictly equal colors just to omit in following routines computation of
        // values which are already computed
        int aiBColorToSimilar[16], aiMinSqDistForBColor[16];
        for (int i = 0; i < 16; i++)
            aiBColorToSimilar[i] = -1;
        const unsigned char* aucBColor0;
        const unsigned char* aucBColor1;
        bool bSimilar;
        for (int i0 = 1; i0 < 16; i0++)
        {
            aucBColor0 = aaucBlock[i0];
            for (int i1 = 0; i1 < i0; i1++)
            {
                aucBColor1 = aaucBlock[i1];

                bSimilar = true;
                for (int i2 = 0; i2 < 3; i2++)
                {
                    if ( aucBColor0[i2] != aucBColor1[i2] )
                    {
                        bSimilar = false;
                        break;
                    }
                }

                if ( bSimilar )
                {
                    aiBColorToSimilar[i0] = i1;
                    break;
                }
            }
        }

        // convert colors into floating-point 3D points (current approach is to use 31-63-31 space rather than
        // unit-dimensioned)
        Vector3f aqBPoint[16];
        static const float s_fInv255 = 1.0f/255;
        float afMult[3] = {
            s_fInv255*s_ai565MaxValue[0],
            s_fInv255*s_ai565MaxValue[1],
            s_fInv255*s_ai565MaxValue[2]};
        for (int i0 = 0; i0 < 16; i0++)
        {
            for (int i1 = 0; i1 < 3; i1++)
                aqBPoint[i0][i1] = ((float)aaucBlock[i0][i1])*afMult[i1];
        }

        // perform Principal Components Analysis on entire data set and get the feature vector
        PrincipalComponentsAnalysisf qPCA(3,16,false);
        for (int i0 = 0; i0 < 16; i0++)
        {
            for (int i1 = 0; i1 < 3; i1++)
                qPCA.SetValue(i1,i0,aqBPoint[i0][i1]);
        }
        qPCA.Analyze();
        Matrix3f qFeature;
        qPCA.GetFeatureVector(qFeature);

        // Transform color points by the transposed feature vector (a matrix in fact) making the block's
        // principal axis parallel to x-axis and passing through the transformed mean point.  This allows the
        // procedures going after to rely only on X component of points being analyzed.
        Matrix3f qFeatureT = qFeature.GetTransposed();
        Vector3f qMeanT = qFeatureT*Vector3f(
            qPCA.GetMean(0),
            qPCA.GetMean(1),
            qPCA.GetMean(2));
        Vector3f aqBPointT[16];
        for (int i = 0; i < 16; i++)
            aqBPointT[i] = qFeatureT*aqBPoint[i];

        // find the extreme color-points along the axis and change length of the resulting segment
        float fMin, fMax, fValue;
        fMin = fMax = aqBPointT[0].X();
        for (int i = 1; i < 16; i++)
        {
            fValue = aqBPointT[i].X();
            if ( fValue < fMin )
                fMin = fValue;
            else if ( fValue > fMax )
                fMax = fValue;
        }
        float fInsLen = fInsetFactor*(fMax - fMin);
        float fInsMin = fMax - fInsLen;
        float fInsMax = fMin + fInsLen;
        if ( fInsMin > fInsMax )
        {
            // swap
            float fSave = fInsMin;
            fInsMin = fInsMax;
            fInsMax = fSave;
        }
        float fSearchRad = fSearchRadToInsLen*fInsLen;
        float fRadSearchLen = 2.0f*fSearchRad;
        float fRadStart0 = fInsMin - fSearchRad;
        float fRadStart1 = fInsMax - fSearchRad;

        // near current endpoints make new ones accordingly to the radius stated and density; test these pairs
        // on produced error and update best-known endpoints
        int iRadTestPoints = Mathi::Max(1,Mathf::RoundToInt(fRadSearchLen*iRadTestPointDensity));
        StillArray<float> qAdds(iRadTestPoints+1);
        float fVar = fRadSearchLen/iRadTestPoints;
        for (int i = 0; i <= iRadTestPoints; i++)
            qAdds[i] = fVar*i;
        float afTestPoint[2], afMostTestPoint[2], fTestPoint2, fTestPoint3, fMinSqDist, fSqDist, fD, fBPProj,
            fMinDifference, fDifference;
        static const float s_fInv3 = 1.0f/3;
        fMinDifference = Mathf::MAX_REAL;
        for (int i0 = 0; i0 <= iRadTestPoints; i0++)
        {
            afTestPoint[0] = fRadStart0 + qAdds[i0];
            for (int i1 = 0; i1 <= iRadTestPoints; i1++)
            {
                afTestPoint[1] = fRadStart1 + qAdds[i1];

                if ( !bDxt1a )
                {
                    fTestPoint2 = (2.0f*afTestPoint[0] + afTestPoint[1])*s_fInv3;
                    fTestPoint3 = (afTestPoint[0] + 2.0f*afTestPoint[1])*s_fInv3;
                }
                else
                {
                    fTestPoint2 = (afTestPoint[0] + afTestPoint[1])*0.5f;
                }

                fDifference = 0.0f;
                for (int i2 = 0; i2 < 16; i2++)
                {
                    fBPProj = aqBPointT[i2].X();

                    fD = fBPProj - afTestPoint[0];
                    fMinSqDist = fD*fD;
                    fD = fBPProj - afTestPoint[1];
                    fSqDist = fD*fD;
                    if ( fSqDist < fMinSqDist )
                        fMinSqDist = fSqDist;
                    fD = fBPProj - fTestPoint2;
                    fSqDist = fD*fD;
                    if ( fSqDist < fMinSqDist )
                        fMinSqDist = fSqDist;
                    if ( !bDxt1a )
                    {
                        fD = fBPProj - fTestPoint3;
                        fSqDist = fD*fD;
                        if ( fSqDist < fMinSqDist )
                            fMinSqDist = fSqDist;
                    }

                    fDifference += fMinSqDist;
                    if ( fDifference >= fMinDifference )
                        break;
                }

                if ( fDifference < fMinDifference )
                {
                    fMinDifference = fDifference;
                    for (int i2 = 0; i2 < 2; i2++)
                        afMostTestPoint[i2] = afTestPoint[i2];
                }
            }
        }
        qAdds.RemoveAll();

        // transform best-known endpoints back and clamp their values if they are out; there is no need to
        // calculate the inverse matrix because the feature vector (a matrix in fact) is made up from
        // unit-length column vectors
        Vector3f aqMostTestPoint[2];
        for (int i0 = 0; i0 < 2; i0++)
        {
            Vector3f& rqMTP = aqMostTestPoint[i0];

            rqMTP.X() = afMostTestPoint[i0];
            rqMTP.Y() = qMeanT.Y();
            rqMTP.Z() = qMeanT.Z();

            rqMTP = qFeature*rqMTP;
            for (int i1 = 0; i1 < 3; i1++)
            {
                if ( rqMTP[i1] < 0.0f )
                    rqMTP[i1] = 0.0f;
                else if ( rqMTP[i1] > (float)s_ai565MaxValue[i1] )
                    rqMTP[i1] = (float)s_ai565MaxValue[i1];
            }
        }

        unsigned char aauc888Color[2][3], aaucSel888Color[4][3];
        int aaaiRng[2][3][2], iS, iE, aaiC[2][3], aaiMostC[2][3], iSqDist, iD, iDifference, iMinDifference,
            iMinSqDist, iPrevMinDifference0, iPrevMinDifference1;
        const unsigned char* aucBColor;
        const unsigned char* aucSColor;

        // get integer 5-6-5-bit precision values
        for (int i0 = 0; i0 < 2; i0++)
        {
            for (int i1 = 0; i1 < 3; i1++)
                aaiMostC[i0][i1] = Mathf::RoundToInt(aqMostTestPoint[i0][i1]);
        }

        // compute present difference (error) to be used in loop-exiting considerations
        iMinDifference = 0;
        for (int i = 0; i < 2; i++)
        {
            aauc888Color[i][0] = m_ai5To8[aaiMostC[i][0]];
            aauc888Color[i][1] = m_ai6To8[aaiMostC[i][1]];
            aauc888Color[i][2] = m_ai5To8[aaiMostC[i][2]];
        }
        for (int i = 0; i < 3; i++)
        {
            aaucSel888Color[0][i] = aauc888Color[0][i];
            aaucSel888Color[1][i] = aauc888Color[1][i];
            if ( !bDxt1a )
            {
                aaucSel888Color[2][i] = (2*aauc888Color[0][i] + aauc888Color[1][i] + 1)/3;
                aaucSel888Color[3][i] = (aauc888Color[0][i] + 2*aauc888Color[1][i] + 1)/3;
            }
            else
            {
                aaucSel888Color[2][i] = (aauc888Color[0][i] + aauc888Color[1][i])/2;
            }
        }
        int iSelCQ = ( !bDxt1a ? 4 : 3 );
        for (int i0 = 0; i0 < 16; i0++)
        {
            aucBColor = aaucBlock[i0];

            iMinSqDist = Mathi::MAX_INT;
            for (int i1 = 0; i1 < iSelCQ; i1++)
            {
                aucSColor = aaucSel888Color[i1];

                iSqDist = 0;
                for (int i2 = 0; i2 < 3; i2++)
                {
                    iD = aucBColor[i2] - aucSColor[i2];
                    iSqDist += iD*iD*m_aiWeightingSqMult[i2];
                }

                if ( iSqDist < iMinSqDist )
                    iMinSqDist = iSqDist;
            }

            iMinDifference += iMinSqDist;
        }

        // the "crawl"
        if ( bDoCrawl )
        {
            int aaaiCrawlDir[2][7][3], iPrevMinDifference, aaiPrecompSqDist[7][16];
            for (int i0 = 0; i0 < iCrawlMaxRepeats; i0++)
            {
                iPrevMinDifference = iMinDifference;

                for (int i1 = 0; i1 < 2; i1++)
                {
                    for (int i2 = 0; i2 < 7; i2++)
                    {
                        for (int i3 = 0; i3 < 3; i3++)
                            aaaiCrawlDir[i1][i2][i3] = aaiMostC[i1][i3];
                    }

                    // six moves to be tested
                    aaaiCrawlDir[i1][1][0] -= 1;
                    aaaiCrawlDir[i1][2][0] += 1;
                    aaaiCrawlDir[i1][3][1] -= 1;
                    aaaiCrawlDir[i1][4][1] += 1;
                    aaaiCrawlDir[i1][5][2] -= 1;
                    aaaiCrawlDir[i1][6][2] += 1;

                    // clamp to appropriate ranges
                    for (int i2 = 1; i2 < 7; i2++)
                    {
                        for (int i3 = 0; i3 < 3; i3++)
                        {
                            if ( aaaiCrawlDir[i1][i2][i3] < 0 )
                                aaaiCrawlDir[i1][i2][i3] = 0;
                            else if ( aaaiCrawlDir[i1][i2][i3] > s_ai565MaxValue[i3] )
                                aaaiCrawlDir[i1][i2][i3] = s_ai565MaxValue[i3];
                        }
                    }
                }

                // precompute values for the inner loop of following testing
                for (int i1 = 0; i1 < 7; i1++)
                {
                    aauc888Color[1][0] = m_ai5To8[aaaiCrawlDir[1][i1][0]];
                    aauc888Color[1][1] = m_ai6To8[aaaiCrawlDir[1][i1][1]];
                    aauc888Color[1][2] = m_ai5To8[aaaiCrawlDir[1][i1][2]];

                    for (int i2 = 0; i2 < 16; i2++)
                    {
                        aucBColor = aaucBlock[i2];

                        aaiPrecompSqDist[i1][i2] =
                            (aucBColor[0] - aauc888Color[1][0])*(aucBColor[0] - aauc888Color[1][0])*
                            m_aiWeightingSqMult[0] +
                            (aucBColor[1] - aauc888Color[1][1])*(aucBColor[1] - aauc888Color[1][1])*
                            m_aiWeightingSqMult[1] +
                            (aucBColor[2] - aauc888Color[1][2])*(aucBColor[2] - aauc888Color[1][2])*
                            m_aiWeightingSqMult[2];
                    }
                }

                // "crawl" testing
                for (int i1 = 0; i1 < 7; i1++)
                {
                    MemCpy(aaiC[0],aaaiCrawlDir[0][i1],sizeof(aaiC[0]));
                    aauc888Color[0][0] = m_ai5To8[aaaiCrawlDir[0][i1][0]];
                    aauc888Color[0][1] = m_ai6To8[aaaiCrawlDir[0][i1][1]];
                    aauc888Color[0][2] = m_ai5To8[aaaiCrawlDir[0][i1][2]];
                    for (int i2 = 0; i2 < 7; i2++)
                    {
                        if ( i1 == 0 && i2 == 0 )
                            continue;

                        MemCpy(aaiC[1],aaaiCrawlDir[1][i2],sizeof(aaiC[1]));
                        aauc888Color[1][0] = m_ai5To8[aaaiCrawlDir[1][i2][0]];
                        aauc888Color[1][1] = m_ai6To8[aaaiCrawlDir[1][i2][1]];
                        aauc888Color[1][2] = m_ai5To8[aaaiCrawlDir[1][i2][2]];

                        GEN_SEL_888_COLORS;

                        iDifference = 0;
                        for (int i3 = 0; i3 < 16; i3++)
                        {
                            if ( aiBColorToSimilar[i3] == -1 )
                            {
                                aucBColor = aaucBlock[i3];

                                aucSColor = aaucSel888Color[0];
                                iMinSqDist =
                                    (aucBColor[0] - aucSColor[0])*(aucBColor[0] - aucSColor[0])*
                                    m_aiWeightingSqMult[0] +
                                    (aucBColor[1] - aucSColor[1])*(aucBColor[1] - aucSColor[1])*
                                    m_aiWeightingSqMult[1] +
                                    (aucBColor[2] - aucSColor[2])*(aucBColor[2] - aucSColor[2])*
                                    m_aiWeightingSqMult[2];

                                iSqDist = aaiPrecompSqDist[i2][i3];
                                if ( iSqDist < iMinSqDist )
                                    iMinSqDist = iSqDist;

                                aucSColor = aaucSel888Color[2];
                                iSqDist =
                                    (aucBColor[0] - aucSColor[0])*(aucBColor[0] - aucSColor[0])*
                                    m_aiWeightingSqMult[0] +
                                    (aucBColor[1] - aucSColor[1])*(aucBColor[1] - aucSColor[1])*
                                    m_aiWeightingSqMult[1] +
                                    (aucBColor[2] - aucSColor[2])*(aucBColor[2] - aucSColor[2])*
                                    m_aiWeightingSqMult[2];
                                if ( iSqDist < iMinSqDist )
                                    iMinSqDist = iSqDist;

                                if ( !bDxt1a )
                                {
                                    aucSColor = aaucSel888Color[3];
                                    iSqDist =
                                        (aucBColor[0] - aucSColor[0])*(aucBColor[0] - aucSColor[0])*
                                        m_aiWeightingSqMult[0] +
                                        (aucBColor[1] - aucSColor[1])*(aucBColor[1] - aucSColor[1])*
                                        m_aiWeightingSqMult[1] +
                                        (aucBColor[2] - aucSColor[2])*(aucBColor[2] - aucSColor[2])*
                                        m_aiWeightingSqMult[2];
                                    if ( iSqDist < iMinSqDist )
                                        iMinSqDist = iSqDist;
                                }

                                aiMinSqDistForBColor[i3] = iMinSqDist;
                            }
                            else
                            {
                                iMinSqDist = aiMinSqDistForBColor[aiBColorToSimilar[i3]];
                            }

                            iDifference += iMinSqDist;
                            if ( iDifference >= iMinDifference )
                                break;
                        }

                        if ( iDifference < iMinDifference )
                        {
                            iMinDifference = iDifference;
                            MemCpy(aaiMostC,aaiC,sizeof(aaiMostC));
                        }
                    }
                }

                if ( iMinDifference == iPrevMinDifference )
                    break;
            }
        }

        // the "light nearby search"
        if ( bDoLightNearbySearch )
        {
            int aiPrecompSqDist[16];
            for (int i0 = 0; i0 < iLightNearbySearchMaxRepeats; i0++)
            {
                iPrevMinDifference0 = iMinDifference;

                for (int i1 = 0; i1 < iLightNearbySearchStages; i1++)
                {
                    iPrevMinDifference1 = iMinDifference;

                    // state search ranges
                    for (int i2 = 0; i2 < 2; i2++)
                    {
                        for (int i3 = 0; i3 < 3; i3++)
                        {
                            iS = aaiMostC[i2][i3] - aaiLightNearbySearchDev[i1][i3];
                            iE = aaiMostC[i2][i3] + aaiLightNearbySearchDev[i1][i3];
                            if ( iS < 0 )
                                iS = 0;
                            if ( iE > s_ai565MaxValue[i3] )
                                iE = s_ai565MaxValue[i3];
                            aaaiRng[i2][i3][0] = iS;
                            aaaiRng[i2][i3][1] = iE;
                        }
                    }

                    // try to find better positions moving only the first endpoint
                    MemCpy(aaiC[1],aaiMostC[1],sizeof(aaiC[1]));
                    aauc888Color[1][0] = m_ai5To8[aaiMostC[1][0]];
                    aauc888Color[1][1] = m_ai6To8[aaiMostC[1][1]];
                    aauc888Color[1][2] = m_ai5To8[aaiMostC[1][2]];
                    for (int i2 = 0; i2 < 16; i2++)
                    {
                        aucBColor = aaucBlock[i2];

                        iSqDist = 0;
                        for (int i3 = 0; i3 < 3; i3++)
                        {
                            iD = aucBColor[i3] - aauc888Color[1][i3];
                            iSqDist += iD*iD*m_aiWeightingSqMult[i3];
                        }

                        aiPrecompSqDist[i2] = iSqDist;
                    }
                    for (aaiC[0][0] = aaaiRng[0][0][0]; aaiC[0][0] <= aaaiRng[0][0][1]; aaiC[0][0]++)
                    {
                        aauc888Color[0][0] = m_ai5To8[aaiC[0][0]];
                        for (aaiC[0][1] = aaaiRng[0][1][0]; aaiC[0][1] <= aaaiRng[0][1][1]; aaiC[0][1]++)
                        {
                            aauc888Color[0][1] = m_ai6To8[aaiC[0][1]];
                            for (aaiC[0][2] = aaaiRng[0][2][0]; aaiC[0][2] <= aaaiRng[0][2][1]; aaiC[0][2]++)
                            {
                                aauc888Color[0][2] = m_ai5To8[aaiC[0][2]];

                                GEN_SEL_888_COLORS;

                                iDifference = 0;
                                for (int i2 = 0; i2 < 16; i2++)
                                {
                                    if ( aiBColorToSimilar[i2] == -1 )
                                    {
                                        aucBColor = aaucBlock[i2];

                                        aucSColor = aaucSel888Color[0];
                                        iMinSqDist =
                                            (aucBColor[0] - aucSColor[0])*(aucBColor[0] - aucSColor[0])*
                                            m_aiWeightingSqMult[0] +
                                            (aucBColor[1] - aucSColor[1])*(aucBColor[1] - aucSColor[1])*
                                            m_aiWeightingSqMult[1] +
                                            (aucBColor[2] - aucSColor[2])*(aucBColor[2] - aucSColor[2])*
                                            m_aiWeightingSqMult[2];

                                        iSqDist = aiPrecompSqDist[i2];
                                        if ( iSqDist < iMinSqDist )
                                            iMinSqDist = iSqDist;

                                        aucSColor = aaucSel888Color[2];
                                        iSqDist =
                                            (aucBColor[0] - aucSColor[0])*(aucBColor[0] - aucSColor[0])*
                                            m_aiWeightingSqMult[0] +
                                            (aucBColor[1] - aucSColor[1])*(aucBColor[1] - aucSColor[1])*
                                            m_aiWeightingSqMult[1] +
                                            (aucBColor[2] - aucSColor[2])*(aucBColor[2] - aucSColor[2])*
                                            m_aiWeightingSqMult[2];
                                        if ( iSqDist < iMinSqDist )
                                            iMinSqDist = iSqDist;

                                        if ( !bDxt1a )
                                        {
                                            aucSColor = aaucSel888Color[3];
                                            iSqDist =
                                                (aucBColor[0] - aucSColor[0])*(aucBColor[0] - aucSColor[0])*
                                                m_aiWeightingSqMult[0] +
                                                (aucBColor[1] - aucSColor[1])*(aucBColor[1] - aucSColor[1])*
                                                m_aiWeightingSqMult[1] +
                                                (aucBColor[2] - aucSColor[2])*(aucBColor[2] - aucSColor[2])*
                                                m_aiWeightingSqMult[2];
                                            if ( iSqDist < iMinSqDist )
                                                iMinSqDist = iSqDist;
                                        }

                                        aiMinSqDistForBColor[i2] = iMinSqDist;
                                    }
                                    else
                                    {
                                        iMinSqDist = aiMinSqDistForBColor[aiBColorToSimilar[i2]];
                                    }

                                    iDifference += iMinSqDist;
                                    if ( iDifference >= iMinDifference )
                                        break;
                                }

                                if ( iDifference < iMinDifference )
                                {
                                    iMinDifference = iDifference;
                                    MemCpy(aaiMostC,aaiC,sizeof(aaiMostC));
                                }
                            }
                        }
                    }

                    // try to find better positions moving only the second endpoint
                    MemCpy(aaiC[0],aaiMostC[0],sizeof(aaiC[0]));
                    aauc888Color[0][0] = m_ai5To8[aaiMostC[0][0]];
                    aauc888Color[0][1] = m_ai6To8[aaiMostC[0][1]];
                    aauc888Color[0][2] = m_ai5To8[aaiMostC[0][2]];
                    for (int i2 = 0; i2 < 16; i2++)
                    {
                        aucBColor = aaucBlock[i2];

                        iSqDist = 0;
                        for (int i3 = 0; i3 < 3; i3++)
                        {
                            iD = aucBColor[i3] - aauc888Color[0][i3];
                            iSqDist += iD*iD*m_aiWeightingSqMult[i3];
                        }

                        aiPrecompSqDist[i2] = iSqDist;
                    }
                    for (aaiC[1][0] = aaaiRng[1][0][0]; aaiC[1][0] <= aaaiRng[1][0][1]; aaiC[1][0]++)
                    {
                        aauc888Color[1][0] = m_ai5To8[aaiC[1][0]];
                        for (aaiC[1][1] = aaaiRng[1][1][0]; aaiC[1][1] <= aaaiRng[1][1][1]; aaiC[1][1]++)
                        {
                            aauc888Color[1][1] = m_ai6To8[aaiC[1][1]];
                            for (aaiC[1][2] = aaaiRng[1][2][0]; aaiC[1][2] <= aaaiRng[1][2][1]; aaiC[1][2]++)
                            {
                                aauc888Color[1][2] = m_ai5To8[aaiC[1][2]];

                                GEN_SEL_888_COLORS;

                                iDifference = 0;
                                for (int i2 = 0; i2 < 16; i2++)
                                {
                                    if ( aiBColorToSimilar[i2] == -1 )
                                    {
                                        aucBColor = aaucBlock[i2];

                                        iMinSqDist = aiPrecompSqDist[i2];

                                        aucSColor = aaucSel888Color[1];
                                        iSqDist =
                                            (aucBColor[0] - aucSColor[0])*(aucBColor[0] - aucSColor[0])*
                                            m_aiWeightingSqMult[0] +
                                            (aucBColor[1] - aucSColor[1])*(aucBColor[1] - aucSColor[1])*
                                            m_aiWeightingSqMult[1] +
                                            (aucBColor[2] - aucSColor[2])*(aucBColor[2] - aucSColor[2])*
                                            m_aiWeightingSqMult[2];
                                        if ( iSqDist < iMinSqDist )
                                            iMinSqDist = iSqDist;

                                        aucSColor = aaucSel888Color[2];
                                        iSqDist =
                                            (aucBColor[0] - aucSColor[0])*(aucBColor[0] - aucSColor[0])*
                                            m_aiWeightingSqMult[0] +
                                            (aucBColor[1] - aucSColor[1])*(aucBColor[1] - aucSColor[1])*
                                            m_aiWeightingSqMult[1] +
                                            (aucBColor[2] - aucSColor[2])*(aucBColor[2] - aucSColor[2])*
                                            m_aiWeightingSqMult[2];
                                        if ( iSqDist < iMinSqDist )
                                            iMinSqDist = iSqDist;

                                        if ( !bDxt1a )
                                        {
                                            aucSColor = aaucSel888Color[3];
                                            iSqDist =
                                                (aucBColor[0] - aucSColor[0])*(aucBColor[0] - aucSColor[0])*
                                                m_aiWeightingSqMult[0] +
                                                (aucBColor[1] - aucSColor[1])*(aucBColor[1] - aucSColor[1])*
                                                m_aiWeightingSqMult[1] +
                                                (aucBColor[2] - aucSColor[2])*(aucBColor[2] - aucSColor[2])*
                                                m_aiWeightingSqMult[2];
                                            if ( iSqDist < iMinSqDist )
                                                iMinSqDist = iSqDist;
                                        }

                                        aiMinSqDistForBColor[i2] = iMinSqDist;
                                    }
                                    else
                                    {
                                        iMinSqDist = aiMinSqDistForBColor[aiBColorToSimilar[i2]];
                                    }

                                    iDifference += iMinSqDist;
                                    if ( iDifference >= iMinDifference )
                                        break;
                                }

                                if ( iDifference < iMinDifference )
                                {
                                    iMinDifference = iDifference;
                                    MemCpy(aaiMostC,aaiC,sizeof(aaiMostC));
                                }
                            }
                        }
                    }

                    if ( iMinDifference == iPrevMinDifference1 )
                        break;
                }

                if ( iMinDifference == iPrevMinDifference0 )
                    break;
            }
        }

        // the "heavy nearby search"
        if ( bDoHeavyNearbySearch )
        {
            for (int i0 = 0; i0 < iHeavyNearbySearchMaxRepeats; i0++)
            {
                iPrevMinDifference0 = iMinDifference;

                for (int i1 = 0; i1 < iHeavyNearbySearchStages; i1++)
                {
                    iPrevMinDifference1 = iMinDifference;

                    // state search ranges
                    for (int i2 = 0; i2 < 2; i2++)
                    {
                        for (int i3 = 0; i3 < 3; i3++)
                        {
                            iS = aaiMostC[i2][i3] - aaiHeavyNearbySearchDev[i1][i3];
                            iE = aaiMostC[i2][i3] + aaiHeavyNearbySearchDev[i1][i3];
                            if ( iS < 0 )
                                iS = 0;
                            if ( iE > s_ai565MaxValue[i3] )
                                iE = s_ai565MaxValue[i3];
                            aaaiRng[i2][i3][0] = iS;
                            aaaiRng[i2][i3][1] = iE;
                        }
                    }

                    // try to find better positions moving both endpoints
                    for (aaiC[0][0] = aaaiRng[0][0][0];
                         aaiC[0][0] <= aaaiRng[0][0][1];
                         aaiC[0][0]++)
                    {
                        aauc888Color[0][0] = m_ai5To8[aaiC[0][0]];
                        for (aaiC[0][1] = aaaiRng[0][1][0];
                             aaiC[0][1] <= aaaiRng[0][1][1];
                             aaiC[0][1]++)
                        {
                            aauc888Color[0][1] = m_ai6To8[aaiC[0][1]];
                            for (aaiC[0][2] = aaaiRng[0][2][0];
                                 aaiC[0][2] <= aaaiRng[0][2][1];
                                 aaiC[0][2]++)
                            {
                                aauc888Color[0][2] = m_ai5To8[aaiC[0][2]];
                                for (aaiC[1][0] = aaaiRng[1][0][0];
                                     aaiC[1][0] <= aaaiRng[1][0][1];
                                     aaiC[1][0]++)
                                {
                                    aauc888Color[1][0] = m_ai5To8[aaiC[1][0]];
                                    for (aaiC[1][1] = aaaiRng[1][1][0];
                                         aaiC[1][1] <= aaaiRng[1][1][1];
                                         aaiC[1][1]++)
                                    {
                                        aauc888Color[1][1] = m_ai6To8[aaiC[1][1]];
                                        for (aaiC[1][2] = aaaiRng[1][2][0];
                                             aaiC[1][2] <= aaaiRng[1][2][1];
                                             aaiC[1][2]++)
                                        {
                                            aauc888Color[1][2] = m_ai5To8[aaiC[1][2]];

                                            GEN_SEL_888_COLORS;

                                            iDifference = 0;
                                            for (int i2 = 0; i2 < 16; i2++)
                                            {
                                                if ( aiBColorToSimilar[i2] == -1 )
                                                {
                                                    aucBColor = aaucBlock[i2];

                                                    aucSColor = aaucSel888Color[0];
                                                    iMinSqDist =
                                                        (aucBColor[0] - aucSColor[0])*
                                                        (aucBColor[0] - aucSColor[0])*m_aiWeightingSqMult[0] +
                                                        (aucBColor[1] - aucSColor[1])*
                                                        (aucBColor[1] - aucSColor[1])*m_aiWeightingSqMult[1] +
                                                        (aucBColor[2] - aucSColor[2])*
                                                        (aucBColor[2] - aucSColor[2])*m_aiWeightingSqMult[2];

                                                    aucSColor = aaucSel888Color[1];
                                                    iSqDist =
                                                        (aucBColor[0] - aucSColor[0])*
                                                        (aucBColor[0] - aucSColor[0])*m_aiWeightingSqMult[0] +
                                                        (aucBColor[1] - aucSColor[1])*
                                                        (aucBColor[1] - aucSColor[1])*m_aiWeightingSqMult[1] +
                                                        (aucBColor[2] - aucSColor[2])*
                                                        (aucBColor[2] - aucSColor[2])*m_aiWeightingSqMult[2];
                                                    if ( iSqDist < iMinSqDist )
                                                        iMinSqDist = iSqDist;

                                                    aucSColor = aaucSel888Color[2];
                                                    iSqDist =
                                                        (aucBColor[0] - aucSColor[0])*
                                                        (aucBColor[0] - aucSColor[0])*m_aiWeightingSqMult[0] +
                                                        (aucBColor[1] - aucSColor[1])*
                                                        (aucBColor[1] - aucSColor[1])*m_aiWeightingSqMult[1] +
                                                        (aucBColor[2] - aucSColor[2])*
                                                        (aucBColor[2] - aucSColor[2])*m_aiWeightingSqMult[2];
                                                    if ( iSqDist < iMinSqDist )
                                                        iMinSqDist = iSqDist;

                                                    if ( !bDxt1a )
                                                    {
                                                        aucSColor = aaucSel888Color[3];
                                                        iSqDist =
                                                            (aucBColor[0] - aucSColor[0])*
                                                            (aucBColor[0] - aucSColor[0])*m_aiWeightingSqMult[0] +
                                                            (aucBColor[1] - aucSColor[1])*
                                                            (aucBColor[1] - aucSColor[1])*m_aiWeightingSqMult[1] +
                                                            (aucBColor[2] - aucSColor[2])*
                                                            (aucBColor[2] - aucSColor[2])*m_aiWeightingSqMult[2];
                                                        if ( iSqDist < iMinSqDist )
                                                            iMinSqDist = iSqDist;
                                                    }

                                                    aiMinSqDistForBColor[i2] = iMinSqDist;
                                                }
                                                else
                                                {
                                                    iMinSqDist = aiMinSqDistForBColor[aiBColorToSimilar[i2]];
                                                }

                                                iDifference += iMinSqDist;
                                                if ( iDifference >= iMinDifference )
                                                    break;
                                            }

                                            if ( iDifference < iMinDifference )
                                            {
                                                iMinDifference = iDifference;
                                                MemCpy(aaiMostC,aaiC,sizeof(aaiMostC));
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }

                    if ( iMinDifference == iPrevMinDifference1 )
                        break;
                }

                if ( iMinDifference == iPrevMinDifference0 )
                    break;
            }
        }

        // pack best-known endpoints into two shorts
        for (int i = 0; i < 2; i++)
        {
            ausColor[i] =
                (aaiMostC[i][0] << 11) |
                (aaiMostC[i][1] <<  5) |
                (aaiMostC[i][2] <<  0);
        }
    }
}
//------------------------------------------------------------------------------------------------------------------
void CompressionDxt::FindExtremeAlphasForBlock (const unsigned char aucBlock[16], bool bHasOutermostAlpha,
    unsigned char aucAlpha[2]) const
{
    // All compression quality settings have the same initial stage in finding of extreme alpha values: to get
    // some starting points, the minimal and maximal alphas are found and moved to each other on the amount
    // obtained by division of distance between them by s_iInsetDiv constant.
    static const int s_iInsetDiv = 24;

    // get the minimal and maximal alphas and shrink the resulting interval
    unsigned char ucMin, ucMax;
    ucMin = ucMax = aucBlock[0];
    for (int i = 1; i < 16; i++)
    {
        if ( aucBlock[i] < ucMin )
            ucMin = aucBlock[i];
        else if ( aucBlock[i] > ucMax )
            ucMax = aucBlock[i];
    }
    int iInsLen = (ucMax - ucMin)/s_iInsetDiv;
    int iInsMin = ucMax - iInsLen;
    int iInsMax = ucMin + iInsLen;
    if ( iInsMin < 0 )
        iInsMin = 0;
    if ( iInsMax > 255 )
        iInsMax = 255;

    if ( m_eCompressionQuality == CQT_LOW )
    {
        // present information is enough for this quality setting
        aucAlpha[0] = iInsMin;
        aucAlpha[1] = iInsMax;
    }
    else  // m_eCompressionQuality = CQT_MEDIUM or m_eCompressionQuality = CQT_HIGH
    {
        // radius of search around each of two currently best-known extreme alphas
        static const int s_iNearbySearchDev = 2;

        if ( iInsMin > iInsMax )
        {
            // it's possible with integer arithmetic, so do swapping
            int iSave = iInsMin;
            iInsMin = iInsMax;
            iInsMax = iSave;
        }
        int aiMostA[2] = {iInsMin, iInsMax};

        // calculate initial minimal difference
        int iMinDist, iDist, iMinDifference = 0;
        unsigned char aucSelAlpha[8];
        for (int i = 0; i < 2; i++)
            aucSelAlpha[i] = aiMostA[i];
        if ( !bHasOutermostAlpha )
        {
            for (int i = 2; i < 8; i++)
                aucSelAlpha[i] = ((8 - i)*aucSelAlpha[0] + (i - 1)*aucSelAlpha[1] + 3)/7;
        }
        else
        {
            for (int i = 2; i < 6; i++)
                aucSelAlpha[i] = ((6 - i)*aucSelAlpha[0] + (i - 1)*aucSelAlpha[1] + 2)/5;
            aucSelAlpha[6] = 0;
            aucSelAlpha[7] = 255;
        }
        int iBAlpha;
        for (int i0 = 0; i0 < 16; i0++)
        {
            iBAlpha = aucBlock[i0];

            iMinDist = Mathi::MAX_INT;
            for (int i1 = 0; i1 < 8; i1++)
            {
                iDist = Mathi::Abs(iBAlpha-aucSelAlpha[i1]);
                if ( iDist < iMinDist )
                    iMinDist = iDist;
            }

            iMinDifference += iMinDist;
        }

        // state search ranges
        int iS, iE, aaiRng[2][2];
        for (int i = 0; i < 2; i++)
        {
            iS = aiMostA[i] - s_iNearbySearchDev;
            iE = aiMostA[i] + s_iNearbySearchDev;
            if ( iS < 0 )
                iS = 0;
            if ( iE > 255 )
                iE = 255;
            aaiRng[i][0] = iS;
            aaiRng[i][1] = iE;
        }

        // contest via testing combinations of endpoint positions on resulting error
        int aiA[2];
        int iDifference;
        for (aiA[0] = aaiRng[0][0]; aiA[0] <= aaiRng[0][1]; aiA[0]++)
        {
            for (aiA[1] = aaiRng[1][0]; aiA[1] <= aaiRng[1][1]; aiA[1]++)
            {
                // alphas to make selection from
                for (int i = 0; i < 2; i++)
                    aucSelAlpha[i] = aiA[i];
                if ( !bHasOutermostAlpha )
                {
                    for (int i = 2; i < 8; i++)
                        aucSelAlpha[i] = ((8 - i)*aucSelAlpha[0] + (i - 1)*aucSelAlpha[1] + 3)/7;
                }
                else
                {
                    for (int i = 2; i < 6; i++)
                        aucSelAlpha[i] = ((6 - i)*aucSelAlpha[0] + (i - 1)*aucSelAlpha[1] + 2)/5;
                    aucSelAlpha[6] = 0;
                    aucSelAlpha[7] = 255;
                }

                iDifference = 0;
                for (int i0 = 0; i0 < 16; i0++)
                {
                    iBAlpha = aucBlock[i0];

                    iMinDist = Mathi::MAX_INT;
                    for (int i1 = 0; i1 < 8; i1++)
                    {
                        iDist = Mathi::Abs(iBAlpha-aucSelAlpha[i1]);
                        if ( iDist < iMinDist )
                            iMinDist = iDist;
                    }

                    iDifference += iMinDist;
                    if ( iDifference >= iMinDifference )
                        break;
                }

                if ( iDifference < iMinDifference )
                {
                    iMinDifference = iDifference;
                    MemCpy(aiMostA,aiA,sizeof(aiMostA));
                }
            }
        }

        for (int i = 0; i < 2; i++)
            aucAlpha[i] = aiMostA[i];
    }
}
//------------------------------------------------------------------------------------------------------------------
void CompressionDxt::FindColorIndicesForBlock (const unsigned char aaucBlock[16][4],
    const unsigned short ausColor[2], int aiIndex[16]) const
{
    unsigned char aaucColor[2][3];
    for (int i = 0; i < 2; i++)
    {
        aaucColor[i][0] = (ausColor[i] & 63488) >> 8;
        aaucColor[i][0] |= aaucColor[i][0] >> 5;
        aaucColor[i][1] = (ausColor[i] & 2016) >> 3;
        aaucColor[i][1] |= aaucColor[i][1] >> 6;
        aaucColor[i][2] = (ausColor[i] & 31) << 3;
        aaucColor[i][2] |= aaucColor[i][2] >> 5;
    }

    unsigned char aaucSelColor[4][3];
    const unsigned char* aucBColor;
    const unsigned char* aucSColor;
    int iMostSC, iSqDist, iD, iMinSqDist;
    if ( !(m_eCompression == CT_DXT_1 && ausColor[0] <= ausColor[1]) )
    {
        // opaque DXT1 or DXT2/3/4/5
        for (int i = 0; i < 3; i++)
        {
            aaucSelColor[0][i] = aaucColor[0][i];
            aaucSelColor[1][i] = aaucColor[1][i];
            aaucSelColor[2][i] = (2*aaucColor[0][i] + aaucColor[1][i] + 1)/3;
            aaucSelColor[3][i] = (aaucColor[0][i] + 2*aaucColor[1][i] + 1)/3;
        }

        for (int i0 = 0; i0 < 16; i0++)
        {
            aucBColor = aaucBlock[i0];

            iMinSqDist = Mathi::MAX_INT;
            for (int i1 = 0; i1 < 4; i1++)
            {
                aucSColor = aaucSelColor[i1];

                iSqDist = 0;
                for (int i2 = 0; i2 < 3; i2++)
                {
                    iD = aucBColor[i2] - aucSColor[i2];
                    iSqDist += iD*iD*m_aiWeightingSqMult[i2];
                }
                if ( iSqDist < iMinSqDist )
                {
                    iMinSqDist = iSqDist;
                    iMostSC = i1;
                }
            }

            aiIndex[i0] = iMostSC;
        }
    }
    else
    {
        // DXT1 with 0/1 alpha
        for (int i = 0; i < 3; i++)
        {
            aaucSelColor[0][i] = aaucColor[0][i];
            aaucSelColor[1][i] = aaucColor[1][i];
            aaucSelColor[2][i] = (aaucColor[0][i] + aaucColor[1][i])/2;
        }

        for (int i0 = 0; i0 < 16; i0++)
        {
            aucBColor = aaucBlock[i0];

            if ( aucBColor[3] < m_iDxt1aThreshold )
            {
                aiIndex[i0] = 3;
            }
            else
            {
                if ( ausColor[0] != ausColor[1] )
                {
                    iMinSqDist = Mathi::MAX_INT;
                    for (int i1 = 0; i1 < 3; i1++)
                    {
                        aucSColor = aaucSelColor[i1];

                        iSqDist = 0;
                        for (int i2 = 0; i2 < 3; i2++)
                        {
                            iD = aucBColor[i2] - aucSColor[i2];
                            iSqDist += iD*iD*m_aiWeightingSqMult[i2];
                        }
                        if ( iSqDist < iMinSqDist )
                        {
                            iMinSqDist = iSqDist;
                            iMostSC = i1;
                        }
                    }
                    aiIndex[i0] = iMostSC;
                }
                else  // ausColor[0] = ausColor[1]
                {
                    // let it be the special case
                    aiIndex[i0] = 0;
                }
            }
        }
    }
}
//------------------------------------------------------------------------------------------------------------------
void CompressionDxt::FindAlphaIndicesForBlock (const unsigned char aucBlock[16], const unsigned char aucAlpha[2],
    int aiIndex[16]) const
{
    if ( aucAlpha[0] != aucAlpha[1] )
    {
        // alphas to select from
        unsigned char aucSelAlpha[8];
        for (int i = 0; i < 2; i++)
            aucSelAlpha[i] = aucAlpha[i];
        if ( aucSelAlpha[0] > aucSelAlpha[1] )
        {
            for (int i = 2; i < 8; i++)
                aucSelAlpha[i] = ((8 - i)*aucSelAlpha[0] + (i - 1)*aucSelAlpha[1] + 3)/7;
        }
        else  // aucSelAlpha[0] <= aucSelAlpha[1]
        {
            for (int i = 2; i < 6; i++)
                aucSelAlpha[i] = ((6 - i)*aucSelAlpha[0] + (i - 1)*aucSelAlpha[1] + 2)/5;
            aucSelAlpha[6] = 0;
            aucSelAlpha[7] = 255;
        }

        // contest
        int iA, iMinDist, iDist, iMostA;
        for (int i0 = 0; i0 < 16; i0++)
        {
            iA = aucBlock[i0];

            iMinDist = Mathi::MAX_INT;
            for (int i1 = 0; i1 < 8; i1++)
            {
                iDist = Mathi::Abs(iA-aucSelAlpha[i1]);
                if ( iDist < iMinDist )
                {
                    iMinDist = iDist;
                    iMostA = i1;
                }
            }

            aiIndex[i0] = iMostA;
        }
    }
    else  // aucAlpha[0] = aucAlpha[1]
    {
        // let it be the special case
        for (int i = 0; i < 16; i++)
            aiIndex[i] = 0;
    }
}
//------------------------------------------------------------------------------------------------------------------
void CompressionDxt::WriteColorData (const unsigned char* pucBasePix, int& riBytePos, int& riBitRatio,
    bool bOpaque, int iBytesPerPixel, int iRowStride, const int aiAlphaForAssoc[16],
    StillArray<char>& rqBytes) const
{
    // form the block buffer
    unsigned char aaucBlock[16][4];
    unsigned char* aucBColor;
    const unsigned char* aucBlockRow = pucBasePix;
    const unsigned char* aucPix = aucBlockRow;
    int iBColor = 0;
    bool bDxt1a = false;
    if ( m_eCompression == CT_DXT_1 )
    {
        for (int i0 = 0; i0 < 4; i0++)
        {
            aucPix = aucBlockRow;
            for (int i1 = 0; i1 < 4; i1++)
            {
                aucBColor = aaucBlock[iBColor++];

                MemCpy(aucBColor,aucPix,3);
                aucBColor[3] = ( bOpaque ? 255 : aucPix[3] );
                if ( aucBColor[3] < m_iDxt1aThreshold )
                    bDxt1a = true;

                aucPix += iBytesPerPixel;
            }

            aucBlockRow += iRowStride;
        }
    }
    else  // m_eCompression = CT_DXT_2 or CT_DXT_3 or CT_DXT_4 or CT_DXT_5
    {
        static const float s_fInv255 = 1.0f/255;
        float fA;
        int iI = 0;
        for (int i0 = 0; i0 < 4; i0++)
        {
            aucPix = aucBlockRow;
            for (int i1 = 0; i1 < 4; i1++)
            {
                aucBColor = aaucBlock[iBColor++];

                MemCpy(aucBColor,aucPix,4);
                aucPix += 4;

                if ( m_eCompression == CT_DXT_2
                ||   m_eCompression == CT_DXT_4 )
                {
                    // associate the alpha with color components
                    fA = s_fInv255*aiAlphaForAssoc[iI++];
                    for (int i2 = 0; i2 < 3; i2++)
                        aucBColor[i2] = Mathf::RoundToInt(fA*aucBColor[i2]);
                }
            }

            aucBlockRow += iRowStride;
        }
    }

    // corresponding to the quality settings, get the endpoints of the fit line in color space
    unsigned short ausColor[2];
    FindExtremeColorsForBlock(aaucBlock,bDxt1a,ausColor);

    // the second line of this conditional statement is just to assure correct reading of compressed DXT data
    // by some out-of-specification designed decoders: they require color0 > color1 for DXT2/3/4/5 types of
    // compression
    if ( (m_eCompression == CT_DXT_1 && bDxt1a != ( ausColor[0] < ausColor[1] ))
    ||   (m_eCompression != CT_DXT_1 && ausColor[0] < ausColor[1]) )
    {
        // perform swapping
        unsigned short usSave = ausColor[0];
        ausColor[0] = ausColor[1];
        ausColor[1] = usSave;
    }

    // choose from color0 and color1 which matches best for each color in the block
    int aiIndex[16];
    FindColorIndicesForBlock(aaucBlock,ausColor,aiIndex);

    // write the extreme colors
    for (int i = 0; i < 2; i++)
        riBytePos += Storage::Write2le(rqBytes+riBytePos,1,&ausColor[i]);

    // write indices
    int iI;
    for (int i0 = 0; i0 < 4; i0++)
    {
        iI = i0*4 + 3;
        for (int i1 = 0; i1 < 4; i1++)
            riBytePos += Storage::WriteBits(rqBytes+riBytePos,riBitRatio,2,aiIndex[iI--]);
    }
}
//------------------------------------------------------------------------------------------------------------------
void CompressionDxt::ReadColorData (const char* acByte, int& riBytePos, int& riBitRatio,
    RasterImagePtr& rspqRasterImage, int iBlockLTX, int iBlockLTY) const
{
    // read the extreme colors
    unsigned short ausColor[2];
    for (int i = 0; i < 2; i++)
        riBytePos += Storage::Read2le(acByte+riBytePos,1,&ausColor[i]);

    // get the extreme colors in RGBA representation
    unsigned char aauc8888Color[4][4];
    for (int i = 0; i < 2; i++)
    {
        aauc8888Color[i][0] = (ausColor[i] & 63488) >> 8;
        aauc8888Color[i][0] |= aauc8888Color[i][0] >> 5;
        aauc8888Color[i][1] = (ausColor[i] & 2016) >> 3;
        aauc8888Color[i][1] |= aauc8888Color[i][1] >> 6;
        aauc8888Color[i][2] = (ausColor[i] & 31) << 3;
        aauc8888Color[i][2] |= aauc8888Color[i][2] >> 5;
        if ( m_eCompression == CT_DXT_1 )
            aauc8888Color[i][3] = 255;
    }

    unsigned char* pucRow;
    unsigned char* aucPix;
    int iValue;
    pucRow = (unsigned char*)(
        rspqRasterImage->GetImage() + iBlockLTY*rspqRasterImage->GetRowStride() + iBlockLTX*4 + 12);
    if ( m_eCompression == CT_DXT_1 )
    {
        if ( ausColor[0] > ausColor[1] )
        {
            // opaque block; find intermediate colors
            for (int i = 0; i < 3; i++)
                aauc8888Color[2][i] = (2*aauc8888Color[0][i] + aauc8888Color[1][i] + 1)/3;
            aauc8888Color[2][3] = 255;
            for (int i = 0; i < 3; i++)
                aauc8888Color[3][i] = (aauc8888Color[0][i] + 2*aauc8888Color[1][i] + 1)/3;
            aauc8888Color[3][3] = 255;
        }
        else  // ausColor[0] <= ausColor[1]
        {
            // some pixels within the block are fully transparent; find single intermediate color
            for (int i = 0; i < 3; i++)
                aauc8888Color[2][i] = (aauc8888Color[0][i] + aauc8888Color[1][i])/2;
            aauc8888Color[2][3] = 255;
            for (int i = 0; i < 4; i++)
                aauc8888Color[3][i] = 0;
        }

        // assign pixels
        for (int i0 = 0; i0 < 4; i0++)
        {
            aucPix = pucRow;
            for (int i1 = 0; i1 < 4; i1++)
            {
                riBytePos += Storage::ReadUBits(acByte+riBytePos,riBitRatio,2,iValue);
                MemCpy(aucPix,aauc8888Color[iValue],4);

                aucPix -= 4;
            }

            pucRow += rspqRasterImage->GetRowStride();
        }
    }
    else  // m_eCompression = CT_DXT_2 or CT_DXT_3 or CT_DXT_4 or CT_DXT_5
    {
        // find intermediate colors
        for (int i = 0; i < 3; i++)
            aauc8888Color[2][i] = (2*aauc8888Color[0][i] + aauc8888Color[1][i] + 1)/3;
        for (int i = 0; i < 3; i++)
            aauc8888Color[3][i] = (aauc8888Color[0][i] + 2*aauc8888Color[1][i] + 1)/3;

        // assign pixels
        float fInvA;
        int iValue;
        for (int i0 = 0; i0 < 4; i0++)
        {
            aucPix = pucRow;
            for (int i1 = 0; i1 < 4; i1++)
            {
                riBytePos += Storage::ReadUBits(acByte+riBytePos,riBitRatio,2,iValue);
                MemCpy(aucPix,aauc8888Color[iValue],3);

                if ( m_eCompression == CT_DXT_2
                ||   m_eCompression == CT_DXT_4 )
                {
                    // unassociate alpha from color components
                    if ( aucPix[3] != 0 )
                    {
                        fInvA = ((float)255)/aucPix[3];
                        for (int i2 = 0; i2 < 3; i2++)
                        {
                            iValue = Mathf::RoundToInt(fInvA*aucPix[i2]);
                            if ( iValue > 255 )
                                iValue = 255;
                            aucPix[i2] = iValue;
                        }
                    }
                    else
                    {
                        for (int i2 = 0; i2 < 3; i2++)
                            aucPix[i2] = 255;
                    }
                }

                aucPix -= 4;
            }

            pucRow += rspqRasterImage->GetRowStride();
        }
    }
}
//------------------------------------------------------------------------------------------------------------------



