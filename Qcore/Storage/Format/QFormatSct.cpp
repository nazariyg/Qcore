#include "QFormatSct.h"
#include "QExptWarnBase.mcr"
#include "QFormatBase.mcr"
#include "QProgressProbe.h"
#include "QStorage.h"
#include "QRasterImageTransformation.h"
using namespace Q;

//------------------------------------------------------------------------------------------------------------------
FormatSct::FormatSct (const RasterImagePtr& rspqImage)
    :
    m_spqImageSaveTo(rspqImage),
    m_iWidth(rspqImage->GetWidth()),
    m_iHeight(rspqImage->GetHeight()),
    m_iHorizontalDpi(72),
    m_iVerticalDpi(72),
    m_eOrientation(OT_TOP_LEFT)
{
    assert( rspqImage->GetWidth() != 0 && rspqImage->GetHeight() != 0 );
    assert( rspqImage->GetColorModel() == RasterImage::CMT_GRAYSCALE
        ||  rspqImage->GetColorModel() == RasterImage::CMT_RGB
        ||  rspqImage->GetColorModel() == RasterImage::CMT_CMYK );
    assert( !rspqImage->HasVisibilityMask() );
}
//------------------------------------------------------------------------------------------------------------------
bool FormatSct::SaveImage (const char* acFilename)
{
    // internal color model
    switch ( m_spqImageSaveTo->GetColorModel() )
    {
    case RasterImage::CMT_GRAYSCALE:
        m_eColorModel = CMT_GRAYSCALE;
        break;
    case RasterImage::CMT_RGB:
        m_eColorModel = CMT_RGB;
        break;
    case RasterImage::CMT_CMYK:
        m_eColorModel = CMT_CMYK;
        break;
    }

    Stream qSaveStream(acFilename,true);
    char cZero = 0;

    // Control Block
    WriteString(m_qNameComment,qSaveStream);
    qSaveStream.Write(2,"CT");
    for (int i = 0; i < 942; i++)
        qSaveStream.Write(cZero);

    // Parameters Block.
    unsigned char ucUnitsOfMeasurement = 1;  // inches
    qSaveStream.Write(ucUnitsOfMeasurement);

    unsigned char ucSeparationsBitMask;
    switch ( m_eColorModel )
    {
    case CMT_GRAYSCALE:
        m_ucNumColorSeparations = 1;
        ucSeparationsBitMask = 8;
        break;
    case CMT_RGB:
        m_ucNumColorSeparations = 3;
        ucSeparationsBitMask = 7;
        break;
    case CMT_CMYK:
        m_ucNumColorSeparations = 4;
        ucSeparationsBitMask = 15;
        break;
    }
    qSaveStream.Write(m_ucNumColorSeparations);
    qSaveStream.Write(cZero);
    qSaveStream.Write(ucSeparationsBitMask);

    float fWidthInUnits = ((float)m_iWidth)/m_iHorizontalDpi;
    float fHeightInUnits = ((float)m_iHeight)/m_iVerticalDpi;
    WriteFloat(fHeightInUnits,qSaveStream);
    WriteFloat(fWidthInUnits,qSaveStream);

    WriteLongInteger(m_iHeight,qSaveStream);
    WriteLongInteger(m_iWidth,qSaveStream);

    unsigned char ucScanDirection;
    switch ( m_eOrientation )
    {
    case OT_TOP_LEFT:
        ucScanDirection = 0;
        break;
    case OT_BOTTOM_LEFT:
        ucScanDirection = 1;
        break;
    case OT_TOP_RIGHT:
        ucScanDirection = 2;
        break;
    case OT_BOTTOM_RIGHT:
        ucScanDirection = 3;
        break;
    case OT_LEFT_BOTTOM:
        ucScanDirection = 4;
        break;
    case OT_RIGHT_BOTTOM:
        ucScanDirection = 5;
        break;
    case OT_LEFT_TOP:
        ucScanDirection = 6;
        break;
    case OT_RIGHT_TOP:
        ucScanDirection = 7;
        break;
    }
    qSaveStream.Write(ucScanDirection);

    for (int i = 0; i < 967; i++)
        qSaveStream.Write(cZero);

    // for a progress bar
    ProgressProbe qProgress(this);
    qProgress.SetAdvance(1.0f/m_iHeight);

    bool bPadding = ( (m_iWidth & 1) != 0 );

    // write the image
    unsigned char ucValue;
    if ( m_eColorModel == CMT_GRAYSCALE )
    {
        float fIntensity;
        for (int iY = 0; iY < m_iHeight; iY++)
        {
            for (int iX = 0; iX < m_iWidth; iX++)
            {
                fIntensity = m_spqImageSaveTo->GetPixelIntensity(iX,iY);
                ucValue = Mathf::RoundToInt(fIntensity*255);
                qSaveStream.Write(ucValue);
            }

            if ( bPadding )
                qSaveStream.Write(cZero);

            qProgress.MakeAdvance();
        }
    }
    else if ( m_eColorModel == CMT_RGB )
    {
        StillArray<ColorRgb> qRowColors(m_iWidth);
        for (int iY = 0; iY < m_iHeight; iY++)
        {
            for (int iX = 0; iX < m_iWidth; iX++)
                qRowColors[iX] = m_spqImageSaveTo->GetPixelRgb(iX,iY);

            for (int i = 0; i < 3; i++)
            {
                for (int iX = 0; iX < m_iWidth; iX++)
                {
                    ucValue = Mathf::RoundToInt(qRowColors[iX][i]*255);
                    qSaveStream.Write(ucValue);
                }

                if ( bPadding )
                    qSaveStream.Write(cZero);
            }

            qProgress.MakeAdvance();
        }
    }
    else  // m_eColorModel = CMT_CMYK
    {
        StillArray<ColorCmyk> qRowColors(m_iWidth);
        for (int iY = 0; iY < m_iHeight; iY++)
        {
            for (int iX = 0; iX < m_iWidth; iX++)
                qRowColors[iX] = m_spqImageSaveTo->GetPixelCmyk(iX,iY);

            for (int i = 0; i < 4; i++)
            {
                for (int iX = 0; iX < m_iWidth; iX++)
                {
                    ucValue = 255 - Mathf::RoundToInt(qRowColors[iX][i]*255);
                    qSaveStream.Write(ucValue);
                }

                if ( bPadding )
                    qSaveStream.Write(cZero);
            }

            qProgress.MakeAdvance();
        }
    }

    // flush to the file
    CONDITIONAL_THROW_EXCEPTION(qSaveStream.Finalize(),ET_CANNOT_ACCESS_FILE);

    return true;
}
//------------------------------------------------------------------------------------------------------------------
FormatSct::FormatSct (const char* acFilename, bool& rbSuccess)
    :
    FormatBase(acFilename)
{
    bool bSuccess;

    // name and signature from Control Block
    int iFileSize;
    C_CONDITIONAL_THROW_EXCEPTION(Storage::GetFileSize(iFileSize,m_qFilename),ET_CANNOT_ACCESS_FILE);
    C_CONDITIONAL_THROW_EXCEPTION_WITHOUT_ASSERT_FALSE(( iFileSize > 82 ),ET_FORMAT_IDENTIFIER_MISMATCH);
    char acFileType[2];
    Stream qNCAndFTStream(m_eStreaming,m_qFilename,0,82,true,bSuccess);
    C_PROCESS_STREAM_SUCCESS_FOR_LOADING(bSuccess);
    C_RETURN_ON_FAIL(ReadString(m_qNameComment,qNCAndFTStream));
    qNCAndFTStream.Read(2,acFileType);
    C_CONDITIONAL_THROW_EXCEPTION_WITHOUT_ASSERT_FALSE(( MemCmp(acFileType,"CT",2) == 0 ),
        ET_FORMAT_IDENTIFIER_MISMATCH);
    qNCAndFTStream.Finalize();

    // Parameters Block.
    Stream qPBStream(m_eStreaming,m_qFilename,1024,57,true,bSuccess);
    C_PROCESS_STREAM_SUCCESS_FOR_LOADING(bSuccess);

    MeasurementUnitType eMeasurementUnit;
    unsigned char ucUnitsOfMeasurement;
    qPBStream.Read(ucUnitsOfMeasurement);
    switch ( ucUnitsOfMeasurement )
    {
    case 0:
        eMeasurementUnit = MUT_MILLIMETER;
        break;
    case 1:
        eMeasurementUnit = MUT_INCH;
        break;
    default:
        C_THROW_EXCEPTION(ET_FORMAT_MISMATCH);
    }

    qPBStream.Read(m_ucNumColorSeparations);

    qPBStream.OffsetBytePosition(1);
    unsigned char ucSeparationsBitMask;
    qPBStream.Read(ucSeparationsBitMask);

    C_CONDITIONAL_THROW_EXCEPTION((
         (m_ucNumColorSeparations == 1 && ucSeparationsBitMask == 8)
    ||   (m_ucNumColorSeparations == 3 && ucSeparationsBitMask == 7)
    ||   (m_ucNumColorSeparations == 4 && ucSeparationsBitMask == 15) ),ET_FORMAT_MISMATCH);

    switch ( m_ucNumColorSeparations )
    {
    case 1:
        m_eColorModel = CMT_GRAYSCALE;
        break;
    case 3:
        m_eColorModel = CMT_RGB;
        break;
    case 4:
        m_eColorModel = CMT_CMYK;
        break;
    }

    float fWidthInUnits, fHeightInUnits;
    C_RETURN_ON_FAIL(ReadFloat(fHeightInUnits,qPBStream));
    C_RETURN_ON_FAIL(ReadFloat(fWidthInUnits,qPBStream));

    C_RETURN_ON_FAIL(ReadLongInteger(m_iHeight,qPBStream));
    C_RETURN_ON_FAIL(ReadLongInteger(m_iWidth,qPBStream));
    C_CONDITIONAL_THROW_EXCEPTION(( m_iWidth > 0 && m_iHeight > 0 ),ET_FORMAT_MISMATCH);

    float fWidthInInches, fHeightInInches;
    if ( eMeasurementUnit == MUT_MILLIMETER )
    {
        fWidthInInches = fWidthInUnits*25.4f;
        fHeightInInches = fHeightInUnits*25.4f;
    }
    else  // eMeasurementUnit = MUT_INCH
    {
        fWidthInInches = fWidthInUnits;
        fHeightInInches = fHeightInUnits;
    }
    m_iHorizontalDpi = Mathf::RoundToInt(((float)m_iWidth)/fWidthInInches);
    m_iVerticalDpi = Mathf::RoundToInt(((float)m_iHeight)/fHeightInInches);

    unsigned char ucScanDirection;
    qPBStream.Read(ucScanDirection);
    switch ( ucScanDirection )
    {
    case 0:
        m_eOrientation = OT_TOP_LEFT;
        break;
    case 1:
        m_eOrientation = OT_BOTTOM_LEFT;
        break;
    case 2:
        m_eOrientation = OT_TOP_RIGHT;
        break;
    case 3:
        m_eOrientation = OT_BOTTOM_RIGHT;
        break;
    case 4:
        m_eOrientation = OT_LEFT_BOTTOM;
        break;
    case 5:
        m_eOrientation = OT_RIGHT_BOTTOM;
        break;
    case 6:
        m_eOrientation = OT_LEFT_TOP;
        break;
    case 7:
        m_eOrientation = OT_RIGHT_TOP;
        break;
    default:  // a concession
        m_eOrientation = OT_TOP_LEFT;
        break;
    }

    qPBStream.Finalize();

    rbSuccess = true;
}
//------------------------------------------------------------------------------------------------------------------
bool FormatSct::LoadImage (RasterImagePtr& rspqImage)
{
    bool bSuccess;

    // expected size of image data
    int iSeparationRowSize = m_iWidth;
    bool bPadding = false;
    if ( (iSeparationRowSize & 1) != 0 )
    {
        iSeparationRowSize++;
        bPadding = true;
    }
    int iImageDataSize = m_ucNumColorSeparations*iSeparationRowSize*m_iHeight;

    // allocate the output image
    if ( m_eColorModel == CMT_GRAYSCALE )
        rspqImage = new RasterImage(RasterImage::IT_GRAYSCALE_8,m_iWidth,m_iHeight);
    else if ( m_eColorModel == CMT_RGB )
        rspqImage = new RasterImage(RasterImage::IT_RGB_8_8_8,m_iWidth,m_iHeight);
    else  // m_eColorModel = CMT_CMYK
        rspqImage = new RasterImage(RasterImage::IT_CMYK_8_8_8_8,m_iWidth,m_iHeight);

    // for a progress bar
    ProgressProbe qProgress(this);
    qProgress.SetAdvance(1.0f/m_iHeight);

    // read the image
    Stream qImageStream(m_eStreaming,m_qFilename,2048,iImageDataSize,true,bSuccess);
    PROCESS_STREAM_SUCCESS_FOR_LOADING(bSuccess);
    unsigned char ucValue;
    unsigned char* pucRow = (unsigned char*)rspqImage->GetImage();
    for (int iY = 0; iY < m_iHeight; iY++)
    {
        for (int i = 0; i < m_ucNumColorSeparations; i++)
        {
            for (int iX = 0; iX < m_iWidth; iX++)
            {
                qImageStream.Read(ucValue);
                if ( m_eColorModel == CMT_CMYK )
                    ucValue = 255 - ucValue;
                *(pucRow + iX*rspqImage->GetBytesPerPixel() + i) = ucValue;
            }

            if ( bPadding )
                qImageStream.OffsetBytePosition(1);
        }

        pucRow += rspqImage->GetRowStride();

        qProgress.MakeAdvance();
    }
    qImageStream.Finalize();

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
void FormatSct::WriteString (const String& rqString, Stream& rqStream)
{
    assert( rqString.GetLength() <= 80 );
    rqStream.Write(rqString.GetLength(),rqString);
    int iRem = 80 - rqString.GetLength();
    char cSpace = ' ';
    for (int i = 0; i < iRem; i++)
        rqStream.Write(cSpace);
}
//------------------------------------------------------------------------------------------------------------------
void FormatSct::WriteInteger (int iValue, Stream& rqStream)
{
    char acNumber[64];
    sprintf(acNumber,"%+06i",iValue);
    assert( StrLen(acNumber) == 6 );
    rqStream.Write(6,acNumber);
}
//------------------------------------------------------------------------------------------------------------------
void FormatSct::WriteLongInteger (int iValue, Stream& rqStream)
{
    char acNumber[64];
    sprintf(acNumber,"%+012i",iValue);
    assert( StrLen(acNumber) == 12 );
    rqStream.Write(12,acNumber);
}
//------------------------------------------------------------------------------------------------------------------
void FormatSct::WriteFloat (float fValue, Stream& rqStream)
{
    char acNumber[64];
    sprintf(acNumber,"%+#.7E",(double)fValue);
    assert( StrLen(acNumber) == 15 && acNumber[12] == '0' );
    acNumber[2] = acNumber[1];
    acNumber[1] = '.';
    int iSExp = atoi(acNumber+11);
    assert( iSExp < 99 );
    iSExp++;
    char acSExp[8];
    sprintf(acSExp,"%+03i",iSExp);
    assert( StrLen(acSExp) == 3 );
    for (int i = 0; i < 11; i++)
        rqStream.Write(acNumber[i]);
    for (int i = 0; i < 3; i++)
        rqStream.Write(acSExp[i]);
}
//------------------------------------------------------------------------------------------------------------------
bool FormatSct::ReadString (String& rqString, Stream& rqStream)
{
    rqString.SetLength(80);
    if ( !rqStream.Read(80,rqString) )
        return false;

    // free it from trailing spaces if they are
    rqString.Trim();

    return true;
}
//------------------------------------------------------------------------------------------------------------------
bool FormatSct::ReadInteger (int& riValue, Stream& rqStream)
{
    unsigned char aucStr[6];
    if ( !rqStream.Read(6,aucStr) )
        return false;

    // check for validity
    if ( !(aucStr[0] == '-' || aucStr[0] == '+') )
        return false;
    for (int i = 1; i < 6; i++)
    {
        if ( !('0' <= aucStr[i] && aucStr[i] <= '9') )
            return false;
    }

    riValue = atoi((const char*)aucStr);
    return true;
}
//------------------------------------------------------------------------------------------------------------------
bool FormatSct::ReadLongInteger (int& riValue, Stream& rqStream)
{
    unsigned char aucStr[12];
    if ( !rqStream.Read(12,aucStr) )
        return false;

    // check for validity
    if ( !(aucStr[0] == '-' || aucStr[0] == '+') )
        return false;
    for (int i = 1; i < 12; i++)
    {
        if ( !('0' <= aucStr[i] && aucStr[i] <= '9') )
            return false;
    }

    riValue = atoi((const char*)aucStr);
    return true;
}
//------------------------------------------------------------------------------------------------------------------
bool FormatSct::ReadFloat (float& rfValue, Stream& rqStream)
{
    unsigned char aucStr[14];
    if ( !rqStream.Read(14,aucStr) )
        return false;

    // check for validity
    if ( !(aucStr[0] == '-' || aucStr[0] == '+') )
        return false;
    if ( aucStr[1] != '.' )
        return false;
    for (int i = 2; i < 10; i++)
    {
        if ( !('0' <= aucStr[i] && aucStr[i] <= '9') )
            return false;
    }
    if ( !(aucStr[10] == 'E' || aucStr[10] == 'e') )  // slight concession about "e"
        return false;
    if ( !(aucStr[11] == '-' || aucStr[11] == '+') )
        return false;
    for (int i = 12; i < 14; i++)
    {
        if ( !('0' <= aucStr[i] && aucStr[i] <= '9') )
            return false;
    }

    rfValue = (float)atof((const char*)aucStr);
    return true;
}
//------------------------------------------------------------------------------------------------------------------



