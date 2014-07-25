//------------------------------------------------------------------------------------------------------------------
#define C_RETURN_FALSE \
{ \
    rbSuccess = false; \
    return; \
}
//------------------------------------------------------------------------------------------------------------------
#define C_THROW_EXCEPTION_WITHOUT_ASSERT_FALSE(exception) \
{ \
    OnException(exception); \
    C_RETURN_FALSE; \
}
//------------------------------------------------------------------------------------------------------------------
#define C_CONDITIONAL_THROW_EXCEPTION_WITHOUT_ASSERT_FALSE(expression,exception) \
{ \
    if ( !expression ) \
        C_THROW_EXCEPTION_WITHOUT_ASSERT_FALSE(exception); \
}
//------------------------------------------------------------------------------------------------------------------
#define C_THROW_EXCEPTION(exception) \
{ \
    assert( false ); \
    C_THROW_EXCEPTION_WITHOUT_ASSERT_FALSE(exception); \
}
//------------------------------------------------------------------------------------------------------------------
#define C_CONDITIONAL_THROW_EXCEPTION(expression,exception) \
{ \
    if ( !expression ) \
        C_THROW_EXCEPTION(exception); \
}
//------------------------------------------------------------------------------------------------------------------
#define THROW_EXCEPTION_WITHOUT_ASSERT_FALSE(exception) \
{ \
    OnException(exception); \
    return false; \
}
//------------------------------------------------------------------------------------------------------------------
#define CONDITIONAL_THROW_EXCEPTION_WITHOUT_ASSERT_FALSE(expression,exception) \
{ \
    if ( !expression ) \
        THROW_EXCEPTION_WITHOUT_ASSERT_FALSE(exception); \
}
//------------------------------------------------------------------------------------------------------------------
#define THROW_EXCEPTION(exception) \
{ \
    assert( false ); \
    THROW_EXCEPTION_WITHOUT_ASSERT_FALSE(exception); \
}
//------------------------------------------------------------------------------------------------------------------
#define CONDITIONAL_THROW_EXCEPTION(expression,exception) \
{ \
    if ( !expression ) \
        THROW_EXCEPTION(exception); \
}
//------------------------------------------------------------------------------------------------------------------
#define THROW_WARNING_WITHOUT_ASSERT_FALSE(warning) \
{ \
    OnWarning(warning); \
}
//------------------------------------------------------------------------------------------------------------------
#define CONDITIONAL_THROW_WARNING_WITHOUT_ASSERT_FALSE(expression,warning) \
{ \
    if ( !expression ) \
        THROW_WARNING_WITHOUT_ASSERT_FALSE(warning); \
}
//------------------------------------------------------------------------------------------------------------------
#define THROW_WARNING(warning) \
{ \
    assert( false ); \
    THROW_WARNING_WITHOUT_ASSERT_FALSE(warning); \
}
//------------------------------------------------------------------------------------------------------------------
#define CONDITIONAL_THROW_WARNING(expression,warning) \
{ \
    if ( !expression ) \
        THROW_WARNING(warning); \
}
//------------------------------------------------------------------------------------------------------------------



