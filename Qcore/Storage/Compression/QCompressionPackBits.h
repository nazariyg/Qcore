#ifndef QCOMPRESSIONPACKBITS_H
#define QCOMPRESSIONPACKBITS_H

// Apple Macintosh PackBits compression/decompression.  The implementation of packing cares about all small
// things.

#include "QStillArray.h"

namespace Q
{

class Q_ITEM CompressionPackBits
{
public:
    // packing
    static void Compress (const char* acByte, int iByteQuantity, StillArray<char>& rqNewBytes);

    // unpacking
    static bool Decompress (const char* acByte, int iByteQuantity, StillArray<char>& rqNewBytes);
};

}

#endif



