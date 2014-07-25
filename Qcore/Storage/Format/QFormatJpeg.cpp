#include "QFormatJpeg.h"
#include "QExptWarnBase.mcr"
#include "QFormatBase.mcr"
#include "QProgressProbe.h"
#include "QStorage.h"
#include "QDiscreteCosineTransformi.h"
#include "QRasterImageTransformation.h"
#include "QColorModelConversion.h"
using namespace Q;

static const int gs_aiLuminanceQuantizationTable[64] = {
    16,  11,  10,  16,  24,  40,  51,  61,
    12,  12,  14,  19,  26,  58,  60,  55,
    14,  13,  16,  24,  40,  57,  69,  56,
    14,  17,  22,  29,  51,  87,  80,  62,
    18,  22,  37,  56,  68, 109, 103,  77,
    24,  35,  55,  64,  81, 104, 113,  92,
    49,  64,  78,  87, 103, 121, 120, 101,
    72,  92,  95,  98, 112, 100, 103,  99};

static const int gs_aiChrominanceQuantizationTable[64] = {
    17,  18,  24,  47,  99,  99,  99,  99,
    18,  21,  26,  66,  99,  99,  99,  99,
    24,  26,  56,  99,  99,  99,  99,  99,
    47,  66,  99,  99,  99,  99,  99,  99,
    99,  99,  99,  99,  99,  99,  99,  99,
    99,  99,  99,  99,  99,  99,  99,  99,
    99,  99,  99,  99,  99,  99,  99,  99,
    99,  99,  99,  99,  99,  99,  99,  99};

static const int gs_aiInvZigZag[64] = {
     0,  1,  8, 16,  9,  2,  3, 10,
    17, 24, 32, 25, 18, 11,  4,  5,
    12, 19, 26, 33, 40, 48, 41, 34,
    27, 20, 13,  6,  7, 14, 21, 28,
    35, 42, 49, 56, 57, 50, 43, 36,
    29, 22, 15, 23, 30, 37, 44, 51,
    58, 59, 52, 45, 38, 31, 39, 46,
    53, 60, 61, 54, 47, 55, 62, 63};

const int FormatJpeg::ms_iBitBuffSize(1024);

