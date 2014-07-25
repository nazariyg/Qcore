#ifndef QCOMPRESSIONZLIB_H
#define QCOMPRESSIONZLIB_H

// The interface to features of the "ZLib general purpose compression library".  Unpacking from dynamic
// bytestreams is supported.
// DEFAULTS. Speed/size balance is 1.0 (best compression ratio); compression strategy is CST_ZLIB_DEFAULT.

#include "ZLib.h"
#include "QStillArray.h"
#include "QHeap.h"

namespace Q
{

class Q_ITEM CompressionZLib : public Heap
{
public:
    // the order of the items must not be changed
    enum CompressionStrategyType
    {
        CST_ZLIB_DEFAULT,
        CST_FILTERED,
        CST_HUFFMAN,
        CST_RLE,
        CST_FIXED,
        CST_BEST_AMONG_4  // among CST_ZLIB_DEFAULT, CST_FILTERED, CST_HUFFMAN, and CST_RLE
    };

    // construction and destruction
    CompressionZLib ();  // for both directions
    ~CompressionZLib ();

    // settings for packing
    void SetSpeedSizeBalance (float fBalance);  // 0.0 is for best speed, 1.0 is for best compression ratio
    void SetCompressionStrategy (CompressionStrategyType eStrategy);

    // engaging dynamical unpacking
    void SetDynamicalDecompressionMode ();

    // packing
    void Compress (const char* acByte, int iByteQuantity, StillArray<char>& rqNewBytes) const;

    // Unpacking.  If dynamical decompression mode is set, if a lack of input data is detected, it doesn't
    // cause any assert-false, the decompression state is remembered (a state record doesn't involve any memory
    // pointers), and 'true' is returned; a following call of the Decompress function will automatically
    // restore the state had been remembered; at each call of the function, acByte must point to the beginning
    // of entire compressed data.
    bool Decompress (const char* acByte, int iByteQuantity, StillArray<char>& rqNewBytes);

private:
    class DecompressionStateRecord
    {
    public:
        z_stream ZStream;
        int IntNextIn, IntNextOut;
    };

    static void CompressWithStrategy (const char* acByte, int iByteQuantity, StillArray<char>& rqNewBytes,
        float fSpeedSizeBalance, CompressionStrategyType eStrategy);

    float m_fSpeedSizeBalance;                          // default: 1.0 (best compression ratio)
    CompressionStrategyType m_eCompressionStrategy;     // CST_ZLIB_DEFAULT
    z_stream m_qZStream;
    bool m_bZAlloc;                                     // default: false
    bool m_bDynamicalDecompressionMode;                 // default: false
    bool m_bHasDecompressionStateRecord;                // default: false
    DecompressionStateRecord m_qDSR;
    StillArray<char> m_qDecompressionBuffer;

    // support for packing
    static const float ms_fCompressionBufferAdvanceForDefaultCompressionStrategy;     // 0.001
    static const float ms_fCompressionBufferAdvanceForNonDefaultCompressionStrategy;  // 0.05

    // support for unpacking
    static const int ms_iKBytesInDecompressionBuffer;  // 128
};

#include "QCompressionZLib.inl"

typedef SmartPointer<CompressionZLib> CompressionZLibPtr;

}

#endif



