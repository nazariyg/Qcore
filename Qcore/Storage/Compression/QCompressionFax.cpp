#include "QCompressionFax.h"
#include "QCompressionFax.mcr"
#include "QStorage.h"
#include "QProgressProbe.h"
using namespace Q;

// Support for Modified Huffman decoding.  Each code record is: {the code in decimal form, code length,
// run-length}.
static const int gs_iModifiedHuffmanCodeQuantity(104);
static const short gs_aasCodeRecordWhite[gs_iModifiedHuffmanCodeQuantity][3] = {
    { 53, 8,   0},{  7, 6,   1},{  7, 4,   2},{  8, 4,   3},{ 11, 4,   4},{ 12, 4,   5},{ 14, 4,   6},{ 15, 4,   7},
    { 19, 5,   8},{ 20, 5,   9},{  7, 5,  10},{  8, 5,  11},{  8, 6,  12},{  3, 6,  13},{ 52, 6,  14},{ 53, 6,  15},
    { 42, 6,  16},{ 43, 6,  17},{ 39, 7,  18},{ 12, 7,  19},{  8, 7,  20},{ 23, 7,  21},{  3, 7,  22},{  4, 7,  23},
    { 40, 7,  24},{ 43, 7,  25},{ 19, 7,  26},{ 36, 7,  27},{ 24, 7,  28},{  2, 8,  29},{  3, 8,  30},{ 26, 8,  31},
    { 27, 8,  32},{ 18, 8,  33},{ 19, 8,  34},{ 20, 8,  35},{ 21, 8,  36},{ 22, 8,  37},{ 23, 8,  38},{ 40, 8,  39},
    { 41, 8,  40},{ 42, 8,  41},{ 43, 8,  42},{ 44, 8,  43},{ 45, 8,  44},{  4, 8,  45},{  5, 8,  46},{ 10, 8,  47},
    { 11, 8,  48},{ 82, 8,  49},{ 83, 8,  50},{ 84, 8,  51},{ 85, 8,  52},{ 36, 8,  53},{ 37, 8,  54},{ 88, 8,  55},
    { 89, 8,  56},{ 90, 8,  57},{ 91, 8,  58},{ 74, 8,  59},{ 75, 8,  60},{ 50, 8,  61},{ 51, 8,  62},{ 52, 8,  63},
    { 27, 5,  64},{ 18, 5, 128},{ 23, 6, 192},{ 55, 7, 256},{ 54, 8, 320},{ 55, 8, 384},{100, 8, 448},{101, 8, 512},
    {104, 8, 576},{103, 8, 640},{204, 9, 704},{205, 9, 768},{210, 9, 832},{211, 9, 896},{212, 9, 960},{213, 9,1024},
    {214, 9,1088},{215, 9,1152},{216, 9,1216},{217, 9,1280},{218, 9,1344},{219, 9,1408},{152, 9,1472},{153, 9,1536},
    {154, 9,1600},{ 24, 6,1664},{155, 9,1728},{  8,11,1792},{ 12,11,1856},{ 13,11,1920},{ 18,12,1984},{ 19,12,2048},
    { 20,12,2112},{ 21,12,2176},{ 22,12,2240},{ 23,12,2304},{ 28,12,2368},{ 29,12,2432},{ 30,12,2496},{ 31,12,2560}
};
static const short gs_aasCodeRecordBlack[gs_iModifiedHuffmanCodeQuantity][3] = {
    { 55,10,   0},{  2, 3,   1},{  3, 2,   2},{  2, 2,   3},{  3, 3,   4},{  3, 4,   5},{  2, 4,   6},{  3, 5,   7},
    {  5, 6,   8},{  4, 6,   9},{  4, 7,  10},{  5, 7,  11},{  7, 7,  12},{  4, 8,  13},{  7, 8,  14},{ 24, 9,  15},
    { 23,10,  16},{ 24,10,  17},{  8,10,  18},{103,11,  19},{104,11,  20},{108,11,  21},{ 55,11,  22},{ 40,11,  23},
    { 23,11,  24},{ 24,11,  25},{202,12,  26},{203,12,  27},{204,12,  28},{205,12,  29},{104,12,  30},{105,12,  31},
    {106,12,  32},{107,12,  33},{210,12,  34},{211,12,  35},{212,12,  36},{213,12,  37},{214,12,  38},{215,12,  39},
    {108,12,  40},{109,12,  41},{218,12,  42},{219,12,  43},{ 84,12,  44},{ 85,12,  45},{ 86,12,  46},{ 87,12,  47},
    {100,12,  48},{101,12,  49},{ 82,12,  50},{ 83,12,  51},{ 36,12,  52},{ 55,12,  53},{ 56,12,  54},{ 39,12,  55},
    { 40,12,  56},{ 88,12,  57},{ 89,12,  58},{ 43,12,  59},{ 44,12,  60},{ 90,12,  61},{102,12,  62},{103,12,  63},
    { 15,10,  64},{200,12, 128},{201,12, 192},{ 91,12, 256},{ 51,12, 320},{ 52,12, 384},{ 53,12, 448},{108,13, 512},
    {109,13, 576},{ 74,13, 640},{ 75,13, 704},{ 76,13, 768},{ 77,13, 832},{114,13, 896},{115,13, 960},{116,13,1024},
    {117,13,1088},{118,13,1152},{119,13,1216},{ 82,13,1280},{ 83,13,1344},{ 84,13,1408},{ 85,13,1472},{ 90,13,1536},
    { 91,13,1600},{100,13,1664},{101,13,1728},{  8,11,1792},{ 12,11,1856},{ 13,11,1920},{ 18,12,1984},{ 19,12,2048},
    { 20,12,2112},{ 21,12,2176},{ 22,12,2240},{ 23,12,2304},{ 28,12,2368},{ 29,12,2432},{ 30,12,2496},{ 31,12,2560}
};

// for packing; indicates how much the buffer is to grow depending on amount of previously allocated memory
const int CompressionFax::ms_iAdvanceQuantityDivider(3);  // on 1/3 each time

// support for 2-dimensional encoding/decoding
class ChangingElementRecord
{
public:
    int Position, Color;
};

