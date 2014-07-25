//------------------------------------------------------------------------------------------------------------------
#define TEST_EXTENSION_AS(fmt) \
{ \
    String qExtSet##fmt(gs_acExtSet##fmt); \
    Array<String> qExtArray##fmt; \
    qExtSet##fmt.GetSplitsByChar(' ',qExtArray##fmt); \
    for (int i = 0; i < qExtArray##fmt.GetQuantity(); i++) \
    { \
        if ( qExt == qExtArray##fmt[i] ) \
        { \
            TRY_TO_INIT_AS(##fmt); \
            break; \
        } \
    } \
}
//------------------------------------------------------------------------------------------------------------------
#define TRY_TO_INIT_AS(fmt) \
{ \
    if ( InitAs##fmt() ) \
    { \
        rbSuccess = true; \
        return; \
    } \
}
//------------------------------------------------------------------------------------------------------------------
#define LOAD_IMAGE(fmt,loadimagecall) \
{ \
    Format##fmt q##fmt(m_qFilename,bSuccess); \
    q##fmt.RedirectProgressTo(this,qProgress,0.5f); \
    CONDITIONAL_THROW_EXCEPTION_WITHOUT_ASSERT_FALSE(q##fmt.##loadimagecall,ET_FORMAT_MISMATCH); \
}
//------------------------------------------------------------------------------------------------------------------



