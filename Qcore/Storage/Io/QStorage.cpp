#include "QStorage.h"
#include <sys/stat.h>
using namespace Q;

// support for GetCrc32Checksum
static const unsigned int gs_auiCrc32Table[256] = {
             0, 1996959894, 3993919788, 2567524794,  124634137, 1886057615, 3915621685, 2657392035,
     249268274, 2044508324, 3772115230, 2547177864,  162941995, 2125561021, 3887607047, 2428444049,
     498536548, 1789927666, 4089016648, 2227061214,  450548861, 1843258603, 4107580753, 2211677639,
     325883990, 1684777152, 4251122042, 2321926636,  335633487, 1661365465, 4195302755, 2366115317,
     997073096, 1281953886, 3579855332, 2724688242, 1006888145, 1258607687, 3524101629, 2768942443,
     901097722, 1119000684, 3686517206, 2898065728,  853044451, 1172266101, 3705015759, 2882616665,
     651767980, 1373503546, 3369554304, 3218104598,  565507253, 1454621731, 3485111705, 3099436303,
     671266974, 1594198024, 3322730930, 2970347812,  795835527, 1483230225, 3244367275, 3060149565,
    1994146192,   31158534, 2563907772, 4023717930, 1907459465,  112637215, 2680153253, 3904427059,
    2013776290,  251722036, 2517215374, 3775830040, 2137656763,  141376813, 2439277719, 3865271297,
    1802195444,  476864866, 2238001368, 4066508878, 1812370925,  453092731, 2181625025, 4111451223,
    1706088902,  314042704, 2344532202, 4240017532, 1658658271,  366619977, 2362670323, 4224994405,
    1303535960,  984961486, 2747007092, 3569037538, 1256170817, 1037604311, 2765210733, 3554079995,
    1131014506,  879679996, 2909243462, 3663771856, 1141124467,  855842277, 2852801631, 3708648649,
    1342533948,  654459306, 3188396048, 3373015174, 1466479909,  544179635, 3110523913, 3462522015,
    1591671054,  702138776, 2966460450, 3352799412, 1504918807,  783551873, 3082640443, 3233442989,
    3988292384, 2596254646,   62317068, 1957810842, 3939845945, 2647816111,   81470997, 1943803523,
    3814918930, 2489596804,  225274430, 2053790376, 3826175755, 2466906013,  167816743, 2097651377,
    4027552580, 2265490386,  503444072, 1762050814, 4150417245, 2154129355,  426522225, 1852507879,
    4275313526, 2312317920,  282753626, 1742555852, 4189708143, 2394877945,  397917763, 1622183637,
    3604390888, 2714866558,  953729732, 1340076626, 3518719985, 2797360999, 1068828381, 1219638859,
    3624741850, 2936675148,  906185462, 1090812512, 3747672003, 2825379669,  829329135, 1181335161,
    3412177804, 3160834842,  628085408, 1382605366, 3423369109, 3138078467,  570562233, 1426400815,
    3317316542, 2998733608,  733239954, 1555261956, 3268935591, 3050360625,  752459403, 1541320221,
    2607071920, 3965973030, 1969922972,   40735498, 2617837225, 3943577151, 1913087877,   83908371,
    2512341634, 3803740692, 2075208622,  213261112, 2463272603, 3855990285, 2094854071,  198958881,
    2262029012, 4057260610, 1759359992,  534414190, 2176718541, 4139329115, 1873836001,  414664567,
    2282248934, 4279200368, 1711684554,  285281116, 2405801727, 4167216745, 1634467795,  376229701,
    2685067896, 3608007406, 1308918612,  956543938, 2808555105, 3495958263, 1231636301, 1047427035,
    2932959818, 3654703836, 1088359270,  936918000, 2847714899, 3736837829, 1202900863,  817233897,
    3183342108, 3401237130, 1404277552,  615818150, 3134207493, 3453421203, 1423857449,  601450431,
    3009837614, 3294710456, 1567103746,  711928724, 3020668471, 3272380065, 1510334235,  755167117};

