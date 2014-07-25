#include "QFormatHdr.h"
#include "QExptWarnBase.mcr"
#include "QFormatBase.mcr"
#include "QFormatHdr.mcr"
#include "QProgressProbe.h"
#include "QStorage.h"
#include "QColorModelConversion.h"
#include "QRasterImageTransformation.h"
using namespace Q;

// support for conversion from XYZ color model to RGB
const float FormatHdr::ms_fXyzToRgbGamma(1.0f);

//------------------------------------------------------------------------------------------------------------------
FormatHdr::FormatHdr (const RasterImagePtr& rspqImage)
    :
    m_spqImageSaveTo(rspqImage),
    m_iWidth(rspqImage->GetWidth()),
    m_iHeight(rspqImage->GetHeight()),
    m_eCompression(CT_NONE),
    m_bHasExposureInfo(false),
    m_bHasColorCorrectionInfo(false),
    m_bHasSoftwareInfo(false),
    m_bHasPixelAspectRatioInfo(false),
    m_bHasGammaInfo(false),
    m_eOrientation(OT_TOP_LEFT)
{
    assert( rspqImage->GetWidth() != 0 && rspqImage->GetHeight() != 0 );
    assert( rspqImage->GetColorModel() == RasterImage::CMT_RGB );
    assert( !rspqImage->HasVisibilityMask() );
}
//------------------------------------------------------------------------------------------------------------------
bool FormatHdr::SaveImage (const char* acFilename)
{
    char cNL = '\n';
    char cWS = ' ';

    Stream qSaveStream(acFilename,true);

    // signature
    qSaveStream.Write(10,"#?RADIANCE");
    qSaveStream.Write(cNL);

    // Header.
    qSaveStream.Write(22,"FORMAT=32-bit_rle_rgbe");
    qSaveStream.Write(cNL);

    if ( m_bHasExposureInfo )
    {
        qSaveStream.Write(9,"EXPOSURE=");
        String qStr(m_fExposure,true);
        qSaveStream.Write(qStr.GetLength(),qStr);
        qSaveStream.Write(cNL);
    }

    if ( m_bHasColorCorrectionInfo )
    {
        qSaveStream.Write(10,"COLORCORR=");
        String qStr0(m_afColorCorr[0],true);
        String qStr1(m_afColorCorr[1],true);
        String qStr2(m_afColorCorr[2],true);
        qSaveStream.Write(cWS);
        qSaveStream.Write(qStr0.GetLength(),qStr0);
        qSaveStream.Write(cWS);
        qSaveStream.Write(qStr1.GetLength(),qStr1);
        qSaveStream.Write(cWS);
        qSaveStream.Write(qStr2.GetLength(),qStr2);
        qSaveStream.Write(cNL);
    }

    if ( m_bHasSoftwareInfo )
    {
        qSaveStream.Write(9,"SOFTWARE=");
        qSaveStream.Write(m_qSoftware.GetLength(),m_qSoftware);
        qSaveStream.Write(cNL);
    }

    if ( m_bHasPixelAspectRatioInfo )
    {
        qSaveStream.Write(10,"PIXASPECT=");
        String qStr(m_fPixelAspectRatio,true);
        qSaveStream.Write(qStr.GetLength(),qStr);
        qSaveStream.Write(cNL);
    }

    if ( m_bHasGammaInfo )
    {
        qSaveStream.Write(6,"GAMMA=");
        String qStr(m_fGamma,true);
        qSaveStream.Write(qStr.GetLength(),qStr);
        qSaveStream.Write(cNL);
    }

    qSaveStream.Write(cNL);

    // image dimensions and orientation
    String qDim0, qDim1;
    char cSign0, cSign1, cAxis0, cAxis1;
    switch ( m_eOrientation )
    {
    case OT_TOP_LEFT:
        qDim0 = String(m_iHeight);
        qDim1 = String(m_iWidth);
        cSign0 = '-';
        cSign1 = '+';
        cAxis0 = 'Y';
        cAxis1 = 'X';
        break;
    case OT_TOP_RIGHT:
        qDim0 = String(m_iHeight);
        qDim1 = String(m_iWidth);
        cSign0 = '-';
        cSign1 = '-';
        cAxis0 = 'Y';
        cAxis1 = 'X';
        break;
    case OT_BOTTOM_RIGHT:
        qDim0 = String(m_iHeight);
        qDim1 = String(m_iWidth);
        cSign0 = '+';
        cSign1 = '-';
        cAxis0 = 'Y';
        cAxis1 = 'X';
        break;
    case OT_BOTTOM_LEFT:
        qDim0 = String(m_iHeight);
        qDim1 = String(m_iWidth);
        cSign0 = '+';
        cSign1 = '+';
        cAxis0 = 'Y';
        cAxis1 = 'X';
        break;
    case OT_LEFT_TOP:
        qDim0 = String(m_iWidth);
        qDim1 = String(m_iHeight);
        cSign0 = '+';
        cSign1 = '-';
        cAxis0 = 'X';
        cAxis1 = 'Y';
        break;
    case OT_RIGHT_TOP:
        qDim0 = String(m_iWidth);
        qDim1 = String(m_iHeight);
        cSign0 = '+';
        cSign1 = '+';
        cAxis0 = 'X';
        cAxis1 = 'Y';
        break;
    case OT_RIGHT_BOTTOM:
        qDim0 = String(m_iWidth);
        qDim1 = String(m_iHeight);
        cSign0 = '-';
        cSign1 = '+';
        cAxis0 = 'X';
        cAxis1 = 'Y';
        break;
    case OT_LEFT_BOTTOM:
        qDim0 = String(m_iWidth);
        qDim1 = String(m_iHeight);
        cSign0 = '-';
        cSign1 = '-';
        cAxis0 = 'X';
        cAxis1 = 'Y';
        break;
    }
    qSaveStream.Write(cSign0);
    qSaveStream.Write(cAxis0);
    qSaveStream.Write(cWS);
    qSaveStream.Write(qDim0.GetLength(),qDim0);
    qSaveStream.Write(cWS);
    qSaveStream.Write(cSign1);
    qSaveStream.Write(cAxis1);
    qSaveStream.Write(cWS);
    qSaveStream.Write(qDim1.GetLength(),qDim1);
    qSaveStream.Write(cNL);

    // for a progress bar
    ProgressProbe qProgress(this);
    qProgress.SetAdvance(1.0f/m_iHeight);

    // write image data
    ColorRgb qColorRgb;
    unsigned char aucByteQuartet[4];
    if ( m_eCompression == CT_NONE || m_iWidth < 8 || m_iWidth > 32767 )
    {
        for (int iY = 0; iY < m_iHeight; iY++)
        {
            for (int iX = 0; iX < m_iWidth; iX++)
            {
                qColorRgb = m_spqImageSaveTo->GetPixelRgb(iX,iY);
                GetByteQuartetFromFloatTriplet(qColorRgb,aucByteQuartet);
                qSaveStream.Write(4,aucByteQuartet);
            }

            qProgress.MakeAdvance();
        }
    }
    else  // m_eCompression = CT_RLE
    {
        int iCur, iBegRun, iRunCount, iOldRunCount, iNonrunCount;
        unsigned char aucBuff[2];
        unsigned char* aucData;
        int iNumOfSL = m_iHeight;
        StillArray<unsigned char> qScanlineBuffer(m_iWidth*4);
        while ( iNumOfSL > 0 )
        {
            aucByteQuartet[0] = 2;
            aucByteQuartet[1] = 2;
            aucByteQuartet[2] = m_iWidth >> 8;
            aucByteQuartet[3] = m_iWidth & 255;
            qSaveStream.Write(4,aucByteQuartet);
            for (int iX = 0; iX < m_iWidth; iX++)
            {
                qColorRgb = m_spqImageSaveTo->GetPixelRgb(iX,m_iHeight-iNumOfSL);
                GetByteQuartetFromFloatTriplet(qColorRgb,aucByteQuartet);
                for (int i = 0; i < 4; i++)
                    qScanlineBuffer[iX+i*m_iWidth] = aucByteQuartet[i];
            }
            for (int i = 0; i < 4; i++)
            {
                aucData = qScanlineBuffer + i*m_iWidth;
                iCur = 0;
                while ( iCur < m_iWidth )
                {
                    iBegRun = iCur;
                    iRunCount = iOldRunCount = 0;
                    while ( iRunCount < 4 && iBegRun < m_iWidth )
                    {
                        iBegRun += iRunCount;
                        iOldRunCount = iRunCount;
                        iRunCount = 1;
                        while ( iBegRun + iRunCount < m_iWidth && iRunCount < 127
                           &&   aucData[iBegRun] == aucData[iBegRun+iRunCount] )
                        {
                            iRunCount++;
                        }
                    }
                    if ( iOldRunCount > 1 && iOldRunCount == iBegRun - iCur )
                    {
                        aucBuff[0] = 128 + iOldRunCount;
                        aucBuff[1] = aucData[iCur];
                        qSaveStream.Write(2,aucBuff);
                        iCur = iBegRun;
                    }
                    while ( iCur < iBegRun )
                    {
                        iNonrunCount = iBegRun - iCur;
                        if ( iNonrunCount > 128 )
                            iNonrunCount = 128;
                        aucBuff[0] = iNonrunCount;
                        qSaveStream.Write(aucBuff[0]);
                        qSaveStream.Write(iNonrunCount,aucData+iCur);
                        iCur += iNonrunCount;
                    }
                    if ( iRunCount >= 4 )
                    {
                        aucBuff[0] = 128 + iRunCount;
                        aucBuff[1] = aucData[iBegRun];
                        qSaveStream.Write(2,aucBuff);
                        iCur += iRunCount;
                    }
                }
            }

            iNumOfSL--;

            qProgress.MakeAdvance();
        }
    }

    // flush to the file
    CONDITIONAL_THROW_EXCEPTION(qSaveStream.Finalize(),ET_CANNOT_ACCESS_FILE);

    return true;
}
//------------------------------------------------------------------------------------------------------------------
FormatHdr::FormatHdr (const char* acFilename, bool& rbSuccess)
    :
    FormatBase(acFilename),
    m_bHasSoftwareInfo(false),
    m_bHasPixelAspectRatioInfo(false),
    m_fPixelAspectRatio(1.0f),
    m_bHasGammaInfo(false),
    m_fGamma(1.0f),
    m_bInvCompCoeffsAreMeaning(false),
    m_bDoRelyOnExposureAndColorCorrection(false)
{
    bool bSuccess;

    int iNextReadPos = 0;

    // header
    bool bHasFormatInfo = false;
    bool bHasExposureInfo = false;
    bool bHasColorCorrInfo = false;
    float fCumulativeExposure = 1.0f;
    float afCumulativeColorCorr[3] = {1.0f, 1.0f, 1.0f};
    float afPrimaries[8] = {
        0.640f, 0.330f,
        0.290f, 0.600f,
        0.150f, 0.060f,
        0.333f, 0.333f};
    unsigned char ucByte;
    REACH_NEXT_STATEMENT;  // get to the first one
    int iVSBeginPos, iVSLength;
    for (/**/; /**/; /**/)
    {
        iVSBeginPos = iNextReadPos;
        REACH_NEXT_STATEMENT;
        iVSLength = iNextReadPos - 1 - iVSBeginPos;

        if ( iVSLength == 0 )
        {
            // end of the header
            break;
        }

        Stream qVSStream(m_eStreaming,m_qFilename,iVSBeginPos,iVSLength,true,bSuccess);
        String qVSStr;
        qVSStr.SetLength(iVSLength);
        qVSStream.Read(iVSLength,qVSStr);
        qVSStream.Finalize();

        // process the variable statement
        if ( qVSStr.BeginsWith("FORMAT=",false) )
        {
            // the file's format
            qVSStr.Remove(0,7);
            if ( qVSStr.BeginsWith("32-bit_rle_rgbe",false) )
                m_eFormat = FT_RLE_RGBE;
            else if ( qVSStr.BeginsWith("32-bit_rle_xyze",false) )
                m_eFormat = FT_RLE_XYZE;
            else
                C_THROW_EXCEPTION(ET_FORMAT_MISMATCH);
            bHasFormatInfo = true;
        }
        else if ( qVSStr.BeginsWith("EXPOSURE=",false) )
        {
            // multiplier that was applied to all pixels in the file
            qVSStr.Remove(0,9);
            float fExposure = (float)qVSStr.ToDouble();
            fCumulativeExposure *= fExposure;
            bHasExposureInfo = true;
        }
        else if ( qVSStr.BeginsWith("COLORCORR=",false) )
        {
            // color correction multiplier that was applied to the picture
            qVSStr.Remove(0,10);
            if ( qVSStr.BeginsWith(" ") )
                qVSStr.Remove(0,1);
            Array<String> qSplits;
            qVSStr.GetSplitsByChar(' ',qSplits);
            C_CONDITIONAL_THROW_EXCEPTION(( qSplits.GetQuantity() >= 3 ),ET_FORMAT_MISMATCH);
            for (int i = 0; i < 3; i++)
            {
                float fColorCorr = (float)qSplits[i].ToDouble();
                afCumulativeColorCorr[i] *= fColorCorr;
            }
            bHasColorCorrInfo = true;
        }
        else if ( qVSStr.BeginsWith("SOFTWARE=",false) )
        {
            // software version used to create the picture
            qVSStr.Remove(0,9);
            m_qSoftware = qVSStr;
            m_bHasSoftwareInfo = true;
        }
        else if ( qVSStr.BeginsWith("PIXASPECT=",false) )
        {
            // pixel aspect ratio
            qVSStr.Remove(0,10);
            float fPixelAspectRatio = (float)qVSStr.ToDouble();
            m_fPixelAspectRatio *= fPixelAspectRatio;
            m_bHasPixelAspectRatioInfo = true;
        }
        else if ( qVSStr.BeginsWith("GAMMA=",false) )
        {
            // applied gamma
            qVSStr.Remove(0,6);
            float fGamma = (float)qVSStr.ToDouble();
            m_fGamma *= fGamma;
            m_bHasGammaInfo = true;
        }
        else if ( qVSStr.BeginsWith("PRIMARIES=",false) )
        {
            // CIE(x,y) chromaticity coordinates and white point
            qVSStr.Remove(0,10);
            if ( qVSStr.BeginsWith(" ") )
                qVSStr.Remove(0,1);
            Array<String> qSplits;
            qVSStr.GetSplitsByChar(' ',qSplits);
            C_CONDITIONAL_THROW_EXCEPTION(( qSplits.GetQuantity() >= 8 ),ET_FORMAT_MISMATCH);
            for (int i = 0; i < 8; i++)
            {
                float fPrimary = (float)qSplits[i].ToDouble();
                afPrimaries[i] = fPrimary;
            }
        }
    }
    C_CONDITIONAL_THROW_EXCEPTION(bHasFormatInfo,ET_FORMAT_MISMATCH);

    if ( bHasExposureInfo || bHasColorCorrInfo )
    {
        for (int i = 0; i < 3; i++)
            m_afInvCompCoeffs[i] = 1.0f/(fCumulativeExposure*afCumulativeColorCorr[i]);
        m_bInvCompCoeffsAreMeaning = true;
    }

    if ( m_eFormat == FT_RLE_XYZE )
    {
        m_qXyzToRgbMatrix = ColorModelConversion::GenerateXyzToRgbMatrix(
            afPrimaries[0], afPrimaries[1],
            afPrimaries[2], afPrimaries[3],
            afPrimaries[4], afPrimaries[5],
            afPrimaries[6], afPrimaries[7]);
    }

    // image dimensions and orientation
    int iResInfoBeginPos = iNextReadPos;
    REACH_NEXT_STATEMENT;
    int iResInfoLength = iNextReadPos - 1 - iResInfoBeginPos;
    Stream qResInfoStream(m_eStreaming,m_qFilename,iResInfoBeginPos,iResInfoLength,true,bSuccess);
    String qResInfoStr;
    qResInfoStr.SetLength(iResInfoLength);
    qResInfoStream.Read(iResInfoLength,qResInfoStr);
    qResInfoStream.Finalize();
    int aiWSPos[3];
    int iWSCount = 0;
    for (int i = 0; i < iResInfoLength; i++)
    {
        if ( qResInfoStr[i] == ' ' )
        {
            C_CONDITIONAL_THROW_EXCEPTION(( iWSCount < 3 ),ET_FORMAT_MISMATCH);
            aiWSPos[iWSCount++] = i;
        }
    }
    C_CONDITIONAL_THROW_EXCEPTION(( iWSCount == 3 ),ET_FORMAT_MISMATCH);
    String aqDim[2] = {
        qResInfoStr.GetSubstringByRange(0,aiWSPos[1]),
        qResInfoStr.GetSubstringByLength(aiWSPos[1]+1)};
    for (int i0 = 0; i0 < 2; i0++)
    {
        const String& rqDim = aqDim[i0];

        C_CONDITIONAL_THROW_EXCEPTION(( rqDim.GetLength() >= 4 ),ET_FORMAT_MISMATCH);
        C_CONDITIONAL_THROW_EXCEPTION(( rqDim[0] == '-' || rqDim[0] == '+' ),ET_FORMAT_MISMATCH);
        C_CONDITIONAL_THROW_EXCEPTION(( rqDim[1] == 'X' || rqDim[1] == 'Y' ),ET_FORMAT_MISMATCH);
        C_CONDITIONAL_THROW_EXCEPTION(( rqDim[2] == ' ' ),ET_FORMAT_MISMATCH);
        for (int i1 = 3; i1 < rqDim.GetLength(); i1++)
            C_CONDITIONAL_THROW_EXCEPTION(( '0' <= rqDim[i1] && rqDim[i1] <= '9' ),ET_FORMAT_MISMATCH);
    }
    C_CONDITIONAL_THROW_EXCEPTION(( aqDim[0][1] != aqDim[1][1] ),ET_FORMAT_MISMATCH);
    int iDim0 = aqDim[0].GetSubstringByLength(3).ToInt();
    int iDim1 = aqDim[1].GetSubstringByLength(3).ToInt();
    if ( aqDim[0][1] == 'Y' )
    {
        m_iWidth = iDim1;
        m_iHeight = iDim0;
        if ( aqDim[0][0] == '-' && aqDim[1][0] == '+' )
            m_eOrientation = OT_TOP_LEFT;
        else if ( aqDim[0][0] == '-' && aqDim[1][0] == '-' )
            m_eOrientation = OT_TOP_RIGHT;
        else if ( aqDim[0][0] == '+' && aqDim[1][0] == '-' )
            m_eOrientation = OT_BOTTOM_RIGHT;
        else  // aqDim[0][0] = '+' and aqDim[1][0] = '+'
            m_eOrientation = OT_BOTTOM_LEFT;
    }
    else
    {
        m_iWidth = iDim0;
        m_iHeight = iDim1;
        if ( aqDim[0][0] == '-' && aqDim[1][0] == '+' )
            m_eOrientation = OT_RIGHT_BOTTOM;
        else if ( aqDim[0][0] == '-' && aqDim[1][0] == '-' )
            m_eOrientation = OT_LEFT_BOTTOM;
        else if ( aqDim[0][0] == '+' && aqDim[1][0] == '-' )
            m_eOrientation = OT_LEFT_TOP;
        else  // aqDim[0][0] = '+' and aqDim[1][0] = '+'
            m_eOrientation = OT_RIGHT_TOP;
    }
    C_CONDITIONAL_THROW_EXCEPTION(( m_iWidth != 0 && m_iHeight != 0 ),ET_FORMAT_MISMATCH);

    m_iImageDataPos = iNextReadPos;

    rbSuccess = true;
}
//------------------------------------------------------------------------------------------------------------------
bool FormatHdr::LoadImage (RasterImagePtr& rspqImage)
{
    bool bSuccess;

    // allocate the resulting image
    rspqImage = new RasterImage(RasterImage::IT_RGB_FLOAT,m_iWidth,m_iHeight,true);

    // for a progress bar
    ProgressProbe qProgress(this);
    qProgress.SetAdvance(1.0f/m_iHeight);

    // read image data
    int iFileSize;
    CONDITIONAL_THROW_EXCEPTION(Storage::GetFileSize(iFileSize,m_qFilename),ET_CANNOT_ACCESS_FILE);
    Stream qImageDataStream(m_eStreaming,m_qFilename,m_iImageDataPos,iFileSize-m_iImageDataPos,true,bSuccess);
    PROCESS_STREAM_SUCCESS_FOR_LOADING(bSuccess);
    unsigned char aucByteQuartet[4];
    float* pfPix;
    if ( m_iWidth < 8 || m_iWidth > 32767 )
    {
        // no RLE compression
        for (int iY = 0; iY < m_iHeight; iY++)
        {
            pfPix = (float*)rspqImage->GetRow(iY);
            for (int iX = 0; iX < m_iWidth; iX++)
            {
                RETURN_ON_FAIL(qImageDataStream.Read(4,aucByteQuartet));
                GetFloatTripletFromByteQuartet(aucByteQuartet,pfPix);
                pfPix += 3;
            }
        }
    }
    else
    {
        unsigned char* pucByte;
        unsigned char* pucEndByte;
        unsigned char aucBuff[2];
        int iCount, iNumOfSL = m_iHeight;
        StillArray<unsigned char> qScanlineBuffer(m_iWidth*4);
        while ( iNumOfSL > 0 )
        {
            pfPix = (float*)rspqImage->GetRow(m_iHeight-iNumOfSL);

            RETURN_ON_FAIL(qImageDataStream.Read(4,aucByteQuartet));
            if ( aucByteQuartet[0] != 2 || aucByteQuartet[1] != 2 || (aucByteQuartet[2] & 128) != 0 )
            {
                // no RLE compression
                CONDITIONAL_THROW_EXCEPTION(( pfPix == (float*)rspqImage->GetRow(0) ),ET_FORMAT_MISMATCH);
                qImageDataStream.OffsetBytePosition(-4);
                for (int iY = 0; iY < m_iHeight; iY++)
                {
                    pfPix = (float*)rspqImage->GetRow(iY);
                    for (int iX = 0; iX < m_iWidth; iX++)
                    {
                        RETURN_ON_FAIL(qImageDataStream.Read(4,aucByteQuartet));
                        GetFloatTripletFromByteQuartet(aucByteQuartet,pfPix);
                        pfPix += 3;
                    }
                }
                break;
            }
            CONDITIONAL_THROW_EXCEPTION(( ((aucByteQuartet[2] << 8) | aucByteQuartet[3]) == m_iWidth ),
                ET_FORMAT_MISMATCH);
            pucByte = qScanlineBuffer;
            for (int i = 0; i < 4; i++)
            {
                pucEndByte = qScanlineBuffer + (i + 1)*m_iWidth;
                while ( pucByte < pucEndByte )
                {
                    RETURN_ON_FAIL(qImageDataStream.Read(2,aucBuff));
                    if ( aucBuff[0] > 128 )
                    {
                        // a run of the same value
                        iCount = aucBuff[0] - 128;
                        CONDITIONAL_THROW_EXCEPTION(( !(iCount == 0 || iCount > pucEndByte - pucByte) ),
                            ET_FORMAT_MISMATCH);
                        while ( iCount-- > 0 )
                            *pucByte++ = aucBuff[1];
                    }
                    else
                    {
                        // a non-run
                        iCount = aucBuff[0];
                        CONDITIONAL_THROW_EXCEPTION(( !(iCount == 0 || iCount > pucEndByte - pucByte) ),
                            ET_FORMAT_MISMATCH);
                        *pucByte++ = aucBuff[1];
                        if ( --iCount > 0 )
                        {
                            RETURN_ON_FAIL(qImageDataStream.Read(iCount,pucByte));
                            pucByte += iCount;
                        }
                    }
                }
            }
            for (int i0 = 0; i0 < m_iWidth; i0++)
            {
                for (int i1 = 0; i1 < 4; i1++)
                    aucByteQuartet[i1] = qScanlineBuffer[i0+i1*m_iWidth];
                GetFloatTripletFromByteQuartet(aucByteQuartet,pfPix);

                if ( m_bDoRelyOnExposureAndColorCorrection && m_bInvCompCoeffsAreMeaning )
                {
                    // retrieve original pixel values
                    for (int i1 = 0; i1 < 3; i1++)
                        pfPix[i1] *= m_afInvCompCoeffs[i1];
                }

                if ( m_eFormat == FT_RLE_XYZE )
                {
                    // convert to RGB
                    ColorRgb qColorRgb = ColorModelConversion::GetConvertedToRgb(
                        ColorXyz(pfPix[0],pfPix[1],pfPix[2]),false,ms_fXyzToRgbGamma,m_qXyzToRgbMatrix);
                    for (int i1 = 0; i1 < 3; i1++)
                        pfPix[i1] = qColorRgb[i1];
                }

                pfPix += 3;
            }

            iNumOfSL--;

            qProgress.MakeAdvance();
        }
    }
    qImageDataStream.Finalize();

    // rely on orientation information
    if ( m_eOrientation == OT_TOP_RIGHT )
    {
        RasterImageTransformation qRIT(rspqImage);
        rspqImage = qRIT.GetFlippedHorizontally();
    }
    else if ( m_eOrientation == OT_BOTTOM_RIGHT )
    {
        RasterImageTransformation qRIT(rspqImage);
        rspqImage = qRIT.GetRotated180();
    }
    else if ( m_eOrientation == OT_BOTTOM_LEFT )
    {
        RasterImageTransformation qRIT(rspqImage);
        rspqImage = qRIT.GetFlippedVertically();
    }
    else if ( m_eOrientation == OT_LEFT_TOP )
    {
        {
            RasterImageTransformation qRIT(rspqImage);
            rspqImage = qRIT.GetRotated90Ccw();
        }
        RasterImageTransformation qRIT(rspqImage);
        rspqImage = qRIT.GetFlippedVertically();
    }
    else if ( m_eOrientation == OT_RIGHT_TOP )
    {
        RasterImageTransformation qRIT(rspqImage);
        rspqImage = qRIT.GetRotated90Ccw();
    }
    else if ( m_eOrientation == OT_RIGHT_BOTTOM )
    {
        {
            RasterImageTransformation qRIT(rspqImage);
            rspqImage = qRIT.GetRotated90Cw();
        }
        RasterImageTransformation qRIT(rspqImage);
        rspqImage = qRIT.GetFlippedVertically();
    }
    else if ( m_eOrientation == OT_LEFT_BOTTOM )
    {
        RasterImageTransformation qRIT(rspqImage);
        rspqImage = qRIT.GetRotated90Cw();
    }

    return true;
}
//------------------------------------------------------------------------------------------------------------------
void FormatHdr::GetByteQuartetFromFloatTriplet (const float afFloat[3], unsigned char aucByte[4])
{
    float fMax = Mathf::Max3(afFloat[0],afFloat[1],afFloat[2]);
    if ( fMax < 1e-32f )
    {
        for (int i = 0; i < 4; i++)
            aucByte[i] = 0;
    }
    else
    {
        int iE;
        float fCoeff = frexp(fMax,&iE)*256.0f/fMax;
        for (int i = 0; i < 3; i++)
            aucByte[i] = Mathf::RoundToInt(fCoeff*afFloat[i]);
        aucByte[3] = iE + 128;
    }
}
//------------------------------------------------------------------------------------------------------------------
void FormatHdr::GetFloatTripletFromByteQuartet (const unsigned char aucByte[4], float afFloat[3])
{
    if ( aucByte[3] != 0 )
    {
        float fCoeff = ldexp(1.0f,aucByte[3]-136);
        for (int i = 0; i < 3; i++)
            afFloat[i] = fCoeff*aucByte[i];
    }
    else
    {
        for (int i = 0; i < 3; i++)
            afFloat[i] = 0.0f;
    }
}
//------------------------------------------------------------------------------------------------------------------



