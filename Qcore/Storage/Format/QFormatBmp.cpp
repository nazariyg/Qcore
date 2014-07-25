#include "QFormatBmp.h"
#include "QExptWarnBase.mcr"
#include "QFormatBase.mcr"
#include "QProgressProbe.h"
#include "QStorage.h"
using namespace Q;

//------------------------------------------------------------------------------------------------------------------
FormatBmp::FormatBmp (const RasterImagePtr& rspqImage, int iBitsPerPixel)
    :
    m_spqImageSaveTo(rspqImage),
    m_iWidth(rspqImage->GetWidth()),
    m_iHeight(rspqImage->GetHeight()),
    m_eOs(OST_WINDOWS),
    m_iNumberOfPlanes(1),
    m_eCompression(CT_NONE),
    m_iPixelsPerMeterHorizontally(2835),
    m_iPixelsPerMeterVertically(2835)
{
    assert( rspqImage->GetWidth() != 0 && rspqImage->GetHeight() != 0 );
    assert( rspqImage->GetWidth() <= 65535 && rspqImage->GetHeight() <= 65535 );
    assert( rspqImage->GetColorModel() == RasterImage::CMT_PALETTED
        ||  rspqImage->GetColorModel() == RasterImage::CMT_RGB );
    assert( !(rspqImage->GetColorModel() != RasterImage::CMT_PALETTED && iBitsPerPixel == -1) );
    assert( !rspqImage->HasVisibilityMask() );

    if ( rspqImage->GetColorModel() == RasterImage::CMT_PALETTED )
    {
        m_bPaletted = true;
        m_iPaletteSize = rspqImage->GetPaletteSize();
    }
    else
    {
        m_bPaletted = false;
    }

    if ( iBitsPerPixel == -1 )
    {
        // find out the minimal possible bits-per-pixel value
        iBitsPerPixel = Storage::GetUBitQuantity(rspqImage->GetPaletteSize()-1);
        if ( 1 < iBitsPerPixel && iBitsPerPixel < 4 )
            iBitsPerPixel = 4;
        else if ( 4 < iBitsPerPixel && iBitsPerPixel < 8 )
            iBitsPerPixel = 8;
    }

    m_iBitsPerPixel = iBitsPerPixel;
}
//------------------------------------------------------------------------------------------------------------------
bool FormatBmp::SaveImage (const char* acFilename)
{
    Stream qSaveStream(acFilename,true);
    WriteImageData(qSaveStream);

    // flush to the file
    CONDITIONAL_THROW_EXCEPTION(qSaveStream.Finalize(),ET_CANNOT_ACCESS_FILE);

    return true;
}
//------------------------------------------------------------------------------------------------------------------
void FormatBmp::SaveImage (StillArray<char>& rqStaticBytestream)
{
    rqStaticBytestream.RemoveAll();

    Stream qSaveStream(true);
    qSaveStream.SetDoNotFreeBufferAfterComplete();
    WriteImageData(qSaveStream);
    qSaveStream.Finalize();  // calls AlignBytes

    rqStaticBytestream.BecomeOwner((char*)qSaveStream.GetBytes(),qSaveStream.GetBytePosition());
}
//------------------------------------------------------------------------------------------------------------------
FormatBmp::FormatBmp (const char* acFilename, bool& rbSuccess)
    :
    FormatBase(acFilename)
{
    rbSuccess = LoadHeadingData();
}
//------------------------------------------------------------------------------------------------------------------
FormatBmp::FormatBmp (bool bBytestreamIsStatic, const char* acBytestream, int iBytestreamSize, bool& rbSuccess)
    :
    FormatBase(bBytestreamIsStatic,acBytestream,( bBytestreamIsStatic ? iBytestreamSize : -1 ))
{
    if ( !bBytestreamIsStatic )
        m_iDevelopedRowQuantity = 0;

    rbSuccess = LoadHeadingData(iBytestreamSize);
}
//------------------------------------------------------------------------------------------------------------------
bool FormatBmp::LoadImage (RasterImagePtr& rspqImage, int iDynamicBytestreamSize)
{
    assert( ( m_eStreaming == Stream::ST_READING_FROM_DYNAMIC_BYTESTREAM ) == ( iDynamicBytestreamSize >= 0 ) );

    int iBytestreamSize = ( m_eStreaming == Stream::ST_READING_FROM_STATIC_BYTESTREAM ? m_iStaticBytestreamSize :
        iDynamicBytestreamSize );

    bool bSuccess;

    // row data must be rounded to the next 4-byte boundary
    int iSizeOfRowInBits = m_iWidth*m_iBitsPerPixel;
    int iSizeOfRow = ( (iSizeOfRowInBits & 7) == 0 ? iSizeOfRowInBits/8 : iSizeOfRowInBits/8 + 1 );
    int iOffsetPerRow = ( (iSizeOfRow & 3) != 0 ? (iSizeOfRow/4 + 1)*4 - iSizeOfRow : 0 );

    // for a progress bar
    ProgressProbe qProgress(this,
        ( m_eStreaming != Stream::ST_READING_FROM_DYNAMIC_BYTESTREAM ? 0.0f :
        ((float)m_iDevelopedRowQuantity)/m_iHeight ));
    if ( m_eStreaming != Stream::ST_READING_FROM_DYNAMIC_BYTESTREAM )
        qProgress.SetAdvance(1.0f/m_iHeight);

    // load pixel data
    int iPixelDataOffset;
    Stream qPixelDataOffsetStream(m_eStreaming,m_acStreamSrcParam,10,4,true,bSuccess,iBytestreamSize);
    PROCESS_STREAM_SUCCESS_FOR_LOADING(bSuccess);
    qPixelDataOffsetStream.Read(iPixelDataOffset);
    qPixelDataOffsetStream.Finalize();

    if ( m_bPaletted )
    {
        // paletted
        if ( !(m_eStreaming == Stream::ST_READING_FROM_DYNAMIC_BYTESTREAM && m_iDevelopedRowQuantity != 0) )
        {
            Stream qPaletteStream(m_eStreaming,m_acStreamSrcParam,m_iOffsetToPalette,
                ( m_eOs == OST_WINDOWS ? 4*m_iPaletteSize : 3*m_iPaletteSize ),true,bSuccess,iBytestreamSize);
            PROCESS_STREAM_SUCCESS_FOR_LOADING(bSuccess);

            RasterImage::Palette qPalette(m_iPaletteSize);
            for (int i = 0; i < m_iPaletteSize; i++)
            {
                RETURN_ON_FAIL(qPaletteStream.Read(3,qPalette[i].Components));
                if ( m_eOs == OST_WINDOWS )
                    qPaletteStream.OffsetBytePosition(1);
            }

            qPaletteStream.Finalize();

            rspqImage = new RasterImage(RasterImage::IT_PALETTED,m_iWidth,m_iHeight,qPalette);

            // for cases involving RLE compression, fill background with the color of the first palette entry
            if ( m_eCompression == CT_RLE_4 || m_eCompression == CT_RLE_8 )
                MemSet(*rspqImage,0,m_iHeight*rspqImage->GetRowStride());
        }

        Stream qImageStream(m_eStreaming,m_acStreamSrcParam,iPixelDataOffset,
            ( m_eStreaming != Stream::ST_READING_FROM_DYNAMIC_BYTESTREAM ? m_iSizeOfImageData :
            iBytestreamSize - iPixelDataOffset ),true,bSuccess,iBytestreamSize);
        PROCESS_STREAM_SUCCESS_FOR_LOADING(bSuccess);

        char* pcTopByte = rspqImage->GetImage() + m_iHeight*rspqImage->GetRowStride() - 1;
        if ( m_eCompression == CT_NONE )
        {
            char* acRow = rspqImage->GetRow(m_iHeight-1);
            if ( m_iBitsPerPixel == 1 )
            {
                int iValue;
                for (int iY = 0; iY < m_iHeight; iY++)
                {
                    if ( m_eStreaming != Stream::ST_READING_FROM_DYNAMIC_BYTESTREAM
                    ||   iY >= m_iDevelopedRowQuantity )
                    {
                        for (int iX = 0; iX < m_iWidth; iX++)
                        {
                            RETURN_ON_FAIL(qImageStream.ReadUBits(iValue,1));
                            CONDITIONAL_THROW_EXCEPTION(( iValue < m_iPaletteSize ),ET_FORMAT_MISMATCH);
                            acRow[iX] = iValue;
                        }

                        if ( m_eStreaming != Stream::ST_READING_FROM_DYNAMIC_BYTESTREAM )
                        {
                            qProgress.MakeAdvance();
                        }
                        else
                        {
                            m_iDevelopedRowQuantity++;
                            qProgress.SetDone(((float)m_iDevelopedRowQuantity)/m_iHeight);
                        }
                    }
                    else
                    {
                        qImageStream.OffsetBitPosition(iSizeOfRowInBits);
                    }
                    qImageStream.AlignBytes();
                    qImageStream.OffsetBytePosition(iOffsetPerRow);

                    acRow -= rspqImage->GetRowStride();
                }
            }
            else if ( m_iBitsPerPixel == 4 )
            {
                int iValue;
                for (int iY = 0; iY < m_iHeight; iY++)
                {
                    if ( m_eStreaming != Stream::ST_READING_FROM_DYNAMIC_BYTESTREAM
                    ||   iY >= m_iDevelopedRowQuantity )
                    {
                        for (int iX = 0; iX < m_iWidth; iX++)
                        {
                            RETURN_ON_FAIL(qImageStream.ReadUBits(iValue,4));
                            CONDITIONAL_THROW_EXCEPTION(( iValue < m_iPaletteSize ),ET_FORMAT_MISMATCH);
                            acRow[iX] = iValue;
                        }

                        if ( m_eStreaming != Stream::ST_READING_FROM_DYNAMIC_BYTESTREAM )
                        {
                            qProgress.MakeAdvance();
                        }
                        else
                        {
                            m_iDevelopedRowQuantity++;
                            qProgress.SetDone(((float)m_iDevelopedRowQuantity)/m_iHeight);
                        }
                    }
                    else
                    {
                        qImageStream.OffsetBitPosition(iSizeOfRowInBits);
                    }
                    qImageStream.AlignBytes();
                    qImageStream.OffsetBytePosition(iOffsetPerRow);

                    acRow -= rspqImage->GetRowStride();
                }
            }
            else  // m_iBitsPerPixel = 8
            {
                unsigned char ucIndex;
                unsigned char* pucIndex = (unsigned char*)acRow;
                for (int iY = 0; iY < m_iHeight; iY++)
                {
                    if ( m_eStreaming != Stream::ST_READING_FROM_DYNAMIC_BYTESTREAM
                    ||   iY >= m_iDevelopedRowQuantity )
                    {
                        for (int iX = 0; iX < m_iWidth; iX++)
                        {
                            RETURN_ON_FAIL(qImageStream.Read(ucIndex));
                            CONDITIONAL_THROW_EXCEPTION(( ucIndex < m_iPaletteSize ),ET_FORMAT_MISMATCH);
                            *pucIndex++ = ucIndex;
                        }

                        if ( m_eStreaming != Stream::ST_READING_FROM_DYNAMIC_BYTESTREAM )
                        {
                            qProgress.MakeAdvance();
                        }
                        else
                        {
                            m_iDevelopedRowQuantity++;
                            qProgress.SetDone(((float)m_iDevelopedRowQuantity)/m_iHeight);
                        }
                    }
                    else
                    {
                        qImageStream.OffsetBytePosition(iSizeOfRow);
                    }
                    qImageStream.OffsetBytePosition(iOffsetPerRow);

                    acRow -= rspqImage->GetRowStride();
                    pucIndex = (unsigned char*)acRow;
                }
            }
        }
        else if ( m_eCompression == CT_RLE_4 )
        {
            unsigned char ucValue0, ucValue1;
            int iValue, iValue0, iValue1;
            int iRow = m_iHeight - 1;
            char* pcComp = rspqImage->GetRow(iRow);
            int iOffsetBefore;
            for (/**/; /**/; /**/)
            {
                RETURN_ON_FAIL(qImageStream.Read(ucValue0));
                if ( ucValue0 != 0 )
                {
                    // encoded mode
                    RETURN_ON_FAIL(qImageStream.ReadUBits(iValue0,4));
                    RETURN_ON_FAIL(qImageStream.ReadUBits(iValue1,4));
                    int i = 0;
                    for (/**/; /**/; /**/)
                    {
                        if ( iValue0 < m_iPaletteSize )
                        {
                            CONDITIONAL_THROW_EXCEPTION(( pcComp <= pcTopByte ),ET_FORMAT_MISMATCH);
                            *pcComp++ = iValue0;
                        }
                        if ( ++i == ucValue0 )
                            break;

                        if ( iValue1 < m_iPaletteSize )
                        {
                            CONDITIONAL_THROW_EXCEPTION(( pcComp <= pcTopByte ),ET_FORMAT_MISMATCH);
                            *pcComp++ = iValue1;
                        }
                        if ( ++i == ucValue0 )
                            break;
                    }
                }
                else
                {
                    RETURN_ON_FAIL(qImageStream.Read(ucValue1));
                    if ( ucValue1 == 0 )
                    {
                        // end-of-line
                        iRow--;
                        CONDITIONAL_THROW_EXCEPTION(( 0 <= iRow && iRow < m_iHeight ),ET_FORMAT_MISMATCH);
                        pcComp = rspqImage->GetRow(iRow);

                        if ( m_eStreaming != Stream::ST_READING_FROM_DYNAMIC_BYTESTREAM )
                        {
                            qProgress.MakeAdvance();
                        }
                        else if ( m_iHeight - iRow - 2 >= m_iDevelopedRowQuantity )
                        {
                            m_iDevelopedRowQuantity++;
                            qProgress.SetDone(((float)m_iDevelopedRowQuantity)/m_iHeight);
                        }
                    }
                    else if ( ucValue1 == 1 )
                    {
                        // end-of-bitmap
                        if ( m_eStreaming != Stream::ST_READING_FROM_DYNAMIC_BYTESTREAM )
                        {
                            qProgress.MakeAdvance();
                        }
                        else if ( m_iHeight - iRow - 1 >= m_iDevelopedRowQuantity )
                        {
                            m_iDevelopedRowQuantity++;
                            qProgress.SetDone(((float)m_iDevelopedRowQuantity)/m_iHeight);
                        }
                        break;
                    }
                    else if ( ucValue1 == 2 )
                    {
                        // delta
                        RETURN_ON_FAIL(qImageStream.Read(ucValue0));
                        RETURN_ON_FAIL(qImageStream.Read(ucValue1));
                        CONDITIONAL_THROW_EXCEPTION(( 0 <= iRow && iRow < m_iHeight ),ET_FORMAT_MISMATCH);
                        iOffsetBefore = (int)(pcComp - rspqImage->GetRow(iRow));
                        iRow -= ucValue1;
                        CONDITIONAL_THROW_EXCEPTION(( 0 <= iRow && iRow < m_iHeight ),ET_FORMAT_MISMATCH);
                        pcComp = rspqImage->GetRow(iRow);
                        pcComp += iOffsetBefore + ucValue0;
                    }
                    else
                    {
                        // absolute mode
                        CONDITIONAL_THROW_EXCEPTION(( (ucValue1 & 1) == 0 ),ET_FORMAT_MISMATCH);
                        for (int i = 0; i < ucValue1; i++)
                        {
                            RETURN_ON_FAIL(qImageStream.ReadUBits(iValue,4));
                            if ( iValue < m_iPaletteSize )
                            {
                                CONDITIONAL_THROW_EXCEPTION(( pcComp <= pcTopByte ),ET_FORMAT_MISMATCH);
                                *pcComp++ = iValue;
                            }
                        }
                        if ( (ucValue1 & 3) != 0 )
                        {
                            // must align to 2-byte boundary
                            qImageStream.OffsetBytePosition(1);
                        }
                    }
                }
            }
        }
        else if ( m_eCompression == CT_RLE_8 )
        {
            unsigned char ucValue0, ucValue1;
            int iRow = m_iHeight - 1;
            char* pcComp = rspqImage->GetRow(iRow);
            int iOffsetBefore;
            for (/**/; /**/; /**/)
            {
                RETURN_ON_FAIL(qImageStream.Read(ucValue0));
                RETURN_ON_FAIL(qImageStream.Read(ucValue1));
                if ( ucValue0 != 0 )
                {
                    // encoded mode
                    CONDITIONAL_THROW_EXCEPTION(( ucValue1 < m_iPaletteSize ),ET_FORMAT_MISMATCH);
                    CONDITIONAL_THROW_EXCEPTION(( pcComp + ucValue0 - 1 <= pcTopByte ),ET_FORMAT_MISMATCH);
                    MemSet(pcComp,ucValue1,ucValue0);
                    pcComp += ucValue0;
                }
                else
                {
                    if ( ucValue1 == 0 )
                    {
                        // end-of-line
                        iRow--;
                        CONDITIONAL_THROW_EXCEPTION(( 0 <= iRow && iRow < m_iHeight ),ET_FORMAT_MISMATCH);
                        pcComp = rspqImage->GetRow(iRow);

                        if ( m_eStreaming != Stream::ST_READING_FROM_DYNAMIC_BYTESTREAM )
                        {
                            qProgress.MakeAdvance();
                        }
                        else if ( m_iHeight - iRow - 2 >= m_iDevelopedRowQuantity )
                        {
                            m_iDevelopedRowQuantity++;
                            qProgress.SetDone(((float)m_iDevelopedRowQuantity)/m_iHeight);
                        }
                    }
                    else if ( ucValue1 == 1 )
                    {
                        // end-of-bitmap
                        if ( m_eStreaming != Stream::ST_READING_FROM_DYNAMIC_BYTESTREAM )
                        {
                            qProgress.MakeAdvance();
                        }
                        else if ( m_iHeight - iRow - 1 >= m_iDevelopedRowQuantity )
                        {
                            m_iDevelopedRowQuantity++;
                            qProgress.SetDone(((float)m_iDevelopedRowQuantity)/m_iHeight);
                        }
                        break;
                    }
                    else if ( ucValue1 == 2 )
                    {
                        // delta
                        RETURN_ON_FAIL(qImageStream.Read(ucValue0));
                        RETURN_ON_FAIL(qImageStream.Read(ucValue1));
                        CONDITIONAL_THROW_EXCEPTION(( 0 <= iRow && iRow < m_iHeight ),ET_FORMAT_MISMATCH);
                        iOffsetBefore = (int)(pcComp - rspqImage->GetRow(iRow));
                        iRow -= ucValue1;
                        CONDITIONAL_THROW_EXCEPTION(( 0 <= iRow && iRow < m_iHeight ),ET_FORMAT_MISMATCH);
                        pcComp = rspqImage->GetRow(iRow);
                        pcComp += iOffsetBefore + ucValue0;
                    }
                    else
                    {
                        // absolute mode
                        for (int i = 0; i < ucValue1; i++)
                        {
                            RETURN_ON_FAIL(qImageStream.Read(ucValue0));
                            CONDITIONAL_THROW_EXCEPTION(( ucValue0 < m_iPaletteSize ),ET_FORMAT_MISMATCH);
                            CONDITIONAL_THROW_EXCEPTION(( pcComp <= pcTopByte ),ET_FORMAT_MISMATCH);
                            *pcComp++ = ucValue0;
                        }
                        if ( (ucValue1 & 1) != 0 )
                        {
                            // must align to 2-byte boundary (make number of bytes even)
                            qImageStream.OffsetBytePosition(1);
                        }
                    }
                }
            }
        }

        qImageStream.Finalize();
    }
    else
    {
        Stream qImageStream(m_eStreaming,m_acStreamSrcParam,iPixelDataOffset,
            ( m_eStreaming != Stream::ST_READING_FROM_DYNAMIC_BYTESTREAM ? m_iSizeOfImageData :
            iBytestreamSize - iPixelDataOffset ),true,bSuccess,iBytestreamSize);
        PROCESS_STREAM_SUCCESS_FOR_LOADING(bSuccess);

        if ( !(m_eStreaming == Stream::ST_READING_FROM_DYNAMIC_BYTESTREAM && m_iDevelopedRowQuantity != 0) )
        {
            if ( m_iBitsPerPixel != 32 )
                rspqImage = new RasterImage(RasterImage::IT_RGB_8_8_8,m_iWidth,m_iHeight);
            else
                rspqImage = new RasterImage(RasterImage::IT_RGB_16_16_16,m_iWidth,m_iHeight);
        }

        // color masks related
        int iROff = 0;
        int iRTopV = 0;
        int iGOff = 0;
        int iGTopV = 0;
        int iBOff = 0;
        int iBTopV = 0;
        if ( m_eCompression == CT_BITFIELDS )
        {
            unsigned int uiRMask = m_uiRedMask;
            unsigned int uiGMask = m_uiGreenMask;
            unsigned int uiBMask = m_uiBlueMask;

            // find out offsets and top values
            while ( (uiRMask & 1) == 0 && iROff < 32 )
            {
                uiRMask >>= 1;
                iROff++;
            }
            while ( (uiRMask & 1) == 1 && iRTopV < 32 )
            {
                uiRMask >>= 1;
                iRTopV++;
            }
            iRTopV = (1 << iRTopV) - 1;
            while ( (uiGMask & 1) == 0 && iGOff < 32 )
            {
                uiGMask >>= 1;
                iGOff++;
            }
            while ( (uiGMask & 1) == 1 && iGTopV < 32 )
            {
                uiGMask >>= 1;
                iGTopV++;
            }
            iGTopV = (1 << iGTopV) - 1;
            while ( (uiBMask & 1) == 0 && iBOff < 32 )
            {
                uiBMask >>= 1;
                iBOff++;
            }
            while ( (uiBMask & 1) == 1 && iBTopV < 32 )
            {
                uiBMask >>= 1;
                iBTopV++;
            }
            iBTopV = (1 << iBTopV) - 1;
        }

        char* acRow = rspqImage->GetRow(m_iHeight-1);
        char* pcComp = acRow;
        if ( m_iBitsPerPixel == 16 )
        {
            unsigned short usValue;
            if ( m_eCompression == CT_NONE )
            {
                // 5,5,5 scheme
                for (int iY = 0; iY < m_iHeight; iY++)
                {
                    if ( m_eStreaming != Stream::ST_READING_FROM_DYNAMIC_BYTESTREAM
                    ||   iY >= m_iDevelopedRowQuantity )
                    {
                        for (int iX = 0; iX < m_iWidth; iX++)
                        {
                            RETURN_ON_FAIL(qImageStream.Read(usValue));

                            *pcComp++ = Mathf::RoundToInt(((float)((usValue >> 10) & 31))/31*255);
                            *pcComp++ = Mathf::RoundToInt(((float)((usValue >>  5) & 31))/31*255);
                            *pcComp++ = Mathf::RoundToInt(((float)((usValue >>  0) & 31))/31*255);
                        }

                        if ( m_eStreaming != Stream::ST_READING_FROM_DYNAMIC_BYTESTREAM )
                        {
                            qProgress.MakeAdvance();
                        }
                        else
                        {
                            m_iDevelopedRowQuantity++;
                            qProgress.SetDone(((float)m_iDevelopedRowQuantity)/m_iHeight);
                        }
                    }
                    else
                    {
                        qImageStream.OffsetBytePosition(iSizeOfRow);
                    }
                    qImageStream.OffsetBytePosition(iOffsetPerRow);

                    acRow -= rspqImage->GetRowStride();
                    pcComp = acRow;
                }
            }
            else  // m_eCompression = CT_BITFIELDS
            {
                // color masks
                for (int iY = 0; iY < m_iHeight; iY++)
                {
                    if ( m_eStreaming != Stream::ST_READING_FROM_DYNAMIC_BYTESTREAM
                    ||   iY >= m_iDevelopedRowQuantity )
                    {
                        for (int iX = 0; iX < m_iWidth; iX++)
                        {
                            RETURN_ON_FAIL(qImageStream.Read(usValue));

                            *pcComp++ = Mathf::RoundToInt(((float)((usValue >> iROff) & iRTopV))/iRTopV*255);
                            *pcComp++ = Mathf::RoundToInt(((float)((usValue >> iGOff) & iGTopV))/iGTopV*255);
                            *pcComp++ = Mathf::RoundToInt(((float)((usValue >> iBOff) & iBTopV))/iBTopV*255);
                        }

                        if ( m_eStreaming != Stream::ST_READING_FROM_DYNAMIC_BYTESTREAM )
                        {
                            qProgress.MakeAdvance();
                        }
                        else
                        {
                            m_iDevelopedRowQuantity++;
                            qProgress.SetDone(((float)m_iDevelopedRowQuantity)/m_iHeight);
                        }
                    }
                    else
                    {
                        qImageStream.OffsetBytePosition(iSizeOfRow);
                    }
                    qImageStream.OffsetBytePosition(iOffsetPerRow);

                    acRow -= rspqImage->GetRowStride();
                    pcComp = acRow;
                }
            }
        }
        else if ( m_iBitsPerPixel == 24 )
        {
            char cRValue, cGValue, cBValue;
            for (int iY = 0; iY < m_iHeight; iY++)
            {
                if ( m_eStreaming != Stream::ST_READING_FROM_DYNAMIC_BYTESTREAM || iY >= m_iDevelopedRowQuantity )
                {
                    for (int iX = 0; iX < m_iWidth; iX++)
                    {
                        RETURN_ON_FAIL(qImageStream.Read(cBValue));
                        RETURN_ON_FAIL(qImageStream.Read(cGValue));
                        RETURN_ON_FAIL(qImageStream.Read(cRValue));

                        *pcComp++ = cRValue;
                        *pcComp++ = cGValue;
                        *pcComp++ = cBValue;
                    }

                    if ( m_eStreaming != Stream::ST_READING_FROM_DYNAMIC_BYTESTREAM )
                    {
                        qProgress.MakeAdvance();
                    }
                    else
                    {
                        m_iDevelopedRowQuantity++;
                        qProgress.SetDone(((float)m_iDevelopedRowQuantity)/m_iHeight);
                    }
                }
                else
                {
                    qImageStream.OffsetBytePosition(iSizeOfRow);
                }
                qImageStream.OffsetBytePosition(iOffsetPerRow);

                acRow -= rspqImage->GetRowStride();
                pcComp = acRow;
            }
        }
        else  // m_iBitsPerPixel = 32
        {
            unsigned short usRValue, usGValue, usBValue;
            if ( m_eCompression == CT_NONE )
            {
                unsigned char ucRValue, ucGValue, ucBValue;
                for (int iY = 0; iY < m_iHeight; iY++)
                {
                    if ( m_eStreaming != Stream::ST_READING_FROM_DYNAMIC_BYTESTREAM
                    ||   iY >= m_iDevelopedRowQuantity )
                    {
                        for (int iX = 0; iX < m_iWidth; iX++)
                        {
                            RETURN_ON_FAIL(qImageStream.Read(ucBValue));
                            RETURN_ON_FAIL(qImageStream.Read(ucGValue));
                            RETURN_ON_FAIL(qImageStream.Read(ucRValue));

                            usRValue = Mathf::RoundToInt(((float)ucRValue)/255*65535);
                            usGValue = Mathf::RoundToInt(((float)ucGValue)/255*65535);
                            usBValue = Mathf::RoundToInt(((float)ucBValue)/255*65535);
                            MemCpy(pcComp,&usRValue,2);
                            pcComp += 2;
                            MemCpy(pcComp,&usGValue,2);
                            pcComp += 2;
                            MemCpy(pcComp,&usBValue,2);
                            pcComp += 2;
                            qImageStream.OffsetBytePosition(1);
                        }

                        if ( m_eStreaming != Stream::ST_READING_FROM_DYNAMIC_BYTESTREAM )
                        {
                            qProgress.MakeAdvance();
                        }
                        else
                        {
                            m_iDevelopedRowQuantity++;
                            qProgress.SetDone(((float)m_iDevelopedRowQuantity)/m_iHeight);
                        }
                    }
                    else
                    {
                        qImageStream.OffsetBytePosition(iSizeOfRow);
                    }
                    acRow -= rspqImage->GetRowStride();
                    pcComp = acRow;
                }
            }
            else  // m_eCompression = CT_BITFIELDS
            {
                // color masks
                unsigned int uiValue;
                for (int iY = 0; iY < m_iHeight; iY++)
                {
                    if ( m_eStreaming != Stream::ST_READING_FROM_DYNAMIC_BYTESTREAM
                    ||   iY >= m_iDevelopedRowQuantity )
                    {
                        for (int iX = 0; iX < m_iWidth; iX++)
                        {
                            RETURN_ON_FAIL(qImageStream.Read(uiValue));

                            usRValue = Mathf::RoundToInt(((float)((uiValue >> iROff) & iRTopV))/iRTopV*65535);
                            usGValue = Mathf::RoundToInt(((float)((uiValue >> iGOff) & iGTopV))/iGTopV*65535);
                            usBValue = Mathf::RoundToInt(((float)((uiValue >> iBOff) & iBTopV))/iBTopV*65535);
                            MemCpy(pcComp,&usRValue,2);
                            pcComp += 2;
                            MemCpy(pcComp,&usGValue,2);
                            pcComp += 2;
                            MemCpy(pcComp,&usBValue,2);
                            pcComp += 2;
                        }

                        if ( m_eStreaming != Stream::ST_READING_FROM_DYNAMIC_BYTESTREAM )
                        {
                            qProgress.MakeAdvance();
                        }
                        else
                        {
                            m_iDevelopedRowQuantity++;
                            qProgress.SetDone(((float)m_iDevelopedRowQuantity)/m_iHeight);
                        }
                    }
                    else
                    {
                        qImageStream.OffsetBytePosition(iSizeOfRow);
                    }
                    acRow -= rspqImage->GetRowStride();
                    pcComp = acRow;
                }
            }
        }

        qImageStream.Finalize();
    }

    return true;
}
//------------------------------------------------------------------------------------------------------------------
void FormatBmp::WriteImageData (Stream& rqStream)
{
    assert( m_iBitsPerPixel == 1
        ||  m_iBitsPerPixel == 4
        ||  m_iBitsPerPixel == 8
        ||  m_iBitsPerPixel == 16
        ||  m_iBitsPerPixel == 24
        ||  m_iBitsPerPixel == 32 );
    assert( m_bPaletted == ( m_iBitsPerPixel == 1 || m_iBitsPerPixel == 4 || m_iBitsPerPixel == 8 ) );
    assert( !(m_eOs == OST_OS2 && !(m_bPaletted || m_iBitsPerPixel == 24))
        &&  !(m_eOs == OST_OS2 && m_eCompression != CT_NONE) );
    assert( !(m_eCompression == CT_RLE_4 && m_iBitsPerPixel != 4)
        &&  !(m_eCompression == CT_RLE_8 && m_iBitsPerPixel != 8) );
    assert( !(m_eCompression == CT_BITFIELDS && m_iBitsPerPixel != 16 && m_iBitsPerPixel != 32)
        &&  !(m_eCompression == CT_BITFIELDS && (m_uiRedMask & m_uiGreenMask & m_uiBlueMask) != 0) );
    assert( !(m_iBitsPerPixel == 1 && m_iPaletteSize > 2) );
    assert( !(m_iBitsPerPixel == 4 && m_iPaletteSize > 16) );

    // row data must be rounded to the next 4-byte boundary
    int iSizeOfRowInBits = m_iWidth*m_iBitsPerPixel;
    int iSizeOfRow = ( (iSizeOfRowInBits & 7) == 0 ? iSizeOfRowInBits/8 : iSizeOfRowInBits/8 + 1 );
    int iOffsetPerRow = ( (iSizeOfRow & 3) != 0 ? (iSizeOfRow/4 + 1)*4 - iSizeOfRow : 0 );

    // File header.
    unsigned short usSignature = 19778;
    rqStream.Write(usSignature);

    // hole #1
    rqStream.OffsetBytePosition(4);

    rqStream.OffsetBytePosition(4);

    // hole #2
    rqStream.OffsetBytePosition(4);

    // Bitmap info header.
    int iHeaderSize = ( m_eOs == OST_WINDOWS ? 40 : 12 );
    rqStream.Write(iHeaderSize);

    if ( m_eOs == OST_WINDOWS )
    {
        rqStream.Write(m_iWidth);
        rqStream.Write(m_iHeight);
    }
    else  // m_eOs = OST_OS2
    {
        unsigned short usWidth = m_iWidth;
        unsigned short usHeight = m_iHeight;
        rqStream.Write(usWidth);
        rqStream.Write(usHeight);
    }

    unsigned short usNumberOfPlanes = m_iNumberOfPlanes;
    rqStream.Write(usNumberOfPlanes);

    unsigned short usBitsPerPixel = m_iBitsPerPixel;
    rqStream.Write(usBitsPerPixel);

    if ( m_eOs == OST_WINDOWS )
    {
        int iCompression;
        switch ( m_eCompression )
        {
        case CT_NONE:
            iCompression = 0;
            break;
        case CT_RLE_8:
            iCompression = 1;
            break;
        case CT_RLE_4:
            iCompression = 2;
            break;
        case CT_BITFIELDS:
            iCompression = 3;
            break;
        }
        rqStream.Write(iCompression);

        // hole #3
        rqStream.OffsetBytePosition(4);

        rqStream.Write(m_iPixelsPerMeterHorizontally);
        rqStream.Write(m_iPixelsPerMeterVertically);

        rqStream.Write(m_iPaletteSize);

        rqStream.Write(m_iPaletteSize);
    }

    // palette/color masks
    if ( m_bPaletted )
    {
        char cReserved = 0;
        for (int i = 0; i < m_iPaletteSize; i++)
        {
            rqStream.Write(3,m_spqImageSaveTo->GetPaletteColor(i).Components);
            if ( m_eOs == OST_WINDOWS )
                rqStream.Write(cReserved);
        }
    }
    else if ( m_eCompression == CT_BITFIELDS )
    {
        rqStream.Write(m_uiRedMask);
        rqStream.Write(m_uiGreenMask);
        rqStream.Write(m_uiBlueMask);
    }

    // fix hole #2
    rqStream.PushPositionState();
    int iPixelDataOffset = rqStream.GetBytePosition();
    rqStream.SetBytePosition(10);
    rqStream.Write(iPixelDataOffset);
    rqStream.PopPositionState();

    // for a progress bar
    ProgressProbe qProgress(this);
    qProgress.SetAdvance(1.0f/m_iHeight);

    // actual image
    if ( m_bPaletted )
    {
        unsigned char ucIndex;
        if ( m_eCompression == CT_NONE )
        {
            if ( m_iBitsPerPixel == 1 )
            {
                for (int iY = m_iHeight-1; iY >= 0; iY--)
                {
                    for (int iX = 0; iX < m_iWidth; iX++)
                    {
                        ucIndex = m_spqImageSaveTo->GetPixelIndex(iX,iY);
                        rqStream.WriteBits(ucIndex,1);
                    }
                    rqStream.AlignBytes();
                    rqStream.OffsetBytePosition(iOffsetPerRow);

                    qProgress.MakeAdvance();
                }
            }
            else if ( m_iBitsPerPixel == 4 )
            {
                for (int iY = m_iHeight-1; iY >= 0; iY--)
                {
                    for (int iX = 0; iX < m_iWidth; iX++)
                    {
                        ucIndex = m_spqImageSaveTo->GetPixelIndex(iX,iY);
                        rqStream.WriteBits(ucIndex,4);
                    }
                    rqStream.AlignBytes();
                    rqStream.OffsetBytePosition(iOffsetPerRow);

                    qProgress.MakeAdvance();
                }
            }
            else  // m_iBitsPerPixel = 8
            {
                for (int iY = m_iHeight-1; iY >= 0; iY--)
                {
                    for (int iX = 0; iX < m_iWidth; iX++)
                    {
                        ucIndex = m_spqImageSaveTo->GetPixelIndex(iX,iY);
                        rqStream.Write(ucIndex);
                    }
                    rqStream.OffsetBytePosition(iOffsetPerRow);

                    qProgress.MakeAdvance();
                }
            }
        }
        else if ( m_eCompression == CT_RLE_4 )
        {
            unsigned char ucIndex0, ucIndex1, ucEMQ, ucAMQ, ucSum;
            unsigned char ucZero = 0;
            unsigned char ucOne = 1;
            unsigned char ucTwo = 2;
            unsigned char uc254 = 254;
            int iXS, iAMQ, iContEMQ, iRotEMQ, iMaxEMQ, iLastAMBytePos, iIndex0, iIndex1, iDecrIndex, iSum, iTop;
            bool bDecr;
            for (int iY = m_iHeight-1; iY >= 0; iY--)
            {
                // make absolute mode ready
                iAMQ = 0;
                rqStream.Write(ucZero);
                iLastAMBytePos = rqStream.GetBytePosition();
                rqStream.Write(ucZero);

                iXS = 0;
                while ( iXS < m_iWidth )
                {
                    iContEMQ = 1;
                    ucIndex = m_spqImageSaveTo->GetPixelIndex(iXS,iY);
                    for (int iX = iXS+1; iX < m_iWidth; iX++)
                    {
                        if ( m_spqImageSaveTo->GetPixelIndex(iX,iY) != ucIndex )
                            break;
                        if ( ++iContEMQ == 255 )
                            break;
                    }
                    iRotEMQ = 0;
                    for (int iX = iXS; iX < m_iWidth; iX++)
                    {
                        if ( iRotEMQ >= 2 )
                        {
                            if ( m_spqImageSaveTo->GetPixelIndex(iX-0,iY) !=
                                 m_spqImageSaveTo->GetPixelIndex(iX-2,iY) )
                            {
                                break;
                            }
                        }
                        if ( ++iRotEMQ == 255 )
                            break;
                    }
                    ucIndex0 = ucIndex;
                    if ( iContEMQ >= iRotEMQ )
                    {
                        iMaxEMQ = iContEMQ;
                        ucIndex1 = ucIndex;
                    }
                    else
                    {
                        iMaxEMQ = iRotEMQ;
                        ucIndex1 = m_spqImageSaveTo->GetPixelIndex(iXS+1,iY);
                    }
                    if ( iMaxEMQ > 8 )
                    {
                        // that's where encoded mode begins to be economic
                        bDecr = ( iAMQ >= 3 && (iAMQ & 1) != 0 );
                        if ( bDecr )
                        {
                            iAMQ--;
                            ucAMQ = iAMQ;
                            rqStream.SetBytePosition(iLastAMBytePos);
                            rqStream.Write(ucAMQ);
                            rqStream.OffsetBytePosition(ucAMQ/2);
                            rqStream.ReadUBits(iDecrIndex,4);
                            rqStream.SetBitRatio(0);
                        }
                        if ( iAMQ < 3 )
                        {
                            if ( iAMQ == 0 )
                            {
                                rqStream.SetBytePosition(iLastAMBytePos-1);
                            }
                            else if ( iAMQ == 1 )
                            {
                                rqStream.SetBytePosition(iLastAMBytePos+1);
                                rqStream.ReadUBits(iIndex0,4);
                                rqStream.SetBytePosition(iLastAMBytePos-1);
                                rqStream.Write(ucOne);
                                rqStream.WriteBits(iIndex0,4);
                                rqStream.WriteBits(ucZero,4);
                            }
                            else  // iAMQ = 2
                            {
                                rqStream.SetBytePosition(iLastAMBytePos+1);
                                rqStream.ReadUBits(iIndex0,4);
                                rqStream.ReadUBits(iIndex1,4);
                                rqStream.SetBytePosition(iLastAMBytePos-1);
                                rqStream.Write(ucTwo);
                                rqStream.WriteBits(iIndex0,4);
                                rqStream.WriteBits(iIndex1,4);
                            }
                        }
                        else if ( (iAMQ & 3) != 0 )
                        {
                            rqStream.Write(ucZero);
                        }
                        if ( bDecr )
                        {
                            rqStream.Write(ucOne);
                            rqStream.WriteBits(iDecrIndex,4);
                            rqStream.WriteBits(ucZero,4);
                        }

                        ucEMQ = iMaxEMQ;
                        rqStream.Write(ucEMQ);
                        rqStream.WriteBits(ucIndex0,4);
                        rqStream.WriteBits(ucIndex1,4);

                        iXS += iMaxEMQ;

                        // make absolute mode ready
                        iAMQ = 0;
                        rqStream.Write(ucZero);
                        iLastAMBytePos = rqStream.GetBytePosition();
                        rqStream.Write(ucZero);
                    }
                    else
                    {
                        // absolute mode
                        iSum = iAMQ + iMaxEMQ;
                        if ( iSum <= 254 )
                        {
                            ucSum = iSum;
                            rqStream.PushPositionState();
                            rqStream.SetBytePosition(iLastAMBytePos);
                            rqStream.Write(ucSum);
                            rqStream.PopPositionState();
                            for (int i = 0; i < iMaxEMQ; i++)
                            {
                                ucIndex = m_spqImageSaveTo->GetPixelIndex(iXS+i,iY);
                                rqStream.WriteBits(ucIndex,4);
                            }
                            iAMQ = iSum;

                            iXS += iMaxEMQ;
                        }
                        else
                        {
                            iTop = 254 - iAMQ;
                            rqStream.PushPositionState();
                            rqStream.SetBytePosition(iLastAMBytePos);
                            rqStream.Write(uc254);
                            rqStream.PopPositionState();
                            for (int i = 0; i < iTop; i++)
                            {
                                ucIndex = m_spqImageSaveTo->GetPixelIndex(iXS+i,iY);
                                rqStream.WriteBits(ucIndex,4);
                            }

                            iXS += iTop;
                        }
                        if ( iSum >= 254 )
                        {
                            rqStream.Write(ucZero);

                            // make absolute mode ready
                            iAMQ = 0;
                            rqStream.Write(ucZero);
                            iLastAMBytePos = rqStream.GetBytePosition();
                            rqStream.Write(ucZero);
                        }
                    }
                }

                bDecr = ( iAMQ >= 3 && (iAMQ & 1) != 0 );
                if ( bDecr )
                {
                    iAMQ--;
                    ucAMQ = iAMQ;
                    rqStream.SetBytePosition(iLastAMBytePos);
                    rqStream.Write(ucAMQ);
                    rqStream.OffsetBytePosition(ucAMQ/2);
                    rqStream.ReadUBits(iDecrIndex,4);
                    rqStream.SetBitRatio(0);
                }
                if ( iAMQ < 3 )
                {
                    if ( iAMQ == 0 )
                    {
                        rqStream.SetBytePosition(iLastAMBytePos-1);
                    }
                    else if ( iAMQ == 1 )
                    {
                        rqStream.SetBytePosition(iLastAMBytePos+1);
                        rqStream.ReadUBits(iIndex0,4);
                        rqStream.SetBytePosition(iLastAMBytePos-1);
                        rqStream.Write(ucOne);
                        rqStream.WriteBits(iIndex0,4);
                        rqStream.WriteBits(ucZero,4);
                    }
                    else  // iAMQ = 2
                    {
                        rqStream.SetBytePosition(iLastAMBytePos+1);
                        rqStream.ReadUBits(iIndex0,4);
                        rqStream.ReadUBits(iIndex1,4);
                        rqStream.SetBytePosition(iLastAMBytePos-1);
                        rqStream.Write(ucTwo);
                        rqStream.WriteBits(iIndex0,4);
                        rqStream.WriteBits(iIndex1,4);
                    }
                }
                else if ( (iAMQ & 3) != 0 )
                {
                    rqStream.Write(ucZero);
                }
                if ( bDecr )
                {
                    rqStream.Write(ucOne);
                    rqStream.WriteBits(iDecrIndex,4);
                    rqStream.WriteBits(ucZero,4);
                }

                if ( iY != 0 )
                {
                    // end-of-line
                    rqStream.Write(ucZero);
                    rqStream.Write(ucZero);
                }

                qProgress.MakeAdvance();
            }

            // end-of-bitmap
            rqStream.Write(ucZero);
            rqStream.Write(ucOne);
        }
        else  // m_eCompression = CT_RLE_8
        {
            unsigned char ucQ, ucSum, ucRealAMQ, ucIndex0, ucIndex1;
            unsigned char ucZero = 0;
            unsigned char ucOne = 1;
            unsigned char uc255 = 255;
            int iXS, iEMQ, iAMQ, iLastAMBytePos, iSum;
            bool bLastWasAM;
            for (int iY = m_iHeight-1; iY >= 0; iY--)
            {
                iXS = 0;
                bLastWasAM = false;
                iAMQ = 0;
                while ( iXS < m_iWidth )
                {
                    iEMQ = 1;
                    ucIndex = m_spqImageSaveTo->GetPixelIndex(iXS,iY);
                    for (int iX = iXS+1; iX < m_iWidth; iX++)
                    {
                        if ( m_spqImageSaveTo->GetPixelIndex(iX,iY) != ucIndex )
                            break;
                        if ( ++iEMQ == 255 )
                            break;
                    }
                    ucQ = iEMQ;
                    iSum = iAMQ + iEMQ;
                    if ( ucQ > 3 )
                    {
                        // that's where encoded mode begins to be economic
                        if ( bLastWasAM )
                        {
                            rqStream.PushPositionState();
                            rqStream.SetBytePosition(iLastAMBytePos);
                            rqStream.Read(ucRealAMQ);
                            rqStream.PopPositionState();
                            if ( ucRealAMQ >= 3 )
                            {
                                if ( (ucRealAMQ & 1) != 0 )
                                {
                                    // make 2-byte alignment
                                    rqStream.Write(ucZero);
                                }
                            }
                            else
                            {
                                rqStream.SetBytePosition(iLastAMBytePos+1);
                                rqStream.Read(ucIndex0);
                                if ( ucRealAMQ == 2 )
                                    rqStream.Read(ucIndex1);

                                rqStream.SetBytePosition(iLastAMBytePos-1);
                                rqStream.Write(ucOne);
                                rqStream.Write(ucIndex0);
                                if ( ucRealAMQ == 2 )
                                {
                                    rqStream.Write(ucOne);
                                    rqStream.Write(ucIndex1);
                                }
                            }
                        }

                        rqStream.Write(ucQ);
                        ucIndex = m_spqImageSaveTo->GetPixelIndex(iXS,iY);
                        rqStream.Write(ucIndex);

                        iAMQ = 0;
                        bLastWasAM = false;
                    }
                    else
                    {
                        // absolute mode
                        if ( iAMQ == 0 )
                        {
                            if ( bLastWasAM )
                            {
                                rqStream.PushPositionState();
                                rqStream.SetBytePosition(iLastAMBytePos);
                                rqStream.Read(ucRealAMQ);
                                rqStream.PopPositionState();
                                if ( ucRealAMQ >= 3 )
                                {
                                    if ( (ucRealAMQ & 1) != 0 )
                                    {
                                        // make 2-byte alignment
                                        rqStream.Write(ucZero);
                                    }
                                }
                                else
                                {
                                    rqStream.SetBytePosition(iLastAMBytePos+1);
                                    rqStream.Read(ucIndex0);
                                    if ( ucRealAMQ == 2 )
                                        rqStream.Read(ucIndex1);

                                    rqStream.SetBytePosition(iLastAMBytePos-1);
                                    rqStream.Write(ucOne);
                                    rqStream.Write(ucIndex0);
                                    if ( ucRealAMQ == 2 )
                                    {
                                        rqStream.Write(ucOne);
                                        rqStream.Write(ucIndex1);
                                    }
                                }
                            }

                            rqStream.Write(ucZero);
                            iLastAMBytePos = rqStream.GetBytePosition();
                            rqStream.Write(ucZero);
                        }

                        if ( iSum <= 255 )
                        {
                            ucSum = iSum;
                            rqStream.PushPositionState();
                            rqStream.SetBytePosition(iLastAMBytePos);
                            rqStream.Write(ucSum);
                            rqStream.PopPositionState();
                            iAMQ = ( ucSum != 255 ? ucSum : 0 );
                        }
                        else
                        {
                            ucQ = 255 - iAMQ;
                            rqStream.PushPositionState();
                            rqStream.SetBytePosition(iLastAMBytePos);
                            rqStream.Write(uc255);
                            rqStream.PopPositionState();
                            iAMQ = 0;
                        }
                        for (int i = 0; i < ucQ; i++)
                        {
                            ucIndex = m_spqImageSaveTo->GetPixelIndex(iXS+i,iY);
                            rqStream.Write(ucIndex);
                        }

                        bLastWasAM = true;
                    }
                    iXS += ucQ;
                }

                if ( bLastWasAM )
                {
                    rqStream.PushPositionState();
                    rqStream.SetBytePosition(iLastAMBytePos);
                    rqStream.Read(ucRealAMQ);
                    rqStream.PopPositionState();
                    if ( ucRealAMQ >= 3 )
                    {
                        if ( (ucRealAMQ & 1) != 0 )
                        {
                            // make 2-byte alignment
                            rqStream.Write(ucZero);
                        }
                    }
                    else
                    {
                        rqStream.SetBytePosition(iLastAMBytePos+1);
                        rqStream.Read(ucIndex0);
                        if ( ucRealAMQ == 2 )
                            rqStream.Read(ucIndex1);

                        rqStream.SetBytePosition(iLastAMBytePos-1);
                        rqStream.Write(ucOne);
                        rqStream.Write(ucIndex0);
                        if ( ucRealAMQ == 2 )
                        {
                            rqStream.Write(ucOne);
                            rqStream.Write(ucIndex1);
                        }
                    }
                }

                if ( iY != 0 )
                {
                    // end-of-line
                    rqStream.Write(ucZero);
                    rqStream.Write(ucZero);
                }

                qProgress.MakeAdvance();
            }

            // end-of-bitmap
            rqStream.Write(ucZero);
            rqStream.Write(ucOne);
        }
    }
    else
    {
        // color masks related
        int iROff = 0;
        int iRTopV = 0;
        int iGOff = 0;
        int iGTopV = 0;
        int iBOff = 0;
        int iBTopV = 0;
        if ( m_eCompression == CT_BITFIELDS )
        {
            unsigned int uiRMask = m_uiRedMask;
            unsigned int uiGMask = m_uiGreenMask;
            unsigned int uiBMask = m_uiBlueMask;

            // find out offsets and top values
            while ( (uiRMask & 1) == 0 && iROff < 32 )
            {
                uiRMask >>= 1;
                iROff++;
            }
            while ( (uiRMask & 1) == 1 && iRTopV < 32 )
            {
                uiRMask >>= 1;
                iRTopV++;
            }
            iRTopV = (1 << iRTopV) - 1;
            while ( (uiGMask & 1) == 0 && iGOff < 32 )
            {
                uiGMask >>= 1;
                iGOff++;
            }
            while ( (uiGMask & 1) == 1 && iGTopV < 32 )
            {
                uiGMask >>= 1;
                iGTopV++;
            }
            iGTopV = (1 << iGTopV) - 1;
            while ( (uiBMask & 1) == 0 && iBOff < 32 )
            {
                uiBMask >>= 1;
                iBOff++;
            }
            while ( (uiBMask & 1) == 1 && iBTopV < 32 )
            {
                uiBMask >>= 1;
                iBTopV++;
            }
            iBTopV = (1 << iBTopV) - 1;
        }

        ColorRgb qColor;
        if ( m_iBitsPerPixel == 16 )
        {
            unsigned short usValue;
            if ( m_eCompression == CT_NONE )
            {
                // 5,5,5 scheme
                for (int iY = m_iHeight-1; iY >= 0; iY--)
                {
                    for (int iX = 0; iX < m_iWidth; iX++)
                    {
                        qColor = m_spqImageSaveTo->GetPixelRgb(iX,iY);
                        usValue = 0;
                        usValue |= Mathf::RoundToInt(qColor.R()*31) << 10;
                        usValue |= Mathf::RoundToInt(qColor.G()*31) << 5;
                        usValue |= Mathf::RoundToInt(qColor.B()*31);
                        rqStream.Write(usValue);
                    }
                    rqStream.OffsetBytePosition(iOffsetPerRow);

                    qProgress.MakeAdvance();
                }
            }
            else  // m_eCompression = CT_BITFIELDS
            {
                // color masks
                for (int iY = m_iHeight-1; iY >= 0; iY--)
                {
                    for (int iX = 0; iX < m_iWidth; iX++)
                    {
                        qColor = m_spqImageSaveTo->GetPixelRgb(iX,iY);
                        usValue = 0;
                        usValue |= Mathf::RoundToInt(qColor.R()*iRTopV) << iROff;
                        usValue |= Mathf::RoundToInt(qColor.G()*iGTopV) << iGOff;
                        usValue |= Mathf::RoundToInt(qColor.B()*iBTopV) << iBOff;
                        rqStream.Write(usValue);
                    }
                    rqStream.OffsetBytePosition(iOffsetPerRow);

                    qProgress.MakeAdvance();
                }
            }
        }
        else if ( m_iBitsPerPixel == 24 )
        {
            unsigned char ucR, ucG, ucB;
            for (int iY = m_iHeight-1; iY >= 0; iY--)
            {
                for (int iX = 0; iX < m_iWidth; iX++)
                {
                    qColor = m_spqImageSaveTo->GetPixelRgb(iX,iY);
                    ucR = Mathf::RoundToInt(qColor.R()*255);
                    ucG = Mathf::RoundToInt(qColor.G()*255);
                    ucB = Mathf::RoundToInt(qColor.B()*255);
                    rqStream.Write(ucB);
                    rqStream.Write(ucG);
                    rqStream.Write(ucR);
                }
                rqStream.OffsetBytePosition(iOffsetPerRow);

                qProgress.MakeAdvance();
            }
        }
        else  // m_iBitsPerPixel = 32
        {
            if ( m_eCompression == CT_NONE )
            {
                unsigned char ucR, ucG, ucB, ucZero = 0;
                for (int iY = m_iHeight-1; iY >= 0; iY--)
                {
                    for (int iX = 0; iX < m_iWidth; iX++)
                    {
                        qColor = m_spqImageSaveTo->GetPixelRgb(iX,iY);
                        ucR = Mathf::RoundToInt(qColor.R()*255);
                        ucG = Mathf::RoundToInt(qColor.G()*255);
                        ucB = Mathf::RoundToInt(qColor.B()*255);
                        rqStream.Write(ucB);
                        rqStream.Write(ucG);
                        rqStream.Write(ucR);
                        rqStream.Write(ucZero);
                    }

                    qProgress.MakeAdvance();
                }
            }
            else  // m_eCompression = CT_BITFIELDS
            {
                // color masks
                unsigned int uiValue;
                for (int iY = m_iHeight-1; iY >= 0; iY--)
                {
                    for (int iX = 0; iX < m_iWidth; iX++)
                    {
                        qColor = m_spqImageSaveTo->GetPixelRgb(iX,iY);
                        uiValue = 0;
                        uiValue |= Mathf::RoundToInt(qColor.R()*iRTopV) << iROff;
                        uiValue |= Mathf::RoundToInt(qColor.G()*iGTopV) << iGOff;
                        uiValue |= Mathf::RoundToInt(qColor.B()*iBTopV) << iBOff;
                        rqStream.Write(uiValue);
                    }

                    qProgress.MakeAdvance();
                }
            }
        }
    }

    int iDataSize = rqStream.GetBytePosition();
    int iSizeOfImageData = iDataSize - iPixelDataOffset;

    // pixel data must be rounded to the next 4-byte boundary
    if ( (iSizeOfImageData & 3) != 0 )
    {
        int iOffset = (iSizeOfImageData/4 + 1)*4 - iSizeOfImageData;

        // recalculate
        iDataSize += iOffset;
        iSizeOfImageData = iDataSize - iPixelDataOffset;

        rqStream.OffsetBytePosition(iOffset);
    }

    rqStream.PushPositionState();

    // fix hole #1
    rqStream.SetBytePosition(2);
    rqStream.Write(iDataSize);

    if ( m_eOs == OST_WINDOWS )
    {
        // fix hole #3
        rqStream.SetBytePosition(34);
        rqStream.Write(iSizeOfImageData);
    }

    rqStream.PopPositionState();
}
//------------------------------------------------------------------------------------------------------------------
bool FormatBmp::LoadHeadingData (int iBytestreamSize)
{
    assert( iBytestreamSize == -1 || iBytestreamSize >= 0 );

    bool bSuccess;

    // signature
    int iDataSize;
    if ( m_eStreaming == Stream::ST_READING_FROM_FILE )
        CONDITIONAL_THROW_EXCEPTION(Storage::GetFileSize(iDataSize,m_qFilename),ET_CANNOT_ACCESS_FILE)
    else
        iDataSize = iBytestreamSize;
    if ( iDataSize < 2 )
    {
        if ( m_eStreaming != Stream::ST_READING_FROM_DYNAMIC_BYTESTREAM )
            THROW_EXCEPTION_WITHOUT_ASSERT_FALSE(ET_FORMAT_IDENTIFIER_MISMATCH)
        else
            return false;
    }
    Stream qSignatureStream(m_eStreaming,m_acStreamSrcParam,0,2,true,bSuccess,iBytestreamSize);
    PROCESS_STREAM_SUCCESS_FOR_LOADING(bSuccess);
    unsigned short usSignature;
    qSignatureStream.Read(usSignature);
    qSignatureStream.Finalize();
    CONDITIONAL_THROW_EXCEPTION_WITHOUT_ASSERT_FALSE(( usSignature == 19778 ),ET_FORMAT_IDENTIFIER_MISMATCH);

    // size of info header
    int iHeaderSize;
    Stream qHeaderSizeStream(m_eStreaming,m_acStreamSrcParam,14,4,true,bSuccess,iBytestreamSize);
    PROCESS_STREAM_SUCCESS_FOR_LOADING(bSuccess);
    qHeaderSizeStream.Read(iHeaderSize);
    qHeaderSizeStream.Finalize();

    // OS
    CONDITIONAL_THROW_EXCEPTION(( iHeaderSize == 40 || iHeaderSize == 56 || iHeaderSize == 12 ),
        ET_FORMAT_MISMATCH);
    m_eOs = ( iHeaderSize == 40 || iHeaderSize == 56 ? OST_WINDOWS : OST_OS2 );
    m_iOffsetToPalette = 14 + ( iHeaderSize != 56 ? iHeaderSize : 40 );
    if ( iHeaderSize == 56 )
        iHeaderSize = 40;

    // Bitmap info header.
    Stream qBmpInfoHeaderStream(m_eStreaming,m_acStreamSrcParam,18,iHeaderSize,true,bSuccess,iBytestreamSize);
    PROCESS_STREAM_SUCCESS_FOR_LOADING(bSuccess);

    if ( m_eOs == OST_WINDOWS )
    {
        qBmpInfoHeaderStream.Read(m_iWidth);
        qBmpInfoHeaderStream.Read(m_iHeight);
    }
    else  // m_eOs = OST_OS2
    {
        unsigned short usWidth, usHeight;
        qBmpInfoHeaderStream.Read(usWidth);
        qBmpInfoHeaderStream.Read(usHeight);
        m_iWidth = usWidth;
        m_iHeight = usHeight;
    }

    CONDITIONAL_THROW_EXCEPTION_WITHOUT_ASSERT_FALSE(( m_iWidth != 0 && m_iHeight != 0 ),ET_FORMAT_MISMATCH);

    unsigned short usNumberOfPlanes;
    qBmpInfoHeaderStream.Read(usNumberOfPlanes);
    m_iNumberOfPlanes = usNumberOfPlanes;

    unsigned short usBitsPerPixel;
    qBmpInfoHeaderStream.Read(usBitsPerPixel);
    m_iBitsPerPixel = usBitsPerPixel;
    CONDITIONAL_THROW_EXCEPTION((
         m_iBitsPerPixel == 1
    ||   m_iBitsPerPixel == 4
    ||   m_iBitsPerPixel == 8
    ||   m_iBitsPerPixel == 16
    ||   m_iBitsPerPixel == 24
    ||   m_iBitsPerPixel == 32 ),ET_FORMAT_MISMATCH);

    if ( m_eOs == OST_WINDOWS )
    {
        int iCompression;
        qBmpInfoHeaderStream.Read(iCompression);
        switch ( iCompression )
        {
        case 0:
            m_eCompression = CT_NONE;
            break;
        case 1:
            m_eCompression = CT_RLE_8;
            break;
        case 2:
            m_eCompression = CT_RLE_4;
            break;
        case 3:
            m_eCompression = CT_BITFIELDS;
            break;
        default:
            THROW_EXCEPTION(ET_FORMAT_MISMATCH);
        }

        qBmpInfoHeaderStream.Read(m_iSizeOfImageData);

        qBmpInfoHeaderStream.Read(m_iPixelsPerMeterHorizontally);
        qBmpInfoHeaderStream.Read(m_iPixelsPerMeterVertically);

        m_bPaletted = ( m_iBitsPerPixel <= 8 );

        unsigned short usPaletteSize;
        qBmpInfoHeaderStream.Read(usPaletteSize);
        if ( usPaletteSize == 0 && m_bPaletted )
        {
            // means that it uses all available colors
            m_iPaletteSize = 1 << m_iBitsPerPixel;
        }
        else
        {
            m_iPaletteSize = usPaletteSize;
        }
        CONDITIONAL_THROW_EXCEPTION(( !(m_bPaletted && !(1 <= m_iPaletteSize && m_iPaletteSize <= 256)) ),
            ET_FORMAT_MISMATCH);

        unsigned short usImportantColorQuantity;
        qBmpInfoHeaderStream.Read(usImportantColorQuantity);
        m_iImportantColorQuantity = usImportantColorQuantity;
    }
    else  // m_eOs = OST_OS2
    {
        m_eCompression = CT_NONE;

        m_iPixelsPerMeterHorizontally = m_iPixelsPerMeterVertically = 0;
        if ( m_iBitsPerPixel <= 8 )
        {
            m_bPaletted = true;
            m_iPaletteSize = 1 << m_iBitsPerPixel;
        }
        else
        {
            m_bPaletted = false;
        }
        m_iImportantColorQuantity = m_iPaletteSize;
    }

    if ( (m_eOs == OST_WINDOWS && m_iSizeOfImageData == 0) || m_eOs == OST_OS2 )
    {
        // row data must be rounded to the next 4-byte boundary
        int iSizeOfRowInBits = m_iWidth*m_iBitsPerPixel;
        int iSizeOfRow = ( (iSizeOfRowInBits & 7) == 0 ? iSizeOfRowInBits/8 : iSizeOfRowInBits/8 + 1 );
        int iOffsetPerRow = ( (iSizeOfRow & 3) != 0 ? (iSizeOfRow/4 + 1)*4 - iSizeOfRow : 0 );

        m_iSizeOfImageData = (iSizeOfRow + iOffsetPerRow)*m_iHeight;
    }

    qBmpInfoHeaderStream.Finalize();

    if ( m_eCompression == CT_BITFIELDS )
    {
        // read masks
        Stream qMaskStream(m_eStreaming,m_acStreamSrcParam,m_iOffsetToPalette,12,true,bSuccess,iBytestreamSize);
        PROCESS_STREAM_SUCCESS_FOR_LOADING(bSuccess);

        qMaskStream.Read(m_uiRedMask);
        qMaskStream.Read(m_uiGreenMask);
        qMaskStream.Read(m_uiBlueMask);

        qMaskStream.Finalize();
    }

    CONDITIONAL_THROW_EXCEPTION(( !(m_eOs == OST_OS2 && !(m_bPaletted || m_iBitsPerPixel == 24)) ),
        ET_FORMAT_MISMATCH);

    CONDITIONAL_THROW_EXCEPTION((
         !(m_eCompression == CT_RLE_4 && m_iBitsPerPixel != 4)
    &&   !(m_eCompression == CT_RLE_8 && m_iBitsPerPixel != 8) ),ET_FORMAT_MISMATCH);

    CONDITIONAL_THROW_EXCEPTION(( !(m_eCompression == CT_BITFIELDS
    &&   (m_bPaletted || (m_uiRedMask & m_uiGreenMask & m_uiBlueMask) != 0)) ),ET_FORMAT_MISMATCH);

    return true;
}
//------------------------------------------------------------------------------------------------------------------