// the table for mirroring bits within bytes
const unsigned char Storage::ReverseBitOrder[256] = {
      0, 128,  64, 192,  32, 160,  96, 224,  16, 144,  80, 208,  48, 176, 112, 240,
      8, 136,  72, 200,  40, 168, 104, 232,  24, 152,  88, 216,  56, 184, 120, 248,
      4, 132,  68, 196,  36, 164, 100, 228,  20, 148,  84, 212,  52, 180, 116, 244,
     12, 140,  76, 204,  44, 172, 108, 236,  28, 156,  92, 220,  60, 188, 124, 252,
      2, 130,  66, 194,  34, 162,  98, 226,  18, 146,  82, 210,  50, 178, 114, 242,
     10, 138,  74, 202,  42, 170, 106, 234,  26, 154,  90, 218,  58, 186, 122, 250,
      6, 134,  70, 198,  38, 166, 102, 230,  22, 150,  86, 214,  54, 182, 118, 246,
     14, 142,  78, 206,  46, 174, 110, 238,  30, 158,  94, 222,  62, 190, 126, 254,
      1, 129,  65, 193,  33, 161,  97, 225,  17, 145,  81, 209,  49, 177, 113, 241,
      9, 137,  73, 201,  41, 169, 105, 233,  25, 153,  89, 217,  57, 185, 121, 249,
      5, 133,  69, 197,  37, 165, 101, 229,  21, 149,  85, 213,  53, 181, 117, 245,
     13, 141,  77, 205,  45, 173, 109, 237,  29, 157,  93, 221,  61, 189, 125, 253,
      3, 131,  67, 195,  35, 163,  99, 227,  19, 147,  83, 211,  51, 179, 115, 243,
     11, 139,  75, 203,  43, 171, 107, 235,  27, 155,  91, 219,  59, 187, 123, 251,
      7, 135,  71, 199,  39, 167, 103, 231,  23, 151,  87, 215,  55, 183, 119, 247,
     15, 143,  79, 207,  47, 175, 111, 239,  31, 159,  95, 223,  63, 191, 127, 255};

