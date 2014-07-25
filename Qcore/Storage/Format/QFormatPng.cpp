#include "QFormatPng.h"
#include "QExptWarnBase.mcr"
#include "QFormatBase.mcr"
#include "QFormatPng.mcr"
#include "QProgressProbe.h"
#include "QStorage.h"
#include "QSet.h"
#include "QSortUtils.h"
#include "QCompressionZLib.h"
using namespace Q;

// signature
static const unsigned char gs_aucSignature[8] = {137, 80, 78, 71, 13, 10, 26, 10};

// chunk types
static const unsigned char gs_auc_IHDR_Type[4] = { 73,  72,  68,  82};
static const unsigned char gs_auc_PLTE_Type[4] = { 80,  76,  84,  69};
static const unsigned char gs_auc_IDAT_Type[4] = { 73,  68,  65,  84};
static const unsigned char gs_auc_IEND_Type[4] = { 73,  69,  78,  68};
static const unsigned char gs_auc_cHRM_Type[4] = { 99,  72,  82,  77};
static const unsigned char gs_auc_gAMA_Type[4] = {103,  65,  77,  65};
static const unsigned char gs_auc_iCCP_Type[4] = {105,  67,  67,  80};
static const unsigned char gs_auc_sBIT_Type[4] = {115,  66,  73,  84};
static const unsigned char gs_auc_sRGB_Type[4] = {115,  82,  71,  66};
static const unsigned char gs_auc_bKGD_Type[4] = { 98,  75,  71,  68};
static const unsigned char gs_auc_hIST_Type[4] = {104,  73,  83,  84};
static const unsigned char gs_auc_tRNS_Type[4] = {116,  82,  78,  83};
static const unsigned char gs_auc_pHYs_Type[4] = {112,  72,  89, 115};
static const unsigned char gs_auc_sPLT_Type[4] = {115,  80,  76,  84};
static const unsigned char gs_auc_tIME_Type[4] = {116,  73,  77,  69};
static const unsigned char gs_auc_tEXt_Type[4] = {116,  69,  88, 116};
static const unsigned char gs_auc_zTXt_Type[4] = {122,  84,  88, 116};

// support for interlacing
static const int gs_aiPassStartW[7] = {0, 4, 0, 2, 0, 1, 0};
static const int gs_aiPassStartH[7] = {0, 0, 4, 0, 2, 0, 1};
static const int gs_aiPassStepW[7] = {8, 8, 4, 4, 2, 2, 1};
static const int gs_aiPassStepH[7] = {8, 8, 8, 4, 4, 2, 2};
static const int gs_aiBlockW[7] = {8, 4, 4, 2, 2, 1, 1};
static const int gs_aiBlockH[7] = {8, 8, 4, 4, 2, 2, 1};

// support for saving of textual information
const int FormatPng::ms_iMinimalTextLengthToBeCompressed(1024);

// if 'false', always the default ZLib strategy is used
const bool FormatPng::ms_bChooseBestFromManifoldZLibStrategies(true);

// support for loading of Latin1 strings
const bool FormatPng::ms_bCorrectLatin1Limited80StringsForLoading(true);

// support for loading of textual information
const bool FormatPng::ms_bCorrectTextsForLoading(true);

// support for finding of an unused color
class ColorRecord
{
public:
    ColorRecord () {};
    ColorRecord (unsigned short usR, unsigned short usG, unsigned short usB)
    {
        Components[0] = usR;
        Components[1] = usG;
        Components[2] = usB;
    };

    bool operator== (const ColorRecord& rqColor) const
    {
        return ( MemCmp(Components,rqColor.Components,6) == 0 );
    }
    bool operator< (const ColorRecord& rqColor) const
    {
        return ( MemCmp(Components,rqColor.Components,6) < 0 );
    }

    unsigned short Components[3];
};

// support for saving of suggested palettes
class SPEComparator
{
public:
    bool operator() (
        const FormatPng::SuggestedPaletteColor& rqSPE0,
        const FormatPng::SuggestedPaletteColor& rqSPE1) const
    {
        return ( rqSPE0.Frequency > rqSPE1.Frequency );
    }
};