//------------------------------------------------------------------------------------------------------------------
FormatJpeg::FormatJpeg (const RasterImagePtr& rspqImage)
    :
    m_spqImageSaveTo(rspqImage),
    m_iWidth(rspqImage->GetWidth()),
    m_iHeight(rspqImage->GetHeight()),
    m_eDataOrganization(DOT_PLANE),
    m_fQuality(0.75f),
    m_eYCbCrChromaSampling(YCCCST_4_4_4),
    m_iProgressiveScanQuantity(5),
    m_bHasJfifMarker(true),
    m_eJfifDotDensityMeaning(JFIFPDMT_PIXELS_PER_INCH),
    m_iJfifDotDensityHorizontally(72),
    m_iJfifDotDensityVertically(72),
    m_bColorModelPrecised(false),
    m_bHasAdobeMarker(true),
    m_bHasExifMetadata(false)
{
    assert( rspqImage->GetWidth() != 0 && rspqImage->GetHeight() != 0 );
    assert( rspqImage->GetWidth() <= 65535 && rspqImage->GetHeight() <= 65535 );
    assert( m_spqImageSaveTo->GetColorModel() == RasterImage::CMT_GRAYSCALE
        ||  m_spqImageSaveTo->GetColorModel() == RasterImage::CMT_RGB
        ||  m_spqImageSaveTo->GetColorModel() == RasterImage::CMT_CMYK );
}
//------------------------------------------------------------------------------------------------------------------
bool FormatJpeg::SaveImage (const char* acFilename)
{
    Stream qSaveStream(acFilename,false);
    WriteImageData(qSaveStream);

    // flush to the file
    CONDITIONAL_THROW_EXCEPTION(qSaveStream.Finalize(),ET_CANNOT_ACCESS_FILE);

    return true;
}
//------------------------------------------------------------------------------------------------------------------
void FormatJpeg::SaveImage (StillArray<char>& rqStaticBytestream)
{
    rqStaticBytestream.RemoveAll();

    Stream qSaveStream(false);
    qSaveStream.SetDoNotFreeBufferAfterComplete();
    WriteImageData(qSaveStream);
    qSaveStream.Finalize();  // calls AlignBytes

    rqStaticBytestream.BecomeOwner((char*)qSaveStream.GetBytes(),qSaveStream.GetBytePosition());
}
//------------------------------------------------------------------------------------------------------------------
FormatJpeg::FormatJpeg (const char* acFilename, bool& rbSuccess)
    :
    FormatBase(acFilename)
{
    rbSuccess = LoadHeadingData();
}
//------------------------------------------------------------------------------------------------------------------
FormatJpeg::FormatJpeg (bool bBytestreamIsStatic, const char* acBytestream, int iBytestreamSize, bool& rbSuccess)
    :
    FormatBase(bBytestreamIsStatic,acBytestream,( bBytestreamIsStatic ? iBytestreamSize : -1 ))
{
    if ( !bBytestreamIsStatic )
    {
        m_iDevelopedRowQuantity = 0;
        m_iCurrentPassIndex = 0;
    }

    rbSuccess = LoadHeadingData(iBytestreamSize);
}
//------------------------------------------------------------------------------------------------------------------
bool FormatJpeg::LoadImage (RasterImagePtr& rspqImage, int iDynamicBytestreamSize)
{
    assert( ( m_eStreaming == Stream::ST_READING_FROM_DYNAMIC_BYTESTREAM ) == ( iDynamicBytestreamSize >= 0 ) );

    int iBytestreamSize = ( m_eStreaming == Stream::ST_READING_FROM_STATIC_BYTESTREAM ? m_iStaticBytestreamSize :
        iDynamicBytestreamSize );

NextPass:

    bool bSuccess;

    int iDataSize;
    if ( m_eStreaming == Stream::ST_READING_FROM_FILE )
        CONDITIONAL_THROW_EXCEPTION(Storage::GetFileSize(iDataSize,m_qFilename),ET_CANNOT_ACCESS_FILE)
    else
        iDataSize = iBytestreamSize;

    Stream qLoadStream(m_eStreaming,m_acStreamSrcParam,2,iDataSize-2,false,bSuccess,iBytestreamSize);
    PROCESS_STREAM_SUCCESS_FOR_LOADING(bSuccess);

    if ( !m_bImageInitialized )
    {
        if ( m_eColorModel == CMT_GRAYSCALE )
        {
            if ( m_eStreaming == Stream::ST_READING_FROM_DYNAMIC_BYTESTREAM )
                rspqImage = new RasterImage(RasterImage::IT_GRAYSCALE_8,m_iWidth,m_iHeight);
        }
        else if ( m_eColorModel == CMT_YCC || m_eColorModel == CMT_RGB )
        {
            rspqImage = new RasterImage(RasterImage::IT_RGB_8_8_8,m_iWidth,m_iHeight);
        }
        else  // m_eColorModel = CMT_YCCK or m_eColorModel = CMT_CMYK
        {
            rspqImage = new RasterImage(RasterImage::IT_CMYK_8_8_8_8,m_iWidth,m_iHeight);
        }

        if ( m_eProcess == PT_PROGRESSIVE_DCT )
        {
            m_qComponentBuffers.SetQuantity(m_iComponentQuantity);
            m_qProgressionRecordsPerComponent.SetQuantity(m_iComponentQuantity);
            for (int i = 0; i < m_iComponentQuantity; i++)
            {
                m_qProgressionRecordsPerComponent[i].First = MakeLink(0,0);
                m_qProgressionRecordsPerComponent[i].Second = MakeLink(-1,0);
            }
        }

        // color conversion tables
        if ( m_eColorModel == CMT_YCC || m_eColorModel == CMT_YCCK )
        {
            for (int i0 = 0, i1 = -128; i0 < 256; i0++, i1++)
            {
                m_aiCrInR[i0] = Mathf::RoundToInt(ColorModelConversion::YccToRgbCrInR*i1);
                m_aiCbInG[i0] = Mathf::RoundToInt(ColorModelConversion::YccToRgbCbInG*i1);
                m_aiCrInG[i0] = Mathf::RoundToInt(ColorModelConversion::YccToRgbCrInG*i1);
                m_aiCbInB[i0] = Mathf::RoundToInt(ColorModelConversion::YccToRgbCbInB*i1);
            }
        }

        m_bImageInitialized = true;
    }

    unsigned char ucByte, ucComponentSelector, ucB = 0;
    int iSuccess, iI, iCheckSum, iDcHuffTableDestSelector, iAcHuffTableDestSelector, iSumOfBlocks, iTotalSFArea,
        iWidthInDataUnits, iHeightInDataUnits, iInDataUnitsArea, iLTXP8, iLTYP8, iEobRun, iT, iK, iSsss, iR,
        iCoefficient, iDiff, iRs, iRrrr, iBit, iNNZQ, iE, iP, iEOSPO, i8MSABPL, iCnt, iComponentIndexInFrame,
        iLTCYM8;
    short* psCoefficient;
    bool bComponentInFrameFound, bProgressiveDecodingBreak, bEndOfMcuRow;
    int aiNNZBlock[64];
    char* acRow;
    float fProgressiveDone;

    // for a progress bar
    float fAlreadyDone;
    if ( m_eStreaming != Stream::ST_READING_FROM_DYNAMIC_BYTESTREAM )
    {
        fAlreadyDone = 0.0f;
    }
    else
    {
        if ( m_eProcess != PT_PROGRESSIVE_DCT )
        {
            // non-progressive
            fAlreadyDone = ((float)m_iDevelopedRowQuantity)/m_iHeight;
        }
        else
        {
            // progressive
            fProgressiveDone = 0.0f;
            for (int i = 0; i < m_iComponentQuantity; i++)
            {
                const Link<Link<int,int>,Link<int,int> >& rqPDR = m_qProgressionRecordsPerComponent[i];

                fProgressiveDone +=
                    (((float)rqPDR.First.First)/1*((float)rqPDR.First.Second)/8 +
                    ((float)rqPDR.Second.First)/63*((float)rqPDR.Second.Second)/8)/2/m_iComponentQuantity;
            }
            fAlreadyDone = fProgressiveDone;
        }
    }
    ProgressProbe qProgress(this,fAlreadyDone);

    if ( m_eStreaming == Stream::ST_READING_FROM_DYNAMIC_BYTESTREAM )
    {
        qLoadStream.OffsetBytePosition(m_iDynamicBytestreamResumeBytePosition);
        switch ( m_eResumeZone )
        {
        case IZT_TABLES_AND_MISC:
            break;  // goes next
        case IZT_NON_PROGRESSIVE_MCU:
            ucB = m_ucResumeB;
            iCnt = m_iResumeCnt;
            goto NonProgressiveMcuResumeZone;
        case IZT_PROGRESSIVE_DC_INITIAL_MCU:
            ucB = m_ucResumeB;
            iCnt = m_iResumeCnt;
            goto ProgressiveDcInitialMcuResumeZone;
        case IZT_PROGRESSIVE_DC_SUCCEEDING_MCU:
            ucB = m_ucResumeB;
            iCnt = m_iResumeCnt;
            goto ProgressiveDcSucceedingMcuResumeZone;
        case IZT_PROGRESSIVE_AC_INITIAL_MCU:
            ucB = m_ucResumeB;
            iCnt = m_iResumeCnt;
            iEobRun = m_iResumeEobRun;
            goto ProgressiveAcInitialMcuResumeZone;
        case IZT_PROGRESSIVE_AC_SUCCEEDING_MCU:
            ucB = m_ucResumeB;
            iCnt = m_iResumeCnt;
            iEobRun = m_iResumeEobRun;
            if ( m_asBlockPtr )
                MemCpy(m_asBlockPtr,m_asResumeBlock,128);
            goto ProgressiveAcSucceedingMcuResumeZone;
        case IZT_RESTART_INTERVAL:
            goto RestartIntervalResumeZone;
        case IZT_END_OF_SCAN:
            goto EndOfScanResumeZone;
        }
    }

    for (/**/; /**/; /**/)
    {
        if ( m_eStreaming == Stream::ST_READING_FROM_DYNAMIC_BYTESTREAM )
        {
            m_eResumeZone = IZT_TABLES_AND_MISC;
            m_iDynamicBytestreamResumeBytePosition = qLoadStream.GetBytePosition();
        }

        iSuccess = ReachMarkerThroughTablesAndMisc(218,qLoadStream);
        PROCESS_INT_SUCCESS_LEVEL_HIGH(iSuccess);

        // Begin processing the new scan.
        qLoadStream.OffsetBytePosition(2);
        RETURN_ON_FAIL(qLoadStream.Read(m_ucInScanComponentQuantity));

        CONDITIONAL_THROW_EXCEPTION(( 1 <= m_ucInScanComponentQuantity && m_ucInScanComponentQuantity <= 4 ),
            ET_FORMAT_MISMATCH);

        CONDITIONAL_THROW_EXCEPTION_WITHOUT_ASSERT_FALSE(( !(
             m_eStreaming == Stream::ST_READING_FROM_DYNAMIC_BYTESTREAM && m_eProcess != PT_PROGRESSIVE_DCT
        &&   m_ucInScanComponentQuantity != m_iComponentQuantity) ),
            ET_COMPONENTS_ARE_NOT_INTERLEAVED);

        m_qScanCompSpecParamsArray.SetQuantity(m_ucInScanComponentQuantity);
        iCheckSum = 0;
        for (int i0 = 0; i0 < m_ucInScanComponentQuantity; i0++)
        {
            ScanCompSpecParams& rqSCSP = m_qScanCompSpecParamsArray[i0];

            RETURN_ON_FAIL(qLoadStream.Read(ucComponentSelector));
            bComponentInFrameFound = false;
            for (int i1 = 0; i1 < m_iComponentQuantity; i1++)
            {
                if ( ucComponentSelector == m_qFrameCompSpecParamsArray[i1].ComponentIdentifier )
                {
                    bComponentInFrameFound = true;
                    iComponentIndexInFrame = i1;
                    break;
                }
            }
            CONDITIONAL_THROW_EXCEPTION(bComponentInFrameFound,ET_FORMAT_MISMATCH);

            rqSCSP.ComponentIndexInFrame = iComponentIndexInFrame;
            rqSCSP.FrameCompSpecParamsPtr = &m_qFrameCompSpecParamsArray[iComponentIndexInFrame];
            iCheckSum +=
                rqSCSP.FrameCompSpecParamsPtr->SamplingFactorH*
                rqSCSP.FrameCompSpecParamsPtr->SamplingFactorV;
            CONDITIONAL_THROW_EXCEPTION(( !(m_ucInScanComponentQuantity != 1 && iCheckSum > 10) ),
                ET_FORMAT_MISMATCH);

            RETURN_ON_FAIL(qLoadStream.ReadUBits(iDcHuffTableDestSelector,4));
            RETURN_ON_FAIL(qLoadStream.ReadUBits(iAcHuffTableDestSelector,4));

            CONDITIONAL_THROW_EXCEPTION((
                 !(m_eProcess == PT_BASELINE_DCT && (iDcHuffTableDestSelector > 1 || iAcHuffTableDestSelector > 1))
            &&   !((m_eProcess == PT_EXTENDED_DCT || m_eProcess == PT_PROGRESSIVE_DCT)
            &&   (iDcHuffTableDestSelector > 3 || iAcHuffTableDestSelector > 3)) ),ET_FORMAT_MISMATCH);

            rqSCSP.DcHuffmanTableSetPtr = &m_aaqHuffmanTableDestination[0][iDcHuffTableDestSelector];
            rqSCSP.AcHuffmanTableSetPtr = &m_aaqHuffmanTableDestination[1][iAcHuffTableDestSelector];
            CONDITIONAL_THROW_EXCEPTION(( !(m_eProcess != PT_PROGRESSIVE_DCT
            &&   (rqSCSP.DcHuffmanTableSetPtr->HuffValList.IsEmpty()
            ||   rqSCSP.AcHuffmanTableSetPtr->HuffValList.IsEmpty())) ),ET_FORMAT_MISMATCH);

            if ( !m_qComponentImages[iComponentIndexInFrame] )
            {
                // create component's own image
                m_qComponentImages[iComponentIndexInFrame] = new RasterImage(RasterImage::IT_GRAYSCALE_8,
                    (int)Mathf::Ceil(m_iWidth*((float)rqSCSP.FrameCompSpecParamsPtr->SamplingFactorH)/
                    m_iMaxSamplingFactorH),
                    (int)Mathf::Ceil(m_iHeight*((float)rqSCSP.FrameCompSpecParamsPtr->SamplingFactorV)/
                    m_iMaxSamplingFactorV));
            }
        }

        RETURN_ON_FAIL(qLoadStream.Read(m_ucStartOfSpectral));
        RETURN_ON_FAIL(qLoadStream.Read(m_ucEndOfSpectral));

        CONDITIONAL_THROW_EXCEPTION((
             !((m_eProcess == PT_BASELINE_DCT || m_eProcess == PT_EXTENDED_DCT)
        &&   (m_ucStartOfSpectral != 0 || m_ucEndOfSpectral > 63))
        &&   !(m_eProcess == PT_PROGRESSIVE_DCT
        &&   (m_ucStartOfSpectral > 63 || !(m_ucStartOfSpectral <= m_ucEndOfSpectral
        &&   m_ucEndOfSpectral <= ( m_ucStartOfSpectral != 0 ? 63 : 0 )))) ),ET_FORMAT_MISMATCH);

        RETURN_ON_FAIL(qLoadStream.ReadUBits(m_iSuccApproxBitPosHigh,4));
        RETURN_ON_FAIL(qLoadStream.ReadUBits(m_iSuccApproxBitPosLow,4));

        CONDITIONAL_THROW_EXCEPTION((
             !((m_eProcess == PT_BASELINE_DCT || m_eProcess == PT_EXTENDED_DCT)
        &&   (m_iSuccApproxBitPosHigh != 0 || m_iSuccApproxBitPosLow != 0))
        &&   !(m_eProcess == PT_PROGRESSIVE_DCT
        &&   (m_iSuccApproxBitPosHigh > 13 || m_iSuccApproxBitPosLow > 13)) ),ET_FORMAT_MISMATCH);

        if ( m_eProcess == PT_PROGRESSIVE_DCT )
        {
            // progressive
            // needed by a progress bar
            if ( m_ucStartOfSpectral == 0 )
            {
                // DC
                for (int i = 0; i < m_ucInScanComponentQuantity; i++)
                {
                    const ScanCompSpecParams& rqSCSP = m_qScanCompSpecParamsArray[i];

                    iEOSPO = m_ucEndOfSpectral + 1;
                    i8MSABPL = 8 - m_iSuccApproxBitPosLow;
                    if ( iEOSPO >
                        m_qProgressionRecordsPerComponent[rqSCSP.ComponentIndexInFrame].First.First )
                    {
                        m_qProgressionRecordsPerComponent[rqSCSP.ComponentIndexInFrame].First.First =
                            iEOSPO;
                    }
                    if ( i8MSABPL >
                        m_qProgressionRecordsPerComponent[rqSCSP.ComponentIndexInFrame].First.Second )
                    {
                        m_qProgressionRecordsPerComponent[rqSCSP.ComponentIndexInFrame].First.Second =
                            i8MSABPL;
                    }
                }
            }
            else
            {
                // AC
                for (int i = 0; i < m_ucInScanComponentQuantity; i++)
                {
                    const ScanCompSpecParams& rqSCSP = m_qScanCompSpecParamsArray[i];

                    i8MSABPL = 8 - m_iSuccApproxBitPosLow;
                    if ( m_ucEndOfSpectral >
                        m_qProgressionRecordsPerComponent[rqSCSP.ComponentIndexInFrame].Second.First )
                    {
                        m_qProgressionRecordsPerComponent[rqSCSP.ComponentIndexInFrame].Second.First =
                            m_ucEndOfSpectral;
                    }
                    if ( i8MSABPL >
                        m_qProgressionRecordsPerComponent[rqSCSP.ComponentIndexInFrame].Second.Second )
                    {
                        m_qProgressionRecordsPerComponent[rqSCSP.ComponentIndexInFrame].Second.Second =
                            i8MSABPL;
                    }
                }
            }
        }

        // find out number of MCUs in this scan
        if ( m_ucInScanComponentQuantity == 1 )
        {
            // non-interleaved
            ScanCompSpecParams& rqSCSP = m_qScanCompSpecParamsArray[0];
            const RasterImagePtr& rspqComponentImage = m_qComponentImages[rqSCSP.ComponentIndexInFrame];

            // sample quantity must be multiple of 8
            iWidthInDataUnits = ( (rspqComponentImage->GetWidth() & 7) == 0 ?
                rspqComponentImage->GetWidth()/8 : rspqComponentImage->GetWidth()/8 + 1 );
            iHeightInDataUnits = ( (rspqComponentImage->GetHeight() & 7) == 0 ?
                rspqComponentImage->GetHeight()/8 : rspqComponentImage->GetHeight()/8 + 1 );

            rqSCSP.WidthInDataUnits = iWidthInDataUnits;

            if ( m_eProcess == PT_PROGRESSIVE_DCT
            &&   m_qComponentBuffers[rqSCSP.ComponentIndexInFrame].IsEmpty() )
            {
                // create component DCT coefficient buffer for progression
                iInDataUnitsArea = iWidthInDataUnits*iHeightInDataUnits;
                StillArray<StillArray<short> >& rqBuffsBlockArray =
                    m_qComponentBuffers[rqSCSP.ComponentIndexInFrame];
                rqBuffsBlockArray.SetQuantity(iInDataUnitsArea);
                for (int i = 0; i < iInDataUnitsArea; i++)
                {
                    rqBuffsBlockArray[i].SetQuantity(64);
                    MemSet(rqBuffsBlockArray[i],0,128);
                }
                rqSCSP.FrameCompSpecParamsPtr->ProgressiveBufferWidthInDataUnits = iWidthInDataUnits;
            }

            m_iInScanMcuQuantity = iWidthInDataUnits*iHeightInDataUnits;
            m_iMcuRowHeight = 8;
        }
        else
        {
            // interleaved
            iSumOfBlocks = 0;
            iTotalSFArea = 0;
            for (int i0 = 0; i0 < m_ucInScanComponentQuantity; i0++)
            {
                ScanCompSpecParams& rqSCSP = m_qScanCompSpecParamsArray[i0];
                const RasterImagePtr& rspqComponentImage = m_qComponentImages[rqSCSP.ComponentIndexInFrame];

                // sample quantity must be multiple of 8
                iWidthInDataUnits = ( (rspqComponentImage->GetWidth() & 7) == 0 ?
                    rspqComponentImage->GetWidth()/8 : rspqComponentImage->GetWidth()/8 + 1 );
                iHeightInDataUnits = ( (rspqComponentImage->GetHeight() & 7) == 0 ?
                    rspqComponentImage->GetHeight()/8 : rspqComponentImage->GetHeight()/8 + 1 );

                // data unit quantity must be multiple of certain sampling factor
                if ( iWidthInDataUnits % rqSCSP.FrameCompSpecParamsPtr->SamplingFactorH != 0 )
                {
                    iWidthInDataUnits =
                        (iWidthInDataUnits/rqSCSP.FrameCompSpecParamsPtr->SamplingFactorH + 1)*
                        rqSCSP.FrameCompSpecParamsPtr->SamplingFactorH;
                }
                if ( iHeightInDataUnits % rqSCSP.FrameCompSpecParamsPtr->SamplingFactorV != 0 )
                {
                    iHeightInDataUnits =
                        (iHeightInDataUnits/rqSCSP.FrameCompSpecParamsPtr->SamplingFactorV + 1)*
                        rqSCSP.FrameCompSpecParamsPtr->SamplingFactorV;
                }

                rqSCSP.WidthInDataUnits = iWidthInDataUnits;
                iSumOfBlocks += iWidthInDataUnits*iHeightInDataUnits;
                iTotalSFArea +=
                    rqSCSP.FrameCompSpecParamsPtr->SamplingFactorH*
                    rqSCSP.FrameCompSpecParamsPtr->SamplingFactorV;

                if ( m_eProcess == PT_PROGRESSIVE_DCT
                &&   m_qComponentBuffers[rqSCSP.ComponentIndexInFrame].IsEmpty() )
                {
                    // create components DCT coefficient buffer for progression
                    iInDataUnitsArea = iWidthInDataUnits*iHeightInDataUnits;
                    StillArray<StillArray<short> >& rqBuffsBlockArray =
                        m_qComponentBuffers[rqSCSP.ComponentIndexInFrame];
                    rqBuffsBlockArray.SetQuantity(iInDataUnitsArea);
                    for (int i1 = 0; i1 < iInDataUnitsArea; i1++)
                    {
                        rqBuffsBlockArray[i1].SetQuantity(64);
                        MemSet(rqBuffsBlockArray[i1],0,128);
                    }
                    rqSCSP.FrameCompSpecParamsPtr->ProgressiveBufferWidthInDataUnits = iWidthInDataUnits;
                }
            }

            CONDITIONAL_THROW_EXCEPTION(( iSumOfBlocks % iTotalSFArea == 0 ),ET_FORMAT_MISMATCH);
            m_iInScanMcuQuantity = iSumOfBlocks/iTotalSFArea;
            m_iMcuRowHeight = 8*m_iMaxSamplingFactorV;
        }

        // Do some preparings.
        if ( m_bRestartIntervalsEnabled )
            m_iRSTCounter = 0;

        m_qPredArray.SetQuantity(m_ucInScanComponentQuantity);
        for (int i = 0; i < m_ucInScanComponentQuantity; i++)
            m_qPredArray[i] = 0;

        m_qLeftTopCoordArray.SetQuantity(m_ucInScanComponentQuantity);
        for (int i = 0; i < m_ucInScanComponentQuantity; i++)
            m_qLeftTopCoordArray[i] = MakeLink(0,0);

        if ( m_eProcess == PT_PROGRESSIVE_DCT )
        {
            // progressive
            // reset the end-of-band variable
            iEobRun = 0;
        }

        // reset the bit counter
        iCnt = 0;

        // decode the scan
        for (/**/; /**/; /**/)
        {
            // determine number of MCUs in ECS and if this ECS is the last
            if ( !m_bRestartIntervalsEnabled )
            {
                m_iInEcsMcuQuantity = m_iInScanMcuQuantity;
            }
            else
            {
                m_iRemainingMcuQuantity = m_iInScanMcuQuantity - m_iRSTCounter*m_iRestartInterval;
                if ( m_iRemainingMcuQuantity >= m_iRestartInterval )
                {
                    m_iInEcsMcuQuantity = m_iRestartInterval;
                    m_bEcsIsLast = ( m_iRemainingMcuQuantity == m_iRestartInterval );
                }
                else
                {
                    m_iInEcsMcuQuantity = m_iRemainingMcuQuantity % m_iRestartInterval;
                    m_bEcsIsLast = true;
                }
            }

            // decode the entropy-coded segment
            if ( m_eProcess != PT_PROGRESSIVE_DCT )
            {
                // non-progressive
                for (m_iMcu = 0; m_iMcu < m_iInEcsMcuQuantity; m_iMcu++)
                {
                    for (m_iComp = 0; m_iComp < m_ucInScanComponentQuantity; m_iComp++)
                    {
                        m_pqSCSP = &m_qScanCompSpecParamsArray[m_iComp];
                        m_spqComponentImage = m_qComponentImages[m_pqSCSP->ComponentIndexInFrame];
                        m_pqLTC = &m_qLeftTopCoordArray[m_iComp];

                        m_iIDUHeight = ( m_ucInScanComponentQuantity == 1 ? 1 :
                            m_pqSCSP->FrameCompSpecParamsPtr->SamplingFactorV );
                        m_iIDUWidth = ( m_ucInScanComponentQuantity == 1 ? 1 :
                            m_pqSCSP->FrameCompSpecParamsPtr->SamplingFactorH );
                        for (m_iIDUY = 0; m_iIDUY < m_iIDUHeight; m_iIDUY++)
                        {
                            for (m_iIDUX = 0; m_iIDUX < m_iIDUWidth; m_iIDUX++)
                            {
                                if ( m_eStreaming == Stream::ST_READING_FROM_DYNAMIC_BYTESTREAM )
                                {
                                    m_eResumeZone = IZT_NON_PROGRESSIVE_MCU;
                                    m_iDynamicBytestreamResumeBytePosition = qLoadStream.GetBytePosition();
                                    m_ucResumeB = ucB;
                                    m_iResumeCnt = iCnt;
                                }
NonProgressiveMcuResumeZone:

                                // DC
                                iSuccess = DoHuffmanDecodeProc(iT,ucB,iCnt,*m_pqSCSP->DcHuffmanTableSetPtr,
                                    qLoadStream);
                                PROCESS_INT_SUCCESS_LEVEL_HIGH(iSuccess);

                                iSuccess = DoReceiveProc(iDiff,iT,ucB,iCnt,qLoadStream);
                                PROCESS_INT_SUCCESS_LEVEL_HIGH(iSuccess);

                                DoExtendProc(iDiff,iDiff,iT);

                                m_asBlockArray[0] = m_qPredArray[m_iComp] + iDiff;

                                // AC
                                iK = 1;
                                MemSet(m_asBlockArray+1,0,126);
                                for (/**/; /**/; /**/)
                                {
                                    iSuccess = DoHuffmanDecodeProc(iRs,ucB,iCnt,*m_pqSCSP->AcHuffmanTableSetPtr,
                                        qLoadStream);
                                    PROCESS_INT_SUCCESS_LEVEL_HIGH(iSuccess);

                                    iSsss = iRs & 15;
                                    iRrrr = iRs >> 4;
                                    iR = iRrrr;

                                    if ( iSsss == 0 )
                                    {
                                        if ( iR != 15 )
                                            break;
                                        iK += 16;
                                    }
                                    else
                                    {
                                        iK += iR;
                                        iSuccess = DoReceiveProc(iCoefficient,iSsss,ucB,iCnt,qLoadStream);
                                        PROCESS_INT_SUCCESS_LEVEL_HIGH(iSuccess);
                                        DoExtendProc(iCoefficient,iCoefficient,iSsss);
                                        CONDITIONAL_THROW_EXCEPTION(( iK < 64 ),ET_FORMAT_MISMATCH);
                                        m_asBlockArray[gs_aiInvZigZag[iK]] = iCoefficient;
                                        if ( iK++ == 63 )
                                            break;
                                    }
                                }

                                // no more interrupts in this block, so can update the predictor
                                m_qPredArray[m_iComp] = m_asBlockArray[0];

                                if ( m_pqLTC->First < m_spqComponentImage->GetWidth()
                                &&   m_pqLTC->Second < m_spqComponentImage->GetHeight() )
                                {
                                    // inverse DCT with parallel dequantizing and clamping
                                    DiscreteCosineTransformi::
                                        Get8x8InverseOrthogonal2dWithDequantizationAndClamping(m_acBlock,
                                        m_asBlockArray,m_aaiQuantizationTableDestination[m_pqSCSP->
                                        FrameCompSpecParamsPtr->QuantTableDestSelector]);

                                    // perform the shifting
                                    for (int i = 0; i < 64; i++)
                                        m_asBlockArray[i] = m_acBlock[i] + 128;

                                    iLTXP8 = m_pqLTC->First + 8;
                                    iLTYP8 = m_pqLTC->Second + 8;
                                    iI = 0;
                                    for (int iY = m_pqLTC->Second; iY < iLTYP8; iY++)
                                    {
                                        if ( iY < m_spqComponentImage->GetHeight() )
                                        {
                                            acRow = m_spqComponentImage->GetRow(iY);
                                            for (int iX = m_pqLTC->First; iX < iLTXP8; iX++)
                                            {
                                                if ( iX < m_spqComponentImage->GetWidth() )
                                                    *(acRow + iX) = (char)m_asBlockArray[iI];
                                                iI++;
                                            }
                                        }
                                        else
                                        {
                                            iI += 8;
                                        }
                                    }
                                }

                                m_pqLTC->First += 8;
                            }

                            m_pqLTC->First -= 8*m_iIDUWidth;
                            m_pqLTC->Second += 8;
                        }

                        bEndOfMcuRow = false;
                        m_pqLTC->First += 8*m_iIDUWidth;
                        if ( m_pqLTC->First/8 != m_pqSCSP->WidthInDataUnits )
                        {
                            m_pqLTC->Second -= 8*m_iIDUHeight;
                        }
                        else
                        {
                            m_pqLTC->First = 0;
                            bEndOfMcuRow = true;
                        }
                    }

                    if ( bEndOfMcuRow )
                    {
                        m_iDevelopedRowQuantityUB += m_iMcuRowHeight;
                        if ( m_iDevelopedRowQuantityUB < m_iHeight )
                        {
                            if ( m_eStreaming != Stream::ST_READING_FROM_DYNAMIC_BYTESTREAM )
                            {
                                m_iDevelopedRowQuantity = m_iDevelopedRowQuantityUB;
                            }
                            else
                            {
                                m_iDevelopedRowQuantity = m_iDevelopedRowQuantityUB -
                                    m_iDynamicSamplingCausedRowMargine;
                                if ( m_iDevelopedRowQuantity < 0 )
                                    m_iDevelopedRowQuantity = 0;
                            }
                        }
                        else
                        {
                            m_iDevelopedRowQuantity = m_iHeight;
                        }

                        if ( m_eStreaming == Stream::ST_READING_FROM_DYNAMIC_BYTESTREAM )
                            CompileImage(rspqImage);

                        qProgress.SetDone(((float)m_iDevelopedRowQuantity)/m_iHeight);
                    }
                }
            }
            else
            {
                // progressive
                CONDITIONAL_THROW_EXCEPTION(( !(m_iSuccApproxBitPosHigh != 0 && m_iSuccApproxBitPosHigh - 1 !=
                    m_iSuccApproxBitPosLow) ),ET_FORMAT_MISMATCH);

                if ( m_ucStartOfSpectral == 0 )
                {
                    // DC
                    CONDITIONAL_THROW_EXCEPTION(( m_ucEndOfSpectral == 0 ),ET_FORMAT_MISMATCH);

                    if ( m_iSuccApproxBitPosHigh == 0 )
                    {
                        // initial pass
                        for (m_iMcu = 0; m_iMcu < m_iInEcsMcuQuantity; m_iMcu++)
                        {
                            for (m_iComp = 0; m_iComp < m_ucInScanComponentQuantity; m_iComp++)
                            {
                                m_pqSCSP = &m_qScanCompSpecParamsArray[m_iComp];
                                m_pqLTC = &m_qLeftTopCoordArray[m_iComp];

                                if ( m_eStreaming == Stream::ST_READING_FROM_DYNAMIC_BYTESTREAM )
                                    m_iDYPRangeTop = 8*m_pqLTC->Second;

                                m_iIDUHeight = ( m_ucInScanComponentQuantity == 1 ? 1 :
                                    m_pqSCSP->FrameCompSpecParamsPtr->SamplingFactorV );
                                m_iIDUWidth = ( m_ucInScanComponentQuantity == 1 ? 1 :
                                    m_pqSCSP->FrameCompSpecParamsPtr->SamplingFactorH );
                                for (m_iIDUY = 0; m_iIDUY < m_iIDUHeight; m_iIDUY++)
                                {
                                    for (m_iIDUX = 0; m_iIDUX < m_iIDUWidth; m_iIDUX++)
                                    {
                                        if ( m_eStreaming == Stream::ST_READING_FROM_DYNAMIC_BYTESTREAM )
                                        {
                                            m_eResumeZone = IZT_PROGRESSIVE_DC_INITIAL_MCU;
                                            m_iDynamicBytestreamResumeBytePosition = qLoadStream.GetBytePosition();
                                            m_ucResumeB = ucB;
                                            m_iResumeCnt = iCnt;
                                        }
ProgressiveDcInitialMcuResumeZone:

                                        m_asBlockPtr =
                                            m_qComponentBuffers[m_pqSCSP->ComponentIndexInFrame]
                                            [m_pqLTC->Second*m_pqSCSP->FrameCompSpecParamsPtr->
                                            ProgressiveBufferWidthInDataUnits+m_pqLTC->First];

                                        CONDITIONAL_THROW_EXCEPTION(
                                            ( !m_pqSCSP->DcHuffmanTableSetPtr->HuffValList.IsEmpty() ),
                                            ET_FORMAT_MISMATCH);
                                        iSuccess = DoHuffmanDecodeProc(iT,ucB,iCnt,*m_pqSCSP->DcHuffmanTableSetPtr,
                                            qLoadStream);
                                        PROCESS_INT_SUCCESS_LEVEL_HIGH(iSuccess);

                                        iSuccess = DoReceiveProc(iDiff,iT,ucB,iCnt,qLoadStream);
                                        PROCESS_INT_SUCCESS_LEVEL_HIGH(iSuccess);

                                        DoExtendProc(iDiff,iDiff,iT);

                                        m_qPredArray[m_iComp] += iDiff;

                                        m_asBlockPtr[0] = m_qPredArray[m_iComp] << m_iSuccApproxBitPosLow;

                                        m_pqLTC->First++;
                                    }

                                    m_pqLTC->First -= m_iIDUWidth;
                                    m_pqLTC->Second++;
                                }

                                m_pqLTC->First += m_iIDUWidth;
                                if ( m_pqLTC->First != m_pqSCSP->WidthInDataUnits )
                                {
                                    m_pqLTC->Second -= m_iIDUHeight;
                                }
                                else
                                {
                                    m_pqLTC->First = 0;
                                    if ( m_eStreaming == Stream::ST_READING_FROM_DYNAMIC_BYTESTREAM )
                                    {
                                        iLTCYM8 = 8*m_pqLTC->Second;
                                        if ( iLTCYM8 > m_iDevelopedRowQuantityUB )
                                        {
                                            m_iDevelopedRowQuantityUB = iLTCYM8;
                                            m_iDevelopedRowQuantity = ( m_iDevelopedRowQuantityUB < m_iHeight ?
                                                m_iDevelopedRowQuantityUB : m_iHeight );
                                        }
                                        m_iDYPRangeBottom = iLTCYM8;
                                        CompileImage(rspqImage);
                                    }
                                }
                            }
                        }
                    }
                    else
                    {
                        // succeeding pass
                        m_iSABPLTopVal = 1 << m_iSuccApproxBitPosLow;
                        for (m_iMcu = 0; m_iMcu < m_iInEcsMcuQuantity; m_iMcu++)
                        {
                            for (m_iComp = 0; m_iComp < m_ucInScanComponentQuantity; m_iComp++)
                            {
                                m_pqSCSP = &m_qScanCompSpecParamsArray[m_iComp];
                                m_pqLTC = &m_qLeftTopCoordArray[m_iComp];

                                if ( m_eStreaming == Stream::ST_READING_FROM_DYNAMIC_BYTESTREAM )
                                    m_iDYPRangeTop = 8*m_pqLTC->Second;

                                m_iIDUHeight = ( m_ucInScanComponentQuantity == 1 ? 1 :
                                    m_pqSCSP->FrameCompSpecParamsPtr->SamplingFactorV );
                                m_iIDUWidth = ( m_ucInScanComponentQuantity == 1 ? 1 :
                                    m_pqSCSP->FrameCompSpecParamsPtr->SamplingFactorH );
                                for (m_iIDUY = 0; m_iIDUY < m_iIDUHeight; m_iIDUY++)
                                {
                                    for (m_iIDUX = 0; m_iIDUX < m_iIDUWidth; m_iIDUX++)
                                    {
                                        if ( m_eStreaming == Stream::ST_READING_FROM_DYNAMIC_BYTESTREAM )
                                        {
                                            m_eResumeZone = IZT_PROGRESSIVE_DC_SUCCEEDING_MCU;
                                            m_iDynamicBytestreamResumeBytePosition = qLoadStream.GetBytePosition();
                                            m_ucResumeB = ucB;
                                            m_iResumeCnt = iCnt;
                                        }
ProgressiveDcSucceedingMcuResumeZone:

                                        m_asBlockPtr =
                                            m_qComponentBuffers[m_pqSCSP->ComponentIndexInFrame]
                                            [m_pqLTC->Second*m_pqSCSP->FrameCompSpecParamsPtr->
                                            ProgressiveBufferWidthInDataUnits+m_pqLTC->First];

                                        iSuccess = DoNextBitProc(iBit,ucB,iCnt,qLoadStream);
                                        PROCESS_INT_SUCCESS_LEVEL_HIGH(iSuccess);

                                        if ( iBit != 0 )
                                            m_asBlockPtr[0] |= m_iSABPLTopVal;

                                        m_pqLTC->First++;
                                    }

                                    m_pqLTC->First -= m_iIDUWidth;
                                    m_pqLTC->Second++;
                                }

                                m_pqLTC->First += m_iIDUWidth;
                                if ( m_pqLTC->First != m_pqSCSP->WidthInDataUnits )
                                {
                                    m_pqLTC->Second -= m_iIDUHeight;
                                }
                                else
                                {
                                    m_pqLTC->First = 0;
                                    if ( m_eStreaming == Stream::ST_READING_FROM_DYNAMIC_BYTESTREAM )
                                    {
                                        iLTCYM8 = 8*m_pqLTC->Second;
                                        if ( iLTCYM8 > m_iDevelopedRowQuantityUB )
                                        {
                                            m_iDevelopedRowQuantityUB = iLTCYM8;
                                            m_iDevelopedRowQuantity = ( m_iDevelopedRowQuantityUB < m_iHeight ?
                                                m_iDevelopedRowQuantityUB : m_iHeight );
                                        }
                                        m_iDYPRangeBottom = iLTCYM8;
                                        CompileImage(rspqImage);
                                    }
                                }
                            }
                        }
                    }
                }
                else
                {
                    // AC
                    CONDITIONAL_THROW_EXCEPTION(( m_ucInScanComponentQuantity == 1
                    &&   m_ucStartOfSpectral <= m_ucEndOfSpectral && m_ucEndOfSpectral < 64 ),
                        ET_FORMAT_MISMATCH);

                    m_pqSCSP = &m_qScanCompSpecParamsArray[0];
                    m_pqLTC = &m_qLeftTopCoordArray[0];

                    CONDITIONAL_THROW_EXCEPTION(( !m_pqSCSP->DcHuffmanTableSetPtr->HuffValList.IsEmpty() ),
                        ET_FORMAT_MISMATCH);

                    if ( m_iSuccApproxBitPosHigh == 0 )
                    {
                        // initial pass
                        for (m_iMcu = 0; m_iMcu < m_iInScanMcuQuantity; m_iMcu++)
                        {
                            if ( m_eStreaming == Stream::ST_READING_FROM_DYNAMIC_BYTESTREAM )
                                m_iDYPRangeTop = 8*m_pqLTC->Second;

                            m_asBlockPtr = m_qComponentBuffers[m_pqSCSP->ComponentIndexInFrame]
                                [m_pqLTC->Second*m_pqSCSP->FrameCompSpecParamsPtr->
                                ProgressiveBufferWidthInDataUnits+m_pqLTC->First];

                            if ( m_eStreaming == Stream::ST_READING_FROM_DYNAMIC_BYTESTREAM )
                            {
                                m_eResumeZone = IZT_PROGRESSIVE_AC_INITIAL_MCU;
                                m_iDynamicBytestreamResumeBytePosition = qLoadStream.GetBytePosition();
                                m_ucResumeB = ucB;
                                m_iResumeCnt = iCnt;
                                m_iResumeEobRun = iEobRun;
                            }
ProgressiveAcInitialMcuResumeZone:

                            if ( iEobRun > 0 )
                            {
                                iEobRun--;
                            }
                            else
                            {
                                for (int i = m_ucStartOfSpectral; i <= m_ucEndOfSpectral; i++)
                                {
                                    iSuccess = DoHuffmanDecodeProc(iRs,ucB,iCnt,*m_pqSCSP->AcHuffmanTableSetPtr,
                                        qLoadStream);
                                    PROCESS_INT_SUCCESS_LEVEL_HIGH(iSuccess);

                                    iRrrr = iRs >> 4;
                                    iRs &= 15;
                                    if ( iRs != 0 )
                                    {
                                        i += iRrrr;
                                        iSuccess = DoReceiveProc(iRrrr,iRs,ucB,iCnt,qLoadStream);
                                        PROCESS_INT_SUCCESS_LEVEL_HIGH(iSuccess);
                                        DoExtendProc(iRs,iRrrr,iRs);
                                        CONDITIONAL_THROW_EXCEPTION(( i < 64 ),ET_FORMAT_MISMATCH);
                                        m_asBlockPtr[gs_aiInvZigZag[i]] = iRs << m_iSuccApproxBitPosLow;
                                    }
                                    else
                                    {
                                        if ( iRrrr == 15 )
                                        {
                                            i += 15;
                                        }
                                        else
                                        {
                                            iEobRun = 1 << iRrrr;
                                            if ( iRrrr != 0 )
                                            {
                                                iSuccess = DoReceiveProc(iRrrr,iRrrr,ucB,iCnt,qLoadStream);
                                                PROCESS_INT_SUCCESS_LEVEL_HIGH(iSuccess);
                                                iEobRun += iRrrr;
                                            }
                                            iEobRun--;
                                            break;
                                        }
                                    }
                                }
                            }

                            m_pqLTC->First++;
                            if ( m_pqLTC->First == m_pqSCSP->WidthInDataUnits )
                            {
                                m_pqLTC->First = 0;
                                m_pqLTC->Second++;
                                if ( m_eStreaming == Stream::ST_READING_FROM_DYNAMIC_BYTESTREAM )
                                {
                                    iLTCYM8 = 8*m_pqLTC->Second;
                                    if ( iLTCYM8 > m_iDevelopedRowQuantityUB )
                                    {
                                        m_iDevelopedRowQuantityUB = iLTCYM8;
                                        m_iDevelopedRowQuantity = ( m_iDevelopedRowQuantityUB < m_iHeight ?
                                            m_iDevelopedRowQuantityUB : m_iHeight );
                                    }
                                    m_iDYPRangeBottom = iLTCYM8;
                                    CompileImage(rspqImage);
                                }
                            }
                        }
                    }
                    else
                    {
                        // succeeding pass
                        m_iSABPLTopVal = 1 << m_iSuccApproxBitPosLow;
                        m_iSABPLNegTopVal = -1 << m_iSuccApproxBitPosLow;

                        for (m_iMcu = 0; m_iMcu < m_iInScanMcuQuantity; m_iMcu++)
                        {
                            if ( m_eStreaming == Stream::ST_READING_FROM_DYNAMIC_BYTESTREAM )
                                m_iDYPRangeTop = 8*m_pqLTC->Second;

                            m_asBlockPtr = m_qComponentBuffers[m_pqSCSP->ComponentIndexInFrame]
                                [m_pqLTC->Second*m_pqSCSP->FrameCompSpecParamsPtr->
                                ProgressiveBufferWidthInDataUnits+m_pqLTC->First];

                            if ( m_eStreaming == Stream::ST_READING_FROM_DYNAMIC_BYTESTREAM )
                            {
                                m_eResumeZone = IZT_PROGRESSIVE_AC_SUCCEEDING_MCU;
                                m_iDynamicBytestreamResumeBytePosition = qLoadStream.GetBytePosition();
                                m_ucResumeB = ucB;
                                m_iResumeCnt = iCnt;
                                m_iResumeEobRun = iEobRun;
                                MemCpy(m_asResumeBlock,m_asBlockPtr,128);
                            }
ProgressiveAcSucceedingMcuResumeZone:

                            iNNZQ = 0;
                            iE = m_ucStartOfSpectral;
                            if ( iEobRun == 0 )
                            {
                                for (/**/; iE <= m_ucEndOfSpectral; iE++)
                                {
                                    iSuccess = DoHuffmanDecodeProc(iRs,ucB,iCnt,*m_pqSCSP->AcHuffmanTableSetPtr,
                                        qLoadStream);
                                    PROCESS_INT_SUCCESS_LEVEL_HIGH(iSuccess);

                                    iRrrr = iRs >> 4;
                                    iRs &= 15;
                                    if ( iRs != 0 )
                                    {
                                        iSuccess = DoNextBitProc(iBit,ucB,iCnt,qLoadStream);
                                        PROCESS_INT_SUCCESS_LEVEL_HIGH(iSuccess);
                                        iRs = ( iBit != 0 ? m_iSABPLTopVal : m_iSABPLNegTopVal );
                                    }
                                    else
                                    {
                                        if ( iRrrr != 15 )
                                        {
                                            iEobRun = 1 << iRrrr;
                                            if ( iRrrr != 0 )
                                            {
                                                iSuccess = DoReceiveProc(iRrrr,iRrrr,ucB,iCnt,qLoadStream);
                                                PROCESS_INT_SUCCESS_LEVEL_HIGH(iSuccess);
                                                iEobRun += iRrrr;
                                            }
                                            break;
                                        }
                                    }

                                    do
                                    {
                                        psCoefficient = m_asBlockPtr + gs_aiInvZigZag[iE];
                                        if ( *psCoefficient != 0 )
                                        {
                                            iSuccess = DoNextBitProc(iBit,ucB,iCnt,qLoadStream);
                                            PROCESS_INT_SUCCESS_LEVEL_HIGH(iSuccess);
                                            if ( iBit != 0 )
                                            {
                                                if ( (*psCoefficient & m_iSABPLTopVal) == 0 )
                                                {
                                                    *psCoefficient += ( *psCoefficient >= 0 ? m_iSABPLTopVal :
                                                        m_iSABPLNegTopVal );
                                                }
                                            }
                                        }
                                        else if ( --iRrrr < 0 )
                                        {
                                            break;
                                        }
                                        iE++;
                                    } while ( iE <= m_ucEndOfSpectral );

                                    if ( iRs != 0 )
                                    {
                                        iP = gs_aiInvZigZag[iE];
                                        m_asBlockPtr[iP] = iRs;
                                        aiNNZBlock[iNNZQ++] = iP;
                                    }
                                }
                            }

                            if ( iEobRun > 0 )
                            {
                                for (/**/; iE <= m_ucEndOfSpectral; iE++)
                                {
                                    psCoefficient = m_asBlockPtr + gs_aiInvZigZag[iE];
                                    if ( *psCoefficient != 0 )
                                    {
                                        iSuccess = DoNextBitProc(iBit,ucB,iCnt,qLoadStream);
                                        PROCESS_INT_SUCCESS_LEVEL_HIGH(iSuccess);
                                        if ( iBit != 0 )
                                        {
                                            if ( (*psCoefficient & m_iSABPLTopVal) == 0 )
                                            {
                                                *psCoefficient += ( *psCoefficient >= 0 ? m_iSABPLTopVal :
                                                    m_iSABPLNegTopVal );
                                            }
                                        }
                                    }
                                }
                                iEobRun--;
                            }

                            m_pqLTC->First++;
                            if ( m_pqLTC->First == m_pqSCSP->WidthInDataUnits )
                            {
                                m_pqLTC->First = 0;
                                m_pqLTC->Second++;
                                if ( m_eStreaming == Stream::ST_READING_FROM_DYNAMIC_BYTESTREAM )
                                {
                                    iLTCYM8 = 8*m_pqLTC->Second;
                                    if ( iLTCYM8 > m_iDevelopedRowQuantityUB )
                                    {
                                        m_iDevelopedRowQuantityUB = iLTCYM8;
                                        m_iDevelopedRowQuantity = ( m_iDevelopedRowQuantityUB < m_iHeight ?
                                            m_iDevelopedRowQuantityUB : m_iHeight );
                                    }
                                    m_iDYPRangeBottom = iLTCYM8;
                                    CompileImage(rspqImage);
                                }
                            }
                        }
                    }
                }
            }

            if ( m_eStreaming == Stream::ST_READING_FROM_DYNAMIC_BYTESTREAM )
            {
                m_eResumeZone = IZT_RESTART_INTERVAL;
                m_iDynamicBytestreamResumeBytePosition = qLoadStream.GetBytePosition();
            }
RestartIntervalResumeZone:

            if ( !m_bRestartIntervalsEnabled )
            {
                break;
            }
            else
            {
                if ( !m_bEcsIsLast )
                {
                    RETURN_ON_FAIL(qLoadStream.Read(ucByte));
                    CONDITIONAL_THROW_EXCEPTION(( ucByte == 255 ),ET_FORMAT_MISMATCH);
                    RETURN_ON_FAIL(qLoadStream.Read(ucByte));
                    CONDITIONAL_THROW_EXCEPTION(( 208 <= ucByte && ucByte <= 215 ),ET_FORMAT_MISMATCH);

                    // RST marker
                    CONDITIONAL_THROW_EXCEPTION(( ucByte - 208 == (m_iRSTCounter & 7) ),ET_FORMAT_MISMATCH);

                    // reset predictors
                    for (int i = 0; i < m_ucInScanComponentQuantity; i++)
                        m_qPredArray[i] = 0;

                    if ( m_eProcess == PT_PROGRESSIVE_DCT )
                    {
                        // progressive
                        // reset the end-of-band variable
                        iEobRun = 0;
                    }

                    // reset the bit counter
                    iCnt = 0;

                    m_iRSTCounter++;
                }
                else
                {
                    break;
                }
            }
        }

        if ( m_eStreaming == Stream::ST_READING_FROM_DYNAMIC_BYTESTREAM )
        {
            m_eResumeZone = IZT_END_OF_SCAN;
            m_iDynamicBytestreamResumeBytePosition = qLoadStream.GetBytePosition();
        }
EndOfScanResumeZone:

        if ( m_bScanIsFirst )
        {
            // can be DNL marker
            RETURN_ON_FAIL(qLoadStream.Read(ucByte));
            if ( ucByte != 255 )
            {
                qLoadStream.OffsetBytePosition(-1);
            }
            else
            {
                RETURN_ON_FAIL(qLoadStream.Read(ucByte));
                if ( ucByte != 220 )
                {
                    qLoadStream.OffsetBytePosition(-2);
                }
                else
                {
                    // DNL marker
                    THROW_EXCEPTION_WITHOUT_ASSERT_FALSE(ET_DNL_MARKER_NOT_SUPPORTED);
                }
            }

            m_bScanIsFirst = false;
        }

        if ( m_eProcess != PT_PROGRESSIVE_DCT )
        {
            // non-progressive
            m_iDecodedComponentQuantity += m_ucInScanComponentQuantity;
            if ( m_iDecodedComponentQuantity == m_iComponentQuantity )
                break;
        }
        else
        {
            // progressive
            fProgressiveDone = 0.0f;
            bProgressiveDecodingBreak = true;
            for (int i = 0; i < m_iComponentQuantity; i++)
            {
                const Link<Link<int,int>,Link<int,int> >& rqPDR = m_qProgressionRecordsPerComponent[i];

                fProgressiveDone +=
                    (((float)rqPDR.First.First)/1*((float)rqPDR.First.Second)/8 +
                    ((float)rqPDR.Second.First)/63*((float)rqPDR.Second.Second)/8)/2/m_iComponentQuantity;

                if ( rqPDR.First.First != 1 || rqPDR.First.Second != 8
                ||   rqPDR.Second.First != 63 || rqPDR.Second.Second != 8 )
                {
                    bProgressiveDecodingBreak = false;
                }
            }
            qProgress.SetDone(fProgressiveDone);

            if ( m_iDevelopedRowQuantity == m_iHeight && !bProgressiveDecodingBreak )
            {
                m_iCurrentPassIndex++;
                m_iDevelopedRowQuantity = 0;
                m_iDevelopedRowQuantityUB = 0;
                goto NextPass;
            }

            if ( bProgressiveDecodingBreak )
                break;
        }
    }

    qLoadStream.Finalize();

    if ( m_eStreaming != Stream::ST_READING_FROM_DYNAMIC_BYTESTREAM )
        CompileImage(rspqImage);

    return true;
}
//------------------------------------------------------------------------------------------------------------------
void FormatJpeg::WriteImageData (Stream& rqStream)
{
    assert( !(m_bColorModelPrecised
        &&  ((m_spqImageSaveTo->GetColorModel() == RasterImage::CMT_GRAYSCALE
        &&  m_eColorModel != CMT_GRAYSCALE)
        ||  (m_spqImageSaveTo->GetColorModel() == RasterImage::CMT_RGB && m_eColorModel != CMT_YCC
        &&  m_eColorModel != CMT_RGB)
        ||  (m_spqImageSaveTo->GetColorModel() == RasterImage::CMT_CMYK && m_eColorModel != CMT_YCCK
        &&  m_eColorModel != CMT_CMYK))) );
    assert( !m_spqImageSaveTo->HasVisibilityMask() );

    if ( !m_bColorModelPrecised )
    {
        // do autodetect
        switch ( m_spqImageSaveTo->GetColorModel() )
        {
        case RasterImage::CMT_GRAYSCALE:
            m_eColorModel = CMT_GRAYSCALE;
            break;
        case RasterImage::CMT_RGB:
            m_eColorModel = CMT_YCC;
            break;
        case RasterImage::CMT_CMYK:
            m_eColorModel = CMT_YCCK;
            break;
        }
    }

    // alternative logic
    if ( m_bHasJfifMarker && m_eColorModel == CMT_RGB )
    {
        // JFIF marker and JPEG's color model CMT_RGB are incompatible
        m_bHasJfifMarker = false;
    }
    if ( m_bHasAdobeMarker && m_eColorModel == CMT_GRAYSCALE )
    {
        // Adobe marker and JPEG's color model CMT_GRAYSCALE are incompatible
        m_bHasAdobeMarker = false;
    }

    // progressive scan scheme
    ModestArray<ScanProgressionParams> qSPPArray;
    if ( m_eDataOrganization == DOT_PROGRESSIVE )
    {
        if ( m_eColorModel == CMT_GRAYSCALE )
        {
            if ( m_iProgressiveScanQuantity == 3 )
            {
                qSPPArray.Push(ScanProgressionParams(0,0,0,0,0));
                qSPPArray.Push(ScanProgressionParams(1,5,0,0,0));
                qSPPArray.Push(ScanProgressionParams(6,63,0,0,0));
            }
            else if ( m_iProgressiveScanQuantity == 4 )
            {
                qSPPArray.Push(ScanProgressionParams(0,0,0,0,0));
                qSPPArray.Push(ScanProgressionParams(1,5,0,1,0));
                qSPPArray.Push(ScanProgressionParams(6,63,0,1,0));
                qSPPArray.Push(ScanProgressionParams(1,63,1,0,0));
            }
            else  // m_iProgressiveScanQuantity = 5
            {
                qSPPArray.Push(ScanProgressionParams(0,0,0,1,0));
                qSPPArray.Push(ScanProgressionParams(1,5,0,2,0));
                qSPPArray.Push(ScanProgressionParams(6,63,0,2,0));
                qSPPArray.Push(ScanProgressionParams(1,63,2,1,0));
                qSPPArray.Push(ScanProgressionParams(0,0,1,0,0));
                qSPPArray.Push(ScanProgressionParams(1,63,1,0,0));
            }
        }
        else if ( m_eColorModel == CMT_YCC )
        {
            if ( m_iProgressiveScanQuantity == 3 )
            {
                qSPPArray.Push(ScanProgressionParams(0,0,0,0));
                qSPPArray.Push(ScanProgressionParams(1,5,0,0,1));
                qSPPArray.Push(ScanProgressionParams(1,5,0,0,2));
                qSPPArray.Push(ScanProgressionParams(1,5,0,0,0));
                qSPPArray.Push(ScanProgressionParams(6,63,0,0,1));
                qSPPArray.Push(ScanProgressionParams(6,63,0,0,2));
                qSPPArray.Push(ScanProgressionParams(6,63,0,0,0));
            }
            else if ( m_iProgressiveScanQuantity == 4 )
            {
                qSPPArray.Push(ScanProgressionParams(0,0,0,0));
                qSPPArray.Push(ScanProgressionParams(1,5,0,1,1));
                qSPPArray.Push(ScanProgressionParams(1,5,0,1,2));
                qSPPArray.Push(ScanProgressionParams(1,5,0,1,0));
                qSPPArray.Push(ScanProgressionParams(6,63,0,1,1));
                qSPPArray.Push(ScanProgressionParams(6,63,0,1,2));
                qSPPArray.Push(ScanProgressionParams(6,63,0,1,0));
                qSPPArray.Push(ScanProgressionParams(1,63,1,0,1));
                qSPPArray.Push(ScanProgressionParams(1,63,1,0,2));
                qSPPArray.Push(ScanProgressionParams(1,63,1,0,0));
            }
            else  // m_iProgressiveScanQuantity = 5
            {
                qSPPArray.Push(ScanProgressionParams(0,0,0,1));
                qSPPArray.Push(ScanProgressionParams(1,5,0,2,0));
                qSPPArray.Push(ScanProgressionParams(1,5,0,2,1));
                qSPPArray.Push(ScanProgressionParams(1,5,0,2,2));
                qSPPArray.Push(ScanProgressionParams(6,63,0,2,1));
                qSPPArray.Push(ScanProgressionParams(6,63,0,2,2));
                qSPPArray.Push(ScanProgressionParams(6,63,0,2,0));
                qSPPArray.Push(ScanProgressionParams(1,63,2,1,0));
                qSPPArray.Push(ScanProgressionParams(1,63,2,1,1));
                qSPPArray.Push(ScanProgressionParams(1,63,2,1,2));
                qSPPArray.Push(ScanProgressionParams(0,0,1,0));
                qSPPArray.Push(ScanProgressionParams(1,63,1,0,0));
                qSPPArray.Push(ScanProgressionParams(1,63,1,0,1));
                qSPPArray.Push(ScanProgressionParams(1,63,1,0,2));
            }
        }
        else if ( m_eColorModel == CMT_RGB )
        {
            if ( m_iProgressiveScanQuantity == 3 )
            {
                qSPPArray.Push(ScanProgressionParams(0,0,0,0));
                qSPPArray.Push(ScanProgressionParams(1,5,0,0,0));
                qSPPArray.Push(ScanProgressionParams(1,5,0,0,1));
                qSPPArray.Push(ScanProgressionParams(1,5,0,0,2));
                qSPPArray.Push(ScanProgressionParams(6,63,0,0,0));
                qSPPArray.Push(ScanProgressionParams(6,63,0,0,1));
                qSPPArray.Push(ScanProgressionParams(6,63,0,0,2));
            }
            else if ( m_iProgressiveScanQuantity == 4 )
            {
                qSPPArray.Push(ScanProgressionParams(0,0,0,0));
                qSPPArray.Push(ScanProgressionParams(1,5,0,1,0));
                qSPPArray.Push(ScanProgressionParams(1,5,0,1,1));
                qSPPArray.Push(ScanProgressionParams(1,5,0,1,2));
                qSPPArray.Push(ScanProgressionParams(6,63,0,1,0));
                qSPPArray.Push(ScanProgressionParams(6,63,0,1,1));
                qSPPArray.Push(ScanProgressionParams(6,63,0,1,2));
                qSPPArray.Push(ScanProgressionParams(1,63,1,0,0));
                qSPPArray.Push(ScanProgressionParams(1,63,1,0,1));
                qSPPArray.Push(ScanProgressionParams(1,63,1,0,2));
            }
            else  // m_iProgressiveScanQuantity = 5
            {
                qSPPArray.Push(ScanProgressionParams(0,0,0,1));
                qSPPArray.Push(ScanProgressionParams(1,5,0,2,0));
                qSPPArray.Push(ScanProgressionParams(1,5,0,2,1));
                qSPPArray.Push(ScanProgressionParams(1,5,0,2,2));
                qSPPArray.Push(ScanProgressionParams(6,63,0,2,0));
                qSPPArray.Push(ScanProgressionParams(6,63,0,2,1));
                qSPPArray.Push(ScanProgressionParams(6,63,0,2,2));
                qSPPArray.Push(ScanProgressionParams(1,63,2,1,0));
                qSPPArray.Push(ScanProgressionParams(1,63,2,1,1));
                qSPPArray.Push(ScanProgressionParams(1,63,2,1,2));
                qSPPArray.Push(ScanProgressionParams(0,0,1,0));
                qSPPArray.Push(ScanProgressionParams(1,63,1,0,0));
                qSPPArray.Push(ScanProgressionParams(1,63,1,0,1));
                qSPPArray.Push(ScanProgressionParams(1,63,1,0,2));
            }
        }
        else if ( m_eColorModel == CMT_YCCK )
        {
            if ( m_iProgressiveScanQuantity == 3 )
            {
                qSPPArray.Push(ScanProgressionParams(0,0,0,0));
                qSPPArray.Push(ScanProgressionParams(1,5,0,0,1));
                qSPPArray.Push(ScanProgressionParams(1,5,0,0,2));
                qSPPArray.Push(ScanProgressionParams(1,5,0,0,0));
                qSPPArray.Push(ScanProgressionParams(1,5,0,0,3));
                qSPPArray.Push(ScanProgressionParams(6,63,0,0,1));
                qSPPArray.Push(ScanProgressionParams(6,63,0,0,2));
                qSPPArray.Push(ScanProgressionParams(6,63,0,0,0));
                qSPPArray.Push(ScanProgressionParams(6,63,0,0,3));
            }
            else if ( m_iProgressiveScanQuantity == 4 )
            {
                qSPPArray.Push(ScanProgressionParams(0,0,0,0));
                qSPPArray.Push(ScanProgressionParams(1,5,0,1,1));
                qSPPArray.Push(ScanProgressionParams(1,5,0,1,2));
                qSPPArray.Push(ScanProgressionParams(1,5,0,1,0));
                qSPPArray.Push(ScanProgressionParams(1,5,0,1,3));
                qSPPArray.Push(ScanProgressionParams(6,63,0,1,1));
                qSPPArray.Push(ScanProgressionParams(6,63,0,1,2));
                qSPPArray.Push(ScanProgressionParams(6,63,0,1,0));
                qSPPArray.Push(ScanProgressionParams(6,63,0,1,3));
                qSPPArray.Push(ScanProgressionParams(1,63,1,0,1));
                qSPPArray.Push(ScanProgressionParams(1,63,1,0,2));
                qSPPArray.Push(ScanProgressionParams(1,63,1,0,0));
                qSPPArray.Push(ScanProgressionParams(1,63,1,0,3));
            }
            else  // m_iProgressiveScanQuantity = 5
            {
                qSPPArray.Push(ScanProgressionParams(0,0,0,1));
                qSPPArray.Push(ScanProgressionParams(1,5,0,2,0));
                qSPPArray.Push(ScanProgressionParams(1,5,0,2,3));
                qSPPArray.Push(ScanProgressionParams(1,5,0,2,1));
                qSPPArray.Push(ScanProgressionParams(1,5,0,2,2));
                qSPPArray.Push(ScanProgressionParams(6,63,0,2,1));
                qSPPArray.Push(ScanProgressionParams(6,63,0,2,2));
                qSPPArray.Push(ScanProgressionParams(6,63,0,2,0));
                qSPPArray.Push(ScanProgressionParams(6,63,0,2,3));
                qSPPArray.Push(ScanProgressionParams(1,63,2,1,0));
                qSPPArray.Push(ScanProgressionParams(1,63,2,1,3));
                qSPPArray.Push(ScanProgressionParams(1,63,2,1,1));
                qSPPArray.Push(ScanProgressionParams(1,63,2,1,2));
                qSPPArray.Push(ScanProgressionParams(0,0,1,0));
                qSPPArray.Push(ScanProgressionParams(1,63,1,0,0));
                qSPPArray.Push(ScanProgressionParams(1,63,1,0,3));
                qSPPArray.Push(ScanProgressionParams(1,63,1,0,1));
                qSPPArray.Push(ScanProgressionParams(1,63,1,0,2));
            }
        }
        else  // m_eColorModel = CMT_CMYK
        {
            if ( m_iProgressiveScanQuantity == 3 )
            {
                qSPPArray.Push(ScanProgressionParams(0,0,0,0));
                qSPPArray.Push(ScanProgressionParams(1,5,0,0,0));
                qSPPArray.Push(ScanProgressionParams(1,5,0,0,1));
                qSPPArray.Push(ScanProgressionParams(1,5,0,0,2));
                qSPPArray.Push(ScanProgressionParams(1,5,0,0,3));
                qSPPArray.Push(ScanProgressionParams(6,63,0,0,0));
                qSPPArray.Push(ScanProgressionParams(6,63,0,0,1));
                qSPPArray.Push(ScanProgressionParams(6,63,0,0,2));
                qSPPArray.Push(ScanProgressionParams(6,63,0,0,3));
            }
            else if ( m_iProgressiveScanQuantity == 4 )
            {
                qSPPArray.Push(ScanProgressionParams(0,0,0,0));
                qSPPArray.Push(ScanProgressionParams(1,5,0,1,0));
                qSPPArray.Push(ScanProgressionParams(1,5,0,1,1));
                qSPPArray.Push(ScanProgressionParams(1,5,0,1,2));
                qSPPArray.Push(ScanProgressionParams(1,5,0,1,3));
                qSPPArray.Push(ScanProgressionParams(6,63,0,1,0));
                qSPPArray.Push(ScanProgressionParams(6,63,0,1,1));
                qSPPArray.Push(ScanProgressionParams(6,63,0,1,2));
                qSPPArray.Push(ScanProgressionParams(6,63,0,1,3));
                qSPPArray.Push(ScanProgressionParams(1,63,1,0,0));
                qSPPArray.Push(ScanProgressionParams(1,63,1,0,1));
                qSPPArray.Push(ScanProgressionParams(1,63,1,0,2));
                qSPPArray.Push(ScanProgressionParams(1,63,1,0,3));
            }
            else  // m_iProgressiveScanQuantity = 5
            {
                qSPPArray.Push(ScanProgressionParams(0,0,0,1));
                qSPPArray.Push(ScanProgressionParams(1,5,0,2,0));
                qSPPArray.Push(ScanProgressionParams(1,5,0,2,1));
                qSPPArray.Push(ScanProgressionParams(1,5,0,2,2));
                qSPPArray.Push(ScanProgressionParams(1,5,0,2,3));
                qSPPArray.Push(ScanProgressionParams(6,63,0,2,0));
                qSPPArray.Push(ScanProgressionParams(6,63,0,2,1));
                qSPPArray.Push(ScanProgressionParams(6,63,0,2,2));
                qSPPArray.Push(ScanProgressionParams(6,63,0,2,3));
                qSPPArray.Push(ScanProgressionParams(1,63,2,1,0));
                qSPPArray.Push(ScanProgressionParams(1,63,2,1,1));
                qSPPArray.Push(ScanProgressionParams(1,63,2,1,2));
                qSPPArray.Push(ScanProgressionParams(1,63,2,1,3));
                qSPPArray.Push(ScanProgressionParams(0,0,1,0));
                qSPPArray.Push(ScanProgressionParams(1,63,1,0,0));
                qSPPArray.Push(ScanProgressionParams(1,63,1,0,1));
                qSPPArray.Push(ScanProgressionParams(1,63,1,0,2));
                qSPPArray.Push(ScanProgressionParams(1,63,1,0,3));
            }
        }
    }

    // for a progress bar
    ProgressProbe qProgress(this);

    // generate component images with pixel replication, initialize frame component specification parameters
    int iAlignedWidth = ( (m_iWidth & 7) != 0 ? (m_iWidth/8 + 1)*8 : m_iWidth );
    int iAlignedHeight = ( (m_iHeight & 7) != 0 ? (m_iHeight/8 + 1)*8 : m_iHeight );
    int iHMO = m_iHeight - 1;
    if ( m_eColorModel == CMT_GRAYSCALE )
    {
        m_qComponentImages.SetQuantity(1);
        m_qComponentImages[0] = new RasterImage(RasterImage::IT_GRAYSCALE_8,iAlignedWidth,iAlignedHeight);

        unsigned char* pucRow = (unsigned char*)m_qComponentImages[0]->GetImage();
        unsigned char* pucComp = pucRow;
        unsigned char ucComp;
        StillArray<unsigned char> qLastRow(m_iWidth);
        for (int iY = 0; iY < m_iHeight; iY++)
        {
            for (int iX = 0; iX < m_iWidth; iX++)
            {
                ucComp = Mathf::RoundToInt(m_spqImageSaveTo->GetPixelIntensity(iX,iY)*255);
                *pucComp++ = ucComp;
                if ( iY == iHMO )
                    qLastRow[iX] = ucComp;
            }
            for (int iX = m_iWidth; iX < iAlignedWidth; iX++)
                *pucComp++ = ucComp;

            pucRow += m_qComponentImages[0]->GetRowStride();
            pucComp = pucRow;
        }
        for (int iY = m_iHeight; iY < iAlignedHeight; iY++)
        {
            MemCpy(pucRow,qLastRow,m_iWidth);
            pucRow += m_qComponentImages[0]->GetRowStride();
        }

        m_qFrameCompSpecParamsArray.SetQuantity(1);
        FrameCompSpecParams& rqFCSP = m_qFrameCompSpecParamsArray[0];
        rqFCSP.ComponentIdentifier = 1;
        rqFCSP.SamplingFactorH = 1;
        rqFCSP.SamplingFactorV = 1;
        rqFCSP.QuantTableDestSelector = 0;
    }
    else if ( m_eColorModel == CMT_YCC )
    {
        m_qComponentImages.SetQuantity(3);
        if ( m_eYCbCrChromaSampling == YCCCST_4_4_4 )
        {
            for (int i = 0; i < 3; i++)
                m_qComponentImages[i] = new RasterImage(RasterImage::IT_GRAYSCALE_8,iAlignedWidth,iAlignedHeight);
        }
        else  // eYCbCrChromaSampling = YCCCST_4_2_2
        {
            // luminance dimensions must be multiple of 16 to be able to interleave
            int iDUAlignedWidth = ( (iAlignedWidth & 15) != 0 ? (iAlignedWidth/16 + 1)*16 :
                iAlignedWidth );
            int iDUAlignedHeight = ( (iAlignedHeight & 15) != 0 ? (iAlignedHeight/16 + 1)*16 :
                iAlignedHeight );
            m_qComponentImages[0] = new RasterImage(RasterImage::IT_GRAYSCALE_8,iDUAlignedWidth,iDUAlignedHeight);
            m_qComponentImages[1] = new RasterImage(RasterImage::IT_GRAYSCALE_8,m_iWidth,m_iHeight);
            m_qComponentImages[2] = new RasterImage(RasterImage::IT_GRAYSCALE_8,m_iWidth,m_iHeight);
        }

        ColorYcc qColorYcc;
        unsigned char* aucRow0 = (unsigned char*)m_qComponentImages[0]->GetImage();
        unsigned char* pucComp0 = aucRow0;
        unsigned char* aucRow1 = (unsigned char*)m_qComponentImages[1]->GetImage();
        unsigned char* pucComp1 = aucRow1;
        unsigned char* aucRow2 = (unsigned char*)m_qComponentImages[2]->GetImage();
        unsigned char* pucComp2 = aucRow2;
        unsigned char ucComp0, ucComp1, ucComp2;
        StillArray<unsigned char> qLastRow0(m_iWidth);
        StillArray<unsigned char> qLastRow1(m_iWidth);
        StillArray<unsigned char> qLastRow2(m_iWidth);
        for (int iY = 0; iY < m_iHeight; iY++)
        {
            for (int iX = 0; iX < m_iWidth; iX++)
            {
                qColorYcc = ColorModelConversion::GetConvertedToYcc(m_spqImageSaveTo->GetPixelRgb(iX,iY));

                ucComp0 = Mathf::RoundToInt(qColorYcc.Y()*255);
                ucComp1 = Mathf::RoundToInt((qColorYcc.Cb()+0.5f)*255);
                ucComp2 = Mathf::RoundToInt((qColorYcc.Cr()+0.5f)*255);

                *pucComp0++ = ucComp0;
                *pucComp1++ = ucComp1;
                *pucComp2++ = ucComp2;

                if ( iY == iHMO )
                {
                    qLastRow0[iX] = ucComp0;
                    qLastRow1[iX] = ucComp1;
                    qLastRow2[iX] = ucComp2;
                }
            }
            for (int iX = m_iWidth; iX < m_qComponentImages[0]->GetWidth(); iX++)
                *pucComp0++ = ucComp0;
            for (int iX = m_iWidth; iX < m_qComponentImages[1]->GetWidth(); iX++)
                *pucComp1++ = ucComp1;
            for (int iX = m_iWidth; iX < m_qComponentImages[2]->GetWidth(); iX++)
                *pucComp2++ = ucComp2;

            aucRow0 += m_qComponentImages[0]->GetRowStride();
            pucComp0 = aucRow0;
            aucRow1 += m_qComponentImages[1]->GetRowStride();
            pucComp1 = aucRow1;
            aucRow2 += m_qComponentImages[2]->GetRowStride();
            pucComp2 = aucRow2;
        }
        for (int iY = m_iHeight; iY < m_qComponentImages[0]->GetHeight(); iY++)
        {
            MemCpy(aucRow0,qLastRow0,m_iWidth);
            aucRow0 += m_qComponentImages[0]->GetRowStride();
        }
        for (int iY = m_iHeight; iY < m_qComponentImages[1]->GetHeight(); iY++)
        {
            MemCpy(aucRow1,qLastRow1,m_iWidth);
            aucRow1 += m_qComponentImages[1]->GetRowStride();
        }
        for (int iY = m_iHeight; iY < m_qComponentImages[2]->GetHeight(); iY++)
        {
            MemCpy(aucRow2,qLastRow2,m_iWidth);
            aucRow2 += m_qComponentImages[2]->GetRowStride();
        }

        if ( m_eYCbCrChromaSampling == YCCCST_4_2_2 )
        {
            // resample chromaticity
            int iDSWidth = (int)Mathf::Ceil(((float)m_iWidth)*0.5f);
            int iDSHeight = (int)Mathf::Ceil(((float)m_iHeight)*0.5f);
            for (int i = 1; i < 3; i++)
            {
                RasterImageTransformation qRIT(m_qComponentImages[i]);
                m_qComponentImages[i] = qRIT.GetResampled(iDSWidth,iDSHeight,
                    RasterImageTransformation::IT_BILINEAR);
            }

            int iDSAlignedWidth = ( (iDSWidth & 7) != 0 ? (iDSWidth/8 + 1)*8 : iDSWidth );
            int iDSAlignedHeight = ( (iDSHeight & 7) != 0 ? (iDSHeight/8 + 1)*8 : iDSHeight );
            if ( iDSAlignedWidth != iDSWidth || iDSAlignedHeight != iDSHeight )
            {
                RasterImagePtr spqAlignedCb = new RasterImage(RasterImage::IT_GRAYSCALE_8,iDSAlignedWidth,
                    iDSAlignedHeight);
                RasterImagePtr spqAlignedCr = new RasterImage(RasterImage::IT_GRAYSCALE_8,iDSAlignedWidth,
                    iDSAlignedHeight);

                unsigned char* aucRowSrc0 = (unsigned char*)m_qComponentImages[1]->GetImage();
                unsigned char* aucRowSrc1 = (unsigned char*)m_qComponentImages[2]->GetImage();
                unsigned char* aucRowDst0 = (unsigned char*)spqAlignedCb->GetImage();
                unsigned char* aucRowDst1 = (unsigned char*)spqAlignedCr->GetImage();
                int iDSHMO = iDSHeight - 1;
                int iDSWMO = iDSWidth - 1;
                int iWDiff = iDSAlignedWidth - iDSWidth;
                for (int iY = 0; iY < iDSHeight; iY++)
                {
                    MemCpy(aucRowDst0,aucRowSrc0,iDSWidth);
                    MemSet(aucRowDst0+iDSWidth,*(aucRowSrc0+iDSWMO),iWDiff);
                    MemCpy(aucRowDst1,aucRowSrc1,iDSWidth);
                    MemSet(aucRowDst1+iDSWidth,*(aucRowSrc1+iDSWMO),iWDiff);

                    if ( iY != iDSHMO )
                    {
                        aucRowSrc0 += m_qComponentImages[1]->GetRowStride();
                        aucRowSrc1 += m_qComponentImages[2]->GetRowStride();
                    }
                    aucRowDst0 += spqAlignedCb->GetRowStride();
                    aucRowDst1 += spqAlignedCr->GetRowStride();
                }
                for (int iY = iDSHeight; iY < iDSAlignedHeight; iY++)
                {
                    MemCpy(aucRowDst0,aucRowSrc0,iDSWidth);
                    MemCpy(aucRowDst1,aucRowSrc1,iDSWidth);

                    aucRowDst0 += spqAlignedCb->GetRowStride();
                    aucRowDst1 += spqAlignedCr->GetRowStride();
                }

                m_qComponentImages[1] = spqAlignedCb;
                m_qComponentImages[2] = spqAlignedCr;
            }
        }

        m_qFrameCompSpecParamsArray.SetQuantity(3);

        FrameCompSpecParams& rqFCSP0 = m_qFrameCompSpecParamsArray[0];
        rqFCSP0.ComponentIdentifier = 1;
        if ( m_eYCbCrChromaSampling == YCCCST_4_4_4 )
        {
            rqFCSP0.SamplingFactorH = 1;
            rqFCSP0.SamplingFactorV = 1;
        }
        else  // eYCbCrChromaSampling = YCCCST_4_2_2
        {
            rqFCSP0.SamplingFactorH = 2;
            rqFCSP0.SamplingFactorV = 2;
        }
        rqFCSP0.QuantTableDestSelector = 0;

        FrameCompSpecParams& rqFCSP1 = m_qFrameCompSpecParamsArray[1];
        rqFCSP1.ComponentIdentifier = 2;
        rqFCSP1.SamplingFactorH = 1;
        rqFCSP1.SamplingFactorV = 1;
        rqFCSP1.QuantTableDestSelector = 1;

        FrameCompSpecParams& rqFCSP2 = m_qFrameCompSpecParamsArray[2];
        rqFCSP2.ComponentIdentifier = 3;
        rqFCSP2.SamplingFactorH = 1;
        rqFCSP2.SamplingFactorV = 1;
        rqFCSP2.QuantTableDestSelector = 1;
    }
    else if ( m_eColorModel == CMT_RGB )
    {
        m_qComponentImages.SetQuantity(3);
        for (int i = 0; i < 3; i++)
            m_qComponentImages[i] = new RasterImage(RasterImage::IT_GRAYSCALE_8,iAlignedWidth,iAlignedHeight);

        ColorRgb qColorRgb;
        unsigned char* aucRow0 = (unsigned char*)m_qComponentImages[0]->GetImage();
        unsigned char* pucComp0 = aucRow0;
        unsigned char* aucRow1 = (unsigned char*)m_qComponentImages[1]->GetImage();
        unsigned char* pucComp1 = aucRow1;
        unsigned char* aucRow2 = (unsigned char*)m_qComponentImages[2]->GetImage();
        unsigned char* pucComp2 = aucRow2;
        unsigned char ucComp0, ucComp1, ucComp2;
        StillArray<unsigned char> qLastRow0(m_iWidth);
        StillArray<unsigned char> qLastRow1(m_iWidth);
        StillArray<unsigned char> qLastRow2(m_iWidth);
        for (int iY = 0; iY < m_iHeight; iY++)
        {
            for (int iX = 0; iX < m_iWidth; iX++)
            {
                qColorRgb = m_spqImageSaveTo->GetPixelRgb(iX,iY);

                ucComp0 = Mathf::RoundToInt(qColorRgb.R()*255);
                ucComp1 = Mathf::RoundToInt(qColorRgb.G()*255);
                ucComp2 = Mathf::RoundToInt(qColorRgb.B()*255);

                *pucComp0++ = ucComp0;
                *pucComp1++ = ucComp1;
                *pucComp2++ = ucComp2;

                if ( iY == iHMO )
                {
                    qLastRow0[iX] = ucComp0;
                    qLastRow1[iX] = ucComp1;
                    qLastRow2[iX] = ucComp2;
                }
            }
            for (int iX = m_iWidth; iX < iAlignedWidth; iX++)
            {
                *pucComp0++ = ucComp0;
                *pucComp1++ = ucComp1;
                *pucComp2++ = ucComp2;
            }

            aucRow0 += m_qComponentImages[0]->GetRowStride();
            pucComp0 = aucRow0;
            aucRow1 += m_qComponentImages[1]->GetRowStride();
            pucComp1 = aucRow1;
            aucRow2 += m_qComponentImages[2]->GetRowStride();
            pucComp2 = aucRow2;
        }
        for (int iY = m_iHeight; iY < iAlignedHeight; iY++)
        {
            MemCpy(aucRow0,qLastRow0,m_iWidth);
            MemCpy(aucRow1,qLastRow1,m_iWidth);
            MemCpy(aucRow2,qLastRow2,m_iWidth);

            aucRow0 += m_qComponentImages[0]->GetRowStride();
            aucRow1 += m_qComponentImages[1]->GetRowStride();
            aucRow2 += m_qComponentImages[2]->GetRowStride();
        }

        m_qFrameCompSpecParamsArray.SetQuantity(3);

        FrameCompSpecParams& rqFCSP0 = m_qFrameCompSpecParamsArray[0];
        rqFCSP0.ComponentIdentifier = 82;
        rqFCSP0.SamplingFactorH = 1;
        rqFCSP0.SamplingFactorV = 1;
        rqFCSP0.QuantTableDestSelector = 0;

        FrameCompSpecParams& rqFCSP1 = m_qFrameCompSpecParamsArray[1];
        rqFCSP1.ComponentIdentifier = 71;
        rqFCSP1.SamplingFactorH = 1;
        rqFCSP1.SamplingFactorV = 1;
        rqFCSP1.QuantTableDestSelector = 0;

        FrameCompSpecParams& rqFCSP2 = m_qFrameCompSpecParamsArray[2];
        rqFCSP2.ComponentIdentifier = 66;
        rqFCSP2.SamplingFactorH = 1;
        rqFCSP2.SamplingFactorV = 1;
        rqFCSP2.QuantTableDestSelector = 0;
    }
    else if ( m_eColorModel == CMT_YCCK )
    {
        m_qComponentImages.SetQuantity(4);
        for (int i = 0; i < 4; i++)
            m_qComponentImages[i] = new RasterImage(RasterImage::IT_GRAYSCALE_8,iAlignedWidth,iAlignedHeight);

        ColorCmyk qColorCmyk;
        ColorYcc qColorYcc;
        unsigned char* aucRow0 = (unsigned char*)m_qComponentImages[0]->GetImage();
        unsigned char* pucComp0 = aucRow0;
        unsigned char* aucRow1 = (unsigned char*)m_qComponentImages[1]->GetImage();
        unsigned char* pucComp1 = aucRow1;
        unsigned char* aucRow2 = (unsigned char*)m_qComponentImages[2]->GetImage();
        unsigned char* pucComp2 = aucRow2;
        unsigned char* aucRow3 = (unsigned char*)m_qComponentImages[3]->GetImage();
        unsigned char* pucComp3 = aucRow3;
        unsigned char ucComp0, ucComp1, ucComp2, ucComp3;
        StillArray<unsigned char> qLastRow0(m_iWidth);
        StillArray<unsigned char> qLastRow1(m_iWidth);
        StillArray<unsigned char> qLastRow2(m_iWidth);
        StillArray<unsigned char> qLastRow3(m_iWidth);
        for (int iY = 0; iY < m_iHeight; iY++)
        {
            for (int iX = 0; iX < m_iWidth; iX++)
            {
                qColorCmyk = m_spqImageSaveTo->GetPixelCmyk(iX,iY);
                qColorYcc =
                    ColorModelConversion::GetConvertedToYcc(ColorModelConversion::GetConvertedToRgb(qColorCmyk));

                ucComp0 = 255 - Mathf::RoundToInt(qColorYcc.Y()*255);
                ucComp1 = 255 - Mathf::RoundToInt((qColorYcc.Cb()+0.5f)*255);
                ucComp2 = 255 - Mathf::RoundToInt((qColorYcc.Cr()+0.5f)*255);
                ucComp3 = 255 - Mathf::RoundToInt(qColorCmyk.K()*255);

                *pucComp0++ = ucComp0;
                *pucComp1++ = ucComp1;
                *pucComp2++ = ucComp2;
                *pucComp3++ = ucComp3;

                if ( iY == iHMO )
                {
                    qLastRow0[iX] = ucComp0;
                    qLastRow1[iX] = ucComp1;
                    qLastRow2[iX] = ucComp2;
                    qLastRow3[iX] = ucComp3;
                }
            }
            for (int iX = m_iWidth; iX < iAlignedWidth; iX++)
            {
                *pucComp0++ = ucComp0;
                *pucComp1++ = ucComp1;
                *pucComp2++ = ucComp2;
                *pucComp3++ = ucComp3;
            }

            aucRow0 += m_qComponentImages[0]->GetRowStride();
            pucComp0 = aucRow0;
            aucRow1 += m_qComponentImages[1]->GetRowStride();
            pucComp1 = aucRow1;
            aucRow2 += m_qComponentImages[2]->GetRowStride();
            pucComp2 = aucRow2;
            aucRow3 += m_qComponentImages[3]->GetRowStride();
            pucComp3 = aucRow3;
        }
        for (int iY = m_iHeight; iY < iAlignedHeight; iY++)
        {
            MemCpy(aucRow0,qLastRow0,m_iWidth);
            MemCpy(aucRow1,qLastRow1,m_iWidth);
            MemCpy(aucRow2,qLastRow2,m_iWidth);
            MemCpy(aucRow3,qLastRow3,m_iWidth);

            aucRow0 += m_qComponentImages[0]->GetRowStride();
            aucRow1 += m_qComponentImages[1]->GetRowStride();
            aucRow2 += m_qComponentImages[2]->GetRowStride();
            aucRow3 += m_qComponentImages[3]->GetRowStride();
        }

        m_qFrameCompSpecParamsArray.SetQuantity(4);

        FrameCompSpecParams& rqFCSP0 = m_qFrameCompSpecParamsArray[0];
        rqFCSP0.ComponentIdentifier = 1;
        rqFCSP0.SamplingFactorH = 1;
        rqFCSP0.SamplingFactorV = 1;
        rqFCSP0.QuantTableDestSelector = 0;

        FrameCompSpecParams& rqFCSP1 = m_qFrameCompSpecParamsArray[1];
        rqFCSP1.ComponentIdentifier = 2;
        rqFCSP1.SamplingFactorH = 1;
        rqFCSP1.SamplingFactorV = 1;
        rqFCSP1.QuantTableDestSelector = 1;

        FrameCompSpecParams& rqFCSP2 = m_qFrameCompSpecParamsArray[2];
        rqFCSP2.ComponentIdentifier = 3;
        rqFCSP2.SamplingFactorH = 1;
        rqFCSP2.SamplingFactorV = 1;
        rqFCSP2.QuantTableDestSelector = 1;

        FrameCompSpecParams& rqFCSP3 = m_qFrameCompSpecParamsArray[3];
        rqFCSP3.ComponentIdentifier = 4;
        rqFCSP3.SamplingFactorH = 1;
        rqFCSP3.SamplingFactorV = 1;
        rqFCSP3.QuantTableDestSelector = 0;
    }
    else  // m_eColorModel = CMT_CMYK
    {
        m_qComponentImages.SetQuantity(4);
        for (int i = 0; i < 4; i++)
            m_qComponentImages[i] = new RasterImage(RasterImage::IT_GRAYSCALE_8,iAlignedWidth,iAlignedHeight);

        ColorCmyk qColorCmyk;
        unsigned char* aucRow0 = (unsigned char*)m_qComponentImages[0]->GetImage();
        unsigned char* pucComp0 = aucRow0;
        unsigned char* aucRow1 = (unsigned char*)m_qComponentImages[1]->GetImage();
        unsigned char* pucComp1 = aucRow1;
        unsigned char* aucRow2 = (unsigned char*)m_qComponentImages[2]->GetImage();
        unsigned char* pucComp2 = aucRow2;
        unsigned char* aucRow3 = (unsigned char*)m_qComponentImages[3]->GetImage();
        unsigned char* pucComp3 = aucRow3;
        unsigned char ucComp0, ucComp1, ucComp2, ucComp3;
        StillArray<unsigned char> qLastRow0(m_iWidth);
        StillArray<unsigned char> qLastRow1(m_iWidth);
        StillArray<unsigned char> qLastRow2(m_iWidth);
        StillArray<unsigned char> qLastRow3(m_iWidth);
        for (int iY = 0; iY < m_iHeight; iY++)
        {
            for (int iX = 0; iX < m_iWidth; iX++)
            {
                qColorCmyk = m_spqImageSaveTo->GetPixelCmyk(iX,iY);

                ucComp0 = 255 - Mathf::RoundToInt(qColorCmyk.C()*255);
                ucComp1 = 255 - Mathf::RoundToInt(qColorCmyk.M()*255);
                ucComp2 = 255 - Mathf::RoundToInt(qColorCmyk.Y()*255);
                ucComp3 = 255 - Mathf::RoundToInt(qColorCmyk.K()*255);

                *pucComp0++ = ucComp0;
                *pucComp1++ = ucComp1;
                *pucComp2++ = ucComp2;
                *pucComp3++ = ucComp3;

                if ( iY == iHMO )
                {
                    qLastRow0[iX] = ucComp0;
                    qLastRow1[iX] = ucComp1;
                    qLastRow2[iX] = ucComp2;
                    qLastRow3[iX] = ucComp3;
                }
            }
            for (int iX = m_iWidth; iX < iAlignedWidth; iX++)
            {
                *pucComp0++ = ucComp0;
                *pucComp1++ = ucComp1;
                *pucComp2++ = ucComp2;
                *pucComp3++ = ucComp3;
            }

            aucRow0 += m_qComponentImages[0]->GetRowStride();
            pucComp0 = aucRow0;
            aucRow1 += m_qComponentImages[1]->GetRowStride();
            pucComp1 = aucRow1;
            aucRow2 += m_qComponentImages[2]->GetRowStride();
            pucComp2 = aucRow2;
            aucRow3 += m_qComponentImages[3]->GetRowStride();
            pucComp3 = aucRow3;
        }
        for (int iY = m_iHeight; iY < iAlignedHeight; iY++)
        {
            MemCpy(aucRow0,qLastRow0,m_iWidth);
            MemCpy(aucRow1,qLastRow1,m_iWidth);
            MemCpy(aucRow2,qLastRow2,m_iWidth);
            MemCpy(aucRow3,qLastRow3,m_iWidth);

            aucRow0 += m_qComponentImages[0]->GetRowStride();
            aucRow1 += m_qComponentImages[1]->GetRowStride();
            aucRow2 += m_qComponentImages[2]->GetRowStride();
            aucRow3 += m_qComponentImages[3]->GetRowStride();
        }

        m_qFrameCompSpecParamsArray.SetQuantity(4);

        FrameCompSpecParams& rqFCSP0 = m_qFrameCompSpecParamsArray[0];
        rqFCSP0.ComponentIdentifier = 67;
        rqFCSP0.SamplingFactorH = 1;
        rqFCSP0.SamplingFactorV = 1;
        rqFCSP0.QuantTableDestSelector = 0;

        FrameCompSpecParams& rqFCSP1 = m_qFrameCompSpecParamsArray[1];
        rqFCSP1.ComponentIdentifier = 77;
        rqFCSP1.SamplingFactorH = 1;
        rqFCSP1.SamplingFactorV = 1;
        rqFCSP1.QuantTableDestSelector = 0;

        FrameCompSpecParams& rqFCSP2 = m_qFrameCompSpecParamsArray[2];
        rqFCSP2.ComponentIdentifier = 89;
        rqFCSP2.SamplingFactorH = 1;
        rqFCSP2.SamplingFactorV = 1;
        rqFCSP2.QuantTableDestSelector = 0;

        FrameCompSpecParams& rqFCSP3 = m_qFrameCompSpecParamsArray[3];
        rqFCSP3.ComponentIdentifier = 75;
        rqFCSP3.SamplingFactorH = 1;
        rqFCSP3.SamplingFactorV = 1;
        rqFCSP3.QuantTableDestSelector = 0;
    }

    // construct quantization table(s)
    unsigned char aucDSLuminanceQuantizationTable[64];
    unsigned char aucDSChrominanceQuantizationTable[64];
    static const float s_fPowerBelowHalfQuality = 1.0f;
    static const float s_fPowerAboveHalfQuality = 7.0f;
    float fMappedQuality = Mathf::Pow(m_fQuality/0.5f,( m_fQuality < 0.5f ? s_fPowerBelowHalfQuality :
        s_fPowerAboveHalfQuality )) + Mathf::ZERO_TOLERANCE;
    int iEntry;
    for (int i = 0; i < 64; i++)
    {
        iEntry = Mathf::RoundToInt(((float)gs_aiLuminanceQuantizationTable[i])/fMappedQuality);
        if ( iEntry < 1 )
            iEntry = 1;
        if ( iEntry > 255 )
            iEntry = 255;
        aucDSLuminanceQuantizationTable[i] = iEntry;

        iEntry = Mathf::RoundToInt(((float)gs_aiChrominanceQuantizationTable[i])/fMappedQuality);
        if ( iEntry < 1 )
            iEntry = 1;
        if ( iEntry > 255 )
            iEntry = 255;
        aucDSChrominanceQuantizationTable[i] = iEntry;
    }

    qProgress.SetDone(0.33f);

    m_iComponentQuantity = m_qComponentImages.GetQuantity();

    // allocate buffers for blocks and make all around DCT
    m_qComponentBuffers.SetQuantity(m_iComponentQuantity);
    StillArray<Link<int,int> > qComponentInDataUnitsExtensions(m_iComponentQuantity);
    int iLTCX, iLTCY, iVar, iI;
    unsigned char* pucRow;
    unsigned char* pucComp;
    unsigned char* aucQuantizationTableToUse;
    for (int i0 = 0; i0 < m_iComponentQuantity; i0++)
    {
        const RasterImagePtr& rspqComponent = m_qComponentImages[i0];
        Link<int,int>& rqCIDUE = qComponentInDataUnitsExtensions[i0];
        StillArray<StillArray<short> >& rqDUArray = m_qComponentBuffers[i0];

        switch ( m_eColorModel )
        {
        case CMT_GRAYSCALE:
            aucQuantizationTableToUse = aucDSLuminanceQuantizationTable;
            break;
        case CMT_YCC:
            aucQuantizationTableToUse = ( i0 == 0 ? aucDSLuminanceQuantizationTable :
                aucDSChrominanceQuantizationTable );
            break;
        case CMT_RGB:
            aucQuantizationTableToUse = aucDSChrominanceQuantizationTable;
            break;
        case CMT_YCCK:
            aucQuantizationTableToUse = ( i0 == 0 || i0 == 3 ? aucDSLuminanceQuantizationTable :
                aucDSChrominanceQuantizationTable );
            break;
        case CMT_CMYK:
            aucQuantizationTableToUse = aucDSChrominanceQuantizationTable;
            break;
        }

        rqCIDUE.First = rspqComponent->GetWidth()/8;
        rqCIDUE.Second = rspqComponent->GetHeight()/8;
        rqDUArray.SetQuantity(rqCIDUE.First*rqCIDUE.Second);
        iLTCX = iLTCY = 0;
        iI = 0;
        for (int i1 = 0; i1 < rqCIDUE.Second; i1++)
        {
            for (int i2 = 0; i2 < rqCIDUE.First; i2++)
            {
                StillArray<short>& rqBlock = rqDUArray[iI++];

                rqBlock.SetQuantity(64);
                pucRow = (unsigned char*)rspqComponent->GetRow(iLTCY);
                pucComp = pucRow + iLTCX;
                for (int i3 = 0; i3 < 8; i3++)
                {
                    iVar = 8*i3;
                    for (int i4 = 0; i4 < 8; i4++)
                    {
                        // assign with shifting
                        rqBlock[iVar+i4] = *pucComp - 128;
                        pucComp++;
                    }

                    pucRow += rspqComponent->GetRowStride();
                    pucComp = pucRow + iLTCX;
                }

                // DCT
                DiscreteCosineTransformi::Apply8x8ForwardOrthogonal2d(rqBlock);

                // quantization
                for (int i3 = 0; i3 < 64; i3++)
                    rqBlock[i3] = Mathf::RoundToInt(((float)rqBlock[i3])/aucQuantizationTableToUse[i3]);

                iLTCX += 8;
            }

            iLTCX = 0;
            iLTCY += 8;
        }
    }

    int aiEntropyCodingTableQuantity[2];
    if ( m_eDataOrganization == DOT_PLANE )
    {
        aiEntropyCodingTableQuantity[0] = aiEntropyCodingTableQuantity[1] =
            ( m_eColorModel == CMT_GRAYSCALE || m_eColorModel == CMT_RGB || m_eColorModel == CMT_CMYK ? 1 : 2 );
    }
    else  // m_eDataOrganization = DOT_PROGRESSIVE
    {
        aiEntropyCodingTableQuantity[0] = aiEntropyCodingTableQuantity[1] = 0;
        for (int i = 0; i < qSPPArray.GetQuantity(); i++)
        {
            if ( qSPPArray[i].Dc )
            {
                if ( qSPPArray[i].SuccApproxBitPosHigh == 0 )
                    aiEntropyCodingTableQuantity[0]++;
            }
            else
            {
                aiEntropyCodingTableQuantity[1]++;
            }
        }
    }

    m_qPredArray.SetQuantity(m_iComponentQuantity);
    int iPrevDc, iEobRun, iBBQ, iDcIndex, iAcIndex;
    char acBitBuff[ms_iBitBuffSize];
    StillArray<int> qComponentBlockIndices(m_iComponentQuantity);

    // gather statistics
    StillArray<StillArray<unsigned int> > aqFrequencies[2];
    for (int i0 = 0; i0 < 2; i0++)
    {
        aqFrequencies[i0].SetQuantity(aiEntropyCodingTableQuantity[i0]);
        for (int i1 = 0; i1 < aiEntropyCodingTableQuantity[i0]; i1++)
        {
            aqFrequencies[i0][i1].SetQuantity(257);
            MemSet(aqFrequencies[i0][i1],0,1028);
        }
    }
    if ( m_eDataOrganization == DOT_PLANE )
    {
        if ( m_eColorModel == CMT_GRAYSCALE )
        {
            const StillArray<StillArray<short> >& rqDUArray = m_qComponentBuffers[0];

            iPrevDc = 0;
            for (int i = 0; i < rqDUArray.GetQuantity(); i++)
                GatherStatistics(rqDUArray[i],iPrevDc,aqFrequencies[0][0],aqFrequencies[1][0]);
        }
        else if ( m_eColorModel == CMT_YCC )
        {
            const StillArray<StillArray<short> >& rqDUArrayY = m_qComponentBuffers[0];
            iPrevDc = 0;
            if ( m_eYCbCrChromaSampling == YCCCST_4_4_4 )
            {
                for (int i = 0; i < rqDUArrayY.GetQuantity(); i++)
                    GatherStatistics(rqDUArrayY[i],iPrevDc,aqFrequencies[0][0],aqFrequencies[1][0]);
            }
            else  // m_eYCbCrChromaSampling = YCCCST_4_2_2
            {
                int iIDULTCX = 0;
                int iIDULTCY = 0;
                int iIndex = 0;
                while ( iIndex != rqDUArrayY.GetQuantity() )
                {
                    for (int i0 = 0; i0 < 2; i0++)
                    {
                        iVar = iIDULTCY*qComponentInDataUnitsExtensions[0].First;
                        for (int i1 = 0; i1 < 2; i1++)
                        {
                            GatherStatistics(rqDUArrayY[iVar+iIDULTCX],iPrevDc,aqFrequencies[0][0],
                                aqFrequencies[1][0]);
                            iIDULTCX++;
                            iIndex++;
                        }

                        iIDULTCX -= 2;
                        iIDULTCY++;
                    }
                    iIDULTCX += 2;
                    if ( iIDULTCX != qComponentInDataUnitsExtensions[0].First )
                        iIDULTCY -= 2;
                    else
                        iIDULTCX = 0;
                }
            }

            for (int i0 = 1; i0 < 3; i0++)
            {
                const StillArray<StillArray<short> >& rqDUArray = m_qComponentBuffers[i0];

                iPrevDc = 0;
                for (int i1 = 0; i1 < rqDUArray.GetQuantity(); i1++)
                    GatherStatistics(rqDUArray[i1],iPrevDc,aqFrequencies[0][1],aqFrequencies[1][1]);
            }
        }
        else if ( m_eColorModel == CMT_RGB )
        {
            for (int i0 = 0; i0 < 3; i0++)
            {
                const StillArray<StillArray<short> >& rqDUArray = m_qComponentBuffers[i0];

                iPrevDc = 0;
                for (int i1 = 0; i1 < rqDUArray.GetQuantity(); i1++)
                    GatherStatistics(rqDUArray[i1],iPrevDc,aqFrequencies[0][0],aqFrequencies[1][0]);
            }
        }
        else if ( m_eColorModel == CMT_YCCK )
        {
            const StillArray<StillArray<short> >& rqDUArrayY = m_qComponentBuffers[0];
            iPrevDc = 0;
            for (int i = 0; i < rqDUArrayY.GetQuantity(); i++)
                GatherStatistics(rqDUArrayY[i],iPrevDc,aqFrequencies[0][0],aqFrequencies[1][0]);

            for (int i0 = 1; i0 < 3; i0++)
            {
                const StillArray<StillArray<short> >& rqDUArray = m_qComponentBuffers[i0];

                iPrevDc = 0;
                for (int i1 = 0; i1 < rqDUArray.GetQuantity(); i1++)
                    GatherStatistics(rqDUArray[i1],iPrevDc,aqFrequencies[0][1],aqFrequencies[1][1]);
            }

            const StillArray<StillArray<short> >& rqDUArrayK = m_qComponentBuffers[3];
            iPrevDc = 0;
            for (int i = 0; i < rqDUArrayK.GetQuantity(); i++)
                GatherStatistics(rqDUArrayK[i],iPrevDc,aqFrequencies[0][0],aqFrequencies[1][0]);
        }
        else  // m_eColorModel = CMT_CMYK
        {
            for (int i0 = 0; i0 < 4; i0++)
            {
                const StillArray<StillArray<short> >& rqDUArray = m_qComponentBuffers[i0];

                iPrevDc = 0;
                for (int i1 = 0; i1 < rqDUArray.GetQuantity(); i1++)
                    GatherStatistics(rqDUArray[i1],iPrevDc,aqFrequencies[0][0],aqFrequencies[1][0]);
            }
        }
    }
    else  // m_eDataOrganization = DOT_PROGRESSIVE
    {
        iEobRun = 0;
        iBBQ = 0;
        iDcIndex = iAcIndex = 0;
        unsigned int* auiFrequencies;
        if ( m_eColorModel == CMT_GRAYSCALE )
        {
            const StillArray<StillArray<short> >& rqDUArray = m_qComponentBuffers[0];

            for (int i0 = 0; i0 < qSPPArray.GetQuantity(); i0++)
            {
                const ScanProgressionParams& rqSPP = qSPPArray[i0];

                if ( rqSPP.Dc )
                {
                    // DC
                    if ( rqSPP.SuccApproxBitPosHigh == 0 )
                    {
                        // initial pass
                        auiFrequencies = aqFrequencies[0][iDcIndex++];

                        m_qPredArray[0] = 0;
                        for (int i1 = 0; i1 < rqDUArray.GetQuantity(); i1++)
                            EncodeDcInitial(auiFrequencies,rqDUArray[i1],m_qPredArray[0],rqSPP,0,0,0);

                        EncodeHuffmanEobRun(auiFrequencies,iEobRun,0,0,acBitBuff,iBBQ,0);
                    }
                }
                else
                {
                    // AC
                    auiFrequencies = aqFrequencies[1][iAcIndex++];
                    if ( rqSPP.SuccApproxBitPosHigh == 0 )
                    {
                        // initial pass
                        for (int i1 = 0; i1 < rqDUArray.GetQuantity(); i1++)
                            EncodeAcInitial(auiFrequencies,rqDUArray[i1],rqSPP,iEobRun,0,0,acBitBuff,iBBQ,0);
                    }
                    else
                    {
                        // succeeding pass
                        for (int i1 = 0; i1 < rqDUArray.GetQuantity(); i1++)
                            EncodeAcSucceeding(auiFrequencies,rqDUArray[i1],rqSPP,iEobRun,0,0,acBitBuff,iBBQ,0);
                    }
                    EncodeHuffmanEobRun(auiFrequencies,iEobRun,0,0,acBitBuff,iBBQ,0);
                }
            }
        }
        else if ( m_eColorModel == CMT_YCC )
        {
            for (int i0 = 0; i0 < qSPPArray.GetQuantity(); i0++)
            {
                const ScanProgressionParams& rqSPP = qSPPArray[i0];

                if ( rqSPP.Dc )
                {
                    // DC
                    if ( rqSPP.SuccApproxBitPosHigh == 0 )
                    {
                        // initial pass
                        auiFrequencies = aqFrequencies[0][iDcIndex++];

                        for (int i1 = 0; i1 < 3; i1++)
                            qComponentBlockIndices[i1] = 0;
                        for (int i1 = 0; i1 < 3; i1++)
                            m_qPredArray[i1] = 0;

                        if ( m_eYCbCrChromaSampling == YCCCST_4_4_4 )
                        {
                            while ( qComponentBlockIndices[0] != m_qComponentBuffers[0].GetQuantity() )
                            {
                                for (int i1 = 0; i1 < 3; i1++)
                                {
                                    EncodeDcInitial(auiFrequencies,
                                        m_qComponentBuffers[i1][qComponentBlockIndices[i1]++],m_qPredArray[i1],
                                        rqSPP,0,0,0);
                                }
                            }
                        }
                        else  // m_eYCbCrChromaSampling = YCCCST_4_2_2
                        {
                            int iIDULTCX = 0;
                            int iIDULTCY = 0;
                            while ( qComponentBlockIndices[1] != m_qComponentBuffers[1].GetQuantity() )
                            {
                                for (int i1 = 0; i1 < 2; i1++)
                                {
                                    iVar = iIDULTCY*qComponentInDataUnitsExtensions[0].First;
                                    for (int i2 = 0; i2 < 2; i2++)
                                    {
                                        EncodeDcInitial(auiFrequencies,m_qComponentBuffers[0][iVar+iIDULTCX],
                                            m_qPredArray[0],rqSPP,0,0,0);

                                        iIDULTCX++;
                                    }

                                    iIDULTCX -= 2;
                                    iIDULTCY++;
                                }
                                iIDULTCX += 2;
                                if ( iIDULTCX != qComponentInDataUnitsExtensions[0].First )
                                    iIDULTCY -= 2;
                                else
                                    iIDULTCX = 0;

                                for (int i1 = 1; i1 < 3; i1++)
                                {
                                    EncodeDcInitial(auiFrequencies,
                                        m_qComponentBuffers[i1][qComponentBlockIndices[i1]++],m_qPredArray[i1],
                                        rqSPP,0,0,0);
                                }
                            }
                        }
                        EncodeHuffmanEobRun(auiFrequencies,iEobRun,0,0,acBitBuff,iBBQ,0);
                    }
                }
                else
                {
                    // AC
                    const StillArray<StillArray<short> >& rqDUArray = m_qComponentBuffers[rqSPP.Component];
                    auiFrequencies = aqFrequencies[1][iAcIndex++];
                    if ( rqSPP.SuccApproxBitPosHigh == 0 )
                    {
                        // initial pass
                        if ( !(rqSPP.Component == 0 && m_eYCbCrChromaSampling == YCCCST_4_2_2) )
                        {
                            for (int i1 = 0; i1 < rqDUArray.GetQuantity(); i1++)
                                EncodeAcInitial(auiFrequencies,rqDUArray[i1],rqSPP,iEobRun,0,0,acBitBuff,iBBQ,0);
                        }
                        else
                        {
                            int iIDUW = iAlignedWidth/8;
                            int iIDUH = iAlignedHeight/8;
                            for (int iY = 0; iY < iIDUH; iY++)
                            {
                                iVar = iY*qComponentInDataUnitsExtensions[0].First;
                                for (int iX = 0; iX < iIDUW; iX++)
                                {
                                    EncodeAcInitial(auiFrequencies,rqDUArray[iVar+iX],rqSPP,iEobRun,0,0,acBitBuff,
                                        iBBQ,0);
                                }
                            }
                        }
                    }
                    else
                    {
                        // succeeding pass
                        if ( !(rqSPP.Component == 0 && m_eYCbCrChromaSampling == YCCCST_4_2_2) )
                        {
                            for (int i1 = 0; i1 < rqDUArray.GetQuantity(); i1++)
                                EncodeAcSucceeding(auiFrequencies,rqDUArray[i1],rqSPP,iEobRun,0,0,acBitBuff,iBBQ,0);
                        }
                        else
                        {
                            int iIDUW = iAlignedWidth/8;
                            int iIDUH = iAlignedHeight/8;
                            for (int iY = 0; iY < iIDUH; iY++)
                            {
                                iVar = iY*qComponentInDataUnitsExtensions[0].First;
                                for (int iX = 0; iX < iIDUW; iX++)
                                {
                                    EncodeAcSucceeding(auiFrequencies,rqDUArray[iVar+iX],rqSPP,iEobRun,0,0,
                                        acBitBuff,iBBQ,0);
                                }
                            }
                        }
                    }
                    EncodeHuffmanEobRun(auiFrequencies,iEobRun,0,0,acBitBuff,iBBQ,0);
                }
            }
        }
        else  // m_eColorModel = CMT_RGB or m_eColorModel = CMT_YCCK or m_eColorModel = CMT_CMYK
        {
            for (int i0 = 0; i0 < qSPPArray.GetQuantity(); i0++)
            {
                const ScanProgressionParams& rqSPP = qSPPArray[i0];

                if ( rqSPP.Dc )
                {
                    // DC
                    if ( rqSPP.SuccApproxBitPosHigh == 0 )
                    {
                        // initial pass
                        auiFrequencies = aqFrequencies[0][iDcIndex++];

                        for (int i1 = 0; i1 < m_iComponentQuantity; i1++)
                            qComponentBlockIndices[i1] = 0;
                        for (int i1 = 0; i1 < m_iComponentQuantity; i1++)
                            m_qPredArray[i1] = 0;

                        while ( qComponentBlockIndices[0] != m_qComponentBuffers[0].GetQuantity() )
                        {
                            for (int i1 = 0; i1 < m_iComponentQuantity; i1++)
                            {
                                EncodeDcInitial(auiFrequencies,
                                    m_qComponentBuffers[i1][qComponentBlockIndices[i1]++],m_qPredArray[i1],
                                    rqSPP,0,0,0);
                            }
                        }
                        EncodeHuffmanEobRun(auiFrequencies,iEobRun,0,0,acBitBuff,iBBQ,0);
                    }
                }
                else
                {
                    // AC
                    const StillArray<StillArray<short> >& rqDUArray = m_qComponentBuffers[rqSPP.Component];
                    auiFrequencies = aqFrequencies[1][iAcIndex++];
                    if ( rqSPP.SuccApproxBitPosHigh == 0 )
                    {
                        // initial pass
                        for (int i1 = 0; i1 < rqDUArray.GetQuantity(); i1++)
                            EncodeAcInitial(auiFrequencies,rqDUArray[i1],rqSPP,iEobRun,0,0,acBitBuff,iBBQ,0);
                    }
                    else
                    {
                        // succeeding pass
                        for (int i1 = 0; i1 < rqDUArray.GetQuantity(); i1++)
                            EncodeAcSucceeding(auiFrequencies,rqDUArray[i1],rqSPP,iEobRun,0,0,acBitBuff,iBBQ,0);
                    }
                    EncodeHuffmanEobRun(auiFrequencies,iEobRun,0,0,acBitBuff,iBBQ,0);
                }
            }
        }
    }

    qProgress.SetDone(0.66f);

    StillArray<HuffmanTableSet> aqHuffmanTableSet[2];
    aqHuffmanTableSet[0].SetQuantity(aiEntropyCodingTableQuantity[0]);
    aqHuffmanTableSet[1].SetQuantity(aiEntropyCodingTableQuantity[1]);

    // generate huffman code tables and ordered derived tables
    StillArray<StillArray<StillArray<int> > > aqHuffOrderedTables[2];
    StillArray<int> qHuffCodeList, qHuffSizeList;
    int iK, iLastK;
    for (int i0 = 0; i0 < 2; i0++)
    {
        aqHuffOrderedTables[i0].SetQuantity(aiEntropyCodingTableQuantity[i0]);
        for (int i1 = 0; i1 < aiEntropyCodingTableQuantity[i0]; i1++)
        {
            HuffmanTableSet& rqHuffmanTableSet = aqHuffmanTableSet[i0][i1];

            // get "bits list" and "huffval list"
            rqHuffmanTableSet.HuffValList.SetQuantity(256);
            GetHuffmanTableSetFromStatistics(rqHuffmanTableSet,aqFrequencies[i0][i1]);

            // get ordered "code list" and "size list" for each huffman table set
            GetHuffmanDerivedTables(qHuffCodeList,qHuffSizeList,rqHuffmanTableSet,&iLastK);
            aqHuffOrderedTables[i0][i1].SetQuantity(2);
            StillArray<int>& rqHuffCodeOrderedList = aqHuffOrderedTables[i0][i1][0];
            StillArray<int>& rqHuffSizeOrderedList = aqHuffOrderedTables[i0][i1][1];
            rqHuffCodeOrderedList.SetQuantity(qHuffCodeList.GetQuantity());
            rqHuffSizeOrderedList.SetQuantity(qHuffSizeList.GetQuantity());
            iK = 0;
            for (/**/; /**/; /**/)
            {
                iI = rqHuffmanTableSet.HuffValList[iK];
                rqHuffCodeOrderedList[iI] = qHuffCodeList[iK];
                rqHuffSizeOrderedList[iI] = qHuffSizeList[iK];
                if ( ++iK >= iLastK )
                    break;
            }
        }
    }

    // Begin writing.
    // SOI marker
    unsigned char aucSOI[2] = {255, 216};
    rqStream.Write(2,aucSOI);

    if ( m_bHasJfifMarker )
    {
        // JFIF marker
        unsigned char aucJfifMarker[2] = {255, 224};
        rqStream.Write(2,aucJfifMarker);

        unsigned short usJfifMarkerSegmentLength = 16;
        rqStream.Write(usJfifMarkerSegmentLength);

        unsigned char aucJfifSignature[5] = {74, 70, 73, 70, 0};
        rqStream.Write(5,aucJfifSignature);

        // version
        unsigned char aucJfifVersion[2] = {1, 1};
        rqStream.Write(2,aucJfifVersion);

        unsigned char ucUnits;
        switch ( m_eJfifDotDensityMeaning )
        {
        case JFIFPDMT_ASPECT_RATIO:
            ucUnits = 0;
            break;
        case JFIFPDMT_PIXELS_PER_INCH:
            ucUnits = 1;
            break;
        case JFIFPDMT_PIXELS_PER_CENTIMETER:
            ucUnits = 2;
            break;
        }
        rqStream.Write(ucUnits);

        unsigned short usPDH = m_iJfifDotDensityHorizontally;
        unsigned short usPDV = m_iJfifDotDensityVertically;
        rqStream.Write(usPDH);
        rqStream.Write(usPDV);

        // no thumbnail
        unsigned char ucXThumbnail = 0;
        unsigned char ucYThumbnail = 0;
        rqStream.Write(ucXThumbnail);
        rqStream.Write(ucYThumbnail);
    }

    if ( m_bHasAdobeMarker )
    {
        // Adobe marker
        unsigned char aucAdobeMarker[2] = {255, 238};
        rqStream.Write(2,aucAdobeMarker);

        unsigned short usAdobeMarkerSegmentLength = 14;
        rqStream.Write(usAdobeMarkerSegmentLength);

        unsigned char aucAdobeSignature[5] = {65, 100, 111, 98, 101};
        rqStream.Write(5,aucAdobeSignature);

        // version
        unsigned short usVersion = 100;
        rqStream.Write(usVersion);

        // flags
        unsigned short usFlag0 = 0;
        unsigned short usFlag1 = 0;
        rqStream.Write(usFlag0);
        rqStream.Write(usFlag1);

        // color transformation
        unsigned char ucAdobeColorTransformation;
        switch ( m_eColorModel )
        {
        case CMT_YCC:
            ucAdobeColorTransformation = 1;
            break;
        case CMT_RGB:
            ucAdobeColorTransformation = 0;
            break;
        case CMT_YCCK:
            ucAdobeColorTransformation = 2;
            break;
        case CMT_CMYK:
            ucAdobeColorTransformation = 0;
            break;
        }
        rqStream.Write(ucAdobeColorTransformation);
    }

    if ( m_bHasExifMetadata )
    {
        // APP1/Exif marker
        StillArray<char> qData;
        m_spqExifMetadata->SaveData(qData,FormatExif::EDDT_STANDALONE_OR_JPEG);

        unsigned char aucExifMarker[2] = {255, 225};
        rqStream.Write(2,aucExifMarker);

        unsigned short usExifMarkerSegmentLength = 8 + qData.GetQuantity();
        rqStream.Write(usExifMarkerSegmentLength);

        unsigned char aucExifSignature[6] = {69, 120, 105, 102, 0, 0};
        rqStream.Write(6,aucExifSignature);

        // Exif metadata
        rqStream.Write(qData.GetQuantity(),qData);
    }

    // Quantization table(s).
    unsigned char aucDQTMarker[2] = {255, 219};
    rqStream.Write(2,aucDQTMarker);

    unsigned short usDQTMarkerSegmentLength = ( m_eColorModel != CMT_YCC && m_eColorModel != CMT_YCCK ? 67 : 132 );
    rqStream.Write(usDQTMarkerSegmentLength);

    rqStream.WriteBits(0,4);
    rqStream.WriteBits(0,4);
    if ( m_eColorModel == CMT_GRAYSCALE )
    {
        for (int i = 0; i < 64; i++)
            rqStream.Write(aucDSLuminanceQuantizationTable[gs_aiInvZigZag[i]]);
    }
    else if ( m_eColorModel == CMT_YCC || m_eColorModel == CMT_YCCK )
    {
        for (int i = 0; i < 64; i++)
            rqStream.Write(aucDSLuminanceQuantizationTable[gs_aiInvZigZag[i]]);

        rqStream.WriteBits(0,4);
        rqStream.WriteBits(1,4);
        for (int i = 0; i < 64; i++)
            rqStream.Write(aucDSChrominanceQuantizationTable[gs_aiInvZigZag[i]]);
    }
    else if ( m_eColorModel == CMT_RGB || m_eColorModel == CMT_CMYK )
    {
        for (int i = 0; i < 64; i++)
            rqStream.Write(aucDSChrominanceQuantizationTable[gs_aiInvZigZag[i]]);
    }

    // SOF
    unsigned char ucByte = 255;
    rqStream.Write(ucByte);
    ucByte = ( m_eDataOrganization == DOT_PLANE ? 192 : 194 );
    rqStream.Write(ucByte);

    unsigned short usSOFMarkerSegmentLength = 8 + 3*m_iComponentQuantity;
    rqStream.Write(usSOFMarkerSegmentLength);

    unsigned char ucSamplePrecision = 8;
    rqStream.Write(ucSamplePrecision);

    unsigned short usComponentMaxHeight = m_iHeight;
    unsigned short usComponentMaxWidth = m_iWidth;
    rqStream.Write(usComponentMaxHeight);
    rqStream.Write(usComponentMaxWidth);

    unsigned char ucComponentQuantity = m_iComponentQuantity;
    rqStream.Write(ucComponentQuantity);

    for (int i = 0; i < m_iComponentQuantity; i++)
    {
        const FrameCompSpecParams& rqFCSP = m_qFrameCompSpecParamsArray[i];

        rqStream.Write(rqFCSP.ComponentIdentifier);
        rqStream.WriteBits(rqFCSP.SamplingFactorH,4);
        rqStream.WriteBits(rqFCSP.SamplingFactorV,4);
        rqStream.Write(rqFCSP.QuantTableDestSelector);
    }

    // huffman tables (for non-progressive)
    unsigned char aucDHTMarker[2] = {255, 196};
    unsigned short usDHTMarkerSegmentLength;
    unsigned char ucValue;
    if ( m_eDataOrganization == DOT_PLANE )
    {
        int aiLSum[2];
        for (int i0 = 0; i0 < aiEntropyCodingTableQuantity[0]; i0++)
        {
            HuffmanTableSet* apqHuffmanTableSet[2] = {
                &aqHuffmanTableSet[0][i0],
                &aqHuffmanTableSet[1][i0]};

            rqStream.Write(2,aucDHTMarker);

            aiLSum[0] = 0;
            aiLSum[1] = 0;
            for (int i1 = 0; i1 < 2; i1++)
            {
                for (int i2 = 1; i2 < 17; i2++)
                    aiLSum[i1] += apqHuffmanTableSet[i1]->BitsList[i2];
            }
            usDHTMarkerSegmentLength = 36 + aiLSum[0] + aiLSum[1];
            rqStream.Write(usDHTMarkerSegmentLength);

            for (int i1 = 0; i1 < 2; i1++)
            {
                rqStream.WriteBits(i1,4);
                rqStream.WriteBits(i0,4);
                for (int i2 = 1; i2 < 17; i2++)
                {
                    ucValue = apqHuffmanTableSet[i1]->BitsList[i2];
                    rqStream.Write(ucValue);
                }
                for (int i2 = 0; i2 < aiLSum[i1]; i2++)
                {
                    ucValue = apqHuffmanTableSet[i1]->HuffValList[i2];
                    rqStream.Write(ucValue);
                }
            }
        }
    }

    // scans
    unsigned char aucSOSMarker[2] = {255, 218};
    char cStuffingByte = 0;
    if ( m_eDataOrganization == DOT_PLANE )
    {
        // SOS
        rqStream.Write(2,aucSOSMarker);
        unsigned short usSOSMarkerSegmentLength = 6 + 2*m_iComponentQuantity;
        rqStream.Write(usSOSMarkerSegmentLength);
        unsigned char ucComponentQuantity = m_iComponentQuantity;
        rqStream.Write(ucComponentQuantity);
        unsigned char ucScanComponentSelector;
        for (int i = 0; i < m_iComponentQuantity; i++)
        {
            ucScanComponentSelector = m_qFrameCompSpecParamsArray[i].ComponentIdentifier;
            rqStream.Write(ucScanComponentSelector);
            if ( m_eColorModel == CMT_GRAYSCALE || (m_eColorModel == CMT_YCC && i == 0) || m_eColorModel == CMT_RGB
            ||   (m_eColorModel == CMT_YCCK && (i == 0 || i == 3)) || m_eColorModel == CMT_CMYK )
            {
                rqStream.WriteBits(0,4);
                rqStream.WriteBits(0,4);
            }
            else
            {
                rqStream.WriteBits(1,4);
                rqStream.WriteBits(1,4);
            }
        }
        unsigned char ucStartOfSpectral = 0;
        unsigned char ucEndOfSpectral = 63;
        rqStream.Write(ucStartOfSpectral);
        rqStream.Write(ucEndOfSpectral);
        rqStream.WriteBits(0,4);
        rqStream.WriteBits(0,4);

        // Write image interleaving components.
        Stream qImageStream(false);

        for (int i = 0; i < m_iComponentQuantity; i++)
            qComponentBlockIndices[i] = 0;
        for (int i = 0; i < m_iComponentQuantity; i++)
            m_qPredArray[i] = 0;

        if ( m_eColorModel == CMT_GRAYSCALE )
        {
            while ( qComponentBlockIndices[0] != m_qComponentBuffers[0].GetQuantity() )
            {
                EncodeBlockHuffman(m_qComponentBuffers[0][qComponentBlockIndices[0]++],m_qPredArray[0],
                    aqHuffOrderedTables[0][0][0],aqHuffOrderedTables[0][0][1],
                    aqHuffOrderedTables[1][0][0],aqHuffOrderedTables[1][0][1],qImageStream);

                qProgress.SetDone(0.66f+0.33f*((float)qComponentBlockIndices[0])/
                    m_qComponentBuffers[0].GetQuantity());
            }
        }
        else if ( m_eColorModel == CMT_YCC )
        {
            if ( m_eYCbCrChromaSampling == YCCCST_4_4_4 )
            {
                while ( qComponentBlockIndices[0] != m_qComponentBuffers[0].GetQuantity() )
                {
                    EncodeBlockHuffman(m_qComponentBuffers[0][qComponentBlockIndices[0]++],m_qPredArray[0],
                        aqHuffOrderedTables[0][0][0],aqHuffOrderedTables[0][0][1],
                        aqHuffOrderedTables[1][0][0],aqHuffOrderedTables[1][0][1],qImageStream);

                    for (int i = 1; i < 3; i++)
                    {
                        EncodeBlockHuffman(m_qComponentBuffers[i][qComponentBlockIndices[i]++],m_qPredArray[i],
                            aqHuffOrderedTables[0][1][0],aqHuffOrderedTables[0][1][1],
                            aqHuffOrderedTables[1][1][0],aqHuffOrderedTables[1][1][1],qImageStream);
                    }

                    qProgress.SetDone(0.66f+0.33f*((float)qComponentBlockIndices[0])/
                        m_qComponentBuffers[0].GetQuantity());
                }
            }
            else  // eYCbCrChromaSampling = YCCCST_4_2_2
            {
                const StillArray<StillArray<short> >& rqDUArrayY = m_qComponentBuffers[0];
                int iIDULTCX = 0;
                int iIDULTCY = 0;
                while ( qComponentBlockIndices[1] != m_qComponentBuffers[1].GetQuantity() )
                {
                    for (int i0 = 0; i0 < 2; i0++)
                    {
                        iVar = iIDULTCY*qComponentInDataUnitsExtensions[0].First;
                        for (int i1 = 0; i1 < 2; i1++)
                        {
                            EncodeBlockHuffman(rqDUArrayY[iVar+iIDULTCX],m_qPredArray[0],
                                aqHuffOrderedTables[0][0][0],aqHuffOrderedTables[0][0][1],
                                aqHuffOrderedTables[1][0][0],aqHuffOrderedTables[1][0][1],qImageStream);
                            iIDULTCX++;
                        }

                        iIDULTCX -= 2;
                        iIDULTCY++;
                    }
                    iIDULTCX += 2;
                    if ( iIDULTCX != qComponentInDataUnitsExtensions[0].First )
                        iIDULTCY -= 2;
                    else
                        iIDULTCX = 0;

                    for (int i = 1; i < 3; i++)
                    {
                        EncodeBlockHuffman(m_qComponentBuffers[i][qComponentBlockIndices[i]++],m_qPredArray[i],
                            aqHuffOrderedTables[0][1][0],aqHuffOrderedTables[0][1][1],
                            aqHuffOrderedTables[1][1][0],aqHuffOrderedTables[1][1][1],qImageStream);
                    }

                    qProgress.SetDone(0.66f+0.33f*((float)qComponentBlockIndices[1])/
                        m_qComponentBuffers[1].GetQuantity());
                }
            }
        }
        else if ( m_eColorModel == CMT_RGB )
        {
            while ( qComponentBlockIndices[0] != m_qComponentBuffers[0].GetQuantity() )
            {
                for (int i = 0; i < 3; i++)
                {
                    EncodeBlockHuffman(m_qComponentBuffers[i][qComponentBlockIndices[i]++],m_qPredArray[i],
                        aqHuffOrderedTables[0][0][0],aqHuffOrderedTables[0][0][1],
                        aqHuffOrderedTables[1][0][0],aqHuffOrderedTables[1][0][1],qImageStream);
                }

                qProgress.SetDone(0.66f+0.33f*((float)qComponentBlockIndices[0])/
                    m_qComponentBuffers[0].GetQuantity());
            }
        }
        else if ( m_eColorModel == CMT_YCCK )
        {
            while ( qComponentBlockIndices[0] != m_qComponentBuffers[0].GetQuantity() )
            {
                EncodeBlockHuffman(m_qComponentBuffers[0][qComponentBlockIndices[0]++],m_qPredArray[0],
                    aqHuffOrderedTables[0][0][0],aqHuffOrderedTables[0][0][1],
                    aqHuffOrderedTables[1][0][0],aqHuffOrderedTables[1][0][1],qImageStream);

                for (int i = 1; i < 3; i++)
                {
                    EncodeBlockHuffman(m_qComponentBuffers[i][qComponentBlockIndices[i]++],m_qPredArray[i],
                        aqHuffOrderedTables[0][1][0],aqHuffOrderedTables[0][1][1],
                        aqHuffOrderedTables[1][1][0],aqHuffOrderedTables[1][1][1],qImageStream);
                }

                EncodeBlockHuffman(m_qComponentBuffers[3][qComponentBlockIndices[3]++],m_qPredArray[3],
                    aqHuffOrderedTables[0][0][0],aqHuffOrderedTables[0][0][1],
                    aqHuffOrderedTables[1][0][0],aqHuffOrderedTables[1][0][1],qImageStream);

                qProgress.SetDone(0.66f+0.33f*((float)qComponentBlockIndices[0])/
                    m_qComponentBuffers[0].GetQuantity());
            }
        }
        else  // m_eColorModel = CMT_CMYK
        {
            while ( qComponentBlockIndices[0] != m_qComponentBuffers[0].GetQuantity() )
            {
                for (int i = 0; i < 4; i++)
                {
                    EncodeBlockHuffman(m_qComponentBuffers[i][qComponentBlockIndices[i]++],m_qPredArray[i],
                        aqHuffOrderedTables[0][0][0],aqHuffOrderedTables[0][0][1],
                        aqHuffOrderedTables[1][0][0],aqHuffOrderedTables[1][0][1],qImageStream);
                }

                qProgress.SetDone(0.66f+0.33f*((float)qComponentBlockIndices[0])/
                    m_qComponentBuffers[0].GetQuantity());
            }
        }

        if ( qImageStream.GetBitRatio() != 0 )
        {
            // fill remaining space by 1s
            int iBitsLeft = 8 - qImageStream.GetBitRatio();
            qImageStream.WriteBits((1 << iBitsLeft)-1,iBitsLeft);
        }

        // copy to the main stream with byte stuffing
        unsigned char* pucByte = (unsigned char*)qImageStream.GetBytes();
        char cStuffingByte = 0;
        for (int i = 0; i < qImageStream.GetBytePosition(); i++)
        {
            rqStream.Write(*pucByte);
            if ( *pucByte == 255 )
                rqStream.Write(cStuffingByte);
            pucByte++;
        }
    }
    else  // m_eDataOrganization = DOT_PROGRESSIVE
    {
        iEobRun = 0;
        iBBQ = 0;
        iDcIndex = iAcIndex = 0;
        int iLSum;
        unsigned short usSOSMarkerSegmentLength;
        unsigned char ucComponentQuantity, ucScanComponentSelector, ucStartOfSpectral, ucEndOfSpectral;
        unsigned char* pucByte;
        for (int i0 = 0; i0 < qSPPArray.GetQuantity(); i0++)
        {
            const ScanProgressionParams& rqSPP = qSPPArray[i0];

            // huffman tables
            if ( !(rqSPP.Dc && rqSPP.SuccApproxBitPosHigh != 0) )
            {
                HuffmanTableSet* pqHuffmanTableSet;
                if ( rqSPP.Dc )
                    pqHuffmanTableSet = &aqHuffmanTableSet[0][iDcIndex];
                else
                    pqHuffmanTableSet = &aqHuffmanTableSet[1][iAcIndex];

                rqStream.Write(2,aucDHTMarker);

                iLSum = 0;
                for (int i1 = 1; i1 < 17; i1++)
                    iLSum += pqHuffmanTableSet->BitsList[i1];
                usDHTMarkerSegmentLength = 19 + iLSum;
                rqStream.Write(usDHTMarkerSegmentLength);

                rqStream.WriteBits(( rqSPP.Dc ? 0 : 1 ),4);
                rqStream.WriteBits(0,4);
                for (int i1 = 1; i1 < 17; i1++)
                {
                    ucValue = pqHuffmanTableSet->BitsList[i1];
                    rqStream.Write(ucValue);
                }
                for (int i1 = 0; i1 < iLSum; i1++)
                {
                    ucValue = pqHuffmanTableSet->HuffValList[i1];
                    rqStream.Write(ucValue);
                }
            }

            // SOS
            rqStream.Write(2,aucSOSMarker);
            ucComponentQuantity = ( rqSPP.Dc ? m_iComponentQuantity : 1 );
            usSOSMarkerSegmentLength = 6 + 2*ucComponentQuantity;
            rqStream.Write(usSOSMarkerSegmentLength);
            rqStream.Write(ucComponentQuantity);
            if ( rqSPP.Dc )
            {
                for (int i1 = 0; i1 < ucComponentQuantity; i1++)
                {
                    ucScanComponentSelector = m_qFrameCompSpecParamsArray[i1].ComponentIdentifier;
                    rqStream.Write(ucScanComponentSelector);
                    rqStream.WriteBits(0,4);
                    rqStream.WriteBits(0,4);
                }
            }
            else
            {
                ucScanComponentSelector = m_qFrameCompSpecParamsArray[rqSPP.Component].ComponentIdentifier;
                rqStream.Write(ucScanComponentSelector);
                rqStream.WriteBits(0,4);
                rqStream.WriteBits(0,4);
            }
            ucStartOfSpectral = rqSPP.StartOfSpectral;
            ucEndOfSpectral = rqSPP.EndOfSpectral;
            rqStream.Write(ucStartOfSpectral);
            rqStream.Write(ucEndOfSpectral);
            rqStream.WriteBits(rqSPP.SuccApproxBitPosHigh,4);
            rqStream.WriteBits(rqSPP.SuccApproxBitPosLow,4);

            // write the image
            Stream qImageStream(false);
            if ( m_eColorModel == CMT_GRAYSCALE )
            {
                const StillArray<StillArray<short> >& rqDUArray = m_qComponentBuffers[0];

                if ( rqSPP.Dc )
                {
                    // DC
                    if ( rqSPP.SuccApproxBitPosHigh == 0 )
                    {
                        // initial pass
                        const StillArray<int>& rqHuffCodeOrderedList = aqHuffOrderedTables[0][iDcIndex][0];
                        const StillArray<int>& rqHuffSizeOrderedList = aqHuffOrderedTables[0][iDcIndex][1];

                        m_qPredArray[0] = 0;
                        for (int i1 = 0; i1 < rqDUArray.GetQuantity(); i1++)
                        {
                            EncodeDcInitial(0,rqDUArray[i1],m_qPredArray[0],rqSPP,rqHuffCodeOrderedList,
                                rqHuffSizeOrderedList,&qImageStream);
                        }
                        EncodeHuffmanEobRun(0,iEobRun,rqHuffCodeOrderedList,rqHuffSizeOrderedList,acBitBuff,iBBQ,
                            &qImageStream);
                    }
                    else
                    {
                        // succeeding pass
                        for (int i1 = 0; i1 < rqDUArray.GetQuantity(); i1++)
                            EncodeDcSucceeding(rqDUArray[i1],rqSPP,&qImageStream);

                        EncodeHuffmanEobRun(0,iEobRun,0,0,acBitBuff,iBBQ,&qImageStream);
                    }
                }
                else
                {
                    // AC
                    const StillArray<int>& rqHuffCodeOrderedList = aqHuffOrderedTables[1][iAcIndex][0];
                    const StillArray<int>& rqHuffSizeOrderedList = aqHuffOrderedTables[1][iAcIndex][1];

                    if ( rqSPP.SuccApproxBitPosHigh == 0 )
                    {
                        // initial pass
                        for (int i1 = 0; i1 < rqDUArray.GetQuantity(); i1++)
                        {
                            EncodeAcInitial(0,rqDUArray[i1],rqSPP,iEobRun,rqHuffCodeOrderedList,
                                rqHuffSizeOrderedList,acBitBuff,iBBQ,&qImageStream);
                        }
                    }
                    else
                    {
                        // succeeding pass
                        for (int i1 = 0; i1 < rqDUArray.GetQuantity(); i1++)
                        {
                            EncodeAcSucceeding(0,rqDUArray[i1],rqSPP,iEobRun,rqHuffCodeOrderedList,
                                rqHuffSizeOrderedList,acBitBuff,iBBQ,&qImageStream);
                        }
                    }
                    EncodeHuffmanEobRun(0,iEobRun,rqHuffCodeOrderedList,rqHuffSizeOrderedList,acBitBuff,iBBQ,
                        &qImageStream);
                }
            }
            else if ( m_eColorModel == CMT_YCC )
            {
                if ( rqSPP.Dc )
                {
                    // DC
                    if ( rqSPP.SuccApproxBitPosHigh == 0 )
                    {
                        // initial pass
                        const StillArray<int>& rqHuffCodeOrderedList = aqHuffOrderedTables[0][iDcIndex][0];
                        const StillArray<int>& rqHuffSizeOrderedList = aqHuffOrderedTables[0][iDcIndex][1];

                        for (int i1 = 0; i1 < 3; i1++)
                            qComponentBlockIndices[i1] = 0;
                        for (int i1 = 0; i1 < 3; i1++)
                            m_qPredArray[i1] = 0;

                        if ( m_eYCbCrChromaSampling == YCCCST_4_4_4 )
                        {
                            while ( qComponentBlockIndices[0] != m_qComponentBuffers[0].GetQuantity() )
                            {
                                for (int i1 = 0; i1 < 3; i1++)
                                {
                                    EncodeDcInitial(0,m_qComponentBuffers[i1][qComponentBlockIndices[i1]++],
                                        m_qPredArray[i1],rqSPP,rqHuffCodeOrderedList,rqHuffSizeOrderedList,
                                        &qImageStream);
                                }
                            }
                        }
                        else  // m_eYCbCrChromaSampling = YCCCST_4_2_2
                        {
                            int iIDULTCX = 0;
                            int iIDULTCY = 0;
                            while ( qComponentBlockIndices[1] != m_qComponentBuffers[1].GetQuantity() )
                            {
                                for (int i1 = 0; i1 < 2; i1++)
                                {
                                    iVar = iIDULTCY*qComponentInDataUnitsExtensions[0].First;
                                    for (int i2 = 0; i2 < 2; i2++)
                                    {
                                        EncodeDcInitial(0,m_qComponentBuffers[0][iVar+iIDULTCX],m_qPredArray[0],
                                            rqSPP,rqHuffCodeOrderedList,rqHuffSizeOrderedList,&qImageStream);

                                        iIDULTCX++;
                                    }

                                    iIDULTCX -= 2;
                                    iIDULTCY++;
                                }
                                iIDULTCX += 2;
                                if ( iIDULTCX != qComponentInDataUnitsExtensions[0].First )
                                    iIDULTCY -= 2;
                                else
                                    iIDULTCX = 0;

                                for (int i1 = 1; i1 < 3; i1++)
                                {
                                    EncodeDcInitial(0,m_qComponentBuffers[i1][qComponentBlockIndices[i1]++],
                                        m_qPredArray[i1],rqSPP,rqHuffCodeOrderedList,rqHuffSizeOrderedList,
                                        &qImageStream);
                                }
                            }
                        }
                        EncodeHuffmanEobRun(0,iEobRun,rqHuffCodeOrderedList,rqHuffSizeOrderedList,acBitBuff,iBBQ,
                            &qImageStream);
                    }
                    else
                    {
                        // succeeding pass
                        for (int i1 = 0; i1 < 3; i1++)
                            qComponentBlockIndices[i1] = 0;

                        if ( m_eYCbCrChromaSampling == YCCCST_4_4_4 )
                        {
                            while ( qComponentBlockIndices[0] != m_qComponentBuffers[0].GetQuantity() )
                            {
                                for (int i1 = 0; i1 < 3; i1++)
                                {
                                    EncodeDcSucceeding(m_qComponentBuffers[i1][qComponentBlockIndices[i1]++],rqSPP,
                                        &qImageStream);
                                }
                            }
                        }
                        else  // m_eYCbCrChromaSampling = YCCCST_4_2_2
                        {
                            int iIDULTCX = 0;
                            int iIDULTCY = 0;
                            while ( qComponentBlockIndices[1] != m_qComponentBuffers[1].GetQuantity() )
                            {
                                for (int i1 = 0; i1 < 2; i1++)
                                {
                                    iVar = iIDULTCY*qComponentInDataUnitsExtensions[0].First;
                                    for (int i2 = 0; i2 < 2; i2++)
                                    {
                                        EncodeDcSucceeding(m_qComponentBuffers[0][iVar+iIDULTCX],rqSPP,
                                            &qImageStream);

                                        iIDULTCX++;
                                    }

                                    iIDULTCX -= 2;
                                    iIDULTCY++;
                                }
                                iIDULTCX += 2;
                                if ( iIDULTCX != qComponentInDataUnitsExtensions[0].First )
                                    iIDULTCY -= 2;
                                else
                                    iIDULTCX = 0;

                                for (int i1 = 1; i1 < 3; i1++)
                                {
                                    EncodeDcSucceeding(m_qComponentBuffers[i1][qComponentBlockIndices[i1]++],rqSPP,
                                        &qImageStream);
                                }
                            }
                        }
                        EncodeHuffmanEobRun(0,iEobRun,0,0,acBitBuff,iBBQ,&qImageStream);
                    }
                }
                else
                {
                    // AC
                    const StillArray<StillArray<short> >& rqDUArray = m_qComponentBuffers[rqSPP.Component];
                    const StillArray<int>& rqHuffCodeOrderedList = aqHuffOrderedTables[1][iAcIndex][0];
                    const StillArray<int>& rqHuffSizeOrderedList = aqHuffOrderedTables[1][iAcIndex][1];

                    if ( rqSPP.SuccApproxBitPosHigh == 0 )
                    {
                        // initial pass
                        if ( !(rqSPP.Component == 0 && m_eYCbCrChromaSampling == YCCCST_4_2_2) )
                        {
                            for (int i1 = 0; i1 < rqDUArray.GetQuantity(); i1++)
                            {
                                EncodeAcInitial(0,rqDUArray[i1],rqSPP,iEobRun,rqHuffCodeOrderedList,
                                    rqHuffSizeOrderedList,acBitBuff,iBBQ,&qImageStream);
                            }
                        }
                        else
                        {
                            int iIDUW = iAlignedWidth/8;
                            int iIDUH = iAlignedHeight/8;
                            for (int iY = 0; iY < iIDUH; iY++)
                            {
                                iVar = iY*qComponentInDataUnitsExtensions[0].First;
                                for (int iX = 0; iX < iIDUW; iX++)
                                {
                                    EncodeAcInitial(0,rqDUArray[iVar+iX],rqSPP,iEobRun,rqHuffCodeOrderedList,
                                        rqHuffSizeOrderedList,acBitBuff,iBBQ,&qImageStream);
                                }
                            }
                        }
                    }
                    else
                    {
                        // succeeding pass
                        if ( !(rqSPP.Component == 0 && m_eYCbCrChromaSampling == YCCCST_4_2_2) )
                        {
                            for (int i1 = 0; i1 < rqDUArray.GetQuantity(); i1++)
                            {
                                EncodeAcSucceeding(0,rqDUArray[i1],rqSPP,iEobRun,rqHuffCodeOrderedList,
                                    rqHuffSizeOrderedList,acBitBuff,iBBQ,&qImageStream);
                            }
                        }
                        else
                        {
                            int iIDUW = iAlignedWidth/8;
                            int iIDUH = iAlignedHeight/8;
                            for (int iY = 0; iY < iIDUH; iY++)
                            {
                                iVar = iY*qComponentInDataUnitsExtensions[0].First;
                                for (int iX = 0; iX < iIDUW; iX++)
                                {
                                    EncodeAcSucceeding(0,rqDUArray[iVar+iX],rqSPP,iEobRun,rqHuffCodeOrderedList,
                                        rqHuffSizeOrderedList,acBitBuff,iBBQ,&qImageStream);
                                }
                            }
                        }
                    }
                    EncodeHuffmanEobRun(0,iEobRun,rqHuffCodeOrderedList,rqHuffSizeOrderedList,acBitBuff,iBBQ,
                        &qImageStream);
                }
            }
            else  // m_eColorModel = CMT_RGB or m_eColorModel = CMT_YCCK or m_eColorModel = CMT_CMYK
            {
                if ( rqSPP.Dc )
                {
                    // DC
                    if ( rqSPP.SuccApproxBitPosHigh == 0 )
                    {
                        // initial pass
                        const StillArray<int>& rqHuffCodeOrderedList = aqHuffOrderedTables[0][iDcIndex][0];
                        const StillArray<int>& rqHuffSizeOrderedList = aqHuffOrderedTables[0][iDcIndex][1];

                        for (int i1 = 0; i1 < m_iComponentQuantity; i1++)
                            qComponentBlockIndices[i1] = 0;
                        for (int i1 = 0; i1 < m_iComponentQuantity; i1++)
                            m_qPredArray[i1] = 0;

                        while ( qComponentBlockIndices[0] != m_qComponentBuffers[0].GetQuantity() )
                        {
                            for (int i1 = 0; i1 < m_iComponentQuantity; i1++)
                            {
                                EncodeDcInitial(0,m_qComponentBuffers[i1][qComponentBlockIndices[i1]++],
                                    m_qPredArray[i1],rqSPP,rqHuffCodeOrderedList,rqHuffSizeOrderedList,
                                    &qImageStream);
                            }
                        }
                        EncodeHuffmanEobRun(0,iEobRun,rqHuffCodeOrderedList,rqHuffSizeOrderedList,acBitBuff,iBBQ,
                            &qImageStream);
                    }
                    else
                    {
                        // succeeding pass
                        for (int i1 = 0; i1 < m_iComponentQuantity; i1++)
                            qComponentBlockIndices[i1] = 0;

                        while ( qComponentBlockIndices[0] != m_qComponentBuffers[0].GetQuantity() )
                        {
                            for (int i1 = 0; i1 < m_iComponentQuantity; i1++)
                            {
                                EncodeDcSucceeding(m_qComponentBuffers[i1][qComponentBlockIndices[i1]++],rqSPP,
                                    &qImageStream);
                            }
                        }
                        EncodeHuffmanEobRun(0,iEobRun,0,0,acBitBuff,iBBQ,&qImageStream);
                    }
                }
                else
                {
                    // AC
                    const StillArray<StillArray<short> >& rqDUArray = m_qComponentBuffers[rqSPP.Component];
                    const StillArray<int>& rqHuffCodeOrderedList = aqHuffOrderedTables[1][iAcIndex][0];
                    const StillArray<int>& rqHuffSizeOrderedList = aqHuffOrderedTables[1][iAcIndex][1];

                    if ( rqSPP.SuccApproxBitPosHigh == 0 )
                    {
                        // initial pass
                        for (int i1 = 0; i1 < rqDUArray.GetQuantity(); i1++)
                        {
                            EncodeAcInitial(0,rqDUArray[i1],rqSPP,iEobRun,rqHuffCodeOrderedList,
                                rqHuffSizeOrderedList,acBitBuff,iBBQ,&qImageStream);
                        }
                    }
                    else
                    {
                        // succeeding pass
                        for (int i1 = 0; i1 < rqDUArray.GetQuantity(); i1++)
                        {
                            EncodeAcSucceeding(0,rqDUArray[i1],rqSPP,iEobRun,rqHuffCodeOrderedList,
                                rqHuffSizeOrderedList,acBitBuff,iBBQ,&qImageStream);
                        }
                    }
                    EncodeHuffmanEobRun(0,iEobRun,rqHuffCodeOrderedList,rqHuffSizeOrderedList,acBitBuff,iBBQ,
                        &qImageStream);
                }
            }

            if ( rqSPP.Dc )
            {
                if ( rqSPP.SuccApproxBitPosHigh == 0 )
                    iDcIndex++;
            }
            else
            {
                iAcIndex++;
            }

            if ( qImageStream.GetBitRatio() != 0 )
            {
                // fill remaining space by 1s
                int iBitsLeft = 8 - qImageStream.GetBitRatio();
                qImageStream.WriteBits((1 << iBitsLeft)-1,iBitsLeft);
            }

            // copy to the main stream with byte stuffing
            pucByte = (unsigned char*)qImageStream.GetBytes();
            for (int i1 = 0; i1 < qImageStream.GetBytePosition(); i1++)
            {
                rqStream.Write(*pucByte);
                if ( *pucByte == 255 )
                    rqStream.Write(cStuffingByte);
                pucByte++;
            }

            qProgress.SetDone(0.66f+0.33f*((float)i0)/qSPPArray.GetQuantity());
        }
    }

    // EOI
    unsigned char aucEOIMarker[2] = {255, 217};
    rqStream.Write(2,aucEOIMarker);
}
//------------------------------------------------------------------------------------------------------------------
void FormatJpeg::GatherStatistics (const StillArray<short>& rqBlock, int& riPrevDc, unsigned int* auiDcFrequencies,
    unsigned int* auiAcFrequencies)
{
    int iValue = rqBlock[0] - riPrevDc;
    riPrevDc = rqBlock[0];
    if ( iValue < 0 )
        iValue = -iValue;
    int iBitSize = 0;
    while ( iValue != 0 )
    {
        iBitSize++;
        iValue >>= 1;
    }
    auiDcFrequencies[iBitSize]++;
    int iJ = 0;
    for (int i = 1; i < 64; i++)
    {
        iValue = rqBlock[gs_aiInvZigZag[i]];
        if ( iValue == 0 )
        {
            iJ++;
        }
        else
        {
            while ( iJ > 15 )
            {
                auiAcFrequencies[240]++;
                iJ -= 16;
            }
            if ( iValue < 0 )
                iValue = -iValue;
            iBitSize = 1;
            iValue >>= 1;
            while ( iValue != 0 )
            {
                iBitSize++;
                iValue >>= 1;
            }
            auiAcFrequencies[(iJ << 4)+iBitSize]++;
            iJ = 0;
        }
    }
    if ( iJ > 0 )
        auiAcFrequencies[0]++;
}
//------------------------------------------------------------------------------------------------------------------
void FormatJpeg::GetHuffmanTableSetFromStatistics (HuffmanTableSet& rqHTS, unsigned int* auiFrequencies)
{
    int aiBitsList[33];
    int aiSize[257];
    int aiRem[257];
    int iCode0, iCode1, iI, iJ, iK;
    unsigned int uiVar;

    MemSet(aiBitsList,0,132);
    MemSet(aiSize,0,1028);
    for (iI = 0; iI < 257; iI++)
        aiRem[iI] = -1;

    auiFrequencies[256] = 1;
    for (/**/; /**/; /**/)
    {
        iCode0 = -1;
        uiVar = Mathi::MAX_INT;
        for (iI = 0; iI <= 256; iI++)
        {
            if ( auiFrequencies[iI] != 0 && auiFrequencies[iI] <= uiVar )
            {
                uiVar = auiFrequencies[iI];
                iCode0 = iI;
            }
        }
        iCode1 = -1;
        uiVar = Mathi::MAX_INT;
        for (iI = 0; iI <= 256; iI++)
        {
            if ( auiFrequencies[iI] != 0 && auiFrequencies[iI] <= uiVar && iI != iCode0 )
            {
                uiVar = auiFrequencies[iI];
                iCode1 = iI;
            }
        }
        if ( iCode1 < 0 )
            break;

        auiFrequencies[iCode0] += auiFrequencies[iCode1];
        auiFrequencies[iCode1] = 0;
        aiSize[iCode0]++;
        while ( aiRem[iCode0] >= 0 )
        {
            iCode0 = aiRem[iCode0];
            aiSize[iCode0]++;
        }
        aiRem[iCode0] = iCode1;
        aiSize[iCode1]++;
        while ( aiRem[iCode1] >= 0 )
        {
            iCode1 = aiRem[iCode1];
            aiSize[iCode1]++;
        }
    }

    for (iI = 0; iI <= 256; iI++)
    {
        if ( aiSize[iI] != 0 )
            aiBitsList[aiSize[iI]]++;
    }
    for (iI = 32; iI > 16; iI--)
    {
        while ( aiBitsList[iI] > 0 )
        {
            iK = iI - 2;
            while ( aiBitsList[iK] == 0 )
                iK--;
            aiBitsList[iI] -= 2;
            aiBitsList[iI-1]++;
            aiBitsList[iK+1] += 2;
            aiBitsList[iK]--;
        }
    }
    while ( aiBitsList[iI] == 0 )
        iI--;
    aiBitsList[iI]--;
    MemCpy(rqHTS.BitsList,aiBitsList,68);
    iJ = 0;
    for (iI = 1; iI <= 32; iI++)
    {
        for (iK = 0; iK <= 255; iK++)
        {
            if ( aiSize[iK] == iI )
                rqHTS.HuffValList[iJ++] = iK;
        }
    }
}
//------------------------------------------------------------------------------------------------------------------
int FormatJpeg::GetHuffmanDerivedTables (StillArray<int>& rqHuffCodeList, StillArray<int>& rqHuffSizeList,
    const HuffmanTableSet& rqHTS, int* piLastK)
{
    // generate "size table"
    rqHuffSizeList.SetQuantity(rqHTS.HuffValList.GetQuantity()+1);
    int iK = 0;
    int iI = 1;
    int iJ = 1;
    for (/**/; /**/; /**/)
    {
        RETURN_POS_ONE_ON_FAIL(( iI < 17 ));

        if ( iJ <= rqHTS.BitsList[iI] )
        {
            RETURN_POS_ONE_ON_FAIL(( iK < rqHuffSizeList.GetQuantity() ));
            rqHuffSizeList[iK++] = iI;
            iJ++;
        }
        else
        {
            iJ = 1;
            if ( ++iI > 16 )
            {
                RETURN_POS_ONE_ON_FAIL(( iK < rqHuffSizeList.GetQuantity() ));
                rqHuffSizeList[iK] = 0;
                break;
            }
        }
    }

    if ( piLastK )
        *piLastK = iK;

    // generate "code table"
    rqHuffCodeList.SetQuantity(rqHTS.HuffValList.GetQuantity());
    iK = 0;
    int iCode = 0;
    int iSize = rqHuffSizeList[0];
    for (/**/; /**/; /**/)
    {
        RETURN_POS_ONE_ON_FAIL(( iK < rqHuffCodeList.GetQuantity() ));
        rqHuffCodeList[iK++] = iCode++;
        RETURN_POS_ONE_ON_FAIL(( iK < rqHuffSizeList.GetQuantity() ));
        if ( rqHuffSizeList[iK] != iSize )
        {
            if ( rqHuffSizeList[iK] == 0 )
                break;

            for (/**/; /**/; /**/)
            {
                iCode <<= 1;
                if ( rqHuffSizeList[iK] == ++iSize )
                    break;
            }
        }
    }

    return 0;
}
//------------------------------------------------------------------------------------------------------------------
void FormatJpeg::EncodeBlockHuffman (const StillArray<short>& rqBlock, int& riPrevDc,
    const StillArray<int>& rqDcHuffCodeOrderedList, const StillArray<int>& rqDcHuffSizeOrderedList,
    const StillArray<int>& rqAcHuffCodeOrderedList, const StillArray<int>& rqAcHuffSizeOrderedList,
    Stream& rqStream)
{
    int iVar0, iVar1;
    iVar0 = iVar1 = rqBlock[0] - riPrevDc;
    riPrevDc = rqBlock[0];
    if ( iVar0 < 0 )
    {
        iVar0 = -iVar0;
        iVar1--;
    }
    int iBitSize = 0;
    while ( iVar0 != 0 )
    {
        iBitSize++;
        iVar0 >>= 1;
    }
    rqStream.WriteBits(rqDcHuffCodeOrderedList[iBitSize],rqDcHuffSizeOrderedList[iBitSize]);
    if ( iBitSize != 0 )
        rqStream.WriteBits(iVar1,iBitSize);
    int iK, iJ = 0;
    for (int i = 1; i < 64; i++)
    {
        iVar0 = rqBlock[gs_aiInvZigZag[i]];
        if ( iVar0 == 0 )
        {
            iJ++;
        }
        else
        {
            while ( iJ > 15 )
            {
                rqStream.WriteBits(rqAcHuffCodeOrderedList[240],rqAcHuffSizeOrderedList[240]);
                iJ -= 16;
            }
            iVar1 = iVar0;
            if ( iVar0 < 0 )
            {
                iVar0 = -iVar0;
                iVar1--;
            }
            iBitSize = 1;
            iVar0 >>= 1;
            while ( iVar0 != 0 )
            {
                iBitSize++;
                iVar0 >>= 1;
            }
            iK = (iJ << 4) + iBitSize;
            rqStream.WriteBits(rqAcHuffCodeOrderedList[iK],rqAcHuffSizeOrderedList[iK]);
            rqStream.WriteBits(iVar1,iBitSize);
            iJ = 0;
        }
    }
    if ( iJ > 0 )
        rqStream.WriteBits(rqAcHuffCodeOrderedList[0],rqAcHuffSizeOrderedList[0]);
}
//------------------------------------------------------------------------------------------------------------------
void FormatJpeg::EncodeHuffmanBits (unsigned int* auiFrequencies, int iValue, int iBitSize, Stream* pqStream)
{
    if ( !auiFrequencies )
        pqStream->WriteBits(iValue,iBitSize);
}
//------------------------------------------------------------------------------------------------------------------
void FormatJpeg::EncodeHuffmanBuffBits (unsigned int* auiFrequencies, char* acBitBuff, int iBitSize,
    Stream* pqStream)
{
    if ( !auiFrequencies )
    {
        while ( iBitSize > 0 )
        {
            EncodeHuffmanBits(auiFrequencies,*acBitBuff,1,pqStream);
            acBitBuff++;
            iBitSize--;
        }
    }
}
//------------------------------------------------------------------------------------------------------------------
void FormatJpeg::EncodeHuffmanSymbol (unsigned int* auiFrequencies, int iSymbol, const int* aiHuffCodeOrderedList,
    const int* aiHuffSizeOrderedList, Stream* pqStream)
{
    if ( auiFrequencies )
        auiFrequencies[iSymbol]++;
    else
        EncodeHuffmanBits(auiFrequencies,aiHuffCodeOrderedList[iSymbol],aiHuffSizeOrderedList[iSymbol],pqStream);
}
//------------------------------------------------------------------------------------------------------------------
void FormatJpeg::EncodeHuffmanEobRun (unsigned int* auiFrequencies, int& riEobRun, const int* aiHuffCodeOrderedList,
    const int* aiHuffSizeOrderedList, char* acBitBuff, int& riBBQ, Stream* pqStream)
{
    if ( riEobRun > 0 )
    {
        int iVar = riEobRun;
        int iBitSize = 0;
        iVar >>= 1;
        while ( iVar != 0 )
        {
            iBitSize++;
            iVar >>= 1;
        }
        EncodeHuffmanSymbol(auiFrequencies,iBitSize << 4,aiHuffCodeOrderedList,aiHuffSizeOrderedList,pqStream);
        if ( iBitSize != 0 )
            EncodeHuffmanBits(auiFrequencies,riEobRun,iBitSize,pqStream);
        riEobRun = 0;
        EncodeHuffmanBuffBits(auiFrequencies,acBitBuff,riBBQ,pqStream);
        riBBQ = 0;
    }
}
//------------------------------------------------------------------------------------------------------------------
void FormatJpeg::EncodeDcInitial (unsigned int* auiFrequencies, const StillArray<short>& rqBlock, int& riPrevDc,
    const ScanProgressionParams& rqSPP, const int* aiHuffCodeOrderedList, const int* aiHuffSizeOrderedList,
    Stream* pqStream)
{
    int iVar1 = rqBlock[0] >> rqSPP.SuccApproxBitPosLow;
    int iVar0 = iVar1 - riPrevDc;
    riPrevDc = iVar1;
    iVar1 = iVar0;
    if ( iVar0 < 0 )
    {
        iVar0 = -iVar0;
        iVar1--;
    }
    int iBitSize = 0;
    while ( iVar0 != 0 )
    {
        iBitSize++;
        iVar0 >>= 1;
    }
    EncodeHuffmanSymbol(auiFrequencies,iBitSize,aiHuffCodeOrderedList,aiHuffSizeOrderedList,pqStream);
    if ( iBitSize != 0 )
        EncodeHuffmanBits(auiFrequencies,iVar1,iBitSize,pqStream);
}
//------------------------------------------------------------------------------------------------------------------
void FormatJpeg::EncodeDcSucceeding (const StillArray<short>& rqBlock, const ScanProgressionParams& rqSPP,
    Stream* pqStream)
{
    EncodeHuffmanBits(0,rqBlock[0] >> rqSPP.SuccApproxBitPosLow,1,pqStream);
}
//------------------------------------------------------------------------------------------------------------------
void FormatJpeg::EncodeAcInitial (unsigned int* auiFrequencies, const StillArray<short>& rqBlock,
    const ScanProgressionParams& rqSPP, int& riEobRun, const int* aiHuffCodeOrderedList,
    const int* aiHuffSizeOrderedList, char* acBitBuff, int& riBBQ, Stream* pqStream)
{
    int iVar0, iVar1, iBitSize, iJ = 0;
    for (int i = rqSPP.StartOfSpectral; i <= rqSPP.EndOfSpectral; i++)
    {
        iVar0 = rqBlock[gs_aiInvZigZag[i]];
        if ( iVar0 == 0 )
        {
            iJ++;
            continue;
        }
        if ( iVar0 < 0 )
        {
            iVar0 = -iVar0;
            iVar0 >>= rqSPP.SuccApproxBitPosLow;
            iVar1 = ~iVar0;
        }
        else
        {
            iVar0 >>= rqSPP.SuccApproxBitPosLow;
            iVar1 = iVar0;
        }
        if ( iVar0 == 0 )
        {
            iJ++;
            continue;
        }
        if ( riEobRun > 0 )
        {
            EncodeHuffmanEobRun(auiFrequencies,riEobRun,aiHuffCodeOrderedList,aiHuffSizeOrderedList,acBitBuff,riBBQ,
                pqStream);
        }
        while ( iJ > 15 )
        {
            EncodeHuffmanSymbol(auiFrequencies,240,aiHuffCodeOrderedList,aiHuffSizeOrderedList,pqStream);
            iJ -= 16;
        }
        iBitSize = 1;
        iVar0 >>= 1;
        while ( iVar0 != 0 )
        {
            iBitSize++;
            iVar0 >>= 1;
        }
        EncodeHuffmanSymbol(auiFrequencies,(iJ << 4)+iBitSize,aiHuffCodeOrderedList,aiHuffSizeOrderedList,pqStream);
        EncodeHuffmanBits(auiFrequencies,iVar1,iBitSize,pqStream);
        iJ = 0;
    }
    if ( iJ > 0 )
    {
        riEobRun++;
        if ( riEobRun == 32767 )
        {
            EncodeHuffmanEobRun(auiFrequencies,riEobRun,aiHuffCodeOrderedList,aiHuffSizeOrderedList,acBitBuff,riBBQ,
                pqStream);
        }
    }
}
//------------------------------------------------------------------------------------------------------------------
void FormatJpeg::EncodeAcSucceeding (unsigned int* auiFrequencies, const StillArray<short>& rqBlock,
    const ScanProgressionParams& rqSPP, int& riEobRun, const int* aiHuffCodeOrderedList,
    const int* aiHuffSizeOrderedList, char* acBitBuff, int& riBBQ, Stream* pqStream)
{
    int aiAbsVal[64];
    int iVar, iEob = 0;
    for (int i = rqSPP.StartOfSpectral; i <= rqSPP.EndOfSpectral; i++)
    {
        iVar = rqBlock[gs_aiInvZigZag[i]];
        if ( iVar < 0 )
            iVar = -iVar;
        iVar >>= rqSPP.SuccApproxBitPosLow;
        aiAbsVal[i] = iVar;
        if ( iVar == 1 )
            iEob = i;
    }
    int iJ = 0;
    int iBR = 0;
    char* pcBitBuff = acBitBuff + riBBQ;
    for (int i = rqSPP.StartOfSpectral; i <= rqSPP.EndOfSpectral; i++)
    {
        iVar = aiAbsVal[i];
        if ( iVar == 0 )
        {
            iJ++;
            continue;
        }
        while ( iJ > 15 && i <= iEob )
        {
            EncodeHuffmanEobRun(auiFrequencies,riEobRun,aiHuffCodeOrderedList,aiHuffSizeOrderedList,acBitBuff,riBBQ,
                pqStream);
            EncodeHuffmanSymbol(auiFrequencies,240,aiHuffCodeOrderedList,aiHuffSizeOrderedList,pqStream);
            iJ -= 16;
            EncodeHuffmanBuffBits(auiFrequencies,pcBitBuff,iBR,pqStream);
            pcBitBuff = acBitBuff;
            iBR = 0;
        }
        if ( iVar > 1 )
        {
            pcBitBuff[iBR++] = iVar & 1;
            continue;
        }
        EncodeHuffmanEobRun(auiFrequencies,riEobRun,aiHuffCodeOrderedList,aiHuffSizeOrderedList,acBitBuff,riBBQ,
            pqStream);
        EncodeHuffmanSymbol(auiFrequencies,(iJ << 4)+1,aiHuffCodeOrderedList,aiHuffSizeOrderedList,pqStream);
        iVar = ( rqBlock[gs_aiInvZigZag[i]] < 0 ? 0 : 1 );
        EncodeHuffmanBits(auiFrequencies,iVar,1,pqStream);
        EncodeHuffmanBuffBits(auiFrequencies,pcBitBuff,iBR,pqStream);
        pcBitBuff = acBitBuff;
        iBR = 0;
        iJ = 0;
    }
    if ( iJ > 0 || iBR > 0 )
    {
        riEobRun++;
        riBBQ += iBR;
        if ( riEobRun == 32767 || riBBQ > ms_iBitBuffSize - 63 )
        {
            EncodeHuffmanEobRun(auiFrequencies,riEobRun,aiHuffCodeOrderedList,aiHuffSizeOrderedList,acBitBuff,riBBQ,
                pqStream);
        }
    }
}
//------------------------------------------------------------------------------------------------------------------
bool FormatJpeg::LoadHeadingData (int iBytestreamSize)
{
    // defaults
    m_eUpsamplingFilter = UFT_BILINEAR;
    m_bHasJfifMarker = false;
    m_bHasAdobeMarker = false;
    m_bHasExifMetadata = false;
    m_bImageInitialized = false;
    m_iDevelopedRowQuantity = 0;  // regardless dynamic or not
    m_iDevelopedRowQuantityUB = 0;

    assert( iBytestreamSize == -1 || iBytestreamSize >= 0 );

    bool bSuccess;

    // Signature.  The first stable bytes in JPEG format are:
    unsigned char aucSignature[3] = {255, 216, 255};
    int iDataSize;
    if ( m_eStreaming == Stream::ST_READING_FROM_FILE )
        CONDITIONAL_THROW_EXCEPTION(Storage::GetFileSize(iDataSize,m_qFilename),ET_CANNOT_ACCESS_FILE)
    else
        iDataSize = iBytestreamSize;
    if ( iDataSize < 3 )
    {
        if ( m_eStreaming != Stream::ST_READING_FROM_DYNAMIC_BYTESTREAM )
            THROW_EXCEPTION_WITHOUT_ASSERT_FALSE(ET_FORMAT_IDENTIFIER_MISMATCH)
        else
            return false;
    }
    Stream qSignatureStream(m_eStreaming,m_acStreamSrcParam,0,3,false,bSuccess,iBytestreamSize);
    PROCESS_STREAM_SUCCESS_FOR_LOADING(bSuccess);
    char acSignature[3];
    qSignatureStream.Read(3,acSignature);
    qSignatureStream.Finalize();
    CONDITIONAL_THROW_EXCEPTION_WITHOUT_ASSERT_FALSE(( MemCmp(acSignature,aucSignature,3) == 0 ),
        ET_FORMAT_IDENTIFIER_MISMATCH);

    unsigned char ucAdobeColorTransformation;

    // find the first frame
    Stream qHeaderFrameStream(m_eStreaming,m_acStreamSrcParam,2,iDataSize-2,false,bSuccess,iBytestreamSize);
    PROCESS_STREAM_SUCCESS_FOR_LOADING(bSuccess);
    bool bFrameFound = false;
    unsigned char ucByte;
    unsigned short usMarkerSegmentLength;
    for (/**/; /**/; /**/)
    {
        RETURN_ON_FAIL(qHeaderFrameStream.Read(ucByte));
        if ( ucByte == 255 )
        {
            do
            {
                RETURN_ON_FAIL(qHeaderFrameStream.Read(ucByte));
            } while ( ucByte == 255 );

            if ( 192 <= ucByte && ucByte <= 194 )
            {
                switch ( ucByte )
                {
                case 192:
                    m_eProcess = PT_BASELINE_DCT;
                    m_iFrameMarkerSecondByte = 192;
                    break;
                case 193:
                    m_eProcess = PT_EXTENDED_DCT;
                    m_iFrameMarkerSecondByte = 193;
                    break;
                case 194:
                    m_eProcess = PT_PROGRESSIVE_DCT;
                    m_iFrameMarkerSecondByte = 194;
                    break;
                }

                bFrameFound = true;
                break;
            }
            else if ( ucByte == 195 )
            {
                THROW_EXCEPTION_WITHOUT_ASSERT_FALSE(ET_LOSSLESS_COMPRESSION_NOT_SUPPORTED);
            }
            else if ( (197 <= ucByte && ucByte <= 199) || (205 <= ucByte && ucByte <= 207) )
            {
                THROW_EXCEPTION_WITHOUT_ASSERT_FALSE(ET_HIERARCHICAL_COMPRESSION_NOT_SUPPORTED);
            }
            else if ( 200 <= ucByte && ucByte <= 203 )
            {
                THROW_EXCEPTION_WITHOUT_ASSERT_FALSE(ET_ARITHMETIC_CODING_NOT_SUPPORTED);
            }
            else if ( ucByte != 1 )  // no stand-alone markers must appear before the first frame except TEM
            {
                // that's a marker with a body
                RETURN_ON_FAIL(qHeaderFrameStream.Read(usMarkerSegmentLength));
                if ( ucByte == 224 )
                {
                    // JFIF marker
                    char acJfifSignature[5];
                    RETURN_ON_FAIL(qHeaderFrameStream.Read(5,acJfifSignature));
                    unsigned char aucJfifSignature[5] = {74, 70, 73, 70, 0};
                    if ( MemCmp(acJfifSignature,aucJfifSignature,5) != 0 )
                    {
                        qHeaderFrameStream.OffsetBytePosition(-5);
                        goto SkipThisMarker;
                    }
                    unsigned char ucMajorRev, ucMinorRev;
                    RETURN_ON_FAIL(qHeaderFrameStream.Read(ucMajorRev));
                    RETURN_ON_FAIL(qHeaderFrameStream.Read(ucMinorRev));
                    CONDITIONAL_THROW_EXCEPTION_WITHOUT_ASSERT_FALSE(
                        ( ucMajorRev <= 1 && !(ucMajorRev == 1 && ucMinorRev > 2) ),ET_JFIF_VERSION_NOT_SUPPORTED);
                    unsigned char ucUnits;
                    RETURN_ON_FAIL(qHeaderFrameStream.Read(ucUnits));
                    switch ( ucUnits )
                    {
                    case 0:
                        m_eJfifDotDensityMeaning = JFIFPDMT_ASPECT_RATIO;
                        break;
                    case 1:
                        m_eJfifDotDensityMeaning = JFIFPDMT_PIXELS_PER_INCH;
                        break;
                    case 2:
                        m_eJfifDotDensityMeaning = JFIFPDMT_PIXELS_PER_CENTIMETER;
                        break;
                    default:
                        THROW_WARNING(WT_JFIF_MARKER_IS_INCORRECT);
                        qHeaderFrameStream.OffsetBytePosition(-8);
                        goto SkipThisMarker;
                    }
                    unsigned short usPDH, usPDV;
                    RETURN_ON_FAIL(qHeaderFrameStream.Read(usPDH));
                    RETURN_ON_FAIL(qHeaderFrameStream.Read(usPDV));
                    m_iJfifDotDensityHorizontally = usPDH;
                    m_iJfifDotDensityVertically = usPDV;
                    m_bHasJfifMarker = true;

                    qHeaderFrameStream.OffsetBytePosition(-12);
                }
                else if ( ucByte == 238 )
                {
                    // Adobe marker
                    char acAdobeSignature[5];
                    RETURN_ON_FAIL(qHeaderFrameStream.Read(5,acAdobeSignature));
                    unsigned char aucAdobeSignature[5] = {65, 100, 111, 98, 101};
                    if ( MemCmp(acAdobeSignature,aucAdobeSignature,5) != 0 )
                    {
                        qHeaderFrameStream.OffsetBytePosition(-5);
                        goto SkipThisMarker;
                    }
                    qHeaderFrameStream.OffsetBytePosition(6);
                    m_bHasAdobeMarker = true;
                    RETURN_ON_FAIL(qHeaderFrameStream.Read(ucAdobeColorTransformation));

                    qHeaderFrameStream.OffsetBytePosition(-12);
                }
                else if ( ucByte == 225 )
                {
                    // APP1 marker, can be Exif metadata
                    char acExifSignature[6];
                    RETURN_ON_FAIL(qHeaderFrameStream.Read(6,acExifSignature));
                    unsigned char aucExifSignature[6] = {69, 120, 105, 102, 0, 0};
                    if ( MemCmp(acExifSignature,aucExifSignature,6) != 0 )
                    {
                        qHeaderFrameStream.OffsetBytePosition(-6);
                        goto SkipThisMarker;
                    }

                    int iSize = qHeaderFrameStream.GetByteQuantity() - qHeaderFrameStream.GetBytePosition();
                    if ( !m_spqExifMetadata )
                    {
                        m_spqExifMetadata = new FormatExif(
                            ( m_eStreaming != Stream::ST_READING_FROM_DYNAMIC_BYTESTREAM ),
                            qHeaderFrameStream.GetBytesUpPosition(),iSize);
                    }
                    bool bExifSuccess;
                    if ( m_eStreaming != Stream::ST_READING_FROM_DYNAMIC_BYTESTREAM )
                        bExifSuccess = m_spqExifMetadata->LoadData();
                    else
                        bExifSuccess = m_spqExifMetadata->LoadData(iSize);
                    if ( bExifSuccess )
                    {
                        m_bHasExifMetadata = true;
                    }
                    else
                    {
                        if ( !m_spqExifMetadata->GetExceptionFlag() )
                        {
                            // have to be dynamic bytestream
                            return false;
                        }

                        // not distinguishing Exif exception type
                        THROW_WARNING(WT_EXIF_METADATA_ARE_INCORRECT);

                        m_spqExifMetadata = 0;
                    }

                    qHeaderFrameStream.OffsetBytePosition(-6);
                }

SkipThisMarker:
                qHeaderFrameStream.OffsetBytePosition(usMarkerSegmentLength-2);
            }
        }
    }
    if ( !bFrameFound )
    {
        if ( m_eStreaming != Stream::ST_READING_FROM_DYNAMIC_BYTESTREAM )
            THROW_EXCEPTION_WITHOUT_ASSERT_FALSE(ET_FORMAT_MISMATCH)
        else
            return false;
    }

    // Read frame's header.
    unsigned short usSOFMarkerSegmentLength;
    RETURN_ON_FAIL(qHeaderFrameStream.Read(usSOFMarkerSegmentLength));

    unsigned char ucSamplePrecision;
    RETURN_ON_FAIL(qHeaderFrameStream.Read(ucSamplePrecision));
    CONDITIONAL_THROW_EXCEPTION((
         !(m_eProcess == PT_BASELINE_DCT && ucSamplePrecision != 8)
    &&   !(m_eProcess == PT_EXTENDED_DCT && ucSamplePrecision != 8 && ucSamplePrecision != 12)
    &&   !(m_eProcess == PT_PROGRESSIVE_DCT && ucSamplePrecision != 8 && ucSamplePrecision != 12) ),
        ET_FORMAT_MISMATCH);
    CONDITIONAL_THROW_EXCEPTION_WITHOUT_ASSERT_FALSE(( ucSamplePrecision == 8 ),
        ET_NON_8_BPS_PRECISION_NOT_SUPPORTED);
    m_iSamplePrecision = ucSamplePrecision;

    unsigned short usComponentMaxHeight, usComponentMaxWidth;
    RETURN_ON_FAIL(qHeaderFrameStream.Read(usComponentMaxHeight));
    RETURN_ON_FAIL(qHeaderFrameStream.Read(usComponentMaxWidth));

    CONDITIONAL_THROW_EXCEPTION_WITHOUT_ASSERT_FALSE(( usComponentMaxHeight != 0 ),ET_DNL_MARKER_NOT_SUPPORTED);
    CONDITIONAL_THROW_EXCEPTION(( usComponentMaxWidth != 0 ),ET_FORMAT_MISMATCH);

    unsigned char ucComponentQuantity;
    RETURN_ON_FAIL(qHeaderFrameStream.Read(ucComponentQuantity));
    CONDITIONAL_THROW_EXCEPTION(( ucComponentQuantity != 0
    &&   !(m_eProcess == PT_PROGRESSIVE_DCT && ucComponentQuantity > 4)
    &&   usSOFMarkerSegmentLength == 8 + 3*ucComponentQuantity ),ET_FORMAT_MISMATCH);

    // read component-specification parameters
    m_qFrameCompSpecParamsArray.SetQuantity(ucComponentQuantity);
    int iMinSamplingFactorV;
    for (int i = 0; i < ucComponentQuantity; i++)
    {
        FrameCompSpecParams& rqFCSP = m_qFrameCompSpecParamsArray[i];

        RETURN_ON_FAIL(qHeaderFrameStream.Read(rqFCSP.ComponentIdentifier));
        RETURN_ON_FAIL(qHeaderFrameStream.ReadUBits(rqFCSP.SamplingFactorH,4));
        RETURN_ON_FAIL(qHeaderFrameStream.ReadUBits(rqFCSP.SamplingFactorV,4));
        RETURN_ON_FAIL(qHeaderFrameStream.Read(rqFCSP.QuantTableDestSelector));

        CONDITIONAL_THROW_EXCEPTION((
             1 <= rqFCSP.SamplingFactorH && rqFCSP.SamplingFactorH <= 4
        &&   1 <= rqFCSP.SamplingFactorV && rqFCSP.SamplingFactorV <= 4
        &&   rqFCSP.QuantTableDestSelector <= 3 ),ET_FORMAT_MISMATCH);

        if ( i == 0 )
        {
            m_iMaxSamplingFactorH = rqFCSP.SamplingFactorH;
            m_iMaxSamplingFactorV = rqFCSP.SamplingFactorV;

            if ( m_eStreaming == Stream::ST_READING_FROM_DYNAMIC_BYTESTREAM )
                iMinSamplingFactorV = rqFCSP.SamplingFactorV;
        }
        else
        {
            if ( rqFCSP.SamplingFactorH > m_iMaxSamplingFactorH )
                m_iMaxSamplingFactorH = rqFCSP.SamplingFactorH;
            if ( rqFCSP.SamplingFactorV > m_iMaxSamplingFactorV )
                m_iMaxSamplingFactorV = rqFCSP.SamplingFactorV;

            if ( m_eStreaming == Stream::ST_READING_FROM_DYNAMIC_BYTESTREAM
            &&   rqFCSP.SamplingFactorV < iMinSamplingFactorV )
            {
                iMinSamplingFactorV = rqFCSP.SamplingFactorV;
            }
        }
    }
    if ( m_eStreaming == Stream::ST_READING_FROM_DYNAMIC_BYTESTREAM && m_eProcess != PT_PROGRESSIVE_DCT )
    {
        if ( iMinSamplingFactorV != m_iMaxSamplingFactorV )
        {
            m_iDynamicSamplingCausedRowMargine = (int)Mathf::Ceil(((float)m_iMaxSamplingFactorV)/
                iMinSamplingFactorV);
        }
        else
        {
            m_iDynamicSamplingCausedRowMargine = 0;
        }
    }

    if ( ucComponentQuantity != 2 && ucComponentQuantity <= 4 )
    {
        // guess the used color model
        if ( ucComponentQuantity == 1 )
        {
            // always grayscale
            m_eColorModel = CMT_GRAYSCALE;
        }
        else if ( ucComponentQuantity == 3 )
        {
            if ( m_bHasJfifMarker )
            {
                // has JFIF marker, always YCbCr
                m_eColorModel = CMT_YCC;
            }
            else if ( m_bHasAdobeMarker )
            {
                // has Adobe marker
                switch ( ucAdobeColorTransformation )
                {
                case 0:
                    m_eColorModel = CMT_RGB;
                    break;
                case 1:
                    m_eColorModel = CMT_YCC;
                    break;
                default:
                    m_eColorModel = CMT_YCC;
                    break;
                }
            }
            else
            {
                if ( m_qFrameCompSpecParamsArray[0].ComponentIdentifier == 82
                &&   m_qFrameCompSpecParamsArray[1].ComponentIdentifier == 71
                &&   m_qFrameCompSpecParamsArray[2].ComponentIdentifier == 66 )
                {
                    // 'R', 'G', 'B'
                    m_eColorModel = CMT_RGB;
                }
                else
                {
                    // let be YCbCr
                    m_eColorModel = CMT_YCC;
                }
            }
        }
        else  // ucComponentQuantity == 4
        {
            if ( m_bHasAdobeMarker )
            {
                // has Adobe marker
                switch ( ucAdobeColorTransformation )
                {
                case 0:
                    m_eColorModel = CMT_CMYK;
                    break;
                case 2:
                    m_eColorModel = CMT_YCCK;
                    break;
                default:
                    m_eColorModel = CMT_YCCK;
                    break;
                }
            }
            else
            {
                // let be CMYK
                m_eColorModel = CMT_CMYK;
            }
        }

        m_iBitsPerPixel = ucComponentQuantity*ucSamplePrecision;
    }
    else
    {
        THROW_EXCEPTION_WITHOUT_ASSERT_FALSE(ET_COMPONENT_QUANTITY_NOT_SUPPORTED);
    }
    m_iComponentQuantity = ucComponentQuantity;

    qHeaderFrameStream.Finalize();

    m_iWidth = usComponentMaxWidth;
    m_iHeight = usComponentMaxHeight;

    m_qComponentImages.SetQuantity(m_iComponentQuantity);
    m_bRestartIntervalsEnabled = false;
    m_bScanIsFirst = true;
    m_iDecodedComponentQuantity = 0;
    if ( m_eStreaming == Stream::ST_READING_FROM_DYNAMIC_BYTESTREAM )
    {
        m_iDynamicBytestreamResumeBytePosition = 0;
        m_eResumeZone = IZT_TABLES_AND_MISC;
        if ( m_eProcess == PT_PROGRESSIVE_DCT )
            m_asBlockPtr = 0;
    }

    return true;
}
//------------------------------------------------------------------------------------------------------------------
int FormatJpeg::ReachMarkerThroughTablesAndMisc (unsigned char ucMarker2ndByte, Stream& rqStream)
{
    unsigned char ucByte;
    for (/**/; /**/; /**/)
    {
        RETURN_NEG_ONE_ON_FAIL(rqStream.Read(ucByte));
        if ( ucByte == 255 )
        {
            do
            {
                RETURN_NEG_ONE_ON_FAIL(rqStream.Read(ucByte));
            } while ( ucByte == 255 );

            if ( ucByte == ucMarker2ndByte )
            {
                break;
            }
            else if ( ucByte == 219 )
            {
                // DQT marker
                int iAfterMarkerOffset = rqStream.GetBytePosition();

                unsigned short usDQTMarkerSegmentLength;
                RETURN_NEG_ONE_ON_FAIL(rqStream.Read(usDQTMarkerSegmentLength));

                int iElementPrecision, iDestId;
                for (/**/; /**/; /**/)
                {
                    RETURN_NEG_ONE_ON_FAIL(rqStream.ReadUBits(iElementPrecision,4));
                    RETURN_NEG_ONE_ON_FAIL(rqStream.ReadUBits(iDestId,4));

                    RETURN_POS_ONE_ON_FAIL((
                         !(m_eProcess == PT_BASELINE_DCT && iElementPrecision != 0)
                    &&   !(m_eProcess == PT_EXTENDED_DCT && iElementPrecision > 1)
                    &&   !(m_eProcess == PT_PROGRESSIVE_DCT && iElementPrecision > 1)
                    &&   iDestId <= 3 ));

                    if ( iElementPrecision == 0 )
                    {
                        // 1-byte entries
                        unsigned char ucValue;
                        for (int i = 0; i < 64; i++)
                        {
                            RETURN_NEG_ONE_ON_FAIL(rqStream.Read(ucValue));
                            RETURN_POS_ONE_ON_FAIL(( ucValue != 0 ));
                            m_aaiQuantizationTableDestination[iDestId][gs_aiInvZigZag[i]] = ucValue;
                        }
                    }
                    else  // iElementPrecision = 1
                    {
                        // 2-byte entries
                        unsigned short usValue;
                        for (int i = 0; i < 64; i++)
                        {
                            RETURN_NEG_ONE_ON_FAIL(rqStream.Read(usValue));
                            RETURN_POS_ONE_ON_FAIL(( usValue != 0 ));
                            m_aaiQuantizationTableDestination[iDestId][gs_aiInvZigZag[i]] = usValue;
                        }
                    }

                    if ( rqStream.GetBytePosition() - iAfterMarkerOffset == usDQTMarkerSegmentLength )
                        break;
                }
            }
            else if ( ucByte == 196 )
            {
                // DHT marker
                int iAfterMarkerOffset = rqStream.GetBytePosition();

                unsigned short usDHTMarkerSegmentLength;
                RETURN_NEG_ONE_ON_FAIL(rqStream.Read(usDHTMarkerSegmentLength));

                int iClass, iDestId;
                for (/**/; /**/; /**/)
                {
                    RETURN_NEG_ONE_ON_FAIL(rqStream.ReadUBits(iClass,4));
                    RETURN_NEG_ONE_ON_FAIL(rqStream.ReadUBits(iDestId,4));

                    RETURN_POS_ONE_ON_FAIL((
                         iClass <= 1
                    &&   !(m_eProcess == PT_BASELINE_DCT && iDestId > 1)
                    &&   !(m_eProcess != PT_BASELINE_DCT && iDestId > 3) ));

                    HuffmanTableSet& rqHuffmanTableSet = m_aaqHuffmanTableDestination[iClass][iDestId];
                    unsigned char ucValue;
                    int iSum = 0;
                    for (int i = 1; i < 17; i++)
                    {
                        RETURN_NEG_ONE_ON_FAIL(rqStream.Read(ucValue));
                        rqHuffmanTableSet.BitsList[i] = ucValue;
                        iSum += ucValue;
                    }

                    rqHuffmanTableSet.HuffValList.SetQuantity(iSum);
                    for (int i = 0; i < iSum; i++)
                    {
                        RETURN_NEG_ONE_ON_FAIL(rqStream.Read(ucValue));
                        rqHuffmanTableSet.HuffValList[i] = ucValue;
                    }

                    // decode Huffman table itself
                    StillArray<int> qHuffCodeList, qHuffSizeList;
                    int iSuccess = GetHuffmanDerivedTables(qHuffCodeList,qHuffSizeList,rqHuffmanTableSet);
                    PROCESS_INT_SUCCESS_LEVEL_LOW(iSuccess);

                    // "decoder tables"
                    int iI = 0;
                    int iJ = 0;
                    for (/**/; /**/; /**/)
                    {
                        if ( ++iI > 16 )
                            break;

                        if ( rqHuffmanTableSet.BitsList[iI] == 0 )
                        {
                            rqHuffmanTableSet.MaxCode[iI] = -1;
                        }
                        else
                        {
                            rqHuffmanTableSet.ValueOffset[iI] = iJ;
                            RETURN_POS_ONE_ON_FAIL(( iJ < qHuffCodeList.GetQuantity() ));
                            rqHuffmanTableSet.MinCode[iI] = qHuffCodeList[iJ];
                            iJ += rqHuffmanTableSet.BitsList[iI] - 1;
                            RETURN_POS_ONE_ON_FAIL(( 0 <= iJ && iJ < qHuffCodeList.GetQuantity() ));
                            rqHuffmanTableSet.MaxCode[iI] = qHuffCodeList[iJ];
                            iJ++;
                        }
                    }

                    if ( rqStream.GetBytePosition() - iAfterMarkerOffset == usDHTMarkerSegmentLength )
                        break;
                }
            }
            else if ( ucByte == 221 )
            {
                // DRI marker
                unsigned short usDRIMarkerSegmentLength;
                RETURN_NEG_ONE_ON_FAIL(rqStream.Read(usDRIMarkerSegmentLength));
                RETURN_POS_ONE_ON_FAIL(( usDRIMarkerSegmentLength == 4 ));
                unsigned short usRestartInterval;
                RETURN_NEG_ONE_ON_FAIL(rqStream.Read(usRestartInterval));

                if ( usRestartInterval != 0 )
                {
                    m_bRestartIntervalsEnabled = true;
                    m_iRestartInterval = usRestartInterval;
                }
                else
                {
                    m_bRestartIntervalsEnabled = false;
                }
            }
            else if ( ucByte == 216 )
            {
                // SOI stand-alone marker.  Only in relation to restart intervals.
                m_bRestartIntervalsEnabled = false;
            }
            else if ( ucByte != 1 )  // no more stand-alone markers must appear except TEM
            {
                // that's a marker with a body
                unsigned short usMarkerSegmentLength;
                RETURN_NEG_ONE_ON_FAIL(rqStream.Read(usMarkerSegmentLength));

                rqStream.OffsetBytePosition(usMarkerSegmentLength-2);
            }
        }
    }
    return 0;
}
//------------------------------------------------------------------------------------------------------------------
void FormatJpeg::CompileImage (RasterImagePtr& rspqImage)
{
    if ( m_eProcess == PT_PROGRESSIVE_DCT )
    {
        // progressive
        int iLTX, iLTY, iLTXP8, iLTYP8, iI, iCStart, iCEnd;
        char* acRow;
        if ( m_eStreaming != Stream::ST_READING_FROM_DYNAMIC_BYTESTREAM
        ||   (m_eStreaming == Stream::ST_READING_FROM_DYNAMIC_BYTESTREAM && m_ucInScanComponentQuantity != 1) )
        {
            iCStart = 0;
            iCEnd = m_iComponentQuantity;
        }
        else
        {
            iCStart = m_qScanCompSpecParamsArray[0].ComponentIndexInFrame;
            iCEnd = iCStart + 1;
        }
        for (int i0 = iCStart; i0 < iCEnd; i0++)
        {
            const FrameCompSpecParams& rqFCSP = m_qFrameCompSpecParamsArray[i0];
            StillArray<StillArray<short> >& rqCPB = m_qComponentBuffers[i0];
            const RasterImagePtr& rspqComponentImage = m_qComponentImages[i0];

            iLTX = 0;
            iLTY = 0;
            for (int i1 = 0; i1 < rqCPB.GetQuantity(); i1++)
            {
                iLTXP8 = iLTX + 8;
                iLTYP8 = iLTY + 8;
                if ( !(m_eStreaming == Stream::ST_READING_FROM_DYNAMIC_BYTESTREAM
                &&   (iLTY < m_iDYPRangeTop || iLTYP8 > m_iDYPRangeBottom))
                &&   iLTX < rspqComponentImage->GetWidth()
                &&   iLTY < rspqComponentImage->GetHeight() )
                {
                    const StillArray<short>& rqBlock = rqCPB[i1];

                    for (int i2 = 0; i2 < 64; i2++)
                        m_asBlockArray[i2] = rqBlock[i2];

                    // inverse DCT with parallel dequantizing and clamping
                    DiscreteCosineTransformi::Get8x8InverseOrthogonal2dWithDequantizationAndClamping(m_acBlock,
                        m_asBlockArray,m_aaiQuantizationTableDestination[rqFCSP.QuantTableDestSelector]);

                    // perform the shifting
                    for (int i2 = 0; i2 < 64; i2++)
                        m_asBlockArray[i2] = m_acBlock[i2] + 128;

                    iI = 0;
                    for (int iY = iLTY; iY < iLTYP8; iY++)
                    {
                        if ( iY < rspqComponentImage->GetHeight() )
                        {
                            acRow = rspqComponentImage->GetRow(iY);
                            for (int iX = iLTX; iX < iLTXP8; iX++)
                            {
                                if ( iX < rspqComponentImage->GetWidth() )
                                    *(acRow + iX) = (char)m_asBlockArray[iI];
                                iI++;
                            }
                        }
                        else
                        {
                            iI += 8;
                        }
                    }
                }

                iLTX += 8;
                if ( iLTX/8 == rqFCSP.ProgressiveBufferWidthInDataUnits )
                {
                    iLTX = 0;
                    iLTY += 8;
                }
            }
        }
    }

    if ( m_eColorModel == CMT_GRAYSCALE )
    {
        rspqImage = m_qComponentImages[0]->GetDuplicate();
    }
    else
    {
        // upsample if needed
        StillArray<RasterImagePtr> qComponentImages(m_iComponentQuantity);
        for (int i = 0; i < m_iComponentQuantity; i++)
        {
            const RasterImagePtr& rspqComponent = m_qComponentImages[i];

            if ( rspqComponent->GetWidth() != m_iWidth || rspqComponent->GetHeight() != m_iHeight )
            {
                RasterImageTransformation qRIT(rspqComponent);
                qComponentImages[i] = qRIT.GetResampled(m_iWidth,m_iHeight,( m_eUpsamplingFilter == UFT_BILINEAR ?
                    RasterImageTransformation::IT_BILINEAR : RasterImageTransformation::IT_NEAREST_NEIGHBOR ));
            }
            else
            {
                qComponentImages[i] = rspqComponent;
            }
        }

        // compile the final image
        int iTop, iBottom;
        if ( m_eStreaming != Stream::ST_READING_FROM_DYNAMIC_BYTESTREAM )
        {
            iTop = 0;
            iBottom = m_iHeight;
        }
        else
        {
            if ( m_eProcess != PT_PROGRESSIVE_DCT )
            {
                iTop = m_iDevelopedRowQuantityUB - m_iMcuRowHeight - m_iDynamicSamplingCausedRowMargine;
                if ( iTop < 0 )
                    iTop = 0;
            }
            else
            {
                iTop = m_iDYPRangeTop;
            }
            iBottom = m_iDevelopedRowQuantity;
        }
        unsigned char* pucRow = (unsigned char*)rspqImage->GetImage() + iTop*rspqImage->GetRowStride();
        unsigned char* pucComp = pucRow;
        ColorRgb qColorRgb;
        if ( m_eColorModel == CMT_YCC )
        {
            unsigned char* aucRow0 = (unsigned char*)qComponentImages[0]->GetImage() +
                iTop*qComponentImages[0]->GetRowStride();
            unsigned char* pucComp0 = aucRow0;
            unsigned char* aucRow1 = (unsigned char*)qComponentImages[1]->GetImage() +
                iTop*qComponentImages[1]->GetRowStride();
            unsigned char* pucComp1 = aucRow1;
            unsigned char* aucRow2 = (unsigned char*)qComponentImages[2]->GetImage() +
                iTop*qComponentImages[2]->GetRowStride();
            unsigned char* pucComp2 = aucRow2;

            int iR, iG, iB;
            for (int iY = iTop; iY < iBottom; iY++)
            {
                for (int iX = 0; iX < m_iWidth; iX++)
                {
                    iR = *pucComp0 + m_aiCrInR[*pucComp2];
                    iG = *pucComp0 + m_aiCbInG[*pucComp1] + m_aiCrInG[*pucComp2];
                    iB = *pucComp0 + m_aiCbInB[*pucComp1];
                    if ( iR < 0 )
                        iR = 0;
                    else if ( iR > 255 )
                        iR = 255;
                    if ( iG < 0 )
                        iG = 0;
                    else if ( iG > 255 )
                        iG = 255;
                    if ( iB < 0 )
                        iB = 0;
                    else if ( iB > 255 )
                        iB = 255;

                    *pucComp++ = iR;
                    *pucComp++ = iG;
                    *pucComp++ = iB;

                    pucComp0++;
                    pucComp1++;
                    pucComp2++;
                }

                pucRow += rspqImage->GetRowStride();
                pucComp = pucRow;

                aucRow0 += qComponentImages[0]->GetRowStride();
                pucComp0 = aucRow0;
                aucRow1 += qComponentImages[1]->GetRowStride();
                pucComp1 = aucRow1;
                aucRow2 += qComponentImages[2]->GetRowStride();
                pucComp2 = aucRow2;
            }
        }
        else if ( m_eColorModel == CMT_RGB )
        {
            unsigned char* aucRow0 = (unsigned char*)qComponentImages[0]->GetImage() +
                iTop*qComponentImages[0]->GetRowStride();
            unsigned char* pucComp0 = aucRow0;
            unsigned char* aucRow1 = (unsigned char*)qComponentImages[1]->GetImage() +
                iTop*qComponentImages[1]->GetRowStride();
            unsigned char* pucComp1 = aucRow1;
            unsigned char* aucRow2 = (unsigned char*)qComponentImages[2]->GetImage() +
                iTop*qComponentImages[2]->GetRowStride();
            unsigned char* pucComp2 = aucRow2;

            for (int iY = iTop; iY < iBottom; iY++)
            {
                for (int iX = 0; iX < m_iWidth; iX++)
                {
                    *pucComp++ = *pucComp0++;
                    *pucComp++ = *pucComp1++;
                    *pucComp++ = *pucComp2++;
                }

                pucRow += rspqImage->GetRowStride();
                pucComp = pucRow;

                aucRow0 += qComponentImages[0]->GetRowStride();
                pucComp0 = aucRow0;
                aucRow1 += qComponentImages[1]->GetRowStride();
                pucComp1 = aucRow1;
                aucRow2 += qComponentImages[2]->GetRowStride();
                pucComp2 = aucRow2;
            }
        }
        else if ( m_eColorModel == CMT_YCCK )
        {
            unsigned char* aucRow0 = (unsigned char*)qComponentImages[0]->GetImage() +
                iTop*qComponentImages[0]->GetRowStride();
            unsigned char* pucComp0 = aucRow0;
            unsigned char* aucRow1 = (unsigned char*)qComponentImages[1]->GetImage() +
                iTop*qComponentImages[1]->GetRowStride();
            unsigned char* pucComp1 = aucRow1;
            unsigned char* aucRow2 = (unsigned char*)qComponentImages[2]->GetImage() +
                iTop*qComponentImages[2]->GetRowStride();
            unsigned char* pucComp2 = aucRow2;
            unsigned char* aucRow3 = (unsigned char*)qComponentImages[3]->GetImage() +
                iTop*qComponentImages[3]->GetRowStride();
            unsigned char* pucComp3 = aucRow3;

            int iC, iM, iY;
            for (int i0 = iTop; i0 < iBottom; i0++)
            {
                for (int i1 = 0; i1 < m_iWidth; i1++)
                {
                    iC = *pucComp0 + m_aiCrInR[*pucComp2];
                    iM = *pucComp0 + m_aiCbInG[*pucComp1] + m_aiCrInG[*pucComp2];
                    iY = *pucComp0 + m_aiCbInB[*pucComp1];
                    if ( iC < 0 )
                        iC = 0;
                    else if ( iC > 255 )
                        iC = 255;
                    if ( iM < 0 )
                        iM = 0;
                    else if ( iM > 255 )
                        iM = 255;
                    if ( iY < 0 )
                        iY = 0;
                    else if ( iY > 255 )
                        iY = 255;

                    *pucComp++ = iC;
                    *pucComp++ = iM;
                    *pucComp++ = iY;
                    *pucComp++ = 255 - *pucComp3;

                    pucComp0++;
                    pucComp1++;
                    pucComp2++;
                    pucComp3++;
                }

                pucRow += rspqImage->GetRowStride();
                pucComp = pucRow;

                aucRow0 += qComponentImages[0]->GetRowStride();
                pucComp0 = aucRow0;
                aucRow1 += qComponentImages[1]->GetRowStride();
                pucComp1 = aucRow1;
                aucRow2 += qComponentImages[2]->GetRowStride();
                pucComp2 = aucRow2;
                aucRow3 += qComponentImages[3]->GetRowStride();
                pucComp3 = aucRow3;
            }
        }
        else  // m_eColorModel = CMT_CMYK
        {
            unsigned char* aucRow0 = (unsigned char*)qComponentImages[0]->GetImage() +
                iTop*qComponentImages[0]->GetRowStride();
            unsigned char* pucComp0 = aucRow0;
            unsigned char* aucRow1 = (unsigned char*)qComponentImages[1]->GetImage() +
                iTop*qComponentImages[1]->GetRowStride();
            unsigned char* pucComp1 = aucRow1;
            unsigned char* aucRow2 = (unsigned char*)qComponentImages[2]->GetImage() +
                iTop*qComponentImages[2]->GetRowStride();
            unsigned char* pucComp2 = aucRow2;
            unsigned char* aucRow3 = (unsigned char*)qComponentImages[3]->GetImage() +
                iTop*qComponentImages[3]->GetRowStride();
            unsigned char* pucComp3 = aucRow3;

            for (int iY = iTop; iY < iBottom; iY++)
            {
                for (int iX = 0; iX < m_iWidth; iX++)
                {
                    *pucComp++ = 255 - *pucComp0++;
                    *pucComp++ = 255 - *pucComp1++;
                    *pucComp++ = 255 - *pucComp2++;
                    *pucComp++ = 255 - *pucComp3++;
                }

                pucRow += rspqImage->GetRowStride();
                pucComp = pucRow;

                aucRow0 += qComponentImages[0]->GetRowStride();
                pucComp0 = aucRow0;
                aucRow1 += qComponentImages[1]->GetRowStride();
                pucComp1 = aucRow1;
                aucRow2 += qComponentImages[2]->GetRowStride();
                pucComp2 = aucRow2;
                aucRow3 += qComponentImages[3]->GetRowStride();
                pucComp3 = aucRow3;
            }
        }
    }
}
//------------------------------------------------------------------------------------------------------------------
int FormatJpeg::DoNextBitProc (int& riBit, unsigned char& rucB, int& riCnt, Stream& rqStream)
{
    if ( riCnt == 0 )
    {
        RETURN_NEG_ONE_ON_FAIL(rqStream.Read(rucB));
        riCnt = 8;
        if ( rucB == 255 )
        {
            unsigned char ucB2;
            RETURN_NEG_ONE_ON_FAIL(rqStream.Read(ucB2));
            if ( ucB2 != 0 )
            {
                rqStream.OffsetBytePosition(-2);
                return 1;
            }
        }
    }
    riBit = rucB >> 7;
    riCnt--;
    rucB <<= 1;
    return 0;
}
//------------------------------------------------------------------------------------------------------------------
int FormatJpeg::DoHuffmanDecodeProc (int& riValue, unsigned char& rucB, int& riCnt, const HuffmanTableSet& rqHTS,
    Stream& rqStream)
{
    int iSuccess, iBit, iI = 1;
    iSuccess = DoNextBitProc(iBit,rucB,riCnt,rqStream);
    PROCESS_INT_SUCCESS_LEVEL_LOW(iSuccess);
    int iCode = iBit;
    for (/**/; /**/; /**/)
    {
        RETURN_POS_ONE_ON_FAIL(( iI != 17 ));
        if ( iCode <= rqHTS.MaxCode[iI] )
            break;

        iI++;
        iSuccess = DoNextBitProc(iBit,rucB,riCnt,rqStream);
        PROCESS_INT_SUCCESS_LEVEL_LOW(iSuccess);
        iCode = (iCode << 1) + iBit;
    }
    int iJ = rqHTS.ValueOffset[iI];
    iJ += iCode - rqHTS.MinCode[iI];
    RETURN_POS_ONE_ON_FAIL(( iJ < rqHTS.HuffValList.GetQuantity() ));
    riValue = rqHTS.HuffValList[iJ];
    return 0;
}
//------------------------------------------------------------------------------------------------------------------
int FormatJpeg::DoReceiveProc (int& riV, int iSsss, unsigned char& rucB, int& riCnt, Stream& rqStream)
{
    int iI, iSuccess, iBit;
    riV = iI = 0;
    for (/**/; /**/; /**/)
    {
        if ( iI == iSsss )
            break;

        iI++;
        iSuccess = DoNextBitProc(iBit,rucB,riCnt,rqStream);
        PROCESS_INT_SUCCESS_LEVEL_LOW(iSuccess);
        riV = (riV << 1) + iBit;
    }
    return 0;
}
//------------------------------------------------------------------------------------------------------------------



