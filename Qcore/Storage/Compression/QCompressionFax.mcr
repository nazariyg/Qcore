//------------------------------------------------------------------------------------------------------------------
#define GET_NEXT_BIT(value,bytepos,bitratio,data,datasize) \
{ \
    if ( bytepos == datasize ) \
    { \
        assert( false ); \
        return false; \
    } \
    bytepos += Storage::ReadUBits(data+bytepos,bitratio,1,value); \
}
//------------------------------------------------------------------------------------------------------------------
#define UM_EDGE_CROSSING \
{ \
    if ( iRowPixelsRead == m_iWidth ) \
    { \
        qProgress.MakeAdvance(); \
 \
        if ( ++iRowsRead == m_iHeight ) \
            break; \
 \
        iRowPixelsRead = 0; \
        iRunQuantity = 0; \
        qA0.Position = 0; \
 \
        if ( !bDecImgRowsAreBytes ) \
        { \
            iBytePos1++; \
            iBitRatio1 = 0; \
        } \
    } \
}
//------------------------------------------------------------------------------------------------------------------



