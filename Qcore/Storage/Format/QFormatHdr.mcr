//------------------------------------------------------------------------------------------------------------------
#define REACH_NEXT_STATEMENT \
{ \
    for (/**/; /**/; /**/) \
    { \
        Stream qByteStream(m_eStreaming,m_qFilename,iNextReadPos++,1,true,bSuccess); \
        C_PROCESS_STREAM_SUCCESS_FOR_LOADING(bSuccess); \
        qByteStream.Read(ucByte); \
        qByteStream.Finalize(); \
        if ( ucByte == '\n' ) \
            break; \
    } \
}
//------------------------------------------------------------------------------------------------------------------