//------------------------------------------------------------------------------------------------------------------
FormatPng::~FormatPng ()
{
    if ( m_pvZLib )
        delete (CompressionZLib*)m_pvZLib;
}
//------------------------------------------------------------------------------------------------------------------
FormatPng::FormatPng (const RasterImagePtr& rspqImage, int iBitsPerPixel)
    :
    m_spqImageSaveTo(rspqImage),
    m_iWidth(rspqImage->GetWidth()),
    m_iHeight(rspqImage->GetHeight()),
    m_eCompression(CT_DEFLATE_INFLATE),
    m_eFilterMethod(FMT_ADAPTIVE),
    m_eInterlaceMethod(IMT_NONE),
    m_eFilterSelectionMethod(FSM_MINIMUM_SUM_OF_ABSOLUTE_DIFFERENCES),
    m_eDataChunkSplitting(DCST_KILOBYTES_8),
    m_bHas_PLTE_Chunk(false),
    m_bHas_cHRM_Chunk(false),
    m_bHas_gAMA_Chunk(false),
    m_bHas_iCCP_Chunk(false),
    m_bHas_sBIT_Chunk(false),
    m_bHas_sRGB_Chunk(false),
    m_bHas_bKGD_Chunk(false),
    m_bHas_hIST_Chunk(false),
    m_bHas_tRNS_Chunk(false),
    m_bHas_pHYs_Chunk(true),
    m_iPixelsPerUnitX(2835),
    m_iPixelsPerUnitY(2835),
    m_ePhysicalPixelDimensionsUnit(PPDUT_METER),
    m_bHas_tIME_Chunk(false),
    m_pvZLib(0)
{
    assert( !(rspqImage->GetColorModel() != RasterImage::CMT_PALETTED
        &&  rspqImage->GetColorModel() != RasterImage::CMT_PALETTED_WITH_ALPHA && iBitsPerPixel == -1) );

    if ( iBitsPerPixel == -1 )
    {
        // find out the minimal possible bits-per-pixel value
        int iUsedPaletteSize = rspqImage->GetPaletteSize();
        if ( rspqImage->HasVisibilityMask() )
            iUsedPaletteSize++;  // since one extra palette entry will be required
        assert( iUsedPaletteSize <= 256 );
        iBitsPerPixel = Storage::GetUBitQuantity(iUsedPaletteSize-1);
        if ( iBitsPerPixel == 3 )
            iBitsPerPixel = 4;
        else if ( 4 < iBitsPerPixel && iBitsPerPixel < 8 )
            iBitsPerPixel = 8;
    }

    m_iBitsPerPixel = iBitsPerPixel;
}
//------------------------------------------------------------------------------------------------------------------
bool FormatPng::SaveImage (const char* acFilename)
{
    Stream qSaveStream(acFilename,false);
    if ( !WriteImageData(qSaveStream) )
        return false;

    // flush to the file
    CONDITIONAL_THROW_EXCEPTION(qSaveStream.Finalize(),ET_CANNOT_ACCESS_FILE);

    return true;
}
//------------------------------------------------------------------------------------------------------------------
bool FormatPng::SaveImage (StillArray<char>& rqStaticBytestream)
{
    rqStaticBytestream.RemoveAll();

    Stream qSaveStream(false);
    qSaveStream.SetDoNotFreeBufferAfterComplete();
    if ( !WriteImageData(qSaveStream) )
        return false;
    qSaveStream.Finalize();  // calls AlignBytes

    rqStaticBytestream.BecomeOwner((char*)qSaveStream.GetBytes(),qSaveStream.GetBytePosition());

    return true;
}
//------------------------------------------------------------------------------------------------------------------
FormatPng::FormatPng (const char* acFilename, bool& rbSuccess)
    :
    FormatBase(acFilename),
    m_pvZLib(0)
{
    rbSuccess = LoadHeadingData();
}
//------------------------------------------------------------------------------------------------------------------
FormatPng::FormatPng (bool bBytestreamIsStatic, const char* acBytestream, int iBytestreamSize, bool& rbSuccess)
    :
    FormatBase(bBytestreamIsStatic,acBytestream,( bBytestreamIsStatic ? iBytestreamSize : -1 )),
    m_pvZLib(0)
{
    if ( !bBytestreamIsStatic )
    {
        m_iDevelopedRowQuantity = 0;
        m_iCurrentPassIndex = 0;
        m_eInterruptionPoint = IPT_LENGTH_FIELD;
        m_bDoneWithTheImage = false;
        m_iDecompressedByteQuantity = 0;
        m_iY = 0;
        m_iPass = 0;
        m_iYInPass = 0;
    }

    rbSuccess = LoadHeadingData(iBytestreamSize);
}
//------------------------------------------------------------------------------------------------------------------
bool FormatPng::LoadImage (RasterImagePtr& rspqImage, int iDynamicBytestreamSize)
{
    assert( ( m_eStreaming == Stream::ST_READING_FROM_DYNAMIC_BYTESTREAM ) == ( iDynamicBytestreamSize >= 0 ) );

    int iBytestreamSize = ( m_eStreaming == Stream::ST_READING_FROM_STATIC_BYTESTREAM ? m_iStaticBytestreamSize :
        iDynamicBytestreamSize );

    if ( !m_bImageIsInitialized )
    {
        // allocate the image
        if ( m_eColorModel == CMT_GRAYSCALE )
        {
            if ( m_iBitDepth <= 8 )
                rspqImage = new RasterImage(RasterImage::IT_GRAYSCALE_8,m_iWidth,m_iHeight);
            else  // m_iBitDepth = 16
                rspqImage = new RasterImage(RasterImage::IT_GRAYSCALE_16,m_iWidth,m_iHeight);
        }
        else if ( m_eColorModel == CMT_RGB )
        {
            if ( m_iBitDepth == 8 )
                rspqImage = new RasterImage(RasterImage::IT_RGB_8_8_8,m_iWidth,m_iHeight);
            else  // m_iBitDepth = 16
                rspqImage = new RasterImage(RasterImage::IT_RGB_16_16_16,m_iWidth,m_iHeight);
        }
        else if ( m_eColorModel == CMT_INDEXED )
        {
            // sBIT chunk can alter palette entries
            if ( m_bHas_sBIT_Chunk )
            {
                for (int i = 0; i < m_qPalette.GetQuantity(); i++)
                {
                    PaletteColor& rqPalC = m_qPalette[i];

                    rqPalC.R() = Mathf::RoundToInt(((float)(rqPalC.R() >> m_iShiftVRed))/m_iMaxVRed*255);
                    rqPalC.G() = Mathf::RoundToInt(((float)(rqPalC.G() >> m_iShiftVGreen))/m_iMaxVGreen*255);
                    rqPalC.B() = Mathf::RoundToInt(((float)(rqPalC.B() >> m_iShiftVBlue))/m_iMaxVBlue*255);
                }
            }

            if ( !m_bHas_tRNS_Chunk )
            {
                rspqImage = new RasterImage(RasterImage::IT_PALETTED,m_iWidth,m_iHeight,m_qPalette);
            }
            else
            {
                // merge the palettes
                for (int i = 0; i < m_qPalette.GetQuantity(); i++)
                    m_qPalette[i].A() = ( i < m_qPaletteAlphaAddon.GetQuantity() ? m_qPaletteAlphaAddon[i] : 255 );

                rspqImage = new RasterImage(RasterImage::IT_PALETTED_WITH_ALPHA,m_iWidth,m_iHeight,
                    m_qPalette);
            }
        }
        else if ( m_eColorModel == CMT_GRAYSCALE_WITH_ALPHA )
        {
            if ( m_iBitDepth == 8 )
                rspqImage = new RasterImage(RasterImage::IT_GRAYSCALE_WITH_ALPHA_8_8,m_iWidth,m_iHeight);
            else  // m_iBitDepth = 16
                rspqImage = new RasterImage(RasterImage::IT_GRAYSCALE_WITH_ALPHA_16_16,m_iWidth,m_iHeight);
        }
        else  // m_eColorModel = CMT_RGBA
        {
            if ( m_iBitDepth == 8 )
                rspqImage = new RasterImage(RasterImage::IT_RGBA_8_8_8_8,m_iWidth,m_iHeight);
            else  // m_iBitDepth = 16
                rspqImage = new RasterImage(RasterImage::IT_RGBA_16_16_16_16,m_iWidth,m_iHeight);
        }
        if ( m_bHas_tRNS_Chunk && m_eColorModel != CMT_INDEXED )
        {
            // allocate the visibility mask
            rspqImage->GetVisibilityMask().SetQuantity(m_iWidth*m_iHeight);
        }

        // some derived values
        if ( m_eColorModel != CMT_INDEXED )
        {
            m_iBytesPerImgSample = rspqImage->GetBitsPerIntegerSample()/8;
            m_iImgSampleMaxValue = (1 << rspqImage->GetBitsPerIntegerSample()) - 1;
        }
        else
        {
            m_iBytesPerImgSample = 1;
            m_iImgSampleMaxValue = -1;
        }

        m_bImageIsInitialized = true;
    }

    bool bSuccess;

    // for a progress bar
    float fAlreadyDone;
    if ( m_eStreaming != Stream::ST_READING_FROM_DYNAMIC_BYTESTREAM )
    {
        fAlreadyDone = 0.0f;
    }
    else
    {
        if ( m_eInterlaceMethod == IMT_NONE )
            fAlreadyDone = ((float)m_iDevelopedRowQuantity)/m_iHeight;
        else  // m_eInterlaceMethod = IMT_ADAM_7
            fAlreadyDone = ((float)m_iCurrentPassIndex + ((float)m_iDevelopedRowQuantity)/m_iHeight)/7;
    }
    ProgressProbe qProgress(this,fAlreadyDone);

    // actual image
    unsigned int uiLength, uiTDCLength, uiCrc;
    char acType[4];
    if ( m_eStreaming != Stream::ST_READING_FROM_DYNAMIC_BYTESTREAM )
    {
        // process IDAT chunks to retrieve compressed data
        StillArray<char> qCompressedData;
        int iPrevQ;
        for (/**/; /**/; /**/)
        {
            Stream qLengthStream(m_eStreaming,m_acStreamSrcParam,m_iNextReadPos,4,false,bSuccess,iBytestreamSize);
            PROCESS_STREAM_SUCCESS_FOR_LOADING(bSuccess);
            m_iNextReadPos += 4;
            qLengthStream.Read(uiLength);
            qLengthStream.Finalize();

            uiTDCLength = uiLength + 8;
            Stream qChunkStream(m_eStreaming,m_acStreamSrcParam,m_iNextReadPos,uiTDCLength,false,bSuccess,
                iBytestreamSize);
            PROCESS_STREAM_SUCCESS_FOR_LOADING(bSuccess);
            m_iNextReadPos += uiTDCLength;

            qChunkStream.Read(4,acType);
            if ( MemCmp(acType,gs_auc_IDAT_Type,4) != 0 )
            {
                m_iNextReadPos -= 4 + uiTDCLength;
                break;
            }

            // CRC
            qChunkStream.OffsetBytePosition(uiLength);
            qChunkStream.Read(uiCrc);
            CONDITIONAL_THROW_EXCEPTION(( uiCrc == Storage::GetCrc32Checksum(qChunkStream,4+uiLength) ),
                ET_FORMAT_MISMATCH);
            qChunkStream.SetBytePosition(4);

            iPrevQ = qCompressedData.GetQuantity();
            qCompressedData.AdvanceQuantity(uiLength);
            MemCpy(qCompressedData+iPrevQ,qChunkStream.GetBytesUpPosition(),uiLength);

            qChunkStream.Finalize();
        }

        // unpack
        StillArray<char> qDecompressedData;
        CompressionZLib qZLib;
        CONDITIONAL_THROW_EXCEPTION(
            qZLib.Decompress(qCompressedData,qCompressedData.GetQuantity(),qDecompressedData),
            ET_ZLIB_EXCEPTION);
        qCompressedData.RemoveAll();

        // check decompressed data size
        CONDITIONAL_THROW_EXCEPTION(( qDecompressedData.GetQuantity() == m_iExpectedImageSize ),
            ET_FORMAT_MISMATCH);

        qProgress.SetDone(0.5f);

        // read the image
        int iBytePos = 0;
        int iBitRatio = 0;
        unsigned char* pucRow;
        if ( m_eInterlaceMethod == IMT_NONE )
        {
            qProgress.SetAdvance(0.5f/m_iHeight);

            // read pixels
            for (int iY = 0; iY < m_iHeight; iY++)
            {
                // unfilter
                pucRow = (unsigned char*)(qDecompressedData + iBytePos);
                UnfilterRow(iY,pucRow,pucRow-m_iRowSizeWithFB,m_iRowSizeWithFB);

                iBytePos++;
                for (int iX = 0; iX < m_iWidth; iX++)
                    ReadPixel(qDecompressedData,iBytePos,iBitRatio,rspqImage,iX,iY);

                if ( iBitRatio != 0 )
                {
                    iBytePos++;
                    iBitRatio = 0;
                }

                qProgress.MakeAdvance();
            }
        }
        else  // m_eInterlaceMethod = IMT_ADAM_7
        {
            qProgress.SetAdvance(0.5f/7);

            // read pixels
            int iX, iY;
            for (int i = 0; i < 7; i++)
            {
                if ( m_aiPassSize[i] != 0 )
                {
                    for (int i0 = 0; i0 < m_aiHeightInPass[i]; i0++)
                    {
                        iY = gs_aiPassStartH[i] + i0*gs_aiPassStepH[i];

                        // unfilter
                        pucRow = (unsigned char*)(qDecompressedData + iBytePos);
                        UnfilterRow(i0,pucRow,pucRow-m_aiRowSizeWithFBInPass[i],m_aiRowSizeWithFBInPass[i]);

                        iBytePos++;
                        for (int i1 = 0; i1 < m_aiWidthInPass[i]; i1++)
                        {
                            iX = gs_aiPassStartW[i] + i1*gs_aiPassStepW[i];

                            ReadPixel(qDecompressedData,iBytePos,iBitRatio,rspqImage,iX,iY);
                        }

                        if ( iBitRatio != 0 )
                        {
                            iBytePos++;
                            iBitRatio = 0;
                        }
                    }
                }

                qProgress.MakeAdvance();
            }
        }
        qDecompressedData.RemoveAll();
    }
    else
    {
        if ( m_bDoneWithTheImage )
        {
            m_iNextReadPos = m_iAfterImagePos;
            goto DoneWithTheImage;
        }

        for (/**/; /**/; /**/)
        {
            if ( m_eInterruptionPoint == IPT_LENGTH_FIELD )
            {
                Stream qLengthStream(m_eStreaming,m_acStreamSrcParam,m_iNextReadPos,4,false,bSuccess,
                    iBytestreamSize);
                PROCESS_STREAM_SUCCESS_FOR_LOADING(bSuccess);
                m_iNextReadPos += 4;
                qLengthStream.Read(uiLength);
                qLengthStream.Finalize();

                m_iCurrentChunkLength = uiLength;
                m_iCurrentChunkDataBeginningPos = m_iNextReadPos + 4;
                m_eInterruptionPoint = IPT_TYPE_FIELD;
            }

            if ( m_eInterruptionPoint == IPT_TYPE_FIELD )
            {
                Stream qTypeStream(m_eStreaming,m_acStreamSrcParam,m_iNextReadPos,4,false,bSuccess,
                    iBytestreamSize);
                PROCESS_STREAM_SUCCESS_FOR_LOADING(bSuccess);
                m_iNextReadPos += 4;
                qTypeStream.Read(4,acType);
                qTypeStream.Finalize();

                if ( MemCmp(acType,gs_auc_IDAT_Type,4) != 0 )
                {
                    m_qCompressedData.RemoveAll();
                    m_qDecompressedRowData.RemoveAll();
                    m_qDecompressedUpRowData.RemoveAll();
                    if ( m_pvZLib )
                    {
                        delete (CompressionZLib*)m_pvZLib;
                        m_pvZLib = 0;
                    }
                    m_iNextReadPos -= 8;
                    m_iAfterImagePos = m_iNextReadPos;
                    m_bDoneWithTheImage = true;
                    goto DoneWithTheImage;
                }

                m_eInterruptionPoint = IPT_DATA;
            }

            if ( m_eInterruptionPoint == IPT_DATA )
            {
                int iBytesRead = m_iNextReadPos - m_iCurrentChunkDataBeginningPos;
                int iBytesToRead = iBytestreamSize - m_iNextReadPos;
                if ( iBytesToRead == 0 )
                    return false;
                if ( iBytesRead + iBytesToRead > m_iCurrentChunkLength )
                    iBytesToRead = m_iCurrentChunkLength - iBytesRead;

                // get the next portion of image data
                Stream qPortionStream(m_eStreaming,m_acStreamSrcParam,m_iNextReadPos,iBytesToRead,false,bSuccess,
                    iBytestreamSize);
                m_iNextReadPos += iBytesToRead;
                int iPrevQ = m_qCompressedData.GetQuantity();
                m_qCompressedData.AdvanceQuantity(iBytesToRead);
                MemCpy(m_qCompressedData+iPrevQ,qPortionStream,iBytesToRead);
                qPortionStream.Finalize();

                // unpack
                if ( !m_pvZLib )
                {
                    m_pvZLib = new CompressionZLib();
                    ((CompressionZLib*)m_pvZLib)->SetDynamicalDecompressionMode();
                }
                iPrevQ = m_qDecompressedRowData.GetQuantity();
                CONDITIONAL_THROW_EXCEPTION(
                    ((CompressionZLib*)m_pvZLib)->Decompress(m_qCompressedData,m_qCompressedData.GetQuantity(),
                    m_qDecompressedRowData),ET_ZLIB_EXCEPTION);
                m_iDecompressedByteQuantity += m_qDecompressedRowData.GetQuantity() - iPrevQ;

                CONDITIONAL_THROW_EXCEPTION(( m_iDecompressedByteQuantity <= m_iExpectedImageSize ),
                    ET_FORMAT_MISMATCH);

                // read the image portion
                int iBytePos, iBitRatio, iSuffQ;
                if ( m_eInterlaceMethod == IMT_NONE )
                {
                    for (/**/; m_iY < m_iHeight; m_iY++)
                    {
                        if ( m_qDecompressedRowData.GetQuantity() < m_iRowSizeWithFB )
                            goto DoneWithThePortion;

                        // unfilter
                        UnfilterRow(m_iY,(unsigned char*)m_qDecompressedRowData.GetElements(),
                            (unsigned char*)m_qDecompressedUpRowData.GetElements(),m_iRowSizeWithFB);

                        iBytePos = 1;
                        iBitRatio = 0;
                        for (int iX = 0; iX < m_iWidth; iX++)
                            ReadPixel(m_qDecompressedRowData,iBytePos,iBitRatio,rspqImage,iX,m_iY);

                        m_qDecompressedUpRowData = m_qDecompressedRowData;

                        iSuffQ = m_qDecompressedRowData.GetQuantity() - m_iRowSizeWithFB;
                        StillArray<char> qSuffixSave(iSuffQ);
                        MemCpy(qSuffixSave,m_qDecompressedRowData+m_iRowSizeWithFB,iSuffQ);
                        m_qDecompressedRowData.SetQuantity(iSuffQ,false);
                        MemCpy(m_qDecompressedRowData,qSuffixSave,iSuffQ);

                        m_iDevelopedRowQuantity++;

                        qProgress.SetDone(((float)m_iDevelopedRowQuantity)/m_iHeight);
                    }
                }
                else  // m_eInterlaceMethod = IMT_ADAM_7
                {
                    int iX, iY, iVMValue;
                    StillArray<char> qPixel(rspqImage->GetBytesPerPixel());
                    for (/**/; m_iPass < 7; m_iPass++)
                    {
                        if ( m_aiPassSize[m_iPass] != 0 )
                        {
                            for (/**/; m_iYInPass < m_aiHeightInPass[m_iPass]; m_iYInPass++)
                            {
                                if ( m_qDecompressedRowData.GetQuantity() < m_aiRowSizeWithFBInPass[m_iPass] )
                                    goto DoneWithThePortion;

                                iY = gs_aiPassStartH[m_iPass] + m_iYInPass*gs_aiPassStepH[m_iPass];

                                // unfilter
                                UnfilterRow(m_iYInPass,(unsigned char*)m_qDecompressedRowData.GetElements(),
                                    (unsigned char*)m_qDecompressedUpRowData.GetElements(),
                                    m_aiRowSizeWithFBInPass[m_iPass]);

                                iBytePos = 1;
                                iBitRatio = 0;
                                for (int iXInPass = 0; iXInPass < m_aiWidthInPass[m_iPass]; iXInPass++)
                                {
                                    iX = gs_aiPassStartW[m_iPass] + iXInPass*gs_aiPassStepW[m_iPass];

                                    ReadPixel(m_qDecompressedRowData,iBytePos,iBitRatio,rspqImage,iX,iY);

                                    // draw the rectangle
                                    MemCpy(qPixel,
                                        rspqImage->GetRow(iY)+iX*rspqImage->GetBytesPerPixel(),
                                        rspqImage->GetBytesPerPixel());
                                    for (int i0 = 0, iRY = iY; i0 < gs_aiBlockH[m_iPass]; i0++, iRY++)
                                    {
                                        for (int i1 = 0, iRX = iX; i1 < gs_aiBlockW[m_iPass]; i1++, iRX++)
                                        {
                                            if ( iRX < m_iWidth && iRY < m_iHeight )
                                            {
                                                MemCpy(
                                                    rspqImage->GetRow(iRY)+
                                                    iRX*rspqImage->GetBytesPerPixel(),qPixel,
                                                    rspqImage->GetBytesPerPixel());
                                            }
                                        }
                                    }

                                    if ( rspqImage->HasVisibilityMask() )
                                    {
                                        // do similar for the visibility mask
                                        iVMValue = rspqImage->GetVisibilityMask()[iY*m_iWidth+iX];
                                        for (int i0 = 0, iRY = iY; i0 < gs_aiBlockH[m_iPass]; i0++, iRY++)
                                        {
                                            for (int i1 = 0, iRX = iX; i1 < gs_aiBlockW[m_iPass]; i1++, iRX++)
                                            {
                                                if ( iRX < m_iWidth && iRY < m_iHeight )
                                                {
                                                    rspqImage->GetVisibilityMask()[iRY*m_iWidth+iRX] =
                                                        iVMValue;
                                                }
                                            }
                                        }
                                    }
                                }

                                m_qDecompressedUpRowData = m_qDecompressedRowData;

                                iSuffQ = m_qDecompressedRowData.GetQuantity() - m_aiRowSizeWithFBInPass[m_iPass];
                                StillArray<char> qSuffixSave(iSuffQ);
                                MemCpy(qSuffixSave,m_qDecompressedRowData+m_aiRowSizeWithFBInPass[m_iPass],iSuffQ);
                                m_qDecompressedRowData.SetQuantity(iSuffQ,false);
                                MemCpy(m_qDecompressedRowData,qSuffixSave,iSuffQ);

                                m_iDevelopedRowQuantity = iY + gs_aiBlockH[m_iPass];
                                if ( m_iDevelopedRowQuantity > m_iHeight )
                                    m_iDevelopedRowQuantity = m_iHeight;

                                qProgress.SetDone(
                                    ((float)m_iCurrentPassIndex+((float)m_iDevelopedRowQuantity)/m_iHeight)/7);
                            }
                            m_iYInPass = 0;
                        }

                        if ( m_iPass != 6 )
                        {
                            m_iCurrentPassIndex++;
                            m_iDevelopedRowQuantity = 0;
                        }
                        else
                        {
                            m_iDevelopedRowQuantity = m_iHeight;
                        }
                    }
                }

                // check decompressed data size
                CONDITIONAL_THROW_EXCEPTION(( m_iDecompressedByteQuantity == m_iExpectedImageSize ),
                    ET_FORMAT_MISMATCH);
DoneWithThePortion:

                if ( iBytesRead + iBytesToRead != m_iCurrentChunkLength )
                {
                    return false;
                }
                else
                {
                    // have completed the chunk
                    m_eInterruptionPoint = IPT_CRC_FIELD;
                }
            }

            if ( m_eInterruptionPoint == IPT_CRC_FIELD )
            {
                Stream qCrcStream(m_eStreaming,m_acStreamSrcParam,m_iNextReadPos,4,false,bSuccess,iBytestreamSize);
                PROCESS_STREAM_SUCCESS_FOR_LOADING(bSuccess);
                m_iNextReadPos += 4;
                qCrcStream.Read(uiCrc);
                qCrcStream.Finalize();

                Stream qCheckStream(m_eStreaming,m_acStreamSrcParam,m_iCurrentChunkDataBeginningPos-4,
                    m_iCurrentChunkLength+4,false,bSuccess,iBytestreamSize);
                CONDITIONAL_THROW_EXCEPTION(
                    ( uiCrc == Storage::GetCrc32Checksum(qCheckStream,qCheckStream.GetByteQuantity()) ),
                    ET_FORMAT_MISMATCH);
                qCheckStream.Finalize();

                m_eInterruptionPoint = IPT_LENGTH_FIELD;
            }
        }
    }
DoneWithTheImage:

    // reach the IEND chunk collecting chunks which can appear after image data
    bool bNullFound;
    int iNullPos, iVar;
    const char* pcNullSearchStart;
    const char* pcChar;
    unsigned char ucChar;
    for (/**/; /**/; /**/)
    {
        Stream qLengthStream(m_eStreaming,m_acStreamSrcParam,m_iNextReadPos,4,false,bSuccess,iBytestreamSize);
        PROCESS_STREAM_SUCCESS_FOR_LOADING(bSuccess);
        m_iNextReadPos += 4;
        qLengthStream.Read(uiLength);
        qLengthStream.Finalize();

        uiTDCLength = uiLength + 8;
        Stream qChunkStream(m_eStreaming,m_acStreamSrcParam,m_iNextReadPos,uiTDCLength,false,bSuccess,
            iBytestreamSize);
        PROCESS_STREAM_SUCCESS_FOR_LOADING(bSuccess);
        m_iNextReadPos += uiTDCLength;

        qChunkStream.Read(4,acType);

        if ( MemCmp(acType,gs_auc_IEND_Type,4) == 0 )
            break;

        if ( MemCmp(acType,gs_auc_tIME_Type,4) == 0 )
        {
            // image last-modification time
            READ_tIME_CHUNK;
        }
        else if ( MemCmp(acType,gs_auc_tEXt_Type,4) == 0 )
        {
            // textual data
            READ_tEXt_CHUNK;
        }
        else if ( MemCmp(acType,gs_auc_zTXt_Type,4) == 0 )
        {
            // textual data
            READ_zTXt_CHUNK;
        }
DoneWithThisChunk:

        qChunkStream.Finalize();
    }

    return true;
}
//------------------------------------------------------------------------------------------------------------------
bool FormatPng::WriteImageData (Stream& rqStream)
{
    assert( m_spqImageSaveTo->GetWidth() != 0 && m_spqImageSaveTo->GetHeight() != 0 );
    assert( (m_spqImageSaveTo->GetColorModel() == RasterImage::CMT_GRAYSCALE
        &&  (m_iBitsPerPixel == 1 || m_iBitsPerPixel == 2 || m_iBitsPerPixel == 4 || m_iBitsPerPixel == 8
        ||  m_iBitsPerPixel == 16))
        ||  (m_spqImageSaveTo->GetColorModel() == RasterImage::CMT_RGB
        &&  (m_iBitsPerPixel == 24 || m_iBitsPerPixel == 48))
        ||  ((m_spqImageSaveTo->GetColorModel() == RasterImage::CMT_PALETTED
        ||  m_spqImageSaveTo->GetColorModel() == RasterImage::CMT_PALETTED_WITH_ALPHA)
        &&  (m_iBitsPerPixel == 1 || m_iBitsPerPixel == 2 || m_iBitsPerPixel == 4 || m_iBitsPerPixel == 8))
        ||  (m_spqImageSaveTo->GetColorModel() == RasterImage::CMT_GRAYSCALE_WITH_ALPHA
        &&  (m_iBitsPerPixel == 16 || m_iBitsPerPixel == 32))
        ||  (m_spqImageSaveTo->GetColorModel() == RasterImage::CMT_RGBA
        &&  (m_iBitsPerPixel == 32 || m_iBitsPerPixel == 64)) );
    assert( !(m_spqImageSaveTo->GetColorModel() == RasterImage::CMT_PALETTED
        &&  m_spqImageSaveTo->HasVisibilityMask()
        &&  m_spqImageSaveTo->GetPaletteSize() == 256) );
    assert( !((m_spqImageSaveTo->GetColorModel() == RasterImage::CMT_PALETTED
        ||  m_spqImageSaveTo->GetColorModel() == RasterImage::CMT_PALETTED_WITH_ALPHA)
        &&  ((m_iBitsPerPixel == 1 && m_spqImageSaveTo->GetPaletteSize() > 2)
        ||  (m_iBitsPerPixel == 2 && m_spqImageSaveTo->GetPaletteSize() > 4)
        ||  (m_iBitsPerPixel == 4 && m_spqImageSaveTo->GetPaletteSize() > 16))) );
    assert( !(m_bHas_iCCP_Chunk && m_bHas_sRGB_Chunk) );

    // color type and derived attributes
    switch ( m_spqImageSaveTo->GetColorModel() )
    {
    case RasterImage::CMT_PALETTED:
    case RasterImage::CMT_PALETTED_WITH_ALPHA:
        m_eColorModel = CMT_INDEXED;
        m_iSamplesPerPixel = 1;
        break;
    case RasterImage::CMT_GRAYSCALE:
        m_eColorModel = CMT_GRAYSCALE;
        m_iSamplesPerPixel = 1;
        break;
    case RasterImage::CMT_GRAYSCALE_WITH_ALPHA:
        m_eColorModel = CMT_GRAYSCALE_WITH_ALPHA;
        m_iSamplesPerPixel = 2;
        break;
    case RasterImage::CMT_RGB:
        m_eColorModel = CMT_RGB;
        m_iSamplesPerPixel = 3;
        break;
    case RasterImage::CMT_RGBA:
        m_eColorModel = CMT_RGBA;
        m_iSamplesPerPixel = 4;
        break;
    }
    m_iBitDepth = m_iBitsPerPixel/m_iSamplesPerPixel;
    m_iSampleRange = 1 << m_iBitDepth;
    m_iSampleMaxValue = m_iSampleRange - 1;

    // will be used instead, pointing to the original image or its altered copy
    RasterImagePtr spqUseImage = m_spqImageSaveTo;

    // things related to tRNS chunk
    if ( m_spqImageSaveTo->GetColorModel() == RasterImage::CMT_PALETTED
    ||   m_spqImageSaveTo->GetColorModel() == RasterImage::CMT_PALETTED_WITH_ALPHA )
    {
        m_qPalette = m_spqImageSaveTo->GetPalette();
        m_bHas_PLTE_Chunk = true;

        if ( m_spqImageSaveTo->HasVisibilityMask() )
        {
            spqUseImage = m_spqImageSaveTo->GetDuplicate();

            // increase palette size
            m_qPalette.SetQuantity(m_spqImageSaveTo->GetPaletteSize()+1,false);
            m_qPalette[0] = PaletteColor(0,0,0);
            for (int i = 0; i < m_spqImageSaveTo->GetPaletteSize(); i++)
                m_qPalette[i+1] = m_spqImageSaveTo->GetPaletteColor(i);

            spqUseImage->SetPalette(m_qPalette);

            if ( m_bHas_hIST_Chunk )
            {
                // need to do it as well
                StillArray<int> qHistSave = m_qHistogramFrequencies;
                m_qHistogramFrequencies.SetQuantity(m_qPalette.GetQuantity(),false);
                m_qHistogramFrequencies[0] = 0;
                for (int i = 0; i < qHistSave.GetQuantity(); i++)
                    m_qHistogramFrequencies[i+1] = qHistSave[i];
            }

            // alternation of bpp may now be required
            m_iBitsPerPixel = Storage::GetUBitQuantity(m_qPalette.GetQuantity()-1);
            if ( 2 < m_iBitsPerPixel && m_iBitsPerPixel < 4 )
                m_iBitsPerPixel = 4;
            else if ( 4 < m_iBitsPerPixel && m_iBitsPerPixel < 8 )
                m_iBitsPerPixel = 8;

            // reassign/shift indices
            unsigned char* pucRow = (unsigned char*)spqUseImage->GetImage();
            unsigned char* pucIndex = pucRow;
            for (int iY = 0; iY < m_iHeight; iY++)
            {
                for (int iX = 0; iX < m_iWidth; iX++)
                {
                    if ( m_spqImageSaveTo->GetVisibilityMask()[iY*m_iWidth+iX] == 0 )
                        *pucIndex = 0;
                    else
                        (*pucIndex)++;

                    pucIndex++;
                }

                pucRow += spqUseImage->GetRowStride();
                pucIndex = pucRow;
            }

            // alpha value
            m_qPaletteAlphaAddon.SetQuantity(1);
            m_qPaletteAlphaAddon[0] = 0;
            m_bHas_tRNS_Chunk = true;
        }

        if ( m_spqImageSaveTo->GetColorModel() == RasterImage::CMT_PALETTED_WITH_ALPHA )
        {
            // alpha values
            m_qPaletteAlphaAddon.SetQuantity(m_qPalette.GetQuantity());
            for (int i = 0; i < m_qPalette.GetQuantity(); i++)
                m_qPaletteAlphaAddon[i] = m_qPalette[i].A();
            m_bHas_tRNS_Chunk = true;
        }
    }
    else if ( m_spqImageSaveTo->GetColorModel() == RasterImage::CMT_GRAYSCALE )
    {
        if ( m_spqImageSaveTo->HasVisibilityMask() )
        {
            // find an unused grayscale value
            Set<unsigned short> qUsedValues;
            for (int iX = 0; iX < m_iWidth; iX++)
            {
                for (int iY = 0; iY < m_iHeight; iY++)
                {
                    qUsedValues.Insert(
                        Mathf::RoundToInt(m_spqImageSaveTo->GetPixelIntensity(iX,iY)*m_iSampleMaxValue));
                }
            }
            bool bFound = false;
            int iGray;
            for (iGray = 0; iGray < m_iSampleRange; iGray++)
            {
                if ( !qUsedValues.IsPresent(iGray) )
                {
                    bFound = true;
                    break;
                }
            }

            CONDITIONAL_THROW_EXCEPTION(bFound,
                ET_VISIBILITY_MASK_SIMULTANEOUSLY_WITH_ALL_POSSIBLE_COLOR_VALUES_USED);

            m_iTransparencyGrayscale = iGray;
            m_bHas_tRNS_Chunk = true;
        }
    }
    else if ( m_spqImageSaveTo->GetColorModel() == RasterImage::CMT_RGB )
    {
        if ( m_spqImageSaveTo->HasVisibilityMask() )
        {
            // find an unused color
            Set<ColorRecord> qUsedColors;
            ColorRgb qColor;
            for (int iX = 0; iX < m_iWidth; iX++)
            {
                for (int iY = 0; iY < m_iHeight; iY++)
                {
                    qColor = m_spqImageSaveTo->GetPixelRgb(iX,iY);
                    qUsedColors.Insert(ColorRecord(
                        Mathf::RoundToInt(qColor.R()*m_iSampleMaxValue),
                        Mathf::RoundToInt(qColor.G()*m_iSampleMaxValue),
                        Mathf::RoundToInt(qColor.B()*m_iSampleMaxValue)));
                }
            }
            bool bFound = false;
            int iR, iG, iB;
            for (iR = 0; iR < m_iSampleRange; iR++)
            {
                for (iG = 0; iG < m_iSampleRange; iG++)
                {
                    for (iB = 0; iB < m_iSampleRange; iB++)
                    {
                        if ( !qUsedColors.IsPresent(ColorRecord(iR,iG,iB)) )
                        {
                            bFound = true;
                            goto Exit;
                        }
                    }
                }
            }
Exit:

            CONDITIONAL_THROW_EXCEPTION(bFound,
                ET_VISIBILITY_MASK_SIMULTANEOUSLY_WITH_ALL_POSSIBLE_COLOR_VALUES_USED);

            m_iTransparencyRed = iR;
            m_iTransparencyGreen = iG;
            m_iTransparencyBlue = iB;
            m_bHas_tRNS_Chunk = true;
        }
    }

    // signature
    rqStream.Write(8,gs_aucSignature);

    unsigned int uiLength, uiCrc;
    int iChunkDataBeginningPos;

    // Header.
    BEFORE_CHUNK_DATA_WRITE(gs_auc_IHDR_Type);

    unsigned int uiWidth = m_iWidth;
    unsigned int uiHeight = m_iHeight;
    rqStream.Write(uiWidth);
    rqStream.Write(uiHeight);

    unsigned char ucBitDepth = m_iBitDepth;
    rqStream.Write(ucBitDepth);

    unsigned char ucColorModelType;
    switch ( m_eColorModel )
    {
    case CMT_GRAYSCALE:
        ucColorModelType = 0;
        break;
    case CMT_RGB:
        ucColorModelType = 2;
        break;
    case CMT_INDEXED:
        ucColorModelType = 3;
        break;
    case CMT_GRAYSCALE_WITH_ALPHA:
        ucColorModelType = 4;
        break;
    case CMT_RGBA:
        ucColorModelType = 6;
        break;
    }
    rqStream.Write(ucColorModelType);

    unsigned char ucCompression;
    switch ( m_eCompression )
    {
    case CT_DEFLATE_INFLATE:
        ucCompression = 0;
        break;
    }
    rqStream.Write(ucCompression);

    unsigned char ucFilterMethod;
    switch ( m_eFilterMethod )
    {
    case FMT_ADAPTIVE:
        ucFilterMethod = 0;
        break;
    }
    rqStream.Write(ucFilterMethod);

    unsigned char ucInterlaceMethod;
    switch ( m_eInterlaceMethod )
    {
    case IMT_NONE:
        ucInterlaceMethod = 0;
        break;
    case IMT_ADAM_7:
        ucInterlaceMethod = 1;
        break;
    }
    rqStream.Write(ucInterlaceMethod);

    AFTER_CHUNK_DATA_WRITE;

    // iCCP
    if ( m_bHas_iCCP_Chunk )
    {
        BEFORE_CHUNK_DATA_WRITE(gs_auc_iCCP_Type);
#ifdef _DEBUG
        CHECK_LATIN1_NONEMPTY_LIMITED80_STRING_FOR_SAVING(m_qICCProfileName);
#endif
        rqStream.Write(m_qICCProfileName.GetLength()+1,m_qICCProfileName);
        ucCompression = 0;
        rqStream.Write(ucCompression);
        StillArray<char> qCompressedProfile;
        CompressionZLib qZLib;
        qZLib.Compress(m_qICCProfile,m_qICCProfile.GetQuantity(),qCompressedProfile);
        rqStream.Write(qCompressedProfile.GetQuantity(),qCompressedProfile);
        AFTER_CHUNK_DATA_WRITE;
    }

    // sRGB
    if ( m_bHas_sRGB_Chunk )
    {
        BEFORE_CHUNK_DATA_WRITE(gs_auc_sRGB_Type);
        unsigned char ucRenderingIntent;
        switch ( m_eStandardRgbColorSpaceRenderingIntent )
        {
        case SRGBCSRIT_PERCEPTUAL:
            ucRenderingIntent = 0;
            break;
        case SRGBCSRIT_RELATIVE_COLORIMETRIC:
            ucRenderingIntent = 1;
            break;
        case SRGBCSRIT_SATURATION:
            ucRenderingIntent = 2;
            break;
        case SRGBCSRIT_ABSOLUTE_COLORIMETRIC:
            ucRenderingIntent = 3;
            break;
        }
        rqStream.Write(ucRenderingIntent);
        AFTER_CHUNK_DATA_WRITE;
    }

    // gAMA
    if ( m_bHas_gAMA_Chunk )
    {
        BEFORE_CHUNK_DATA_WRITE(gs_auc_gAMA_Type);
        unsigned int uiGamma = Mathf::RoundToInt(m_fGamma*100000);
        rqStream.Write(uiGamma);
        AFTER_CHUNK_DATA_WRITE;
    }

    // cHRM
    if ( m_bHas_cHRM_Chunk )
    {
        BEFORE_CHUNK_DATA_WRITE(gs_auc_cHRM_Type);
        unsigned int uiWhitePointX = Mathf::RoundToInt(m_fChromaticityWhitePointX*100000);
        unsigned int uiWhitePointY = Mathf::RoundToInt(m_fChromaticityWhitePointY*100000);
        unsigned int uiRedX = Mathf::RoundToInt(m_fChromaticityRedX*100000);
        unsigned int uiRedY = Mathf::RoundToInt(m_fChromaticityRedY*100000);
        unsigned int uiGreenX = Mathf::RoundToInt(m_fChromaticityGreenX*100000);
        unsigned int uiGreenY = Mathf::RoundToInt(m_fChromaticityGreenY*100000);
        unsigned int uiBlueX = Mathf::RoundToInt(m_fChromaticityBlueX*100000);
        unsigned int uiBlueY = Mathf::RoundToInt(m_fChromaticityBlueY*100000);
        rqStream.Write(uiWhitePointX);
        rqStream.Write(uiWhitePointY);
        rqStream.Write(uiRedX);
        rqStream.Write(uiRedY);
        rqStream.Write(uiGreenX);
        rqStream.Write(uiGreenY);
        rqStream.Write(uiBlueX);
        rqStream.Write(uiBlueY);
        AFTER_CHUNK_DATA_WRITE;
    }

    // PLTE
    if ( m_bHas_PLTE_Chunk )
    {
        BEFORE_CHUNK_DATA_WRITE(gs_auc_PLTE_Type);
        for (int i = 0; i < m_qPalette.GetQuantity(); i++)
        {
            const PaletteColor& rqPaletteColor = m_qPalette[i];

            rqStream.Write(rqPaletteColor.R());
            rqStream.Write(rqPaletteColor.G());
            rqStream.Write(rqPaletteColor.B());
        }
        AFTER_CHUNK_DATA_WRITE;
    }

    // tRNS
    if ( m_bHas_tRNS_Chunk )
    {
        BEFORE_CHUNK_DATA_WRITE(gs_auc_tRNS_Type);
        if ( m_eColorModel == CMT_GRAYSCALE )
        {
            unsigned short usTGray = m_iTransparencyGrayscale;
            rqStream.Write(usTGray);
        }
        else if ( m_eColorModel == CMT_RGB )
        {
            unsigned short usTRed = m_iTransparencyRed;
            unsigned short usTGreen = m_iTransparencyGreen;
            unsigned short usTBlue = m_iTransparencyBlue;
            rqStream.Write(usTRed);
            rqStream.Write(usTGreen);
            rqStream.Write(usTBlue);
        }
        else  // m_eColorModel = CMT_INDEXED
        {
            for (int i = 0; i < m_qPaletteAlphaAddon.GetQuantity(); i++)
                rqStream.Write(m_qPaletteAlphaAddon[i]);
        }
        AFTER_CHUNK_DATA_WRITE;
    }

    // hIST
    if ( m_bHas_hIST_Chunk )
    {
        BEFORE_CHUNK_DATA_WRITE(gs_auc_hIST_Type);
        unsigned short usFrequency;
        for (int i = 0; i < m_qHistogramFrequencies.GetQuantity(); i++)
        {
            usFrequency = m_qHistogramFrequencies[i];
            rqStream.Write(usFrequency);
        }
        AFTER_CHUNK_DATA_WRITE;
    }

    // bKGD
    if ( m_bHas_bKGD_Chunk )
    {
        BEFORE_CHUNK_DATA_WRITE(gs_auc_bKGD_Type);
        if ( m_eColorModel == CMT_GRAYSCALE || m_eColorModel == CMT_GRAYSCALE_WITH_ALPHA )
        {
            assert( 0 <= m_iBackgroundGrayscale && m_iBackgroundGrayscale <= m_iSampleMaxValue );

            unsigned short usGrayscale = m_iBackgroundGrayscale;
            rqStream.Write(usGrayscale);
        }
        else if ( m_eColorModel == CMT_RGB || m_eColorModel == CMT_RGBA )
        {
            assert( 0 <= m_iBackgroundRed && m_iBackgroundRed <= m_iSampleMaxValue );
            assert( 0 <= m_iBackgroundGreen && m_iBackgroundGreen <= m_iSampleMaxValue );
            assert( 0 <= m_iBackgroundBlue && m_iBackgroundBlue <= m_iSampleMaxValue );

            unsigned short usRed = m_iBackgroundRed;
            unsigned short usGreen = m_iBackgroundGreen;
            unsigned short usBlue = m_iBackgroundBlue;
            rqStream.Write(usRed);
            rqStream.Write(usGreen);
            rqStream.Write(usBlue);
        }
        else  // m_eColorModel = CMT_INDEXED
        {
            assert( 0 <= m_iBackgroundPaletteIndex && m_iBackgroundPaletteIndex < m_qPalette.GetQuantity() );

            unsigned char ucPaletteIndex = m_iBackgroundPaletteIndex;
            rqStream.Write(ucPaletteIndex);
        }
        AFTER_CHUNK_DATA_WRITE;
    }

    // pHYs
    if ( m_bHas_pHYs_Chunk )
    {
        BEFORE_CHUNK_DATA_WRITE(gs_auc_pHYs_Type);
        unsigned int uiPixelsPerUnitX = m_iPixelsPerUnitX;
        unsigned int uiPixelsPerUnitY = m_iPixelsPerUnitY;
        rqStream.Write(uiPixelsPerUnitX);
        rqStream.Write(uiPixelsPerUnitY);
        unsigned char ucUnitSpecifier;
        switch ( m_ePhysicalPixelDimensionsUnit )
        {
        case PPDUT_UNKNOWN:
            ucUnitSpecifier = 0;
            break;
        case PPDUT_METER:
            ucUnitSpecifier = 1;
            break;
        }
        rqStream.Write(ucUnitSpecifier);
        AFTER_CHUNK_DATA_WRITE;
    }

    // sPLT
    if ( !m_qSuggestedPalettes.IsEmpty() )
    {
        for (int i0 = 0; i0 < m_qSuggestedPalettes.GetQuantity(); i0++)
        {
            SuggestedPalette& rqSuggestedPalette = m_qSuggestedPalettes[i0];

            BEFORE_CHUNK_DATA_WRITE(gs_auc_sPLT_Type);
#ifdef _DEBUG
            CHECK_LATIN1_NONEMPTY_LIMITED80_STRING_FOR_SAVING(rqSuggestedPalette.Name);
#endif
            rqStream.Write(rqSuggestedPalette.Name.GetLength()+1,rqSuggestedPalette.Name);
            unsigned char ucSampleDepth;
            switch ( rqSuggestedPalette.SampleDepth )
            {
            case SPSDT_8_BITS:
                ucSampleDepth = 8;
                break;
            case SPSDT_16_BITS:
                ucSampleDepth = 16;
                break;
            }
            rqStream.Write(ucSampleDepth);
            SortUtils<SuggestedPaletteColor,SPEComparator>::Sort(rqSuggestedPalette.Palette);
            unsigned short usFrequency;
            if ( rqSuggestedPalette.SampleDepth == SPSDT_8_BITS )
            {
                unsigned char ucRed, ucGreen, ucBlue, ucAlpha;
                for (int i1 = 0; i1 < rqSuggestedPalette.Palette.GetQuantity(); i1++)
                {
                    const SuggestedPaletteColor& rqSPE = rqSuggestedPalette.Palette[i1];

                    ucRed = rqSPE.Red;
                    ucGreen = rqSPE.Green;
                    ucBlue = rqSPE.Blue;
                    ucAlpha = rqSPE.Alpha;
                    usFrequency = rqSPE.Frequency;
                    rqStream.Write(ucRed);
                    rqStream.Write(ucGreen);
                    rqStream.Write(ucBlue);
                    rqStream.Write(ucAlpha);
                    rqStream.Write(usFrequency);
                }
            }
            else  // rqSuggestedPalette.SampleDepth = SPSDT_16_BITS
            {
                unsigned short usRed, usGreen, usBlue, usAlpha;
                for (int i1 = 0; i1 < rqSuggestedPalette.Palette.GetQuantity(); i1++)
                {
                    const SuggestedPaletteColor& rqSPE = rqSuggestedPalette.Palette[i1];

                    usRed = rqSPE.Red;
                    usGreen = rqSPE.Green;
                    usBlue = rqSPE.Blue;
                    usAlpha = rqSPE.Alpha;
                    usFrequency = rqSPE.Frequency;
                    rqStream.Write(usRed);
                    rqStream.Write(usGreen);
                    rqStream.Write(usBlue);
                    rqStream.Write(usAlpha);
                    rqStream.Write(usFrequency);
                }
            }
            AFTER_CHUNK_DATA_WRITE;
        }
    }

    // tIME
    if ( m_bHas_tIME_Chunk )
    {
        BEFORE_CHUNK_DATA_WRITE(gs_auc_tIME_Type);
        unsigned short usYear = m_qLastModificationTime.GetYear();
        unsigned char ucMonth = m_qLastModificationTime.GetMonth();
        unsigned char ucDay = m_qLastModificationTime.GetDay();
        unsigned char ucHour = m_qLastModificationTime.GetHour();
        unsigned char ucMinute = m_qLastModificationTime.GetMinute();
        unsigned char ucSecond = m_qLastModificationTime.GetSecond();
        rqStream.Write(usYear);
        rqStream.Write(ucMonth);
        rqStream.Write(ucDay);
        rqStream.Write(ucHour);
        rqStream.Write(ucMinute);
        rqStream.Write(ucSecond);
        AFTER_CHUNK_DATA_WRITE;
    }

    // tEXt and zTXt
    if ( !m_qTextualDataArray.IsEmpty() )
    {
        for (int i = 0; i < m_qTextualDataArray.GetQuantity(); i++)
        {
            const TextualData& rqTextualData = m_qTextualDataArray[i];

#ifdef _DEBUG
            CHECK_LATIN1_NONEMPTY_LIMITED80_STRING_FOR_SAVING(rqTextualData.Keyword);
            CHECK_TEXT_FOR_SAVING(rqTextualData.Text);
#endif
            if ( rqTextualData.m_bOnlyAs_tEXt
            ||   rqTextualData.Text.GetLength() <= ms_iMinimalTextLengthToBeCompressed )
            {
                // tEXt
                BEFORE_CHUNK_DATA_WRITE(gs_auc_tEXt_Type);
                rqStream.Write(rqTextualData.Keyword.GetLength()+1,rqTextualData.Keyword);
                rqStream.Write(rqTextualData.Text.GetLength(),rqTextualData.Text);
                AFTER_CHUNK_DATA_WRITE;
            }
            else
            {
                // zTXt
                BEFORE_CHUNK_DATA_WRITE(gs_auc_zTXt_Type);
                rqStream.Write(rqTextualData.Keyword.GetLength()+1,rqTextualData.Keyword);
                ucCompression = 0;
                rqStream.Write(ucCompression);
                StillArray<char> qCompressedText;
                CompressionZLib qZLib;
                qZLib.Compress(rqTextualData.Text,rqTextualData.Text.GetLength(),qCompressedText);
                rqStream.Write(qCompressedText.GetQuantity(),qCompressedText);
                AFTER_CHUNK_DATA_WRITE;
            }
        }
    }

    // assign image size value and attributes
    COMPUTE_EXPECTED_IMAGE_SIZE_AND_RELATED_VALUES;

    // some derived values
    m_bUniformByteWay = (
        spqUseImage->GetColorModel() == RasterImage::CMT_PALETTED
    ||  spqUseImage->GetColorModel() == RasterImage::CMT_PALETTED_WITH_ALPHA
    ||  (spqUseImage->HasGradIntSampleUniformity()
    &&  (spqUseImage->GetBitsPerIntegerSample() == 8
    ||  spqUseImage->GetBitsPerIntegerSample() == 16)) );
    if ( m_bUniformByteWay )
    {
        if ( m_eColorModel != CMT_INDEXED )
        {
            m_iBytesPerImgSample = spqUseImage->GetBitsPerIntegerSample()/8;
            m_iImgSampleMaxValue = (1 << spqUseImage->GetBitsPerIntegerSample()) - 1;
        }
        else
        {
            m_iBytesPerImgSample = 1;
            m_iImgSampleMaxValue = -1;
        }
    }

    // for a progress bar
    ProgressProbe qProgress(this);

    // write the image
    StillArray<char> qDecompressedImageData(m_iExpectedImageSize);
    int iBytePos = 0;
    int iBitRatio = 0;
    if ( m_eInterlaceMethod == IMT_NONE )
    {
        qProgress.SetAdvance(0.5f/m_iHeight);

        // write pixels
        for (int iY = 0; iY < m_iHeight; iY++)
        {
            iBytePos++;
            for (int iX = 0; iX < m_iWidth; iX++)
                WritePixel(spqUseImage,iX,iY,qDecompressedImageData,iBytePos,iBitRatio);

            if ( iBitRatio != 0 )
            {
                iBytePos++;
                iBitRatio = 0;
            }

            qProgress.MakeAdvance();
        }

        unsigned char* aucFirstRow = (unsigned char*)qDecompressedImageData.GetElements();

        // find out filter types and do row filtering
        PROCESS_FILTERING_FOR_SAVING(m_iHeight,aucFirstRow,m_iRowSizeWithFB);
    }
    else  // m_eInterlaceMethod = IMT_ADAM_7
    {
        qProgress.SetAdvance(0.5f/7);

        // write pixels
        int iX, iY;
        unsigned char* aucFirstRow;
        for (int i = 0; i < 7; i++)
        {
            if ( m_aiPassSize[i] != 0 )
            {
                aucFirstRow = (unsigned char*)(qDecompressedImageData + iBytePos);
                for (int i0 = 0; i0 < m_aiHeightInPass[i]; i0++)
                {
                    iY = gs_aiPassStartH[i] + i0*gs_aiPassStepH[i];

                    iBytePos++;
                    for (int i1 = 0; i1 < m_aiWidthInPass[i]; i1++)
                    {
                        iX = gs_aiPassStartW[i] + i1*gs_aiPassStepW[i];

                        WritePixel(spqUseImage,iX,iY,qDecompressedImageData,iBytePos,iBitRatio);
                    }

                    if ( iBitRatio != 0 )
                    {
                        iBytePos++;
                        iBitRatio = 0;
                    }
                }

                // find out filter types and do row filtering
                PROCESS_FILTERING_FOR_SAVING(m_aiHeightInPass[i],aucFirstRow,m_aiRowSizeWithFBInPass[i]);
            }

            qProgress.MakeAdvance();
        }
    }

    // pack
    StillArray<char> qCompressedImageData;
    CompressionZLib qZLib;
    if ( ms_bChooseBestFromManifoldZLibStrategies )
        qZLib.SetCompressionStrategy(CompressionZLib::CST_BEST_AMONG_4);
    qZLib.Compress(qDecompressedImageData,qDecompressedImageData.GetQuantity(),qCompressedImageData);
    qDecompressedImageData.RemoveAll();

    qProgress.MakeOneTimeAdvance(0.5f);

    // flow out the compressed data
    if ( m_eDataChunkSplitting == DCST_SINGLE )
    {
        BEFORE_CHUNK_DATA_WRITE(gs_auc_IDAT_Type);
        rqStream.Write(qCompressedImageData.GetQuantity(),qCompressedImageData);
        AFTER_CHUNK_DATA_WRITE;
    }
    else  // m_eDataChunkSplitting = DCST_KILOBYTES_8 or m_eDataChunkSplitting = DCST_KILOBYTES_32
    {
        int iDataPortionSize = ( m_eDataChunkSplitting == DCST_KILOBYTES_8 ? 8192 : 32768 );

        int iBytesToWrite, iWritten = 0;
        while ( iWritten != qCompressedImageData.GetQuantity() )
        {
            BEFORE_CHUNK_DATA_WRITE(gs_auc_IDAT_Type);
            iBytesToWrite = ( iWritten + iDataPortionSize <= qCompressedImageData.GetQuantity() ?
                iDataPortionSize : qCompressedImageData.GetQuantity() - iWritten );
            rqStream.Write(iBytesToWrite,qCompressedImageData+iWritten);
            iWritten += iBytesToWrite;
            AFTER_CHUNK_DATA_WRITE;
        }
    }
    qCompressedImageData.RemoveAll();

    // IEND
    BEFORE_CHUNK_DATA_WRITE(gs_auc_IEND_Type);
    AFTER_CHUNK_DATA_WRITE;

    return true;
}
//------------------------------------------------------------------------------------------------------------------
void FormatPng::WritePixel (const RasterImagePtr& rspqImage, int iX, int iY, char* acDecompressedData,
    int& riBytePos, int& riBitRatio) const
{
    if ( m_bUniformByteWay )
    {
        char* pcSrcSample = rspqImage->GetRow(iY) + iX*rspqImage->GetBytesPerPixel();
        int iValue;
        unsigned short usValue;
        for (int i = 0; i < m_iSamplesPerPixel; i++)
        {
            if ( m_eColorModel != CMT_INDEXED )
            {
                if ( !(m_bHas_tRNS_Chunk && rspqImage->GetVisibilityMask()[iY*m_iWidth+iX] == 0) )
                {
                    iValue = 0;
#ifndef Q_BIG_ENDIAN
                    MemCpy(&iValue,pcSrcSample,m_iBytesPerImgSample);
#else
                    MemCpy(&iValue+(4-m_iBytesPerImgSample),pcSrcSample,m_iBytesPerImgSample);
#endif

                    if ( rspqImage->GetBitsPerIntegerSample() != m_iBitDepth )
                        iValue = Mathf::RoundToInt(((float)iValue)/m_iImgSampleMaxValue*m_iSampleMaxValue);
                }
                else
                {
                    if ( m_eColorModel == CMT_GRAYSCALE )
                    {
                        iValue = m_iTransparencyGrayscale;
                    }
                    else  // m_eColorModel = CMT_RGB
                    {
                        switch ( i )
                        {
                        case 0:
                            iValue = m_iTransparencyRed;
                            break;
                        case 1:
                            iValue = m_iTransparencyGreen;
                            break;
                        case 2:
                            iValue = m_iTransparencyBlue;
                            break;
                        }
                    }
                }
            }
            else
            {
                iValue = *pcSrcSample;
            }

            if ( m_iBitDepth <= 8 )
            {
                riBytePos += Storage::WriteBits(acDecompressedData+riBytePos,riBitRatio,m_iBitDepth,iValue);
            }
            else  // m_iBitDepth = 16
            {
                usValue = iValue;
                riBytePos += Storage::Write2be(acDecompressedData+riBytePos,1,&usValue);
            }

            pcSrcSample += m_iBytesPerImgSample;
        }
    }
    else
    {
        int aiValue[4], iSQ;
        if ( !(m_bHas_tRNS_Chunk && rspqImage->GetVisibilityMask()[iY*m_iWidth+iX] == 0) )
        {
            if ( rspqImage->GetColorModel() == RasterImage::CMT_GRAYSCALE )
            {
                float fIntensity = rspqImage->GetPixelIntensity(iX,iY);
                aiValue[0] = Mathf::RoundToInt(fIntensity*m_iSampleMaxValue);
                iSQ = 1;
            }
            else if ( rspqImage->GetColorModel() == RasterImage::CMT_GRAYSCALE_WITH_ALPHA )
            {
                Link<float,float> qIntensityWithAlpha = rspqImage->GetPixelIntensityWithAlpha(iX,iY);
                aiValue[0] = Mathf::RoundToInt(qIntensityWithAlpha.First*m_iSampleMaxValue);
                aiValue[1] = Mathf::RoundToInt(qIntensityWithAlpha.Second*m_iSampleMaxValue);
                iSQ = 2;
            }
            else if ( rspqImage->GetColorModel() == RasterImage::CMT_RGB )
            {
                ColorRgb qRgb = rspqImage->GetPixelRgb(iX,iY);
                for (int i = 0; i < 3; i++)
                    aiValue[i] = Mathf::RoundToInt(qRgb[i]*m_iSampleMaxValue);
                iSQ = 3;
            }
            else  // rspqImage->GetColorModel() = RasterImage::CMT_RGBA
            {
                ColorRgba qRgba = rspqImage->GetPixelRgba(iX,iY);
                for (int i = 0; i < 4; i++)
                    aiValue[i] = Mathf::RoundToInt(qRgba[i]*m_iSampleMaxValue);
                iSQ = 4;
            }
        }
        else
        {
            if ( m_eColorModel == CMT_GRAYSCALE )
            {
                aiValue[0] = m_iTransparencyGrayscale;
                iSQ = 1;
            }
            else  // m_eColorModel = CMT_RGB
            {
                aiValue[0] = m_iTransparencyRed;
                aiValue[1] = m_iTransparencyGreen;
                aiValue[2] = m_iTransparencyBlue;
                iSQ = 3;
            }
        }

        if ( m_iBitDepth <= 8 )
        {
            for (int i = 0; i < iSQ; i++)
                riBytePos += Storage::WriteBits(acDecompressedData+riBytePos,riBitRatio,m_iBitDepth,aiValue[i]);
        }
        else  // m_iBitDepth = 16
        {
            unsigned short usValue;
            for (int i = 0; i < iSQ; i++)
            {
                usValue = aiValue[i];
                riBytePos += Storage::Write2be(acDecompressedData+riBytePos,1,&usValue);
            }
        }
    }
}
//------------------------------------------------------------------------------------------------------------------
void FormatPng::AssignFBAndFilterRow (int iFilterType, int iRow, unsigned char* pucRow,
    const unsigned char* aucOrigRow, const unsigned char* aucOrigUpRow, int iRowSizeWithFB) const
{
    *pucRow = iFilterType;
    if ( iFilterType == 0 )
    {
        // none
        return;
    }
    else if ( iFilterType == 1 )
    {
        // sub
        int iA;

        if ( m_eColorModel == CMT_INDEXED || m_iBitDepth < 8
        ||   (m_iBitDepth == 8 && m_eColorModel == CMT_GRAYSCALE) )
        {
            for (int i = 1; i < iRowSizeWithFB; i++)
            {
                iA = ( i > 1 ? *(aucOrigRow + i - 1) : 0 );
                *(pucRow + i) = *(aucOrigRow + i) - iA;
            }
        }
        else
        {
            if ( m_eColorModel == CMT_GRAYSCALE )
            {
                // m_iBitDepth = 16
                for (int i = 1; i < iRowSizeWithFB; i++)
                {
                    iA = ( i > 2 ? *(aucOrigRow + i - 2) : 0 );
                    *(pucRow + i) = *(aucOrigRow + i) - iA;
                }
            }
            else if ( m_eColorModel == CMT_RGB )
            {
                if ( m_iBitDepth == 8 )
                {
                    for (int i = 1; i < iRowSizeWithFB; i++)
                    {
                        iA = ( i > 3 ? *(aucOrigRow + i - 3) : 0 );
                        *(pucRow + i) = *(aucOrigRow + i) - iA;
                    }
                }
                else  // m_iBitDepth = 16
                {
                    for (int i = 1; i < iRowSizeWithFB; i++)
                    {
                        iA = ( i > 6 ? *(aucOrigRow + i - 6) : 0 );
                        *(pucRow + i) = *(aucOrigRow + i) - iA;
                    }
                }
            }
            else if ( m_eColorModel == CMT_GRAYSCALE_WITH_ALPHA )
            {
                if ( m_iBitDepth == 8 )
                {
                    for (int i = 1; i < iRowSizeWithFB; i++)
                    {
                        iA = ( i > 2 ? *(aucOrigRow + i - 2) : 0 );
                        *(pucRow + i) = *(aucOrigRow + i) - iA;
                    }
                }
                else  // m_iBitDepth = 16
                {
                    for (int i = 1; i < iRowSizeWithFB; i++)
                    {
                        iA = ( i > 4 ? *(aucOrigRow + i - 4) : 0 );
                        *(pucRow + i) = *(aucOrigRow + i) - iA;
                    }
                }
            }
            else  // m_eColorModel = CMT_RGBA
            {
                if ( m_iBitDepth == 8 )
                {
                    for (int i = 1; i < iRowSizeWithFB; i++)
                    {
                        iA = ( i > 4 ? *(aucOrigRow + i - 4) : 0 );
                        *(pucRow + i) = *(aucOrigRow + i) - iA;
                    }
                }
                else  // m_iBitDepth = 16
                {
                    for (int i = 1; i < iRowSizeWithFB; i++)
                    {
                        iA = ( i > 8 ? *(aucOrigRow + i - 8) : 0 );
                        *(pucRow + i) = *(aucOrigRow + i) - iA;
                    }
                }
            }
        }
    }
    else if ( iFilterType == 2 )
    {
        // up
        int iB;

        for (int i = 1; i < iRowSizeWithFB; i++)
        {
            iB = ( iRow != 0 ? *(aucOrigUpRow + i) : 0 );
            *(pucRow + i) = *(aucOrigRow + i) - iB;
        }
    }
    else if ( iFilterType == 3 )
    {
        // average
        int iA, iB;

        if ( m_eColorModel == CMT_INDEXED || m_iBitDepth < 8
        ||   (m_iBitDepth == 8 && m_eColorModel == CMT_GRAYSCALE) )
        {
            for (int i = 1; i < iRowSizeWithFB; i++)
            {
                iA = ( i > 1 ? *(aucOrigRow + i - 1) : 0 );
                iB = ( iRow != 0 ? *(aucOrigUpRow + i) : 0 );
                *(pucRow + i) = *(aucOrigRow + i) - (iA + iB)/2;
            }
        }
        else
        {
            if ( m_eColorModel == CMT_GRAYSCALE )
            {
                // m_iBitDepth = 16
                for (int i = 1; i < iRowSizeWithFB; i++)
                {
                    iA = ( i > 2 ? *(aucOrigRow + i - 2) : 0 );
                    iB = ( iRow != 0 ? *(aucOrigUpRow + i) : 0 );
                    *(pucRow + i) = *(aucOrigRow + i) - (iA + iB)/2;
                }
            }
            else if ( m_eColorModel == CMT_RGB )
            {
                if ( m_iBitDepth == 8 )
                {
                    for (int i = 1; i < iRowSizeWithFB; i++)
                    {
                        iA = ( i > 3 ? *(aucOrigRow + i - 3) : 0 );
                        iB = ( iRow != 0 ? *(aucOrigUpRow + i) : 0 );
                        *(pucRow + i) = *(aucOrigRow + i) - (iA + iB)/2;
                    }
                }
                else  // m_iBitDepth = 16
                {
                    for (int i = 1; i < iRowSizeWithFB; i++)
                    {
                        iA = ( i > 6 ? *(aucOrigRow + i - 6) : 0 );
                        iB = ( iRow != 0 ? *(aucOrigUpRow + i) : 0 );
                        *(pucRow + i) = *(aucOrigRow + i) - (iA + iB)/2;
                    }
                }
            }
            else if ( m_eColorModel == CMT_GRAYSCALE_WITH_ALPHA )
            {
                if ( m_iBitDepth == 8 )
                {
                    for (int i = 1; i < iRowSizeWithFB; i++)
                    {
                        iA = ( i > 2 ? *(aucOrigRow + i - 2) : 0 );
                        iB = ( iRow != 0 ? *(aucOrigUpRow + i) : 0 );
                        *(pucRow + i) = *(aucOrigRow + i) - (iA + iB)/2;
                    }
                }
                else  // m_iBitDepth = 16
                {
                    for (int i = 1; i < iRowSizeWithFB; i++)
                    {
                        iA = ( i > 4 ? *(aucOrigRow + i - 4) : 0 );
                        iB = ( iRow != 0 ? *(aucOrigUpRow + i) : 0 );
                        *(pucRow + i) = *(aucOrigRow + i) - (iA + iB)/2;
                    }
                }
            }
            else  // m_eColorModel = CMT_RGBA
            {
                if ( m_iBitDepth == 8 )
                {
                    for (int i = 1; i < iRowSizeWithFB; i++)
                    {
                        iA = ( i > 4 ? *(aucOrigRow + i - 4) : 0 );
                        iB = ( iRow != 0 ? *(aucOrigUpRow + i) : 0 );
                        *(pucRow + i) = *(aucOrigRow + i) - (iA + iB)/2;
                    }
                }
                else  // m_iBitDepth = 16
                {
                    for (int i = 1; i < iRowSizeWithFB; i++)
                    {
                        iA = ( i > 8 ? *(aucOrigRow + i - 8) : 0 );
                        iB = ( iRow != 0 ? *(aucOrigUpRow + i) : 0 );
                        *(pucRow + i) = *(aucOrigRow + i) - (iA + iB)/2;
                    }
                }
            }
        }
    }
    else  // iFilterType = 4
    {
        // Paeth
        int iA, iB, iC;

        if ( m_eColorModel == CMT_INDEXED || m_iBitDepth < 8
        ||   (m_iBitDepth == 8 && m_eColorModel == CMT_GRAYSCALE) )
        {
            for (int i = 1; i < iRowSizeWithFB; i++)
            {
                iA = ( i > 1 ? *(aucOrigRow + i - 1) : 0 );
                iB = ( iRow != 0 ? *(aucOrigUpRow + i) : 0 );
                iC = ( i > 1 && iRow != 0 ? *(aucOrigUpRow + i - 1) : 0 );
                *(pucRow + i) = *(aucOrigRow + i) - PredictPaeth(iA,iB,iC);
            }
        }
        else
        {
            if ( m_eColorModel == CMT_GRAYSCALE )
            {
                // m_iBitDepth = 16
                for (int i = 1; i < iRowSizeWithFB; i++)
                {
                    iA = ( i > 2 ? *(aucOrigRow + i - 2) : 0 );
                    iB = ( iRow != 0 ? *(aucOrigUpRow + i) : 0 );
                    iC = ( i > 2 && iRow != 0 ? *(aucOrigUpRow + i - 2) : 0 );
                    *(pucRow + i) = *(aucOrigRow + i) - PredictPaeth(iA,iB,iC);
                }
            }
            else if ( m_eColorModel == CMT_RGB )
            {
                if ( m_iBitDepth == 8 )
                {
                    for (int i = 1; i < iRowSizeWithFB; i++)
                    {
                        iA = ( i > 3 ? *(aucOrigRow + i - 3) : 0 );
                        iB = ( iRow != 0 ? *(aucOrigUpRow + i) : 0 );
                        iC = ( i > 3 && iRow != 0 ? *(aucOrigUpRow + i - 3) : 0 );
                        *(pucRow + i) = *(aucOrigRow + i) - PredictPaeth(iA,iB,iC);
                    }
                }
                else  // m_iBitDepth = 16
                {
                    for (int i = 1; i < iRowSizeWithFB; i++)
                    {
                        iA = ( i > 6 ? *(aucOrigRow + i - 6) : 0 );
                        iB = ( iRow != 0 ? *(aucOrigUpRow + i) : 0 );
                        iC = ( i > 6 && iRow != 0 ? *(aucOrigUpRow + i - 6) : 0 );
                        *(pucRow + i) = *(aucOrigRow + i) - PredictPaeth(iA,iB,iC);
                    }
                }
            }
            else if ( m_eColorModel == CMT_GRAYSCALE_WITH_ALPHA )
            {
                if ( m_iBitDepth == 8 )
                {
                    for (int i = 1; i < iRowSizeWithFB; i++)
                    {
                        iA = ( i > 2 ? *(aucOrigRow + i - 2) : 0 );
                        iB = ( iRow != 0 ? *(aucOrigUpRow + i) : 0 );
                        iC = ( i > 2 && iRow != 0 ? *(aucOrigUpRow + i - 2) : 0 );
                        *(pucRow + i) = *(aucOrigRow + i) - PredictPaeth(iA,iB,iC);
                    }
                }
                else  // m_iBitDepth = 16
                {
                    for (int i = 1; i < iRowSizeWithFB; i++)
                    {
                        iA = ( i > 4 ? *(aucOrigRow + i - 4) : 0 );
                        iB = ( iRow != 0 ? *(aucOrigUpRow + i) : 0 );
                        iC = ( i > 4 && iRow != 0 ? *(aucOrigUpRow + i - 4) : 0 );
                        *(pucRow + i) = *(aucOrigRow + i) - PredictPaeth(iA,iB,iC);
                    }
                }
            }
            else  // m_eColorModel = CMT_RGBA
            {
                if ( m_iBitDepth == 8 )
                {
                    for (int i = 1; i < iRowSizeWithFB; i++)
                    {
                        iA = ( i > 4 ? *(aucOrigRow + i - 4) : 0 );
                        iB = ( iRow != 0 ? *(aucOrigUpRow + i) : 0 );
                        iC = ( i > 4 && iRow != 0 ? *(aucOrigUpRow + i - 4) : 0 );
                        *(pucRow + i) = *(aucOrigRow + i) - PredictPaeth(iA,iB,iC);
                    }
                }
                else  // m_iBitDepth = 16
                {
                    for (int i = 1; i < iRowSizeWithFB; i++)
                    {
                        iA = ( i > 8 ? *(aucOrigRow + i - 8) : 0 );
                        iB = ( iRow != 0 ? *(aucOrigUpRow + i) : 0 );
                        iC = ( i > 8 && iRow != 0 ? *(aucOrigUpRow + i - 8) : 0 );
                        *(pucRow + i) = *(aucOrigRow + i) - PredictPaeth(iA,iB,iC);
                    }
                }
            }
        }
    }
}
//------------------------------------------------------------------------------------------------------------------
int FormatPng::GetMSADForFilteredRow (const unsigned char* pucRow, int iRowSizeWithFB)
{
    int iSum = 0;
    const unsigned char* pucByte = pucRow;
    for (int i = 0; i < iRowSizeWithFB; i++)
    {
        iSum += ( *pucByte < 128 ? *pucByte : 256 - *pucByte );
        pucByte++;
    }
    return iSum;
}
//------------------------------------------------------------------------------------------------------------------
void FormatPng::GetFilterTypesForRows (StillArray<int>& rqFilterTypes, int iHeight,
    const unsigned char* aucFirstRow, int iRowSizeWithFB) const
{
    rqFilterTypes.SetQuantity(iHeight);
    if ( m_eFilterSelectionMethod == FSM_MINIMUM_SUM_OF_ABSOLUTE_DIFFERENCES )
    {
        StillArray<unsigned char> qTestRow(iRowSizeWithFB);
        const unsigned char* aucUpRow = 0;
        const unsigned char* pucRow = aucFirstRow;
        int iMinMSAD, iMSAD, iMinMSADFilterType;
        for (int iY = 0; iY < iHeight; iY++)
        {
            for (int i = 0; i < 5; i++)
            {
                MemCpy(qTestRow,pucRow,iRowSizeWithFB);
                AssignFBAndFilterRow(i,iY,qTestRow,pucRow,aucUpRow,iRowSizeWithFB);
                iMSAD = GetMSADForFilteredRow(qTestRow,iRowSizeWithFB);
                if ( i == 0 || iMSAD < iMinMSAD )
                {
                    iMinMSAD = iMSAD;
                    iMinMSADFilterType = i;
                }
            }
            rqFilterTypes[iY] = iMinMSADFilterType;

            aucUpRow = pucRow;
            pucRow += iRowSizeWithFB;
        }
    }
    else  // m_eFilterSelectionMethod = FSM_LOCAL_MULTICHOICE
    {
        StillArray<unsigned char> qTestRows(2*iRowSizeWithFB);
        int iLeft = iHeight;
        int iRow0 = 0;
        int iRow1 = 1;
        const unsigned char* aucRow0;
        const unsigned char* aucRow1;
        int iMinComprSize, iChosenType0, iChosenType1;
        while ( iLeft >= 2 )
        {
            for (int i0 = 0; i0 < 5; i0++)
            {
                for (int i1 = 0; i1 < 5; i1++)
                {
                    aucRow0 = aucFirstRow + iRow0*iRowSizeWithFB;
                    aucRow1 = aucFirstRow + iRow1*iRowSizeWithFB;
                    MemCpy(qTestRows,aucRow0,iRowSizeWithFB);
                    MemCpy(qTestRows+iRowSizeWithFB,aucRow1,iRowSizeWithFB);
                    AssignFBAndFilterRow(i0,iRow0,qTestRows,aucRow0,aucRow0-iRowSizeWithFB,iRowSizeWithFB);
                    AssignFBAndFilterRow(i1,iRow1,qTestRows+iRowSizeWithFB,aucRow1,aucRow0,iRowSizeWithFB);
                    StillArray<char> qComprRows;
                    CompressionZLib qZLib;
                    qZLib.SetSpeedSizeBalance(0.0f);
                    qZLib.Compress((char*)qTestRows.GetElements(),qTestRows.GetQuantity(),qComprRows);
                    if ( (i0 == 0 && i1 == 0) || qComprRows.GetQuantity() < iMinComprSize )
                    {
                        iMinComprSize = qComprRows.GetQuantity();
                        iChosenType0 = i0;
                        iChosenType1 = i1;
                    }
                }
            }

            rqFilterTypes[iRow0] = iChosenType0;
            rqFilterTypes[iRow1] = iChosenType1;

            iLeft -= 2;
            iRow0 += 2;
            iRow1 += 2;
        }
        if ( (iHeight & 1) != 0 )
            rqFilterTypes.GetLast() = 4;
    }
}
//------------------------------------------------------------------------------------------------------------------
bool FormatPng::LoadHeadingData (int iBytestreamSize)
{
    // defaults:
    m_bHas_PLTE_Chunk = false;
    m_bHas_cHRM_Chunk = false;
    m_bHas_gAMA_Chunk = false;
    m_bHas_iCCP_Chunk = false;
    m_bHas_sBIT_Chunk = false;
    m_bHas_sRGB_Chunk = false;
    m_bHas_bKGD_Chunk = false;
    m_bHas_hIST_Chunk = false;
    m_bHas_tRNS_Chunk = false;
    m_bHas_pHYs_Chunk = false;
    m_bHas_tIME_Chunk = false;
    m_bImageIsInitialized = false;

    assert( iBytestreamSize == -1 || iBytestreamSize >= 0 );

    bool bSuccess;

    // signature
    int iDataSize;
    if ( m_eStreaming == Stream::ST_READING_FROM_FILE )
        CONDITIONAL_THROW_EXCEPTION(Storage::GetFileSize(iDataSize,m_qFilename),ET_CANNOT_ACCESS_FILE)
    else
        iDataSize = iBytestreamSize;
    if ( iDataSize < 8 )
    {
        if ( m_eStreaming != Stream::ST_READING_FROM_DYNAMIC_BYTESTREAM )
            THROW_EXCEPTION_WITHOUT_ASSERT_FALSE(ET_FORMAT_IDENTIFIER_MISMATCH)
        else
            return false;
    }
    Stream qSignatureStream(m_eStreaming,m_acStreamSrcParam,0,8,false,bSuccess,iBytestreamSize);
    PROCESS_STREAM_SUCCESS_FOR_LOADING(bSuccess);
    char acSignature[8];
    qSignatureStream.Read(8,acSignature);
    qSignatureStream.Finalize();
    CONDITIONAL_THROW_EXCEPTION_WITHOUT_ASSERT_FALSE(
        ( MemCmp(acSignature,gs_aucSignature,8) == 0 ),ET_FORMAT_IDENTIFIER_MISMATCH);

    char acType[4];
    unsigned int uiLength, uiCrc;

    // IHDR.
    Stream qHeaderStream(m_eStreaming,m_acStreamSrcParam,8,25,false,bSuccess,iBytestreamSize);
    PROCESS_STREAM_SUCCESS_FOR_LOADING(bSuccess);

    qHeaderStream.Read(uiLength);
    CONDITIONAL_THROW_EXCEPTION(( uiLength == 13 ),ET_FORMAT_MISMATCH);

    qHeaderStream.OffsetBytePosition(4+uiLength);
    qHeaderStream.Read(uiCrc);
    CONDITIONAL_THROW_EXCEPTION(( uiCrc == Storage::GetCrc32Checksum(qHeaderStream+4,17) ),ET_FORMAT_MISMATCH);
    qHeaderStream.SetBytePosition(4);

    qHeaderStream.Read(4,acType);
    CONDITIONAL_THROW_EXCEPTION(( MemCmp(acType,gs_auc_IHDR_Type,4) == 0 ),ET_FORMAT_MISMATCH);

    unsigned int uiWidth, uiHeight;
    qHeaderStream.Read(uiWidth);
    qHeaderStream.Read(uiHeight);
    CONDITIONAL_THROW_EXCEPTION(( uiWidth != 0 && uiHeight != 0 ),ET_FORMAT_MISMATCH);
    m_iWidth = uiWidth;
    m_iHeight = uiHeight;

    unsigned char ucBitDepth;
    qHeaderStream.Read(ucBitDepth);
    m_iBitDepth = ucBitDepth;

    m_iSampleRange = 1 << m_iBitDepth;
    m_iSampleMaxValue = m_iSampleRange - 1;

    unsigned char ucColorModelType;
    qHeaderStream.Read(ucColorModelType);
    switch ( ucColorModelType )
    {
    case 0:
        m_eColorModel = CMT_GRAYSCALE;
        break;
    case 2:
        m_eColorModel = CMT_RGB;
        break;
    case 3:
        m_eColorModel = CMT_INDEXED;
        break;
    case 4:
        m_eColorModel = CMT_GRAYSCALE_WITH_ALPHA;
        break;
    case 6:
        m_eColorModel = CMT_RGBA;
        break;
    default:
        THROW_EXCEPTION(ET_FORMAT_MISMATCH);
    }

    CONDITIONAL_THROW_EXCEPTION((
         (m_eColorModel == CMT_GRAYSCALE
    &&   (m_iBitDepth == 1 || m_iBitDepth == 2 || m_iBitDepth == 4 || m_iBitDepth == 8 || m_iBitDepth == 16))
    ||   (m_eColorModel == CMT_RGB && (m_iBitDepth == 8 || m_iBitDepth == 16))
    ||   (m_eColorModel == CMT_INDEXED
    &&   (m_iBitDepth == 1 || m_iBitDepth == 2 || m_iBitDepth == 4 || m_iBitDepth == 8))
    ||   (m_eColorModel == CMT_GRAYSCALE_WITH_ALPHA && (m_iBitDepth == 8 || m_iBitDepth == 16))
    ||   (m_eColorModel == CMT_RGBA && (m_iBitDepth == 8 || m_iBitDepth == 16)) ),
    ET_FORMAT_MISMATCH);

    switch ( m_eColorModel )
    {
    case CMT_GRAYSCALE:
        m_iSamplesPerPixel = 1;
        break;
    case CMT_RGB:
        m_iSamplesPerPixel = 3;
        break;
    case CMT_INDEXED:
        m_iSamplesPerPixel = 1;
        break;
    case CMT_GRAYSCALE_WITH_ALPHA:
        m_iSamplesPerPixel = 2;
        break;
    case CMT_RGBA:
        m_iSamplesPerPixel = 4;
        break;
    }

    m_iBitsPerPixel = m_iSamplesPerPixel*m_iBitDepth;

    unsigned char ucCompression;
    qHeaderStream.Read(ucCompression);
    switch ( ucCompression )
    {
    case 0:
        m_eCompression = CT_DEFLATE_INFLATE;
        break;
    default:
        THROW_EXCEPTION(ET_FORMAT_MISMATCH);
    }

    unsigned char ucFilterMethod;
    qHeaderStream.Read(ucFilterMethod);
    switch ( ucFilterMethod )
    {
    case 0:
        m_eFilterMethod = FMT_ADAPTIVE;
        break;
    default:
        THROW_EXCEPTION(ET_FORMAT_MISMATCH);
    }

    unsigned char ucInterlaceMethod;
    qHeaderStream.Read(ucInterlaceMethod);
    switch ( ucInterlaceMethod )
    {
    case 0:
        m_eInterlaceMethod = IMT_NONE;
        break;
    case 1:
        m_eInterlaceMethod = IMT_ADAM_7;
        break;
    default:
        THROW_EXCEPTION(ET_FORMAT_MISMATCH);
    }

    qHeaderStream.Finalize();

    // reach the first IDAT chunk collecting information from other chunks on the way
    unsigned int uiTDCLength;
    bool bNullFound;
    int iNullPos, iSize, iVar;
    const char* pcNullSearchStart;
    const char* pcChar;
    unsigned char ucChar;
    m_iNextReadPos = 33;
    for (/**/; /**/; /**/)
    {
        Stream qLengthStream(m_eStreaming,m_acStreamSrcParam,m_iNextReadPos,4,false,bSuccess,iBytestreamSize);
        PROCESS_STREAM_SUCCESS_FOR_LOADING(bSuccess);
        m_iNextReadPos += 4;
        qLengthStream.Read(uiLength);
        qLengthStream.Finalize();

        Stream qTypeStream(m_eStreaming,m_acStreamSrcParam,m_iNextReadPos,4,false,bSuccess,iBytestreamSize);
        PROCESS_STREAM_SUCCESS_FOR_LOADING(bSuccess);
        qTypeStream.Read(4,acType);
        if ( MemCmp(acType,gs_auc_IDAT_Type,4) == 0 )
        {
            m_iNextReadPos -= 4;
            break;
        }
        qTypeStream.Finalize();

        uiTDCLength = uiLength + 8;
        Stream qChunkStream(m_eStreaming,m_acStreamSrcParam,m_iNextReadPos,uiTDCLength,false,bSuccess,
            iBytestreamSize);
        PROCESS_STREAM_SUCCESS_FOR_LOADING(bSuccess);
        m_iNextReadPos += uiTDCLength;

        qChunkStream.Read(4,acType);
        if ( MemCmp(acType,gs_auc_PLTE_Type,4) == 0 )
        {
            // palette
            CONDITIONAL_THROW_EXCEPTION(( !(m_eColorModel == CMT_INDEXED && m_bHas_PLTE_Chunk) ),
                ET_FORMAT_MISMATCH);

            qChunkStream.OffsetBytePosition(uiLength);
            qChunkStream.Read(uiCrc);
            if ( uiCrc != Storage::GetCrc32Checksum(qChunkStream,4+uiLength) )
            {
                if ( m_eColorModel == CMT_INDEXED )
                    THROW_EXCEPTION(ET_FORMAT_MISMATCH)
                else
                    LEAVE_ANCILLARY_CHUNK(WT_CHUNK_PLTE_IS_INCORRECT);
            }
            qChunkStream.SetBytePosition(4);
            CONDITIONAL_LEAVE_ANCILLARY_CHUNK(( !(m_eColorModel != CMT_INDEXED
            &&   m_eColorModel != CMT_RGB && m_eColorModel != CMT_RGBA) ),
            WT_CHUNK_PLTE_IS_INCORRECT);
            if ( uiLength == 0 || uiLength % 3 != 0 )
            {
                if ( m_eColorModel == CMT_INDEXED )
                    THROW_EXCEPTION(ET_FORMAT_MISMATCH)
                else
                    LEAVE_ANCILLARY_CHUNK(WT_CHUNK_PLTE_IS_INCORRECT);
            }

            int iPaletteSize = uiLength/3;
            if ( iPaletteSize > m_iSampleRange )
            {
                if ( m_eColorModel == CMT_INDEXED )
                    THROW_EXCEPTION(ET_FORMAT_MISMATCH)
                else
                    LEAVE_ANCILLARY_CHUNK(WT_CHUNK_PLTE_IS_INCORRECT);
            }

            m_qPalette.SetQuantity(iPaletteSize);
            for (int i = 0; i < iPaletteSize; i++)
            {
                PaletteColor& rqPaletteColor = m_qPalette[i];

                qChunkStream.Read(rqPaletteColor.R());
                qChunkStream.Read(rqPaletteColor.G());
                qChunkStream.Read(rqPaletteColor.B());
            }

            m_bHas_PLTE_Chunk = true;
        }
        else if ( MemCmp(acType,gs_auc_cHRM_Type,4) == 0 )
        {
            // white point and primary chromaticities
            CHECK_CRC_FOR_ANCILLARY_CHUNK(WT_ANCILLARY_CHUNK_cHRM_IS_INCORRECT);
            CONDITIONAL_LEAVE_ANCILLARY_CHUNK(( !m_bHas_PLTE_Chunk && uiLength == 32 ),
                WT_ANCILLARY_CHUNK_cHRM_IS_INCORRECT);

            unsigned int uiChromaticityWhitePointX, uiChromaticityWhitePointY,
                uiChromaticityRedX, uiChromaticityRedY, uiChromaticityGreenX, uiChromaticityGreenY,
                uiChromaticityBlueX, uiChromaticityBlueY;
            qChunkStream.Read(uiChromaticityWhitePointX);
            qChunkStream.Read(uiChromaticityWhitePointY);
            qChunkStream.Read(uiChromaticityRedX);
            qChunkStream.Read(uiChromaticityRedY);
            qChunkStream.Read(uiChromaticityGreenX);
            qChunkStream.Read(uiChromaticityGreenY);
            qChunkStream.Read(uiChromaticityBlueX);
            qChunkStream.Read(uiChromaticityBlueY);

            m_fChromaticityWhitePointX = ((float)uiChromaticityWhitePointX)/100000;
            m_fChromaticityWhitePointY = ((float)uiChromaticityWhitePointY)/100000;
            m_fChromaticityRedX = ((float)uiChromaticityRedX)/100000;
            m_fChromaticityRedY = ((float)uiChromaticityRedY)/100000;
            m_fChromaticityGreenX = ((float)uiChromaticityGreenX)/100000;
            m_fChromaticityGreenY = ((float)uiChromaticityGreenY)/100000;
            m_fChromaticityBlueX = ((float)uiChromaticityBlueX)/100000;
            m_fChromaticityBlueY = ((float)uiChromaticityBlueY)/100000;

            m_bHas_cHRM_Chunk = true;
        }
        else if ( MemCmp(acType,gs_auc_gAMA_Type,4) == 0 )
        {
            // image gamma
            CHECK_CRC_FOR_ANCILLARY_CHUNK(WT_ANCILLARY_CHUNK_gAMA_IS_INCORRECT);
            CONDITIONAL_LEAVE_ANCILLARY_CHUNK(( !m_bHas_PLTE_Chunk && uiLength == 4 ),
                WT_ANCILLARY_CHUNK_gAMA_IS_INCORRECT);

            unsigned int uiGamma;
            qChunkStream.Read(uiGamma);
            m_fGamma = ((float)uiGamma)/100000;

            m_bHas_gAMA_Chunk = true;
        }
        else if ( MemCmp(acType,gs_auc_iCCP_Type,4) == 0 )
        {
            // embedded ICC profile
            CHECK_CRC_FOR_ANCILLARY_CHUNK(WT_ANCILLARY_CHUNK_iCCP_IS_INCORRECT);
            CONDITIONAL_LEAVE_ANCILLARY_CHUNK(( !m_bHas_PLTE_Chunk && uiLength >= 3 ),
                WT_ANCILLARY_CHUNK_iCCP_IS_INCORRECT);

            READ_AND_PROCESS_LATIN1_NONEMPTY_LIMITED80_STRING_FOR_ANCILLARY_CHUNK(4,m_qICCProfileName,
                WT_ANCILLARY_CHUNK_iCCP_IS_INCORRECT);

            unsigned char ucCompression;
            qChunkStream.Read(ucCompression);
            CONDITIONAL_LEAVE_ANCILLARY_CHUNK(( ucCompression == 0 ),WT_ANCILLARY_CHUNK_iCCP_IS_INCORRECT);

            iSize = qChunkStream.GetByteQuantity() - 4 - qChunkStream.GetBytePosition();
            CompressionZLib qZLib;
            CONDITIONAL_LEAVE_ANCILLARY_CHUNK(
                qZLib.Decompress(qChunkStream.GetBytesUpPosition(),iSize,m_qICCProfile),
                WT_ANCILLARY_CHUNK_iCCP_IS_INCORRECT);

            m_bHas_iCCP_Chunk = true;
        }
        else if ( MemCmp(acType,gs_auc_sBIT_Type,4) == 0 )
        {
            // significant bits
            CHECK_CRC_FOR_ANCILLARY_CHUNK(WT_ANCILLARY_CHUNK_sBIT_IS_INCORRECT);
            CONDITIONAL_LEAVE_ANCILLARY_CHUNK((
                 !m_bHas_PLTE_Chunk
            &&   ((m_eColorModel == CMT_GRAYSCALE && uiLength == 1)
            ||   (m_eColorModel == CMT_RGB && uiLength == 3)
            ||   (m_eColorModel == CMT_INDEXED && uiLength == 3)
            ||   (m_eColorModel == CMT_GRAYSCALE_WITH_ALPHA && uiLength == 2)
            ||   (m_eColorModel == CMT_RGBA && uiLength == 4)) ),
            WT_ANCILLARY_CHUNK_sBIT_IS_INCORRECT);

            unsigned char ucSGray, ucSRed, ucSGreen, ucSBlue, ucSAlpha;
            if ( m_eColorModel == CMT_GRAYSCALE )
            {
                qChunkStream.Read(ucSGray);
                CONDITIONAL_LEAVE_ANCILLARY_CHUNK(( 0 < ucSGray && ucSGray <= m_iBitDepth ),
                    WT_ANCILLARY_CHUNK_sBIT_IS_INCORRECT);

                m_iSignificantBitsGrayscale = ucSGray;

                m_iShiftVGray = m_iBitDepth - m_iSignificantBitsGrayscale;

                m_iMaxVGray = (1 << m_iSignificantBitsGrayscale) - 1;
            }
            else if ( m_eColorModel == CMT_RGB || m_eColorModel == CMT_INDEXED )
            {
                qChunkStream.Read(ucSRed);
                qChunkStream.Read(ucSGreen);
                qChunkStream.Read(ucSBlue);
                int iRoof = ( m_eColorModel == CMT_RGB ? m_iBitDepth : 8 );
                CONDITIONAL_LEAVE_ANCILLARY_CHUNK((
                     0 < ucSRed && ucSRed <= iRoof
                &&   0 < ucSGreen && ucSGreen <= iRoof
                &&   0 < ucSBlue && ucSBlue <= iRoof ),WT_ANCILLARY_CHUNK_sBIT_IS_INCORRECT);

                m_iSignificantBitsRed = ucSRed;
                m_iSignificantBitsGreen = ucSGreen;
                m_iSignificantBitsBlue = ucSBlue;

                m_iShiftVRed = iRoof - m_iSignificantBitsRed;
                m_iShiftVGreen = iRoof - m_iSignificantBitsGreen;
                m_iShiftVBlue = iRoof - m_iSignificantBitsBlue;

                m_iMaxVRed = (1 << m_iSignificantBitsRed) - 1;
                m_iMaxVGreen = (1 << m_iSignificantBitsGreen) - 1;
                m_iMaxVBlue = (1 << m_iSignificantBitsBlue) - 1;
            }
            else if ( m_eColorModel == CMT_GRAYSCALE_WITH_ALPHA )
            {
                qChunkStream.Read(ucSGray);
                qChunkStream.Read(ucSAlpha);
                CONDITIONAL_LEAVE_ANCILLARY_CHUNK((
                     0 < ucSGray && ucSGray <= m_iBitDepth
                &&   0 < ucSAlpha && ucSAlpha <= m_iBitDepth ),WT_ANCILLARY_CHUNK_sBIT_IS_INCORRECT);

                m_iSignificantBitsGrayscale = ucSGray;
                m_iSignificantBitsAlpha = ucSAlpha;

                m_iShiftVGray = m_iBitDepth - m_iSignificantBitsGrayscale;
                m_iShiftVAlpha = m_iBitDepth - m_iSignificantBitsAlpha;

                m_iMaxVGray = (1 << m_iSignificantBitsGrayscale) - 1;
                m_iMaxVAlpha = (1 << m_iSignificantBitsAlpha) - 1;
            }
            else  // m_eColorModel = CMT_RGBA
            {
                qChunkStream.Read(ucSRed);
                qChunkStream.Read(ucSGreen);
                qChunkStream.Read(ucSBlue);
                qChunkStream.Read(ucSAlpha);
                CONDITIONAL_LEAVE_ANCILLARY_CHUNK((
                     0 < ucSRed && ucSRed <= m_iBitDepth
                &&   0 < ucSGreen && ucSGreen <= m_iBitDepth
                &&   0 < ucSBlue && ucSBlue <= m_iBitDepth
                &&   0 < ucSAlpha && ucSAlpha <= m_iBitDepth ),WT_ANCILLARY_CHUNK_sBIT_IS_INCORRECT);

                m_iSignificantBitsRed = ucSRed;
                m_iSignificantBitsGreen = ucSGreen;
                m_iSignificantBitsBlue = ucSBlue;
                m_iSignificantBitsAlpha = ucSAlpha;

                m_iShiftVRed = m_iBitDepth - m_iSignificantBitsRed;
                m_iShiftVGreen = m_iBitDepth - m_iSignificantBitsGreen;
                m_iShiftVBlue = m_iBitDepth - m_iSignificantBitsBlue;
                m_iShiftVAlpha = m_iBitDepth - m_iSignificantBitsAlpha;

                m_iMaxVRed = (1 << m_iSignificantBitsRed) - 1;
                m_iMaxVGreen = (1 << m_iSignificantBitsGreen) - 1;
                m_iMaxVBlue = (1 << m_iSignificantBitsBlue) - 1;
                m_iMaxVAlpha = (1 << m_iSignificantBitsAlpha) - 1;
            }

            m_bHas_sBIT_Chunk = true;
        }
        else if ( MemCmp(acType,gs_auc_sRGB_Type,4) == 0 )
        {
            // standard RGB color space
            CHECK_CRC_FOR_ANCILLARY_CHUNK(WT_ANCILLARY_CHUNK_sRGB_IS_INCORRECT);
            CONDITIONAL_LEAVE_ANCILLARY_CHUNK(( !m_bHas_PLTE_Chunk && uiLength == 1 ),
                WT_ANCILLARY_CHUNK_sRGB_IS_INCORRECT);

            unsigned char ucRenderingIntent;
            qChunkStream.Read(ucRenderingIntent);
            switch ( ucRenderingIntent )
            {
            case 0:
                m_eStandardRgbColorSpaceRenderingIntent = SRGBCSRIT_PERCEPTUAL;
                break;
            case 1:
                m_eStandardRgbColorSpaceRenderingIntent = SRGBCSRIT_RELATIVE_COLORIMETRIC;
                break;
            case 2:
                m_eStandardRgbColorSpaceRenderingIntent = SRGBCSRIT_SATURATION;
                break;
            case 3:
                m_eStandardRgbColorSpaceRenderingIntent = SRGBCSRIT_ABSOLUTE_COLORIMETRIC;
                break;
            default:
                LEAVE_ANCILLARY_CHUNK(WT_ANCILLARY_CHUNK_sRGB_IS_INCORRECT);
            }

            m_bHas_sRGB_Chunk = true;
        }
        else if ( MemCmp(acType,gs_auc_bKGD_Type,4) == 0 )
        {
            // background color
            CHECK_CRC_FOR_ANCILLARY_CHUNK(WT_ANCILLARY_CHUNK_bKGD_IS_INCORRECT);
            CONDITIONAL_LEAVE_ANCILLARY_CHUNK((
                 !(m_eColorModel == CMT_INDEXED && !m_bHas_PLTE_Chunk)
            &&   ((m_eColorModel == CMT_GRAYSCALE && uiLength == 2)
            ||   (m_eColorModel == CMT_RGB && uiLength == 6)
            ||   (m_eColorModel == CMT_INDEXED && uiLength == 1)
            ||   (m_eColorModel == CMT_GRAYSCALE_WITH_ALPHA && uiLength == 2)
            ||   (m_eColorModel == CMT_RGBA && uiLength == 6)) ),
            WT_ANCILLARY_CHUNK_bKGD_IS_INCORRECT);

            if ( m_eColorModel == CMT_GRAYSCALE || m_eColorModel == CMT_GRAYSCALE_WITH_ALPHA )
            {
                unsigned short usGray;
                qChunkStream.Read(usGray);
                CONDITIONAL_LEAVE_ANCILLARY_CHUNK(( usGray < m_iSampleRange ),
                    WT_ANCILLARY_CHUNK_bKGD_IS_INCORRECT);

                m_iBackgroundGrayscale = usGray;
            }
            else if ( m_eColorModel == CMT_RGB || m_eColorModel == CMT_RGBA )
            {
                unsigned short usRed, usGreen, usBlue;
                qChunkStream.Read(usRed);
                qChunkStream.Read(usGreen);
                qChunkStream.Read(usBlue);
                CONDITIONAL_LEAVE_ANCILLARY_CHUNK(
                    ( usRed < m_iSampleRange && usGreen < m_iSampleRange && usBlue < m_iSampleRange ),
                    WT_ANCILLARY_CHUNK_bKGD_IS_INCORRECT);

                m_iBackgroundRed = usRed;
                m_iBackgroundGreen = usGreen;
                m_iBackgroundBlue = usBlue;
            }
            else  // m_eColorModel = CMT_INDEXED
            {
                unsigned char ucPaletteIndex;
                qChunkStream.Read(ucPaletteIndex);
                CONDITIONAL_LEAVE_ANCILLARY_CHUNK(( ucPaletteIndex < m_qPalette.GetQuantity() ),
                    WT_ANCILLARY_CHUNK_bKGD_IS_INCORRECT);

                m_iBackgroundPaletteIndex = ucPaletteIndex;
            }

            m_bHas_bKGD_Chunk = true;
        }
        else if ( MemCmp(acType,gs_auc_hIST_Type,4) == 0 )
        {
            // image histogram
            CHECK_CRC_FOR_ANCILLARY_CHUNK(WT_ANCILLARY_CHUNK_hIST_IS_INCORRECT);
            CONDITIONAL_LEAVE_ANCILLARY_CHUNK(( m_bHas_PLTE_Chunk
            &&   uiLength != 0 && (uiLength & 1) == 0 && uiLength/2 == m_qPalette.GetQuantity() ),
            WT_ANCILLARY_CHUNK_hIST_IS_INCORRECT);

            m_qHistogramFrequencies.SetQuantity(m_qPalette.GetQuantity());
            unsigned short usFrequency;
            for (int i = 0; i < m_qPalette.GetQuantity(); i++)
            {
                qChunkStream.Read(usFrequency);
                m_qHistogramFrequencies[i] = usFrequency;
            }

            m_bHas_hIST_Chunk = true;
        }
        else if ( MemCmp(acType,gs_auc_tRNS_Type,4) == 0 )
        {
            // transparency
            CHECK_CRC_FOR_ANCILLARY_CHUNK(WT_ANCILLARY_CHUNK_tRNS_IS_INCORRECT);
            CONDITIONAL_LEAVE_ANCILLARY_CHUNK((
                 !(m_eColorModel == CMT_INDEXED && !m_bHas_PLTE_Chunk)
            &&   ((m_eColorModel == CMT_GRAYSCALE && uiLength == 2)
            ||   (m_eColorModel == CMT_RGB && uiLength == 6)
            ||   (m_eColorModel == CMT_INDEXED && (int)uiLength <= m_qPalette.GetQuantity())) ),
            WT_ANCILLARY_CHUNK_tRNS_IS_INCORRECT);

            if ( m_eColorModel == CMT_GRAYSCALE )
            {
                unsigned short usTGray;
                qChunkStream.Read(usTGray);
                CONDITIONAL_LEAVE_ANCILLARY_CHUNK(( usTGray < m_iSampleRange ),
                    WT_ANCILLARY_CHUNK_tRNS_IS_INCORRECT);

                m_iTransparencyGrayscale = usTGray;
            }
            else if ( m_eColorModel == CMT_RGB )
            {
                unsigned short usTRed, usTGreen, usTBlue;
                qChunkStream.Read(usTRed);
                qChunkStream.Read(usTGreen);
                qChunkStream.Read(usTBlue);
                CONDITIONAL_LEAVE_ANCILLARY_CHUNK(
                    ( usTRed < m_iSampleRange && usTGreen < m_iSampleRange && usTBlue < m_iSampleRange ),
                    WT_ANCILLARY_CHUNK_tRNS_IS_INCORRECT);

                m_iTransparencyRed = usTRed;
                m_iTransparencyGreen = usTGreen;
                m_iTransparencyBlue = usTBlue;
            }
            else  // m_eColorModel = CMT_INDEXED
            {
                m_qPaletteAlphaAddon.SetQuantity(uiLength);
                qChunkStream.Read(uiLength,m_qPaletteAlphaAddon);
            }

            m_bHas_tRNS_Chunk = true;
        }
        else if ( MemCmp(acType,gs_auc_pHYs_Type,4) == 0 )
        {
            // physical pixel dimensions
            CHECK_CRC_FOR_ANCILLARY_CHUNK(WT_ANCILLARY_CHUNK_pHYs_IS_INCORRECT);
            CONDITIONAL_LEAVE_ANCILLARY_CHUNK(( uiLength == 9 ),WT_ANCILLARY_CHUNK_pHYs_IS_INCORRECT);

            unsigned int uiPPUX, uiPPUY;
            qChunkStream.Read(uiPPUX);
            qChunkStream.Read(uiPPUY);
            m_iPixelsPerUnitX = uiPPUX;
            m_iPixelsPerUnitY = uiPPUY;

            unsigned char ucUnit;
            qChunkStream.Read(ucUnit);
            switch ( ucUnit )
            {
            case 0:
                m_ePhysicalPixelDimensionsUnit = PPDUT_UNKNOWN;
                break;
            case 1:
                m_ePhysicalPixelDimensionsUnit = PPDUT_METER;
                break;
            default:
                LEAVE_ANCILLARY_CHUNK(WT_ANCILLARY_CHUNK_pHYs_IS_INCORRECT);
            }

            m_bHas_pHYs_Chunk = true;
        }
        else if ( MemCmp(acType,gs_auc_sPLT_Type,4) == 0 )
        {
            // suggested palette
            CHECK_CRC_FOR_ANCILLARY_CHUNK(WT_ONE_OR_MORE_ANCILLARY_CHUNKS_sPLT_ARE_INCORRECT);

            SuggestedPalette qSugPal;

            READ_AND_PROCESS_LATIN1_NONEMPTY_LIMITED80_STRING_FOR_ANCILLARY_CHUNK(4,qSugPal.Name,
                WT_ONE_OR_MORE_ANCILLARY_CHUNKS_sPLT_ARE_INCORRECT);

            unsigned char ucSampleDepth;
            qChunkStream.Read(ucSampleDepth);
            switch ( ucSampleDepth )
            {
            case 8:
                qSugPal.SampleDepth = SPSDT_8_BITS;
                break;
            case 16:
                qSugPal.SampleDepth = SPSDT_16_BITS;
                break;
            default:
                LEAVE_ANCILLARY_CHUNK(WT_ONE_OR_MORE_ANCILLARY_CHUNKS_sPLT_ARE_INCORRECT);
            }

            int iRemainder = uiLength - (qChunkStream.GetBytePosition() - 4);
            CONDITIONAL_LEAVE_ANCILLARY_CHUNK(( iRemainder > 0 ),
                WT_ONE_OR_MORE_ANCILLARY_CHUNKS_sPLT_ARE_INCORRECT);
            CONDITIONAL_LEAVE_ANCILLARY_CHUNK((
                 (qSugPal.SampleDepth == SPSDT_8_BITS && iRemainder % 6 == 0)
            ||   (qSugPal.SampleDepth == SPSDT_16_BITS && iRemainder % 10 == 0) ),
            WT_ONE_OR_MORE_ANCILLARY_CHUNKS_sPLT_ARE_INCORRECT);

            unsigned short usFrequency;
            if ( qSugPal.SampleDepth == SPSDT_8_BITS )
            {
                qSugPal.Palette.SetQuantity(iRemainder/6);

                unsigned char ucRed, ucGreen, ucBlue, ucAlpha;
                for (int i = 0; i < qSugPal.Palette.GetQuantity(); i++)
                {
                    SuggestedPaletteColor& rqSPEntry = qSugPal.Palette[i];

                    qChunkStream.Read(ucRed);
                    qChunkStream.Read(ucGreen);
                    qChunkStream.Read(ucBlue);
                    qChunkStream.Read(ucAlpha);
                    qChunkStream.Read(usFrequency);

                    rqSPEntry.Red = ucRed;
                    rqSPEntry.Green = ucGreen;
                    rqSPEntry.Blue = ucBlue;
                    rqSPEntry.Alpha = ucAlpha;
                    rqSPEntry.Frequency = usFrequency;
                }
            }
            else  // qSugPal.SampleDepth = SPSDT_16_BITS
            {
                qSugPal.Palette.SetQuantity(iRemainder/10);

                unsigned short usRed, usGreen, usBlue, usAlpha;
                for (int i = 0; i < qSugPal.Palette.GetQuantity(); i++)
                {
                    SuggestedPaletteColor& rqSPEntry = qSugPal.Palette[i];

                    qChunkStream.Read(usRed);
                    qChunkStream.Read(usGreen);
                    qChunkStream.Read(usBlue);
                    qChunkStream.Read(usAlpha);
                    qChunkStream.Read(usFrequency);

                    rqSPEntry.Red = usRed;
                    rqSPEntry.Green = usGreen;
                    rqSPEntry.Blue = usBlue;
                    rqSPEntry.Alpha = usAlpha;
                    rqSPEntry.Frequency = usFrequency;
                }
            }

            m_qSuggestedPalettes.Push(qSugPal);
        }
        else if ( MemCmp(acType,gs_auc_tIME_Type,4) == 0 )
        {
            // image last-modification time
            READ_tIME_CHUNK;
        }
        else if ( MemCmp(acType,gs_auc_tEXt_Type,4) == 0 )
        {
            // textual data
            READ_tEXt_CHUNK;
        }
        else if ( MemCmp(acType,gs_auc_zTXt_Type,4) == 0 )
        {
            // textual data
            READ_zTXt_CHUNK;
        }
        else
        {
            // Unknown chunk; check chunk type for plausibility
            for (int i = 0; i < 4; i++)
            {
                ucChar = acType[i];
                if ( !((65 <= ucChar && ucChar <= 90) || (97 <= ucChar && ucChar <= 122)) )
                    THROW_EXCEPTION(ET_FORMAT_MISMATCH);
            }

            if ( ((acType[0] >> 5) & 1) == 0 )
            {
                // unknown critical chunk
                THROW_EXCEPTION(ET_UNKNOWN_CRITICAL_CHUNK_ENCOUNTERED);
            }
        }
DoneWithThisChunk:

        qChunkStream.Finalize();
    }
    CONDITIONAL_THROW_EXCEPTION(( !(m_eColorModel == CMT_INDEXED && !m_bHas_PLTE_Chunk) ),
        ET_FORMAT_MISMATCH);

    // assign helping derived attributes
    COMPUTE_EXPECTED_IMAGE_SIZE_AND_RELATED_VALUES;

    return true;
}
//------------------------------------------------------------------------------------------------------------------
void FormatPng::UnfilterRow (int iRow, unsigned char* pucRow, const unsigned char* aucUpRow,
    int iRowSizeWithFB) const
{
    int iFilterType = ( *pucRow < 5 ? *pucRow : 0 );
    if ( iFilterType == 0 )
    {
        // none
        return;
    }
    else if ( iFilterType == 1 )
    {
        // sub
        int iA;

        if ( m_eColorModel == CMT_INDEXED || m_iBitDepth < 8
        ||   (m_iBitDepth == 8 && m_eColorModel == CMT_GRAYSCALE) )
        {
            for (int i = 1; i < iRowSizeWithFB; i++)
            {
                iA = ( i > 1 ? *(pucRow + i - 1) : 0 );
                *(pucRow + i) = *(pucRow + i) + iA;
            }
        }
        else
        {
            if ( m_eColorModel == CMT_GRAYSCALE )
            {
                // m_iBitDepth = 16
                for (int i = 1; i < iRowSizeWithFB; i++)
                {
                    iA = ( i > 2 ? *(pucRow + i - 2) : 0 );
                    *(pucRow + i) = *(pucRow + i) + iA;
                }
            }
            else if ( m_eColorModel == CMT_RGB )
            {
                if ( m_iBitDepth == 8 )
                {
                    for (int i = 1; i < iRowSizeWithFB; i++)
                    {
                        iA = ( i > 3 ? *(pucRow + i - 3) : 0 );
                        *(pucRow + i) = *(pucRow + i) + iA;
                    }
                }
                else  // m_iBitDepth = 16
                {
                    for (int i = 1; i < iRowSizeWithFB; i++)
                    {
                        iA = ( i > 6 ? *(pucRow + i - 6) : 0 );
                        *(pucRow + i) = *(pucRow + i) + iA;
                    }
                }
            }
            else if ( m_eColorModel == CMT_GRAYSCALE_WITH_ALPHA )
            {
                if ( m_iBitDepth == 8 )
                {
                    for (int i = 1; i < iRowSizeWithFB; i++)
                    {
                        iA = ( i > 2 ? *(pucRow + i - 2) : 0 );
                        *(pucRow + i) = *(pucRow + i) + iA;
                    }
                }
                else  // m_iBitDepth = 16
                {
                    for (int i = 1; i < iRowSizeWithFB; i++)
                    {
                        iA = ( i > 4 ? *(pucRow + i - 4) : 0 );
                        *(pucRow + i) = *(pucRow + i) + iA;
                    }
                }
            }
            else  // m_eColorModel = CMT_RGBA
            {
                if ( m_iBitDepth == 8 )
                {
                    for (int i = 1; i < iRowSizeWithFB; i++)
                    {
                        iA = ( i > 4 ? *(pucRow + i - 4) : 0 );
                        *(pucRow + i) = *(pucRow + i) + iA;
                    }
                }
                else  // m_iBitDepth = 16
                {
                    for (int i = 1; i < iRowSizeWithFB; i++)
                    {
                        iA = ( i > 8 ? *(pucRow + i - 8) : 0 );
                        *(pucRow + i) = *(pucRow + i) + iA;
                    }
                }
            }
        }
    }
    else if ( iFilterType == 2 )
    {
        // up
        int iB;

        for (int i = 1; i < iRowSizeWithFB; i++)
        {
            iB = ( iRow != 0 ? *(aucUpRow + i) : 0 );
            *(pucRow + i) = *(pucRow + i) + iB;
        }
    }
    else if ( iFilterType == 3 )
    {
        // average
        int iA, iB;

        if ( m_eColorModel == CMT_INDEXED || m_iBitDepth < 8
        ||   (m_iBitDepth == 8 && m_eColorModel == CMT_GRAYSCALE) )
        {
            for (int i = 1; i < iRowSizeWithFB; i++)
            {
                iA = ( i > 1 ? *(pucRow + i - 1) : 0 );
                iB = ( iRow != 0 ? *(aucUpRow + i) : 0 );
                *(pucRow + i) = *(pucRow + i) + (iA + iB)/2;
            }
        }
        else
        {
            if ( m_eColorModel == CMT_GRAYSCALE )
            {
                // m_iBitDepth = 16
                for (int i = 1; i < iRowSizeWithFB; i++)
                {
                    iA = ( i > 2 ? *(pucRow + i - 2) : 0 );
                    iB = ( iRow != 0 ? *(aucUpRow + i) : 0 );
                    *(pucRow + i) = *(pucRow + i) + (iA + iB)/2;
                }
            }
            else if ( m_eColorModel == CMT_RGB )
            {
                if ( m_iBitDepth == 8 )
                {
                    for (int i = 1; i < iRowSizeWithFB; i++)
                    {
                        iA = ( i > 3 ? *(pucRow + i - 3) : 0 );
                        iB = ( iRow != 0 ? *(aucUpRow + i) : 0 );
                        *(pucRow + i) = *(pucRow + i) + (iA + iB)/2;
                    }
                }
                else  // m_iBitDepth = 16
                {
                    for (int i = 1; i < iRowSizeWithFB; i++)
                    {
                        iA = ( i > 6 ? *(pucRow + i - 6) : 0 );
                        iB = ( iRow != 0 ? *(aucUpRow + i) : 0 );
                        *(pucRow + i) = *(pucRow + i) + (iA + iB)/2;
                    }
                }
            }
            else if ( m_eColorModel == CMT_GRAYSCALE_WITH_ALPHA )
            {
                if ( m_iBitDepth == 8 )
                {
                    for (int i = 1; i < iRowSizeWithFB; i++)
                    {
                        iA = ( i > 2 ? *(pucRow + i - 2) : 0 );
                        iB = ( iRow != 0 ? *(aucUpRow + i) : 0 );
                        *(pucRow + i) = *(pucRow + i) + (iA + iB)/2;
                    }
                }
                else  // m_iBitDepth = 16
                {
                    for (int i = 1; i < iRowSizeWithFB; i++)
                    {
                        iA = ( i > 4 ? *(pucRow + i - 4) : 0 );
                        iB = ( iRow != 0 ? *(aucUpRow + i) : 0 );
                        *(pucRow + i) = *(pucRow + i) + (iA + iB)/2;
                    }
                }
            }
            else  // m_eColorModel = CMT_RGBA
            {
                if ( m_iBitDepth == 8 )
                {
                    for (int i = 1; i < iRowSizeWithFB; i++)
                    {
                        iA = ( i > 4 ? *(pucRow + i - 4) : 0 );
                        iB = ( iRow != 0 ? *(aucUpRow + i) : 0 );
                        *(pucRow + i) = *(pucRow + i) + (iA + iB)/2;
                    }
                }
                else  // m_iBitDepth = 16
                {
                    for (int i = 1; i < iRowSizeWithFB; i++)
                    {
                        iA = ( i > 8 ? *(pucRow + i - 8) : 0 );
                        iB = ( iRow != 0 ? *(aucUpRow + i) : 0 );
                        *(pucRow + i) = *(pucRow + i) + (iA + iB)/2;
                    }
                }
            }
        }
    }
    else  // iFilterType = 4
    {
        // Paeth
        int iA, iB, iC;

        if ( m_eColorModel == CMT_INDEXED || m_iBitDepth < 8
        ||   (m_iBitDepth == 8 && m_eColorModel == CMT_GRAYSCALE) )
        {
            for (int i = 1; i < iRowSizeWithFB; i++)
            {
                iA = ( i > 1 ? *(pucRow + i - 1) : 0 );
                iB = ( iRow != 0 ? *(aucUpRow + i) : 0 );
                iC = ( i > 1 && iRow != 0 ? *(aucUpRow + i - 1) : 0 );
                *(pucRow + i) = *(pucRow + i) + PredictPaeth(iA,iB,iC);
            }
        }
        else
        {
            if ( m_eColorModel == CMT_GRAYSCALE )
            {
                // m_iBitDepth = 16
                for (int i = 1; i < iRowSizeWithFB; i++)
                {
                    iA = ( i > 2 ? *(pucRow + i - 2) : 0 );
                    iB = ( iRow != 0 ? *(aucUpRow + i) : 0 );
                    iC = ( i > 2 && iRow != 0 ? *(aucUpRow + i - 2) : 0 );
                    *(pucRow + i) = *(pucRow + i) + PredictPaeth(iA,iB,iC);
                }
            }
            else if ( m_eColorModel == CMT_RGB )
            {
                if ( m_iBitDepth == 8 )
                {
                    for (int i = 1; i < iRowSizeWithFB; i++)
                    {
                        iA = ( i > 3 ? *(pucRow + i - 3) : 0 );
                        iB = ( iRow != 0 ? *(aucUpRow + i) : 0 );
                        iC = ( i > 3 && iRow != 0 ? *(aucUpRow + i - 3) : 0 );
                        *(pucRow + i) = *(pucRow + i) + PredictPaeth(iA,iB,iC);
                    }
                }
                else  // m_iBitDepth = 16
                {
                    for (int i = 1; i < iRowSizeWithFB; i++)
                    {
                        iA = ( i > 6 ? *(pucRow + i - 6) : 0 );
                        iB = ( iRow != 0 ? *(aucUpRow + i) : 0 );
                        iC = ( i > 6 && iRow != 0 ? *(aucUpRow + i - 6) : 0 );
                        *(pucRow + i) = *(pucRow + i) + PredictPaeth(iA,iB,iC);
                    }
                }
            }
            else if ( m_eColorModel == CMT_GRAYSCALE_WITH_ALPHA )
            {
                if ( m_iBitDepth == 8 )
                {
                    for (int i = 1; i < iRowSizeWithFB; i++)
                    {
                        iA = ( i > 2 ? *(pucRow + i - 2) : 0 );
                        iB = ( iRow != 0 ? *(aucUpRow + i) : 0 );
                        iC = ( i > 2 && iRow != 0 ? *(aucUpRow + i - 2) : 0 );
                        *(pucRow + i) = *(pucRow + i) + PredictPaeth(iA,iB,iC);
                    }
                }
                else  // m_iBitDepth = 16
                {
                    for (int i = 1; i < iRowSizeWithFB; i++)
                    {
                        iA = ( i > 4 ? *(pucRow + i - 4) : 0 );
                        iB = ( iRow != 0 ? *(aucUpRow + i) : 0 );
                        iC = ( i > 4 && iRow != 0 ? *(aucUpRow + i - 4) : 0 );
                        *(pucRow + i) = *(pucRow + i) + PredictPaeth(iA,iB,iC);
                    }
                }
            }
            else  // m_eColorModel = CMT_RGBA
            {
                if ( m_iBitDepth == 8 )
                {
                    for (int i = 1; i < iRowSizeWithFB; i++)
                    {
                        iA = ( i > 4 ? *(pucRow + i - 4) : 0 );
                        iB = ( iRow != 0 ? *(aucUpRow + i) : 0 );
                        iC = ( i > 4 && iRow != 0 ? *(aucUpRow + i - 4) : 0 );
                        *(pucRow + i) = *(pucRow + i) + PredictPaeth(iA,iB,iC);
                    }
                }
                else  // m_iBitDepth = 16
                {
                    for (int i = 1; i < iRowSizeWithFB; i++)
                    {
                        iA = ( i > 8 ? *(pucRow + i - 8) : 0 );
                        iB = ( iRow != 0 ? *(aucUpRow + i) : 0 );
                        iC = ( i > 8 && iRow != 0 ? *(aucUpRow + i - 8) : 0 );
                        *(pucRow + i) = *(pucRow + i) + PredictPaeth(iA,iB,iC);
                    }
                }
            }
        }
    }
}
//------------------------------------------------------------------------------------------------------------------
void FormatPng::ReadPixel (const char* acDecompressedData, int& riBytePos, int& riBitRatio,
    const RasterImagePtr& rspqImage, int iX, int iY) const
{
    char* pcDstSample = rspqImage->GetRow(iY) + iX*rspqImage->GetBytesPerPixel();

    int iValue;
    unsigned short usValue;
    int aiOrigValue[4];
    for (int i = 0; i < m_iSamplesPerPixel; i++)
    {
        if ( m_iBitDepth <= 8 )
        {
            riBytePos += Storage::ReadUBits(acDecompressedData+riBytePos,riBitRatio,m_iBitDepth,iValue);
        }
        else  // m_iBitDepth = 16
        {
            riBytePos += Storage::Read2be(acDecompressedData+riBytePos,1,&usValue);
            iValue = usValue;
        }
        aiOrigValue[i] = iValue;

        if ( m_bHas_sBIT_Chunk && m_eColorModel != CMT_INDEXED )
        {
            if ( m_eColorModel == CMT_GRAYSCALE )
            {
                iValue = Mathf::RoundToInt(((float)(iValue >> m_iShiftVGray))/m_iMaxVGray*m_iSampleMaxValue);
            }
            else if ( m_eColorModel == CMT_RGB )
            {
                switch ( i )
                {
                case 0:
                    iValue = Mathf::RoundToInt(((float)(iValue >> m_iShiftVRed))/m_iMaxVRed*m_iSampleMaxValue);
                    break;
                case 1:
                    iValue = Mathf::RoundToInt(((float)(iValue >> m_iShiftVGreen))/m_iMaxVGreen*m_iSampleMaxValue);
                    break;
                case 2:
                    iValue = Mathf::RoundToInt(((float)(iValue >> m_iShiftVBlue))/m_iMaxVBlue*m_iSampleMaxValue);
                    break;
                }
            }
            else if ( m_eColorModel == CMT_GRAYSCALE_WITH_ALPHA )
            {
                switch ( i )
                {
                case 0:
                    iValue = Mathf::RoundToInt(((float)(iValue >> m_iShiftVGray))/m_iMaxVGray*m_iSampleMaxValue);
                    break;
                case 1:
                    iValue = Mathf::RoundToInt(((float)(iValue >> m_iShiftVAlpha))/m_iMaxVAlpha*m_iSampleMaxValue);
                    break;
                }
            }
            else  // m_eColorModel = CMT_RGBA
            {
                switch ( i )
                {
                case 0:
                    iValue = Mathf::RoundToInt(((float)(iValue >> m_iShiftVRed))/m_iMaxVRed*m_iSampleMaxValue);
                    break;
                case 1:
                    iValue = Mathf::RoundToInt(((float)(iValue >> m_iShiftVGreen))/m_iMaxVGreen*m_iSampleMaxValue);
                    break;
                case 2:
                    iValue = Mathf::RoundToInt(((float)(iValue >> m_iShiftVBlue))/m_iMaxVBlue*m_iSampleMaxValue);
                    break;
                case 3:
                    iValue = Mathf::RoundToInt(((float)(iValue >> m_iShiftVAlpha))/m_iMaxVAlpha*m_iSampleMaxValue);
                    break;
                }
            }
        }

        if ( m_eColorModel != CMT_INDEXED )
        {
            if ( m_iBitDepth != rspqImage->GetBitsPerIntegerSample() )
                iValue = Mathf::RoundToInt(((float)iValue)/m_iSampleMaxValue*m_iImgSampleMaxValue);

#ifndef Q_BIG_ENDIAN
            MemCpy(pcDstSample,&iValue,m_iBytesPerImgSample);
#else
            MemCpy(pcDstSample,&iValue+(4-m_iBytesPerImgSample),m_iBytesPerImgSample);
#endif
        }
        else
        {
            *pcDstSample = iValue;
        }

        pcDstSample += m_iBytesPerImgSample;
    }

    if ( m_bHas_tRNS_Chunk && m_eColorModel != CMT_INDEXED )
    {
        if ( m_eColorModel == CMT_GRAYSCALE )
        {
            rspqImage->GetVisibilityMask()[iY*m_iWidth+iX] =
                ( aiOrigValue[0] == m_iTransparencyGrayscale ? 0 : 1 );
        }
        else  // m_eColorModel = CMT_RGB
        {
            rspqImage->GetVisibilityMask()[iY*m_iWidth+iX] = (
                 aiOrigValue[0] == m_iTransparencyRed
            &&   aiOrigValue[1] == m_iTransparencyGreen
            &&   aiOrigValue[2] == m_iTransparencyBlue ? 0 : 1 );
        }
    }
}
//------------------------------------------------------------------------------------------------------------------