//------------------------------------------------------------------------------------------------------------------
CompressionFax::CompressionFax (EncodingType eEncoding, bool bForPacking)
    :
    m_eEncoding(eEncoding),
    m_bForPacking(bForPacking),
    m_iK(4),
    m_eUncompressedModeEncoding(UMET_CCITT)
{
    if ( !bForPacking )
    {
        // generate the Huffman's tree: it's possible since there is no code which is prefix of another
        m_apqRootNode[0] = new BspNode<int>();
        m_apqRootNode[1] = new BspNode<int>();
        BspNode<int>* pqNode;
        const short* asCodeRecord;
        int iCode, iCodeLength, iRunLength;
        for (int i0 = 0; i0 < 2; i0++)
        {
            for (int i1 = 0; i1 < gs_iModifiedHuffmanCodeQuantity; i1++)
            {
                asCodeRecord = ( i0 == 0 ? gs_aasCodeRecordWhite[i1] : gs_aasCodeRecordBlack[i1] );
                iCode = asCodeRecord[0];
                iCodeLength = asCodeRecord[1];
                iRunLength = asCodeRecord[2];

                pqNode = m_apqRootNode[i0];
                for (int i2 = iCodeLength-1; i2 >= 0; i2--)
                {
                    if ( ((iCode >> i2) & 1) == 0 )
                    {
                        if ( pqNode->GetNegativeChild() == 0 )
                            pqNode->AttachNegativeChild(new BspNode<int>(-1,pqNode));
                        pqNode = pqNode->GetNegativeChild();
                    }
                    else
                    {
                        if ( pqNode->GetPositiveChild() == 0 )
                            pqNode->AttachPositiveChild(new BspNode<int>(-1,pqNode));
                        pqNode = pqNode->GetPositiveChild();
                    }
                }

                pqNode->GetElement() = iRunLength;
            }
        }
        if ( eEncoding == ET_GROUP_3_2D )
        {
            // add the uncompressed mode entrance code to the tree
            iCode = 15;
            iCodeLength = 12;
            iRunLength = 2561;

            for (int i0 = 0; i0 < 2; i0++)
            {
                pqNode = m_apqRootNode[i0];
                for (int i1 = iCodeLength-1; i1 >= 0; i1--)
                {
                    if ( ((iCode >> i1) & 1) == 0 )
                    {
                        if ( pqNode->GetNegativeChild() == 0 )
                            pqNode->AttachNegativeChild(new BspNode<int>(-1,pqNode));
                        pqNode = pqNode->GetNegativeChild();
                    }
                    else
                    {
                        if ( pqNode->GetPositiveChild() == 0 )
                            pqNode->AttachPositiveChild(new BspNode<int>(-1,pqNode));
                        pqNode = pqNode->GetPositiveChild();
                    }
                }

                pqNode->GetElement() = iRunLength;
            }
        }
    }
}
//------------------------------------------------------------------------------------------------------------------
CompressionFax::~CompressionFax ()
{
    if ( !m_bForPacking )
    {
        // free the Huffman tree's memory emulating stack
        BspNode<int>* pqLast;
        for (int i = 0; i < 2; i++)
        {
            Array<BspNode<int>*> qNodeStack;
            qNodeStack.Push(m_apqRootNode[i]);
            while ( !qNodeStack.IsEmpty() )
            {
                pqLast = qNodeStack.GetLast();
                if ( pqLast->GetNegativeChild() )
                {
                    qNodeStack.Push(pqLast->GetNegativeChild());
                }
                else if ( pqLast->GetPositiveChild() )
                {
                    qNodeStack.Push(pqLast->GetPositiveChild());
                }
                else
                {
                    if ( pqLast->GetParentNode() )
                    {
                        // set child's pointer to 0
                        if ( pqLast == pqLast->GetParentNode()->GetNegativeChild() )
                            pqLast->GetParentNode()->DetachNegativeChild();
                        else
                            pqLast->GetParentNode()->DetachPositiveChild();
                    }
                    delete pqLast;
                    qNodeStack.Pop();
                }
            }
        }
    }
}
//------------------------------------------------------------------------------------------------------------------
void CompressionFax::Compress (const char* acByte, StillArray<char>& rqNewBytes)
{
    rqNewBytes.RemoveAll();

    bool bDecImgRowsAreBytes = ( (m_iWidth & 7) == 0 );
    int iDecImgRowSizeInBytes = ( bDecImgRowsAreBytes ? m_iWidth/8 : m_iWidth/8 + 1 );

    // for a progress bar
    ProgressProbe qProgress(this);
    qProgress.SetAdvance(1.0f/m_iHeight);

    int iBytePos0 = 0;
    int iBitRatio0 = 0;
    int iBytePos1 = 0;
    int iBitRatio1 = 0;
    if ( m_eEncoding == ET_GROUP_3_1D || m_eEncoding == ET_GROUP_3_1D_TIFF )
    {
        if ( m_eEncoding == ET_GROUP_3_1D )
        {
            // initial EOL
            WriteBits(1,12,iBytePos1,iBitRatio1,rqNewBytes);
        }

        for (int iY = 0; iY < m_iHeight; iY++)
        {
            EncodeRow1d(iBytePos0,iBitRatio0,iBytePos1,iBitRatio1,acByte,rqNewBytes);

            if ( iBitRatio0 != 0 )
            {
                iBytePos0++;
                iBitRatio0 = 0;
            }

            if ( m_eEncoding == ET_GROUP_3_1D )
                WriteBits(1,12,iBytePos1,iBitRatio1,rqNewBytes);
            else  // m_eEncoding = ET_GROUP_3_1D_TIFF
                AlignBytes(iBytePos1,iBitRatio1,rqNewBytes);

            qProgress.MakeAdvance();
        }

        if ( m_eEncoding == ET_GROUP_3_1D )
        {
            // RTC
            for (int i = 0; i < 6; i++)
                WriteBits(1,12,iBytePos1,iBitRatio1,rqNewBytes);
        }
    }
    else  // m_eEncoding = ET_GROUP_3_2D or m_eEncoding = ET_GROUP_4
    {
        int iRowsRead;
        if ( m_eEncoding == ET_GROUP_3_2D )
        {
            // initial "EOL+1"
            WriteBits(3,13,iBytePos1,iBitRatio1,rqNewBytes);

            // initial 1-dimensional line
            EncodeRow1d(iBytePos0,iBitRatio0,iBytePos1,iBitRatio1,acByte,rqNewBytes);
            if ( iBitRatio0 != 0 )
            {
                iBytePos0++;
                iBitRatio0 = 0;
            }
            iRowsRead = 1;

            qProgress.MakeAdvance();
        }
        else  // m_eEncoding = ET_GROUP_4
        {
            iRowsRead = 0;
        }

        const char* acCodingLine;
        const char* acReferenceLine;
        bool bWhiteRun;
        int iRightHandPos, iPrevColor, iCESBytePos, iCESBitRatio, iValue, iVDiff, iBitsLeft, iSubRunLength;
        int aiRL[2];
        ChangingElementRecord qA0, qA1, qA2, qB1, qB2;
        int iKMO = m_iK - 1;
        StillArray<char> qG4RefRow;
        if ( m_eEncoding == ET_GROUP_4 )
        {
            qG4RefRow.SetQuantity(iDecImgRowSizeInBytes);
            MemSet(qG4RefRow,0,iDecImgRowSizeInBytes);
        }
        for (/**/; /**/; /**/)
        {
            if ( iRowsRead == m_iHeight )
                goto EncodingExit;

            for (int i0 = 0; i0 < iKMO || m_eEncoding == ET_GROUP_4; i0++)
            {
                if ( m_eEncoding == ET_GROUP_3_2D )
                    WriteBits(2,13,iBytePos1,iBitRatio1,rqNewBytes);

                bWhiteRun = true;
                qA0.Position = -1;

                acCodingLine = acByte + iRowsRead*iDecImgRowSizeInBytes;
                acReferenceLine =
                    ( iRowsRead != 0 ? acByte + (iRowsRead - 1)*iDecImgRowSizeInBytes : qG4RefRow );
                for (/**/; /**/; /**/)
                {
                    qA0.Color = ( bWhiteRun ? 0 : 1 );

                    // update changing element a1
                    iRightHandPos = qA0.Position + 1;
                    iCESBytePos = iRightHandPos/8;
                    iCESBitRatio = iRightHandPos & 7;
                    qA1.Position = m_iWidth;
                    for (int i1 = iRightHandPos; i1 < m_iWidth; i1++)
                    {
                        iCESBytePos += Storage::ReadUBits(acCodingLine+iCESBytePos,iCESBitRatio,1,iValue);
                        if ( iValue != qA0.Color )
                        {
                            qA1.Position = i1;
                            qA1.Color = iValue;
                            break;
                        }
                    }

                    // update changing element a2
                    iRightHandPos = qA1.Position + 1;
                    iCESBytePos = iRightHandPos/8;
                    iCESBitRatio = iRightHandPos & 7;
                    qA2.Position = m_iWidth;
                    for (int i1 = iRightHandPos; i1 < m_iWidth; i1++)
                    {
                        iCESBytePos += Storage::ReadUBits(acCodingLine+iCESBytePos,iCESBitRatio,1,iValue);
                        if ( iValue == qA0.Color )
                        {
                            qA2.Position = i1;
                            qA2.Color = iValue;
                            break;
                        }
                    }

                    // update changing element b1
                    if ( qA0.Position == -1 )
                    {
                        iPrevColor = 0;
                    }
                    else
                    {
                        iCESBytePos = qA0.Position/8;
                        iCESBitRatio = qA0.Position & 7;
                        Storage::ReadUBits(acReferenceLine+iCESBytePos,iCESBitRatio,1,iPrevColor);
                    }
                    iRightHandPos = qA0.Position + 1;
                    iCESBytePos = iRightHandPos/8;
                    iCESBitRatio = iRightHandPos & 7;
                    qB1.Position = m_iWidth;
                    for (int i1 = iRightHandPos; i1 < m_iWidth; i1++)
                    {
                        iCESBytePos += Storage::ReadUBits(acReferenceLine+iCESBytePos,iCESBitRatio,1,iValue);
                        if ( iValue != iPrevColor && iValue != qA0.Color )
                        {
                            qB1.Position = i1;
                            qB1.Color = iValue;
                            break;
                        }
                        else
                        {
                            iPrevColor = iValue;
                        }
                    }

                    // update changing element b2
                    iRightHandPos = qB1.Position + 1;
                    iCESBytePos = iRightHandPos/8;
                    iCESBitRatio = iRightHandPos & 7;
                    qB2.Position = m_iWidth;
                    for (int i1 = iRightHandPos; i1 < m_iWidth; i1++)
                    {
                        iCESBytePos += Storage::ReadUBits(acReferenceLine+iCESBytePos,iCESBitRatio,1,iValue);
                        if ( iValue == qA0.Color )
                        {
                            qB2.Position = i1;
                            qB2.Color = iValue;
                            break;
                        }
                    }

                    if ( qA0.Position == -1 )
                    {
                        // simplifies a lot of things
                        qA0.Position = 0;
                    }

                    if ( qB2.Position < qA1.Position )
                    {
                        // pass mode
                        WriteBits(1,4,iBytePos1,iBitRatio1,rqNewBytes);
                        qA0.Position = qB2.Position;
                    }
                    else
                    {
                        iVDiff = qA1.Position - qB1.Position;
                        if ( Mathi::Abs(iVDiff) <= 3 )
                        {
                            // vertical mode
                            switch ( iVDiff )
                            {
                            case 0:
                                WriteBits(1,1,iBytePos1,iBitRatio1,rqNewBytes);
                                break;
                            case -1:
                                WriteBits(2,3,iBytePos1,iBitRatio1,rqNewBytes);
                                break;
                            case 1:
                                WriteBits(3,3,iBytePos1,iBitRatio1,rqNewBytes);
                                break;
                            case -2:
                                WriteBits(2,6,iBytePos1,iBitRatio1,rqNewBytes);
                                break;
                            case 2:
                                WriteBits(3,6,iBytePos1,iBitRatio1,rqNewBytes);
                                break;
                            case -3:
                                WriteBits(2,7,iBytePos1,iBitRatio1,rqNewBytes);
                                break;
                            case 3:
                                WriteBits(3,7,iBytePos1,iBitRatio1,rqNewBytes);
                                break;
                            }
                            if ( qA1.Position == m_iWidth )
                                break;
                            qA0.Position = qA1.Position;
                            bWhiteRun = !bWhiteRun;
                        }
                        else
                        {
                            // horizontal mode
                            WriteBits(1,3,iBytePos1,iBitRatio1,rqNewBytes);
                            aiRL[0] = qA1.Position - qA0.Position;
                            aiRL[1] = qA2.Position - qA1.Position;
                            for (int i1 = 0; i1 < 2; i1++)
                            {
                                iBitsLeft = aiRL[i1];
                                for (/**/; /**/; /**/)
                                {
                                    if ( iBitsLeft < 64 )
                                    {
                                        WriteHuffmanCode(iBitsLeft,bWhiteRun,iBytePos1,iBitRatio1,rqNewBytes);
                                        break;
                                    }
                                    else
                                    {
                                        iSubRunLength = (iBitsLeft/64)*64;
                                        if ( iSubRunLength > 2560 )
                                            iSubRunLength = 2560;
                                        WriteHuffmanCode(iSubRunLength,bWhiteRun,iBytePos1,iBitRatio1,rqNewBytes);
                                        iBitsLeft -= iSubRunLength;
                                    }
                                }
                                bWhiteRun = !bWhiteRun;
                            }
                            if ( qA2.Position == m_iWidth )
                                break;
                            qA0.Position = qA2.Position;
                        }
                    }
                }

                qProgress.MakeAdvance();

                if ( ++iRowsRead == m_iHeight )
                    goto EncodingExit;
            }

            iBytePos0 = iRowsRead*iDecImgRowSizeInBytes;

            // Kth 1-dimensional line
            WriteBits(3,13,iBytePos1,iBitRatio1,rqNewBytes);
            EncodeRow1d(iBytePos0,iBitRatio0,iBytePos1,iBitRatio1,acByte,rqNewBytes);
            if ( iBitRatio0 != 0 )
            {
                iBytePos0++;
                iBitRatio0 = 0;
            }
            iRowsRead++;

            qProgress.MakeAdvance();
        }

EncodingExit:

        if ( m_eEncoding == ET_GROUP_3_2D )
        {
            // RTC
            for (int i = 0; i < 6; i++)
                WriteBits(3,13,iBytePos1,iBitRatio1,rqNewBytes);
        }
    }

    // cut off unused trailing bytes
    if ( iBitRatio1 != 0 )
        rqNewBytes.SetQuantity(iBytePos1+1);
    else
        rqNewBytes.SetQuantity(iBytePos1);
}
//------------------------------------------------------------------------------------------------------------------
bool CompressionFax::Decompress (const char* acByte, int iByteQuantity, StillArray<char>& rqNewBytes)
{
    rqNewBytes.RemoveAll();

    // the output bitmap will be byte-aligned
    bool bDecImgRowsAreBytes = ( (m_iWidth & 7) == 0 );
    int iDecImgRowSizeInBytes = ( bDecImgRowsAreBytes ? m_iWidth/8 : m_iWidth/8 + 1 );
    rqNewBytes.SetQuantity(m_iHeight*iDecImgRowSizeInBytes);

    // for a progress bar
    ProgressProbe qProgress(this);
    qProgress.SetAdvance(1.0f/m_iHeight);

    int iValue, iRunLength;
    int iBytePos0 = 0;
    int iBytePos1 = 0;
    int iBitRatio0 = 0;
    int iBitRatio1 = 0;
    int iRowPixelsRead = 0;
    int iRowsRead = 0;
    bool bWhiteRun = true;
    if ( m_eEncoding == ET_GROUP_3_1D || m_eEncoding == ET_GROUP_3_1D_TIFF )
    {
        // Modified Huffman encoding
        if ( m_eEncoding == ET_GROUP_3_1D )
        {
            // go behind the initial EOL code (zero fill-bits may appear before)
            for (/**/; /**/; /**/)
            {
                GET_NEXT_BIT(iValue,iBytePos0,iBitRatio0,acByte,iByteQuantity);
                if ( iValue == 1 )
                    break;
            }
        }

        for (/**/; /**/; /**/)
        {
            // navigate the tree in accordance with the code's bits
            if ( !ReadHuffmanCode(iRunLength,bWhiteRun,iBytePos0,iBitRatio0,acByte,iByteQuantity) )
                return false;

            // got the run length, so write appropriate bits
            if ( !WritePixels(iRunLength,bWhiteRun,iRowPixelsRead,iBytePos1,iBitRatio1,rqNewBytes) )
                return false;

            if ( iRunLength < 64 )
            {
                // terminating code
                if ( iRowPixelsRead != m_iWidth )
                {
                    bWhiteRun = !bWhiteRun;
                }
                else
                {
                    // have finished the row
                    qProgress.MakeAdvance();

                    if ( ++iRowsRead == m_iHeight )
                        break;

                    if ( m_eEncoding == ET_GROUP_3_1D )
                    {
                        // rely on possibility of zero fill-bits before an EOL (or RTC) code
                        for (/**/; /**/; /**/)
                        {
                            GET_NEXT_BIT(iValue,iBytePos0,iBitRatio0,acByte,iByteQuantity);
                            if ( iValue == 1 )
                                break;
                        }
                    }

                    iRowPixelsRead = 0;
                    bWhiteRun = true;

                    if ( m_eEncoding == ET_GROUP_3_1D_TIFF )
                    {
                        // compressed data alignment
                        if ( iBitRatio0 != 0 )
                        {
                            iBytePos0++;
                            iBitRatio0 = 0;
                        }
                    }

                    // uncompressed data alignment
                    if ( !bDecImgRowsAreBytes )
                    {
                        iBytePos1++;
                        iBitRatio1 = 0;
                    }
                }
            }
        }
    }
    else  // m_eEncoding = ET_GROUP_3_2D or m_eEncoding = ET_GROUP_4
    {
        if ( m_eEncoding == ET_GROUP_3_2D )
        {
            // go behind the initial "EOL+1" code (zero fill-bits may appear before)
            for (/**/; /**/; /**/)
            {
                GET_NEXT_BIT(iValue,iBytePos0,iBitRatio0,acByte,iByteQuantity);
                if ( iValue == 1 )
                    break;
            }
            GET_NEXT_BIT(iValue,iBytePos0,iBitRatio0,acByte,iByteQuantity);
            if ( iValue != 1 )
            {
                // initial "EOL+1" must indicate 1-dimensional coded line
                assert( false );
                return false;
            }
        }

        int iRightHandPos, iPrevColor, iCESBytePos, iCESBitRatio, iCodeLength, iZeroes0, iOnes0, iZeroes1, iZeroes;
        char* acCodingLine;
        char* acReferenceLine;
        ChangingElementRecord qA0, qB1, qB2;
        int iBeforeA0AccumRL = 0;
        int iRunQuantity = 0;
        qA0.Position = -1;
        bool b1dMode = ( m_eEncoding == ET_GROUP_3_2D );
        bool bUncompressedMode = false;
        StillArray<char> qG4RefRow;
        if ( m_eEncoding == ET_GROUP_4 )
        {
            qG4RefRow.SetQuantity(iDecImgRowSizeInBytes);
            MemSet(qG4RefRow,0,iDecImgRowSizeInBytes);
        }
        for (/**/; /**/; /**/)
        {
            if ( !bUncompressedMode )
            {
                if ( b1dMode )
                {
                    // 1-dimensional sub-mode
                    for (/**/; /**/; /**/)
                    {
                        if ( !ReadHuffmanCode(iRunLength,bWhiteRun,iBytePos0,iBitRatio0,acByte,iByteQuantity) )
                            return false;

                        // got the run length, so write appropriate bits
                        if ( !WritePixels(iRunLength,bWhiteRun,iRowPixelsRead,iBytePos1,iBitRatio1,rqNewBytes) )
                            return false;

                        if ( iRunLength == 2561 )
                        {
                            // entrance into uncompressed mode
                            bUncompressedMode = true;
                            break;
                        }

                        if ( iRunLength < 64 )
                        {
                            // terminating code
                            bWhiteRun = !bWhiteRun;
                            break;
                        }
                    }
                }
                else
                {
                    // 2-dimensional sub-mode
                    acCodingLine = rqNewBytes + iRowsRead*iDecImgRowSizeInBytes;
                    acReferenceLine = ( !(m_eEncoding == ET_GROUP_4 && iRowsRead == 0) ?
                        rqNewBytes + (iRowsRead - 1)*iDecImgRowSizeInBytes : qG4RefRow );

                    qA0.Color = ( bWhiteRun ? 0 : 1 );

                    // update changing element b1
                    if ( qA0.Position == -1 )
                    {
                        iPrevColor = 0;
                    }
                    else
                    {
                        iCESBytePos = qA0.Position/8;
                        iCESBitRatio = qA0.Position & 7;
                        Storage::ReadUBits(acReferenceLine+iCESBytePos,iCESBitRatio,1,iPrevColor);
                    }
                    iRightHandPos = qA0.Position + 1;
                    iCESBytePos = iRightHandPos/8;
                    iCESBitRatio = iRightHandPos & 7;
                    qB1.Position = m_iWidth;
                    for (int i = iRightHandPos; i < m_iWidth; i++)
                    {
                        iCESBytePos += Storage::ReadUBits(acReferenceLine+iCESBytePos,iCESBitRatio,1,iValue);
                        if ( iValue != iPrevColor && iValue != qA0.Color )
                        {
                            qB1.Position = i;
                            qB1.Color = iValue;
                            break;
                        }
                        else
                        {
                            iPrevColor = iValue;
                        }
                    }

                    // update changing element b2
                    iRightHandPos = qB1.Position + 1;
                    iCESBytePos = iRightHandPos/8;
                    iCESBitRatio = iRightHandPos & 7;
                    qB2.Position = m_iWidth;
                    for (int i = iRightHandPos; i < m_iWidth; i++)
                    {
                        iCESBytePos += Storage::ReadUBits(acReferenceLine+iCESBytePos,iCESBitRatio,1,iValue);
                        if ( iValue == qA0.Color )
                        {
                            qB2.Position = i;
                            qB2.Color = iValue;
                            break;
                        }
                    }

                    if ( qA0.Position == -1 )
                    {
                        // simplifies a lot of things
                        qA0.Position = 0;
                    }

                    // read 2D code
                    iCodeLength = iZeroes0 = iOnes0 = iZeroes1 = 0;
                    for (/**/; /**/; /**/)
                    {
                        // the maximal code length for 2D-encoding is 10
                        if ( ++iCodeLength > 10 )
                        {
                            assert( false );
                            return false;
                        }

                        GET_NEXT_BIT(iValue,iBytePos0,iBitRatio0,acByte,iByteQuantity);
                        if ( iValue == 0 )
                        {
                            if ( iOnes0 == 0 )
                                iZeroes0++;
                            else
                                iZeroes1++;
                        }
                        else  // iValue = 1
                        {
                            if ( iZeroes1 != 0 )
                            {
                                assert( false );
                                return false;
                            }
                            iOnes0++;
                        }

                        // identify the code
                        if ( iZeroes0 == 3 && iOnes0 == 1 && iZeroes1 == 0 )
                        {
                            // pass
                            iBeforeA0AccumRL += qB2.Position - qA0.Position;
                            qA0.Position = qB2.Position;
                            break;
                        }
                        else if ( iZeroes0 == 0 && iOnes0 == 1 && iZeroes1 == 0 )
                        {
                            // V0
                            iRunLength = qB1.Position - qA0.Position;
                            if ( !WritePixels(iBeforeA0AccumRL+iRunLength,bWhiteRun,iRowPixelsRead,iBytePos1,
                                 iBitRatio1,rqNewBytes) )
                            {
                                return false;
                            }
                            bWhiteRun = !bWhiteRun;
                            iBeforeA0AccumRL = 0;
                            qA0.Position += iRunLength;
                            iRunQuantity++;
                            break;
                        }
                        else if ( iZeroes0 == 1 && iOnes0 == 1 && iZeroes1 == 1 )
                        {
                            // VL1
                            iRunLength = qB1.Position - qA0.Position - 1;
                            if ( !WritePixels(iBeforeA0AccumRL+iRunLength,bWhiteRun,iRowPixelsRead,iBytePos1,
                                 iBitRatio1,rqNewBytes) )
                            {
                                return false;
                            }
                            bWhiteRun = !bWhiteRun;
                            iBeforeA0AccumRL = 0;
                            qA0.Position += iRunLength;
                            iRunQuantity++;
                            break;
                        }
                        else if ( iZeroes0 == 1 && iOnes0 == 2 && iZeroes1 == 0 )
                        {
                            // VR1
                            iRunLength = qB1.Position - qA0.Position + 1;
                            if ( !WritePixels(iBeforeA0AccumRL+iRunLength,bWhiteRun,iRowPixelsRead,iBytePos1,
                                 iBitRatio1,rqNewBytes) )
                            {
                                return false;
                            }
                            bWhiteRun = !bWhiteRun;
                            iBeforeA0AccumRL = 0;
                            qA0.Position += iRunLength;
                            iRunQuantity++;
                            break;
                        }
                        else if ( iZeroes0 == 4 && iOnes0 == 1 && iZeroes1 == 1 )
                        {
                            // VL2
                            iRunLength = qB1.Position - qA0.Position - 2;
                            if ( !WritePixels(iBeforeA0AccumRL+iRunLength,bWhiteRun,iRowPixelsRead,iBytePos1,
                                 iBitRatio1,rqNewBytes) )
                            {
                                return false;
                            }
                            bWhiteRun = !bWhiteRun;
                            iBeforeA0AccumRL = 0;
                            qA0.Position += iRunLength;
                            iRunQuantity++;
                            break;
                        }
                        else if ( iZeroes0 == 4 && iOnes0 == 2 && iZeroes1 == 0 )
                        {
                            // VR2
                            iRunLength = qB1.Position - qA0.Position + 2;
                            if ( !WritePixels(iBeforeA0AccumRL+iRunLength,bWhiteRun,iRowPixelsRead,iBytePos1,
                                 iBitRatio1,rqNewBytes) )
                            {
                                return false;
                            }
                            bWhiteRun = !bWhiteRun;
                            iBeforeA0AccumRL = 0;
                            qA0.Position += iRunLength;
                            iRunQuantity++;
                            break;
                        }
                        else if ( iZeroes0 == 5 && iOnes0 == 1 && iZeroes1 == 1 )
                        {
                            // VL3
                            iRunLength = qB1.Position - qA0.Position - 3;
                            if ( !WritePixels(iBeforeA0AccumRL+iRunLength,bWhiteRun,iRowPixelsRead,iBytePos1,
                                 iBitRatio1,rqNewBytes) )
                            {
                                return false;
                            }
                            bWhiteRun = !bWhiteRun;
                            iBeforeA0AccumRL = 0;
                            qA0.Position += iRunLength;
                            iRunQuantity++;
                            break;
                        }
                        else if ( iZeroes0 == 5 && iOnes0 == 2 && iZeroes1 == 0 )
                        {
                            // VR3
                            iRunLength = qB1.Position - qA0.Position + 3;
                            if ( !WritePixels(iBeforeA0AccumRL+iRunLength,bWhiteRun,iRowPixelsRead,iBytePos1,
                                 iBitRatio1,rqNewBytes) )
                            {
                                return false;
                            }
                            bWhiteRun = !bWhiteRun;
                            iBeforeA0AccumRL = 0;
                            qA0.Position += iRunLength;
                            iRunQuantity++;
                            break;
                        }
                        else if ( iZeroes0 == 2 && iOnes0 == 1 && iZeroes1 == 0 )
                        {
                            // horizontal mode's prefix
                            if ( (iRunQuantity & 1) == 0 )
                            {
                                for (/**/; /**/; /**/)
                                {
                                    // navigate the tree in accordance with the code's bits
                                    if ( !ReadHuffmanCode(iRunLength,true,iBytePos0,iBitRatio0,acByte,
                                         iByteQuantity) )
                                    {
                                        return false;
                                    }

                                    if ( iRunLength >= 64 )
                                    {
                                        // make-up code
                                        iBeforeA0AccumRL += iRunLength;
                                        qA0.Position += iRunLength;
                                    }
                                    else
                                    {
                                        // terminating code
                                        if ( !WritePixels(iBeforeA0AccumRL+iRunLength,true,iRowPixelsRead,
                                             iBytePos1,iBitRatio1,rqNewBytes) )
                                        {
                                            return false;
                                        }
                                        iBeforeA0AccumRL = 0;
                                        qA0.Position += iRunLength;
                                        iRunQuantity++;
                                        break;
                                    }
                                }
                                for (/**/; /**/; /**/)
                                {
                                    // navigate the tree in accordance with the code's bits
                                    if ( !ReadHuffmanCode(iRunLength,false,iBytePos0,iBitRatio0,acByte,
                                         iByteQuantity) )
                                    {
                                        return false;
                                    }

                                    if ( iRunLength >= 64 )
                                    {
                                        // make-up code
                                        iBeforeA0AccumRL += iRunLength;
                                        qA0.Position += iRunLength;
                                    }
                                    else
                                    {
                                        // terminating code
                                        if ( !WritePixels(iBeforeA0AccumRL+iRunLength,false,iRowPixelsRead,
                                             iBytePos1,iBitRatio1,rqNewBytes) )
                                        {
                                            return false;
                                        }
                                        iBeforeA0AccumRL = 0;
                                        qA0.Position += iRunLength;
                                        iRunQuantity++;
                                        break;
                                    }
                                }
                            }
                            else
                            {
                                for (/**/; /**/; /**/)
                                {
                                    // navigate the tree in accordance with the code's bits
                                    if ( !ReadHuffmanCode(iRunLength,false,iBytePos0,iBitRatio0,acByte,
                                         iByteQuantity) )
                                    {
                                        return false;
                                    }

                                    if ( iRunLength >= 64 )
                                    {
                                        // make-up code
                                        iBeforeA0AccumRL += iRunLength;
                                        qA0.Position += iRunLength;
                                    }
                                    else
                                    {
                                        // terminating code
                                        if ( !WritePixels(iBeforeA0AccumRL+iRunLength,false,iRowPixelsRead,
                                             iBytePos1,iBitRatio1,rqNewBytes) )
                                        {
                                            return false;
                                        }
                                        iBeforeA0AccumRL = 0;
                                        qA0.Position += iRunLength;
                                        iRunQuantity++;
                                        break;
                                    }
                                }
                                for (/**/; /**/; /**/)
                                {
                                    // navigate the tree in accordance with the code's bits
                                    if ( !ReadHuffmanCode(iRunLength,true,iBytePos0,iBitRatio0,acByte,
                                         iByteQuantity) )
                                    {
                                        return false;
                                    }

                                    if ( iRunLength >= 64 )
                                    {
                                        // make-up code
                                        iBeforeA0AccumRL += iRunLength;
                                        qA0.Position += iRunLength;
                                    }
                                    else
                                    {
                                        // terminating code
                                        if ( !WritePixels(iBeforeA0AccumRL+iRunLength,true,iRowPixelsRead,
                                             iBytePos1,iBitRatio1,rqNewBytes) )
                                        {
                                            return false;
                                        }
                                        iBeforeA0AccumRL = 0;
                                        qA0.Position += iRunLength;
                                        iRunQuantity++;
                                        break;
                                    }
                                }
                            }
                            break;
                        }
                        else if ( iZeroes0 == 6 && iOnes0 == 4 && iZeroes1 == 0 )
                        {
                            // entrance into uncompressed mode
                            iBeforeA0AccumRL = 0;
                            bUncompressedMode = true;
                            break;
                        }
                    }
                }

                if ( iRowPixelsRead == m_iWidth )
                {
                    // have finished the row
                    qProgress.MakeAdvance();

                    if ( ++iRowsRead == m_iHeight )
                        break;

                    if ( m_eEncoding == ET_GROUP_3_2D )
                    {
                        // rely on possibility of zero fill-bits before an "EOL+1" (or RTC) code
                        for (/**/; /**/; /**/)
                        {
                            GET_NEXT_BIT(iValue,iBytePos0,iBitRatio0,acByte,iByteQuantity);
                            if ( iValue == 1 )
                                break;
                        }
                        GET_NEXT_BIT(iValue,iBytePos0,iBitRatio0,acByte,iByteQuantity);
                        b1dMode = ( iValue == 1 );
                    }

                    iRowPixelsRead = 0;
                    iRunQuantity = 0;
                    bWhiteRun = true;
                    qA0.Position = -1;

                    // uncompressed data alignment
                    if ( !bDecImgRowsAreBytes )
                    {
                        iBytePos1++;
                        iBitRatio1 = 0;
                    }
                }
            }
            else
            {
                // uncompressed mode
                iZeroes = 0;
                if ( m_eUncompressedModeEncoding == UMET_CCITT )
                {
                    for (/**/; /**/; /**/)
                    {
                        GET_NEXT_BIT(iValue,iBytePos0,iBitRatio0,acByte,iByteQuantity);
                        if ( iValue == 1 )
                            break;

                        if ( ++iZeroes == 11 )
                        {
                            assert( false );
                            return false;
                        }
                    }

                    if ( iZeroes == 0 )
                    {
                        if ( !WritePixels(1,false,iRowPixelsRead,iBytePos1,iBitRatio1,rqNewBytes) )
                            return false;
                        qA0.Position++;
                    }
                    else if ( iZeroes == 1 )
                    {
                        if ( !WritePixels(1,true,iRowPixelsRead,iBytePos1,iBitRatio1,rqNewBytes) )
                            return false;
                        qA0.Position++;

                        UM_EDGE_CROSSING;

                        if ( !WritePixels(1,false,iRowPixelsRead,iBytePos1,iBitRatio1,rqNewBytes) )
                            return false;
                        qA0.Position++;
                    }
                    else if ( iZeroes == 2 )
                    {
                        for (int i = 0; i < 2; i++)
                        {
                            if ( !WritePixels(1,true,iRowPixelsRead,iBytePos1,iBitRatio1,rqNewBytes) )
                                return false;
                            qA0.Position++;

                            UM_EDGE_CROSSING;
                        }

                        if ( !WritePixels(1,false,iRowPixelsRead,iBytePos1,iBitRatio1,rqNewBytes) )
                            return false;
                        qA0.Position++;
                    }
                    else if ( iZeroes == 3 )
                    {
                        for (int i = 0; i < 3; i++)
                        {
                            if ( !WritePixels(1,true,iRowPixelsRead,iBytePos1,iBitRatio1,rqNewBytes) )
                                return false;
                            qA0.Position++;

                            UM_EDGE_CROSSING;
                        }

                        if ( !WritePixels(1,false,iRowPixelsRead,iBytePos1,iBitRatio1,rqNewBytes) )
                            return false;
                        qA0.Position++;
                    }
                    else if ( iZeroes == 4 )
                    {
                        for (int i = 0; i < 4; i++)
                        {
                            if ( !WritePixels(1,true,iRowPixelsRead,iBytePos1,iBitRatio1,rqNewBytes) )
                                return false;
                            qA0.Position++;

                            UM_EDGE_CROSSING;
                        }

                        if ( !WritePixels(1,false,iRowPixelsRead,iBytePos1,iBitRatio1,rqNewBytes) )
                            return false;
                        qA0.Position++;
                    }
                    else if ( iZeroes == 5 )
                    {
                        for (int i = 0; i < 5; i++)
                        {
                            if ( !WritePixels(1,true,iRowPixelsRead,iBytePos1,iBitRatio1,rqNewBytes) )
                                return false;
                            qA0.Position++;

                            UM_EDGE_CROSSING;
                        }
                    }
                    else if ( iZeroes == 6 )
                    {
                        GET_NEXT_BIT(iValue,iBytePos0,iBitRatio0,acByte,iByteQuantity);
                        bWhiteRun = ( iValue == 0 );
                        bUncompressedMode = false;
                    }
                    else if ( iZeroes == 7 )
                    {
                        if ( !WritePixels(1,true,iRowPixelsRead,iBytePos1,iBitRatio1,rqNewBytes) )
                            return false;
                        qA0.Position++;

                        GET_NEXT_BIT(iValue,iBytePos0,iBitRatio0,acByte,iByteQuantity);
                        bWhiteRun = ( iValue == 0 );
                        bUncompressedMode = false;
                    }
                    else if ( iZeroes == 8 )
                    {
                        for (int i = 0; i < 2; i++)
                        {
                            if ( !WritePixels(1,true,iRowPixelsRead,iBytePos1,iBitRatio1,rqNewBytes) )
                                return false;
                            qA0.Position++;

                            UM_EDGE_CROSSING;
                        }

                        GET_NEXT_BIT(iValue,iBytePos0,iBitRatio0,acByte,iByteQuantity);
                        bWhiteRun = ( iValue == 0 );
                        bUncompressedMode = false;
                    }
                    else if ( iZeroes == 9 )
                    {
                        for (int i = 0; i < 3; i++)
                        {
                            if ( !WritePixels(1,true,iRowPixelsRead,iBytePos1,iBitRatio1,rqNewBytes) )
                                return false;
                            qA0.Position++;

                            UM_EDGE_CROSSING;
                        }

                        GET_NEXT_BIT(iValue,iBytePos0,iBitRatio0,acByte,iByteQuantity);
                        bWhiteRun = ( iValue == 0 );
                        bUncompressedMode = false;
                    }
                    else  // iZeroes = 10
                    {
                        for (int i = 0; i < 4; i++)
                        {
                            if ( !WritePixels(1,true,iRowPixelsRead,iBytePos1,iBitRatio1,rqNewBytes) )
                                return false;
                            qA0.Position++;

                            UM_EDGE_CROSSING;
                        }

                        GET_NEXT_BIT(iValue,iBytePos0,iBitRatio0,acByte,iByteQuantity);
                        bWhiteRun = ( iValue == 0 );
                        bUncompressedMode = false;
                    }

                    UM_EDGE_CROSSING;
                }
                else  // m_eUncompressedModeEncoding = UMET_TIFF
                {
                    for (/**/; /**/; /**/)
                    {
                        GET_NEXT_BIT(iValue,iBytePos0,iBitRatio0,acByte,iByteQuantity);
                        if ( iValue == 0 )
                        {
                            if ( iZeroes != 5 )
                            {
                                if ( !WritePixels(1,true,iRowPixelsRead,iBytePos1,iBitRatio1,rqNewBytes) )
                                    return false;
                                qA0.Position++;
                                iZeroes++;
                            }
                            else
                            {
                                // may be stuffing 1-bit
                                GET_NEXT_BIT(iValue,iBytePos0,iBitRatio0,acByte,iByteQuantity);
                                if ( iValue == 1 )
                                {
                                    // stuffing
                                    iZeroes = 0;
                                }
                                else
                                {
                                    // must be exit-uncompressed-mode code
                                    GET_NEXT_BIT(iValue,iBytePos0,iBitRatio0,acByte,iByteQuantity);
                                    if ( iValue == 0 )
                                    {
                                        assert( false );
                                        return false;
                                    }

                                    GET_NEXT_BIT(iValue,iBytePos0,iBitRatio0,acByte,iByteQuantity);
                                    bWhiteRun = ( iValue == 0 );
                                    bUncompressedMode = false;
                                    break;
                                }
                            }
                        }
                        else
                        {
                            if ( !WritePixels(1,false,iRowPixelsRead,iBytePos1,iBitRatio1,rqNewBytes) )
                                return false;
                            qA0.Position++;
                            iZeroes = 0;
                        }
                    }
                }
            }
        }
    }

    return true;
}
//------------------------------------------------------------------------------------------------------------------
void CompressionFax::EncodeRow1d (int& riBytePos0, int& riBitRatio0, int& riBytePos1, int& riBitRatio1,
    const char* acByte, StillArray<char>& rqNewBytes) const
{
    bool bWhiteRun = true;
    int iRunLength, iBitsLeft, iSubRunLength, iValue, iX = 0;
    for (/**/; /**/; /**/)
    {
        iRunLength = 0;
        for (/**/; /**/; /**/)
        {
            riBytePos0 += Storage::ReadUBits(acByte+riBytePos0,riBitRatio0,1,iValue);
            if ( iValue == ( bWhiteRun ? 0 : 1 ) )
            {
                iRunLength++;
                if ( ++iX == m_iWidth )
                    break;
            }
            else
            {
                if ( riBitRatio0 != 0 )
                {
                    riBitRatio0--;
                }
                else
                {
                    riBytePos0--;
                    riBitRatio0 = 7;
                }
                break;
            }
        }

        // got the runlength, so write a code
        iBitsLeft = iRunLength;
        for (/**/; /**/; /**/)
        {
            if ( iBitsLeft < 64 )
            {
                WriteHuffmanCode(iBitsLeft,bWhiteRun,riBytePos1,riBitRatio1,rqNewBytes);
                break;
            }
            else
            {
                iSubRunLength = (iBitsLeft/64)*64;
                if ( iSubRunLength > 2560 )
                    iSubRunLength = 2560;
                WriteHuffmanCode(iSubRunLength,bWhiteRun,riBytePos1,riBitRatio1,rqNewBytes);
                iBitsLeft -= iSubRunLength;
            }
        }

        if ( iX == m_iWidth )
            break;
        bWhiteRun = !bWhiteRun;
    }
}
//------------------------------------------------------------------------------------------------------------------
void CompressionFax::WriteBits (int iValue, int iLength, int& riBytePos, int& riBitRatio, StillArray<char>& rqBytes)
{
    int iBitSum = riBitRatio + iLength;
    int iNewByteQuantity = riBytePos + ( (iBitSum & 7) != 0 ? iBitSum/8 + 1 : iBitSum/8 );
    if ( iNewByteQuantity > rqBytes.GetQuantity() )
    {
        // advance the buffer
        rqBytes.SetQuantity(iNewByteQuantity+iNewByteQuantity/ms_iAdvanceQuantityDivider);
    }
    riBytePos += Storage::WriteBits(rqBytes+riBytePos,riBitRatio,iLength,iValue);
}
//------------------------------------------------------------------------------------------------------------------
void CompressionFax::WriteHuffmanCode (int iRunLength, bool bWhiteRun, int& riBytePos, int& riBitRatio,
    StillArray<char>& rqBytes)
{
    int iIndex = ( iRunLength < 64 ? iRunLength : 63 + iRunLength/64 );
    const short* asCodeRecord = ( bWhiteRun ? gs_aasCodeRecordWhite[iIndex] : gs_aasCodeRecordBlack[iIndex] );

    WriteBits(asCodeRecord[0],asCodeRecord[1],riBytePos,riBitRatio,rqBytes);
}
//------------------------------------------------------------------------------------------------------------------
void CompressionFax::AlignBytes (int& riBytePos, int& riBitRatio, StillArray<char>& rqBytes)
{
    if ( riBitRatio != 0 )
    {
        int iNewByteQuantity = riBytePos + 1;
        if ( iNewByteQuantity > rqBytes.GetQuantity() )
        {
            // advance the buffer
            rqBytes.SetQuantity(iNewByteQuantity+iNewByteQuantity/ms_iAdvanceQuantityDivider);
        }
        riBytePos = iNewByteQuantity;
        riBitRatio = 0;
    }
}
//------------------------------------------------------------------------------------------------------------------
bool CompressionFax::ReadHuffmanCode (int& riRunLength, bool bWhiteRun, int& riBytePos, int& riBitRatio,
    const char* acByte, int iByteQuantity) const
{
    int iValue;
    BspNode<int>* pqNode = ( bWhiteRun ? m_apqRootNode[0] : m_apqRootNode[1] );
    for (/**/; /**/; /**/)
    {
        GET_NEXT_BIT(iValue,riBytePos,riBitRatio,acByte,iByteQuantity);
        pqNode = ( iValue == 0 ? pqNode->GetNegativeChild() : pqNode->GetPositiveChild() );
        if ( pqNode )
        {
            if ( pqNode->GetElement() != -1 )
            {
                // found valid code's bit sequence
                break;
            }
        }
        else
        {
            // out of the tree
            assert( false );
            return false;
        }
    }
    riRunLength = pqNode->GetElement();
    return true;
}
//------------------------------------------------------------------------------------------------------------------
bool CompressionFax::WritePixels (int iRunLength, bool bWhiteRun, int& riRowPixelsRead, int& riBytePos,
    int& riBitRatio, char* acDecompressedData) const
{
    if ( iRunLength != 0 )
    {
        if ( iRunLength < 0 || riRowPixelsRead + iRunLength > m_iWidth )
        {
            assert( false );
            return false;
        }
        int iColor = ( bWhiteRun ? 0 : -1 );
        int iBitsLeft = iRunLength;
        for (/**/; /**/; /**/)
        {
            if ( iBitsLeft >= 32 )
            {
                riBytePos += Storage::WriteBits(acDecompressedData+riBytePos,riBitRatio,32,iColor);
                iBitsLeft -= 32;
                if ( iBitsLeft == 0 )
                    break;
            }
            else
            {
                riBytePos += Storage::WriteBits(acDecompressedData+riBytePos,riBitRatio,iBitsLeft,iColor);
                break;
            }
        }
        riRowPixelsRead += iRunLength;
    }
    return true;
}
//------------------------------------------------------------------------------------------------------------------



