//------------------------------------------------------------------------------------------------------------------
#define ON_READCODE_FAIL(restorepointid) \
{ \
    if ( !m_bDynamicalDecompressionMode ) \
    { \
        if ( m_eEncoding != ET_GIF ) \
            assert( false ); \
        bSuccess = false; \
    } \
    else \
    { \
        bReadCodeFailedForDyna = true; \
        m_iRestorePointId = restorepointid; \
    } \
    goto Exit; \
}
//------------------------------------------------------------------------------------------------------------------
#define ON_CRITICAL_CHECK_FAIL \
{ \
    if ( m_eEncoding != ET_GIF ) \
        assert( false ); \
    bSuccess = false; \
    goto Exit; \
}
//------------------------------------------------------------------------------------------------------------------



