#include "QFormatRas.h"
#include "QExptWarnBase.mcr"
#include "QFormatBase.mcr"
#include "QProgressProbe.h"
#include "QStorage.h"
using namespace Q;

//------------------------------------------------------------------------------------------------------------------
FormatRas::FormatRas (const RasterImagePtr& rspqImage, int iBitsPerPixel)
    :
    m_spqImageSaveTo(rspqImage),
    m_iWidth(rspqImage->GetWidth()),
    m_iHeight(rspqImage->GetHeight()),
    m_eCompression(CT_NONE)
{
    assert( rspqImage->GetWidth() != 0 && rspqImage->GetHeight() != 0 );
    assert( (rspqImage->GetColorModel() == RasterImage::CMT_PALETTED
        &&  (iBitsPerPixel == -1 || iBitsPerPixel == 1 || iBitsPerPixel == 8))
        ||  (rspqImage->GetColorModel() == RasterImage::CMT_GRAYSCALE
        &&  (iBitsPerPixel == 1 || iBitsPerPixel == 8))
        ||  (rspqImage->GetColorModel() == RasterImage::CMT_RGB && iBitsPerPixel == 24)
        ||  (rspqImage->GetColorModel() == RasterImage::CMT_RGBA && iBitsPerPixel == 32) );
    assert( !(rspqImage->GetColorModel() == RasterImage::CMT_PALETTED && iBitsPerPixel != -1
        &&  1 << iBitsPerPixel < rspqImage->GetPaletteSize()) );
    assert( !rspqImage->HasVisibilityMask() );

    // internal color model
    switch ( rspqImage->GetColorModel() )
    {
    case RasterImage::CMT_PALETTED:
        m_eColorModel = CMT_PALETTED;
        m_iPaletteSize = rspqImage->GetPaletteSize();
        break;
    case RasterImage::CMT_GRAYSCALE:
        m_eColorModel = CMT_GRAYSCALE;
        break;
    case RasterImage::CMT_RGB:
        m_eColorModel = CMT_RGB;
        break;
    case RasterImage::CMT_RGBA:
        m_eColorModel = CMT_RGBA;
        break;
    }

    if ( iBitsPerPixel == -1 )
    {
        // find out the minimal possible bits-per-pixel value
        iBitsPerPixel = Storage::GetUBitQuantity(m_iPaletteSize-1);
        if ( 1 < iBitsPerPixel && iBitsPerPixel < 8 )
            iBitsPerPixel = 8;
    }

    m_iBitsPerPixel = iBitsPerPixel;
}
//------------------------------------------------------------------------------------------------------------------
bool FormatRas::SaveImage (const char* acFilename)
{
    char cZero = 0;

    Stream qSaveStream(acFilename,false);

    // Header.
    int iSignature = 1504078485;
    qSaveStream.Write(iSignature);

    qSaveStream.Write(m_iWidth);
    qSaveStream.Write(m_iHeight);

    qSaveStream.Write(m_iBitsPerPixel);

    // hole #1
    for (int i = 0; i < 4; i++)
        qSaveStream.Write(cZero);

    int iType = ( m_eCompression == CT_NONE ? 1 : 2 );
    qSaveStream.Write(iType);

    int iMapType, iMapLength;
    if ( m_eColorModel == CMT_PALETTED
    ||   m_eColorModel == CMT_GRAYSCALE )
    {
        iMapType = 1;
        if ( m_eColorModel == CMT_PALETTED )
            iMapLength = m_iPaletteSize*3;
        else  // m_eColorModel = CMT_GRAYSCALE
            iMapLength = (1 << m_iBitsPerPixel)*3;
    }
    else  // m_eColorModel = CMT_RGB or m_eColorModel = CMT_RGBA
    {
        iMapType = 0;
        iMapLength = 0;
    }
    qSaveStream.Write(iMapType);
    qSaveStream.Write(iMapLength);

    // palette
    if ( m_eColorModel == CMT_PALETTED )
    {
        for (int i0 = 2; i0 >= 0; i0--)
        {
            for (int i1 = 0; i1 < m_iPaletteSize; i1++)
                qSaveStream.Write(m_spqImageSaveTo->GetPaletteColor(i1).Components[i0]);
        }
    }
    else if ( m_eColorModel == CMT_GRAYSCALE )
    {
        if ( m_iBitsPerPixel == 1 )
        {
            unsigned char ucComp0 = 0;
            unsigned char ucComp1 = 255;
            for (int i = 0; i < 3; i++)
            {
                qSaveStream.Write(ucComp0);
                qSaveStream.Write(ucComp1);
            }
        }
        else  // m_iBitsPerPixel = 8
        {
            unsigned char ucComp;
            for (int i0 = 0; i0 < 3; i0++)
            {
                for (int i1 = 0; i1 < 256; i1++)
                {
                    ucComp = i1;
                    qSaveStream.Write(ucComp);
                }
            }
        }
    }

    // calculate image size
    int iRowSizeInBits = m_iWidth*m_iBitsPerPixel;
    int iRowSize = ( (iRowSizeInBits & 7) != 0 ? iRowSizeInBits/8 + 1 : iRowSizeInBits/8 );
    bool bPadding = false;
    if ( (iRowSize & 1) != 0 )
    {
        iRowSize++;
        bPadding = true;
    }
    int iImageSize = m_iHeight*iRowSize;

    // for a progress bar
    ProgressProbe qProgress(this);
    qProgress.SetAdvance(1.0f/m_iHeight);

    // make up an uncompressed image
    StillArray<unsigned char> qUncompressedImage(iImageSize);
    if ( m_eColorModel == CMT_PALETTED )
    {
        int iAtRowStartBytePos, iBytePos, iBitRatio, iIndex;
        for (int iY = 0; iY < m_iHeight; iY++)
        {
            iAtRowStartBytePos = iY*iRowSize;
            iBytePos = iAtRowStartBytePos;
            iBitRatio = 0;
            for (int iX = 0; iX < m_iWidth; iX++)
            {
                iIndex = m_spqImageSaveTo->GetPixelIndex(iX,iY);
                iBytePos += Storage::WriteBits((char*)(qUncompressedImage+iBytePos),iBitRatio,m_iBitsPerPixel,
                    iIndex);
            }

            // fill remaining bits in the row with 0s
            while ( iBytePos - iAtRowStartBytePos != iRowSize )
                iBytePos += Storage::WriteBits((char*)(qUncompressedImage+iBytePos),iBitRatio,1,0);

            qProgress.MakeAdvance();
        }
    }
    else if ( m_eColorModel == CMT_GRAYSCALE )
    {
        int iAtRowStartBytePos, iBytePos, iBitRatio, iValue, iMaxValue = (1 << m_iBitsPerPixel) - 1;
        float fIntensity;
        for (int iY = 0; iY < m_iHeight; iY++)
        {
            iAtRowStartBytePos = iY*iRowSize;
            iBytePos = iAtRowStartBytePos;
            iBitRatio = 0;
            for (int iX = 0; iX < m_iWidth; iX++)
            {
                fIntensity = m_spqImageSaveTo->GetPixelIntensity(iX,iY);
                iValue = Mathf::RoundToInt(fIntensity*iMaxValue);
                iBytePos += Storage::WriteBits((char*)(qUncompressedImage+iBytePos),iBitRatio,m_iBitsPerPixel,
                    iValue);
            }

            // fill remaining bits in the row with 0s
            while ( iBytePos - iAtRowStartBytePos != iRowSize )
                iBytePos += Storage::WriteBits((char*)(qUncompressedImage+iBytePos),iBitRatio,1,0);

            qProgress.MakeAdvance();
        }
    }
    else if ( m_eColorModel == CMT_RGB )
    {
        int iBytePos = 0;
        unsigned char ucR, ucG, ucB;
        ColorRgb qColorRgb;
        for (int iY = 0; iY < m_iHeight; iY++)
        {
            for (int iX = 0; iX < m_iWidth; iX++)
            {
                qColorRgb = m_spqImageSaveTo->GetPixelRgb(iX,iY);
                ucR = Mathf::RoundToInt(qColorRgb.R()*255);
                ucG = Mathf::RoundToInt(qColorRgb.G()*255);
                ucB = Mathf::RoundToInt(qColorRgb.B()*255);

                qUncompressedImage[iBytePos++] = ucB;
                qUncompressedImage[iBytePos++] = ucG;
                qUncompressedImage[iBytePos++] = ucR;
            }

            if ( bPadding )
                qUncompressedImage[iBytePos++] = cZero;

            qProgress.MakeAdvance();
        }
    }
    else  // m_eColorModel = CMT_RGBA
    {
        // no padding needed
        int iBytePos = 0;
        unsigned char ucR, ucG, ucB, ucA;
        ColorRgba qColorRgba;
        for (int iY = 0; iY < m_iHeight; iY++)
        {
            for (int iX = 0; iX < m_iWidth; iX++)
            {
                qColorRgba = m_spqImageSaveTo->GetPixelRgba(iX,iY);
                ucR = Mathf::RoundToInt(qColorRgba.R()*255);
                ucG = Mathf::RoundToInt(qColorRgba.G()*255);
                ucB = Mathf::RoundToInt(qColorRgba.B()*255);
                ucA = Mathf::RoundToInt(qColorRgba.A()*255);

                qUncompressedImage[iBytePos++] = ucA;
                qUncompressedImage[iBytePos++] = ucB;
                qUncompressedImage[iBytePos++] = ucG;
                qUncompressedImage[iBytePos++] = ucR;
            }

            qProgress.MakeAdvance();
        }
    }

    int iBeforeImageDataPos = qSaveStream.GetBytePosition();

    // write image data
    if ( m_eCompression == CT_NONE )
    {
        qSaveStream.Write(iImageSize,qUncompressedImage);
    }
    else  // m_eCompression = CT_RLE
    {
        int iRunLength, iCodedByteQuantity = 0;
        unsigned char ucByte, ucRunLengthMO, uc128 = 128;
        while ( iCodedByteQuantity != iImageSize )
        {
            iRunLength = 1;
            ucByte = qUncompressedImage[iCodedByteQuantity];
            for (int i = iCodedByteQuantity+1; i < iImageSize; i++)
            {
                if ( qUncompressedImage[i] != ucByte )
                    break;
                if ( ++iRunLength == 256 )
                    break;
            }

            if ( iRunLength == 1 )
            {
                if ( ucByte != 128 )
                {
                    // case #1
                    qSaveStream.Write(ucByte);
                }
                else
                {
                    // case #2
                    qSaveStream.Write(uc128);
                    qSaveStream.Write(cZero);
                }
            }
            else if ( iRunLength == 2 && ucByte != 128 )
            {
                for (int i = 0; i < 2; i++)
                {
                    // case #1
                    qSaveStream.Write(ucByte);
                }
            }
            else
            {
                // case #3
                ucRunLengthMO = iRunLength - 1;
                qSaveStream.Write(uc128);
                qSaveStream.Write(ucRunLengthMO);
                qSaveStream.Write(ucByte);
            }

            iCodedByteQuantity += iRunLength;
        }
    }

    // fix hole #1
    int iImageDataLength = qSaveStream.GetBytePosition() - iBeforeImageDataPos;
    qSaveStream.PushPositionState();
    qSaveStream.SetBytePosition(16);
    qSaveStream.Write(iImageDataLength);
    qSaveStream.PopPositionState();

    // flush to the file
    CONDITIONAL_THROW_EXCEPTION(qSaveStream.Finalize(),ET_CANNOT_ACCESS_FILE);

    return true;
}
//------------------------------------------------------------------------------------------------------------------
FormatRas::FormatRas (const char* acFilename, bool& rbSuccess)
    :
    FormatBase(acFilename)
{
    bool bSuccess;

    // signature
    C_CONDITIONAL_THROW_EXCEPTION(Storage::GetFileSize(m_iFileSize,m_qFilename),ET_CANNOT_ACCESS_FILE);
    C_CONDITIONAL_THROW_EXCEPTION_WITHOUT_ASSERT_FALSE(( m_iFileSize >= 4 ),ET_FORMAT_IDENTIFIER_MISMATCH);
    Stream qSignatureStream(m_eStreaming,m_qFilename,0,4,false,bSuccess);
    C_PROCESS_STREAM_SUCCESS_FOR_LOADING(bSuccess);
    int iSignature;
    qSignatureStream.Read(iSignature);
    C_CONDITIONAL_THROW_EXCEPTION_WITHOUT_ASSERT_FALSE(( iSignature == 1504078485 ),
        ET_FORMAT_IDENTIFIER_MISMATCH);
    qSignatureStream.Finalize();

    // Header.
    Stream qHeaderStream(m_eStreaming,m_qFilename,4,28,false,bSuccess);
    C_PROCESS_STREAM_SUCCESS_FOR_LOADING(bSuccess);

    qHeaderStream.Read(m_iWidth);
    qHeaderStream.Read(m_iHeight);
    C_CONDITIONAL_THROW_EXCEPTION(( m_iWidth > 0 && m_iHeight > 0 ),ET_FORMAT_MISMATCH);

    qHeaderStream.Read(m_iBitsPerPixel);
    C_CONDITIONAL_THROW_EXCEPTION(
        ( m_iBitsPerPixel == 1 || m_iBitsPerPixel == 8 || m_iBitsPerPixel == 24 || m_iBitsPerPixel == 32 ),
        ET_FORMAT_MISMATCH);

    qHeaderStream.OffsetBytePosition(4);

    int iType;
    qHeaderStream.Read(iType);
    C_CONDITIONAL_THROW_EXCEPTION(( 0 <= iType && iType <= 3 ),ET_FORMAT_MISMATCH);
    m_eCompression = ( iType != 2 ? CT_NONE : CT_RLE );
    m_bOrderRgb = ( iType == 3 );
    C_CONDITIONAL_THROW_EXCEPTION(( !(m_bOrderRgb && m_iBitsPerPixel != 24 && m_iBitsPerPixel != 32) ),
        ET_FORMAT_MISMATCH);

    int iMapType;
    qHeaderStream.Read(iMapType);
    C_CONDITIONAL_THROW_EXCEPTION(( iMapType == 0 || iMapType == 1 ),ET_FORMAT_MISMATCH);
    if ( iMapType == 0 )
    {
        switch ( m_iBitsPerPixel )
        {
        case 1:
        case 8:
            m_eColorModel = CMT_GRAYSCALE;
            break;
        case 24:
            m_eColorModel = CMT_RGB;
            break;
        case 32:
            m_eColorModel = CMT_RGBA;
            break;
        }
    }
    else  // iMapType = 1
    {
        C_CONDITIONAL_THROW_EXCEPTION(( m_iBitsPerPixel == 1 || m_iBitsPerPixel == 8 ),ET_FORMAT_MISMATCH);
        m_eColorModel = CMT_PALETTED;
    }

    if ( m_eColorModel == CMT_PALETTED )
    {
        int iMapLength;
        qHeaderStream.Read(iMapLength);
        C_CONDITIONAL_THROW_EXCEPTION(( 3 <= iMapLength && iMapLength <= 768 && iMapLength % 3 == 0 ),
            ET_FORMAT_MISMATCH);
        m_iPaletteSize = iMapLength/3;
    }

    qHeaderStream.Finalize();

    rbSuccess = true;
}
//------------------------------------------------------------------------------------------------------------------
bool FormatRas::LoadImage (RasterImagePtr& rspqImage)
{
    bool bSuccess;

    int iNextReadPos = 32;

    RasterImage::Palette qPalette;
    if ( m_eColorModel == CMT_PALETTED )
    {
        // read the palette
        qPalette.SetQuantity(m_iPaletteSize);
        int iPaletteByteSize = m_iPaletteSize*3;
        Stream qPaletteStream(m_eStreaming,m_qFilename,iNextReadPos,iPaletteByteSize,false,bSuccess);
        PROCESS_STREAM_SUCCESS_FOR_LOADING(bSuccess);
        for (int i0 = 2; i0 >= 0; i0--)
        {
            for (int i1 = 0; i1 < m_iPaletteSize; i1++)
                qPaletteStream.Read(qPalette[i1].Components[i0]);
        }
        qPaletteStream.Finalize();

        iNextReadPos += iPaletteByteSize;
    }

    // calculate expected image size
    int iRowSizeInBits = m_iWidth*m_iBitsPerPixel;
    int iRowSize = ( (iRowSizeInBits & 7) != 0 ? iRowSizeInBits/8 + 1 : iRowSizeInBits/8 );
    if ( (iRowSize & 1) != 0 )
        iRowSize++;
    int iExpectedImageSize = m_iHeight*iRowSize;

    // get the uncompressed image
    StillArray<unsigned char> qUncompressedImage(iExpectedImageSize);
    int iImageDataSize = m_iFileSize - iNextReadPos;
    Stream qImageDataStream(m_eStreaming,m_qFilename,iNextReadPos,iImageDataSize,false,bSuccess);
    PROCESS_STREAM_SUCCESS_FOR_LOADING(bSuccess);
    if ( m_eCompression == CT_NONE )
    {
        CONDITIONAL_THROW_EXCEPTION(( iImageDataSize >= iExpectedImageSize ),ET_FORMAT_MISMATCH);
        MemCpy(qUncompressedImage,qImageDataStream,iExpectedImageSize);
    }
    else  // m_eCompression = CT_RLE
    {
        // unpack
        unsigned char ucByte;
        int iRepeatCount, iReceived = 0;
        for (/**/; /**/; /**/)
        {
            RETURN_ON_FAIL(qImageDataStream.Read(ucByte));
            if ( ucByte != 128 )
            {
                // case #1
                RETURN_ON_FAIL(( iReceived < iExpectedImageSize ));
                qUncompressedImage[iReceived++] = ucByte;
            }
            else
            {
                RETURN_ON_FAIL(qImageDataStream.Read(ucByte));
                if ( ucByte == 0 )
                {
                    // case #2
                    RETURN_ON_FAIL(( iReceived < iExpectedImageSize ));
                    qUncompressedImage[iReceived++] = 128;
                }
                else
                {
                    // case #3
                    iRepeatCount = ucByte + 1;
                    RETURN_ON_FAIL(( iReceived + iRepeatCount <= iExpectedImageSize ));
                    RETURN_ON_FAIL(qImageDataStream.Read(ucByte));
                    for (int i = 0; i < iRepeatCount; i++)
                        qUncompressedImage[iReceived++] = ucByte;
                }
            }

            if ( iReceived == iExpectedImageSize )
                break;
        }
    }
    qImageDataStream.Finalize();

    // for a progress bar
    ProgressProbe qProgress(this);
    qProgress.SetAdvance(1.0f/m_iHeight);

    // read the image
    unsigned char* pucRow;
    unsigned char* pucComp;
    if ( m_eColorModel == CMT_PALETTED )
    {
        // allocate the output image
        rspqImage = new RasterImage(RasterImage::IT_PALETTED,m_iWidth,m_iHeight,qPalette);
        pucRow = (unsigned char*)rspqImage->GetImage();
        pucComp = pucRow;

        int iBytePos, iBitRatio, iValue;
        for (int iY = 0; iY < m_iHeight; iY++)
        {
            iBytePos = iY*iRowSize;
            iBitRatio = 0;
            for (int iX = 0; iX < m_iWidth; iX++)
            {
                iBytePos += Storage::ReadUBits((const char*)(qUncompressedImage+iBytePos),iBitRatio,
                    m_iBitsPerPixel,iValue);
                CONDITIONAL_THROW_EXCEPTION(( iValue < m_iPaletteSize ),ET_FORMAT_MISMATCH);
                *pucComp++ = iValue;
            }

            pucRow += rspqImage->GetRowStride();
            pucComp = pucRow;

            qProgress.MakeAdvance();
        }
    }
    else if ( m_eColorModel == CMT_GRAYSCALE )
    {
        // allocate the output image
        rspqImage = new RasterImage(RasterImage::IT_GRAYSCALE_8,m_iWidth,m_iHeight);
        pucRow = (unsigned char*)rspqImage->GetImage();
        pucComp = pucRow;

        if ( m_iBitsPerPixel == 1 )
        {
            int iBytePos, iBitRatio, iValue;
            for (int iY = 0; iY < m_iHeight; iY++)
            {
                iBytePos = iY*iRowSize;
                iBitRatio = 0;
                for (int iX = 0; iX < m_iWidth; iX++)
                {
                    iBytePos += Storage::ReadUBits((const char*)(qUncompressedImage+iBytePos),iBitRatio,1,iValue);
                    *pucComp++ = ( iValue == 0 ? 0 : 255 );
                }

                pucRow += rspqImage->GetRowStride();
                pucComp = pucRow;

                qProgress.MakeAdvance();
            }
        }
        else  // m_iBitsPerPixel = 8
        {
            int iBytePos;
            for (int iY = 0; iY < m_iHeight; iY++)
            {
                iBytePos = iY*iRowSize;
                for (int iX = 0; iX < m_iWidth; iX++)
                    *pucComp++ = qUncompressedImage[iBytePos++];

                pucRow += rspqImage->GetRowStride();
                pucComp = pucRow;

                qProgress.MakeAdvance();
            }
        }
    }
    else if ( m_eColorModel == CMT_RGB )
    {
        // allocate the output image
        rspqImage = new RasterImage(RasterImage::IT_RGB_8_8_8,m_iWidth,m_iHeight);
        pucRow = (unsigned char*)rspqImage->GetImage();
        pucComp = pucRow;

        int iBytePos;
        for (int iY = 0; iY < m_iHeight; iY++)
        {
            iBytePos = iY*iRowSize;
            if ( !m_bOrderRgb )
            {
                for (int iX = 0; iX < m_iWidth; iX++)
                {
                    pucComp[2] = qUncompressedImage[iBytePos++];
                    pucComp[1] = qUncompressedImage[iBytePos++];
                    pucComp[0] = qUncompressedImage[iBytePos++];

                    pucComp += 3;
                }
            }
            else
            {
                for (int iX = 0; iX < m_iWidth; iX++)
                {
                    pucComp[0] = qUncompressedImage[iBytePos++];
                    pucComp[1] = qUncompressedImage[iBytePos++];
                    pucComp[2] = qUncompressedImage[iBytePos++];

                    pucComp += 3;
                }
            }

            pucRow += rspqImage->GetRowStride();
            pucComp = pucRow;

            qProgress.MakeAdvance();
        }
    }
    else  // m_eColorModel = CMT_RGBA
    {
        // allocate the output image
        rspqImage = new RasterImage(RasterImage::IT_RGBA_8_8_8_8,m_iWidth,m_iHeight);
        pucRow = (unsigned char*)rspqImage->GetImage();
        pucComp = pucRow;

        int iBytePos;
        for (int iY = 0; iY < m_iHeight; iY++)
        {
            iBytePos = iY*iRowSize;
            if ( !m_bOrderRgb )
            {
                for (int iX = 0; iX < m_iWidth; iX++)
                {
                    pucComp[3] = qUncompressedImage[iBytePos++];
                    pucComp[2] = qUncompressedImage[iBytePos++];
                    pucComp[1] = qUncompressedImage[iBytePos++];
                    pucComp[0] = qUncompressedImage[iBytePos++];

                    pucComp += 4;
                }
            }
            else
            {
                for (int iX = 0; iX < m_iWidth; iX++)
                {
                    pucComp[3] = qUncompressedImage[iBytePos++];
                    pucComp[0] = qUncompressedImage[iBytePos++];
                    pucComp[1] = qUncompressedImage[iBytePos++];
                    pucComp[2] = qUncompressedImage[iBytePos++];

                    pucComp += 4;
                }
            }

            pucRow += rspqImage->GetRowStride();
            pucComp = pucRow;

            qProgress.MakeAdvance();
        }
    }

    return true;
}
//------------------------------------------------------------------------------------------------------------------



