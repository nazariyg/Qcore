#include "QFormatCin.h"
#include "QExptWarnBase.mcr"
#include "QFormatBase.mcr"
#include "QProgressProbe.h"
#include "QStorage.h"
using namespace Q;

// support for color conversions
const int FormatCin::ms_iRefBlack(0);
const int FormatCin::ms_iRefWhite(684);
const float FormatCin::ms_fDispGamma(1.7f);

//------------------------------------------------------------------------------------------------------------------
FormatCin::FormatCin (const RasterImagePtr& rspqImage)
    :
    m_spqImageSaveTo(rspqImage),
    m_iWidth(rspqImage->GetWidth()),
    m_iHeight(rspqImage->GetHeight())
{
    assert( rspqImage->GetWidth() != 0 && rspqImage->GetHeight() != 0 );
    assert( rspqImage->GetColorModel() == RasterImage::CMT_RGB );
    assert( !rspqImage->HasVisibilityMask() );
}
//------------------------------------------------------------------------------------------------------------------
bool FormatCin::SaveImage (const char* acFilename)
{
    char cZero = 0;

    Stream qSaveStream(acFilename,false);

    // File information.
    unsigned int uiSignature = 2150260695;
    qSaveStream.Write(uiSignature);

    unsigned int uiImageDataOffset = 2048;
    qSaveStream.Write(uiImageDataOffset);

    unsigned int uiGenericSHLength = 1024;
    unsigned int uiIndustrySpecificSHLength = 1024;
    qSaveStream.Write(uiGenericSHLength);
    qSaveStream.Write(uiIndustrySpecificSHLength);

    unsigned int uiVLSLength = 0;
    qSaveStream.Write(uiVLSLength);

    unsigned int uiFileSize = 2048 + m_spqImageSaveTo->GetArea()*4;
    qSaveStream.Write(uiFileSize);

    qSaveStream.Write(4,"V4.5");
    for (int i = 0; i < 4; i++)
        qSaveStream.Write(cZero);

    for (int i = 0; i < 160; i++)
        qSaveStream.Write(cZero);

    // Image information.
    unsigned char ucImageOrientation = 0;
    qSaveStream.Write(ucImageOrientation);

    unsigned char ucNumberOfChannels = 3;
    qSaveStream.Write(ucNumberOfChannels);

    for (int i = 0; i < 2; i++)
        qSaveStream.Write(cZero);

    unsigned char ucDesignator0 = 0;
    unsigned char ucDesignator1;
    unsigned char ucBitsPerSample = 10;
    unsigned int uiPixelsPerLine = m_iWidth;
    unsigned int uiLinesPerImage = m_iHeight;
    float fMinimumDataValue = 0.0f;
    float fMinimumQuantityRepresented = 0.0f;
    float fMaximumDataValue = 1023.0f;
    float fMaximumQuantityRepresented = 1023.0f;
    for (int i = 0; i < 3; i++)
    {
        ucDesignator1 = i + 1;
        qSaveStream.Write(ucDesignator0);
        qSaveStream.Write(ucDesignator1);

        qSaveStream.Write(ucBitsPerSample);

        qSaveStream.Write(cZero);

        qSaveStream.Write(uiPixelsPerLine);
        qSaveStream.Write(uiLinesPerImage);

        qSaveStream.Write(fMinimumDataValue);
        qSaveStream.Write(fMinimumQuantityRepresented);
        qSaveStream.Write(fMaximumDataValue);
        qSaveStream.Write(fMaximumQuantityRepresented);
    }
    for (int i0 = 0; i0 < 5; i0++)
    {
        for (int i1 = 0; i1 < 28; i1++)
            qSaveStream.Write(cZero);
    }

    for (int i = 0; i < 260; i++)
        qSaveStream.Write(cZero);

    // Image data format information.
    unsigned char ucDataInterleave = 0;
    qSaveStream.Write(ucDataInterleave);

    unsigned char ucPacking = 5;
    qSaveStream.Write(ucPacking);

    unsigned char ucSign = 0;
    qSaveStream.Write(ucSign);

    unsigned char ucImageSence = 0;
    qSaveStream.Write(ucImageSence);

    for (int i = 0; i < 28; i++)
        qSaveStream.Write(cZero);

    // image origination information
    for (int i = 0; i < 312; i++)
        qSaveStream.Write(cZero);

    // film/frame information
    for (int i = 0; i < 1024; i++)
        qSaveStream.Write(cZero);

    // for a progress bar
    ProgressProbe qProgress(this);
    qProgress.SetAdvance(1.0f/m_iHeight);

    // write the image
    int iRefDiff = ms_iRefBlack - ms_iRefWhite;
    float fRatio0 = 0.002f/0.6f;
    float fRatio1 = ms_fDispGamma/1.7f;
    float fGain = 1.0f/(1.0f - Mathf::Pow(Mathf::Pow(10.0f,fRatio0*iRefDiff),fRatio1));
    float fOffset = fGain - 1.0f;
    float fVar0 = fRatio0*fRatio1*Mathf::Log(10.0f);
    float fVar1 = fVar0*ms_iRefWhite;
    float fVar2 = 1.0f/fVar0;
    float fInvGain = 1.0f/fGain;
    ColorRgb qColorRgb;
    int iValue, iZeroValue = 0;
    for (int iY = 0; iY < m_iHeight; iY++)
    {
        for (int iX = 0; iX < m_iWidth; iX++)
        {
            qColorRgb = m_spqImageSaveTo->GetPixelRgb(iX,iY);
            for (int i = 0; i < 3; i++)
            {
                iValue = Mathf::RoundToInt((fVar1+Mathf::Log((qColorRgb[i]+fOffset)*fInvGain))*fVar2);
                if ( iValue < 0 )
                    iValue = 0;
                else if ( iValue > 1023 )
                    iValue = 1023;

                qSaveStream.WriteBits(iValue,10);
            }

            qSaveStream.WriteBits(iZeroValue,2);
        }

        qProgress.MakeAdvance();
    }

    // flush to the file
    CONDITIONAL_THROW_EXCEPTION(qSaveStream.Finalize(),ET_CANNOT_ACCESS_FILE);

    return true;
}
//------------------------------------------------------------------------------------------------------------------
FormatCin::FormatCin (const char* acFilename, bool& rbSuccess)
    :
    FormatBase(acFilename)
{
    bool bSuccess;

    // signature
    int iFileSize;
    C_CONDITIONAL_THROW_EXCEPTION(Storage::GetFileSize(iFileSize,m_qFilename),ET_CANNOT_ACCESS_FILE);
    C_CONDITIONAL_THROW_EXCEPTION_WITHOUT_ASSERT_FALSE(( iFileSize >= 4 ),ET_FORMAT_IDENTIFIER_MISMATCH);
    Stream qSignatureStream(m_eStreaming,m_qFilename,0,4,false,bSuccess);
    C_PROCESS_STREAM_SUCCESS_FOR_LOADING(bSuccess);
    unsigned int uiSignature;
    qSignatureStream.Read(uiSignature);
    C_CONDITIONAL_THROW_EXCEPTION_WITHOUT_ASSERT_FALSE(( uiSignature == 2150260695 || uiSignature == 3613338240 ),
        ET_FORMAT_IDENTIFIER_MISMATCH);
    qSignatureStream.Finalize();

    bool bLittleEndian = ( uiSignature == 3613338240 );

    // File information.
    Stream qFileInfoStream(m_eStreaming,m_qFilename,4,12,bLittleEndian,bSuccess);
    C_PROCESS_STREAM_SUCCESS_FOR_LOADING(bSuccess);

    unsigned int uiImageDataOffset;
    qFileInfoStream.Read(uiImageDataOffset);
    m_iImageDataOffset = uiImageDataOffset;

    unsigned int uiGenericSHLength, uiIndustrySpecificSHLength;
    qFileInfoStream.Read(uiGenericSHLength);
    qFileInfoStream.Read(uiIndustrySpecificSHLength);
    C_CONDITIONAL_THROW_EXCEPTION(( uiGenericSHLength == 1024 && uiIndustrySpecificSHLength == 1024 ),
        ET_FORMAT_MISMATCH);

    qFileInfoStream.Finalize();

    // Image information.
    Stream qImageInfoStream(m_eStreaming,m_qFilename,192,88,bLittleEndian,bSuccess);
    C_PROCESS_STREAM_SUCCESS_FOR_LOADING(bSuccess);

    unsigned char ucImageOrientation;
    qImageInfoStream.Read(ucImageOrientation);
    C_CONDITIONAL_THROW_EXCEPTION(( ucImageOrientation == 0 ),ET_UNSUPPORTED_FILE);

    unsigned char ucNumberOfChannels;
    qImageInfoStream.Read(ucNumberOfChannels);
    C_CONDITIONAL_THROW_EXCEPTION(( ucNumberOfChannels == 3 ),ET_UNSUPPORTED_FILE);

    qImageInfoStream.OffsetBytePosition(2);

    unsigned int uiPixelsPerLine, uiLinesPerImage;
    qImageInfoStream.PushPositionState();
    qImageInfoStream.SetBytePosition(8);
    qImageInfoStream.Read(uiPixelsPerLine);
    qImageInfoStream.Read(uiLinesPerImage);
    qImageInfoStream.PopPositionState();
    C_CONDITIONAL_THROW_EXCEPTION(( uiPixelsPerLine != 0 && uiLinesPerImage != 0 ),ET_FORMAT_MISMATCH);
    m_iWidth = uiPixelsPerLine;
    m_iHeight = uiLinesPerImage;

    unsigned char ucDesignator0, ucDesignator1, ucBitsPerSample;
    for (int i = 0; i < 3; i++)
    {
        qImageInfoStream.Read(ucDesignator0);
        qImageInfoStream.Read(ucDesignator1);
        C_CONDITIONAL_THROW_EXCEPTION(( ucDesignator0 == 0 && ucDesignator1 == i + 1 ),ET_UNSUPPORTED_FILE);

        qImageInfoStream.Read(ucBitsPerSample);
        C_CONDITIONAL_THROW_EXCEPTION(( ucBitsPerSample == 10 ),ET_UNSUPPORTED_FILE);

        qImageInfoStream.OffsetBytePosition(1);

        qImageInfoStream.Read(uiPixelsPerLine);
        qImageInfoStream.Read(uiLinesPerImage);
        C_CONDITIONAL_THROW_EXCEPTION(( uiPixelsPerLine == m_iWidth && uiLinesPerImage == m_iHeight ),
            ET_UNSUPPORTED_FILE);

        qImageInfoStream.OffsetBytePosition(16);
    }

    qImageInfoStream.Finalize();

    // Image data format information.
    Stream qIDFInfoStream(m_eStreaming,m_qFilename,680,4,bLittleEndian,bSuccess);
    C_PROCESS_STREAM_SUCCESS_FOR_LOADING(bSuccess);

    unsigned char ucDataInterleave;
    qIDFInfoStream.Read(ucDataInterleave);
    C_CONDITIONAL_THROW_EXCEPTION(( ucDataInterleave == 0 ),ET_UNSUPPORTED_FILE);

    unsigned char ucPacking;
    qIDFInfoStream.Read(ucPacking);
    C_CONDITIONAL_THROW_EXCEPTION(( ucPacking == 5 ),ET_UNSUPPORTED_FILE);

    unsigned char ucSign;
    qIDFInfoStream.Read(ucSign);
    C_CONDITIONAL_THROW_EXCEPTION(( ucSign == 0 ),ET_UNSUPPORTED_FILE);

    unsigned char ucImageSence;
    qIDFInfoStream.Read(ucImageSence);
    C_CONDITIONAL_THROW_EXCEPTION(( ucImageSence == 0 ),ET_UNSUPPORTED_FILE);

    qIDFInfoStream.Finalize();

    rbSuccess = true;
}
//------------------------------------------------------------------------------------------------------------------
bool FormatCin::LoadImage (RasterImagePtr& rspqImage)
{
    bool bSuccess;

    // allocate the output image
    rspqImage = new RasterImage(RasterImage::IT_RGB_16_16_16,m_iWidth,m_iHeight);

    // generate the look-up table
    int iRefDiff = ms_iRefBlack - ms_iRefWhite;
    float fRatio0 = 0.002f/0.6f;
    float fRatio1 = ms_fDispGamma/1.7f;
    float fGain = 65535.0f/(1.0f - Mathf::Pow(Mathf::Pow(10.0f,fRatio0*iRefDiff),fRatio1));
    float fOffset = fGain - 65535.0f;
    unsigned short ausLookUpTable[1024];
    int iOut;
    for (int i = 0; i < 1024; i++)
    {
        iOut = Mathf::RoundToInt(Mathf::Pow(10.0f,Mathf::Pow((i-ms_iRefWhite)*fRatio0,fRatio1))*fGain - fOffset);
        if ( iOut < 0 )
            iOut = 0;
        else if ( iOut > 65535 )
            iOut = 65535;

        ausLookUpTable[i] = iOut;
    }

    // for a progress bar
    ProgressProbe qProgress(this);
    qProgress.SetAdvance(1.0f/m_iHeight);

    // read the image
    Stream qImageDataStream(m_eStreaming,m_qFilename,m_iImageDataOffset,rspqImage->GetArea()*4,true,bSuccess);
    PROCESS_STREAM_SUCCESS_FOR_LOADING(bSuccess);
    unsigned short* pusComp;
    int iValue;
    for (int iY = 0; iY < m_iHeight; iY++)
    {
        pusComp = (unsigned short*)rspqImage->GetRow(iY);
        for (int iX = 0; iX < m_iWidth; iX++)
        {
            for (int i = 0; i < 3; i++)
            {
                qImageDataStream.ReadUBits(iValue,10);
                *pusComp++ = ausLookUpTable[iValue];
            }

            qImageDataStream.AlignBytes();
        }

        qProgress.MakeAdvance();
    }
    qImageDataStream.Finalize();

    return true;
}
//------------------------------------------------------------------------------------------------------------------



