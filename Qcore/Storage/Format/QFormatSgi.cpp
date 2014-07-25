#include "QFormatSgi.h"
#include "QExptWarnBase.mcr"
#include "QFormatBase.mcr"
#include "QProgressProbe.h"
#include "QStorage.h"
using namespace Q;

// support for loading
class RleScanlineRecord
{
public:
    int Offset, ByteLength;
};

//------------------------------------------------------------------------------------------------------------------
FormatSgi::FormatSgi (const RasterImagePtr& rspqImage, int iBitsPerPixel)
    :
    m_spqImageSaveTo(rspqImage),
    m_iWidth(rspqImage->GetWidth()),
    m_iHeight(rspqImage->GetHeight()),
    m_iBitsPerPixel(iBitsPerPixel),
    m_eCompression(CT_NONE)
{
    assert( rspqImage->GetWidth() != 0 && rspqImage->GetHeight() != 0 );
    assert( rspqImage->GetWidth() <= 65535 && rspqImage->GetHeight() <= 65535 );
    assert( (rspqImage->GetColorModel() == RasterImage::CMT_GRAYSCALE
        &&  (iBitsPerPixel == 8 || iBitsPerPixel == 16))
        ||  (rspqImage->GetColorModel() == RasterImage::CMT_GRAYSCALE_WITH_ALPHA
        &&  (iBitsPerPixel == 16 || iBitsPerPixel == 32))
        ||  (rspqImage->GetColorModel() == RasterImage::CMT_RGB
        &&  (iBitsPerPixel == 24 || iBitsPerPixel == 48))
        ||  (rspqImage->GetColorModel() == RasterImage::CMT_RGBA
        &&  (iBitsPerPixel == 32 || iBitsPerPixel == 64)) );
    assert( !rspqImage->HasVisibilityMask() );

    switch ( rspqImage->GetColorModel() )
    {
    case RasterImage::CMT_GRAYSCALE:
        m_eColorModel = CMT_GRAYSCALE;
        m_iSampleQuantity = 1;
        break;
    case RasterImage::CMT_GRAYSCALE_WITH_ALPHA:
        m_eColorModel = CMT_GRAYSCALE_WITH_ALPHA;
        m_iSampleQuantity = 2;
        break;
    case RasterImage::CMT_RGB:
        m_eColorModel = CMT_RGB;
        m_iSampleQuantity = 3;
        break;
    case RasterImage::CMT_RGBA:
        m_eColorModel = CMT_RGBA;
        m_iSampleQuantity = 4;
        break;
    }
    m_iBytesPerSample = iBitsPerPixel/(m_iSampleQuantity*8);
}
//------------------------------------------------------------------------------------------------------------------
bool FormatSgi::SaveImage (const char* acFilename)
{
    char cZero = 0;

    Stream qSaveStream(acFilename,false);

    // Header.
    short sSignature = 474;
    qSaveStream.Write(sSignature);

    char cStorage;
    switch ( m_eCompression )
    {
    case CT_NONE:
        cStorage = 0;
        break;
    case CT_RLE:
        cStorage = 1;
        break;
    }
    qSaveStream.Write(cStorage);

    char cBytesPerChannel = m_iBytesPerSample;
    qSaveStream.Write(cBytesPerChannel);

    unsigned short usNumberOfDimensions;
    switch ( m_eColorModel )
    {
    case CMT_GRAYSCALE:
        usNumberOfDimensions = 2;
        break;
    case CMT_GRAYSCALE_WITH_ALPHA:
    case CMT_RGB:
    case CMT_RGBA:
        usNumberOfDimensions = 3;
        break;
    }
    qSaveStream.Write(usNumberOfDimensions);

    unsigned short usSizeX = m_iWidth;
    unsigned short usSizeY = m_iHeight;
    unsigned short usSizeZ = m_iSampleQuantity;
    qSaveStream.Write(usSizeX);
    qSaveStream.Write(usSizeY);
    qSaveStream.Write(usSizeZ);

    int iMinPixVal = 0;
    int iMaxPixVal = (1 << m_iBytesPerSample*8) - 1;
    qSaveStream.Write(iMinPixVal);
    qSaveStream.Write(iMaxPixVal);

    for (int i = 0; i < 84; i++)
        qSaveStream.Write(cZero);

    int iColormapId = 0;
    qSaveStream.Write(iColormapId);

    for (int i = 0; i < 404; i++)
        qSaveStream.Write(cZero);

    int iScanlineQuantity = m_iSampleQuantity*m_iHeight;

    if ( m_eCompression == CT_RLE )
    {
        // will overwrite SGI scanline offsets and lengths later
        int iTablesDataSize = iScanlineQuantity*8;
        for (int i = 0; i < iTablesDataSize; i++)
            qSaveStream.Write(cZero);
    }

    // for a progress bar
    ProgressProbe qProgress(this);
    qProgress.SetAdvance(1.0f/iScanlineQuantity);

    // write the image
    int iScanlineByteSize = m_iWidth*m_iBytesPerSample;
    StillArray<unsigned char> qUncompressedScanline(iScanlineByteSize);
    int iHMO = m_iHeight - 1;
    unsigned char* pucSample;
    unsigned short usValue;
    unsigned char ucSample0, ucSample1, ucPrevSample;
    unsigned short usSample0, usSample1, usPrevSample;
    bool bLastRunWasLiteral;
    int iSamplesLeft, iRunLength, iLastLiteralRunLength, iLastLiteralCounterPos, iCodedSampleQuantity;
    int iLengthsTablePos, iSLRecCount, iInTabOff, iOffset, iByteLength;
    if ( m_eCompression == CT_RLE )
    {
        iLengthsTablePos = 512 + iScanlineQuantity*4;
        iSLRecCount = 0;
    }
    float afSamples[4];
    for (int i0 = 0; i0 < m_iSampleQuantity; i0++)
    {
        for (int iY = iHMO; iY >= 0; iY--)
        {
            // assign sample values
            pucSample = qUncompressedScanline;
            for (int iX = 0; iX < m_iWidth; iX++)
            {
                if ( m_eColorModel == CMT_GRAYSCALE )
                {
                    afSamples[0] = m_spqImageSaveTo->GetPixelIntensity(iX,iY);
                }
                else if ( m_eColorModel == CMT_GRAYSCALE_WITH_ALPHA )
                {
                    Link<float,float> qIntensityWithAlpha =
                        m_spqImageSaveTo->GetPixelIntensityWithAlpha(iX,iY);
                    afSamples[0] = qIntensityWithAlpha.First;
                    afSamples[1] = qIntensityWithAlpha.Second;
                }
                else if ( m_eColorModel == CMT_RGB )
                {
                    ColorRgb qColorRgb = m_spqImageSaveTo->GetPixelRgb(iX,iY);
                    for (int i1 = 0; i1 < 3; i1++)
                        afSamples[i1] = qColorRgb[i1];
                }
                else  // m_eColorModel = CMT_RGBA
                {
                    ColorRgba qColorRgba = m_spqImageSaveTo->GetPixelRgba(iX,iY);
                    for (int i1 = 0; i1 < 4; i1++)
                        afSamples[i1] = qColorRgba[i1];
                }

                if ( m_iBytesPerSample == 1 )
                {
                    *pucSample++ = Mathf::RoundToInt(afSamples[i0]*255);
                }
                else  // m_iBytesPerSample = 2
                {
                    usValue = Mathf::RoundToInt(afSamples[i0]*65535);
                    Storage::Write2be((char*)pucSample,1,&usValue);
                    pucSample += 2;
                }
            }

            if ( m_eCompression == CT_NONE )
            {
                qSaveStream.Write(iScanlineByteSize,qUncompressedScanline);
            }
            else
            {
                // pack the scanline
                bLastRunWasLiteral = false;
                iCodedSampleQuantity = 0;
                if ( m_iBytesPerSample == 1 )
                {
                    Array<unsigned char> qCompressedScanline;
                    for (/**/; /**/; /**/)
                    {
                        iSamplesLeft = m_iWidth - iCodedSampleQuantity;
                        if ( iSamplesLeft == 0 )
                            break;
                        if ( iSamplesLeft == 1 )
                        {
                            if ( bLastRunWasLiteral && iLastLiteralRunLength < 127 )
                                qCompressedScanline[iLastLiteralCounterPos]++;
                            else
                                qCompressedScanline.Push(129);
                            qCompressedScanline.Push(qUncompressedScanline[iCodedSampleQuantity]);
                            break;
                        }

                        ucSample0 = qUncompressedScanline[iCodedSampleQuantity+0];
                        ucSample1 = qUncompressedScanline[iCodedSampleQuantity+1];
                        iRunLength = 2;
                        if ( ucSample0 != ucSample1 )
                        {
                            ucPrevSample = ucSample1;
                            for (int i1 = iCodedSampleQuantity+2; i1 < m_iWidth; i1++)
                            {
                                if ( qUncompressedScanline[i1] == ucPrevSample )
                                {
                                    iRunLength--;
                                    break;
                                }
                                if ( ++iRunLength == 127 )
                                    break;
                                ucPrevSample = qUncompressedScanline[i1];
                            }
                            if ( bLastRunWasLiteral && iLastLiteralRunLength + iRunLength <= 127 )
                            {
                                qCompressedScanline[iLastLiteralCounterPos] += iRunLength;
                                for (int i1 = 0; i1 < iRunLength; i1++)
                                    qCompressedScanline.Push(qUncompressedScanline[iCodedSampleQuantity++]);
                                iLastLiteralRunLength += iRunLength;
                            }
                            else
                            {
                                iLastLiteralCounterPos = qCompressedScanline.GetQuantity();
                                qCompressedScanline.Push(128 | iRunLength);
                                for (int i1 = 0; i1 < iRunLength; i1++)
                                    qCompressedScanline.Push(qUncompressedScanline[iCodedSampleQuantity++]);
                                iLastLiteralRunLength = iRunLength;
                                bLastRunWasLiteral = true;
                            }
                        }
                        else
                        {
                            for (int i1 = iCodedSampleQuantity+2; i1 < m_iWidth; i1++)
                            {
                                if ( qUncompressedScanline[i1] != ucSample0 )
                                    break;
                                if ( ++iRunLength == 127 )
                                    break;
                            }
                            if ( iRunLength == 2 && bLastRunWasLiteral && iLastLiteralRunLength + 2 <= 127 )
                            {
                                qCompressedScanline[iLastLiteralCounterPos] += 2;
                                for (int i1 = 0; i1 < 2; i1++)
                                    qCompressedScanline.Push(ucSample0);
                                iLastLiteralRunLength += 2;
                                iCodedSampleQuantity += 2;
                            }
                            else
                            {
                                qCompressedScanline.Push(iRunLength);
                                qCompressedScanline.Push(ucSample0);
                                iCodedSampleQuantity += iRunLength;
                                bLastRunWasLiteral = false;
                            }
                        }
                    }
                    qCompressedScanline.Push(0);

                    iOffset = qSaveStream.GetBytePosition();
                    qSaveStream.PushPositionState();
                    iInTabOff = iSLRecCount*4;
                    qSaveStream.SetBytePosition(512+iInTabOff);
                    qSaveStream.Write(iOffset);
                    iByteLength = qCompressedScanline.GetQuantity();
                    qSaveStream.SetBytePosition(iLengthsTablePos+iInTabOff);
                    qSaveStream.Write(iByteLength);
                    qSaveStream.PopPositionState();

                    qSaveStream.Write(qCompressedScanline.GetQuantity(),qCompressedScanline);
                }
                else  // m_iBytesPerSample = 2
                {
                    Array<unsigned short> qCompressedScanline;
                    for (/**/; /**/; /**/)
                    {
                        iSamplesLeft = m_iWidth - iCodedSampleQuantity;
                        if ( iSamplesLeft == 0 )
                            break;
                        if ( iSamplesLeft == 1 )
                        {
                            if ( bLastRunWasLiteral && iLastLiteralRunLength < 127 )
                                qCompressedScanline[iLastLiteralCounterPos]++;
                            else
                                qCompressedScanline.Push(129);
                            qCompressedScanline.Push(
                                ExtractUShortValue(qUncompressedScanline,iCodedSampleQuantity));
                            break;
                        }

                        usSample0 = ExtractUShortValue(qUncompressedScanline,iCodedSampleQuantity+0);
                        usSample1 = ExtractUShortValue(qUncompressedScanline,iCodedSampleQuantity+1);
                        iRunLength = 2;
                        if ( usSample0 != usSample1 )
                        {
                            usPrevSample = usSample1;
                            for (int i1 = iCodedSampleQuantity+2; i1 < m_iWidth; i1++)
                            {
                                if ( ExtractUShortValue(qUncompressedScanline,i1) == usPrevSample )
                                {
                                    iRunLength--;
                                    break;
                                }
                                if ( ++iRunLength == 127 )
                                    break;
                                usPrevSample = ExtractUShortValue(qUncompressedScanline,i1);
                            }
                            if ( bLastRunWasLiteral && iLastLiteralRunLength + iRunLength <= 127 )
                            {
                                qCompressedScanline[iLastLiteralCounterPos] += iRunLength;
                                for (int i1 = 0; i1 < iRunLength; i1++)
                                {
                                    qCompressedScanline.Push(
                                        ExtractUShortValue(qUncompressedScanline,iCodedSampleQuantity++));
                                }
                                iLastLiteralRunLength += iRunLength;
                            }
                            else
                            {
                                iLastLiteralCounterPos = qCompressedScanline.GetQuantity();
                                qCompressedScanline.Push(128 | iRunLength);
                                for (int i1 = 0; i1 < iRunLength; i1++)
                                {
                                    qCompressedScanline.Push(
                                        ExtractUShortValue(qUncompressedScanline,iCodedSampleQuantity++));
                                }
                                iLastLiteralRunLength = iRunLength;
                                bLastRunWasLiteral = true;
                            }
                        }
                        else
                        {
                            for (int i1 = iCodedSampleQuantity+2; i1 < m_iWidth; i1++)
                            {
                                if ( ExtractUShortValue(qUncompressedScanline,i1) != usSample0 )
                                    break;
                                if ( ++iRunLength == 127 )
                                    break;
                            }
                            if ( iRunLength == 2 && bLastRunWasLiteral && iLastLiteralRunLength + 2 <= 127 )
                            {
                                qCompressedScanline[iLastLiteralCounterPos] += 2;
                                for (int i1 = 0; i1 < 2; i1++)
                                    qCompressedScanline.Push(usSample0);
                                iLastLiteralRunLength += 2;
                                iCodedSampleQuantity += 2;
                            }
                            else
                            {
                                qCompressedScanline.Push(iRunLength);
                                qCompressedScanline.Push(usSample0);
                                iCodedSampleQuantity += iRunLength;
                                bLastRunWasLiteral = false;
                            }
                        }
                    }
                    qCompressedScanline.Push(0);

                    iOffset = qSaveStream.GetBytePosition();
                    qSaveStream.PushPositionState();
                    iInTabOff = iSLRecCount*4;
                    qSaveStream.SetBytePosition(512+iInTabOff);
                    qSaveStream.Write(iOffset);
                    iByteLength = qCompressedScanline.GetQuantity()*2;
                    qSaveStream.SetBytePosition(iLengthsTablePos+iInTabOff);
                    qSaveStream.Write(iByteLength);
                    qSaveStream.PopPositionState();

                    qSaveStream.Write(qCompressedScanline.GetQuantity(),qCompressedScanline);
                }

                iSLRecCount++;
            }

            qProgress.MakeAdvance();
        }
    }
    qUncompressedScanline.RemoveAll();

    // flush to the file
    CONDITIONAL_THROW_EXCEPTION(qSaveStream.Finalize(),ET_CANNOT_ACCESS_FILE);

    return true;
}
//------------------------------------------------------------------------------------------------------------------
FormatSgi::FormatSgi (const char* acFilename, bool& rbSuccess)
    :
    FormatBase(acFilename)
{
    bool bSuccess;

    // signature
    int iFileSize;
    C_CONDITIONAL_THROW_EXCEPTION(Storage::GetFileSize(iFileSize,m_qFilename),ET_CANNOT_ACCESS_FILE);
    C_CONDITIONAL_THROW_EXCEPTION_WITHOUT_ASSERT_FALSE(( iFileSize >= 2 ),ET_FORMAT_IDENTIFIER_MISMATCH);
    Stream qSignatureStream(m_eStreaming,m_qFilename,0,2,false,bSuccess);
    C_PROCESS_STREAM_SUCCESS_FOR_LOADING(bSuccess);
    short sSignature;
    qSignatureStream.Read(sSignature);
    C_CONDITIONAL_THROW_EXCEPTION_WITHOUT_ASSERT_FALSE(( sSignature == 474 ),ET_FORMAT_IDENTIFIER_MISMATCH);
    qSignatureStream.Finalize();

    // Header.
    Stream qHeaderStream(m_eStreaming,m_qFilename,2,106,false,bSuccess);
    C_PROCESS_STREAM_SUCCESS_FOR_LOADING(bSuccess);

    char cStorage;
    qHeaderStream.Read(cStorage);
    switch ( cStorage )
    {
    case 0:
        m_eCompression = CT_NONE;
        break;
    case 1:
        m_eCompression = CT_RLE;
        break;
    default:
        C_THROW_EXCEPTION(ET_FORMAT_MISMATCH);
    }

    char cBytesPerChannel;
    qHeaderStream.Read(cBytesPerChannel);
    C_CONDITIONAL_THROW_EXCEPTION(( cBytesPerChannel == 1 || cBytesPerChannel == 2 ),ET_FORMAT_MISMATCH);
    m_iBytesPerSample = cBytesPerChannel;

    qHeaderStream.OffsetBytePosition(2);

    unsigned short usSizeX, usSizeY, usSizeZ;
    qHeaderStream.Read(usSizeX);
    qHeaderStream.Read(usSizeY);
    qHeaderStream.Read(usSizeZ);
    C_CONDITIONAL_THROW_EXCEPTION(( usSizeX != 0 && usSizeY != 0 ),ET_FORMAT_MISMATCH);
    C_CONDITIONAL_THROW_EXCEPTION(( 1 <= usSizeZ && usSizeZ <= 4 ),ET_FORMAT_MISMATCH);
    m_iSampleQuantity = usSizeZ;
    m_iBitsPerPixel = m_iSampleQuantity*m_iBytesPerSample*8;
    m_iWidth = usSizeX;
    m_iHeight = usSizeY;
    switch ( usSizeZ )
    {
    case 1:
        m_eColorModel = CMT_GRAYSCALE;
        break;
    case 2:
        m_eColorModel = CMT_GRAYSCALE_WITH_ALPHA;
        break;
    case 3:
        m_eColorModel = CMT_RGB;
        break;
    case 4:
        m_eColorModel = CMT_RGBA;
        break;
    }

    qHeaderStream.OffsetBytePosition(92);

    int iColormapId;
    qHeaderStream.Read(iColormapId);
    C_CONDITIONAL_THROW_EXCEPTION(( iColormapId == 0 ),ET_FORMAT_MISMATCH);

    qHeaderStream.Finalize();

    rbSuccess = true;
}
//------------------------------------------------------------------------------------------------------------------
bool FormatSgi::LoadImage (RasterImagePtr& rspqImage)
{
    bool bSuccess;

    // allocate the output image
    if ( m_eColorModel == CMT_GRAYSCALE )
    {
        if ( m_iBytesPerSample == 1 )
            rspqImage = new RasterImage(RasterImage::IT_GRAYSCALE_8,m_iWidth,m_iHeight);
        else  // m_iBytesPerSample = 2
            rspqImage = new RasterImage(RasterImage::IT_GRAYSCALE_16,m_iWidth,m_iHeight);
    }
    else if ( m_eColorModel == CMT_GRAYSCALE_WITH_ALPHA )
    {
        if ( m_iBytesPerSample == 1 )
            rspqImage = new RasterImage(RasterImage::IT_GRAYSCALE_WITH_ALPHA_8_8,m_iWidth,m_iHeight);
        else  // m_iBytesPerSample = 2
            rspqImage = new RasterImage(RasterImage::IT_GRAYSCALE_WITH_ALPHA_16_16,m_iWidth,m_iHeight);
    }
    else if ( m_eColorModel == CMT_RGB )
    {
        if ( m_iBytesPerSample == 1 )
            rspqImage = new RasterImage(RasterImage::IT_RGB_8_8_8,m_iWidth,m_iHeight);
        else  // m_iBytesPerSample = 2
            rspqImage = new RasterImage(RasterImage::IT_RGB_16_16_16,m_iWidth,m_iHeight);
    }
    else  // m_eColorModel = CMT_RGBA
    {
        if ( m_iBytesPerSample == 1 )
            rspqImage = new RasterImage(RasterImage::IT_RGBA_8_8_8_8,m_iWidth,m_iHeight);
        else  // m_iBytesPerSample = 2
            rspqImage = new RasterImage(RasterImage::IT_RGBA_16_16_16_16,m_iWidth,m_iHeight);
    }

    int iScanlineQuantity = m_iSampleQuantity*m_iHeight;

    StillArray<RleScanlineRecord> qRleSLRecs;
    if ( m_eCompression == CT_RLE )
    {
        // read offsets and lengths for each SGI scanline
        qRleSLRecs.SetQuantity(iScanlineQuantity);
        Stream qTablesStream(m_eStreaming,m_qFilename,512,iScanlineQuantity*8,false,bSuccess);
        PROCESS_STREAM_SUCCESS_FOR_LOADING(bSuccess);
        for (int i = 0; i < iScanlineQuantity; i++)
            qTablesStream.Read(qRleSLRecs[i].Offset);
        for (int i = 0; i < iScanlineQuantity; i++)
            qTablesStream.Read(qRleSLRecs[i].ByteLength);
        qTablesStream.Finalize();
    }

    // for a progress bar
    ProgressProbe qProgress(this);
    qProgress.SetAdvance(1.0f/iScanlineQuantity);

    // read the image
    int iScanlineByteSize = m_iWidth*m_iBytesPerSample;
    StillArray<unsigned char> qUncompressedScanline(iScanlineByteSize);
    int iNextScanlineOffset, iSLRecCount;
    if ( m_eCompression == CT_NONE )
        iNextScanlineOffset = 512;
    else  // m_eCompression = CT_RLE
        iSLRecCount = 0;
    unsigned char* pucSample;
    unsigned char* pucRow;
    unsigned char* aucPix;
    unsigned short usValue;
    unsigned char ucValue;
    int iSampleOffset, iRunLength, iDecodedSampleQuantity, iHMO = m_iHeight - 1;
    for (int i0 = 0; i0 < m_iSampleQuantity; i0++)
    {
        pucRow = (unsigned char*)rspqImage->GetRow(iHMO);
        aucPix = pucRow;
        iSampleOffset = i0*m_iBytesPerSample;
        for (int iY = 0; iY < m_iHeight; iY++)
        {
            if ( m_eCompression == CT_NONE )
            {
                Stream qScanlineStream(m_eStreaming,m_qFilename,iNextScanlineOffset,iScanlineByteSize,false,
                    bSuccess);
                PROCESS_STREAM_SUCCESS_FOR_LOADING(bSuccess);
                iNextScanlineOffset += iScanlineByteSize;
                if ( m_iBytesPerSample == 1 )
                {
                    qScanlineStream.Read(m_iWidth,qUncompressedScanline);
                }
                else  // m_iBytesPerSample = 2
                {
                    pucSample = qUncompressedScanline;
                    for (int iX = 0; iX < m_iWidth; iX++)
                    {
                        qScanlineStream.Read(usValue);
                        MemCpy(pucSample,&usValue,2);

                        pucSample += 2;
                    }
                }
                qScanlineStream.Finalize();
            }
            else  // m_eCompression = CT_RLE
            {
                const RleScanlineRecord& rqRleSLRec = qRleSLRecs[iSLRecCount++];
                Stream qRleScanlineStream(m_eStreaming,m_qFilename,rqRleSLRec.Offset,rqRleSLRec.ByteLength,false,
                    bSuccess);
                PROCESS_STREAM_SUCCESS_FOR_LOADING(bSuccess);

                // unpack the scanline
                iDecodedSampleQuantity = 0;
                if ( m_iBytesPerSample == 1 )
                {
                    for (/**/; /**/; /**/)
                    {
                        RETURN_ON_FAIL(qRleScanlineStream.Read(ucValue));
                        iRunLength = ucValue & 127;
                        RETURN_ON_FAIL(( iDecodedSampleQuantity + iRunLength <= m_iWidth ));
                        if ( (ucValue & 128) != 0 )
                        {
                            // copy following data literally
                            RETURN_ON_FAIL(
                                qRleScanlineStream.Read(iRunLength,qUncompressedScanline+iDecodedSampleQuantity));
                        }
                        else
                        {
                            // repeat following data unit
                            RETURN_ON_FAIL(qRleScanlineStream.Read(ucValue));
                            pucSample = qUncompressedScanline + iDecodedSampleQuantity;
                            for (int i1 = 0; i1 < iRunLength; i1++)
                                *pucSample++ = ucValue;
                        }

                        iDecodedSampleQuantity += iRunLength;
                        if ( iDecodedSampleQuantity == m_iWidth )
                            break;
                    }
                }
                else  // m_iBytesPerSample = 2
                {
                    for (/**/; /**/; /**/)
                    {
                        RETURN_ON_FAIL(qRleScanlineStream.Read(usValue));
                        iRunLength = usValue & 127;
                        RETURN_ON_FAIL(( iDecodedSampleQuantity + iRunLength <= m_iWidth ));
                        pucSample = qUncompressedScanline + iDecodedSampleQuantity*2;
                        if ( (usValue & 128) != 0 )
                        {
                            // copy following data literally
                            for (int i1 = 0; i1 < iRunLength; i1++)
                            {
                                RETURN_ON_FAIL(qRleScanlineStream.Read(usValue));
                                MemCpy(pucSample,&usValue,2);
                                pucSample += 2;
                            }
                        }
                        else
                        {
                            // repeat following data unit
                            RETURN_ON_FAIL(qRleScanlineStream.Read(usValue));
                            for (int i1 = 0; i1 < iRunLength; i1++)
                            {
                                MemCpy(pucSample,&usValue,2);
                                pucSample += 2;
                            }
                        }

                        iDecodedSampleQuantity += iRunLength;
                        if ( iDecodedSampleQuantity == m_iWidth )
                            break;
                    }
                }

                qRleScanlineStream.Finalize();
            }

            // assign sample values
            pucSample = qUncompressedScanline;
            for (int iX = 0; iX < m_iWidth; iX++)
            {
                MemCpy(aucPix+iSampleOffset,pucSample,m_iBytesPerSample);

                pucSample += m_iBytesPerSample;
                aucPix += rspqImage->GetBytesPerPixel();
            }

            pucRow -= rspqImage->GetRowStride();
            aucPix = pucRow;

            qProgress.MakeAdvance();
        }
    }

    return true;
}
//------------------------------------------------------------------------------------------------------------------
unsigned short FormatSgi::ExtractUShortValue (const StillArray<unsigned char>& rqUncompressedScanline,
    int iSamplePos)
{
    unsigned short usValue;
    MemCpy(&usValue,rqUncompressedScanline.GetElements()+iSamplePos*2,2);
    return usValue;
}
//------------------------------------------------------------------------------------------------------------------



