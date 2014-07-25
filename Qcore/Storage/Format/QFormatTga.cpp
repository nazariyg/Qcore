#include "QFormatTga.h"
#include "QExptWarnBase.mcr"
#include "QFormatBase.mcr"
#include "QProgressProbe.h"
#include "QStorage.h"
#include "QRasterImageTransformation.h"
using namespace Q;

//------------------------------------------------------------------------------------------------------------------
FormatTga::FormatTga (const RasterImagePtr& rspqImage, int iBitsPerPixel)
    :
    m_spqImageSaveTo(rspqImage),
    m_iWidth(rspqImage->GetWidth()),
    m_iHeight(rspqImage->GetHeight()),
    m_eCompression(CT_NONE)
{
    assert( rspqImage->GetWidth() != 0 && rspqImage->GetHeight() != 0 );
    assert( rspqImage->GetWidth() <= 65535 && rspqImage->GetHeight() <= 65535 );
    assert( rspqImage->GetColorModel() == RasterImage::CMT_PALETTED
        ||  rspqImage->GetColorModel() == RasterImage::CMT_GRAYSCALE
        ||  rspqImage->GetColorModel() == RasterImage::CMT_RGB
        ||  rspqImage->GetColorModel() == RasterImage::CMT_RGBA );
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
        iBitsPerPixel = 8;

    m_iBitsPerPixel = iBitsPerPixel;
}
//------------------------------------------------------------------------------------------------------------------
bool FormatTga::SaveImage (const char* acFilename)
{
    assert( m_iBitsPerPixel == 8
        ||  m_iBitsPerPixel == 16
        ||  m_iBitsPerPixel == 24
        ||  m_iBitsPerPixel == 32 );
    assert( ( m_bPaletted || m_spqImageSaveTo->GetColorModel() == RasterImage::CMT_GRAYSCALE ) ==
        ( m_iBitsPerPixel == 8 ) );
    assert( ( m_spqImageSaveTo->GetColorModel() == RasterImage::CMT_RGBA ) == ( m_iBitsPerPixel == 32 ) );

    // do autodetect
    switch ( m_spqImageSaveTo->GetColorModel() )
    {
    case RasterImage::CMT_PALETTED:
        m_eImage = IT_PALETTED;
        break;
    case RasterImage::CMT_GRAYSCALE:
        m_eImage = IT_GRAYSCALE;
        break;
    case RasterImage::CMT_RGB:
        m_eImage = IT_CHANNELED;
        break;
    case RasterImage::CMT_RGBA:
        m_eImage = IT_CHANNELED;
        break;
    }

    Stream qSaveStream(acFilename,true);

    unsigned char ucIdLength = 0;
    qSaveStream.Write(ucIdLength);

    char cColorMapType = ( !m_bPaletted ? 0 : 1 );
    qSaveStream.Write(cColorMapType);

    char cImageType;
    switch ( m_eImage )
    {
    case IT_PALETTED:
        cImageType = ( m_eCompression == CT_NONE ? 1 : 9 );
        break;
    case IT_CHANNELED:
        cImageType = ( m_eCompression == CT_NONE ? 2 : 10 );
        break;
    case IT_GRAYSCALE:
        cImageType = ( m_eCompression == CT_NONE ? 3 : 11 );
        break;
    }
    qSaveStream.Write(cImageType);

    unsigned short usFirstEntryIndex = 0;
    qSaveStream.Write(usFirstEntryIndex);

    unsigned short usPaletteSize = ( m_bPaletted ? m_iPaletteSize : 0 );
    qSaveStream.Write(usPaletteSize);

    unsigned char ucBitsPerPaletteColor = ( m_bPaletted ? 24 : 0 );
    qSaveStream.Write(ucBitsPerPaletteColor);

    // ignore information about position of the image's origin
    int iValue = 0;
    qSaveStream.Write(iValue);

    unsigned short usWidth = m_iWidth;
    unsigned short usHeight = m_iHeight;
    qSaveStream.Write(usWidth);
    qSaveStream.Write(usHeight);

    unsigned char ucBitsPerPixel = m_iBitsPerPixel;
    qSaveStream.Write(ucBitsPerPixel);

    // orientation of the image is top-to-bottom
    qSaveStream.WriteBits(2,4);

    if ( m_iBitsPerPixel != 32 )
        qSaveStream.WriteBits(0,4);
    else
        qSaveStream.WriteBits(8,4);

    // the palette
    if ( m_bPaletted )
    {
        for (int i = 0; i < m_iPaletteSize; i++)
            qSaveStream.Write(3,m_spqImageSaveTo->GetPaletteColor(i).Components);
    }

    // for a progress bar
    ProgressProbe qProgress(this);
    qProgress.SetAdvance(1.0f/m_iHeight);

    // actual image
    if ( m_bPaletted )
    {
        unsigned char ucIndex;
        if ( cImageType == 1 )
        {
            for (int iY = 0; iY < m_iHeight; iY++)
            {
                for (int iX = 0; iX < m_iWidth; iX++)
                {
                    ucIndex = m_spqImageSaveTo->GetPixelIndex(iX,iY);
                    qSaveStream.WriteBits(ucIndex,m_iBitsPerPixel);
                }

                qProgress.MakeAdvance();
            }
        }
        else  // cImageType = 9
        {
            int iXS, iRepeat, iRPQ, iSum, iTop, iLastRPBytePos;
            bool bLastWasRP;
            for (int iY = 0; iY < m_iHeight; iY++)
            {
                iXS = 0;
                bLastWasRP = false;
                while ( iXS < m_iWidth )
                {
                    iRepeat = 1;
                    ucIndex = m_spqImageSaveTo->GetPixelIndex(iXS,iY);
                    for (int iX = iXS+1; iX < m_iWidth; iX++)
                    {
                        if ( m_spqImageSaveTo->GetPixelIndex(iX,iY) != ucIndex )
                            break;
                        if ( ++iRepeat == 128 )
                            break;
                    }

                    if ( iRepeat > 2 )
                    {
                        // that's where run-length packets begin to be economic
                        qSaveStream.WriteBits(1,1);
                        qSaveStream.WriteBits(iRepeat-1,7);

                        qSaveStream.Write(ucIndex);

                        iXS += iRepeat;

                        bLastWasRP = false;
                    }
                    else
                    {
                        // raw packet
                        if ( !bLastWasRP )
                        {
                            iRPQ = 0;
                            iLastRPBytePos = qSaveStream.GetBytePosition();
                            qSaveStream.WriteBits(0,1);
                            qSaveStream.WriteBits(0,7);
                        }

                        iSum = iRPQ + iRepeat;
                        qSaveStream.PushPositionState();
                        qSaveStream.SetBytePosition(iLastRPBytePos);
                        qSaveStream.SetBitRatio(1);
                        if ( iSum <= 128 )
                        {
                            qSaveStream.WriteBits(iSum-1,7);
                            qSaveStream.PopPositionState();
                            for (int i = 0; i < iRepeat; i++)
                            {
                                ucIndex = m_spqImageSaveTo->GetPixelIndex(iXS+i,iY);
                                qSaveStream.Write(ucIndex);
                            }

                            iXS += iRepeat;
                        }
                        else
                        {
                            qSaveStream.WriteBits(127,7);
                            qSaveStream.PopPositionState();
                            iTop = 128 - iRPQ;
                            for (int i = 0; i < iTop; i++)
                            {
                                ucIndex = m_spqImageSaveTo->GetPixelIndex(iXS+i,iY);
                                qSaveStream.Write(ucIndex);
                            }

                            iXS += iTop;
                        }
                        if ( iSum < 128 )
                        {
                            iRPQ = iSum;
                            bLastWasRP = true;
                        }
                        else
                        {
                            bLastWasRP = false;
                        }
                    }
                }

                qProgress.MakeAdvance();
            }
        }
    }
    else
    {
        if ( cImageType == 2 )
        {
            if ( m_iBitsPerPixel == 16 )
            {
                ColorRgb qColor;
                short sValue;
                for (int iY = 0; iY < m_iHeight; iY++)
                {
                    for (int iX = 0; iX < m_iWidth; iX++)
                    {
                        qColor = m_spqImageSaveTo->GetPixelRgb(iX,iY);

                        sValue = 0;
                        sValue |= Mathf::RoundToInt(qColor.R()*31) << 10;
                        sValue |= Mathf::RoundToInt(qColor.G()*31) << 5;
                        sValue |= Mathf::RoundToInt(qColor.B()*31);
                        qSaveStream.Write(sValue);
                    }

                    qProgress.MakeAdvance();
                }
            }
            else if ( m_iBitsPerPixel == 24 )
            {
                ColorRgb qColor;
                unsigned char ucR, ucG, ucB;
                for (int iY = 0; iY < m_iHeight; iY++)
                {
                    for (int iX = 0; iX < m_iWidth; iX++)
                    {
                        qColor = m_spqImageSaveTo->GetPixelRgb(iX,iY);
                        ucR = Mathf::RoundToInt(qColor.R()*255);
                        ucG = Mathf::RoundToInt(qColor.G()*255);
                        ucB = Mathf::RoundToInt(qColor.B()*255);

                        qSaveStream.Write(ucB);
                        qSaveStream.Write(ucG);
                        qSaveStream.Write(ucR);
                    }

                    qProgress.MakeAdvance();
                }
            }
            else  // m_iBitsPerPixel = 32
            {
                ColorRgba qColor;
                unsigned char ucR, ucG, ucB, ucA;
                for (int iY = 0; iY < m_iHeight; iY++)
                {
                    for (int iX = 0; iX < m_iWidth; iX++)
                    {
                        qColor = m_spqImageSaveTo->GetPixelRgba(iX,iY);
                        ucR = Mathf::RoundToInt(qColor.R()*255);
                        ucG = Mathf::RoundToInt(qColor.G()*255);
                        ucB = Mathf::RoundToInt(qColor.B()*255);
                        ucA = Mathf::RoundToInt(qColor.A()*255);

                        qSaveStream.Write(ucB);
                        qSaveStream.Write(ucG);
                        qSaveStream.Write(ucR);
                        qSaveStream.Write(ucA);
                    }

                    qProgress.MakeAdvance();
                }
            }
        }
        else if ( cImageType == 10 )
        {
            short sValue;
            ColorRgb qColorRgb;
            ColorRgba qColorRgba;
            unsigned char ucR, ucG, ucB, ucA;
            int iXS, iRepeat, iRPQ, iSum, iTop, iLastRPBytePos;
            bool bLastWasRP;
            for (int iY = 0; iY < m_iHeight; iY++)
            {
                iXS = 0;
                bLastWasRP = false;
                while ( iXS < m_iWidth )
                {
                    iRepeat = 1;
                    if ( m_iBitsPerPixel == 16 || m_iBitsPerPixel == 24 )
                    {
                        qColorRgb = m_spqImageSaveTo->GetPixelRgb(iXS,iY);
                        for (int iX = iXS+1; iX < m_iWidth; iX++)
                        {
                            if ( m_spqImageSaveTo->GetPixelRgb(iX,iY) != qColorRgb )
                                break;
                            if ( ++iRepeat == 128 )
                                break;
                        }
                    }
                    else  // m_iBitsPerPixel = 32
                    {
                        qColorRgba = m_spqImageSaveTo->GetPixelRgba(iXS,iY);
                        for (int iX = iXS+1; iX < m_iWidth; iX++)
                        {
                            if ( m_spqImageSaveTo->GetPixelRgba(iX,iY) != qColorRgba )
                                break;
                            if ( ++iRepeat == 128 )
                                break;
                        }
                    }

                    if ( iRepeat > 1 )
                    {
                        // that's where run-length packets begin to be economic
                        qSaveStream.WriteBits(1,1);
                        qSaveStream.WriteBits(iRepeat-1,7);

                        if ( m_iBitsPerPixel == 16 )
                        {
                            sValue = 0;
                            sValue |= Mathf::RoundToInt(qColorRgb.R()*31) << 10;
                            sValue |= Mathf::RoundToInt(qColorRgb.G()*31) << 5;
                            sValue |= Mathf::RoundToInt(qColorRgb.B()*31);
                            qSaveStream.Write(sValue);
                        }
                        else if ( m_iBitsPerPixel == 24 )
                        {
                            ucR = Mathf::RoundToInt(qColorRgb.R()*255);
                            ucG = Mathf::RoundToInt(qColorRgb.G()*255);
                            ucB = Mathf::RoundToInt(qColorRgb.B()*255);

                            qSaveStream.Write(ucB);
                            qSaveStream.Write(ucG);
                            qSaveStream.Write(ucR);
                        }
                        else  // m_iBitsPerPixel = 32
                        {
                            ucR = Mathf::RoundToInt(qColorRgba.R()*255);
                            ucG = Mathf::RoundToInt(qColorRgba.G()*255);
                            ucB = Mathf::RoundToInt(qColorRgba.B()*255);
                            ucA = Mathf::RoundToInt(qColorRgba.A()*255);

                            qSaveStream.Write(ucB);
                            qSaveStream.Write(ucG);
                            qSaveStream.Write(ucR);
                            qSaveStream.Write(ucA);
                        }

                        iXS += iRepeat;

                        bLastWasRP = false;
                    }
                    else
                    {
                        // raw packet
                        if ( !bLastWasRP )
                        {
                            iRPQ = 0;
                            iLastRPBytePos = qSaveStream.GetBytePosition();
                            qSaveStream.WriteBits(0,1);
                            qSaveStream.WriteBits(0,7);
                        }

                        iSum = iRPQ + iRepeat;
                        qSaveStream.PushPositionState();
                        qSaveStream.SetBytePosition(iLastRPBytePos);
                        qSaveStream.SetBitRatio(1);
                        if ( iSum <= 128 )
                        {
                            qSaveStream.WriteBits(iSum-1,7);
                            qSaveStream.PopPositionState();
                            if ( m_iBitsPerPixel == 16 )
                            {
                                for (int i = 0; i < iRepeat; i++)
                                {
                                    qColorRgb = m_spqImageSaveTo->GetPixelRgb(iXS+i,iY);

                                    sValue = 0;
                                    sValue |= Mathf::RoundToInt(qColorRgb.R()*31) << 10;
                                    sValue |= Mathf::RoundToInt(qColorRgb.G()*31) << 5;
                                    sValue |= Mathf::RoundToInt(qColorRgb.B()*31);
                                    qSaveStream.Write(sValue);
                                }
                            }
                            else if ( m_iBitsPerPixel == 24 )
                            {
                                for (int i = 0; i < iRepeat; i++)
                                {
                                    qColorRgb = m_spqImageSaveTo->GetPixelRgb(iXS+i,iY);
                                    ucR = Mathf::RoundToInt(qColorRgb.R()*255);
                                    ucG = Mathf::RoundToInt(qColorRgb.G()*255);
                                    ucB = Mathf::RoundToInt(qColorRgb.B()*255);

                                    qSaveStream.Write(ucB);
                                    qSaveStream.Write(ucG);
                                    qSaveStream.Write(ucR);
                                }
                            }
                            else  // m_iBitsPerPixel = 32
                            {
                                for (int i = 0; i < iRepeat; i++)
                                {
                                    qColorRgba = m_spqImageSaveTo->GetPixelRgba(iXS+i,iY);
                                    ucR = Mathf::RoundToInt(qColorRgba.R()*255);
                                    ucG = Mathf::RoundToInt(qColorRgba.G()*255);
                                    ucB = Mathf::RoundToInt(qColorRgba.B()*255);
                                    ucA = Mathf::RoundToInt(qColorRgba.A()*255);

                                    qSaveStream.Write(ucB);
                                    qSaveStream.Write(ucG);
                                    qSaveStream.Write(ucR);
                                    qSaveStream.Write(ucA);
                                }
                            }

                            iXS += iRepeat;
                        }
                        else
                        {
                            qSaveStream.WriteBits(127,7);
                            qSaveStream.PopPositionState();
                            iTop = 128 - iRPQ;
                            if ( m_iBitsPerPixel == 16 )
                            {
                                for (int i = 0; i < iTop; i++)
                                {
                                    qColorRgb = m_spqImageSaveTo->GetPixelRgb(iXS+i,iY);

                                    sValue = 0;
                                    sValue |= Mathf::RoundToInt(qColorRgb.R()*31) << 10;
                                    sValue |= Mathf::RoundToInt(qColorRgb.G()*31) << 5;
                                    sValue |= Mathf::RoundToInt(qColorRgb.B()*31);
                                    qSaveStream.Write(sValue);
                                }
                            }
                            else if ( m_iBitsPerPixel == 24 )
                            {
                                for (int i = 0; i < iTop; i++)
                                {
                                    qColorRgb = m_spqImageSaveTo->GetPixelRgb(iXS+i,iY);
                                    ucR = Mathf::RoundToInt(qColorRgb.R()*255);
                                    ucG = Mathf::RoundToInt(qColorRgb.G()*255);
                                    ucB = Mathf::RoundToInt(qColorRgb.B()*255);

                                    qSaveStream.Write(ucB);
                                    qSaveStream.Write(ucG);
                                    qSaveStream.Write(ucR);
                                }
                            }
                            else  // m_iBitsPerPixel = 32
                            {
                                for (int i = 0; i < iTop; i++)
                                {
                                    qColorRgba = m_spqImageSaveTo->GetPixelRgba(iXS+i,iY);
                                    ucR = Mathf::RoundToInt(qColorRgba.R()*255);
                                    ucG = Mathf::RoundToInt(qColorRgba.G()*255);
                                    ucB = Mathf::RoundToInt(qColorRgba.B()*255);
                                    ucA = Mathf::RoundToInt(qColorRgba.A()*255);

                                    qSaveStream.Write(ucB);
                                    qSaveStream.Write(ucG);
                                    qSaveStream.Write(ucR);
                                    qSaveStream.Write(ucA);
                                }
                            }

                            iXS += iTop;
                        }
                        if ( iSum < 128 )
                        {
                            iRPQ = iSum;
                            bLastWasRP = true;
                        }
                        else
                        {
                            bLastWasRP = false;
                        }
                    }
                }

                qProgress.MakeAdvance();
            }
        }
        else if ( cImageType == 3 )
        {
            unsigned char ucIntensity;
            for (int iY = 0; iY < m_iHeight; iY++)
            {
                for (int iX = 0; iX < m_iWidth; iX++)
                {
                    ucIntensity = Mathf::RoundToInt(m_spqImageSaveTo->GetPixelIntensity(iX,iY)*255);
                    qSaveStream.Write(ucIntensity);
                }

                qProgress.MakeAdvance();
            }
        }
        else  // cImageType = 11
        {
            float fIntensity;
            unsigned char ucIntensity;
            int iXS, iRepeat, iRPQ, iSum, iTop, iLastRPBytePos;
            bool bLastWasRP;
            for (int iY = 0; iY < m_iHeight; iY++)
            {
                iXS = 0;
                bLastWasRP = false;
                while ( iXS < m_iWidth )
                {
                    iRepeat = 1;
                    fIntensity = m_spqImageSaveTo->GetPixelIntensity(iXS,iY);
                    for (int iX = iXS+1; iX < m_iWidth; iX++)
                    {
                        if ( m_spqImageSaveTo->GetPixelIntensity(iX,iY) != fIntensity )
                            break;
                        if ( ++iRepeat == 128 )
                            break;
                    }

                    if ( iRepeat > 2 )
                    {
                        // that's where run-length packets begin to be economic
                        qSaveStream.WriteBits(1,1);
                        qSaveStream.WriteBits(iRepeat-1,7);

                        ucIntensity = Mathf::RoundToInt(fIntensity*255);
                        qSaveStream.Write(ucIntensity);

                        iXS += iRepeat;

                        bLastWasRP = false;
                    }
                    else
                    {
                        // raw packet
                        if ( !bLastWasRP )
                        {
                            iRPQ = 0;
                            iLastRPBytePos = qSaveStream.GetBytePosition();
                            qSaveStream.WriteBits(0,1);
                            qSaveStream.WriteBits(0,7);
                        }

                        iSum = iRPQ + iRepeat;
                        qSaveStream.PushPositionState();
                        qSaveStream.SetBytePosition(iLastRPBytePos);
                        qSaveStream.SetBitRatio(1);
                        if ( iSum <= 128 )
                        {
                            qSaveStream.WriteBits(iSum-1,7);
                            qSaveStream.PopPositionState();
                            for (int i = 0; i < iRepeat; i++)
                            {
                                fIntensity = m_spqImageSaveTo->GetPixelIntensity(iXS+i,iY);
                                ucIntensity = Mathf::RoundToInt(fIntensity*255);
                                qSaveStream.Write(ucIntensity);
                            }

                            iXS += iRepeat;
                        }
                        else
                        {
                            qSaveStream.WriteBits(127,7);
                            qSaveStream.PopPositionState();
                            iTop = 128 - iRPQ;
                            for (int i = 0; i < iTop; i++)
                            {
                                fIntensity = m_spqImageSaveTo->GetPixelIntensity(iXS+i,iY);
                                ucIntensity = Mathf::RoundToInt(fIntensity*255);
                                qSaveStream.Write(ucIntensity);
                            }

                            iXS += iTop;
                        }
                        if ( iSum < 128 )
                        {
                            iRPQ = iSum;
                            bLastWasRP = true;
                        }
                        else
                        {
                            bLastWasRP = false;
                        }
                    }
                }

                qProgress.MakeAdvance();
            }
        }
    }

    // TGA footer
    int iExtensionAreaOffset = 0;
    int iDeveloperDirectoryOffset = 0;
    qSaveStream.Write(iExtensionAreaOffset);
    qSaveStream.Write(iDeveloperDirectoryOffset);
    const char* acSignature = "TRUEVISION-XFILE.";
    qSaveStream.Write(18,acSignature);

    // flush to the file
    CONDITIONAL_THROW_EXCEPTION(qSaveStream.Finalize(),ET_CANNOT_ACCESS_FILE);

    return true;
}
//------------------------------------------------------------------------------------------------------------------
FormatTga::FormatTga (const char* acFilename, bool& rbSuccess)
    :
    FormatBase(acFilename)
{
    bool bSuccess;

    Stream qHeaderStream(m_eStreaming,m_qFilename,0,18,true,bSuccess);
    C_PROCESS_STREAM_SUCCESS_FOR_LOADING(bSuccess);

    unsigned char ucIdLength;
    qHeaderStream.Read(ucIdLength);
    m_iIdLength = ucIdLength;

    char cColorMapType;
    ColorMapPresenceType eColorMap;
    qHeaderStream.Read(cColorMapType);
    switch ( cColorMapType )
    {
    case 0:
        eColorMap = CMPT_NOT_INCLUDED;
        break;
    case 1:
        eColorMap = CMPT_INCLUDED;
        break;
    default:
        C_THROW_EXCEPTION(ET_FORMAT_MISMATCH);
    }

    qHeaderStream.Read(m_cImageType);
    switch ( m_cImageType )
    {
    case 0:
        C_THROW_EXCEPTION(ET_FORMAT_MISMATCH);
        break;
    case 1:
        m_eImage = IT_PALETTED;
        m_eCompression = CT_NONE;
        break;
    case 2:
        m_eImage = IT_CHANNELED;
        m_eCompression = CT_NONE;
        break;
    case 3:
        m_eImage = IT_GRAYSCALE;
        m_eCompression = CT_NONE;
        break;
    case 9:
        m_eImage = IT_PALETTED;
        m_eCompression = CT_RLE;
        break;
    case 10:
        m_eImage = IT_CHANNELED;
        m_eCompression = CT_RLE;
        break;
    case 11:
        m_eImage = IT_GRAYSCALE;
        m_eCompression = CT_RLE;
        break;
    default:
        C_THROW_EXCEPTION(ET_FORMAT_MISMATCH);
    }

    // eColorMap/m_eImage contradiction
    C_CONDITIONAL_THROW_EXCEPTION(( ( eColorMap == CMPT_INCLUDED ) == ( m_eImage == IT_PALETTED ) ),
        ET_FORMAT_MISMATCH);

    m_bPaletted = ( eColorMap == CMPT_INCLUDED );

    unsigned short usFirstEntryIndex;
    qHeaderStream.Read(usFirstEntryIndex);
    m_iFirstEntryIndex = usFirstEntryIndex;

    unsigned short usPaletteSize;
    qHeaderStream.Read(usPaletteSize);
    m_iPaletteSize = usPaletteSize;

    C_CONDITIONAL_THROW_EXCEPTION(( !(m_bPaletted && m_iFirstEntryIndex >= m_iPaletteSize) ),
        ET_FORMAT_MISMATCH);

    unsigned char ucBitsPerPaletteColor;
    qHeaderStream.Read(ucBitsPerPaletteColor);
    m_iBitsPerPaletteColor = ucBitsPerPaletteColor;
    C_CONDITIONAL_THROW_EXCEPTION(( !(m_bPaletted && m_iBitsPerPaletteColor != 24) ),ET_FORMAT_MISMATCH);

    // ignore information about position of the image's origin
    qHeaderStream.OffsetBytePosition(4);

    unsigned short usWidth, usHeight;
    qHeaderStream.Read(usWidth);
    qHeaderStream.Read(usHeight);
    C_CONDITIONAL_THROW_EXCEPTION(( usWidth != 0 && usHeight != 0 ),ET_FORMAT_MISMATCH);
    m_iWidth = usWidth;
    m_iHeight = usHeight;

    unsigned char ucBitsPerPixel;
    qHeaderStream.Read(ucBitsPerPixel);
    m_iBitsPerPixel = ucBitsPerPixel;
    C_CONDITIONAL_THROW_EXCEPTION(
        ( m_iBitsPerPixel == 8 || m_iBitsPerPixel == 16 || m_iBitsPerPixel == 24 || m_iBitsPerPixel == 32 ),
        ET_FORMAT_MISMATCH);
    C_CONDITIONAL_THROW_EXCEPTION(( ( m_bPaletted || m_eImage == IT_GRAYSCALE ) == ( m_iBitsPerPixel == 8 ) ),
        ET_FORMAT_MISMATCH);

    // orientation of the image
    qHeaderStream.SetBitRatio(2);
    qHeaderStream.ReadUBits(m_iImageOrientation,2);

    qHeaderStream.ReadUBits(m_iBitsPerAttribute,4);

    qHeaderStream.Finalize();

    rbSuccess = true;
}
//------------------------------------------------------------------------------------------------------------------
bool FormatTga::LoadImage (RasterImagePtr& rspqImage)
{
    bool bSuccess;

    int iSizeOfPaletteData = ( m_bPaletted ? 3*m_iPaletteSize : 0 );

    // to read pixel data, make stream from all remaining bytes up to the file's end
    int iAlreadyRead = 18 + m_iIdLength + iSizeOfPaletteData;
    int iFileSize;
    CONDITIONAL_THROW_EXCEPTION(Storage::GetFileSize(iFileSize,m_qFilename),ET_CANNOT_ACCESS_FILE);

    Stream qImageStream(m_eStreaming,m_acStreamSrcParam,iAlreadyRead,iFileSize-iAlreadyRead,true,bSuccess);
    PROCESS_STREAM_SUCCESS_FOR_LOADING(bSuccess);

    // for a progress bar
    ProgressProbe qProgress(this);
    qProgress.SetAdvance(1.0f/m_iHeight);

    if ( m_bPaletted )
    {
        // read the palette
        Stream qPaletteStream(m_eStreaming,m_acStreamSrcParam,18+m_iIdLength,iSizeOfPaletteData,true,bSuccess);
        PROCESS_STREAM_SUCCESS_FOR_LOADING(bSuccess);
        RasterImage::Palette qPalette(m_iPaletteSize);
        qPaletteStream.OffsetBytePosition(3*m_iFirstEntryIndex);
        for (int i = 0; i < m_iPaletteSize; i++)
            qPaletteStream.Read(3,qPalette[i].Components);
        qPaletteStream.Finalize();

        rspqImage = new RasterImage(RasterImage::IT_PALETTED,m_iWidth,m_iHeight,qPalette);

        // read actual image
        char* pcTopByte = rspqImage->GetImage() + m_iHeight*rspqImage->GetRowStride() - 1;
        char* acRow = rspqImage->GetRow(0);
        if ( m_cImageType == 1 )
        {
            unsigned char ucValue;
            char* pcPixel = acRow;
            for (int iY = 0; iY < m_iHeight; iY++)
            {
                for (int iX = 0; iX < m_iWidth; iX++)
                {
                    RETURN_ON_FAIL(qImageStream.Read(ucValue));
                    CONDITIONAL_THROW_EXCEPTION(( ucValue < m_iPaletteSize ),ET_FORMAT_MISMATCH);
                    *pcPixel++ = ucValue;
                }
                acRow += rspqImage->GetRowStride();
                pcPixel = acRow;

                qProgress.MakeAdvance();
            }
        }
        else  // m_cImageType = 9
        {
            int iY = 0;
            int iX;
            int iPacketType, iCount;
            unsigned char ucValue;
            char* pcPixel = acRow;
            while ( iY < m_iHeight )
            {
                iX = 0;
                while ( iX < m_iWidth )
                {
                    RETURN_ON_FAIL(qImageStream.ReadUBits(iPacketType,1));
                    RETURN_ON_FAIL(qImageStream.ReadUBits(iCount,7));
                    iCount++;
                    if ( iPacketType == 1 )
                    {
                        // run-length packet
                        RETURN_ON_FAIL(qImageStream.Read(ucValue));
                        CONDITIONAL_THROW_EXCEPTION(( ucValue < m_iPaletteSize ),ET_FORMAT_MISMATCH);
                        for (int i = 0; i < iCount; i++)
                        {
                            CONDITIONAL_THROW_EXCEPTION(( pcPixel <= pcTopByte ),ET_FORMAT_MISMATCH);
                            *pcPixel++ = ucValue;
                        }
                    }
                    else
                    {
                        // raw packet
                        for (int i = 0; i < iCount; i++)
                        {
                            RETURN_ON_FAIL(qImageStream.Read(ucValue));
                            CONDITIONAL_THROW_EXCEPTION(( ucValue < m_iPaletteSize ),ET_FORMAT_MISMATCH);
                            CONDITIONAL_THROW_EXCEPTION(( pcPixel <= pcTopByte ),ET_FORMAT_MISMATCH);
                            *pcPixel++ = ucValue;
                        }
                    }
                    iX += iCount;
                    CONDITIONAL_THROW_EXCEPTION(( iX <= m_iWidth ),ET_FORMAT_MISMATCH);
                }
                acRow += rspqImage->GetRowStride();
                pcPixel = acRow;
                iY++;

                qProgress.MakeAdvance();
            }
        }
    }
    else
    {
        if ( m_eImage == IT_CHANNELED )
        {
            if ( m_iBitsPerPixel != 32 )
                rspqImage = new RasterImage(RasterImage::IT_RGB_8_8_8,m_iWidth,m_iHeight);
            else
                rspqImage = new RasterImage(RasterImage::IT_RGBA_8_8_8_8,m_iWidth,m_iHeight);
        }
        else  // m_eImage = IT_GRAYSCALE
        {
            rspqImage = new RasterImage(RasterImage::IT_GRAYSCALE_8,m_iWidth,m_iHeight);
        }

        // read actual image
        char* pcTopByte = rspqImage->GetImage() + m_iHeight*rspqImage->GetRowStride() - 1;
        char* acRow = rspqImage->GetRow(0);
        if ( m_cImageType == 2 )
        {
            char* pcComp = acRow;
            if ( m_iBitsPerPixel == 16 )
            {
                short sValue;
                for (int iY = 0; iY < m_iHeight; iY++)
                {
                    for (int iX = 0; iX < m_iWidth; iX++)
                    {
                        RETURN_ON_FAIL(qImageStream.Read(sValue));

                        *pcComp++ = Mathf::RoundToInt(((float)((sValue >> 10) & 31))/31*255);
                        *pcComp++ = Mathf::RoundToInt(((float)((sValue >>  5) & 31))/31*255);
                        *pcComp++ = Mathf::RoundToInt(((float)((sValue >>  0) & 31))/31*255);
                    }
                    acRow += rspqImage->GetRowStride();
                    pcComp = acRow;

                    qProgress.MakeAdvance();
                }
            }
            else if ( m_iBitsPerPixel == 24 )
            {
                unsigned char ucR, ucG, ucB;
                for (int iY = 0; iY < m_iHeight; iY++)
                {
                    for (int iX = 0; iX < m_iWidth; iX++)
                    {
                        RETURN_ON_FAIL(qImageStream.Read(ucB));
                        RETURN_ON_FAIL(qImageStream.Read(ucG));
                        RETURN_ON_FAIL(qImageStream.Read(ucR));

                        *pcComp++ = ucR;
                        *pcComp++ = ucG;
                        *pcComp++ = ucB;
                    }
                    acRow += rspqImage->GetRowStride();
                    pcComp = acRow;

                    qProgress.MakeAdvance();
                }
            }
            else  // m_iBitsPerPixel = 32
            {
                unsigned char ucR, ucG, ucB, ucA;
                for (int iY = 0; iY < m_iHeight; iY++)
                {
                    for (int iX = 0; iX < m_iWidth; iX++)
                    {
                        RETURN_ON_FAIL(qImageStream.Read(ucB));
                        RETURN_ON_FAIL(qImageStream.Read(ucG));
                        RETURN_ON_FAIL(qImageStream.Read(ucR));
                        RETURN_ON_FAIL(qImageStream.Read(ucA));

                        *pcComp++ = ucR;
                        *pcComp++ = ucG;
                        *pcComp++ = ucB;
                        *pcComp++ = ucA;
                    }
                    acRow += rspqImage->GetRowStride();
                    pcComp = acRow;

                    qProgress.MakeAdvance();
                }
            }
        }
        else if ( m_cImageType == 10 )
        {
            int iY = 0;
            int iX;
            int iPacketType, iCount;
            short sValue;
            unsigned char ucR, ucG, ucB, ucA;
            char* pcComp = acRow;
            while ( iY < m_iHeight )
            {
                iX = 0;
                while ( iX < m_iWidth )
                {
                    RETURN_ON_FAIL(qImageStream.ReadUBits(iPacketType,1));
                    RETURN_ON_FAIL(qImageStream.ReadUBits(iCount,7));
                    iCount++;
                    if ( iPacketType == 1 )
                    {
                        // run-length packet
                        if ( m_iBitsPerPixel == 16 )
                        {
                            RETURN_ON_FAIL(qImageStream.Read(sValue));
                            for (int i = 0; i < iCount; i++)
                            {
                                CONDITIONAL_THROW_EXCEPTION(( pcComp <= pcTopByte ),ET_FORMAT_MISMATCH);
                                *pcComp++ = Mathf::RoundToInt(((float)((sValue >> 10) & 31))/31*255);
                                CONDITIONAL_THROW_EXCEPTION(( pcComp <= pcTopByte ),ET_FORMAT_MISMATCH);
                                *pcComp++ = Mathf::RoundToInt(((float)((sValue >>  5) & 31))/31*255);
                                CONDITIONAL_THROW_EXCEPTION(( pcComp <= pcTopByte ),ET_FORMAT_MISMATCH);
                                *pcComp++ = Mathf::RoundToInt(((float)((sValue >>  0) & 31))/31*255);
                            }
                        }
                        else if ( m_iBitsPerPixel == 24 )
                        {
                            RETURN_ON_FAIL(qImageStream.Read(ucB));
                            RETURN_ON_FAIL(qImageStream.Read(ucG));
                            RETURN_ON_FAIL(qImageStream.Read(ucR));
                            for (int i = 0; i < iCount; i++)
                            {
                                CONDITIONAL_THROW_EXCEPTION(( pcComp <= pcTopByte ),ET_FORMAT_MISMATCH);
                                *pcComp++ = ucR;
                                CONDITIONAL_THROW_EXCEPTION(( pcComp <= pcTopByte ),ET_FORMAT_MISMATCH);
                                *pcComp++ = ucG;
                                CONDITIONAL_THROW_EXCEPTION(( pcComp <= pcTopByte ),ET_FORMAT_MISMATCH);
                                *pcComp++ = ucB;
                            }
                        }
                        else  // m_iBitsPerPixel = 32
                        {
                            RETURN_ON_FAIL(qImageStream.Read(ucB));
                            RETURN_ON_FAIL(qImageStream.Read(ucG));
                            RETURN_ON_FAIL(qImageStream.Read(ucR));
                            RETURN_ON_FAIL(qImageStream.Read(ucA));
                            for (int i = 0; i < iCount; i++)
                            {
                                CONDITIONAL_THROW_EXCEPTION(( pcComp <= pcTopByte ),ET_FORMAT_MISMATCH);
                                *pcComp++ = ucR;
                                CONDITIONAL_THROW_EXCEPTION(( pcComp <= pcTopByte ),ET_FORMAT_MISMATCH);
                                *pcComp++ = ucG;
                                CONDITIONAL_THROW_EXCEPTION(( pcComp <= pcTopByte ),ET_FORMAT_MISMATCH);
                                *pcComp++ = ucB;
                                CONDITIONAL_THROW_EXCEPTION(( pcComp <= pcTopByte ),ET_FORMAT_MISMATCH);
                                *pcComp++ = ucA;
                            }
                        }
                    }
                    else
                    {
                        // raw packet
                        if ( m_iBitsPerPixel == 16 )
                        {
                            for (int i = 0; i < iCount; i++)
                            {
                                RETURN_ON_FAIL(qImageStream.Read(sValue));

                                CONDITIONAL_THROW_EXCEPTION(( pcComp <= pcTopByte ),ET_FORMAT_MISMATCH);
                                *pcComp++ = Mathf::RoundToInt(((float)((sValue >> 10) & 31))/31*255);
                                CONDITIONAL_THROW_EXCEPTION(( pcComp <= pcTopByte ),ET_FORMAT_MISMATCH);
                                *pcComp++ = Mathf::RoundToInt(((float)((sValue >>  5) & 31))/31*255);
                                CONDITIONAL_THROW_EXCEPTION(( pcComp <= pcTopByte ),ET_FORMAT_MISMATCH);
                                *pcComp++ = Mathf::RoundToInt(((float)((sValue >>  0) & 31))/31*255);
                            }
                        }
                        else if ( m_iBitsPerPixel == 24 )
                        {
                            for (int i = 0; i < iCount; i++)
                            {
                                RETURN_ON_FAIL(qImageStream.Read(ucB));
                                RETURN_ON_FAIL(qImageStream.Read(ucG));
                                RETURN_ON_FAIL(qImageStream.Read(ucR));

                                CONDITIONAL_THROW_EXCEPTION(( pcComp <= pcTopByte ),ET_FORMAT_MISMATCH);
                                *pcComp++ = ucR;
                                CONDITIONAL_THROW_EXCEPTION(( pcComp <= pcTopByte ),ET_FORMAT_MISMATCH);
                                *pcComp++ = ucG;
                                CONDITIONAL_THROW_EXCEPTION(( pcComp <= pcTopByte ),ET_FORMAT_MISMATCH);
                                *pcComp++ = ucB;
                            }
                        }
                        else  // m_iBitsPerPixel = 32
                        {
                            for (int i = 0; i < iCount; i++)
                            {
                                RETURN_ON_FAIL(qImageStream.Read(ucB));
                                RETURN_ON_FAIL(qImageStream.Read(ucG));
                                RETURN_ON_FAIL(qImageStream.Read(ucR));
                                RETURN_ON_FAIL(qImageStream.Read(ucA));

                                CONDITIONAL_THROW_EXCEPTION(( pcComp <= pcTopByte ),ET_FORMAT_MISMATCH);
                                *pcComp++ = ucR;
                                CONDITIONAL_THROW_EXCEPTION(( pcComp <= pcTopByte ),ET_FORMAT_MISMATCH);
                                *pcComp++ = ucG;
                                CONDITIONAL_THROW_EXCEPTION(( pcComp <= pcTopByte ),ET_FORMAT_MISMATCH);
                                *pcComp++ = ucB;
                                CONDITIONAL_THROW_EXCEPTION(( pcComp <= pcTopByte ),ET_FORMAT_MISMATCH);
                                *pcComp++ = ucA;
                            }
                        }
                    }
                    iX += iCount;
                    CONDITIONAL_THROW_EXCEPTION(( iX <= m_iWidth ),ET_FORMAT_MISMATCH);
                }
                acRow += rspqImage->GetRowStride();
                pcComp = acRow;
                iY++;

                qProgress.MakeAdvance();
            }
        }
        else if ( m_cImageType == 3 )
        {
            unsigned char ucValue;
            char* pcPixel = acRow;
            for (int iY = 0; iY < m_iHeight; iY++)
            {
                for (int iX = 0; iX < m_iWidth; iX++)
                {
                    RETURN_ON_FAIL(qImageStream.Read(ucValue));
                    *pcPixel++ = ucValue;
                }
                acRow += rspqImage->GetRowStride();
                pcPixel = acRow;

                qProgress.MakeAdvance();
            }
        }
        else  // m_cImageType = 11
        {
            int iY = 0;
            int iX;
            int iPacketType, iCount;
            unsigned char ucValue;
            char* pcPixel = acRow;
            while ( iY < m_iHeight )
            {
                iX = 0;
                while ( iX < m_iWidth )
                {
                    RETURN_ON_FAIL(qImageStream.ReadUBits(iPacketType,1));
                    RETURN_ON_FAIL(qImageStream.ReadUBits(iCount,7));
                    iCount++;
                    if ( iPacketType == 1 )
                    {
                        // run-length packet
                        RETURN_ON_FAIL(qImageStream.Read(ucValue));
                        for (int i = 0; i < iCount; i++)
                        {
                            CONDITIONAL_THROW_EXCEPTION(( pcPixel <= pcTopByte ),ET_FORMAT_MISMATCH);
                            *pcPixel++ = ucValue;
                        }
                    }
                    else
                    {
                        // raw packet
                        for (int i = 0; i < iCount; i++)
                        {
                            RETURN_ON_FAIL(qImageStream.Read(ucValue));
                            CONDITIONAL_THROW_EXCEPTION(( pcPixel <= pcTopByte ),ET_FORMAT_MISMATCH);
                            *pcPixel++ = ucValue;
                        }
                    }
                    iX += iCount;
                    CONDITIONAL_THROW_EXCEPTION(( iX <= m_iWidth ),ET_FORMAT_MISMATCH);
                }
                acRow += rspqImage->GetRowStride();
                pcPixel = acRow;
                iY++;

                qProgress.MakeAdvance();
            }
        }
    }

    qImageStream.Finalize();

    // re-orientate the image if needed
    if ( m_iImageOrientation == 0 )
    {
        RasterImageTransformation qRIT(rspqImage);
        rspqImage = qRIT.GetFlippedVertically();
    }
    else if ( m_iImageOrientation == 1 )
    {
        RasterImageTransformation qRIT(rspqImage);
        rspqImage = qRIT.GetRotated180();
    }
    else if ( m_iImageOrientation == 3 )
    {
        RasterImageTransformation qRIT(rspqImage);
        rspqImage = qRIT.GetFlippedHorizontally();
    }

    return true;
}
//------------------------------------------------------------------------------------------------------------------



