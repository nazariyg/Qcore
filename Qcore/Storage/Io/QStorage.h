#ifndef QSTORAGE_H
#define QSTORAGE_H

// The Storage class collects functions and data associated with direct memory operating.

#include "QSystem.h"

namespace Q
{

class Q_ITEM Storage
{
public:
    // swap byte order (size = 2, 4, or 8)
    static void SwapBytes (int iSize, void* pvValue);
    static void SwapBytes (int iSize, int iQuantity, void* pvValue);

    // One-byte data operations.  Endianness is irrelevant.
    static int Write1 (char* acBuffer, int iQuantity, const void* pvData);
    static int Read1 (const char* acBuffer, int iQuantity, void* pvData);

    // The operations store the results in little-endian order.  The return value is the number of bytes read
    // or written.  The function name suffix indicates the number of bytes transferred per item.
    static int Write2le (char* acBuffer, int iQuantity, const void* pvData);
    static int Write4le (char* acBuffer, int iQuantity, const void* pvData);
    static int Write8le (char* acBuffer, int iQuantity, const void* pvData);
    static int Read2le (const char* acBuffer, int iQuantity, void* pvData);
    static int Read4le (const char* acBuffer, int iQuantity, void* pvData);
    static int Read8le (const char* acBuffer, int iQuantity, void* pvData);

    // The operations store the results in big-endian order.  The return value is the number of bytes read or
    // written.  The function name suffix indicates the number of bytes transferred per item.
    static int Write2be (char* acBuffer, int iQuantity, const void* pvData);
    static int Write4be (char* acBuffer, int iQuantity, const void* pvData);
    static int Write8be (char* acBuffer, int iQuantity, const void* pvData);
    static int Read2be (const char* acBuffer, int iQuantity, void* pvData);
    static int Read4be (const char* acBuffer, int iQuantity, void* pvData);
    static int Read8be (const char* acBuffer, int iQuantity, void* pvData);

    // Operations with bitfields.  In addition to standard unsigned bitfields signed bitfields are also
    // applicable.  In a signed bitfield the sign is indicated by the first bit and in the standard way: 0
    // means positive, 1 means negative.  The GetUBitQuantity and GetSBitQuantity functions are to return
    // bitfield lengths of unsigned and signed value kinds respectively.  For writing, the kind of a bitfield
    // is unrelated because the sign bit for signed values will be written automatically proceeding from the
    // indicated length of the bitfield (iBitQuantity).  For reading, it's necessary to know how to treat the
    // bitfield being read, so the two functions exist.  The writing/reading functions return the number of
    // crosses made through byte boundaries.  The acBuffer parameter is to point to the byte inside which
    // writing/reading must start; by adding to the pointer the number of crosses the user can get the address
    // of the next-to-operate byte where subsequent writing/reading may begin.  The riBitRatio parameter can
    // vary in the range [0, 7]; on input it denotes the position of a starting bit within the starting byte,
    // on done it is the position of a next-to-operate bit within the next-to-operate byte.  Endianness is
    // irrelevant.
    static int GetUBitQuantity (int iValue);
    static int GetSBitQuantity (int iValue);
    static int WriteBits (char* acBuffer, int& riBitRatio, int iBitQuantity, int iValue);
    static int ReadUBits (const char* acBuffer, int& riBitRatio, int iBitQuantity, int& riValue);
    static int ReadSBits (const char* acBuffer, int& riBitRatio, int iBitQuantity, int& riValue);

    // to know if a file exists and is accessible for reading
    static bool FileExists (const char* acFilename);

    // getting of file sizes (in bytes); returns success of the query
    static bool GetFileSize (int& riFileSize, const char* acFilename);

    // computing of cyclic redundancy check; used method is the one based on the number 0xedb88320
    static unsigned int GetCrc32Checksum (const char* acByte, int iQuantity);

    // the table for mirroring bits within bytes
    static const unsigned char ReverseBitOrder[256];
};

}

#endif



