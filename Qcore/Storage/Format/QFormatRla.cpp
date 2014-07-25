#include "QFormatRla.h"
#include "QExptWarnBase.mcr"
#include "QFormatBase.mcr"
#include "QFormatRla.mcr"
#include "QProgressProbe.h"
#include "QStorage.h"
using namespace Q;

//------------------------------------------------------------------------------------------------------------------
FormatRla::FormatRla (const RasterImagePtr& rspqImage, int iBitsPerPixel)
    :
    m_spqImageSaveTo(rspqImage),
    m_iWidth(rspqImage->GetWidth()),
    m_iHeight(rspqImage->GetHeight()),
    m_iBitsPerPixel(iBitsPerPixel)
{
    assert( rspqImage->GetWidth() != 0 && rspqImage->GetHeight() != 0 );
    assert( rspqImage->GetWidth() <= 32768 && rspqImage->GetHeight() <= 32768 );
    assert( rspqImage->GetColorModel() == RasterImage::CMT_RGB
        ||  rspqImage->GetColorModel() == RasterImage::CMT_RGBA );
    assert( !rspqImage->HasVisibilityMask() );
    assert( !(rspqImage->GetColorModel() == RasterImage::CMT_RGB
        &&  (!(3 <= iBitsPerPixel && iBitsPerPixel <= 96) || iBitsPerPixel % 3 != 0)) );
    assert( !(rspqImage->GetColorModel() == RasterImage::CMT_RGBA
        &&  (!(4 <= iBitsPerPixel && iBitsPerPixel <= 128) || iBitsPerPixel % 4 != 0)) );

    switch ( rspqImage->GetColorModel() )
    {
    case RasterImage::CMT_RGB:
        m_eColorModel = CMT_RGB;
        break;
    case RasterImage::CMT_RGBA:
        m_eColorModel = CMT_RGBA;
        break;
    }

    m_iBitsPerColorSample = m_iBitsPerAlphaSample = iBitsPerPixel/rspqImage->GetComponentsPerPixel();

    m_bColorSamplesAreFP = ( m_iBitsPerColorSample == 32 );
    m_bAlphaSamplesAreFP = ( m_iBitsPerAlphaSample == 32 );
}
//------------------------------------------------------------------------------------------------------------------
bool FormatRla::SaveImage (const char* acFilename)
{
    char cZero = 0;

    Stream qSaveStream(acFilename,false);

    // Header.
    short sWLeft, sWRight, sWBottom, sWTop, sAWLeft, sAWRight, sAWBottom, sAWTop;
    sWLeft = sAWLeft = 0;
    sWRight = sAWRight = m_iWidth - 1;
    sWBottom = sAWBottom = 0;
    sWTop = sAWTop = m_iHeight - 1;
    qSaveStream.Write(sWLeft);
    qSaveStream.Write(sWRight);
    qSaveStream.Write(sWBottom);
    qSaveStream.Write(sWTop);
    qSaveStream.Write(sAWLeft);
    qSaveStream.Write(sAWRight);
    qSaveStream.Write(sAWBottom);
    qSaveStream.Write(sAWTop);

    short sFrame = 1;
    qSaveStream.Write(sFrame);

    short sStorageType = 0;
    qSaveStream.Write(sStorageType);

    short sNumChan = 3;
    qSaveStream.Write(sNumChan);

    short sNumMatte = ( m_eColorModel == CMT_RGBA ? 1 : 0 );
    qSaveStream.Write(sNumMatte);

    short sNumAux = 0;
    qSaveStream.Write(sNumAux);

    short sRevision = (short)65534;
    qSaveStream.Write(sRevision);

    char acGamma[16];
    MemSet(acGamma,0,16);
    sprintf(acGamma,"%.4f",1.0f);
    qSaveStream.Write(16,acGamma);

    char acRedPri[24];
    char acGreenPri[24];
    char acBluePri[24];
    char acWhitePt[24];
    MemSet(acRedPri,0,24);
    MemSet(acGreenPri,0,24);
    MemSet(acBluePri,0,24);
    MemSet(acWhitePt,0,24);
    sprintf(acRedPri,"%.4f %.4f",0.670f,0.080f);
    sprintf(acGreenPri,"%.4f %.4f",0.210f,0.710f);
    sprintf(acBluePri,"%.4f %.4f",0.140f,0.330f);
    sprintf(acWhitePt,"%.4f %.4f",0.310f,0.316f);
    qSaveStream.Write(24,acRedPri);
    qSaveStream.Write(24,acGreenPri);
    qSaveStream.Write(24,acBluePri);
    qSaveStream.Write(24,acWhitePt);

    for (int i = 0; i < 432; i++)
        qSaveStream.Write(cZero);

    char acAspectRatio[8];
    MemSet(acAspectRatio,0,8);
    sprintf(acAspectRatio,"%.4f",1.3333f);
    qSaveStream.Write(8,acAspectRatio);

    char acChan[32];
    MemSet(acChan,0,32);
    acChan[0] = 'r';
    acChan[1] = 'g';
    acChan[2] = 'b';
    qSaveStream.Write(32,acChan);

    short sField = 0;
    qSaveStream.Write(sField);

    for (int i = 0; i < 44; i++)
        qSaveStream.Write(cZero);

    short sChanBits = m_iBitsPerColorSample;
    qSaveStream.Write(sChanBits);

    short sMatteType = 0;
    qSaveStream.Write(sMatteType);

    short sMatteBits = ( m_eColorModel == CMT_RGBA ? m_iBitsPerAlphaSample : 8 );
    qSaveStream.Write(sMatteBits);

    short sAuxType = 0;
    qSaveStream.Write(sAuxType);

    short sAuxBits = 8;
    qSaveStream.Write(sAuxBits);

    for (int i = 0; i < 68; i++)
        qSaveStream.Write(cZero);

    int iNext = 0;
    qSaveStream.Write(iNext);

    // reserve space for the scanline offset table
    int iSLOTSize = m_iHeight*4;
    for (int i = 0; i < iSLOTSize; i++)
        qSaveStream.Write(cZero);

    // allocate RLA channel string(s)
    StillArray<unsigned char> qChannelUChar;
    StillArray<unsigned short> qChannelUShort;
    StillArray<unsigned int> qChannelUInt;
    StillArray<float> qChannelFloat;
    if ( m_iBitsPerColorSample <= 8
    ||   m_iBitsPerAlphaSample <= 8 )
    {
        qChannelUChar.SetQuantity(m_iWidth);
    }
    if ( (8 < m_iBitsPerColorSample && m_iBitsPerColorSample <= 16)
    ||   (8 < m_iBitsPerAlphaSample && m_iBitsPerAlphaSample <= 16) )
    {
        qChannelUShort.SetQuantity(m_iWidth);
    }
    if ( (16 < m_iBitsPerColorSample && m_iBitsPerColorSample <= 32)
    ||   (16 < m_iBitsPerAlphaSample && m_iBitsPerAlphaSample <= 32) )
    {
        qChannelUInt.SetQuantity(m_iWidth);
    }
    if ( m_bColorSamplesAreFP
    ||   m_bAlphaSamplesAreFP )
    {
        qChannelFloat.SetQuantity(m_iWidth);
    }

    // for a progress bar
    ProgressProbe qProgress(this);
    qProgress.SetAdvance(1.0f/m_iHeight);

    // write the image
    int iSLOffset, iSLOCount = 0;
    unsigned int uiColorSampleMaxValue = (1 << m_iBitsPerColorSample) - 1;
    unsigned int uiAlphaSampleMaxValue = (1 << m_iBitsPerAlphaSample) - 1;
    StillArray<ColorRgb> qRgbRow(m_iWidth);
    StillArray<float> qAlphaRow;
    if ( m_eColorModel == CMT_RGBA )
        qAlphaRow.SetQuantity(m_iWidth);
    ColorRgba qColorRgba;
    unsigned short usChannelDataLength;
    unsigned short usValue;
    unsigned int uiValue;
    float fValue;
    for (int iY = m_iHeight-1; iY >= 0; iY--)
    {
        // write offset for the scanline
        iSLOffset = qSaveStream.GetBytePosition();
        qSaveStream.PushPositionState();
        qSaveStream.SetBytePosition(740+iSLOCount*4);
        iSLOCount++;
        qSaveStream.Write(iSLOffset);
        qSaveStream.PopPositionState();

        if ( m_eColorModel == CMT_RGB )
        {
            for (int iX = 0; iX < m_iWidth; iX++)
                qRgbRow[iX] = m_spqImageSaveTo->GetPixelRgb(iX,iY);
        }
        else  // m_eColorModel = CMT_RGBA
        {
            for (int iX = 0; iX < m_iWidth; iX++)
            {
                qColorRgba = m_spqImageSaveTo->GetPixelRgba(iX,iY);
                for (int i = 0; i < 3; i++)
                    qRgbRow[iX][i] = qColorRgba[i]*qColorRgba[3];
                qAlphaRow[iX] = qColorRgba[3];
            }
        }

        // write color channels
        for (int i = 0; i < 3; i++)
        {
            StillArray<char> qEncodedBytes;
            if ( m_iBitsPerColorSample <= 8 )
            {
                for (int iX = 0; iX < m_iWidth; iX++)
                    qChannelUChar[iX] = Mathf::RoundToInt(qRgbRow[iX][i]*uiColorSampleMaxValue);
                EncodeChannelUChar(qChannelUChar,qEncodedBytes);
            }
            else if ( m_iBitsPerColorSample <= 16 )
            {
                for (int iX = 0; iX < m_iWidth; iX++)
                {
                    usValue = Mathf::RoundToInt(qRgbRow[iX][i]*uiColorSampleMaxValue);
                    Storage::Write2be((char*)&qChannelUShort[iX],1,&usValue);
                }
                EncodeChannelUShort(qChannelUShort,qEncodedBytes);
            }
            else  // 16 < m_iBitsPerColorSample and m_iBitsPerColorSample <= 32
            {
                if ( !m_bColorSamplesAreFP )
                {
                    for (int iX = 0; iX < m_iWidth; iX++)
                    {
                        uiValue = Mathf::RoundToInt(qRgbRow[iX][i]*uiColorSampleMaxValue);
                        Storage::Write4be((char*)&qChannelUInt[iX],1,&uiValue);
                    }
                    EncodeChannelUInt(qChannelUInt,qEncodedBytes);
                }
                else
                {
                    for (int iX = 0; iX < m_iWidth; iX++)
                    {
                        fValue = qRgbRow[iX][i];
                        Storage::Write4be((char*)&qChannelFloat[iX],1,&fValue);
                    }
                    EncodeChannelFloat(qChannelFloat,qEncodedBytes);
                }
            }
            usChannelDataLength = qEncodedBytes.GetQuantity();
            qSaveStream.Write(usChannelDataLength);
            qSaveStream.Write(usChannelDataLength,qEncodedBytes);
        }

        if ( m_eColorModel == CMT_RGBA )
        {
            // write alpha channel
            StillArray<char> qEncodedBytes;
            if ( m_iBitsPerAlphaSample <= 8 )
            {
                for (int iX = 0; iX < m_iWidth; iX++)
                    qChannelUChar[iX] = Mathf::RoundToInt(qAlphaRow[iX]*uiAlphaSampleMaxValue);
                EncodeChannelUChar(qChannelUChar,qEncodedBytes);
            }
            else if ( m_iBitsPerAlphaSample <= 16 )
            {
                for (int iX = 0; iX < m_iWidth; iX++)
                {
                    usValue = Mathf::RoundToInt(qAlphaRow[iX]*uiAlphaSampleMaxValue);
                    Storage::Write2be((char*)&qChannelUShort[iX],1,&usValue);
                }
                EncodeChannelUShort(qChannelUShort,qEncodedBytes);
            }
            else  // 16 < m_iBitsPerAlphaSample and m_iBitsPerAlphaSample <= 32
            {
                if ( !m_bAlphaSamplesAreFP )
                {
                    for (int iX = 0; iX < m_iWidth; iX++)
                    {
                        uiValue = Mathf::RoundToInt(qAlphaRow[iX]*uiAlphaSampleMaxValue);
                        Storage::Write4be((char*)&qChannelUInt[iX],1,&uiValue);
                    }
                    EncodeChannelUInt(qChannelUInt,qEncodedBytes);
                }
                else
                {
                    for (int iX = 0; iX < m_iWidth; iX++)
                    {
                        fValue = qAlphaRow[iX];
                        Storage::Write4be((char*)&qChannelFloat[iX],1,&fValue);
                    }
                    EncodeChannelFloat(qChannelFloat,qEncodedBytes);
                }
            }
            usChannelDataLength = qEncodedBytes.GetQuantity();
            qSaveStream.Write(usChannelDataLength);
            qSaveStream.Write(usChannelDataLength,qEncodedBytes);
        }

        qProgress.MakeAdvance();
    }
    qChannelUChar.RemoveAll();
    qChannelUShort.RemoveAll();
    qChannelUInt.RemoveAll();
    qChannelFloat.RemoveAll();
    qRgbRow.RemoveAll();

    // flush to the file
    CONDITIONAL_THROW_EXCEPTION(qSaveStream.Finalize(),ET_CANNOT_ACCESS_FILE);

    return true;
}
//------------------------------------------------------------------------------------------------------------------
FormatRla::FormatRla (const char* acFilename, bool& rbSuccess)
    :
    FormatBase(acFilename)
{
    bool bSuccess;

    // Header.
    Stream qHeaderStream(m_eStreaming,m_qFilename,0,740,false,bSuccess);
    C_PROCESS_STREAM_SUCCESS_FOR_LOADING(bSuccess);

    qHeaderStream.OffsetBytePosition(8);

    short sLeft, sRight, sBottom, sTop;
    qHeaderStream.Read(sLeft);
    qHeaderStream.Read(sRight);
    qHeaderStream.Read(sBottom);
    qHeaderStream.Read(sTop);
    m_iWidth = sRight - sLeft + 1;
    m_iHeight = sTop - sBottom + 1;
    C_CONDITIONAL_THROW_EXCEPTION(( m_iWidth > 0 && m_iHeight > 0 ),ET_FORMAT_MISMATCH);

    qHeaderStream.OffsetBytePosition(2);

    short sStorageType;
    qHeaderStream.Read(sStorageType);
    C_CONDITIONAL_THROW_EXCEPTION(( sStorageType == 0 ),ET_UNSUPPORTED_FILE);

    short sNumChan;
    qHeaderStream.Read(sNumChan);
    C_CONDITIONAL_THROW_EXCEPTION(( sNumChan == 3 ),ET_UNSUPPORTED_FILE);

    short sNumMatte;
    qHeaderStream.Read(sNumMatte);
    C_CONDITIONAL_THROW_EXCEPTION(( sNumMatte == 0 || sNumMatte == 1 ),ET_UNSUPPORTED_FILE);

    m_eColorModel = ( sNumMatte == 0 ? CMT_RGB : CMT_RGBA );

    qHeaderStream.OffsetBytePosition(556);

    char acChan[32];
    qHeaderStream.Read(32,acChan);
    C_CONDITIONAL_THROW_EXCEPTION(( acChan[0] == 'r' && acChan[1] == 'g' && acChan[2] == 'b' ),
        ET_UNSUPPORTED_FILE);

    short sField;
    qHeaderStream.Read(sField);
    C_CONDITIONAL_THROW_EXCEPTION(( sField == 0 ),ET_UNSUPPORTED_FILE);

    qHeaderStream.OffsetBytePosition(44);

    short sChanBits;
    qHeaderStream.Read(sChanBits);
    C_CONDITIONAL_THROW_EXCEPTION(( 1 <= sChanBits && sChanBits <= 32 ),ET_FORMAT_MISMATCH);
    m_iBitsPerColorSample = sChanBits;

    m_iBitsPerPixel = 3*m_iBitsPerColorSample;

    short sMatteType;
    qHeaderStream.Read(sMatteType);
    C_CONDITIONAL_THROW_EXCEPTION(( !(m_eColorModel == CMT_RGBA && sMatteType != 0) ),ET_UNSUPPORTED_FILE);

    short sMatteBits;
    qHeaderStream.Read(sMatteBits);
    C_CONDITIONAL_THROW_EXCEPTION(( !(m_eColorModel == CMT_RGBA && !(1 <= sMatteBits && sMatteBits <= 32)) ),
        ET_FORMAT_MISMATCH);
    m_iBitsPerAlphaSample = sMatteBits;

    if ( m_eColorModel == CMT_RGBA )
        m_iBitsPerPixel += m_iBitsPerAlphaSample;

    m_bColorSamplesAreFP = ( m_iBitsPerColorSample == 32 );
    m_bAlphaSamplesAreFP = ( m_iBitsPerAlphaSample == 32 );

    qHeaderStream.Finalize();

    rbSuccess = true;
}
//------------------------------------------------------------------------------------------------------------------
bool FormatRla::LoadImage (RasterImagePtr& rspqImage)
{
    bool bSuccess;

    // allocate the output image
    if ( m_eColorModel == CMT_RGB )
    {
        if ( m_iBitsPerColorSample <= 8 )
            rspqImage = new RasterImage(RasterImage::IT_RGB_8_8_8,m_iWidth,m_iHeight);
        else if ( m_iBitsPerColorSample <= 16 )
            rspqImage = new RasterImage(RasterImage::IT_RGB_16_16_16,m_iWidth,m_iHeight);
        else  // 16 < m_iBitsPerColorSample and m_iBitsPerColorSample <= 32
            rspqImage = new RasterImage(RasterImage::IT_RGB_FLOAT,m_iWidth,m_iHeight);
    }
    else  // m_eColorModel = CMT_RGBA
    {
        if ( m_iBitsPerColorSample <= 8 )
            rspqImage = new RasterImage(RasterImage::IT_RGBA_8_8_8_8,m_iWidth,m_iHeight);
        else if ( m_iBitsPerColorSample <= 16 )
            rspqImage = new RasterImage(RasterImage::IT_RGBA_16_16_16_16,m_iWidth,m_iHeight);
        else  // 16 < m_iBitsPerColorSample and m_iBitsPerColorSample <= 32
            rspqImage = new RasterImage(RasterImage::IT_RGBA_FLOAT,m_iWidth,m_iHeight);
    }

    // allocate RLA channel string(s)
    StillArray<unsigned char> qChannelUChar;
    StillArray<unsigned short> qChannelUShort;
    StillArray<unsigned int> qChannelUInt;
    StillArray<float> qChannelFloat;
    if ( m_iBitsPerColorSample <= 8
    ||   m_iBitsPerAlphaSample <= 8 )
    {
        qChannelUChar.SetQuantity(m_iWidth);
    }
    if ( (8 < m_iBitsPerColorSample && m_iBitsPerColorSample <= 16)
    ||   (8 < m_iBitsPerAlphaSample && m_iBitsPerAlphaSample <= 16) )
    {
        qChannelUShort.SetQuantity(m_iWidth);
    }
    if ( (16 < m_iBitsPerColorSample && m_iBitsPerColorSample <= 32)
    ||   (16 < m_iBitsPerAlphaSample && m_iBitsPerAlphaSample <= 32) )
    {
        qChannelUInt.SetQuantity(m_iWidth);
    }
    if ( m_bColorSamplesAreFP
    ||   m_bAlphaSamplesAreFP )
    {
        qChannelFloat.SetQuantity(m_iWidth);
    }

    // for a progress bar
    ProgressProbe qProgress(this);
    qProgress.SetAdvance(1.0f/m_iHeight);

    // read the image
    unsigned char* pucRow;
    unsigned short* ausRow;
    float* afRow;
    unsigned short usValue;
    unsigned int uiValue;
    float fValue, fAlphaValue;
    ColorRgba qColorRgba;
    int iNextReadPos, iRevY;
    unsigned short usChannelDataLength;
    char* acRow = rspqImage->GetImage() + (m_iHeight - 1)*rspqImage->GetRowStride();
    unsigned int uiColorSampleMaxValue = (1 << m_iBitsPerColorSample) - 1;
    unsigned int uiAlphaSampleMaxValue = (1 << m_iBitsPerAlphaSample) - 1;
    for (int iY = 0; iY < m_iHeight; iY++)
    {
        // get scanline offset
        Stream qSLOffsetStream(m_eStreaming,m_qFilename,740+iY*4,4,false,bSuccess);
        PROCESS_STREAM_SUCCESS_FOR_LOADING(bSuccess);
        qSLOffsetStream.Read(iNextReadPos);
        qSLOffsetStream.Finalize();

        // read color channels
        for (int i = 0; i < 3; i++)
        {
            // get length of channel data
            Stream qCDLStream(m_eStreaming,m_qFilename,iNextReadPos,2,false,bSuccess);
            PROCESS_STREAM_SUCCESS_FOR_LOADING(bSuccess);
            iNextReadPos += 2;
            qCDLStream.Read(usChannelDataLength);
            qCDLStream.Finalize();

            // process channel data
            Stream qChannelDataStream(m_eStreaming,m_qFilename,iNextReadPos,usChannelDataLength,false,bSuccess);
            PROCESS_STREAM_SUCCESS_FOR_LOADING(bSuccess);
            iNextReadPos += usChannelDataLength;
            if ( m_iBitsPerColorSample <= 8 )
            {
                pucRow = (unsigned char*)acRow;
                RETURN_ON_FAIL(DecodeChannelUChar(qChannelDataStream,usChannelDataLength,qChannelUChar));
                for (int iX = 0; iX < m_iWidth; iX++)
                {
                    pucRow[iX*rspqImage->GetComponentsPerPixel()+i] =
                        Mathf::RoundToInt(((float)qChannelUChar[iX])/uiColorSampleMaxValue*255);
                }
            }
            else if ( m_iBitsPerColorSample <= 16 )
            {
                ausRow = (unsigned short*)acRow;
                RETURN_ON_FAIL(DecodeChannelUShort(qChannelDataStream,usChannelDataLength,qChannelUShort));
                for (int iX = 0; iX < m_iWidth; iX++)
                {
                    Storage::Read2be((const char*)&qChannelUShort[iX],1,&usValue);
                    ausRow[iX*rspqImage->GetComponentsPerPixel()+i] =
                        Mathf::RoundToInt(((float)usValue)/uiColorSampleMaxValue*65535);
                }
            }
            else  // 16 < m_iBitsPerColorSample and m_iBitsPerColorSample <= 32
            {
                if ( !m_bColorSamplesAreFP )
                {
                    afRow = (float*)acRow;
                    RETURN_ON_FAIL(DecodeChannelUInt(qChannelDataStream,usChannelDataLength,qChannelUInt));
                    for (int iX = 0; iX < m_iWidth; iX++)
                    {
                        Storage::Read4be((const char*)&qChannelUInt[iX],1,&uiValue);
                        afRow[iX*rspqImage->GetComponentsPerPixel()+i] = ((float)uiValue)/uiColorSampleMaxValue;
                    }
                }
                else
                {
                    afRow = (float*)acRow;
                    RETURN_ON_FAIL(DecodeChannelFloat(qChannelDataStream,usChannelDataLength,qChannelFloat));
                    for (int iX = 0; iX < m_iWidth; iX++)
                    {
                        Storage::Read4be((const char*)&qChannelFloat[iX],1,&fValue);
                        afRow[iX*rspqImage->GetComponentsPerPixel()+i] = fValue;
                    }
                }
            }
            qChannelDataStream.Finalize();
        }

        if ( m_eColorModel == CMT_RGBA )
        {
            // read alpha channel
            iRevY = m_iHeight - iY - 1;
            Stream qCDLStream(m_eStreaming,m_qFilename,iNextReadPos,2,false,bSuccess);
            PROCESS_STREAM_SUCCESS_FOR_LOADING(bSuccess);
            iNextReadPos += 2;
            qCDLStream.Read(usChannelDataLength);
            qCDLStream.Finalize();
            Stream qChannelDataStream(m_eStreaming,m_qFilename,iNextReadPos,usChannelDataLength,false,bSuccess);
            PROCESS_STREAM_SUCCESS_FOR_LOADING(bSuccess);
            if ( m_iBitsPerAlphaSample <= 8 )
            {
                RETURN_ON_FAIL(DecodeChannelUChar(qChannelDataStream,usChannelDataLength,qChannelUChar));
                for (int iX = 0; iX < m_iWidth; iX++)
                {
                    fAlphaValue = ((float)qChannelUChar[iX])/uiAlphaSampleMaxValue;
                    PROCESS_ALPHA_FOR_PIXEL;
                }
            }
            else if ( m_iBitsPerAlphaSample <= 16 )
            {
                RETURN_ON_FAIL(DecodeChannelUShort(qChannelDataStream,usChannelDataLength,qChannelUShort));
                for (int iX = 0; iX < m_iWidth; iX++)
                {
                    Storage::Read2be((const char*)&qChannelUShort[iX],1,&usValue);
                    fAlphaValue = ((float)usValue)/uiAlphaSampleMaxValue;
                    PROCESS_ALPHA_FOR_PIXEL;
                }
            }
            else  // 16 < m_iBitsPerAlphaSample and m_iBitsPerAlphaSample <= 32
            {
                if ( !m_bAlphaSamplesAreFP )
                {
                    RETURN_ON_FAIL(DecodeChannelUInt(qChannelDataStream,usChannelDataLength,qChannelUInt));
                    for (int iX = 0; iX < m_iWidth; iX++)
                    {
                        Storage::Read4be((const char*)&qChannelUInt[iX],1,&uiValue);
                        fAlphaValue = ((float)uiValue)/uiAlphaSampleMaxValue;
                        PROCESS_ALPHA_FOR_PIXEL;
                    }
                }
                else
                {
                    RETURN_ON_FAIL(DecodeChannelFloat(qChannelDataStream,usChannelDataLength,qChannelFloat));
                    for (int iX = 0; iX < m_iWidth; iX++)
                    {
                        Storage::Read4be((const char*)&qChannelFloat[iX],1,&fAlphaValue);
                        PROCESS_ALPHA_FOR_PIXEL;
                    }
                }
            }
            qChannelDataStream.Finalize();
        }

        acRow -= rspqImage->GetRowStride();

        qProgress.MakeAdvance();
    }

    return true;
}
//------------------------------------------------------------------------------------------------------------------
void FormatRla::EncodeByteSequence (const unsigned char* aucByte, int iByteQuantity,
    StillArray<char>& rqEncodedBytes)
{
    Array<char> qNewBytesBuffer;
    unsigned char ucByte0, ucByte1, ucPrevByte;
    bool bLastRunWasLiteral = false;
    int iBytesLeft, iRunLength, iLastLiteralRunLength, iLastLiteralCounterPos, iCodedByteQuantity = 0;
    for (/**/; /**/; /**/)
    {
        iBytesLeft = iByteQuantity - iCodedByteQuantity;
        if ( iBytesLeft == 0 )
            break;
        if ( iBytesLeft == 1 )
        {
            if ( bLastRunWasLiteral && iLastLiteralRunLength < 128 )
                qNewBytesBuffer[iLastLiteralCounterPos]--;
            else
                qNewBytesBuffer.Push(-1);
            qNewBytesBuffer.Push(aucByte[iCodedByteQuantity]);
            break;
        }

        ucByte0 = aucByte[iCodedByteQuantity+0];
        ucByte1 = aucByte[iCodedByteQuantity+1];
        iRunLength = 2;
        if ( ucByte0 != ucByte1 )
        {
            // literal run
            ucPrevByte = ucByte1;
            for (int i = iCodedByteQuantity+2; i < iByteQuantity; i++)
            {
                if ( aucByte[i] == ucPrevByte )
                {
                    iRunLength--;
                    break;
                }
                if ( ++iRunLength == 128 )
                    break;
                ucPrevByte = aucByte[i];
            }
            if ( bLastRunWasLiteral && iLastLiteralRunLength + iRunLength <= 128 )
            {
                // join two literal runs
                qNewBytesBuffer[iLastLiteralCounterPos] -= iRunLength;
                for (int i = 0; i < iRunLength; i++)
                    qNewBytesBuffer.Push(aucByte[iCodedByteQuantity++]);
                iLastLiteralRunLength += iRunLength;
            }
            else
            {
                // new literal run
                iLastLiteralCounterPos = qNewBytesBuffer.GetQuantity();
                qNewBytesBuffer.Push(-iRunLength);
                for (int i = 0; i < iRunLength; i++)
                    qNewBytesBuffer.Push(aucByte[iCodedByteQuantity++]);
                iLastLiteralRunLength = iRunLength;
                bLastRunWasLiteral = true;
            }
        }
        else
        {
            // replicate run (at first glance)
            for (int i = iCodedByteQuantity+2; i < iByteQuantity; i++)
            {
                if ( aucByte[i] != ucByte0 )
                    break;
                if ( ++iRunLength == 128 )
                    break;
            }
            if ( iRunLength == 2 )
            {
                // an RLA's replicate run must be >= 3 of length, so use literal run instead
                if ( bLastRunWasLiteral && iLastLiteralRunLength + 2 <= 128 )
                {
                    // use the previous literal run
                    qNewBytesBuffer[iLastLiteralCounterPos] -= 2;
                    for (int i = 0; i < 2; i++)
                        qNewBytesBuffer.Push(ucByte0);
                    iLastLiteralRunLength += 2;
                }
                else
                {
                    // new literal run
                    iLastLiteralCounterPos = qNewBytesBuffer.GetQuantity();
                    qNewBytesBuffer.Push(-2);
                    for (int i = 0; i < 2; i++)
                        qNewBytesBuffer.Push(ucByte0);
                    iLastLiteralRunLength = 2;
                    bLastRunWasLiteral = true;
                }
                iCodedByteQuantity += 2;
            }
            else
            {
                qNewBytesBuffer.Push(iRunLength-1);
                qNewBytesBuffer.Push(ucByte0);
                iCodedByteQuantity += iRunLength;
                bLastRunWasLiteral = false;
            }
        }
    }
    rqEncodedBytes = qNewBytesBuffer;
}
//------------------------------------------------------------------------------------------------------------------
void FormatRla::EncodeChannelUChar (const unsigned char* aucSample, StillArray<char>& rqEncodedBytes) const
{
    EncodeByteSequence(aucSample,m_iWidth,rqEncodedBytes);
}
//------------------------------------------------------------------------------------------------------------------
void FormatRla::EncodeChannelUShort (const unsigned short* ausSample, StillArray<char>& rqEncodedBytes) const
{
    rqEncodedBytes.RemoveAll();
    StillArray<unsigned char> qUnencodedBytes(m_iWidth);
    for (int i = 0; i < 2; i++)
    {
        for (int iX = 0; iX < m_iWidth; iX++)
            qUnencodedBytes[iX] = ((unsigned char*)&ausSample[iX])[i];

        StillArray<char> qEB;
        EncodeByteSequence(qUnencodedBytes,m_iWidth,qEB);
        rqEncodedBytes.Append(qEB);
    }
}
//------------------------------------------------------------------------------------------------------------------
void FormatRla::EncodeChannelUInt (const unsigned int* auiSample, StillArray<char>& rqEncodedBytes) const
{
    rqEncodedBytes.RemoveAll();
    StillArray<unsigned char> qUnencodedBytes(m_iWidth);
    for (int i = 0; i < 4; i++)
    {
        for (int iX = 0; iX < m_iWidth; iX++)
            qUnencodedBytes[iX] = ((unsigned char*)&auiSample[iX])[i];

        StillArray<char> qEB;
        EncodeByteSequence(qUnencodedBytes,m_iWidth,qEB);
        rqEncodedBytes.Append(qEB);
    }
}
//------------------------------------------------------------------------------------------------------------------
void FormatRla::EncodeChannelFloat (const float* afSample, StillArray<char>& rqEncodedBytes) const
{
    int iByteQuantity = m_iWidth*4;
    rqEncodedBytes.SetQuantity(iByteQuantity);
    MemCpy(rqEncodedBytes,afSample,iByteQuantity);
}
//------------------------------------------------------------------------------------------------------------------
bool FormatRla::DecodeByteSequence (const char* acByte, int iByteQuantity, unsigned char* aucDecodedByte,
    int iDecodedByteQuantity)
{
    int iRunLength;
    int iDecodedIn = 0;
    int iDecodedOut = 0;
    for (/**/; /**/; /**/)
    {
        if ( acByte[iDecodedIn] < 0 )
        {
            // literal run
            iRunLength = -acByte[iDecodedIn++];
            if ( iDecodedIn + iRunLength > iByteQuantity
            ||   iDecodedOut + iRunLength > iDecodedByteQuantity )
            {
                return false;
            }
            for (int i = 0; i < iRunLength; i++)
                aucDecodedByte[iDecodedOut++] = acByte[iDecodedIn++];
        }
        else
        {
            // replicate run
            iRunLength = acByte[iDecodedIn++] + 1;
            if ( iDecodedIn + 1 > iByteQuantity
            ||   iDecodedOut + iRunLength > iDecodedByteQuantity )
            {
                return false;
            }
            for (int i = 0; i < iRunLength; i++)
                aucDecodedByte[iDecodedOut++] = acByte[iDecodedIn];
            iDecodedIn++;
        }

        if ( iDecodedIn == iByteQuantity )
        {
            if ( iDecodedOut != iDecodedByteQuantity )
                return false;

            break;
        }
    }

    return true;
}
//------------------------------------------------------------------------------------------------------------------
bool FormatRla::DecodeChannelUChar (const char* acByte, int iByteQuantity, unsigned char* aucDecodedSample) const
{
    if ( !DecodeByteSequence(acByte,iByteQuantity,aucDecodedSample,m_iWidth) )
        return false;

    return true;
}
//------------------------------------------------------------------------------------------------------------------
bool FormatRla::DecodeChannelUShort (const char* acByte, int iByteQuantity, unsigned short* ausDecodedSample) const
{
    int iDecodedByteQuantity = m_iWidth*2;
    StillArray<unsigned char> qDecodedBytes(iDecodedByteQuantity);
    if ( !DecodeByteSequence(acByte,iByteQuantity,qDecodedBytes,iDecodedByteQuantity) )
        return false;
    unsigned char* aucMB;
    for (int iX = 0; iX < m_iWidth; iX++)
    {
        aucMB = (unsigned char*)&ausDecodedSample[iX];
        for (int i = 0; i < 2; i++)
            aucMB[i] = qDecodedBytes[i*m_iWidth+iX];
    }

    return true;
}
//------------------------------------------------------------------------------------------------------------------
bool FormatRla::DecodeChannelUInt (const char* acByte, int iByteQuantity, unsigned int* auiDecodedSample) const
{
    int iDecodedByteQuantity = m_iWidth*4;
    StillArray<unsigned char> qDecodedBytes(iDecodedByteQuantity);
    if ( !DecodeByteSequence(acByte,iByteQuantity,qDecodedBytes,iDecodedByteQuantity) )
        return false;
    unsigned char* aucMB;
    for (int iX = 0; iX < m_iWidth; iX++)
    {
        aucMB = (unsigned char*)&auiDecodedSample[iX];
        for (int i = 0; i < 4; i++)
            aucMB[i] = qDecodedBytes[i*m_iWidth+iX];
    }

    return true;
}
//------------------------------------------------------------------------------------------------------------------
bool FormatRla::DecodeChannelFloat (const char* acByte, int iByteQuantity, float* afDecodedSample) const
{
    if ( iByteQuantity != m_iWidth*4 )
        return false;
    MemCpy(afDecodedSample,acByte,iByteQuantity);

    return true;
}
//------------------------------------------------------------------------------------------------------------------



