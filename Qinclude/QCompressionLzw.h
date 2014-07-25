#ifndef QCOMPRESSIONLZW_H
#define QCOMPRESSIONLZW_H

// Variable length code Lempel-Ziv-Welch compression/decompression.  Unpacking from dynamic bytestreams is
// supported.  Symbol length is fixed for ET_TIFF type; maximal code length is not limited but only for
// ET_FREE.  ET_TIFF also has "hurried code length change" implemented.  ET_GIF works with data without
// sub-block separation and reverses bit order within codes.
// DEFAULTS. Symbol length is 8; maximal code length is 16 for ET_FREE and 12 for other types.
//
// For packing, usage of 16 as maximal code length makes noticeable improvement in compression ratio and even
// in speed if to compare with usage results of 12; if to go above 16 it seems that things still become better
// but too gradually now.  A string of characters is not used to express "string + extension character", but an
// integer value is adopted instead: a value represents two indices into the translation table concatenated
// together.  This made possible to use a hash map for encoding purposes.
//
// For unpacking, "deferred clear code" is implemented for ET_GIF.

#include "QProgressBase.h"
#include "QHeap.h"
#include "QStillArray.h"

namespace Q
{

class Q_ITEM CompressionLzw : public ProgressBase, public Heap
{
public:
    enum EncodingType
    {
        ET_FREE,
        ET_GIF,
        ET_TIFF
    };

    // Construction.  For both directions.
    CompressionLzw (EncodingType eEncoding = ET_FREE);

    // Settings for both directions.  A symbol length must be less than a maximal code length; the sum of a
    // symbol length and a maximal code length must be less than 32.
    void SetSymbolLength (int iSymbolLength);            // only if encoding is ET_FREE or ET_GIF
    void SetMaximalCodeLength (int iMaximalCodeLength);  // only if encoding is ET_FREE

    // dynamical unpacking related
    void SetDynamicalDecompressionMode ();      // engaging
    bool HasDecompressionStateRecord () const;  // recent state record presence
    int GetDecodedSymbolQuantity () const;      // only if has the recent state record

    // Packing.  Progressed.
    void Compress (const char* acByte, int iByteQuantity, StillArray<char>& rqNewBytes);

    // Unpacking.  The function does usual severe treatment of data being decompressed non-dynamically except
    // for one case: if any critical check fails when doing with ET_GIF, assert-false doesn't occur (and
    // 'false' is returned).  If dynamical decompression mode is set, if a read-next-code failure happens, it
    // doesn't cause any assert-false, the decompression state is remembered (a state record doesn't involve
    // any memory pointers), and 'true' is returned; a following call of the Decompress function will
    // automatically restore the state had been remembered; at each call of the function, acByte must point to
    // the beginning of entire compressed data.  Progressed.
    bool Decompress (const char* acByte, int iByteQuantity, StillArray<char>& rqNewBytes);

private:
    // support for decompression
    class StringRecord
    {
    public:
        StringRecord ()
            :
            SymbolQuantity(0),
            PrefixStringPtr(0)
        {
        };

        int SymbolQuantity, FirstSymbol, ExtensionSymbol;
        StringRecord* PrefixStringPtr;
    };

    // support for decompression
    class DecompressionStateRecord
    {
    public:
        bool Initialized;
        int TwoPowSymLen, SymLenPlusOne, ClrCode, EoiCode, BytePos0, BitRatio0, BytePos1, BitRatio1, MaxTableSize,
            Code, PrevCode, TableSize, SpecTS, CurrentCodeLength, DecodedSymbolQuantity;
    };

    int Concatenate (int iPrefixStringIndex, int iExtensionSymbol) const;
    void WriteCode (int iCode, int iCodeLength, int& riBytePos, int& riBitRatio, StillArray<char>& rqBytes) const;
    bool ReadCode (int& riCode, int iCodeLength, int& riBytePos, int& riBitRatio, const char* acByte,
        int iByteQuantity) const;
    bool WriteString (const StringRecord& rqStrRec, int& riBytePos, int& riBitRatio, int& riDecodedSymbolQuantity,
        StillArray<char>& rqBytes) const;

    EncodingType m_eEncoding;
    int m_iSymbolLength;                  // default: 8
    int m_iMaximalCodeLength;             // default: 16 if ET_FREE, 12 otherwise
    StillArray<StillArray<int> > m_qReverseBitOrderArrays;
    bool m_bDynamicalDecompressionMode;   // default: false
    bool m_bHasDecompressionStateRecord;  // default: false
    DecompressionStateRecord m_qIDSR;
    StillArray<StringRecord> m_qDecompressionTranslationTable;
    int m_iRestorePointId;

    // indicates how much the buffer is to grow depending on the amount of previously allocated memory (for
    // packing)
    static const int ms_iAdvanceQuantityDivider;  // 3 (on 1/3 each time)
};

#include "QCompressionLzw.inl"

typedef SmartPointer<CompressionLzw> CompressionLzwPtr;

}

#endif



