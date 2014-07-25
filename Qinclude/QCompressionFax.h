#ifndef QCOMPRESSIONFAX_H
#define QCOMPRESSIONFAX_H

// Fax compression/decompression.  An image consisting of 1-bit pixels, here represented as an array of bytes,
// must be for packing and will be on unpacking with byte-aligned bit rows.  Meaning of bits is the same as for
// CCITT: 0 is white and 1 is black.  The "uncompressed mode" is abandoned by common fax compressors and not
// supported by the class' packing procedure because of too insufficient documentation about.  However the mode
// is supported by unpacking.  Packed data on output will contain proper EOLs and RTC for all types except for
// ET_GROUP_3_1D_TIFF and ET_GROUP_4.  Unpacking processes EOLs by simply skipping all consecutive 0s and one
// or two (as for ET_GROUP_3_2D) bits after, i.e. not counting or checking them.
// DEFAULTS. K-factor is 4 (for packing); "uncompressed mode" is UMET_CCITT (for unpacking).

#include "QBspNode.h"
#include "QSmartPointer.h"
#include "QProgressBase.h"

namespace Q
{

class Q_ITEM CompressionFax : public ProgressBase, public Heap
{
public:
    enum EncodingType
    {
        ET_GROUP_3_1D,       // CCITT Rec. T.4 1-dimensional
        ET_GROUP_3_1D_TIFF,  // also known as "Group 3, No EOLs"
        ET_GROUP_3_2D,       // CCITT Rec. T.4 2-dimensional (with fibers of 1D)
        ET_GROUP_4           // CCITT Rec. T.6 2-dimensional
    };

    enum UncompressedModeEncodingType
    {
        UMET_CCITT,
        UMET_TIFF
    };

    // Construction and destruction.  Construction is one for both directions; it generates a Huffman tree if
    // for unpacking.
    CompressionFax (EncodingType eEncoding, bool bForPacking);
    ~CompressionFax ();

    // settings
    void SetImageDimensions (int iWidth, int iHeight);  // must be called in all cases
    void SetKFactor (int iK);  // may be useful only when packing with ET_GROUP_3_2D
    void SetUncompressedModeEncoding (  // may be useful only when unpacking with ET_GROUP_3_2D or ET_GROUP_4
        UncompressedModeEncodingType eUncompressedModeEncoding);

    // Packing.  Progressed.
    void Compress (const char* acByte, StillArray<char>& rqNewBytes);

    // Unpacking.  Progressed.
    bool Decompress (const char* acByte, int iByteQuantity, StillArray<char>& rqNewBytes);

private:
    void EncodeRow1d (int& riBytePos0, int& riBitRatio0, int& riBytePos1, int& riBitRatio1, const char* acByte,
        StillArray<char>& rqNewBytes) const;
    static void WriteBits (int iValue, int iLength, int& riBytePos, int& riBitRatio, StillArray<char>& rqBytes);
    static void WriteHuffmanCode (int iRunLength, bool bWhiteRun, int& riBytePos, int& riBitRatio,
        StillArray<char>& rqBytes);
    static void AlignBytes (int& riBytePos, int& riBitRatio, StillArray<char>& rqBytes);
    bool ReadHuffmanCode (int& riRunLength, bool bWhiteRun, int& riBytePos, int& riBitRatio,
        const char* acByte, int iByteQuantity) const;
    bool WritePixels (int iRunLength, bool bWhiteRun, int& riRowPixelsRead, int& riBytePos, int& riBitRatio,
        char* acDecompressedData) const;

    EncodingType m_eEncoding;
    bool m_bForPacking;
    int m_iWidth, m_iHeight;
    int m_iK;                                                   // default: 4
    UncompressedModeEncodingType m_eUncompressedModeEncoding;   // default: UMET_CCITT
    BspNode<int>* m_apqRootNode[2];

    // indicates how much the buffer is to grow depending on the amount of previously allocated memory (for
    // packing)
    static const int ms_iAdvanceQuantityDivider;  // 3 (on 1/3 each time)
};

#include "QCompressionFax.inl"

typedef SmartPointer<CompressionFax> CompressionFaxPtr;

}

#endif



