#include "QFormatPsd.h"
#include "QExptWarnBase.mcr"
#include "QFormatBase.mcr"
#include "QFormatPsd.mcr"
#include "QProgressProbe.h"
#include "QCompressionPackBits.h"
#include "QStorage.h"
using namespace Q;

//------------------------------------------------------------------------------------------------------------------
FormatPsd::FormatPsd (const RasterImagePtr& rspqCompositeImage, int iBitsPerPixel, bool bTreatPalettedAsBitmap)
    :
    m_spqCRISaveTo(rspqCompositeImage),
    m_bTreatPalettedAsBitmap(bTreatPalettedAsBitmap),
    m_eCompression(CT_RLE),
    m_iCIWidth(rspqCompositeImage->GetWidth()),
    m_iCIHeight(rspqCompositeImage->GetHeight()),
    m_bHasResolutionInfo(true),
    m_iHorizontalResolution(72),
    m_iVerticalResolution(72),
    m_bHasICCProfile(false)
{
    assert( rspqCompositeImage->GetWidth() != 0 && rspqCompositeImage->GetHeight() != 0 );
    assert( rspqCompositeImage->GetWidth() <= 30000 && rspqCompositeImage->GetHeight() <= 30000 );
    assert( rspqCompositeImage->GetColorModel() == RasterImage::CMT_PALETTED
       ||   rspqCompositeImage->GetColorModel() == RasterImage::CMT_GRAYSCALE
       ||   rspqCompositeImage->GetColorModel() == RasterImage::CMT_RGB
       ||   rspqCompositeImage->GetColorModel() == RasterImage::CMT_CMYK
       ||   rspqCompositeImage->GetColorModel() == RasterImage::CMT_LAB );
    assert( !(rspqCompositeImage->GetColorModel() != RasterImage::CMT_PALETTED && iBitsPerPixel == -1) );
    assert( !rspqCompositeImage->HasVisibilityMask() );
    assert( !(bTreatPalettedAsBitmap && rspqCompositeImage->GetColorModel() != RasterImage::CMT_PALETTED) );
    assert( !(bTreatPalettedAsBitmap && rspqCompositeImage->GetPaletteSize() != 2) );
    assert( !(bTreatPalettedAsBitmap
       &&   (rspqCompositeImage->GetPaletteColor(0) != PaletteColor(0,0,0)
       ||   rspqCompositeImage->GetPaletteColor(1) != PaletteColor(255,255,255))) );

    switch ( rspqCompositeImage->GetColorModel() )
    {
    case RasterImage::CMT_PALETTED:
        m_eColorModel = ( !bTreatPalettedAsBitmap ? CMT_PALETTED : CMT_BITMAP );
        break;
    case RasterImage::CMT_GRAYSCALE:
        m_eColorModel = CMT_GRAYSCALE;
        break;
    case RasterImage::CMT_RGB:
        m_eColorModel = CMT_RGB;
        break;
    case RasterImage::CMT_CMYK:
        m_eColorModel = CMT_CMYK;
        break;
    case RasterImage::CMT_LAB:
        m_eColorModel = CMT_LAB;
        break;
    }

    if ( iBitsPerPixel == -1 )
    {
        m_iBitsPerChannel = ( !bTreatPalettedAsBitmap ? 8 : 1 );
    }
    else
    {
        assert( iBitsPerPixel > 0 && iBitsPerPixel % rspqCompositeImage->GetComponentsPerPixel() == 0 );
        m_iBitsPerChannel = iBitsPerPixel/rspqCompositeImage->GetComponentsPerPixel();
    }
}
//------------------------------------------------------------------------------------------------------------------
void FormatPsd::InsertLayer (const Layer& rqLayer)
{
    assert( m_eColorModel != CMT_BITMAP && m_eColorModel != CMT_PALETTED );
    assert( rqLayer.PosRect.Width >= 0 && rqLayer.PosRect.Height >= 0 );
    assert( rqLayer.PosRect.Width <= 30000 && rqLayer.PosRect.Height <= 30000 );
    assert( (m_eColorModel == CMT_GRAYSCALE
       &&   rqLayer.ImagePtr->GetColorModel() == RasterImage::CMT_GRAYSCALE_WITH_ALPHA)
       ||   (m_eColorModel == CMT_RGB
       &&   rqLayer.ImagePtr->GetColorModel() == RasterImage::CMT_RGBA)
       ||   (m_eColorModel == CMT_CMYK
       &&   rqLayer.ImagePtr->GetColorModel() == RasterImage::CMT_CMYK_WITH_ALPHA)
       ||   (m_eColorModel == CMT_LAB
       &&   rqLayer.ImagePtr->GetColorModel() == RasterImage::CMT_LAB_WITH_ALPHA) );

    m_qLayersToSave.Push(rqLayer);
}
//------------------------------------------------------------------------------------------------------------------
bool FormatPsd::SaveFile (const char* acFilename)
{
    assert( m_iBitsPerChannel == 1 || m_iBitsPerChannel == 8 || m_iBitsPerChannel == 16 );
    assert( !(m_eColorModel == CMT_BITMAP && m_iBitsPerChannel != 1) );
    assert( !(m_eColorModel == CMT_PALETTED && m_iBitsPerChannel != 8) );
    assert( !((m_eColorModel == CMT_GRAYSCALE || m_eColorModel == CMT_RGB || m_eColorModel == CMT_CMYK
        ||  m_eColorModel == CMT_LAB) && m_iBitsPerChannel == 1) );
    assert( !(!m_qLayersToSave.IsEmpty() && m_iBitsPerChannel != 8) );

    // main channel quantity
    switch ( m_eColorModel )
    {
    case CMT_BITMAP:
    case CMT_GRAYSCALE:
    case CMT_PALETTED:
        m_iChannelQuantity = 1;
        break;
    case CMT_RGB:
        m_iChannelQuantity = 3;
        break;
    case CMT_CMYK:
        m_iChannelQuantity = 4;
        break;
    case CMT_LAB:
        m_iChannelQuantity = 3;
        break;
    }

    Stream qSaveStream(acFilename,false);

    // Header.
    qSaveStream.Write(4,"8BPS");

    unsigned short usVersion = 1;
    qSaveStream.Write(usVersion);

    char acReserved[6] = {0, 0, 0, 0, 0, 0};
    qSaveStream.Write(6,acReserved);

    unsigned short usChannels = m_iChannelQuantity;
    qSaveStream.Write(usChannels);

    unsigned int uiRows = m_iCIHeight;
    unsigned int uiColumns = m_iCIWidth;
    qSaveStream.Write(uiRows);
    qSaveStream.Write(uiColumns);

    unsigned short usDepth = m_iBitsPerChannel;
    qSaveStream.Write(usDepth);

    unsigned short usMode;
    switch ( m_eColorModel )
    {
    case CMT_BITMAP:
        usMode = 0;
        break;
    case CMT_GRAYSCALE:
        usMode = 1;
        break;
    case CMT_PALETTED:
        usMode = 2;
        break;
    case CMT_RGB:
        usMode = 3;
        break;
    case CMT_CMYK:
        usMode = 4;
        break;
    case CMT_LAB:
        usMode = 9;
        break;
    }
    qSaveStream.Write(usMode);

    char cZero = 0;

    // color mode data section
    unsigned int uiCMDSLength;
    if ( m_eColorModel != CMT_PALETTED )
    {
        uiCMDSLength = 0;
        qSaveStream.Write(uiCMDSLength);
    }
    else
    {
        uiCMDSLength = 768;
        qSaveStream.Write(uiCMDSLength);

        // write the palette
        int iPalBuff = 256 - m_spqCRISaveTo->GetPaletteSize();
        for (int i0 = 0; i0 < 3; i0++)
        {
            for (int i1 = 0; i1 < m_spqCRISaveTo->GetPaletteSize(); i1++)
                qSaveStream.Write(m_spqCRISaveTo->GetPaletteColor(i1).Components[2-i0]);
            for (int i1 = 0; i1 < iPalBuff; i1++)
                qSaveStream.Write(cZero);
        }
    }

    unsigned int uiSize;
    unsigned int uiZero = 0;
    unsigned short usZero = 0;

    // image resource section
    unsigned int uiIRSLength;
    if ( m_bHasResolutionInfo || m_bHasICCProfile )
    {
        int iBeforeIRSL = qSaveStream.GetBytePosition();
        qSaveStream.Write(uiZero);

        unsigned short usId;

        if ( m_bHasResolutionInfo )
        {
            // resolution info
            qSaveStream.Write(4,"8BIM");
            usId = 1005;
            qSaveStream.Write(usId);
            qSaveStream.Write(cZero);
            qSaveStream.Write(cZero);
            uiSize = 16;
            qSaveStream.Write(uiSize);
            unsigned int uiHRes = m_iHorizontalResolution << 16;
            unsigned int uiVRes = m_iVerticalResolution << 16;
            unsigned short usRU = 1;
            unsigned short usU = 2;
            qSaveStream.Write(uiHRes);
            qSaveStream.Write(usRU);
            qSaveStream.Write(usU);
            qSaveStream.Write(uiVRes);
            qSaveStream.Write(usRU);
            qSaveStream.Write(usU);
        }

        if ( m_bHasICCProfile )
        {
            // ICC Profile
            qSaveStream.Write(4,"8BIM");
            usId = 1039;
            qSaveStream.Write(usId);
            qSaveStream.Write(cZero);
            qSaveStream.Write(cZero);
            uiSize = m_qICCProfile.GetQuantity();
            qSaveStream.Write(uiSize);
            qSaveStream.Write(uiSize,m_qICCProfile);
            if ( (uiSize & 1) != 0 )
                qSaveStream.Write(cZero);
        }

        uiIRSLength = qSaveStream.GetBytePosition() - iBeforeIRSL - 4;
        qSaveStream.PushPositionState();
        qSaveStream.SetBytePosition(iBeforeIRSL);
        qSaveStream.Write(uiIRSLength);
        qSaveStream.PopPositionState();
    }
    else
    {
        uiIRSLength = 0;
        qSaveStream.Write(uiIRSLength);
    }

    unsigned short usCompression;
    int iRowSize, iBeforeByteCounts, iBCQuantity, iBCC;
    StillArray<unsigned short> qByteCounts;

    // for a progress bar
    ProgressProbe qProgress(this);

    // layer and mask information section
    unsigned int uiMiscLength;
    unsigned char ucValue;
    if ( !m_qLayersToSave.IsEmpty() )
    {
        m_iLayerQuantity = m_qLayersToSave.GetQuantity();
        int iChannelQuantityWA = m_iChannelQuantity + 1;
        StillArray<StillArray<int> > qFCDLOffsets(m_iLayerQuantity);
        for (int i = 0; i < m_iLayerQuantity; i++)
            qFCDLOffsets[i].SetQuantity(iChannelQuantityWA);

        qProgress.SetAdvance(0.5f/(m_iLayerQuantity*iChannelQuantityWA));

        int iBeforeML = qSaveStream.GetBytePosition();
        qSaveStream.Write(uiZero);
        int iBeforeLISL = qSaveStream.GetBytePosition();
        qSaveStream.Write(uiZero);

        short sCount = m_iLayerQuantity;  // positive
        qSaveStream.Write(sCount);

        int iTop, iLeft, iBottom, iRight, iBeforeEDS, iVar;
        unsigned short usNumberChannels;
        short sChannelId;
        unsigned int uiLBRDLength, uiEDSize;
        unsigned char ucClipping, ucFlags, ucLNLength;
        for (int i = 0; i < m_iLayerQuantity; i++)
        {
            Layer& rqLayer = m_qLayersToSave[i];

            assert( rqLayer.BlendingMode != BMT_UNKNOWN );
            assert( rqLayer.Name.GetLength() <= 255 );
            assert( rqLayer.PosRect.Width == rqLayer.ImagePtr->GetWidth() );
            assert( rqLayer.PosRect.Height == rqLayer.ImagePtr->GetHeight() );

            iTop = rqLayer.PosRect.Y;
            iLeft = rqLayer.PosRect.X;
            iBottom = rqLayer.PosRect.Y + rqLayer.PosRect.Height;
            iRight = rqLayer.PosRect.X + rqLayer.PosRect.Width;
            qSaveStream.Write(iTop);
            qSaveStream.Write(iLeft);
            qSaveStream.Write(iBottom);
            qSaveStream.Write(iRight);

            usNumberChannels = iChannelQuantityWA;
            qSaveStream.Write(usNumberChannels);

            // transparency will go first
            for (int i0 = 0; i0 < iChannelQuantityWA; i0++)
            {
                sChannelId = i0 - 1;
                qSaveStream.Write(sChannelId);

                qFCDLOffsets[i][i0] = qSaveStream.GetBytePosition();
                qSaveStream.Write(uiZero);
            }

            qSaveStream.Write(4,"8BIM");

            switch ( rqLayer.BlendingMode )
            {
            case BMT_NORMAL:
                qSaveStream.Write(4,"norm");
                break;
            case BMT_DISSOLVE:
                qSaveStream.Write(4,"diss");
                break;
            case BMT_DARKEN:
                qSaveStream.Write(4,"dark");
                break;
            case BMT_MULTIPLY:
                qSaveStream.Write(4,"mul ");
                break;
            case BMT_COLOR_BURN:
                qSaveStream.Write(4,"idiv");
                break;
            case BMT_LINEAR_BURN:
                qSaveStream.Write(4,"lbrn");
                break;
            case BMT_LIGHTEN:
                qSaveStream.Write(4,"lite");
                break;
            case BMT_SCREEN:
                qSaveStream.Write(4,"scrn");
                break;
            case BMT_COLOR_DODGE:
                qSaveStream.Write(4,"div ");
                break;
            case BMT_LINEAR_DODGE:
                qSaveStream.Write(4,"lddg");
                break;
            case BMT_OVERLAY:
                qSaveStream.Write(4,"over");
                break;
            case BMT_SOFT_LIGHT:
                qSaveStream.Write(4,"sLit");
                break;
            case BMT_HARD_LIGHT:
                qSaveStream.Write(4,"hLit");
                break;
            case BMT_VIVID_LIGHT:
                qSaveStream.Write(4,"vLit");
                break;
            case BMT_LINEAR_LIGHT:
                qSaveStream.Write(4,"lLit");
                break;
            case BMT_PIN_LIGHT:
                qSaveStream.Write(4,"pLit");
                break;
            case BMT_HARD_MIX:
                qSaveStream.Write(4,"hMix");
                break;
            case BMT_DIFFERENCE:
                qSaveStream.Write(4,"diff");
                break;
            case BMT_EXCLUSION:
                qSaveStream.Write(4,"smud");
                break;
            case BMT_HUE:
                qSaveStream.Write(4,"hue ");
                break;
            case BMT_SATURATION:
                qSaveStream.Write(4,"sat ");
                break;
            case BMT_COLOR:
                qSaveStream.Write(4,"colr");
                break;
            case BMT_LUMINOSITY:
                qSaveStream.Write(4,"lum ");
                break;
            }

            qSaveStream.Write(rqLayer.Opacity);

            switch ( rqLayer.Clipping )
            {
            case CTT_BASE:
                ucClipping = 0;
                break;
            case CTT_NON_BASE:
                ucClipping = 1;
                break;
            }
            qSaveStream.Write(ucClipping);

            ucFlags = 0;
            if ( rqLayer.TransparencyProtected )
                ucFlags |= 1;
            if ( !rqLayer.Visible )
                ucFlags |= 2;
            qSaveStream.Write(ucFlags);

            qSaveStream.Write(cZero);

            iBeforeEDS = qSaveStream.GetBytePosition();
            qSaveStream.Write(uiZero);

            qSaveStream.Write(uiZero);  // no layer mask data

            if ( rqLayer.HasBlendingRangesData )
            {
                uiLBRDLength = 8 + rqLayer.ChannelRanges.GetQuantity()*8;
                qSaveStream.Write(uiLBRDLength);

                qSaveStream.Write(rqLayer.CGSrcBlack0);
                qSaveStream.Write(rqLayer.CGSrcBlack1);
                qSaveStream.Write(rqLayer.CGSrcWhite0);
                qSaveStream.Write(rqLayer.CGSrcWhite1);
                qSaveStream.Write(rqLayer.CGDstBlack0);
                qSaveStream.Write(rqLayer.CGDstBlack1);
                qSaveStream.Write(rqLayer.CGDstWhite0);
                qSaveStream.Write(rqLayer.CGDstWhite1);
                for (int i0 = 0; i0 < rqLayer.ChannelRanges.GetQuantity(); i0++)
                {
                    const ChannelRange& rqChannelRange = rqLayer.ChannelRanges[i0];

                    qSaveStream.Write(rqChannelRange.SrcBlack0);
                    qSaveStream.Write(rqChannelRange.SrcBlack1);
                    qSaveStream.Write(rqChannelRange.SrcWhite0);
                    qSaveStream.Write(rqChannelRange.SrcWhite1);
                    qSaveStream.Write(rqChannelRange.DstBlack0);
                    qSaveStream.Write(rqChannelRange.DstBlack1);
                    qSaveStream.Write(rqChannelRange.DstWhite0);
                    qSaveStream.Write(rqChannelRange.DstWhite1);
                }
            }
            else
            {
                qSaveStream.Write(uiZero);
            }

            ucLNLength = rqLayer.Name.GetLength();
            qSaveStream.Write(ucLNLength);
            qSaveStream.Write(ucLNLength,rqLayer.Name);
            iVar = 1 + ucLNLength;
            while ( (iVar & 3) != 0 )
            {
                qSaveStream.Write(cZero);
                iVar++;
            }

            uiEDSize = qSaveStream.GetBytePosition() - iBeforeEDS - 4;
            qSaveStream.PushPositionState();
            qSaveStream.SetBytePosition(iBeforeEDS);
            qSaveStream.Write(uiEDSize);
            qSaveStream.PopPositionState();
        }

        // channel data
        int iComponent, iBeforeCC;
        float fValue;
        unsigned int uiCDLength;
        usCompression = ( m_eCompression == CT_NONE ? 0 : 1 );
        for (int i0 = 0; i0 < m_iLayerQuantity; i0++)
        {
            const Layer& rqLayer = m_qLayersToSave[i0];

            iRowSize = rqLayer.ImagePtr->GetWidth();
            for (int i1 = 0; i1 < iChannelQuantityWA; i1++)
            {
                // alpha goes first
                iComponent = ( i1 == 0 ? iChannelQuantityWA - 1 : i1 - 1 );

                iBeforeCC = qSaveStream.GetBytePosition();
                qSaveStream.Write(usCompression);

                if ( m_eCompression == CT_RLE )
                {
                    iBeforeByteCounts = qSaveStream.GetBytePosition();

                    iBCQuantity = rqLayer.ImagePtr->GetHeight();
                    qByteCounts.SetQuantity(iBCQuantity,false);
                    for (int i2 = 0; i2 < iBCQuantity; i2++)
                        qSaveStream.Write(usZero);

                    iBCC = 0;
                }

                if ( m_eColorModel == CMT_GRAYSCALE )
                {
                    if ( m_eCompression == CT_NONE )
                    {
                        // no compression
                        for (int iY = 0; iY < rqLayer.ImagePtr->GetHeight(); iY++)
                        {
                            for (int iX = 0; iX < rqLayer.ImagePtr->GetWidth(); iX++)
                            {
                                if ( iComponent == 0 )
                                    fValue = rqLayer.ImagePtr->GetPixelIntensityWithAlpha(iX,iY).First;
                                else
                                    fValue = rqLayer.ImagePtr->GetPixelIntensityWithAlpha(iX,iY).Second;
                                ucValue = Mathf::RoundToInt(fValue*255);
                                qSaveStream.Write(ucValue);
                            }
                        }
                    }
                    else  // m_eCompression = CT_RLE
                    {
                        // compressed
                        StillArray<char> qDecompressedRow(iRowSize);
                        for (int iY = 0; iY < rqLayer.ImagePtr->GetHeight(); iY++)
                        {
                            for (int iX = 0; iX < rqLayer.ImagePtr->GetWidth(); iX++)
                            {
                                if ( iComponent == 0 )
                                    fValue = rqLayer.ImagePtr->GetPixelIntensityWithAlpha(iX,iY).First;
                                else
                                    fValue = rqLayer.ImagePtr->GetPixelIntensityWithAlpha(iX,iY).Second;
                                qDecompressedRow[iX] = Mathf::RoundToInt(fValue*255);
                            }

                            COMPRESS_ROW_AND_WRITE;
                        }
                    }
                }
                else if ( m_eColorModel == CMT_RGB )
                {
                    if ( m_eCompression == CT_NONE )
                    {
                        // no compression
                        for (int iY = 0; iY < rqLayer.ImagePtr->GetHeight(); iY++)
                        {
                            for (int iX = 0; iX < rqLayer.ImagePtr->GetWidth(); iX++)
                            {
                                ucValue = Mathf::RoundToInt(rqLayer.ImagePtr->GetPixelRgba(iX,iY)[iComponent]*255);
                                qSaveStream.Write(ucValue);
                            }
                        }
                    }
                    else  // m_eCompression = CT_RLE
                    {
                        // compressed
                        StillArray<char> qDecompressedRow(iRowSize);
                        for (int iY = 0; iY < rqLayer.ImagePtr->GetHeight(); iY++)
                        {
                            for (int iX = 0; iX < rqLayer.ImagePtr->GetWidth(); iX++)
                            {
                                qDecompressedRow[iX] =
                                    Mathf::RoundToInt(rqLayer.ImagePtr->GetPixelRgba(iX,iY)[iComponent]*255);
                            }

                            COMPRESS_ROW_AND_WRITE;
                        }
                    }
                }
                else if ( m_eColorModel == CMT_CMYK )
                {
                    if ( m_eCompression == CT_NONE )
                    {
                        // no compression
                        for (int iY = 0; iY < rqLayer.ImagePtr->GetHeight(); iY++)
                        {
                            for (int iX = 0; iX < rqLayer.ImagePtr->GetWidth(); iX++)
                            {
                                ucValue = Mathf::RoundToInt(rqLayer.ImagePtr->GetPixelCmyka(iX,iY)[iComponent]*255);
                                if ( iComponent < 4 )
                                    ucValue = 255 - ucValue;
                                qSaveStream.Write(ucValue);
                            }
                        }
                    }
                    else  // m_eCompression = CT_RLE
                    {
                        // compressed
                        StillArray<char> qDecompressedRow(iRowSize);
                        for (int iY = 0; iY < rqLayer.ImagePtr->GetHeight(); iY++)
                        {
                            for (int iX = 0; iX < rqLayer.ImagePtr->GetWidth(); iX++)
                            {
                                ucValue = Mathf::RoundToInt(rqLayer.ImagePtr->GetPixelCmyka(iX,iY)[iComponent]*255);
                                if ( iComponent < 4 )
                                    ucValue = 255 - ucValue;
                                qDecompressedRow[iX] = ucValue;
                            }

                            COMPRESS_ROW_AND_WRITE;
                        }
                    }
                }
                else  // m_eColorModel = CMT_LAB
                {
                    if ( m_eCompression == CT_NONE )
                    {
                        // no compression
                        for (int iY = 0; iY < rqLayer.ImagePtr->GetHeight(); iY++)
                        {
                            for (int iX = 0; iX < rqLayer.ImagePtr->GetWidth(); iX++)
                            {
                                fValue = rqLayer.ImagePtr->GetPixelLaba(iX,iY)[iComponent];
                                if ( iComponent == 0 )
                                {
                                    ucValue = Mathf::RoundToInt(fValue/100*255);
                                }
                                else if ( iComponent < 3 )
                                {
                                    fValue += 128;
                                    if ( fValue < 0 )
                                        fValue = 0;
                                    else if ( fValue > 255 )
                                        fValue = 255;
                                    ucValue = Mathf::RoundToInt(fValue);
                                }
                                else
                                {
                                    ucValue = Mathf::RoundToInt(fValue*255);
                                }
                                qSaveStream.Write(ucValue);
                            }
                        }
                    }
                    else  // m_eCompression = CT_RLE
                    {
                        // compressed
                        StillArray<char> qDecompressedRow(iRowSize);
                        for (int iY = 0; iY < rqLayer.ImagePtr->GetHeight(); iY++)
                        {
                            for (int iX = 0; iX < rqLayer.ImagePtr->GetWidth(); iX++)
                            {
                                fValue = rqLayer.ImagePtr->GetPixelLaba(iX,iY)[iComponent];
                                if ( iComponent == 0 )
                                {
                                    ucValue = Mathf::RoundToInt(fValue/100*255);
                                }
                                else if ( iComponent < 3 )
                                {
                                    fValue += 128;
                                    if ( fValue < 0 )
                                        fValue = 0;
                                    else if ( fValue > 255 )
                                        fValue = 255;
                                    ucValue = Mathf::RoundToInt(fValue);
                                }
                                else
                                {
                                    ucValue = Mathf::RoundToInt(fValue*255);
                                }
                                qDecompressedRow[iX] = ucValue;
                            }

                            COMPRESS_ROW_AND_WRITE;
                        }
                    }
                }

                RETURN_AND_WRITE_ROWS_BYTE_COUNTS;

                // return and write channel data length
                uiCDLength = qSaveStream.GetBytePosition() - iBeforeCC;
                qSaveStream.PushPositionState();
                qSaveStream.SetBytePosition(qFCDLOffsets[i0][i1]);
                qSaveStream.Write(uiCDLength);
                qSaveStream.PopPositionState();

                qProgress.MakeAdvance();
            }
        }

        if ( ((qSaveStream.GetBytePosition() - iBeforeLISL - 4) & 1) != 0 )
            qSaveStream.Write(cZero);

        unsigned int uiLISLength = qSaveStream.GetBytePosition() - iBeforeLISL - 4;
        qSaveStream.PushPositionState();
        qSaveStream.SetBytePosition(iBeforeLISL);
        qSaveStream.Write(uiLISLength);
        qSaveStream.PopPositionState();

        unsigned int uiMiscLength = qSaveStream.GetBytePosition() - iBeforeML - 4;
        qSaveStream.PushPositionState();
        qSaveStream.SetBytePosition(iBeforeML);
        qSaveStream.Write(uiMiscLength);
        qSaveStream.PopPositionState();
    }
    else
    {
        uiMiscLength = 0;
        qSaveStream.Write(uiMiscLength);

        qProgress.SetDone(0.5f);
    }

    // Image data section.
    if ( m_eCompression == CT_NONE )
    {
        usCompression = 0;
        qSaveStream.Write(usCompression);
    }
    else  // m_eCompression = CT_RLE
    {
        usCompression = 1;
        qSaveStream.Write(usCompression);

        iBeforeByteCounts = qSaveStream.GetBytePosition();

        iBCQuantity = m_iCIHeight*m_iChannelQuantity;
        qByteCounts.SetQuantity(iBCQuantity,false);
        for (int i = 0; i < iBCQuantity; i++)
            qSaveStream.Write(usZero);
    }

    qProgress.SetAdvance(0.5f/(m_iCIHeight*m_iChannelQuantity));

    unsigned char* pucRow;
    unsigned char* pucComp;
    int iValue, iBytePos, iBitRatio;
    unsigned short usValue;
    iBCC = 0;
    if ( m_eColorModel == CMT_BITMAP )
    {
        iRowSize = ( (m_iCIWidth & 7) != 0 ? m_iCIWidth/8 + 1 : m_iCIWidth/8 );

        if ( m_eCompression == CT_NONE )
        {
            // no compression
            pucRow = (unsigned char*)m_spqCRISaveTo->GetImage();
            pucComp = pucRow;
            for (int iY = 0; iY < m_iCIHeight; iY++)
            {
                for (int iX = 0; iX < m_iCIWidth; iX++)
                {
                    iValue = ( *pucComp == 0 ? 1 : 0 );
                    pucComp++;
                    qSaveStream.WriteBits(iValue,1);
                }

                qSaveStream.AlignBytes();

                pucRow += m_spqCRISaveTo->GetRowStride();
                pucComp = pucRow;

                qProgress.MakeAdvance();
            }
        }
        else  // m_eCompression = CT_RLE
        {
            // compressed
            StillArray<char> qDecompressedRow(iRowSize);
            pucRow = (unsigned char*)m_spqCRISaveTo->GetImage();
            pucComp = pucRow;
            for (int iY = 0; iY < m_iCIHeight; iY++)
            {
                iBytePos = 0;
                iBitRatio = 0;
                for (int iX = 0; iX < m_iCIWidth; iX++)
                {
                    iValue = ( *pucComp == 0 ? 1 : 0 );
                    pucComp++;
                    iBytePos += Storage::WriteBits(qDecompressedRow+iBytePos,iBitRatio,1,iValue);
                }

                COMPRESS_ROW_AND_WRITE;

                pucRow += m_spqCRISaveTo->GetRowStride();
                pucComp = pucRow;

                qProgress.MakeAdvance();
            }
        }
    }
    else if ( m_eColorModel == CMT_GRAYSCALE )
    {
        if ( m_iBitsPerChannel == 8 )
        {
            iRowSize = m_iCIWidth;

            if ( m_eCompression == CT_NONE )
            {
                // no compression
                for (int iY = 0; iY < m_iCIHeight; iY++)
                {
                    for (int iX = 0; iX < m_iCIWidth; iX++)
                    {
                        ucValue = Mathf::RoundToInt(m_spqCRISaveTo->GetPixelIntensity(iX,iY)*255);
                        qSaveStream.Write(ucValue);
                    }

                    qProgress.MakeAdvance();
                }
            }
            else  // m_eCompression = CT_RLE
            {
                // compressed
                StillArray<char> qDecompressedRow(iRowSize);
                for (int iY = 0; iY < m_iCIHeight; iY++)
                {
                    for (int iX = 0; iX < m_iCIWidth; iX++)
                        qDecompressedRow[iX] = Mathf::RoundToInt(m_spqCRISaveTo->GetPixelIntensity(iX,iY)*255);

                    COMPRESS_ROW_AND_WRITE;

                    qProgress.MakeAdvance();
                }
            }
        }
        else  // m_iBitsPerChannel = 16
        {
            iRowSize = m_iCIWidth*2;

            if ( m_eCompression == CT_NONE )
            {
                // no compression
                for (int iY = 0; iY < m_iCIHeight; iY++)
                {
                    for (int iX = 0; iX < m_iCIWidth; iX++)
                    {
                        usValue = Mathf::RoundToInt(m_spqCRISaveTo->GetPixelIntensity(iX,iY)*65535);
                        qSaveStream.Write(usValue);
                    }

                    qProgress.MakeAdvance();
                }
            }
            else  // m_eCompression = CT_RLE
            {
                // compressed
                StillArray<char> qDecompressedRow(iRowSize);
                for (int iY = 0; iY < m_iCIHeight; iY++)
                {
                    iBytePos = 0;
                    for (int iX = 0; iX < m_iCIWidth; iX++)
                    {
                        usValue = Mathf::RoundToInt(m_spqCRISaveTo->GetPixelIntensity(iX,iY)*65535);
                        iBytePos += Storage::Write2be(qDecompressedRow+iBytePos,1,&usValue);
                    }

                    COMPRESS_ROW_AND_WRITE;

                    qProgress.MakeAdvance();
                }
            }
        }
    }
    else if ( m_eColorModel == CMT_PALETTED )
    {
        iRowSize = m_iCIWidth;

        if ( m_eCompression == CT_NONE )
        {
            // no compression
            pucRow = (unsigned char*)m_spqCRISaveTo->GetImage();
            pucComp = pucRow;
            for (int iY = 0; iY < m_iCIHeight; iY++)
            {
                for (int iX = 0; iX < m_iCIWidth; iX++)
                    qSaveStream.Write(*pucComp++);

                pucRow += m_spqCRISaveTo->GetRowStride();
                pucComp = pucRow;

                qProgress.MakeAdvance();
            }
        }
        else  // m_eCompression = CT_RLE
        {
            // compressed
            StillArray<char> qDecompressedRow(iRowSize);
            pucRow = (unsigned char*)m_spqCRISaveTo->GetImage();
            pucComp = pucRow;
            for (int iY = 0; iY < m_iCIHeight; iY++)
            {
                for (int iX = 0; iX < m_iCIWidth; iX++)
                    qDecompressedRow[iX] = *pucComp++;

                COMPRESS_ROW_AND_WRITE;

                pucRow += m_spqCRISaveTo->GetRowStride();
                pucComp = pucRow;

                qProgress.MakeAdvance();
            }
        }
    }
    else if ( m_eColorModel == CMT_RGB )
    {
        if ( m_iBitsPerChannel == 8 )
        {
            iRowSize = m_iCIWidth;

            if ( m_eCompression == CT_NONE )
            {
                // no compression
                for (int i = 0; i < 3; i++)
                {
                    for (int iY = 0; iY < m_iCIHeight; iY++)
                    {
                        for (int iX = 0; iX < m_iCIWidth; iX++)
                        {
                            ucValue = Mathf::RoundToInt(m_spqCRISaveTo->GetPixelRgb(iX,iY)[i]*255);
                            qSaveStream.Write(ucValue);
                        }

                        qProgress.MakeAdvance();
                    }
                }
            }
            else  // m_eCompression = CT_RLE
            {
                // compressed
                StillArray<char> qDecompressedRow(iRowSize);
                for (int i = 0; i < 3; i++)
                {
                    for (int iY = 0; iY < m_iCIHeight; iY++)
                    {
                        for (int iX = 0; iX < m_iCIWidth; iX++)
                            qDecompressedRow[iX] = Mathf::RoundToInt(m_spqCRISaveTo->GetPixelRgb(iX,iY)[i]*255);

                        COMPRESS_ROW_AND_WRITE;

                        qProgress.MakeAdvance();
                    }
                }
            }
        }
        else  // m_iBitsPerChannel = 16
        {
            iRowSize = m_iCIWidth*2;

            if ( m_eCompression == CT_NONE )
            {
                // no compression
                for (int i = 0; i < 3; i++)
                {
                    for (int iY = 0; iY < m_iCIHeight; iY++)
                    {
                        for (int iX = 0; iX < m_iCIWidth; iX++)
                        {
                            usValue = Mathf::RoundToInt(m_spqCRISaveTo->GetPixelRgb(iX,iY)[i]*65535);
                            qSaveStream.Write(usValue);
                        }

                        qProgress.MakeAdvance();
                    }
                }
            }
            else  // m_eCompression = CT_RLE
            {
                // compressed
                StillArray<char> qDecompressedRow(iRowSize);
                for (int i = 0; i < 3; i++)
                {
                    for (int iY = 0; iY < m_iCIHeight; iY++)
                    {
                        iBytePos = 0;
                        for (int iX = 0; iX < m_iCIWidth; iX++)
                        {
                            usValue = Mathf::RoundToInt(m_spqCRISaveTo->GetPixelRgb(iX,iY)[i]*65535);
                            iBytePos += Storage::Write2be(qDecompressedRow+iBytePos,1,&usValue);
                        }

                        COMPRESS_ROW_AND_WRITE;

                        qProgress.MakeAdvance();
                    }
                }
            }
        }
    }
    else if ( m_eColorModel == CMT_CMYK )
    {
        if ( m_iBitsPerChannel == 8 )
        {
            iRowSize = m_iCIWidth;

            if ( m_eCompression == CT_NONE )
            {
                // no compression
                for (int i = 0; i < 4; i++)
                {
                    for (int iY = 0; iY < m_iCIHeight; iY++)
                    {
                        for (int iX = 0; iX < m_iCIWidth; iX++)
                        {
                            ucValue = 255 - Mathf::RoundToInt(m_spqCRISaveTo->GetPixelCmyk(iX,iY)[i]*255);
                            qSaveStream.Write(ucValue);
                        }

                        qProgress.MakeAdvance();
                    }
                }
            }
            else  // m_eCompression = CT_RLE
            {
                // compressed
                StillArray<char> qDecompressedRow(iRowSize);
                for (int i = 0; i < 4; i++)
                {
                    for (int iY = 0; iY < m_iCIHeight; iY++)
                    {
                        for (int iX = 0; iX < m_iCIWidth; iX++)
                        {
                            qDecompressedRow[iX] =
                                255 - Mathf::RoundToInt(m_spqCRISaveTo->GetPixelCmyk(iX,iY)[i]*255);
                        }

                        COMPRESS_ROW_AND_WRITE;

                        qProgress.MakeAdvance();
                    }
                }
            }
        }
        else  // m_iBitsPerChannel = 16
        {
            iRowSize = m_iCIWidth*2;

            if ( m_eCompression == CT_NONE )
            {
                // no compression
                for (int i = 0; i < 4; i++)
                {
                    for (int iY = 0; iY < m_iCIHeight; iY++)
                    {
                        for (int iX = 0; iX < m_iCIWidth; iX++)
                        {
                            usValue = 65535 - Mathf::RoundToInt(m_spqCRISaveTo->GetPixelCmyk(iX,iY)[i]*65535);
                            qSaveStream.Write(usValue);
                        }

                        qProgress.MakeAdvance();
                    }
                }
            }
            else  // m_eCompression = CT_RLE
            {
                // compressed
                StillArray<char> qDecompressedRow(iRowSize);
                for (int i = 0; i < 4; i++)
                {
                    for (int iY = 0; iY < m_iCIHeight; iY++)
                    {
                        iBytePos = 0;
                        for (int iX = 0; iX < m_iCIWidth; iX++)
                        {
                            usValue = 65535 - Mathf::RoundToInt(m_spqCRISaveTo->GetPixelCmyk(iX,iY)[i]*65535);
                            iBytePos += Storage::Write2be(qDecompressedRow+iBytePos,1,&usValue);
                        }

                        COMPRESS_ROW_AND_WRITE;

                        qProgress.MakeAdvance();
                    }
                }
            }
        }
    }
    else  // m_eColorModel = CMT_LAB
    {
        float fValue;
        if ( m_iBitsPerChannel == 8 )
        {
            iRowSize = m_iCIWidth;

            if ( m_eCompression == CT_NONE )
            {
                // no compression
                for (int i = 0; i < 3; i++)
                {
                    for (int iY = 0; iY < m_iCIHeight; iY++)
                    {
                        for (int iX = 0; iX < m_iCIWidth; iX++)
                        {
                            fValue = m_spqCRISaveTo->GetPixelLab(iX,iY)[i];
                            if ( i == 0 )
                            {
                                ucValue = Mathf::RoundToInt(fValue/100*255);
                            }
                            else
                            {
                                fValue += 128;
                                if ( fValue < 0 )
                                    fValue = 0;
                                else if ( fValue > 255 )
                                    fValue = 255;
                                ucValue = Mathf::RoundToInt(fValue);
                            }
                            qSaveStream.Write(ucValue);
                        }

                        qProgress.MakeAdvance();
                    }
                }
            }
            else  // m_eCompression = CT_RLE
            {
                // compressed
                StillArray<char> qDecompressedRow(iRowSize);
                for (int i = 0; i < 3; i++)
                {
                    for (int iY = 0; iY < m_iCIHeight; iY++)
                    {
                        for (int iX = 0; iX < m_iCIWidth; iX++)
                        {
                            fValue = m_spqCRISaveTo->GetPixelLab(iX,iY)[i];
                            if ( i == 0 )
                            {
                                ucValue = Mathf::RoundToInt(fValue/100*255);
                            }
                            else
                            {
                                fValue += 128;
                                if ( fValue < 0 )
                                    fValue = 0;
                                else if ( fValue > 255 )
                                    fValue = 255;
                                ucValue = Mathf::RoundToInt(fValue);
                            }
                            qDecompressedRow[iX] = ucValue;
                        }

                        COMPRESS_ROW_AND_WRITE;

                        qProgress.MakeAdvance();
                    }
                }
            }
        }
        else  // m_iBitsPerChannel = 16
        {
            iRowSize = m_iCIWidth*2;

            if ( m_eCompression == CT_NONE )
            {
                // no compression
                for (int i = 0; i < 3; i++)
                {
                    for (int iY = 0; iY < m_iCIHeight; iY++)
                    {
                        for (int iX = 0; iX < m_iCIWidth; iX++)
                        {
                            fValue = m_spqCRISaveTo->GetPixelLab(iX,iY)[i];
                            if ( i == 0 )
                            {
                                usValue = Mathf::RoundToInt(fValue/100*65535);
                            }
                            else
                            {
                                fValue = (fValue + 128)*256;
                                if ( fValue < 0 )
                                    fValue = 0;
                                else if ( fValue > 65535 )
                                    fValue = 65535;
                                usValue = Mathf::RoundToInt(fValue);
                            }
                            qSaveStream.Write(usValue);
                        }

                        qProgress.MakeAdvance();
                    }
                }
            }
            else  // m_eCompression = CT_RLE
            {
                // compressed
                StillArray<char> qDecompressedRow(iRowSize);
                for (int i = 0; i < 3; i++)
                {
                    for (int iY = 0; iY < m_iCIHeight; iY++)
                    {
                        iBytePos = 0;
                        for (int iX = 0; iX < m_iCIWidth; iX++)
                        {
                            fValue = m_spqCRISaveTo->GetPixelLab(iX,iY)[i];
                            if ( i == 0 )
                            {
                                usValue = Mathf::RoundToInt(fValue/100*65535);
                            }
                            else
                            {
                                fValue = (fValue + 128)*256;
                                if ( fValue < 0 )
                                    fValue = 0;
                                else if ( fValue > 65535 )
                                    fValue = 65535;
                                usValue = Mathf::RoundToInt(fValue);
                            }
                            iBytePos += Storage::Write2be(qDecompressedRow+iBytePos,1,&usValue);
                        }

                        COMPRESS_ROW_AND_WRITE;

                        qProgress.MakeAdvance();
                    }
                }
            }
        }
    }

    RETURN_AND_WRITE_ROWS_BYTE_COUNTS;

    // flush to the file
    CONDITIONAL_THROW_EXCEPTION(qSaveStream.Finalize(),ET_CANNOT_ACCESS_FILE);

    return true;
}
//------------------------------------------------------------------------------------------------------------------
FormatPsd::FormatPsd (const char* acFilename, bool& rbSuccess)
    :
    FormatBase(acFilename),
    m_bHasResolutionInfo(false),
    m_bHasICCProfile(false),
    m_iLayerQuantity(0)
{
    bool bSuccess;

    // Header.
    int iFileSize;
    C_CONDITIONAL_THROW_EXCEPTION(Storage::GetFileSize(iFileSize,m_qFilename),ET_CANNOT_ACCESS_FILE);
    C_CONDITIONAL_THROW_EXCEPTION_WITHOUT_ASSERT_FALSE(( iFileSize >= 26 ),ET_FORMAT_IDENTIFIER_MISMATCH);
    Stream qHeaderStream(m_eStreaming,m_qFilename,0,26,false,bSuccess);
    C_PROCESS_STREAM_SUCCESS_FOR_LOADING(bSuccess);

    char acSignature[4];
    qHeaderStream.Read(4,acSignature);
    C_CONDITIONAL_THROW_EXCEPTION_WITHOUT_ASSERT_FALSE(( MemCmp(acSignature,"8BPS",4) == 0 ),
        ET_FORMAT_IDENTIFIER_MISMATCH);

    unsigned short usVersion;
    qHeaderStream.Read(usVersion);
    C_CONDITIONAL_THROW_EXCEPTION(( usVersion == 1 ),ET_FORMAT_MISMATCH);

    qHeaderStream.OffsetBytePosition(6);

    unsigned short usChannelQuantity;
    qHeaderStream.Read(usChannelQuantity);
    C_CONDITIONAL_THROW_EXCEPTION(( 1 <= usChannelQuantity && usChannelQuantity <= 24 ),ET_FORMAT_MISMATCH);
    m_iChannelQuantity = usChannelQuantity;

    unsigned int uiRows, uiColumns;
    qHeaderStream.Read(uiRows);
    qHeaderStream.Read(uiColumns);
    C_CONDITIONAL_THROW_EXCEPTION((
         1 <= uiRows && uiRows <= 30000
    &&   1 <= uiColumns && uiColumns <= 30000 ),ET_FORMAT_MISMATCH);
    m_iCIWidth = uiColumns;
    m_iCIHeight = uiRows;

    unsigned short usBitsPerChannel;
    qHeaderStream.Read(usBitsPerChannel);
    C_CONDITIONAL_THROW_EXCEPTION(( usBitsPerChannel == 1 || usBitsPerChannel == 8 || usBitsPerChannel == 16 ),
        ET_FORMAT_MISMATCH);
    m_iBitsPerChannel = usBitsPerChannel;

    unsigned short usMode;
    qHeaderStream.Read(usMode);
    switch ( usMode )
    {
    case 0:
        m_eColorModel = CMT_BITMAP;
        break;
    case 1:
        m_eColorModel = CMT_GRAYSCALE;
        break;
    case 2:
        m_eColorModel = CMT_PALETTED;
        break;
    case 3:
        m_eColorModel = CMT_RGB;
        break;
    case 4:
        m_eColorModel = CMT_CMYK;
        break;
    case 7:
        C_THROW_EXCEPTION(ET_MULTICHANNEL_COLOR_MODE_NOT_SUPPORTED);
    case 8:
        m_eColorModel = CMT_GRAYSCALE;  // duotone becomes grayscale as recommended by the specification
        break;
    case 9:
        m_eColorModel = CMT_LAB;
        break;
    default:
        C_THROW_EXCEPTION(ET_FORMAT_MISMATCH);
    }

    C_CONDITIONAL_THROW_EXCEPTION(
        ( !((m_eColorModel == CMT_BITMAP || m_eColorModel == CMT_PALETTED) && m_iChannelQuantity != 1) ),
        ET_FORMAT_MISMATCH);
    C_CONDITIONAL_THROW_EXCEPTION(( !(m_eColorModel == CMT_RGB && m_iChannelQuantity < 3) ),
        ET_FORMAT_MISMATCH);
    C_CONDITIONAL_THROW_EXCEPTION(( !(m_eColorModel == CMT_CMYK && m_iChannelQuantity < 4) ),
        ET_FORMAT_MISMATCH);
    C_CONDITIONAL_THROW_EXCEPTION(( !(m_eColorModel == CMT_LAB && m_iChannelQuantity < 3) ),
        ET_FORMAT_MISMATCH);
    C_CONDITIONAL_THROW_EXCEPTION(( !(m_eColorModel == CMT_BITMAP && m_iBitsPerChannel != 1) ),
        ET_FORMAT_MISMATCH);
    C_CONDITIONAL_THROW_EXCEPTION(( !(m_eColorModel == CMT_PALETTED && m_iBitsPerChannel != 8) ),
        ET_FORMAT_MISMATCH);
    C_CONDITIONAL_THROW_EXCEPTION(( !((
         m_eColorModel == CMT_GRAYSCALE
    ||   m_eColorModel == CMT_RGB
    ||   m_eColorModel == CMT_CMYK
    ||   m_eColorModel == CMT_LAB) && m_iBitsPerChannel == 1) ),ET_FORMAT_MISMATCH);

    qHeaderStream.Finalize();

    // color mode data (skipping)
    Stream qCMDLStream(m_eStreaming,m_qFilename,26,4,false,bSuccess);
    C_PROCESS_STREAM_SUCCESS_FOR_LOADING(bSuccess);
    unsigned int uiCMDLength;
    qCMDLStream.Read(uiCMDLength);
    qCMDLStream.Finalize();

    C_CONDITIONAL_THROW_EXCEPTION(( !(m_eColorModel == CMT_PALETTED && uiCMDLength != 768) ),
        ET_FORMAT_MISMATCH);

    // image resources
    Stream qIRLengthStream(m_eStreaming,m_qFilename,30+uiCMDLength,4,false,bSuccess);
    C_PROCESS_STREAM_SUCCESS_FOR_LOADING(bSuccess);
    unsigned int uiIRLength;
    qIRLengthStream.Read(uiIRLength);
    qIRLengthStream.Finalize();
    if ( uiIRLength != 0 )
    {
        Stream qIRStream(m_eStreaming,m_qFilename,30+uiCMDLength+4,uiIRLength,false,bSuccess);
        C_PROCESS_STREAM_SUCCESS_FOR_LOADING(bSuccess);
        char acOSType[4];
        unsigned short usId;
        unsigned char ucStrLen;
        unsigned int uiSize;
        int iStrSize, iBlockSize, iVar;
        int iIRRead = 0;
        for (/**/; /**/; /**/)
        {
            C_RETURN_ON_FAIL(qIRStream.Read(4,acOSType));  // without check if it's "8BIM"

            C_RETURN_ON_FAIL(qIRStream.Read(usId));

            // ignore a string
            C_RETURN_ON_FAIL(qIRStream.Read(ucStrLen));
            iStrSize = 1 + ucStrLen;
            if ( (iStrSize & 1) != 0 )
                iStrSize++;
            qIRStream.OffsetBytePosition(iStrSize-1);

            C_RETURN_ON_FAIL(qIRStream.Read(uiSize));

            iVar = 6 + iStrSize + 4;
            iBlockSize = iVar + uiSize;
            if ( (iBlockSize & 1) != 0 )
                iBlockSize++;

            if ( usId == 1005 )
            {
                // resolution information (only hRes and vRes are needed)
                unsigned int uiHRes, uiVRes;
                C_RETURN_ON_FAIL(qIRStream.Read(uiHRes));
                qIRStream.OffsetBytePosition(4);
                C_RETURN_ON_FAIL(qIRStream.Read(uiVRes));
                qIRStream.OffsetBytePosition(4);
                m_iHorizontalResolution = uiHRes >> 16;
                m_iVerticalResolution = uiVRes >> 16;

                m_bHasResolutionInfo = true;
            }
            else if ( usId == 1039 )
            {
                // ICC profile
                m_qICCProfile.SetQuantity(uiSize);
                C_RETURN_ON_FAIL(qIRStream.Read(uiSize,m_qICCProfile));

                m_bHasICCProfile = true;

                if ( (uiSize & 1) != 0 )
                    qIRStream.OffsetBytePosition(1);
            }
            else
            {
                // resource block to be skipped
                qIRStream.OffsetBytePosition(iBlockSize-iVar);
            }

            iIRRead += iBlockSize;
            if ( iIRRead == uiIRLength )
                break;
        }
        qIRStream.Finalize();
    }

    int iMiscOffset = 26 + 4 + uiCMDLength + 4 + uiIRLength;

    Stream qMiscLengthStream(m_eStreaming,m_qFilename,iMiscOffset,4,false,bSuccess);
    C_PROCESS_STREAM_SUCCESS_FOR_LOADING(bSuccess);
    unsigned int uiMiscLength;
    qMiscLengthStream.Read(uiMiscLength);
    qMiscLengthStream.Finalize();
    if ( uiMiscLength != 0 )
    {
        Stream qLISLengthStream(m_eStreaming,m_qFilename,iMiscOffset+4,4,false,bSuccess);
        C_PROCESS_STREAM_SUCCESS_FOR_LOADING(bSuccess);
        unsigned int uiLISLength;
        qLISLengthStream.Read(uiLISLength);
        qLISLengthStream.Finalize();
        if ( uiLISLength != 0 )
        {
            Stream qLQStream(m_eStreaming,m_qFilename,iMiscOffset+8,2,false,bSuccess);
            C_PROCESS_STREAM_SUCCESS_FOR_LOADING(bSuccess);
            short sLayerQuantity;
            qLQStream.Read(sLayerQuantity);
            m_iLayerQuantity = Mathi::Abs(sLayerQuantity);  // ignore sign
            qLQStream.Finalize();
        }
    }

    C_CONDITIONAL_THROW_EXCEPTION(
        ( !((m_eColorModel == CMT_BITMAP || m_eColorModel == CMT_PALETTED) && m_iLayerQuantity != 0) ),
        ET_FORMAT_MISMATCH);

    m_iAfterLQOffset = iMiscOffset + 10;
    m_iIDOffset = iMiscOffset + 4 + uiMiscLength;

    rbSuccess = true;
}
//------------------------------------------------------------------------------------------------------------------
bool FormatPsd::LoadCompositeImage (RasterImagePtr& rspqImage)
{
    bool bSuccess;
    int iProceeded, iValue, iByteCountsSize, iByteCountQuantity, iRowSize, iBytePos, iImageSize, iChannelSize,
        iByteCount, iVar;
    StillArray<unsigned short> qByteCounts;
    unsigned char* pucRow;
    unsigned char* pucComp;
    unsigned short usValue;

    Stream qCCStream(m_eStreaming,m_qFilename,m_iIDOffset,2,false,bSuccess);
    PROCESS_STREAM_SUCCESS_FOR_LOADING(bSuccess);
    unsigned short usComprCode;
    qCCStream.Read(usComprCode);
    qCCStream.Finalize();
    if ( usComprCode != 0 )
    {
        // RLE compression; read rows' byte counts
        iProceeded = 2;
        iByteCountQuantity = m_iCIHeight*m_iChannelQuantity;
        qByteCounts.SetQuantity(iByteCountQuantity);
        iByteCountsSize = iByteCountQuantity*2;
        Stream qBCStream(m_eStreaming,m_qFilename,m_iIDOffset+iProceeded,iByteCountsSize,false,bSuccess);
        PROCESS_STREAM_SUCCESS_FOR_LOADING(bSuccess);
        for (int i = 0; i < iByteCountQuantity; i++)
            qBCStream.Read(qByteCounts[i]);
        qBCStream.Finalize();
        iProceeded += iByteCountsSize;
    }

    // for a progress bar
    ProgressProbe qProgress(this);
    switch ( m_eColorModel )
    {
    case CMT_BITMAP:
    case CMT_GRAYSCALE:
    case CMT_PALETTED:
        qProgress.SetAdvance(1.0f/(1*m_iCIHeight));
        break;
    case CMT_RGB:
    case CMT_LAB:
        qProgress.SetAdvance(1.0f/(3*m_iCIHeight));
        break;
    case CMT_CMYK:
        qProgress.SetAdvance(1.0f/(4*m_iCIHeight));
        break;
    }

    if ( m_eColorModel == CMT_BITMAP )
    {
        // interpret as paletted
        RasterImage::Palette qBWPalette(2);
        qBWPalette[0] = PaletteColor(0,0,0);
        qBWPalette[1] = PaletteColor(255,255,255);
        rspqImage = new RasterImage(RasterImage::IT_PALETTED,m_iCIWidth,m_iCIHeight,qBWPalette);

        pucRow = (unsigned char*)rspqImage->GetImage();
        pucComp = pucRow;
        iRowSize = ( (m_iCIWidth & 7) != 0 ? m_iCIWidth/8 + 1 : m_iCIWidth/8 );
        if ( usComprCode == 0 )
        {
            // no compression
            Stream qChannelStream(m_eStreaming,m_qFilename,m_iIDOffset+2,iRowSize*m_iCIHeight,false,bSuccess);
            PROCESS_STREAM_SUCCESS_FOR_LOADING(bSuccess);
            for (int iY = 0; iY < m_iCIHeight; iY++)
            {
                for (int iX = 0; iX < m_iCIWidth; iX++)
                {
                    qChannelStream.ReadUBits(iValue,1);
                    *pucComp++ = ( iValue == 0 ? 1 : 0 );
                }

                qChannelStream.AlignBytes();

                pucRow += rspqImage->GetRowStride();
                pucComp = pucRow;

                qProgress.MakeAdvance();
            }
            qChannelStream.Finalize();
        }
        else
        {
            // compressed
            for (int iY = 0; iY < m_iCIHeight; iY++)
            {
                // unpack
                StillArray<char> qDecompressedRow;
                Stream qRowStream(m_eStreaming,m_qFilename,m_iIDOffset+iProceeded,qByteCounts[iY],false,bSuccess);
                PROCESS_STREAM_SUCCESS_FOR_LOADING(bSuccess);
                CONDITIONAL_THROW_EXCEPTION(
                    CompressionPackBits::Decompress(qRowStream,qByteCounts[iY],qDecompressedRow),
                    ET_FORMAT_MISMATCH);
                qRowStream.Finalize();
                iProceeded += qByteCounts[iY];

                CONDITIONAL_THROW_EXCEPTION(( qDecompressedRow.GetQuantity() == iRowSize ),ET_FORMAT_MISMATCH);

                iBytePos = 0;
                int iBitRatio = 0;
                for (int iX = 0; iX < m_iCIWidth; iX++)
                {
                    iBytePos += Storage::ReadUBits(qDecompressedRow+iBytePos,iBitRatio,1,iValue);
                    *pucComp++ = ( iValue == 0 ? 1 : 0 );
                }

                pucRow += rspqImage->GetRowStride();
                pucComp = pucRow;

                qProgress.MakeAdvance();
            }
        }
    }
    else if ( m_eColorModel == CMT_GRAYSCALE )
    {
        if ( m_iBitsPerChannel == 8 )
            rspqImage = new RasterImage(RasterImage::IT_GRAYSCALE_8,m_iCIWidth,m_iCIHeight);
        else  // m_iBitsPerChannel = 16
            rspqImage = new RasterImage(RasterImage::IT_GRAYSCALE_16,m_iCIWidth,m_iCIHeight);

        // read channels
        pucRow = (unsigned char*)rspqImage->GetImage();
        pucComp = pucRow;
        if ( m_iBitsPerChannel == 8 )
        {
            iRowSize = m_iCIWidth;

            if ( usComprCode == 0 )
            {
                // no compression
                iImageSize = iRowSize*m_iCIHeight;
                Stream qChannelStream(m_eStreaming,m_qFilename,m_iIDOffset+2,iImageSize,false,bSuccess);
                PROCESS_STREAM_SUCCESS_FOR_LOADING(bSuccess);
                for (int iY = 0; iY < m_iCIHeight; iY++)
                {
                    for (int iX = 0; iX < m_iCIWidth; iX++)
                    {
                        qChannelStream.Read(*pucComp);
                        pucComp += rspqImage->GetBytesPerPixel();
                    }

                    pucRow += rspqImage->GetRowStride();
                    pucComp = pucRow;

                    qProgress.MakeAdvance();
                }
                qChannelStream.Finalize();
            }
            else
            {
                // compressed
                for (int iY = 0; iY < m_iCIHeight; iY++)
                {
                    // unpack
                    StillArray<char> qDecompressedRow;
                    Stream qRowStream(m_eStreaming,m_qFilename,m_iIDOffset+iProceeded,qByteCounts[iY],false,
                        bSuccess);
                    PROCESS_STREAM_SUCCESS_FOR_LOADING(bSuccess);
                    CONDITIONAL_THROW_EXCEPTION(
                        CompressionPackBits::Decompress(qRowStream,qByteCounts[iY],qDecompressedRow),
                        ET_FORMAT_MISMATCH);
                    qRowStream.Finalize();
                    iProceeded += qByteCounts[iY];

                    CONDITIONAL_THROW_EXCEPTION(( qDecompressedRow.GetQuantity() == iRowSize ),
                        ET_FORMAT_MISMATCH);

                    for (int iX = 0; iX < m_iCIWidth; iX++)
                    {
                        *pucComp = qDecompressedRow[iX];
                        pucComp += rspqImage->GetBytesPerPixel();
                    }

                    pucRow += rspqImage->GetRowStride();
                    pucComp = pucRow;

                    qProgress.MakeAdvance();
                }
            }
        }
        else  // m_iBitsPerChannel = 16
        {
            iRowSize = m_iCIWidth*2;

            if ( usComprCode == 0 )
            {
                // no compression
                Stream qChannelStream(m_eStreaming,m_qFilename,m_iIDOffset+2,iRowSize*m_iCIHeight,false,bSuccess);
                PROCESS_STREAM_SUCCESS_FOR_LOADING(bSuccess);
                for (int iY = 0; iY < m_iCIHeight; iY++)
                {
                    for (int iX = 0; iX < m_iCIWidth; iX++)
                    {
                        qChannelStream.Read(usValue);
                        MemCpy(pucComp,&usValue,2);
                        pucComp += rspqImage->GetBytesPerPixel();
                    }

                    pucRow += rspqImage->GetRowStride();
                    pucComp = pucRow;

                    qProgress.MakeAdvance();
                }
                qChannelStream.Finalize();
            }
            else
            {
                // compressed
                for (int iY = 0; iY < m_iCIHeight; iY++)
                {
                    // unpack
                    StillArray<char> qDecompressedRow;
                    Stream qRowStream(m_eStreaming,m_qFilename,m_iIDOffset+iProceeded,qByteCounts[iY],false,
                        bSuccess);
                    PROCESS_STREAM_SUCCESS_FOR_LOADING(bSuccess);
                    CONDITIONAL_THROW_EXCEPTION(
                        CompressionPackBits::Decompress(qRowStream,qByteCounts[iY],qDecompressedRow),
                        ET_FORMAT_MISMATCH);
                    qRowStream.Finalize();
                    iProceeded += qByteCounts[iY];

                    CONDITIONAL_THROW_EXCEPTION(( qDecompressedRow.GetQuantity() == iRowSize ),
                        ET_FORMAT_MISMATCH);

                    iBytePos = 0;
                    for (int iX = 0; iX < m_iCIWidth; iX++)
                    {
                        iBytePos += Storage::Read2be(qDecompressedRow+iBytePos,1,pucComp);
                        pucComp += rspqImage->GetBytesPerPixel();
                    }

                    pucRow += rspqImage->GetRowStride();
                    pucComp = pucRow;

                    qProgress.MakeAdvance();
                }
            }
        }
    }
    else if ( m_eColorModel == CMT_PALETTED )
    {
        // read the palette
        RasterImage::Palette qPalette(256);
        Stream qPaletteStream(m_eStreaming,m_qFilename,30,768,false,bSuccess);
        PROCESS_STREAM_SUCCESS_FOR_LOADING(bSuccess);
        for (int i = 0; i < 768; i++)
            qPaletteStream.Read(qPalette[i & 255].Components[2-(i >> 8)]);
        qPaletteStream.Finalize();

        rspqImage = new RasterImage(RasterImage::IT_PALETTED,m_iCIWidth,m_iCIHeight,qPalette);

        pucRow = (unsigned char*)rspqImage->GetImage();
        pucComp = pucRow;
        iRowSize = m_iCIWidth;
        if ( usComprCode == 0 )
        {
            // no compression
            iImageSize = iRowSize*m_iCIHeight;
            Stream qChannelStream(m_eStreaming,m_qFilename,m_iIDOffset+2,iImageSize,false,bSuccess);
            PROCESS_STREAM_SUCCESS_FOR_LOADING(bSuccess);
            for (int iY = 0; iY < m_iCIHeight; iY++)
            {
                for (int iX = 0; iX < m_iCIWidth; iX++)
                {
                    qChannelStream.Read(*pucComp);
                    pucComp += rspqImage->GetBytesPerPixel();
                }

                pucRow += rspqImage->GetRowStride();
                pucComp = pucRow;

                qProgress.MakeAdvance();
            }
            qChannelStream.Finalize();
        }
        else
        {
            // compressed
            for (int iY = 0; iY < m_iCIHeight; iY++)
            {
                // unpack
                StillArray<char> qDecompressedRow;
                Stream qRowStream(m_eStreaming,m_qFilename,m_iIDOffset+iProceeded,qByteCounts[iY],false,bSuccess);
                PROCESS_STREAM_SUCCESS_FOR_LOADING(bSuccess);
                CONDITIONAL_THROW_EXCEPTION(
                    CompressionPackBits::Decompress(qRowStream,qByteCounts[iY],qDecompressedRow),
                    ET_FORMAT_MISMATCH);
                qRowStream.Finalize();
                iProceeded += qByteCounts[iY];

                CONDITIONAL_THROW_EXCEPTION(( qDecompressedRow.GetQuantity() == iRowSize ),
                    ET_FORMAT_MISMATCH);

                for (int iX = 0; iX < m_iCIWidth; iX++)
                {
                    *pucComp = qDecompressedRow[iX];
                    pucComp += rspqImage->GetBytesPerPixel();
                }

                pucRow += rspqImage->GetRowStride();
                pucComp = pucRow;

                qProgress.MakeAdvance();
            }
        }
    }
    else if ( m_eColorModel == CMT_RGB || m_eColorModel == CMT_LAB )
    {
        if ( m_eColorModel == CMT_RGB )
        {
            if ( m_iBitsPerChannel == 8 )
                rspqImage = new RasterImage(RasterImage::IT_RGB_8_8_8,m_iCIWidth,m_iCIHeight);
            else  // m_iBitsPerChannel = 16
                rspqImage = new RasterImage(RasterImage::IT_RGB_16_16_16,m_iCIWidth,m_iCIHeight);
        }
        else  // m_eColorModel = CMT_LAB
        {
            if ( m_iBitsPerChannel == 8 )
                rspqImage = new RasterImage(RasterImage::IT_LAB_8_8_8,m_iCIWidth,m_iCIHeight);
            else  // m_iBitsPerChannel = 16
                rspqImage = new RasterImage(RasterImage::IT_LAB_16_16_16,m_iCIWidth,m_iCIHeight);
        }

        // read channels
        if ( m_iBitsPerChannel == 8 )
        {
            iRowSize = m_iCIWidth;

            if ( usComprCode == 0 )
            {
                // no compression
                iChannelSize = iRowSize*m_iCIHeight;
                for (int i = 0; i < 3; i++)
                {
                    Stream qChannelStream(m_eStreaming,m_qFilename,m_iIDOffset+2+i*iChannelSize,iChannelSize,false,
                        bSuccess);
                    PROCESS_STREAM_SUCCESS_FOR_LOADING(bSuccess);
                    pucRow = (unsigned char*)rspqImage->GetImage();
                    pucComp = pucRow;
                    for (int iY = 0; iY < m_iCIHeight; iY++)
                    {
                        for (int iX = 0; iX < m_iCIWidth; iX++)
                        {
                            qChannelStream.Read(pucComp[i]);
                            pucComp += rspqImage->GetBytesPerPixel();
                        }

                        pucRow += rspqImage->GetRowStride();
                        pucComp = pucRow;

                        qProgress.MakeAdvance();
                    }
                    qChannelStream.Finalize();
                }
            }
            else
            {
                // compressed
                for (int i = 0; i < 3; i++)
                {
                    pucRow = (unsigned char*)rspqImage->GetImage();
                    pucComp = pucRow;
                    for (int iY = 0; iY < m_iCIHeight; iY++)
                    {
                        // unpack
                        iByteCount = qByteCounts[i*m_iCIHeight+iY];
                        StillArray<char> qDecompressedRow;
                        Stream qRowStream(m_eStreaming,m_qFilename,m_iIDOffset+iProceeded,iByteCount,false,
                            bSuccess);
                        PROCESS_STREAM_SUCCESS_FOR_LOADING(bSuccess);
                        CONDITIONAL_THROW_EXCEPTION(
                            CompressionPackBits::Decompress(qRowStream,iByteCount,qDecompressedRow),
                            ET_FORMAT_MISMATCH);
                        qRowStream.Finalize();
                        iProceeded += iByteCount;

                        CONDITIONAL_THROW_EXCEPTION(( qDecompressedRow.GetQuantity() == iRowSize ),
                            ET_FORMAT_MISMATCH);

                        for (int iX = 0; iX < m_iCIWidth; iX++)
                        {
                            pucComp[i] = qDecompressedRow[iX];
                            pucComp += rspqImage->GetBytesPerPixel();
                        }

                        pucRow += rspqImage->GetRowStride();
                        pucComp = pucRow;

                        qProgress.MakeAdvance();
                    }
                }
            }
        }
        else  // m_iBitsPerChannel = 16
        {
            iRowSize = m_iCIWidth*2;

            if ( usComprCode == 0 )
            {
                // no compression
                iChannelSize = iRowSize*m_iCIHeight;
                for (int i = 0; i < 3; i++)
                {
                    Stream qChannelStream(m_eStreaming,m_qFilename,m_iIDOffset+2+i*iChannelSize,iChannelSize,false,
                        bSuccess);
                    PROCESS_STREAM_SUCCESS_FOR_LOADING(bSuccess);
                    pucRow = (unsigned char*)rspqImage->GetImage();
                    pucComp = pucRow;
                    iVar = i*2;
                    for (int iY = 0; iY < m_iCIHeight; iY++)
                    {
                        for (int iX = 0; iX < m_iCIWidth; iX++)
                        {
                            qChannelStream.Read(usValue);
                            MemCpy(pucComp+iVar,&usValue,2);
                            pucComp += rspqImage->GetBytesPerPixel();
                        }

                        pucRow += rspqImage->GetRowStride();
                        pucComp = pucRow;

                        qProgress.MakeAdvance();
                    }
                    qChannelStream.Finalize();
                }
            }
            else
            {
                // compressed
                for (int i = 0; i < 3; i++)
                {
                    pucRow = (unsigned char*)rspqImage->GetImage();
                    pucComp = pucRow;
                    iVar = i*2;
                    for (int iY = 0; iY < m_iCIHeight; iY++)
                    {
                        // unpack
                        iByteCount = qByteCounts[i*m_iCIHeight+iY];
                        StillArray<char> qDecompressedRow;
                        Stream qRowStream(m_eStreaming,m_qFilename,m_iIDOffset+iProceeded,iByteCount,false,
                            bSuccess);
                        PROCESS_STREAM_SUCCESS_FOR_LOADING(bSuccess);
                        CONDITIONAL_THROW_EXCEPTION(
                            CompressionPackBits::Decompress(qRowStream,iByteCount,qDecompressedRow),
                            ET_FORMAT_MISMATCH);
                        qRowStream.Finalize();
                        iProceeded += iByteCount;

                        CONDITIONAL_THROW_EXCEPTION(( qDecompressedRow.GetQuantity() == iRowSize ),
                            ET_FORMAT_MISMATCH);

                        iBytePos = 0;
                        for (int iX = 0; iX < m_iCIWidth; iX++)
                        {
                            iBytePos += Storage::Read2be(qDecompressedRow+iBytePos,1,pucComp+iVar);
                            pucComp += rspqImage->GetBytesPerPixel();
                        }

                        pucRow += rspqImage->GetRowStride();
                        pucComp = pucRow;

                        qProgress.MakeAdvance();
                    }
                }
            }
        }
    }
    else  // m_eColorModel = CMT_CMYK
    {
        if ( m_iBitsPerChannel == 8 )
            rspqImage = new RasterImage(RasterImage::IT_CMYK_8_8_8_8,m_iCIWidth,m_iCIHeight);
        else  // m_iBitsPerChannel = 16
            rspqImage = new RasterImage(RasterImage::IT_CMYK_16_16_16_16,m_iCIWidth,m_iCIHeight);

        // read channels
        if ( m_iBitsPerChannel == 8 )
        {
            iRowSize = m_iCIWidth;

            if ( usComprCode == 0 )
            {
                // no compression
                iChannelSize = iRowSize*m_iCIHeight;
                for (int i = 0; i < 4; i++)
                {
                    Stream qChannelStream(m_eStreaming,m_qFilename,m_iIDOffset+2+i*iChannelSize,iChannelSize,false,
                        bSuccess);
                    PROCESS_STREAM_SUCCESS_FOR_LOADING(bSuccess);
                    pucRow = (unsigned char*)rspqImage->GetImage();
                    pucComp = pucRow;
                    for (int iY = 0; iY < m_iCIHeight; iY++)
                    {
                        for (int iX = 0; iX < m_iCIWidth; iX++)
                        {
                            qChannelStream.Read(pucComp[i]);
                            pucComp[i] = 255 - pucComp[i];
                            pucComp += rspqImage->GetBytesPerPixel();
                        }

                        pucRow += rspqImage->GetRowStride();
                        pucComp = pucRow;

                        qProgress.MakeAdvance();
                    }
                    qChannelStream.Finalize();
                }
            }
            else
            {
                // compressed
                for (int i = 0; i < 4; i++)
                {
                    pucRow = (unsigned char*)rspqImage->GetImage();
                    pucComp = pucRow;
                    for (int iY = 0; iY < m_iCIHeight; iY++)
                    {
                        // unpack
                        iByteCount = qByteCounts[i*m_iCIHeight+iY];
                        StillArray<char> qDecompressedRow;
                        Stream qRowStream(m_eStreaming,m_qFilename,m_iIDOffset+iProceeded,iByteCount,false,
                            bSuccess);
                        PROCESS_STREAM_SUCCESS_FOR_LOADING(bSuccess);
                        CONDITIONAL_THROW_EXCEPTION(
                            CompressionPackBits::Decompress(qRowStream,iByteCount,qDecompressedRow),
                            ET_FORMAT_MISMATCH);
                        qRowStream.Finalize();
                        iProceeded += iByteCount;

                        CONDITIONAL_THROW_EXCEPTION(( qDecompressedRow.GetQuantity() == iRowSize ),
                            ET_FORMAT_MISMATCH);

                        for (int iX = 0; iX < m_iCIWidth; iX++)
                        {
                            pucComp[i] = qDecompressedRow[iX];
                            pucComp[i] = 255 - pucComp[i];
                            pucComp += rspqImage->GetBytesPerPixel();
                        }

                        pucRow += rspqImage->GetRowStride();
                        pucComp = pucRow;

                        qProgress.MakeAdvance();
                    }
                }
            }
        }
        else  // m_iBitsPerChannel = 16
        {
            iRowSize = m_iCIWidth*2;

            if ( usComprCode == 0 )
            {
                // no compression
                iChannelSize = iRowSize*m_iCIHeight;
                for (int i = 0; i < 4; i++)
                {
                    Stream qChannelStream(m_eStreaming,m_qFilename,m_iIDOffset+2+i*iChannelSize,iChannelSize,false,
                        bSuccess);
                    PROCESS_STREAM_SUCCESS_FOR_LOADING(bSuccess);
                    pucRow = (unsigned char*)rspqImage->GetImage();
                    pucComp = pucRow;
                    iVar = i*2;
                    for (int iY = 0; iY < m_iCIHeight; iY++)
                    {
                        for (int iX = 0; iX < m_iCIWidth; iX++)
                        {
                            qChannelStream.Read(usValue);
                            usValue = 65535 - usValue;
                            MemCpy(pucComp+iVar,&usValue,2);
                            pucComp += rspqImage->GetBytesPerPixel();
                        }

                        pucRow += rspqImage->GetRowStride();
                        pucComp = pucRow;

                        qProgress.MakeAdvance();
                    }
                    qChannelStream.Finalize();
                }
            }
            else
            {
                // compressed
                for (int i = 0; i < 4; i++)
                {
                    pucRow = (unsigned char*)rspqImage->GetImage();
                    pucComp = pucRow;
                    iVar = i*2;
                    for (int iY = 0; iY < m_iCIHeight; iY++)
                    {
                        // unpack
                        iByteCount = qByteCounts[i*m_iCIHeight+iY];
                        StillArray<char> qDecompressedRow;
                        Stream qRowStream(m_eStreaming,m_qFilename,m_iIDOffset+iProceeded,iByteCount,false,
                            bSuccess);
                        PROCESS_STREAM_SUCCESS_FOR_LOADING(bSuccess);
                        CONDITIONAL_THROW_EXCEPTION(
                            CompressionPackBits::Decompress(qRowStream,iByteCount,qDecompressedRow),
                            ET_FORMAT_MISMATCH);
                        qRowStream.Finalize();
                        iProceeded += iByteCount;

                        CONDITIONAL_THROW_EXCEPTION(( qDecompressedRow.GetQuantity() == iRowSize ),
                            ET_FORMAT_MISMATCH);

                        iBytePos = 0;
                        for (int iX = 0; iX < m_iCIWidth; iX++)
                        {
                            iBytePos += Storage::Read2be(qDecompressedRow+iBytePos,1,&usValue);
                            usValue = 65535 - usValue;
                            MemCpy(pucComp+iVar,&usValue,2);
                            pucComp += rspqImage->GetBytesPerPixel();
                        }

                        pucRow += rspqImage->GetRowStride();
                        pucComp = pucRow;

                        qProgress.MakeAdvance();
                    }
                }
            }
        }
    }

    return true;
}
//------------------------------------------------------------------------------------------------------------------
bool FormatPsd::LoadLayer (Layer& rqLayer, int iLayer)
{
    assert( 0 <= iLayer && iLayer < m_iLayerQuantity );
    assert( m_eColorModel != CMT_BITMAP && m_eColorModel != CMT_PALETTED );

    // layers are allowed only for a PSD file with 8 bits per sample
    CONDITIONAL_THROW_EXCEPTION(( m_iBitsPerChannel == 8 ),ET_FORMAT_MISMATCH);

    // main channel quantity
    int iMCQ;
    switch ( m_eColorModel )
    {
    case CMT_GRAYSCALE:
        iMCQ = 1;
        break;
    case CMT_RGB:
        iMCQ = 3;
        break;
    case CMT_CMYK:
        iMCQ = 4;
        break;
    case CMT_LAB:
        iMCQ = 3;
        break;
    }

    // layer attributes
    StillArray<LayerRecord> qLRecords(m_iLayerQuantity);
    bool bSuccess;
    int iSize, iRemNRP, iNextReadPos = m_iAfterLQOffset;
    for (int i = 0; i < m_iLayerQuantity; i++)
    {
        LayerRecord& rqLR = qLRecords[i];
        Layer qLayer;

        Stream qLRFirstStream(m_eStreaming,m_qFilename,iNextReadPos,18,false,bSuccess);
        PROCESS_STREAM_SUCCESS_FOR_LOADING(bSuccess);

        int iTop, iLeft, iBottom, iRight;
        qLRFirstStream.Read(iTop);
        qLRFirstStream.Read(iLeft);
        qLRFirstStream.Read(iBottom);
        qLRFirstStream.Read(iRight);
        qLayer.PosRect.X = iLeft;
        qLayer.PosRect.Y = iTop;
        qLayer.PosRect.Width = iRight - iLeft;
        qLayer.PosRect.Height = iBottom - iTop;
        CONDITIONAL_THROW_EXCEPTION(( qLayer.PosRect.Width >= 0 && qLayer.PosRect.Height >= 0 ),
            ET_FORMAT_MISMATCH);

        unsigned short usChannelQuantity;
        qLRFirstStream.Read(usChannelQuantity);
        CONDITIONAL_THROW_EXCEPTION(( 1 <= usChannelQuantity && usChannelQuantity <= 24 ),ET_FORMAT_MISMATCH);
        CONDITIONAL_THROW_EXCEPTION(( usChannelQuantity >= iMCQ ),ET_FORMAT_MISMATCH);

        qLRFirstStream.Finalize();
        iNextReadPos += 18;

        rqLR.PosRect = qLayer.PosRect;
        rqLR.ChannelQuantity = usChannelQuantity;

        iSize = usChannelQuantity*6 + 16;
        Stream qLRSecondStream(m_eStreaming,m_qFilename,iNextReadPos,iSize,false,bSuccess);
        PROCESS_STREAM_SUCCESS_FOR_LOADING(bSuccess);

        rqLR.ChannelInfos.SetQuantity(usChannelQuantity);
        for (int i0 = 0; i0 < usChannelQuantity; i0++)
        {
            ChannelInfo& rqCI = rqLR.ChannelInfos[i0];

            qLRSecondStream.Read(rqCI.Id);
            CONDITIONAL_THROW_EXCEPTION(( -2 <= rqCI.Id && rqCI.Id < iMCQ ),ET_FORMAT_MISMATCH);

            qLRSecondStream.Read(rqCI.Length);
        }

        char acBMS[4];
        qLRSecondStream.Read(4,acBMS);
        CONDITIONAL_THROW_EXCEPTION(( MemCmp(acBMS,"8BIM",4) == 0 ),ET_FORMAT_MISMATCH);

        char acBMK[4];
        qLRSecondStream.Read(4,acBMK);
        if ( MemCmp(acBMK,"norm",4) == 0 )
            qLayer.BlendingMode = BMT_NORMAL;
        else if ( MemCmp(acBMK,"diss",4) == 0 )
            qLayer.BlendingMode = BMT_DISSOLVE;
        else if ( MemCmp(acBMK,"dark",4) == 0 )
            qLayer.BlendingMode = BMT_DARKEN;
        else if ( MemCmp(acBMK,"mul ",4) == 0 )
            qLayer.BlendingMode = BMT_MULTIPLY;
        else if ( MemCmp(acBMK,"idiv",4) == 0 )
            qLayer.BlendingMode = BMT_COLOR_BURN;
        else if ( MemCmp(acBMK,"lbrn",4) == 0 )
            qLayer.BlendingMode = BMT_LINEAR_BURN;
        else if ( MemCmp(acBMK,"lite",4) == 0 )
            qLayer.BlendingMode = BMT_LIGHTEN;
        else if ( MemCmp(acBMK,"scrn",4) == 0 )
            qLayer.BlendingMode = BMT_SCREEN;
        else if ( MemCmp(acBMK,"div ",4) == 0 )
            qLayer.BlendingMode = BMT_COLOR_DODGE;
        else if ( MemCmp(acBMK,"lddg",4) == 0 )
            qLayer.BlendingMode = BMT_LINEAR_DODGE;
        else if ( MemCmp(acBMK,"over",4) == 0 )
            qLayer.BlendingMode = BMT_OVERLAY;
        else if ( MemCmp(acBMK,"sLit",4) == 0 )
            qLayer.BlendingMode = BMT_SOFT_LIGHT;
        else if ( MemCmp(acBMK,"hLit",4) == 0 )
            qLayer.BlendingMode = BMT_HARD_LIGHT;
        else if ( MemCmp(acBMK,"vLit",4) == 0 )
            qLayer.BlendingMode = BMT_VIVID_LIGHT;
        else if ( MemCmp(acBMK,"lLit",4) == 0 )
            qLayer.BlendingMode = BMT_LINEAR_LIGHT;
        else if ( MemCmp(acBMK,"pLit",4) == 0 )
            qLayer.BlendingMode = BMT_PIN_LIGHT;
        else if ( MemCmp(acBMK,"hMix",4) == 0 )
            qLayer.BlendingMode = BMT_HARD_MIX;
        else if ( MemCmp(acBMK,"diff",4) == 0 )
            qLayer.BlendingMode = BMT_DIFFERENCE;
        else if ( MemCmp(acBMK,"smud",4) == 0 )
            qLayer.BlendingMode = BMT_EXCLUSION;
        else if ( MemCmp(acBMK,"hue ",4) == 0 )
            qLayer.BlendingMode = BMT_HUE;
        else if ( MemCmp(acBMK,"sat ",4) == 0 )
            qLayer.BlendingMode = BMT_SATURATION;
        else if ( MemCmp(acBMK,"colr",4) == 0 )
            qLayer.BlendingMode = BMT_COLOR;
        else if ( MemCmp(acBMK,"lum ",4) == 0 )
            qLayer.BlendingMode = BMT_LUMINOSITY;
        else
            qLayer.BlendingMode = BMT_UNKNOWN;

        qLRSecondStream.Read(qLayer.Opacity);

        unsigned char ucClipping;
        qLRSecondStream.Read(ucClipping);
        switch ( ucClipping )
        {
        case 0:
            qLayer.Clipping = CTT_BASE;
            break;
        case 1:
            qLayer.Clipping = CTT_NON_BASE;
            break;
        default:
            THROW_EXCEPTION(ET_FORMAT_MISMATCH);
        }

        unsigned char ucFlags;
        qLRSecondStream.Read(ucFlags);
        qLayer.TransparencyProtected = ( (ucFlags & 1) == 1 );
        qLayer.Visible = ( ((ucFlags >> 1) & 1) == 0 );

        qLRSecondStream.OffsetBytePosition(1);

        unsigned int uiExtraDataSize;
        qLRSecondStream.Read(uiExtraDataSize);

        qLRSecondStream.Finalize();
        iNextReadPos += iSize;

        iRemNRP = iNextReadPos;

        // layer mask data
        Stream qLMDSizeStream(m_eStreaming,m_qFilename,iNextReadPos,4,false,bSuccess);
        PROCESS_STREAM_SUCCESS_FOR_LOADING(bSuccess);
        unsigned int uiLMDSize;
        qLMDSizeStream.Read(uiLMDSize);
        CONDITIONAL_THROW_EXCEPTION(( uiLMDSize == 0 || uiLMDSize == 20 ),ET_FORMAT_MISMATCH);
        qLMDSizeStream.Finalize();
        iNextReadPos += 4;
        if ( uiLMDSize != 0 )
        {
            Stream qMaskDataStream(m_eStreaming,m_qFilename,iNextReadPos,uiLMDSize,false,bSuccess);
            PROCESS_STREAM_SUCCESS_FOR_LOADING(bSuccess);

            qMaskDataStream.Read(iTop);
            qMaskDataStream.Read(iLeft);
            qMaskDataStream.Read(iBottom);
            qMaskDataStream.Read(iRight);
            rqLR.LMR.PosRect.X = iLeft;
            rqLR.LMR.PosRect.Y = iTop;
            rqLR.LMR.PosRect.Width = iRight - iLeft;
            rqLR.LMR.PosRect.Height = iBottom - iTop;

            qMaskDataStream.Read(rqLR.LMR.DefaultColor);
            CONDITIONAL_THROW_EXCEPTION(( rqLR.LMR.DefaultColor == 0 || rqLR.LMR.DefaultColor == 255 ),
                ET_FORMAT_MISMATCH);

            unsigned char ucFlags;
            qMaskDataStream.Read(ucFlags);
            rqLR.LMR.PositionRelativeToLayer = ( (ucFlags & 1) == 1 );
            rqLR.LMR.LayerMaskDisabled = ( ((ucFlags >> 1) & 1) == 1 );
            rqLR.LMR.InvertLayerMaskWhenBlending = ( ((ucFlags >> 2) & 1) == 1 );

            rqLR.LMR.IsPresent = true;

            qMaskDataStream.Finalize();
            iNextReadPos += uiLMDSize;
        }

        // layer blending ranges data
        Stream qLBRDLStream(m_eStreaming,m_qFilename,iNextReadPos,4,false,bSuccess);
        PROCESS_STREAM_SUCCESS_FOR_LOADING(bSuccess);
        unsigned int uiLBRDLength;
        qLBRDLStream.Read(uiLBRDLength);
        qLBRDLStream.Finalize();
        iNextReadPos += 4;
        if ( uiLBRDLength == 0 )
        {
            qLayer.HasBlendingRangesData = false;
        }
        else
        {
            CONDITIONAL_THROW_EXCEPTION(( (uiLBRDLength - 8) % 8 == 0 ),ET_FORMAT_MISMATCH);
            Stream qLBRDStream(m_eStreaming,m_qFilename,iNextReadPos,uiLBRDLength,false,bSuccess);
            PROCESS_STREAM_SUCCESS_FOR_LOADING(bSuccess);
            qLBRDStream.Read(qLayer.CGSrcBlack0);
            qLBRDStream.Read(qLayer.CGSrcBlack1);
            qLBRDStream.Read(qLayer.CGSrcWhite0);
            qLBRDStream.Read(qLayer.CGSrcWhite1);
            qLBRDStream.Read(qLayer.CGDstBlack0);
            qLBRDStream.Read(qLayer.CGDstBlack1);
            qLBRDStream.Read(qLayer.CGDstWhite0);
            qLBRDStream.Read(qLayer.CGDstWhite1);
            qLayer.ChannelRanges.SetQuantity((uiLBRDLength-8)/8);
            for (int i0 = 0; i0 < qLayer.ChannelRanges.GetQuantity(); i0++)
            {
                ChannelRange& rqChannelRange = qLayer.ChannelRanges[i0];

                qLBRDStream.Read(rqChannelRange.SrcBlack0);
                qLBRDStream.Read(rqChannelRange.SrcBlack1);
                qLBRDStream.Read(rqChannelRange.SrcWhite0);
                qLBRDStream.Read(rqChannelRange.SrcWhite1);
                qLBRDStream.Read(rqChannelRange.DstBlack0);
                qLBRDStream.Read(rqChannelRange.DstBlack1);
                qLBRDStream.Read(rqChannelRange.DstWhite0);
                qLBRDStream.Read(rqChannelRange.DstWhite1);
            }
            qLayer.HasBlendingRangesData = true;

            qLBRDStream.Finalize();
            iNextReadPos += uiLBRDLength;
        }

        // layer's name
        Stream qLNLStream(m_eStreaming,m_qFilename,iNextReadPos,1,false,bSuccess);
        PROCESS_STREAM_SUCCESS_FOR_LOADING(bSuccess);
        unsigned char ucLNL;
        qLNLStream.Read(ucLNL);
        qLNLStream.Finalize();
        iNextReadPos += 1;
        Stream qLNStream(m_eStreaming,m_qFilename,iNextReadPos,ucLNL,false,bSuccess);
        PROCESS_STREAM_SUCCESS_FOR_LOADING(bSuccess);
        qLayer.Name.SetLength(ucLNL);
        qLNStream.Read(ucLNL,qLayer.Name);
        qLNStream.Finalize();

        iNextReadPos = iRemNRP + uiExtraDataSize;

        if ( i == iLayer )
        {
            // that's the one
            rqLayer = qLayer;
        }
    }

    int iBytesPerSample = m_iBitsPerChannel/8;

    // allocate the image with alpha channel
    if ( m_eColorModel == CMT_GRAYSCALE )
    {
        rqLayer.ImagePtr = new RasterImage(RasterImage::IT_GRAYSCALE_WITH_ALPHA_8_8,
            rqLayer.PosRect.Width,rqLayer.PosRect.Height);
    }
    else if ( m_eColorModel == CMT_RGB )
    {
        rqLayer.ImagePtr = new RasterImage(RasterImage::IT_RGBA_8_8_8_8,
            rqLayer.PosRect.Width,rqLayer.PosRect.Height);
    }
    else if ( m_eColorModel == CMT_CMYK )
    {
        rqLayer.ImagePtr = new RasterImage(RasterImage::IT_CMYK_WITH_ALPHA_8_8_8_8_8,
            rqLayer.PosRect.Width,rqLayer.PosRect.Height);
    }
    else  // m_eColorModel = CMT_LAB
    {
        rqLayer.ImagePtr = new RasterImage(RasterImage::IT_LAB_WITH_ALPHA_8_8_8_8,
            rqLayer.PosRect.Width,rqLayer.PosRect.Height);
    }

    unsigned char* pucRow;
    unsigned char* pucComp;
    int iVar;

    // set alpha channel samples to the maximal value
    pucRow = (unsigned char*)rqLayer.ImagePtr->GetImage();
    pucComp = pucRow;
    iVar = iMCQ*iBytesPerSample;
    for (int iY = 0; iY < rqLayer.PosRect.Height; iY++)
    {
        for (int iX = 0; iX < rqLayer.PosRect.Width; iX++)
        {
            MemSet(pucComp+iVar,255,iBytesPerSample);
            pucComp += rqLayer.ImagePtr->GetBytesPerPixel();
        }

        pucRow += rqLayer.ImagePtr->GetRowStride();
        pucComp = pucRow;
    }

    // for a progress bar
    ProgressProbe qProgress(this);
    qProgress.SetAdvance(1.0f/qLRecords[iLayer].ChannelQuantity);

    // pixel data
    unsigned short usComprCode;
    StillArray<unsigned short> qByteCounts;
    int iByteCountsSize, iChannelSize, iRowSize, iOffsetX, iOffsetY;
    bool bIsMask, bIsMainChannel, bInvertMask;
    unsigned char ucValue;
    Recti PosRectToUse;
    for (int i0 = 0; /**/; i0++)
    {
        LayerRecord& rqLR = qLRecords[i0];

        for (int i1 = 0; i1 < rqLR.ChannelQuantity; i1++)
        {
            const ChannelInfo& rqChannelInfo = rqLR.ChannelInfos[i1];

            bIsMask = ( rqChannelInfo.Id == -2 );
            if ( !bIsMask )
            {
                PosRectToUse = rqLR.PosRect;
            }
            else
            {
                // it's a mask, make its PosRect global
                CONDITIONAL_THROW_EXCEPTION(rqLR.LMR.IsPresent,ET_FORMAT_MISMATCH);
                PosRectToUse = rqLR.LMR.PosRect;
                if ( rqLR.LMR.PositionRelativeToLayer )
                {
                    PosRectToUse.X += rqLR.PosRect.X;
                    PosRectToUse.Y += rqLR.PosRect.Y;
                }
            }

            Stream qCCStream(m_eStreaming,m_qFilename,iNextReadPos,2,false,bSuccess);
            PROCESS_STREAM_SUCCESS_FOR_LOADING(bSuccess);
            qCCStream.Read(usComprCode);
            qCCStream.Finalize();
            iNextReadPos += 2;
            if ( usComprCode == 0 )
            {
                // no compression
                iChannelSize = PosRectToUse.Width*PosRectToUse.Height*iBytesPerSample;
            }
            else
            {
                // RLE compression; read rows' byte counts
                qByteCounts.SetQuantity(PosRectToUse.Height,false);
                iByteCountsSize = PosRectToUse.Height*2;
                Stream qBCStream(m_eStreaming,m_qFilename,iNextReadPos,iByteCountsSize,false,bSuccess);
                PROCESS_STREAM_SUCCESS_FOR_LOADING(bSuccess);
                iChannelSize = 0;
                for (int i = 0; i < PosRectToUse.Height; i++)
                {
                    qBCStream.Read(qByteCounts[i]);
                    iChannelSize += qByteCounts[i];
                }
                qBCStream.Finalize();
                iNextReadPos += iByteCountsSize;
            }
            iRemNRP = iNextReadPos;

            if ( i0 == iLayer && !(bIsMask && rqLR.LMR.LayerMaskDisabled) )
            {
                bIsMainChannel = ( rqChannelInfo.Id >= 0 );
                iRowSize = PosRectToUse.Width*iBytesPerSample;
                iOffsetX = PosRectToUse.X - rqLR.PosRect.X;
                iOffsetY = PosRectToUse.Y - rqLR.PosRect.Y;
                iVar = iOffsetX*rqLayer.ImagePtr->GetBytesPerPixel();
                bInvertMask = ( bIsMask && rqLR.LMR.InvertLayerMaskWhenBlending );

                pucRow = (unsigned char*)(rqLayer.ImagePtr->GetImage() + iOffsetY*rqLayer.ImagePtr->GetRowStride());
                pucComp = pucRow + iVar;
                if ( usComprCode == 0 )
                {
                    // no compression
                    Stream qChannelStream(m_eStreaming,m_qFilename,iNextReadPos,iChannelSize,false,bSuccess);
                    PROCESS_STREAM_SUCCESS_FOR_LOADING(bSuccess);

                    for (int iY = 0; iY < PosRectToUse.Height; iY++)
                    {
                        for (int iX = 0; iX < PosRectToUse.Width; iX++)
                        {
                            qChannelStream.Read(ucValue);
                            ASSIGN_SAMPLE;
                        }

                        pucRow += rqLayer.ImagePtr->GetRowStride();
                        pucComp = pucRow + iVar;
                    }

                    qChannelStream.Finalize();
                }
                else
                {
                    // compressed
                    for (int iY = 0; iY < PosRectToUse.Height; iY++)
                    {
                        StillArray<char> qDecompressedRow;
                        Stream qRowStream(m_eStreaming,m_qFilename,iNextReadPos,qByteCounts[iY],false,bSuccess);
                        PROCESS_STREAM_SUCCESS_FOR_LOADING(bSuccess);
                        CONDITIONAL_THROW_EXCEPTION(
                            CompressionPackBits::Decompress(qRowStream,qByteCounts[iY],qDecompressedRow),
                            ET_FORMAT_MISMATCH);
                        qRowStream.Finalize();
                        iNextReadPos += qByteCounts[iY];

                        CONDITIONAL_THROW_EXCEPTION(( qDecompressedRow.GetQuantity() == iRowSize ),
                            ET_FORMAT_MISMATCH);

                        for (int iX = 0; iX < PosRectToUse.Width; iX++)
                        {
                            ucValue = qDecompressedRow[iX];
                            ASSIGN_SAMPLE;
                        }

                        pucRow += rqLayer.ImagePtr->GetRowStride();
                        pucComp = pucRow + iVar;
                    }
                }

                qProgress.MakeAdvance();
            }

            iNextReadPos = iRemNRP + iChannelSize;
        }

        if ( i0 == iLayer )
            break;
    }

    return true;
}
//------------------------------------------------------------------------------------------------------------------