//------------------------------------------------------------------------------------------------------------------
void Storage::SwapBytes (int iSize, void* pvValue)
{
    // size must be even
    assert( iSize >= 2 && (iSize & 1) == 0 );

    char* acBytes = (char*)pvValue;
    int iSD2 = iSize/2;
    for (int i0 = 0, i1 = iSize-1; i0 < iSD2; i0++, i1--)
    {
        char cSave = acBytes[i0];
        acBytes[i0] = acBytes[i1];
        acBytes[i1] = cSave;
    }
}
//------------------------------------------------------------------------------------------------------------------
void Storage::SwapBytes (int iSize, int iQuantity, void* pvValue)
{
    // size must be even
    assert( iSize >= 2 && (iSize & 1) == 0 && iQuantity >= 0 );

    char* acBytes = (char*)pvValue;
    int iSD2 = iSize/2;
    for (int i = 0; i < iQuantity; i++, acBytes += iSize)
    {
        for (int i0 = 0, i1 = iSize-1; i0 < iSD2; i0++, i1--)
        {
            char cSave = acBytes[i0];
            acBytes[i0] = acBytes[i1];
            acBytes[i1] = cSave;
        }
    }
}
//------------------------------------------------------------------------------------------------------------------
int Storage::Write1 (char* acBuffer, int iQuantity, const void* pvData)
{
    assert( iQuantity >= 0 );

    MemCpy(acBuffer,pvData,iQuantity);
    return iQuantity;
}
//------------------------------------------------------------------------------------------------------------------
int Storage::Read1 (const char* acBuffer, int iQuantity, void* pvData)
{
    assert( iQuantity >= 0 );

    MemCpy(pvData,acBuffer,iQuantity);
    return iQuantity;
}
//------------------------------------------------------------------------------------------------------------------
int Storage::Write2le (char* acBuffer, int iQuantity, const void* pvData)
{
    assert( iQuantity >= 0 );

    int iNumBytes = 2*iQuantity;
    MemCpy(acBuffer,pvData,iNumBytes);
#ifdef Q_BIG_ENDIAN
    SwapBytes(2,iQuantity,acBuffer);
#endif
    return iNumBytes;
}
//------------------------------------------------------------------------------------------------------------------
int Storage::Write4le (char* acBuffer, int iQuantity, const void* pvData)
{
    assert( iQuantity >= 0 );

    int iNumBytes = 4*iQuantity;
    MemCpy(acBuffer,pvData,iNumBytes);
#ifdef Q_BIG_ENDIAN
    SwapBytes(4,iQuantity,acBuffer);
#endif
    return iNumBytes;
}
//------------------------------------------------------------------------------------------------------------------
int Storage::Write8le (char* acBuffer, int iQuantity, const void* pvData)
{
    assert( iQuantity >= 0 );

    int iNumBytes = 8*iQuantity;
    MemCpy(acBuffer,pvData,iNumBytes);
#ifdef Q_BIG_ENDIAN
    SwapBytes(8,iQuantity,acBuffer);
#endif
    return iNumBytes;
}
//------------------------------------------------------------------------------------------------------------------
int Storage::Read2le (const char* acBuffer, int iQuantity, void* pvData)
{
    assert( iQuantity >= 0 );

    int iNumBytes = 2*iQuantity;
    MemCpy(pvData,acBuffer,iNumBytes);
#ifdef Q_BIG_ENDIAN
    SwapBytes(2,iQuantity,pvData);
#endif
    return iNumBytes;
}
//------------------------------------------------------------------------------------------------------------------
int Storage::Read4le (const char* acBuffer, int iQuantity, void* pvData)
{
    assert( iQuantity >= 0 );

    int iNumBytes = 4*iQuantity;
    MemCpy(pvData,acBuffer,iNumBytes);
#ifdef Q_BIG_ENDIAN
    SwapBytes(4,iQuantity,pvData);
#endif
    return iNumBytes;
}
//------------------------------------------------------------------------------------------------------------------
int Storage::Read8le (const char* acBuffer, int iQuantity, void* pvData)
{
    assert( iQuantity >= 0 );

    int iNumBytes = 8*iQuantity;
    MemCpy(pvData,acBuffer,iNumBytes);
#ifdef Q_BIG_ENDIAN
    SwapBytes(8,iQuantity,pvData);
#endif
    return iNumBytes;
}
//------------------------------------------------------------------------------------------------------------------
int Storage::Write2be (char* acBuffer, int iQuantity, const void* pvData)
{
    assert( iQuantity >= 0 );

    int iNumBytes = 2*iQuantity;
    MemCpy(acBuffer,pvData,iNumBytes);
#ifndef Q_BIG_ENDIAN
    SwapBytes(2,iQuantity,acBuffer);
#endif
    return iNumBytes;
}
//------------------------------------------------------------------------------------------------------------------
int Storage::Write4be (char* acBuffer, int iQuantity, const void* pvData)
{
    assert( iQuantity >= 0 );

    int iNumBytes = 4*iQuantity;
    MemCpy(acBuffer,pvData,iNumBytes);
#ifndef Q_BIG_ENDIAN
    SwapBytes(4,iQuantity,acBuffer);
#endif
    return iNumBytes;
}
//------------------------------------------------------------------------------------------------------------------
int Storage::Write8be (char* acBuffer, int iQuantity, const void* pvData)
{
    assert( iQuantity >= 0 );

    int iNumBytes = 8*iQuantity;
    MemCpy(acBuffer,pvData,iNumBytes);
#ifndef Q_BIG_ENDIAN
    SwapBytes(8,iQuantity,acBuffer);
#endif
    return iNumBytes;
}
//------------------------------------------------------------------------------------------------------------------
int Storage::Read2be (const char* acBuffer, int iQuantity, void* pvData)
{
    assert( iQuantity >= 0 );

    int iNumBytes = 2*iQuantity;
    MemCpy(pvData,acBuffer,iNumBytes);
#ifndef Q_BIG_ENDIAN
    SwapBytes(2,iQuantity,pvData);
#endif
    return iNumBytes;
}
//------------------------------------------------------------------------------------------------------------------
int Storage::Read4be (const char* acBuffer, int iQuantity, void* pvData)
{
    assert( iQuantity >= 0 );

    int iNumBytes = 4*iQuantity;
    MemCpy(pvData,acBuffer,iNumBytes);
#ifndef Q_BIG_ENDIAN
    SwapBytes(4,iQuantity,pvData);
#endif
    return iNumBytes;
}
//------------------------------------------------------------------------------------------------------------------
int Storage::Read8be (const char* acBuffer, int iQuantity, void* pvData)
{
    assert( iQuantity >= 0 );

    int iNumBytes = 8*iQuantity;
    MemCpy(pvData,acBuffer,iNumBytes);
#ifndef Q_BIG_ENDIAN
    SwapBytes(8,iQuantity,pvData);
#endif
    return iNumBytes;
}
//------------------------------------------------------------------------------------------------------------------
int Storage::GetUBitQuantity (int iValue)
{
    assert( iValue >= 0 );

    if ( iValue == 0 )
        return 1;

    int iSize = 0;
    while ( iValue > 0 )
    {
        iValue >>= 1;
        iSize++;
    }
    return iSize;
}
//------------------------------------------------------------------------------------------------------------------
int Storage::GetSBitQuantity (int iValue)
{
    return ( iValue < 0 ? GetUBitQuantity(-iValue) : GetUBitQuantity(iValue) ) + 1;
}
//------------------------------------------------------------------------------------------------------------------
int Storage::WriteBits (char* acBuffer, int& riBitRatio, int iBitQuantity, int iValue)
{
    assert( 0 <= riBitRatio && riBitRatio < 8 && 0 <= iBitQuantity && iBitQuantity <= 32
        &&  (iBitQuantity == 32 || ((-1 << iBitQuantity) & iValue) == 0
        ||  ((-1 << iBitQuantity) & iValue) == (-1 << iBitQuantity)) );

    if ( riBitRatio == 0 && iBitQuantity == 8 )
    {
        // special case
        *acBuffer = iValue;
        return 1;
    }

    int iBitSum = riBitRatio + iBitQuantity;
    int iBytes = iBitSum >> 3;
    int iNewBitRatio = iBitSum & 7;
    int iTop = 8 - riBitRatio;

    // clear bits for further layering
    if ( iBitSum < 8 )
    {
        int iOff = iTop - iBitQuantity;
        *acBuffer &= ~((255 >> iOff << iOff) & ~(255 >> iTop << iTop));
    }
    else
    {
        *acBuffer = (char)(*acBuffer >> iTop << iTop);
    }
    if ( iNewBitRatio != 0 && iBytes != 0 )
    {
        int iOff = 8 - iNewBitRatio;
        acBuffer[iBytes] &= ~(255 >> iOff << iOff);
    }

    // in the first byte
    if ( iBitSum < 8 )
    {
        *acBuffer |= (iValue << (8 - iBitSum)) & ~(255 >> iTop << iTop);
        riBitRatio = iNewBitRatio;
        return 0;
    }
    else
    {
        *acBuffer |= (iValue >> (iBitQuantity - iTop)) & ~(255 >> iTop << iTop);
    }

    // in inner bytes
    for (int i0 = 1, i1 = 2; i1 <= iBytes; i0 = i1++)
        acBuffer[i0] = (char)(iValue >> (((iBytes - i1) << 3) + iNewBitRatio));

    // in the last byte
    if ( iNewBitRatio != 0 && iBytes != 0 )
        acBuffer[iBytes] |= iValue << (8 - iNewBitRatio);

    riBitRatio = iNewBitRatio;
    return iBytes;
}
//------------------------------------------------------------------------------------------------------------------
int Storage::ReadUBits (const char* acBuffer, int& riBitRatio, int iBitQuantity, int& riValue)
{
    assert( 0 <= riBitRatio && riBitRatio < 8 && 0 <= iBitQuantity && iBitQuantity <= 32 );

    unsigned char* aucBuffer = (unsigned char*)acBuffer;

    if ( riBitRatio == 0 && iBitQuantity == 8 )
    {
        // special case
        riValue = *aucBuffer;
        return 1;
    }

    int iBitSum = riBitRatio + iBitQuantity;
    int iBytes = iBitSum >> 3;
    int iNewBitRatio = iBitSum & 7;
    int iTop = 8 - riBitRatio;

    // in the first byte
    if ( iBitSum < 8 )
    {
        riValue = (*aucBuffer >> (iTop - iBitQuantity)) & ~(255 >> iBitQuantity << iBitQuantity);
        riBitRatio = iNewBitRatio;
        return 0;
    }
    else
    {
        riValue = 0;
        riValue |= (*aucBuffer & ~(255 >> iTop << iTop)) << (iBitQuantity - iTop);
    }

    // in inner bytes
    if ( iBytes != 0 )
    {
        int iTopDiff = iBitQuantity - iTop;
        for (int i = 1; i < iBytes; i++)
            riValue |= aucBuffer[i] << (iTopDiff - (i << 3));
    }

    // in the last byte
    if ( iNewBitRatio != 0 && iBytes != 0 )
        riValue |= (aucBuffer[iBytes] >> (8 - iNewBitRatio)) & ~(255 >> iNewBitRatio << iNewBitRatio);

    riBitRatio = iNewBitRatio;
    return iBytes;
}
//------------------------------------------------------------------------------------------------------------------
int Storage::ReadSBits (const char* acBuffer, int& riBitRatio, int iBitQuantity, int& riValue)
{
    int iBytes = ReadUBits(acBuffer,riBitRatio,iBitQuantity,riValue);

    // expand the sign bit
    int iBottom = 32 - iBitQuantity;
    riValue = (riValue << iBottom) >> iBottom;

    return iBytes;
}
//------------------------------------------------------------------------------------------------------------------
bool Storage::FileExists (const char* acFilename)
{
    FILE* pqFile = fopen(acFilename,"r");
    if ( pqFile )
    {
        fclose(pqFile);
        return true;
    }
    return false;
}
//------------------------------------------------------------------------------------------------------------------
bool Storage::GetFileSize (int& riFileSize, const char* acFilename)
{
    FILE* pqFile = fopen(acFilename,"r");
    if ( !pqFile )
    {
        assert( false );
        fclose(pqFile);
        return false;
    }
    struct _stat qStat;
    if ( _fstat(pqFile->_file,&qStat) != 0 )
    {
        assert( false );
        fclose(pqFile);
        return false;
    }
    riFileSize = qStat.st_size;
    if ( fclose(pqFile) != 0 )
        assert( false );

    return true;
}
//------------------------------------------------------------------------------------------------------------------
unsigned int Storage::GetCrc32Checksum (const char* acByte, int iQuantity)
{
    assert( iQuantity >= 0 );

    unsigned int uiValue = 4294967295;
    for (int i = 0; i < iQuantity; i++)
        uiValue = gs_auiCrc32Table[(uiValue ^ acByte[i]) & 255] ^ (uiValue >> 8);
    return uiValue ^ 4294967295;
}
//------------------------------------------------------------------------------------------------------------------



