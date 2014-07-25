#include "QFormatIff.h"
#include "QExptWarnBase.mcr"
#include "QFormatBase.mcr"
#include "QFormatIff.mcr"
#include "QProgressProbe.h"
#include "QStorage.h"
#include "QCompressionPackBits.h"
using namespace Q;

// supported type IDs
static const char* gs_acFORM = "FORM";
static const char* gs_acILBM = "ILBM";
static const char* gs_acBMHD = "BMHD";
static const char* gs_acCMAP = "CMAP";
static const char* gs_acCAMG = "CAMG";
static const char* gs_acDPI  = "DPI ";
static const char* gs_acBODY = "BODY";

// support for saving
class UcRgb
{
public:
    unsigned char Rgb[3];
};
class UcRgba
{
public:
    unsigned char Rgba[4];
};

//------------------------------------------------------------------------------------------------------------------
FormatIff::FormatIff (const RasterImagePtr& rspqImage, int iBitsPerPixel)
    :
    m_spqImageSaveTo(rspqImage),
    m_iWidth(rspqImage->GetWidth()),
    m_iHeight(rspqImage->GetHeight()),
    m_eCompression(CT_NONE),
    m_bHasDPIChunk(true),
    m_iHorizontalDpi(72),
    m_iVerticalDpi(72)
{
    assert( rspqImage->GetWidth() != 0 && rspqImage->GetHeight() != 0 );
    assert( rspqImage->GetWidth() <= 65535 && rspqImage->GetHeight() <= 65535 );
    assert( (rspqImage->GetColorModel() == RasterImage::CMT_PALETTED
        &&  (iBitsPerPixel == -1 || (1 <= iBitsPerPixel && iBitsPerPixel <= 8)))
        ||  (rspqImage->GetColorModel() == RasterImage::CMT_GRAYSCALE && iBitsPerPixel == 8)
        ||  (rspqImage->GetColorModel() == RasterImage::CMT_RGB && iBitsPerPixel == 24)
        ||  (rspqImage->GetColorModel() == RasterImage::CMT_RGBA && iBitsPerPixel == 32) );
    assert( !(rspqImage->GetColorModel() == RasterImage::CMT_PALETTED && iBitsPerPixel != -1
        &&  1 << iBitsPerPixel < rspqImage->GetPaletteSize()) );

    // internal color model
    switch ( rspqImage->GetColorModel() )
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
    case RasterImage::CMT_RGBA:
        m_eColorModel = CMT_RGBA;
        break;
    }

    if ( iBitsPerPixel == -1 )
    {
        // find out the minimal possible bits-per-pixel value
        iBitsPerPixel = Storage::GetUBitQuantity(rspqImage->GetPaletteSize()-1);
    }

    m_iBitsPerPixel = iBitsPerPixel;

    m_iBitPlaneQuantity = m_iBitsPerPixel;
    m_eMasking = MT_NONE;
    if ( rspqImage->HasVisibilityMask() )
    {
        m_iBitPlaneQuantity++;
        m_eMasking = MT_HAS_MASK;
    }
}
//------------------------------------------------------------------------------------------------------------------
bool FormatIff::SaveImage (const char* acFilename)
{
    char cZero = 0;
    int iChunkDataSize;

    Stream qSaveStream(acFilename,false);

    qSaveStream.Write(4,gs_acFORM);
    for (int i = 0; i < 4; i++)  // hole #1
        qSaveStream.Write(cZero);
    qSaveStream.Write(4,gs_acILBM);

    // Image header.
    qSaveStream.Write(4,gs_acBMHD);

    iChunkDataSize = 20;
    qSaveStream.Write(iChunkDataSize);

    unsigned short usWidth = m_iWidth;
    unsigned short usHeight = m_iHeight;
    qSaveStream.Write(usWidth);
    qSaveStream.Write(usHeight);

    short sPosX = 0;
    short sPosY = 0;
    qSaveStream.Write(sPosX);
    qSaveStream.Write(sPosY);

    unsigned char ucPlanes = m_iBitPlaneQuantity;
    qSaveStream.Write(ucPlanes);

    unsigned char ucMasking;
    switch ( m_eMasking )
    {
    case MT_NONE:
        ucMasking = 0;
        break;
    case MT_HAS_MASK:
        ucMasking = 1;
        break;
    }
    qSaveStream.Write(ucMasking);

    unsigned char ucCompression;
    switch ( m_eCompression )
    {
    case CT_NONE:
        ucCompression = 0;
        break;
    case CT_RLE:
        ucCompression = 1;
        break;
    }
    qSaveStream.Write(ucCompression);

    qSaveStream.Write(cZero);

    unsigned short usTransparentColor = 0;
    qSaveStream.Write(usTransparentColor);

    unsigned char ucAspectX = 1;
    unsigned char ucAspectY = 1;
    qSaveStream.Write(ucAspectX);
    qSaveStream.Write(ucAspectY);

    short sPageWidth = m_iWidth;
    short sPageHeight = m_iHeight;
    qSaveStream.Write(sPageWidth);
    qSaveStream.Write(sPageHeight);

    if ( m_bHasDPIChunk )
    {
        // dots per inch
        qSaveStream.Write(4,gs_acDPI);

        iChunkDataSize = 4;
        qSaveStream.Write(iChunkDataSize);

        unsigned short usDpiX = m_iHorizontalDpi;
        unsigned short usDpiY = m_iVerticalDpi;
        qSaveStream.Write(usDpiX);
        qSaveStream.Write(usDpiY);
    }

    if ( m_eColorModel == CMT_PALETTED
    ||   m_eColorModel == CMT_GRAYSCALE )
    {
        // palette
        qSaveStream.Write(4,gs_acCMAP);

        if ( m_eColorModel == CMT_PALETTED )
        {
            iChunkDataSize = m_spqImageSaveTo->GetPaletteSize()*3;
            qSaveStream.Write(iChunkDataSize);

            for (int i = 0; i < m_spqImageSaveTo->GetPaletteSize(); i++)
            {
                const PaletteColor& rqPaletteColor = m_spqImageSaveTo->GetPaletteColor(i);

                qSaveStream.Write(rqPaletteColor.R());
                qSaveStream.Write(rqPaletteColor.G());
                qSaveStream.Write(rqPaletteColor.B());
            }

            if ( (iChunkDataSize & 1) != 0 )
            {
                // pad byte
                qSaveStream.Write(cZero);
            }
        }
        else  // m_eColorModel = CMT_GRAYSCALE
        {
            // write the grayscale palette to avoid some decoder's confusion
            iChunkDataSize = 768;
            qSaveStream.Write(iChunkDataSize);

            unsigned char ucComp;
            for (int i0 = 0; i0 < 256; i0++)
            {
                ucComp = i0;
                for (int i1 = 0; i1 < 3; i1++)
                    qSaveStream.Write(ucComp);
            }
        }
    }

    int iIffRowByteSize = ((m_iWidth + 15)/16)*2;

    // for a progress bar
    ProgressProbe qProgress(this);
    qProgress.SetAdvance(1.0f/m_iHeight);

    // write the image
    qSaveStream.Write(4,gs_acBODY);
    for (int i = 0; i < 4; i++)  // hole #2
        qSaveStream.Write(cZero);
    int iBeforeImageDataPos = qSaveStream.GetBytePosition();
    StillArray<char> qIffRow(iIffRowByteSize);
    int iBytePos, iBitRatio, iValue;
    if ( m_eColorModel == CMT_PALETTED )
    {
        StillArray<int> qIndexRow(m_iWidth);
        for (int iY = 0; iY < m_iHeight; iY++)
        {
            for (int iX = 0; iX < m_iWidth; iX++)
                qIndexRow[iX] = m_spqImageSaveTo->GetPixelIndex(iX,iY);

            // IFF scanline
            for (int i = 0; i < m_iBitsPerPixel; i++)
            {
                // IFF row
                iBytePos = 0;
                iBitRatio = 0;
                for (int iX = 0; iX < m_iWidth; iX++)
                {
                    iValue = (qIndexRow[iX] >> i) & 1;
                    iBytePos += Storage::WriteBits(qIffRow+iBytePos,iBitRatio,1,iValue);
                }

                COMPLETE_AND_SAVE_IFF_ROW;
            }

            PROCESS_MASKING;

            qProgress.MakeAdvance();
        }
    }
    else if ( m_eColorModel == CMT_GRAYSCALE )
    {
        StillArray<unsigned char> qIntensityRow(m_iWidth);
        for (int iY = 0; iY < m_iHeight; iY++)
        {
            for (int iX = 0; iX < m_iWidth; iX++)
                qIntensityRow[iX] = Mathf::RoundToInt(m_spqImageSaveTo->GetPixelIntensity(iX,iY)*255);

            // IFF scanline
            for (int i = 0; i < m_iBitsPerPixel; i++)
            {
                // IFF row
                iBytePos = 0;
                iBitRatio = 0;
                for (int iX = 0; iX < m_iWidth; iX++)
                {
                    iValue = (qIntensityRow[iX] >> i) & 1;
                    iBytePos += Storage::WriteBits(qIffRow+iBytePos,iBitRatio,1,iValue);
                }

                COMPLETE_AND_SAVE_IFF_ROW;
            }

            PROCESS_MASKING;

            qProgress.MakeAdvance();
        }
    }
    else if ( m_eColorModel == CMT_RGB )
    {
        StillArray<UcRgb> qRgbRow(m_iWidth);
        ColorRgb qColorRgb;
        int iPixBytePos, iPixBitRatio;
        for (int iY = 0; iY < m_iHeight; iY++)
        {
            for (int iX = 0; iX < m_iWidth; iX++)
            {
                UcRgb& rqRgb = qRgbRow[iX];

                qColorRgb = m_spqImageSaveTo->GetPixelRgb(iX,iY);
                for (int i = 0; i < 3; i++)
                    rqRgb.Rgb[i] = Mathf::RoundToInt(qColorRgb[i]*255);
            }

            // IFF scanline
            for (int i = 0; i < m_iBitsPerPixel; i++)
            {
                // IFF row
                iBytePos = 0;
                iBitRatio = 0;
                iPixBytePos = i/8;
                iPixBitRatio = i & 7;
                for (int iX = 0; iX < m_iWidth; iX++)
                {
                    iValue = (qRgbRow[iX].Rgb[iPixBytePos] >> iPixBitRatio) & 1;
                    iBytePos += Storage::WriteBits(qIffRow+iBytePos,iBitRatio,1,iValue);
                }

                COMPLETE_AND_SAVE_IFF_ROW;
            }

            PROCESS_MASKING;

            qProgress.MakeAdvance();
        }
    }
    else  // m_eColorModel = CMT_RGBA
    {
        StillArray<UcRgba> qRgbaRow(m_iWidth);
        ColorRgba qColorRgba;
        int iPixBytePos, iPixBitRatio;
        for (int iY = 0; iY < m_iHeight; iY++)
        {
            for (int iX = 0; iX < m_iWidth; iX++)
            {
                UcRgba& rqRgba = qRgbaRow[iX];

                qColorRgba = m_spqImageSaveTo->GetPixelRgba(iX,iY);
                for (int i = 0; i < 4; i++)
                    rqRgba.Rgba[i] = Mathf::RoundToInt(qColorRgba[i]*255);
            }

            // IFF scanline
            for (int i = 0; i < m_iBitsPerPixel; i++)
            {
                // IFF row
                iBytePos = 0;
                iBitRatio = 0;
                iPixBytePos = i/8;
                iPixBitRatio = i & 7;
                for (int iX = 0; iX < m_iWidth; iX++)
                {
                    iValue = (qRgbaRow[iX].Rgba[iPixBytePos] >> iPixBitRatio) & 1;
                    iBytePos += Storage::WriteBits(qIffRow+iBytePos,iBitRatio,1,iValue);
                }

                COMPLETE_AND_SAVE_IFF_ROW;
            }

            qProgress.MakeAdvance();
        }
    }
    qIffRow.RemoveAll();

    // fix hole #2 and append the pad byte if needed
    iChunkDataSize = qSaveStream.GetBytePosition() - iBeforeImageDataPos;
    qSaveStream.PushPositionState();
    qSaveStream.SetBytePosition(iBeforeImageDataPos-4);
    qSaveStream.Write(iChunkDataSize);
    qSaveStream.PopPositionState();
    if ( (iChunkDataSize & 1) != 0 )
    {
        // pad byte
        qSaveStream.Write(cZero);
    }

    // fix hole #1
    int iFORMDataSize = qSaveStream.GetBytePosition() - 8;
    qSaveStream.PushPositionState();
    qSaveStream.SetBytePosition(4);
    qSaveStream.Write(iFORMDataSize);
    qSaveStream.PopPositionState();

    // flush to the file
    CONDITIONAL_THROW_EXCEPTION(qSaveStream.Finalize(),ET_CANNOT_ACCESS_FILE);

    return true;
}
//------------------------------------------------------------------------------------------------------------------
FormatIff::FormatIff (const char* acFilename, bool& rbSuccess)
    :
    FormatBase(acFilename),
    m_bHasDPIChunk(false),
    m_ePalettedImageSpecific(PIST_NONE)
{
    bool bSuccess;
    char acTypeId[4];

    // check identification bytes
    int iFileSize;
    C_CONDITIONAL_THROW_EXCEPTION(Storage::GetFileSize(iFileSize,m_qFilename),ET_CANNOT_ACCESS_FILE);
    C_CONDITIONAL_THROW_EXCEPTION_WITHOUT_ASSERT_FALSE(( iFileSize >= 12 ),ET_FORMAT_IDENTIFIER_MISMATCH);
    Stream qIBStream(m_eStreaming,m_qFilename,0,12,false,bSuccess);
    C_PROCESS_STREAM_SUCCESS_FOR_LOADING(bSuccess);
    qIBStream.Read(4,acTypeId);
    C_CONDITIONAL_THROW_EXCEPTION_WITHOUT_ASSERT_FALSE(( MemCmp(acTypeId,gs_acFORM,4) == 0 ),
        ET_FORMAT_IDENTIFIER_MISMATCH);
    qIBStream.OffsetBytePosition(4);
    qIBStream.Read(4,acTypeId);
    C_CONDITIONAL_THROW_EXCEPTION_WITHOUT_ASSERT_FALSE(( MemCmp(acTypeId,gs_acILBM,4) == 0 ),
        ET_FORMAT_IDENTIFIER_MISMATCH);
    qIBStream.Finalize();

    // reach BODY chunk reading informational chunks on the way
    int iChunkDataSize;
    unsigned int uiAmigaViewmodeFlags;
    bool bHasCMAPChunk = false;
    bool bHasCAMGChunk = false;
    m_iNextReadPos = 12;
    for (/**/; /**/; /**/)
    {
        // Type ID and Size
        Stream qTSStream(m_eStreaming,m_qFilename,m_iNextReadPos,8,false,bSuccess);
        C_PROCESS_STREAM_SUCCESS_FOR_LOADING(bSuccess);
        qTSStream.Read(4,acTypeId);
        qTSStream.Read(iChunkDataSize);
        C_CONDITIONAL_THROW_EXCEPTION(( iChunkDataSize >= 0 ),ET_FORMAT_MISMATCH);
        qTSStream.Finalize();

        if ( MemCmp(acTypeId,gs_acBODY,4) == 0 )
        {
            // has reached image data
            break;
        }

        m_iNextReadPos += 8;

        // Data
        Stream qChunkStream(m_eStreaming,m_qFilename,m_iNextReadPos,iChunkDataSize,false,bSuccess);
        C_PROCESS_STREAM_SUCCESS_FOR_LOADING(bSuccess);
        if ( MemCmp(acTypeId,gs_acBMHD,4) == 0 )
        {
            // image header
            C_CONDITIONAL_THROW_EXCEPTION(( iChunkDataSize == 20 ),ET_FORMAT_MISMATCH);

            unsigned short usWidth, usHeight;
            qChunkStream.Read(usWidth);
            qChunkStream.Read(usHeight);
            C_CONDITIONAL_THROW_EXCEPTION(( usWidth != 0 && usHeight != 0 ),ET_FORMAT_MISMATCH);
            m_iWidth = usWidth;
            m_iHeight = usHeight;

            qChunkStream.OffsetBytePosition(4);

            unsigned char ucPlanes;
            qChunkStream.Read(ucPlanes);
            C_CONDITIONAL_THROW_EXCEPTION(( 1 <= ucPlanes && ucPlanes <= 32 ),ET_FORMAT_MISMATCH);
            m_iBitPlaneQuantity = ucPlanes;
            m_iBitsPerPixel = m_iBitPlaneQuantity;

            unsigned char ucMasking;
            qChunkStream.Read(ucMasking);
            switch ( ucMasking )
            {
            case 0:
                m_eMasking = MT_NONE;
                break;
            case 1:
                m_eMasking = MT_HAS_MASK;
                C_CONDITIONAL_THROW_EXCEPTION(( m_iBitsPerPixel != 1 ),ET_FORMAT_MISMATCH);
                m_iBitsPerPixel--;
                break;
            case 2:
            case 3:
                THROW_WARNING(WT_UNSUPPORTED_MASKING_TECHNIQUE);
                m_eMasking = MT_NONE;
                break;
            default:
                C_THROW_EXCEPTION(ET_FORMAT_MISMATCH);
            }

            unsigned char ucCompression;
            qChunkStream.Read(ucCompression);
            switch ( ucCompression )
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
        }
        else if ( MemCmp(acTypeId,gs_acCMAP,4) == 0 )
        {
            // palette
            C_CONDITIONAL_THROW_EXCEPTION(( iChunkDataSize <= 768 && iChunkDataSize % 3 == 0 ),
                ET_FORMAT_MISMATCH);

            m_qPalette.SetQuantity(iChunkDataSize/3);
            for (int i = 0; i < m_qPalette.GetQuantity(); i++)
            {
                PaletteColor& rqPaletteColor = m_qPalette[i];

                qChunkStream.Read(rqPaletteColor.R());
                qChunkStream.Read(rqPaletteColor.G());
                qChunkStream.Read(rqPaletteColor.B());
            }
            bHasCMAPChunk = true;
        }
        else if ( MemCmp(acTypeId,gs_acCAMG,4) == 0 )
        {
            // Commodore Amiga specifics (not only)
            C_CONDITIONAL_THROW_EXCEPTION(( iChunkDataSize == 4 ),ET_FORMAT_MISMATCH);

            qChunkStream.Read(uiAmigaViewmodeFlags);
            bHasCAMGChunk = true;
        }
        else if ( MemCmp(acTypeId,gs_acDPI,4) == 0 )
        {
            // dots per inch
            if ( iChunkDataSize != 4 )
            {
                THROW_WARNING(WT_DPI_CHUNK_IS_INCORRECT);
                goto DoneWithThisChunk;
            }

            unsigned short usDpiX, usDpiY;
            qChunkStream.Read(usDpiX);
            qChunkStream.Read(usDpiY);
            m_iHorizontalDpi = usDpiX;
            m_iVerticalDpi = usDpiY;
            m_bHasDPIChunk = true;
        }
DoneWithThisChunk:

        qChunkStream.Finalize();

        m_iNextReadPos += iChunkDataSize;
        if ( (iChunkDataSize & 1) != 0 )
        {
            // count pad byte
            m_iNextReadPos++;
        }
    }

    // find out used color model
    if ( bHasCMAPChunk && m_iBitsPerPixel <= 8 )
    {
        m_eColorModel = CMT_PALETTED;
    }
    else
    {
        switch ( m_iBitsPerPixel )
        {
        case 8:
            m_eColorModel = CMT_GRAYSCALE;
            break;
        case 24:
            m_eColorModel = CMT_RGB;
            break;
        case 32:
            m_eColorModel = CMT_RGBA;
            break;
        default:
            C_THROW_EXCEPTION(ET_FORMAT_MISMATCH);
        }
    }

    if ( bHasCAMGChunk )
    {
        bool bHAM = ( (uiAmigaViewmodeFlags & 2048) != 0 );
        bool bEHB = ( (uiAmigaViewmodeFlags & 128) != 0 );
        if ( bHAM || bEHB )
        {
            C_CONDITIONAL_THROW_EXCEPTION(( !(bHAM && bEHB) ),ET_FORMAT_MISMATCH);
            C_CONDITIONAL_THROW_EXCEPTION(( m_eColorModel == CMT_PALETTED ),ET_FORMAT_MISMATCH);
            C_CONDITIONAL_THROW_EXCEPTION(( !(bHAM && m_iBitsPerPixel != 6 && m_iBitsPerPixel != 8) ),
                ET_FORMAT_MISMATCH);
            C_CONDITIONAL_THROW_EXCEPTION(( !(bEHB && m_qPalette.GetQuantity() != 32) ),ET_FORMAT_MISMATCH);
            if ( bHAM )
            {
                switch ( m_iBitsPerPixel )
                {
                case 6:
                    m_ePalettedImageSpecific = PIST_HAM;
                    break;
                case 8:
                    m_ePalettedImageSpecific = PIST_HAM_8;
                    break;
                }
            }
            else  // bEHB = true
            {
                m_ePalettedImageSpecific = PIST_EHB;

                // extend palette
                m_qPalette.SetQuantity(64,true);
                for (int i0 = 0, i1 = 32; i0 < 32; i0++, i1++)
                {
                    const PaletteColor& rqPaletteColor0 = m_qPalette[i0];
                    PaletteColor& rqPaletteColor1 = m_qPalette[i1];
                    for (int i2 = 0; i2 < 3; i2++)
                        rqPaletteColor1.Components[i2] = rqPaletteColor0.Components[i2] >> 1;
                }
            }
        }
    }

    rbSuccess = true;
}
//------------------------------------------------------------------------------------------------------------------
bool FormatIff::LoadImage (RasterImagePtr& rspqImage)
{
    bool bSuccess;

    // size of BODY chunk's data
    int iSize;
    Stream qSStream(m_eStreaming,m_qFilename,m_iNextReadPos+4,4,false,bSuccess);
    PROCESS_STREAM_SUCCESS_FOR_LOADING(bSuccess);
    m_iNextReadPos += 8;
    qSStream.Read(iSize);
    CONDITIONAL_THROW_EXCEPTION(( iSize >= 0 ),ET_FORMAT_MISMATCH);
    qSStream.Finalize();

    // get the data in uncompressed form
    StillArray<char> qUncompressedImageData;
    Stream qImageDataStream(m_eStreaming,m_qFilename,m_iNextReadPos,iSize,false,bSuccess);
    PROCESS_STREAM_SUCCESS_FOR_LOADING(bSuccess);
    if ( m_eCompression == CT_NONE )
    {
        qUncompressedImageData.SetQuantity(iSize);
        MemCpy(qUncompressedImageData,qImageDataStream,iSize);
    }
    else  // m_eCompression = CT_RLE
    {
        CONDITIONAL_THROW_EXCEPTION(CompressionPackBits::Decompress(qImageDataStream,iSize,qUncompressedImageData),
            ET_FORMAT_MISMATCH);
    }
    qImageDataStream.Finalize();

    // calculate expected data size and perform the check
    int iIffRowByteSize = ((m_iWidth + 15)/16)*2;
    int iIffScanlineByteSize = m_iBitPlaneQuantity*iIffRowByteSize;
    int iExpectedImageByteSize = m_iHeight*iIffScanlineByteSize;
    CONDITIONAL_THROW_EXCEPTION(( qUncompressedImageData.GetQuantity() == iExpectedImageByteSize ),
        ET_FORMAT_MISMATCH);

    // for a progress bar
    ProgressProbe qProgress(this);
    qProgress.SetAdvance(1.0f/m_iHeight);

    // read the image
    int iBytePos, iBitRatio, iSLVar, iValue;
    unsigned char* pucRow;
    if ( m_eColorModel == CMT_PALETTED )
    {
        if ( m_ePalettedImageSpecific == PIST_NONE
        ||   m_ePalettedImageSpecific == PIST_EHB )
        {
            // can be processed as a regular paletted IFF image
            rspqImage = new RasterImage(RasterImage::IT_PALETTED,m_iWidth,m_iHeight,m_qPalette);
            MemSet(*rspqImage,0,m_iHeight*rspqImage->GetRowStride());

            pucRow = (unsigned char*)rspqImage->GetImage();
            for (int iY = 0; iY < m_iHeight; iY++)
            {
                // IFF scanline
                iSLVar = iY*iIffScanlineByteSize;
                for (int i = 0; i < m_iBitsPerPixel; i++)
                {
                    // IFF row
                    iBytePos = iSLVar + i*iIffRowByteSize;
                    iBitRatio = 0;
                    for (int iX = 0; iX < m_iWidth; iX++)
                    {
                        iBytePos += Storage::ReadUBits(qUncompressedImageData+iBytePos,iBitRatio,1,iValue);
                        pucRow[iX] |= iValue << i;
                    }
                }

                for (int iX = 0; iX < m_iWidth; iX++)
                {
                    CONDITIONAL_THROW_EXCEPTION(( pucRow[iX] < rspqImage->GetPaletteSize() ),
                        ET_FORMAT_MISMATCH);
                }

                pucRow += rspqImage->GetRowStride();

                qProgress.MakeAdvance();
            }
        }
        else  // m_ePalettedImageSpecific = PIST_HAM or m_ePalettedImageSpecific = PIST_HAM_8
        {
            // HAM (Hold-and-Modify)
            rspqImage = new RasterImage(RasterImage::IT_RGB_8_8_8,m_iWidth,m_iHeight);

            int iRShift, iMask, iLShift, iReplShift;
            if ( m_ePalettedImageSpecific == PIST_HAM )
            {
                iRShift = 4;
                iMask = 15;
                iLShift = 4;
                iReplShift = 0;
            }
            else  // m_ePalettedImageSpecific = PIST_HAM_8
            {
                iRShift = 6;
                iMask = 63;
                iLShift = 2;
                iReplShift = 4;
            }
            StillArray<unsigned char> qCodeRow(m_iWidth);
            pucRow = (unsigned char*)rspqImage->GetImage();
            unsigned char* pucComp = pucRow;
            int iMode, iData;
            unsigned char aucPrevColor[3];
            for (int iY = 0; iY < m_iHeight; iY++)
            {
                MemSet(qCodeRow,0,m_iWidth);

                // IFF scanline
                iSLVar = iY*iIffScanlineByteSize;
                for (int i = 0; i < m_iBitsPerPixel; i++)
                {
                    // IFF row
                    iBytePos = iSLVar + i*iIffRowByteSize;
                    iBitRatio = 0;
                    for (int iX = 0; iX < m_iWidth; iX++)
                    {
                        iBytePos += Storage::ReadUBits(qUncompressedImageData+iBytePos,iBitRatio,1,iValue);
                        qCodeRow[iX] |= iValue << i;
                    }
                }

                for (int i = 0; i < 3; i++)
                    aucPrevColor[i] = 0;
                for (int iX = 0; iX < m_iWidth; iX++)
                {
                    iMode = qCodeRow[iX] >> iRShift;
                    iData = qCodeRow[iX] & iMask;
                    if ( iMode == 0 )
                    {
                        CONDITIONAL_THROW_EXCEPTION(( iData < m_qPalette.GetQuantity() ),ET_FORMAT_MISMATCH);
                        const PaletteColor& rqPaletteColor = m_qPalette[iData];
                        pucComp[0] = rqPaletteColor.R();
                        pucComp[1] = rqPaletteColor.G();
                        pucComp[2] = rqPaletteColor.B();
                    }
                    else if ( iMode == 1 )
                    {
                        pucComp[0] = aucPrevColor[0];
                        pucComp[1] = aucPrevColor[1];
                        pucComp[2] = (iData << iLShift) | (iData >> iReplShift);
                    }
                    else if ( iMode == 2 )
                    {
                        pucComp[0] = (iData << iLShift) | (iData >> iReplShift);
                        pucComp[1] = aucPrevColor[1];
                        pucComp[2] = aucPrevColor[2];
                    }
                    else  // iMode = 3
                    {
                        pucComp[0] = aucPrevColor[0];
                        pucComp[1] = (iData << iLShift) | (iData >> iReplShift);
                        pucComp[2] = aucPrevColor[2];
                    }

                    for (int i = 0; i < 3; i++)
                        aucPrevColor[i] = pucComp[i];

                    pucComp += 3;
                }

                pucRow += rspqImage->GetRowStride();
                pucComp = pucRow;

                qProgress.MakeAdvance();
            }
        }
    }
    else  // m_eColorModel = CMT_GRAYSCALE or m_eColorModel = CMT_RGB or m_eColorModel = CMT_RGBA
    {
        if ( m_eColorModel == CMT_GRAYSCALE )
            rspqImage = new RasterImage(RasterImage::IT_GRAYSCALE_8,m_iWidth,m_iHeight);
        else if ( m_eColorModel == CMT_RGB )
            rspqImage = new RasterImage(RasterImage::IT_RGB_8_8_8,m_iWidth,m_iHeight);
        else  // m_eColorModel = CMT_RGBA
            rspqImage = new RasterImage(RasterImage::IT_RGBA_8_8_8_8,m_iWidth,m_iHeight);
        MemSet(*rspqImage,0,m_iHeight*rspqImage->GetRowStride());

        pucRow = (unsigned char*)rspqImage->GetImage();
        unsigned char* aucPix = pucRow;
        int iPixBytePos, iPixBitRatio;
        for (int iY = 0; iY < m_iHeight; iY++)
        {
            // IFF scanline
            iSLVar = iY*iIffScanlineByteSize;
            for (int i = 0; i < m_iBitsPerPixel; i++)
            {
                // IFF row
                iBytePos = iSLVar + i*iIffRowByteSize;
                iBitRatio = 0;
                iPixBytePos = i/8;
                iPixBitRatio = i & 7;
                for (int iX = 0; iX < m_iWidth; iX++)
                {
                    iBytePos += Storage::ReadUBits(qUncompressedImageData+iBytePos,iBitRatio,1,iValue);
                    aucPix[iPixBytePos] |= iValue << iPixBitRatio;

                    aucPix += rspqImage->GetBytesPerPixel();
                }

                aucPix = pucRow;
            }

            pucRow += rspqImage->GetRowStride();
            aucPix = pucRow;

            qProgress.MakeAdvance();
        }
    }

    if ( m_eMasking == MT_HAS_MASK )
    {
        RasterImage::VisibilityMask qVisibilityMask(rspqImage->GetArea());
        char* pcCell = qVisibilityMask;
        int iRVar = m_iBitsPerPixel*iIffRowByteSize;
        for (int iY = 0; iY < m_iHeight; iY++)
        {
            iBytePos = iY*iIffScanlineByteSize + iRVar;
            iBitRatio = 0;
            for (int iX = 0; iX < m_iWidth; iX++)
            {
                iBytePos += Storage::ReadUBits(qUncompressedImageData+iBytePos,iBitRatio,1,iValue);
                *pcCell++ = iValue;
            }
        }
        rspqImage->SetVisibilityMask(qVisibilityMask);
    }

    return true;
}
//------------------------------------------------------------------------------------------------------------------



