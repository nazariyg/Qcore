#include "QFormatWbmp.h"
#include "QExptWarnBase.mcr"
#include "QFormatBase.mcr"
#include "QProgressProbe.h"
#include "QStorage.h"
using namespace Q;

//------------------------------------------------------------------------------------------------------------------
FormatWbmp::FormatWbmp (const RasterImagePtr& rspqImage)
    :
    m_spqImageSaveTo(rspqImage),
    m_iWidth(rspqImage->GetWidth()),
    m_iHeight(rspqImage->GetHeight())
{
    assert( rspqImage->GetWidth() != 0 && rspqImage->GetHeight() != 0 );
    assert( rspqImage->GetColorModel() == RasterImage::CMT_PALETTED );
    assert( rspqImage->GetPaletteSize() == 2
        &&  rspqImage->GetPaletteColor(0) == PaletteColor(0,0,0)
        &&  rspqImage->GetPaletteColor(1) == PaletteColor(255,255,255) );
    assert( !rspqImage->HasVisibilityMask() );
}
//------------------------------------------------------------------------------------------------------------------
bool FormatWbmp::SaveImage (const char* acFilename)
{
    Stream qSaveStream(acFilename,true);

    // Type
    unsigned int uiTypeField = 0;
    WriteMultiByteUI32(uiTypeField,qSaveStream);

    // FixHeader
    unsigned char ucFixHeaderField = 0;
    qSaveStream.Write(ucFixHeaderField);

    // image dimensions
    unsigned int uiWidth = m_iWidth;
    unsigned int uiHeight = m_iHeight;
    WriteMultiByteUI32(uiWidth,qSaveStream);
    WriteMultiByteUI32(uiHeight,qSaveStream);

    // for a progress bar
    ProgressProbe qProgress(this);
    qProgress.SetAdvance(1.0f/m_iHeight);

    // write the image
    unsigned char* pucRow = (unsigned char*)m_spqImageSaveTo->GetImage();
    unsigned char* aucPix = pucRow;
    int iValue;
    for (int iY = 0; iY < m_iHeight; iY++)
    {
        for (int iX = 0; iX < m_iWidth; iX++)
        {
            iValue = *aucPix++;
            qSaveStream.WriteBits(iValue,1);
        }

        // alignment (fill unused bits with 0s)
        if ( qSaveStream.GetBitRatio() != 0 )
            qSaveStream.WriteBits(0,8-qSaveStream.GetBitRatio());

        pucRow += m_spqImageSaveTo->GetRowStride();
        aucPix = pucRow;

        qProgress.MakeAdvance();
    }

    // flush to the file
    CONDITIONAL_THROW_EXCEPTION(qSaveStream.Finalize(),ET_CANNOT_ACCESS_FILE);

    return true;
}
//------------------------------------------------------------------------------------------------------------------
FormatWbmp::FormatWbmp (const char* acFilename, bool& rbSuccess)
    :
    FormatBase(acFilename)
{
    bool bSuccess;

    C_CONDITIONAL_THROW_EXCEPTION(Storage::GetFileSize(m_iFileSize,m_qFilename),ET_CANNOT_ACCESS_FILE);
    Stream qDimStream(m_eStreaming,m_acStreamSrcParam,0,m_iFileSize,true,bSuccess);
    C_PROCESS_STREAM_SUCCESS_FOR_LOADING(bSuccess);

    // Type
    unsigned int uiTypeField;
    C_RETURN_ON_FAIL(ReadMultiByteUI32(uiTypeField,qDimStream));
    C_CONDITIONAL_THROW_EXCEPTION(( uiTypeField == 0 ),ET_UNSUPPORTED_WBMP_TYPE);

    // FixHeader
    unsigned char ucFixHeaderField;
    C_RETURN_ON_FAIL(qDimStream.Read(ucFixHeaderField));
    C_CONDITIONAL_THROW_EXCEPTION(( (ucFixHeaderField & 128) == 0 ),ET_UNSUPPORTED_WBMP_TYPE);

    // image dimensions
    unsigned int uiWidth, uiHeight;
    C_RETURN_ON_FAIL(ReadMultiByteUI32(uiWidth,qDimStream));
    C_RETURN_ON_FAIL(ReadMultiByteUI32(uiHeight,qDimStream));
    C_CONDITIONAL_THROW_EXCEPTION(( uiWidth != 0 && uiHeight != 0 ),ET_FORMAT_MISMATCH);
    m_iWidth = uiWidth;
    m_iHeight = uiHeight;

    // for further actual image loading
    m_iImageStartByte = qDimStream.GetBytePosition();

    qDimStream.Finalize();

    rbSuccess = true;
}
//------------------------------------------------------------------------------------------------------------------
bool FormatWbmp::LoadImage (RasterImagePtr& rspqImage)
{
    bool bSuccess;

    // allocate the image
    RasterImage::Palette qBWPalette(2);
    qBWPalette[0] = PaletteColor(0,0,0);
    qBWPalette[1] = PaletteColor(255,255,255);
    rspqImage = new RasterImage(RasterImage::IT_PALETTED,m_iWidth,m_iHeight,qBWPalette);

    // for a progress bar
    ProgressProbe qProgress(this);
    qProgress.SetAdvance(1.0f/m_iHeight);

    // load the image
    CONDITIONAL_THROW_EXCEPTION(( m_iFileSize > m_iImageStartByte ),ET_FORMAT_MISMATCH);
    Stream qImageStream(m_eStreaming,m_acStreamSrcParam,m_iImageStartByte,m_iFileSize-m_iImageStartByte,true,
        bSuccess);
    PROCESS_STREAM_SUCCESS_FOR_LOADING(bSuccess);
    unsigned char* pucRow = (unsigned char*)rspqImage->GetImage();
    unsigned char* aucPix = pucRow;
    int iValue;
    for (int iY = 0; iY < m_iHeight; iY++)
    {
        for (int iX = 0; iX < m_iWidth; iX++)
        {
            RETURN_ON_FAIL(qImageStream.ReadUBits(iValue,1));
            *aucPix++ = iValue;
        }

        // alignment
        qImageStream.AlignBytes();

        pucRow += rspqImage->GetRowStride();
        aucPix = pucRow;

        qProgress.MakeAdvance();
    }
    qImageStream.Finalize();

    return true;
}
//------------------------------------------------------------------------------------------------------------------
void FormatWbmp::WriteMultiByteUI32 (unsigned int uiValue, Stream& rqStream)
{
    unsigned int uiV = uiValue;
    int iMBytes = 1;
    for (/**/; /**/; /**/)
    {
        uiV >>= 7;
        if ( uiV == 0 )
            break;
        iMBytes++;
    }

    unsigned char aucByte[5];
    int iMBMO = iMBytes - 1;
    for (int i = iMBMO; i >= 0; i--)
    {
        aucByte[i] = ( i == iMBMO ? 0 : 128 ) | (uiValue & 127);
        uiValue >>= 7;
    }

    rqStream.Write(iMBytes,aucByte);
}
//------------------------------------------------------------------------------------------------------------------
bool FormatWbmp::ReadMultiByteUI32 (unsigned int& ruiValue, Stream& rqStream)
{
    int iMBytes = 0;
    unsigned char ucByte;
    for (/**/; /**/; /**/)
    {
        if ( iMBytes == 5 )
        {
            // the previous byte had to be the last one
            return false;
        }
        if ( !rqStream.Read(ucByte) )
            return false;
        iMBytes++;
        if ( (ucByte & 128) == 0 )
            break;
    }

    ruiValue = 0;
    const char* pcByte = rqStream.GetBytesUpPosition() - 1;
    int iOffset = 0;
    for (int i = 0; i < iMBytes; i++, pcByte--, iOffset += 7)
        ruiValue |= (*pcByte & 127) << iOffset;

    return true;
}
//------------------------------------------------------------------------------------------------------------------



