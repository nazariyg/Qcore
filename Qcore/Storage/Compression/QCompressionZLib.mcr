//------------------------------------------------------------------------------------------------------------------
#define TERMINATE_DECOMPRESSION \
{ \
    assert( false ); \
    if ( inflateEnd(&m_qZStream) != Z_OK ) \
        assert( false ); \
    m_bZAlloc = false; \
    m_qDecompressionBuffer.RemoveAll(); \
    return false; \
}
//------------------------------------------------------------------------------------------------------------------
#define SAVE_DECOMPRESSION_STATE \
{ \
    m_qDSR.ZStream = m_qZStream; \
    m_qDSR.IntNextIn = (int)(m_qZStream.next_in - (Bytef*)acByte); \
    m_qDSR.IntNextOut = (int)(m_qZStream.next_out - (Bytef*)m_qDecompressionBuffer.GetElements()); \
    m_bHasDecompressionStateRecord = true; \
}
//------------------------------------------------------------------------------------------------------------------



