#include "QFormatPcx.h"
#include "QExptWarnBase.mcr"
#include "QFormatBase.mcr"
#include "QProgressProbe.h"
#include "QStorage.h"
using namespace Q;

// support for loading
static const int gs_iSignatureQuantity(5);
static const unsigned char gs_aaucSignature[gs_iSignatureQuantity][3] = {
    {10,  0,  1},
    {10,  2,  1},
    {10,  3,  1},
    {10,  4,  1},
    {10,  5,  1}};

//------------------------------------------------------------------------------------------------------------------
FormatPcx::FormatPcx (const RasterImagePtr& rspqImage, int iBitsPerPixel)
    :
    m_spqImageSaveTo(rspqImage),
    m_iWidth(rspqImage->GetWidth()),
    m_iHeight(rspqImage->GetHeight()),
    m_iHorizontalResolution(72),
    m_iVerticalResolution(72)
{
    assert( rspqImage->GetWidth() != 0 && rspqImage->GetHeight() != 0 );
    assert( rspqImage->GetWidth() <= 65536 && rspqImage->GetHeight() <= 65536 );
    assert( rspqImage->GetColorModel() == RasterImage::CMT_PALETTED
        ||  rspqImage->GetColorModel() == RasterImage::CMT_GRAYSCALE
        ||  rspqImage->GetColorModel() == RasterImage::CMT_RGB );
    assert( !(rspqImage->GetColorModel() != RasterImage::CMT_PALETTED && iBitsPerPixel == -1) );
    assert( !rspqImage->HasVisibilityMask() );

    if ( iBitsPerPixel == -1 )
    {
        // find out the minimal possible bits-per-pixel value
        iBitsPerPixel = Storage::GetUBitQuantity(rspqImage->GetPaletteSize()-1);
        if ( iBitsPerPixel == 1 )
        {
            // rely on the special case
            iBitsPerPixel = 2;
        }
        else
        {
            if ( iBitsPerPixel == 3 )
                iBitsPerPixel = 4;
            else if ( 4 < iBitsPerPixel && iBitsPerPixel < 8 )
                iBitsPerPixel = 8;
        }
    }

    m_iBitsPerPixel = iBitsPerPixel;
}
//------------------------------------------------------------------------------------------------------------------
bool FormatPcx::SaveImage (const char* acFilename)
{
    assert( (m_spqImageSaveTo->GetColorModel() == RasterImage::CMT_PALETTED
        &&  (m_iBitsPerPixel == 1 || m_iBitsPerPixel == 2 || m_iBitsPerPixel == 4 || m_iBitsPerPixel == 8))
        ||  (m_spqImageSaveTo->GetColorModel() == RasterImage::CMT_GRAYSCALE
        &&  (m_iBitsPerPixel == 2 || m_iBitsPerPixel == 4 || m_iBitsPerPixel == 8))
        ||  (m_spqImageSaveTo->GetColorModel() == RasterImage::CMT_RGB
        &&  (m_iBitsPerPixel == 3 || m_iBitsPerPixel == 6 || m_iBitsPerPixel == 12 || m_iBitsPerPixel == 24)) );
    assert( !(m_spqImageSaveTo->GetColorModel() == RasterImage::CMT_PALETTED
        &&  1 << m_iBitsPerPixel < m_spqImageSaveTo->GetPaletteSize()) );

    // detect PCX's color model
    switch ( m_spqImageSaveTo->GetColorModel() )
    {
    case RasterImage::CMT_PALETTED:
        m_eColorModel = CMT_PALETTED;
        break;
    case RasterImage::CMT_GRAYSCALE:
        m_eColorModel = CMT_GRAYSCALE;
        break;
    case RasterImage::CMT_RGB:
        m_eColorModel = CMT_RGB;
        break;
    }

    Stream qSaveStream(acFilename,true);

    // Header.

    // write PCX's signature, latest version, and encoding type
    qSaveStream.Write(3,gs_aaucSignature[gs_iSignatureQuantity-1]);

    m_iColorPlaneQuantity = ( m_eColorModel != CMT_RGB ? 1 : 3 );
    m_iBitsPerSample = m_iBitsPerPixel/m_iColorPlaneQuantity;

    unsigned char ucBitsPerSample = m_iBitsPerSample;
    qSaveStream.Write(ucBitsPerSample);

    unsigned short usMinX = 0;
    unsigned short usMinY = 0;
    unsigned short usMaxX = m_iWidth - 1;
    unsigned short usMaxY = m_iHeight - 1;
    qSaveStream.Write(usMinX);
    qSaveStream.Write(usMinY);
    qSaveStream.Write(usMaxX);
    qSaveStream.Write(usMaxY);

    unsigned short usDpiH = m_iHorizontalResolution;
    unsigned short usDpiV = m_iVerticalResolution;
    qSaveStream.Write(usDpiH);
    qSaveStream.Write(usDpiV);

    unsigned char ucByte;
    char cZero = 0;
    if ( m_eColorModel == CMT_PALETTED && (m_iBitsPerSample == 2 || m_iBitsPerSample == 4) )
    {
        // flow out the palette
        for (int i = 0; i < m_spqImageSaveTo->GetPaletteSize(); i++)
        {
            const PaletteColor& rqPaletteColor = m_spqImageSaveTo->GetPaletteColor(i);

            qSaveStream.Write(rqPaletteColor.R());
            qSaveStream.Write(rqPaletteColor.G());
            qSaveStream.Write(rqPaletteColor.B());
        }

        // fill remaining space
        int iDegree = 16 - m_spqImageSaveTo->GetPaletteSize();
        for (int i = 0; i < iDegree; i++)
        {
            qSaveStream.Write(cZero);
            qSaveStream.Write(cZero);
            qSaveStream.Write(cZero);
        }
    }
    else
    {
        if ( m_eColorModel == CMT_GRAYSCALE && m_iBitsPerSample == 2 )
        {
            // weird grayscale palette
            ucByte = 0;
            qSaveStream.Write(ucByte);
            qSaveStream.Write(ucByte);
            qSaveStream.Write(ucByte);
            for (int i = 1; i < 3; i++)
            {
                ucByte = 255*i/3;
                qSaveStream.Write(ucByte);
                qSaveStream.Write(ucByte);
                qSaveStream.Write(ucByte);
            }
            ucByte = 255;
            qSaveStream.Write(ucByte);
            qSaveStream.Write(ucByte);
            qSaveStream.Write(ucByte);

            // fill remaining space
            for (int i = 0; i < 12; i++)
            {
                qSaveStream.Write(cZero);
                qSaveStream.Write(cZero);
                qSaveStream.Write(cZero);
            }
        }
        else if ( m_eColorModel == CMT_GRAYSCALE && m_iBitsPerSample == 4 )
        {
            // weird grayscale palette
            ucByte = 0;
            qSaveStream.Write(ucByte);
            qSaveStream.Write(ucByte);
            qSaveStream.Write(ucByte);
            for (int i = 1; i < 15; i++)
            {
                ucByte = 255*i/15;
                qSaveStream.Write(ucByte);
                qSaveStream.Write(ucByte);
                qSaveStream.Write(ucByte);
            }
            ucByte = 255;
            qSaveStream.Write(ucByte);
            qSaveStream.Write(ucByte);
            qSaveStream.Write(ucByte);
        }
        else
        {
            // output zero bytes
            for (int i = 0; i < 48; i++)
                qSaveStream.Write(cZero);
        }
    }

    qSaveStream.Write(cZero);

    unsigned char ucPlanes = m_iColorPlaneQuantity;
    qSaveStream.Write(ucPlanes);

    int iBitsPerRow = m_iWidth*m_iBitsPerSample;
    m_iBytesPerPlaneRow = ( (iBitsPerRow & 7) != 0 ? iBitsPerRow/8 + 1 : iBitsPerRow/8 );
    if ( (m_iBytesPerPlaneRow & 1) != 0 )
        m_iBytesPerPlaneRow++;
    unsigned short usBytesPerLine = m_iBytesPerPlaneRow;
    qSaveStream.Write(usBytesPerLine);

    unsigned short usPaletteInfo = ( m_eColorModel != CMT_GRAYSCALE ? 1 : 2 );
    qSaveStream.Write(usPaletteInfo);

    // no PCX's "HscreenSize" or "VscreenSize"
    for (int i = 0; i < 58; i++)
        qSaveStream.Write(cZero);

    // for a progress bar
    ProgressProbe qProgress(this);
    qProgress.SetAdvance(1.0f/m_iHeight);

    // write image data
    int iTotalBytesPerRow = m_iColorPlaneQuantity*m_iBytesPerPlaneRow;
    StillArray<char> qDecompressedRow(iTotalBytesPerRow);
    int iBytePos, iBitRatio, iValue, iStartByte, iEqLength;
    unsigned char ucEqByte, ucCountByte;
    int iMaxValue = (1 << m_iBitsPerSample) - 1;
    int aiVarA[3] = {
        0*m_iBytesPerPlaneRow,
        1*m_iBytesPerPlaneRow,
        2*m_iBytesPerPlaneRow};
    for (int iY = 0; iY < m_iHeight; iY++)
    {
        // to make RLE more efficient
        MemSet(qDecompressedRow,0,iTotalBytesPerRow);

        if ( m_eColorModel == CMT_PALETTED )
        {
            iBytePos = 0;
            iBitRatio = 0;
            for (int iX = 0; iX < m_iWidth; iX++)
            {
                iValue = m_spqImageSaveTo->GetPixelIndex(iX,iY);
                iBytePos += Storage::WriteBits(qDecompressedRow+iBytePos,iBitRatio,m_iBitsPerSample,iValue);
            }
        }
        else if ( m_eColorModel == CMT_GRAYSCALE )
        {
            iBytePos = 0;
            iBitRatio = 0;
            for (int iX = 0; iX < m_iWidth; iX++)
            {
                iValue = Mathf::RoundToInt(m_spqImageSaveTo->GetPixelIntensity(iX,iY)*iMaxValue);
                iBytePos += Storage::WriteBits(qDecompressedRow+iBytePos,iBitRatio,m_iBitsPerSample,iValue);
            }
        }
        else  // m_eColorModel = CMT_RGB
        {
            ColorRgb qColor;
            int iVar0 = 0;
            int iVar1, iVar2;
            for (int iX = 0; iX < m_iWidth; iX++, iVar0 += m_iBitsPerSample)
            {
                qColor = m_spqImageSaveTo->GetPixelRgb(iX,iY);
                iVar1 = iVar0/8;
                iVar2 = iVar0 & 7;
                for (int i = 0; i < 3; i++)
                {
                    iBitRatio = iVar2;
                    iValue = Mathf::RoundToInt(qColor[i]*iMaxValue);
                    Storage::WriteBits(qDecompressedRow+aiVarA[i]+iVar1,iBitRatio,m_iBitsPerSample,iValue);
                }
            }
        }

        // encode the row with RLE
        iStartByte = 0;
        for (/**/; /**/; /**/)
        {
            iEqLength = 1;
            for (int i0 = iStartByte, i1 = iStartByte+1; i1 < iTotalBytesPerRow; i0++, i1++)
            {
                if ( qDecompressedRow[i1] != qDecompressedRow[i0] )
                    break;
                if ( ++iEqLength == 63 )
                    break;
            }

            ucEqByte = qDecompressedRow[iStartByte];
            if ( iEqLength != 1 || ucEqByte >> 6 == 3 )
            {
                ucCountByte = iEqLength | 192;
                qSaveStream.Write(ucCountByte);
            }
            qSaveStream.Write(ucEqByte);

            iStartByte += iEqLength;
            if ( iStartByte == iTotalBytesPerRow )
                break;
        }

        qProgress.MakeAdvance();
    }

    if ( m_iBitsPerSample == 8 )
    {
        if ( m_eColorModel == CMT_PALETTED )
        {
            // flow out the palette of 256 length
            unsigned char ucCheckValue = 12;
            qSaveStream.Write(ucCheckValue);
            for (int i = 0; i < m_spqImageSaveTo->GetPaletteSize(); i++)
            {
                const PaletteColor& rqPaletteColor = m_spqImageSaveTo->GetPaletteColor(i);

                qSaveStream.Write(rqPaletteColor.R());
                qSaveStream.Write(rqPaletteColor.G());
                qSaveStream.Write(rqPaletteColor.B());
            }

            // fill remaining space
            int iDegree = 256 - m_spqImageSaveTo->GetPaletteSize();
            for (int i = 0; i < iDegree; i++)
            {
                qSaveStream.Write(cZero);
                qSaveStream.Write(cZero);
                qSaveStream.Write(cZero);
            }
        }
        else if ( m_eColorModel == CMT_GRAYSCALE )
        {
            // weird one
            unsigned char ucCheckValue = 12;
            qSaveStream.Write(ucCheckValue);
            for (int i = 0; i < 256; i++)
            {
                ucByte = i;
                qSaveStream.Write(ucByte);
                qSaveStream.Write(ucByte);
                qSaveStream.Write(ucByte);
            }
        }
    }

    // flush to the file
    CONDITIONAL_THROW_EXCEPTION(qSaveStream.Finalize(),ET_CANNOT_ACCESS_FILE);

    return true;
}
//------------------------------------------------------------------------------------------------------------------
FormatPcx::FormatPcx (const char* acFilename, bool& rbSuccess)
    :
    FormatBase(acFilename)
{
    bool bSuccess;

    // signature(s)
    int iFileSize;
    C_CONDITIONAL_THROW_EXCEPTION(Storage::GetFileSize(iFileSize,m_qFilename),ET_CANNOT_ACCESS_FILE);
    C_CONDITIONAL_THROW_EXCEPTION_WITHOUT_ASSERT_FALSE(( iFileSize >= 3 ),ET_FORMAT_IDENTIFIER_MISMATCH);
    Stream qSignatureStream(m_eStreaming,m_acStreamSrcParam,0,3,true,bSuccess);
    C_PROCESS_STREAM_SUCCESS_FOR_LOADING(bSuccess);
    char acSignature[3];
    qSignatureStream.Read(3,acSignature);
    qSignatureStream.Finalize();
    bool bAtLeastOne = false;
    for (int i = 0; i < gs_iSignatureQuantity; i++)
    {
        if ( MemCmp(acSignature,gs_aaucSignature[i],3) == 0 )
        {
            bAtLeastOne = true;
            break;
        }
    }
    C_CONDITIONAL_THROW_EXCEPTION_WITHOUT_ASSERT_FALSE(bAtLeastOne,ET_FORMAT_IDENTIFIER_MISMATCH);

    // Header.
    Stream qHeaderStream(m_eStreaming,m_acStreamSrcParam,3,67,true,bSuccess);
    C_PROCESS_STREAM_SUCCESS_FOR_LOADING(bSuccess);

    unsigned char ucBitsPerSample;
    qHeaderStream.Read(ucBitsPerSample);
    C_CONDITIONAL_THROW_EXCEPTION((
         ucBitsPerSample == 1
    ||   ucBitsPerSample == 2
    ||   ucBitsPerSample == 4
    ||   ucBitsPerSample == 8 ),ET_FORMAT_MISMATCH);
    m_iBitsPerSample = ucBitsPerSample;

    unsigned short usMinX, usMinY, usMaxX, usMaxY;
    qHeaderStream.Read(usMinX);
    qHeaderStream.Read(usMinY);
    qHeaderStream.Read(usMaxX);
    qHeaderStream.Read(usMaxY);
    C_CONDITIONAL_THROW_EXCEPTION(( usMinX <= usMaxX && usMinY <= usMaxY ),ET_FORMAT_MISMATCH);
    m_iWidth = usMaxX - usMinX + 1;
    m_iHeight = usMaxY - usMinY + 1;

    unsigned short usDpiH, usDpiV;
    qHeaderStream.Read(usDpiH);
    qHeaderStream.Read(usDpiV);
    m_iHorizontalResolution = usDpiH;
    m_iVerticalResolution = usDpiV;

    qHeaderStream.OffsetBytePosition(49);

    // for plane number 4 only the case supported when bits-per-sample is 1
    unsigned char ucPlanes;
    qHeaderStream.Read(ucPlanes);
    C_CONDITIONAL_THROW_EXCEPTION(( ucPlanes == 1 || ucPlanes == 3 || (ucPlanes == 4 && m_iBitsPerSample == 1) ),
        ET_FORMAT_MISMATCH);
    m_iColorPlaneQuantity = ucPlanes;

    m_iBitsPerPixel = m_iColorPlaneQuantity*m_iBitsPerSample;

    // PCX's "BytesPerLine" must be an even number, but it's common to do the concession
    unsigned short usBytesPerLine;
    qHeaderStream.Read(usBytesPerLine);
    int iBitsPerRow = m_iWidth*m_iBitsPerSample;
    int iBytesPerPlaneRowNoPadding = ( (iBitsPerRow & 7) != 0 ? iBitsPerRow/8 + 1 : iBitsPerRow/8 );
    C_CONDITIONAL_THROW_EXCEPTION(( usBytesPerLine >= iBytesPerPlaneRowNoPadding ),ET_FORMAT_MISMATCH);
    m_iBytesPerPlaneRow = usBytesPerLine;

    // some programs write 0 in this field
    unsigned short usPaletteInfo;
    qHeaderStream.Read(usPaletteInfo);
    C_CONDITIONAL_THROW_EXCEPTION(( 0 <= usPaletteInfo && usPaletteInfo <= 2 ),ET_FORMAT_MISMATCH);

    qHeaderStream.Finalize();

    // find out the used color model
    if ( m_iColorPlaneQuantity == 1 )
        m_eColorModel = ( usPaletteInfo != 2 ? CMT_PALETTED : CMT_GRAYSCALE );
    else  // m_iColorPlaneQuantity = 3 or m_iColorPlaneQuantity = 4
        m_eColorModel = CMT_RGB;

    rbSuccess = true;
}
//------------------------------------------------------------------------------------------------------------------
bool FormatPcx::LoadImage (RasterImagePtr& rspqImage)
{
    bool bSuccess;

    int iFileSize;
    CONDITIONAL_THROW_EXCEPTION(Storage::GetFileSize(iFileSize,m_qFilename),ET_CANNOT_ACCESS_FILE);

    if ( m_eColorModel == CMT_PALETTED )
    {
        // firstly, load the palette
        RasterImage::Palette qPalette;
        if ( m_iBitsPerSample != 8 )
        {
            if ( m_iBitsPerSample != 1 )
            {
                // palette will have length 16
                Stream qPaletteStream(m_eStreaming,m_acStreamSrcParam,16,48,true,bSuccess);
                PROCESS_STREAM_SUCCESS_FOR_LOADING(bSuccess);
                qPalette.SetQuantity(16);
                for (int i = 0; i < 16; i++)
                {
                    PaletteColor& rqPaletteColor = qPalette[i];

                    qPaletteStream.Read(rqPaletteColor.R());
                    qPaletteStream.Read(rqPaletteColor.G());
                    qPaletteStream.Read(rqPaletteColor.B());
                }
                qPaletteStream.Finalize();
            }
            else
            {
                // palette will have length 2
                qPalette.SetQuantity(2);
                qPalette[0] = PaletteColor(0,0,0);
                qPalette[1] = PaletteColor(255,255,255);
            }
        }
        else
        {
            // palette will have length 256
            CONDITIONAL_THROW_EXCEPTION(( iFileSize > 769 ),ET_FORMAT_MISMATCH);
            Stream qPaletteStream(m_eStreaming,m_acStreamSrcParam,iFileSize-769,769,true,bSuccess);
            PROCESS_STREAM_SUCCESS_FOR_LOADING(bSuccess);
            unsigned char ucCheckValue;
            qPaletteStream.Read(ucCheckValue);
            CONDITIONAL_THROW_EXCEPTION(( ucCheckValue == 12 ),ET_FORMAT_MISMATCH);
            qPalette.SetQuantity(256);
            for (int i = 0; i < 256; i++)
            {
                PaletteColor& rqPaletteColor = qPalette[i];

                qPaletteStream.Read(rqPaletteColor.R());
                qPaletteStream.Read(rqPaletteColor.G());
                qPaletteStream.Read(rqPaletteColor.B());
            }
            qPaletteStream.Finalize();
        }

        rspqImage = new RasterImage(RasterImage::IT_PALETTED,m_iWidth,m_iHeight,qPalette);
    }
    else if ( m_eColorModel == CMT_GRAYSCALE )
    {
        rspqImage = new RasterImage(RasterImage::IT_GRAYSCALE_8,m_iWidth,m_iHeight);
    }
    else  // m_eColorModel = CMT_RGB
    {
        rspqImage = new RasterImage(RasterImage::IT_RGB_8_8_8,m_iWidth,m_iHeight);
    }

    // for a progress bar
    ProgressProbe qProgress(this);
    qProgress.SetAdvance(1.0f/m_iHeight);

    // read image data
    CONDITIONAL_THROW_EXCEPTION(( iFileSize > 128 ),ET_FORMAT_MISMATCH);
    Stream qImageStream(m_eStreaming,m_acStreamSrcParam,128,iFileSize-128,true,bSuccess);
    PROCESS_STREAM_SUCCESS_FOR_LOADING(bSuccess);
    unsigned char ucValue, ucPixValue;
    int iCount, iBytePos, iBitRatio, iValue, iTotalBytesPerRow = m_iColorPlaneQuantity*m_iBytesPerPlaneRow;
    StillArray<char> qDecompressedRow(iTotalBytesPerRow);
    int iBytesInRow = 0;
    int iRowsRead = 0;
    unsigned char* pucRow;
    unsigned char* pucComp;
    int iMaxValue = (1 << m_iBitsPerSample) - 1;
    int aiVarA[4] = {
        0*m_iBytesPerPlaneRow,
        1*m_iBytesPerPlaneRow,
        2*m_iBytesPerPlaneRow,
        3*m_iBytesPerPlaneRow};
    for (/**/; /**/; /**/)
    {
        RETURN_ON_FAIL(qImageStream.Read(ucValue));
        if ( ucValue >> 6 == 3 )
        {
            // "count" run
            iCount = ucValue & 63;
            RETURN_ON_FAIL(qImageStream.Read(ucPixValue));
        }
        else
        {
            // one-step run
            iCount = 1;
            ucPixValue = ucValue;
        }

        for (int i = 0; i < iCount; i++)
        {
            if ( iBytesInRow == iTotalBytesPerRow )
                goto Exit;  // a concession
            qDecompressedRow[iBytesInRow++] = ucPixValue;
        }

        if ( iBytesInRow == iTotalBytesPerRow )
        {
            // got the next row for the output image
            if ( iRowsRead == m_iHeight )
                break;  // a concession
            pucRow = (unsigned char*)rspqImage->GetRow(iRowsRead++);
            pucComp = pucRow;
            if ( m_eColorModel == CMT_PALETTED )
            {
                iBytePos = 0;
                iBitRatio = 0;
                for (int iX = 0; iX < m_iWidth; iX++)
                {
                    iBytePos += Storage::ReadUBits(qDecompressedRow+iBytePos,iBitRatio,m_iBitsPerSample,iValue);
                    CONDITIONAL_THROW_EXCEPTION(( iValue < rspqImage->GetPaletteSize() ),
                        ET_FORMAT_MISMATCH);
                    *pucComp++ = iValue;
                }
            }
            else if ( m_eColorModel == CMT_GRAYSCALE )
            {
                iBytePos = 0;
                iBitRatio = 0;
                for (int iX = 0; iX < m_iWidth; iX++)
                {
                    iBytePos += Storage::ReadUBits(qDecompressedRow+iBytePos,iBitRatio,m_iBitsPerSample,iValue);
                    *pucComp++ = Mathf::RoundToInt(((float)iValue)/iMaxValue*255);
                }
            }
            else  // m_eColorModel = CMT_RGB
            {
                if ( m_iColorPlaneQuantity != 4 )
                {
                    // regular case for 3 color planes
                    for (int i = 0; i < 3; i++)
                    {
                        iBytePos = aiVarA[i];
                        iBitRatio = 0;

                        pucComp = pucRow + i;
                        for (int iX = 0; iX < m_iWidth; iX++)
                        {
                            iBytePos += Storage::ReadUBits(qDecompressedRow+iBytePos,iBitRatio,m_iBitsPerSample,
                                iValue);
                            *pucComp = Mathf::RoundToInt(((float)iValue)/iMaxValue*255);
                            pucComp += 3;
                        }
                    }
                }
                else
                {
                    // special CGA's RGBI case
                    int aiValue[4];
                    int iVar0, iVar1;
                    for (int iX = 0; iX < m_iWidth; iX++)
                    {
                        iVar0 = iX/8;
                        iVar1 = iX & 7;
                        for (int i = 0; i < 4; i++)
                        {
                            iBitRatio = iVar1;
                            Storage::ReadUBits(qDecompressedRow+aiVarA[i]+iVar0,iBitRatio,1,aiValue[i]);
                        }

                        for (int i = 0; i < 3; i++)
                            pucComp[i] = ( aiValue[i] == 0 ? 0 : 128 );
                        if ( aiValue[3] == 0 )
                        {
                            if ( pucComp[0] == 128
                            &&   pucComp[1] == 128
                            &&   pucComp[2] == 128 )
                            {
                                // first special case
                                for (int i = 0; i < 3; i++)
                                    pucComp[i] = 192;
                            }
                        }
                        else
                        {
                            if ( pucComp[0] == 0
                            &&   pucComp[1] == 0
                            &&   pucComp[2] == 0 )
                            {
                                // second special case
                                for (int i = 0; i < 3; i++)
                                    pucComp[i] = 128;
                            }
                            else
                            {
                                // "intensify" color
                                for (int i = 0; i < 3; i++)
                                {
                                    if ( aiValue[i] != 0 )
                                        pucComp[i] = 255;
                                }
                            }
                        }

                        pucComp += 3;
                    }
                }
            }

            qProgress.MakeAdvance();

            if ( iRowsRead == m_iHeight )
                break;
            iBytesInRow = 0;
        }
    }
Exit:
    qImageStream.Finalize();

    return true;
}
//------------------------------------------------------------------------------------------------------------------



