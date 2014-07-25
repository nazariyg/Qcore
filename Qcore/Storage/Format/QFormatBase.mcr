//------------------------------------------------------------------------------------------------------------------
#define C_RETURN_ON_FAIL(expression) \
{ \
    if ( !expression ) \
    { \
        if ( m_eStreaming != Stream::ST_READING_FROM_DYNAMIC_BYTESTREAM ) \
        { \
            assert( false ); \
            OnException(ET_FORMAT_MISMATCH); \
        } \
        C_RETURN_FALSE; \
    } \
}
//------------------------------------------------------------------------------------------------------------------
#define C_PROCESS_STREAM_SUCCESS_FOR_LOADING(success) \
{ \
    if ( !success ) \
    { \
        if ( m_eStreaming != Stream::ST_READING_FROM_DYNAMIC_BYTESTREAM ) \
            C_THROW_EXCEPTION(ET_FORMAT_MISMATCH) \
        else \
            C_RETURN_FALSE; \
    } \
}
//------------------------------------------------------------------------------------------------------------------
#define RETURN_ON_FAIL(expression) \
{ \
    if ( !expression ) \
    { \
        if ( m_eStreaming != Stream::ST_READING_FROM_DYNAMIC_BYTESTREAM ) \
        { \
            assert( false ); \
            OnException(ET_FORMAT_MISMATCH); \
        } \
        return false; \
    } \
}
//------------------------------------------------------------------------------------------------------------------
#define PROCESS_STREAM_SUCCESS_FOR_LOADING(success) \
{ \
    if ( !success ) \
    { \
        if ( m_eStreaming != Stream::ST_READING_FROM_DYNAMIC_BYTESTREAM ) \
            THROW_EXCEPTION(ET_FORMAT_MISMATCH) \
        else \
            return false; \
    } \
}
//------------------------------------------------------------------------------------------------------------------
#define LEVEL_LOW_PROCESS_STREAM_SUCCESS_FOR_LOADING(success) \
{ \
    if ( !success ) \
    { \
        if ( m_eStreaming != Stream::ST_READING_FROM_DYNAMIC_BYTESTREAM ) \
        { \
            assert( false ); \
            OnException(ET_FORMAT_MISMATCH); \
            return 1; \
        } \
        else \
        { \
            return -1; \
        } \
    } \
}
//------------------------------------------------------------------------------------------------------------------
#define RETURN_NEG_ONE_ON_FAIL(expression) \
{ \
    if ( !expression ) \
        return -1; \
}
//------------------------------------------------------------------------------------------------------------------
#define RETURN_POS_ONE_ON_FAIL(expression) \
{ \
    if ( !expression ) \
        return 1; \
}
//------------------------------------------------------------------------------------------------------------------
#define PROCESS_INT_SUCCESS_LEVEL_LOW(success) \
{ \
    if ( success != 0 ) \
        return success; \
}
//------------------------------------------------------------------------------------------------------------------
#define PROCESS_INT_SUCCESS_LEVEL_HIGH(success) \
{ \
    if ( success == -1 ) \
        return false; \
    if ( success == 1 ) \
        THROW_EXCEPTION(ET_FORMAT_MISMATCH); \
}
//------------------------------------------------------------------------------------------------------------------



