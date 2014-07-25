#include "QCompressionZLib.h"
#include "QCompressionZLib.mcr"
using namespace Q;

// support for packing
const float CompressionZLib::ms_fCompressionBufferAdvanceForDefaultCompressionStrategy(0.001f);
const float CompressionZLib::ms_fCompressionBufferAdvanceForNonDefaultCompressionStrategy(0.05f);

// support for unpacking
const int CompressionZLib::ms_iKBytesInDecompressionBuffer(128);

//------------------------------------------------------------------------------------------------------------------
CompressionZLib::CompressionZLib ()
    :
    m_fSpeedSizeBalance(1.0f),
    m_eCompressionStrategy(CST_ZLIB_DEFAULT),
    m_bZAlloc(false),
    m_bDynamicalDecompressionMode(false),
    m_bHasDecompressionStateRecord(false)
{
}
//------------------------------------------------------------------------------------------------------------------
CompressionZLib::~CompressionZLib ()
{
    if ( m_bZAlloc )
    {
        if ( inflateEnd(&m_qZStream) != Z_OK )
            assert( false );
    }
}
//------------------------------------------------------------------------------------------------------------------
void CompressionZLib::Compress (const char* acByte, int iByteQuantity, StillArray<char>& rqNewBytes) const
{
    rqNewBytes.RemoveAll();

    if ( m_eCompressionStrategy != CST_BEST_AMONG_4 )
    {
        CompressWithStrategy(acByte,iByteQuantity,rqNewBytes,m_fSpeedSizeBalance,m_eCompressionStrategy);
    }
    else
    {
        // contest between strategies
        int iMinNewByteQuantity;
        CompressionStrategyType eStrategy, eBestStrategy;
        for (int i = 0; i < 4; i++)
        {
            eStrategy = (CompressionStrategyType)i;
            StillArray<char> qTestNewBytes;
            CompressWithStrategy(acByte,iByteQuantity,qTestNewBytes,m_fSpeedSizeBalance,eStrategy);
            if ( i == 0 || qTestNewBytes.GetQuantity() < iMinNewByteQuantity )
            {
                iMinNewByteQuantity = qTestNewBytes.GetQuantity();
                eBestStrategy = eStrategy;
            }
        }

        CompressWithStrategy(acByte,iByteQuantity,rqNewBytes,m_fSpeedSizeBalance,eBestStrategy);
    }
}
//------------------------------------------------------------------------------------------------------------------
bool CompressionZLib::Decompress (const char* acByte, int iByteQuantity, StillArray<char>& rqNewBytes)
{
    assert( iByteQuantity >= 0 );

    if ( !m_bHasDecompressionStateRecord )
        rqNewBytes.RemoveAll();

    int iBufferSize = ms_iKBytesInDecompressionBuffer*1024;
    if ( m_qDecompressionBuffer.IsEmpty() )
        m_qDecompressionBuffer.SetQuantity(iBufferSize);

    if ( !m_bHasDecompressionStateRecord )
    {
        m_qZStream.next_in = (Bytef*)acByte;
        m_qZStream.avail_in = iByteQuantity;
        m_qZStream.next_out = (Bytef*)m_qDecompressionBuffer.GetElements();
        m_qZStream.avail_out = iBufferSize;
        m_qZStream.zalloc = Z_NULL;
        m_qZStream.zfree = Z_NULL;
        m_qZStream.opaque = Z_NULL;

        if ( inflateInit(&m_qZStream) != Z_OK )
        {
            assert( false );
            return false;
        }
        m_bZAlloc = true;
    }
    else
    {
        // restore the state
        m_qZStream = m_qDSR.ZStream;
        m_qZStream.next_in = (Bytef*)(acByte + m_qDSR.IntNextIn);
        m_qZStream.avail_in = iByteQuantity - m_qDSR.IntNextIn;
        m_qZStream.next_out = (Bytef*)(m_qDecompressionBuffer + m_qDSR.IntNextOut);
    }

    int iRet, iAdvance, iPrevQ, iBuffAdv;
    while ( m_qZStream.avail_in != 0 )
    {
        iBuffAdv = 0;
        for (/**/; /**/; /**/)
        {
            iRet = inflate(&m_qZStream,Z_NO_FLUSH);

            if ( iRet != Z_BUF_ERROR )
            {
                break;
            }
            else
            {
                if ( m_qZStream.avail_out != 0 )
                    TERMINATE_DECOMPRESSION;
                m_qDecompressionBuffer.AdvanceQuantity(1);
                m_qZStream.next_out = (Bytef*)(m_qDecompressionBuffer + m_qDecompressionBuffer.GetQuantity() - 1);
                m_qZStream.avail_out++;
                iBuffAdv++;
            }
        }

        if ( iRet != Z_OK && iRet != Z_STREAM_END )
            TERMINATE_DECOMPRESSION;

        if ( m_qZStream.avail_out == 0 || iRet == Z_OK || iRet == Z_STREAM_END )
        {
            if ( !m_bHasDecompressionStateRecord )
            {
                iAdvance = m_qDecompressionBuffer.GetQuantity() - m_qZStream.avail_out;
                iPrevQ = rqNewBytes.GetQuantity();
                rqNewBytes.AdvanceQuantity(iAdvance);
                MemCpy(rqNewBytes+iPrevQ,m_qDecompressionBuffer,iAdvance);
            }
            else
            {
                iAdvance = m_qDSR.ZStream.avail_out + iBuffAdv - m_qZStream.avail_out;
                iPrevQ = rqNewBytes.GetQuantity();
                rqNewBytes.AdvanceQuantity(iAdvance);
                MemCpy(rqNewBytes+iPrevQ,m_qDecompressionBuffer+(iBufferSize-m_qDSR.ZStream.avail_out),iAdvance);
            }

            if ( iBuffAdv != 0 )
            {
                m_qDecompressionBuffer.SetQuantity(iBufferSize);
                m_qZStream.next_out = (Bytef*)(m_qDecompressionBuffer + iBufferSize);
            }

            if ( m_bDynamicalDecompressionMode && m_qZStream.avail_out != 0 && iRet == Z_OK )
                break;

            if ( iRet == Z_STREAM_END )
            {
                if ( inflateEnd(&m_qZStream) != Z_OK )
                    assert( false );
                m_bZAlloc = false;
                m_qDecompressionBuffer.RemoveAll();
                break;
            }
            else
            {
                m_qZStream.next_out = (Bytef*)m_qDecompressionBuffer.GetElements();
                m_qZStream.avail_out = iBufferSize;

                if ( m_bDynamicalDecompressionMode )
                    SAVE_DECOMPRESSION_STATE;
            }
        }
    }

    if ( m_bDynamicalDecompressionMode )
        SAVE_DECOMPRESSION_STATE;

    return true;
}
//------------------------------------------------------------------------------------------------------------------
void CompressionZLib::CompressWithStrategy (const char* acByte, int iByteQuantity, StillArray<char>& rqNewBytes,
    float fSpeedSizeBalance, CompressionStrategyType eStrategy)
{
    float fBufferAdvance =
        ( eStrategy == CST_ZLIB_DEFAULT ? ms_fCompressionBufferAdvanceForDefaultCompressionStrategy :
        ms_fCompressionBufferAdvanceForNonDefaultCompressionStrategy );
    int iBufferSize = (int)Mathf::Ceil((1.0f+fBufferAdvance)*iByteQuantity) + 12;
    int iLevel = 1 + Mathf::RoundToInt(fSpeedSizeBalance*8);
    int iStrategy;
    switch ( eStrategy )
    {
    case CST_ZLIB_DEFAULT:
        iStrategy = Z_DEFAULT_STRATEGY;
        break;
    case CST_FILTERED:
        iStrategy = Z_FILTERED;
        break;
    case CST_HUFFMAN:
        iStrategy = Z_HUFFMAN_ONLY;
        break;
    case CST_RLE:
        iStrategy = Z_RLE;
        break;
    case CST_FIXED:
        iStrategy = Z_FIXED;
        break;
    }
    rqNewBytes.SetQuantity(iBufferSize,false);

    Bytef* dest = (Bytef*)rqNewBytes.GetElements();
    uLongf destLenV = iBufferSize;
    uLongf* destLen = &destLenV;
    const Bytef* source = (const Bytef*)acByte;
    uLong sourceLen = iByteQuantity;
    int level = iLevel;
    int strategy = iStrategy;

    z_stream stream;
    int err;
    stream.next_in = (Bytef*)source;
    stream.avail_in = (uInt)sourceLen;
#ifdef MAXSEG_64K
    if ( (uLong)stream.avail_in != sourceLen )
    {
        assert( false );
        return;
    }
#endif
    stream.next_out = dest;
    stream.avail_out = (uInt)*destLen;
    if ( (uLong)stream.avail_out != *destLen )
    {
        assert( false );
        return;
    }
    stream.zalloc = (alloc_func)0;
    stream.zfree = (free_func)0;
    stream.opaque = (voidpf)0;
    err = deflateInit2(&stream,level,Z_DEFLATED,15,8,strategy);
    if ( err != Z_OK )
    {
        assert( false );
        return;
    }
    err = deflate(&stream,Z_FINISH);
    if ( err != Z_STREAM_END )
    {
        deflateEnd(&stream);
        assert( false );
        return;
    }
    *destLen = stream.total_out;
    err = deflateEnd(&stream);
    if ( err != Z_OK )
    {
        assert( false );
        return;
    }

    rqNewBytes.SetQuantity(*destLen);
}
//------------------------------------------------------------------------------------------------------------------



