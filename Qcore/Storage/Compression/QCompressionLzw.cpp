#include "QCompressionLzw.h"
#include "QCompressionLzw.mcr"
#include "QHashMap.h"
#include "QStorage.h"
#include "QArray.h"
#include "QSystUtils.h"
#include "QProgressProbe.h"
using namespace Q;

// for packing; indicates how much the buffer is to grow depending on amount of previously allocated memory
const int CompressionLzw::ms_iAdvanceQuantityDivider(3);

// support for packing
class HashFunc
{
public:
    int operator() (int iKey) const
    {
        return iKey;
    };
};

//------------------------------------------------------------------------------------------------------------------
CompressionLzw::CompressionLzw (EncodingType eEncoding)
    :
    m_eEncoding(eEncoding),
    m_iSymbolLength(8),
    m_iMaximalCodeLength(( eEncoding == ET_FREE ? 16 : 12 )),
    m_bDynamicalDecompressionMode(false),
    m_bHasDecompressionStateRecord(false)
{
    if ( eEncoding == ET_GIF )
    {
        int iValue, iRevValue, i0MO;
        m_qReverseBitOrderArrays.SetQuantity(13);
        for (int i0 = 3; i0 <= 12; i0++)
        {
            i0MO = i0 - 1;
            StillArray<int>& rqRBO = m_qReverseBitOrderArrays[i0];

            rqRBO.SetQuantity(1 << i0);
            for (int i1 = 0; i1 < rqRBO.GetQuantity(); i1++)
            {
                iValue = i1;
                iRevValue = 0;
                for (int i2 = i0MO; i2 >= 0; i2--)
                {
                    iRevValue |= (iValue & 1) << i2;
                    iValue >>= 1;
                }
                rqRBO[i1] = iRevValue;
            }
        }
    }
}
//------------------------------------------------------------------------------------------------------------------
void CompressionLzw::Compress (const char* acByte, int iByteQuantity, StillArray<char>& rqNewBytes)
{
    assert( m_iSymbolLength < m_iMaximalCodeLength );
    assert( m_iSymbolLength + m_iMaximalCodeLength < 32 );

    rqNewBytes.RemoveAll();

    int iTwoPowSymLen = 1 << m_iSymbolLength;
    int iSymLenPlusOne = m_iSymbolLength + 1;

    // control codes
    int iClrCode = iTwoPowSymLen;
    int iEoiCode = iTwoPowSymLen + 1;

    rqNewBytes.SetQuantity(1);
    int iBytePos0 = 0;
    int iBitRatio0 = 0;
    int iBytePos1 = 0;
    int iBitRatio1 = 0;
    int iMaxTableSize = 1 << m_iMaximalCodeLength;
    int iSymbolQuantity = iByteQuantity*8/m_iSymbolLength;

    HashMap<int,int,HashFunc> qTranslationTable;
    HashMap<int,int,HashFunc>::IteratorT qIt;
    int iPrevTableSize, iTableSize, iK, iConcat;
    int iCurrentCodeLength = iSymLenPlusOne;
    int iSrtSym = 0;
    int iOmegaIndex = -1;

    // for a progress bar
    ProgressProbe qProgress(this);

NewSession:

    // initialize the translation table
    qTranslationTable.RemoveAll();
    for (int i = 0; i < iTwoPowSymLen; i++)
        qTranslationTable.Insert(Concatenate(-1,i),i);
    iTableSize = iTwoPowSymLen + 2;  // reserve the two codes: "clear" and "end-of-information"

    // encode
    WriteCode(iClrCode,iCurrentCodeLength,iBytePos1,iBitRatio1,rqNewBytes);
    iCurrentCodeLength = iSymLenPlusOne;
    for (int i0 = iSrtSym; i0 < iSymbolQuantity; i0++)
    {
        iBytePos0 += Storage::ReadUBits(acByte+iBytePos0,iBitRatio0,m_iSymbolLength,iK);
        iConcat = Concatenate(iOmegaIndex,iK);
        qIt = qTranslationTable.Find(iConcat);
        if ( qIt != qTranslationTable.GetEnd() )
        {
            iOmegaIndex = qIt->Second;
        }
        else
        {
            WriteCode(iOmegaIndex,iCurrentCodeLength,iBytePos1,iBitRatio1,rqNewBytes);
            iPrevTableSize = iTableSize;
            qTranslationTable.Insert(iConcat,iTableSize++);
            iOmegaIndex = iK;

            if ( m_eEncoding != ET_TIFF )
            {
                if ( iTableSize == iMaxTableSize )
                {
                    // reset
                    iSrtSym = i0 + 1;
                    goto NewSession;
                }
                if ( IsPowerOfTwo(iPrevTableSize) )
                {
                    iCurrentCodeLength = Storage::GetUBitQuantity(iPrevTableSize);
                    qProgress.SetDone(((float)i0)/iSymbolQuantity);
                }
            }
            else
            {
                if ( iTableSize + 1 == iMaxTableSize )
                {
                    // reset
                    iSrtSym = i0 + 1;
                    goto NewSession;
                }
                if ( IsPowerOfTwo(iTableSize) )
                {
                    iCurrentCodeLength = Storage::GetUBitQuantity(iTableSize);
                    qProgress.SetDone(((float)i0)/iSymbolQuantity);
                }
            }
        }
    }
    WriteCode(iOmegaIndex,iCurrentCodeLength,iBytePos1,iBitRatio1,rqNewBytes);
    if ( m_eEncoding != ET_TIFF )
    {
        if ( IsPowerOfTwo(iTableSize) )
            iCurrentCodeLength = Storage::GetUBitQuantity(iTableSize);
    }
    else
    {
        if ( IsPowerOfTwo(iTableSize+1) )
            iCurrentCodeLength = Storage::GetUBitQuantity(iTableSize+1);
    }
    WriteCode(iEoiCode,iCurrentCodeLength,iBytePos1,iBitRatio1,rqNewBytes);

    qProgress.SetDone(1.0f);

    // cut off unused trailing bytes
    if ( iBitRatio1 != 0 )
        rqNewBytes.SetQuantity(iBytePos1+1);
    else
        rqNewBytes.SetQuantity(iBytePos1);
}
//------------------------------------------------------------------------------------------------------------------
bool CompressionLzw::Decompress (const char* acByte, int iByteQuantity, StillArray<char>& rqNewBytes)
{
    if ( !m_bHasDecompressionStateRecord )
        rqNewBytes.RemoveAll();

    // for a progress bar
    ProgressProbe qProgress(this);

    bool bSuccess = true;
    bool bReadCodeFailedForDyna = false;

    DecompressionStateRecord qState;
    if ( !m_bHasDecompressionStateRecord )
    {
        qState.TwoPowSymLen = 1 << m_iSymbolLength;
        qState.SymLenPlusOne = m_iSymbolLength + 1;

        // control codes
        qState.ClrCode = qState.TwoPowSymLen;
        qState.EoiCode = qState.TwoPowSymLen + 1;

        rqNewBytes.SetQuantity(1);
        qState.BytePos0 = 0;
        qState.BitRatio0 = 0;
        qState.BytePos1 = 0;
        qState.BitRatio1 = 0;
        qState.MaxTableSize = 1 << m_iMaximalCodeLength;

        qState.CurrentCodeLength = qState.SymLenPlusOne;
        qState.Initialized = false;

        qState.DecodedSymbolQuantity = 0;
    }
    else
    {
        // restore the state
        qState = m_qIDSR;

        if ( m_iRestorePointId == 0 )
            goto Restoring0;
        else  // m_iRestorePointId = 1
            goto Restoring1;
    }

    // decode
    for (/**/; /**/; /**/)
    {
Restoring0:
        if ( !ReadCode(qState.Code,qState.CurrentCodeLength,qState.BytePos0,qState.BitRatio0,acByte,
             iByteQuantity) )
        {
            ON_READCODE_FAIL(0);
        }
        if ( qState.Code == qState.EoiCode )
            break;
        if ( qState.Code == qState.ClrCode )
        {
            // initialize the translation table
            m_qDecompressionTranslationTable.RemoveAll();
            m_qDecompressionTranslationTable.SetQuantity(qState.MaxTableSize);
            for (int i = 0; i < qState.TwoPowSymLen; i++)
            {
                StringRecord& rqStrRec = m_qDecompressionTranslationTable[i];

                rqStrRec.SymbolQuantity = 1;
                rqStrRec.FirstSymbol = rqStrRec.ExtensionSymbol = i;
            }
            qState.TableSize = qState.TwoPowSymLen + 2;  // reserve the two codes: "clear" and "end-of-information"

            qState.CurrentCodeLength = qState.SymLenPlusOne;

Restoring1:
            if ( !ReadCode(qState.Code,qState.CurrentCodeLength,qState.BytePos0,qState.BitRatio0,acByte,
                 iByteQuantity) )
            {
                ON_READCODE_FAIL(1);
            }
            if ( qState.Code == qState.EoiCode )
                break;
            if ( !WriteString(m_qDecompressionTranslationTable[qState.Code],qState.BytePos1,qState.BitRatio1,
                 qState.DecodedSymbolQuantity,rqNewBytes) )
            {
                ON_CRITICAL_CHECK_FAIL;
            }

            if ( m_iSymbolLength == 1 )
            {
                // special case
                qState.CurrentCodeLength = 3;
            }

            qState.Initialized = true;
        }
        else
        {
            if ( !qState.Initialized
            ||   qState.Code >= qState.MaxTableSize || qState.TableSize == qState.MaxTableSize )
            {
                ON_CRITICAL_CHECK_FAIL;
            }

            const StringRecord& rqStrRec0 = m_qDecompressionTranslationTable[qState.Code];
            StringRecord& rqStrRec1 = m_qDecompressionTranslationTable[qState.PrevCode];
            StringRecord& rqStrRec2 = m_qDecompressionTranslationTable[qState.TableSize++];

            if ( rqStrRec1.SymbolQuantity == 0 )
                ON_CRITICAL_CHECK_FAIL;

            rqStrRec2.SymbolQuantity = rqStrRec1.SymbolQuantity + 1;
            rqStrRec2.FirstSymbol = rqStrRec1.FirstSymbol;
            rqStrRec2.PrefixStringPtr = &rqStrRec1;
            if ( rqStrRec0.SymbolQuantity != 0 )
            {
                rqStrRec2.ExtensionSymbol = rqStrRec0.FirstSymbol;
                if ( !WriteString(rqStrRec0,qState.BytePos1,qState.BitRatio1,qState.DecodedSymbolQuantity,
                     rqNewBytes) )
                {
                    ON_CRITICAL_CHECK_FAIL;
                }
            }
            else
            {
                rqStrRec2.ExtensionSymbol = rqStrRec1.FirstSymbol;
                if ( !WriteString(rqStrRec2,qState.BytePos1,qState.BitRatio1,qState.DecodedSymbolQuantity,
                     rqNewBytes) )
                {
                    ON_CRITICAL_CHECK_FAIL;
                }
            }

            qState.SpecTS = ( m_eEncoding != ET_TIFF ? qState.TableSize : qState.TableSize + 1 );
            if ( IsPowerOfTwo(qState.SpecTS) )
            {
                qState.CurrentCodeLength = Storage::GetUBitQuantity(qState.SpecTS);

                if ( qState.CurrentCodeLength > m_iMaximalCodeLength )
                {
                    if ( m_eEncoding != ET_GIF )
                    {
                        assert( false );
                        bSuccess = false;
                        goto Exit;
                    }
                    else
                    {
                        // "deferred clear code"
                        qState.CurrentCodeLength = m_iMaximalCodeLength;
                    }
                }

                qProgress.SetDone(((float)qState.BytePos0)/iByteQuantity);
            }
        }
        qState.PrevCode = qState.Code;
    }
Exit:

    qProgress.SetDone(1.0f);

    if ( !bReadCodeFailedForDyna )
    {
        m_qDecompressionTranslationTable.RemoveAll();
        m_bHasDecompressionStateRecord = false;
    }
    else
    {
        // save the state
        m_qIDSR = qState;
        m_bHasDecompressionStateRecord = true;
    }

    // cut off unused trailing bytes
    if ( qState.BitRatio1 != 0 )
        rqNewBytes.SetQuantity(qState.BytePos1+1);
    else
        rqNewBytes.SetQuantity(qState.BytePos1);

    return bSuccess;
}
//------------------------------------------------------------------------------------------------------------------
void CompressionLzw::WriteCode (int iCode, int iCodeLength, int& riBytePos, int& riBitRatio,
    StillArray<char>& rqBytes) const
{
    int iBitSum = riBitRatio + iCodeLength;
    int iNewByteQuantity = riBytePos + ( (iBitSum & 7) != 0 ? iBitSum/8 + 1 : iBitSum/8 );
    if ( iNewByteQuantity > rqBytes.GetQuantity() )
    {
        // advance the buffer
        rqBytes.SetQuantity(iNewByteQuantity+iNewByteQuantity/ms_iAdvanceQuantityDivider);
    }

    if ( m_eEncoding == ET_GIF )
        iCode = m_qReverseBitOrderArrays[iCodeLength][iCode];

    riBytePos += Storage::WriteBits(rqBytes+riBytePos,riBitRatio,iCodeLength,iCode);
}
//------------------------------------------------------------------------------------------------------------------
bool CompressionLzw::ReadCode (int& riCode, int iCodeLength, int& riBytePos, int& riBitRatio, const char* acByte,
    int iByteQuantity) const
{
    int iBitSum = riBitRatio + iCodeLength;
    if ( riBytePos + ( (iBitSum & 7) != 0 ? iBitSum/8 + 1 : iBitSum/8 ) > iByteQuantity )
        return false;
    riBytePos += Storage::ReadUBits(acByte+riBytePos,riBitRatio,iCodeLength,riCode);

    if ( m_eEncoding == ET_GIF )
        riCode = m_qReverseBitOrderArrays[iCodeLength][riCode];

    return true;
}
//------------------------------------------------------------------------------------------------------------------
bool CompressionLzw::WriteString (const StringRecord& rqStrRec, int& riBytePos, int& riBitRatio,
    int& riDecodedSymbolQuantity, StillArray<char>& rqBytes) const
{
    int iBitSum = riBitRatio + rqStrRec.SymbolQuantity*m_iSymbolLength;
    int iNewByteQuantity = riBytePos + ( (iBitSum & 7) != 0 ? iBitSum/8 + 1 : iBitSum/8 );
    if ( iNewByteQuantity > rqBytes.GetQuantity() )
    {
        // advance the buffer
        rqBytes.SetQuantity(iNewByteQuantity+iNewByteQuantity/ms_iAdvanceQuantityDivider);
    }

    char* acBytesUpPos = rqBytes + riBytePos;
    int iOnceBitRatio, iBitsLeft = iBitSum - m_iSymbolLength;
    const StringRecord* pqStrRec = &rqStrRec;
    for (int i = 0; i < rqStrRec.SymbolQuantity; i++)
    {
        if ( !pqStrRec )
            return false;

        iOnceBitRatio = iBitsLeft & 7;
        Storage::WriteBits(acBytesUpPos+iBitsLeft/8,iOnceBitRatio,m_iSymbolLength,pqStrRec->ExtensionSymbol);

        iBitsLeft -= m_iSymbolLength;
        pqStrRec = pqStrRec->PrefixStringPtr;
    }
    riBytePos += iBitSum/8;
    riBitRatio = iBitSum & 7;

    riDecodedSymbolQuantity += rqStrRec.SymbolQuantity;

    return true;
}
//------------------------------------------------------------------------------------------------------------------



