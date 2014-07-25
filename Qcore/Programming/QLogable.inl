//------------------------------------------------------------------------------------------------------------------
inline void Logable::EnableLogging (bool bEnable)
{
    m_bEnabled = bEnable;
}
//------------------------------------------------------------------------------------------------------------------
inline bool Logable::IsLoggingEnabled () const
{
    return m_bEnabled;
}
//------------------------------------------------------------------------------------------------------------------
inline void Logable::EnableAutoHeadLogItems (bool bEnable)
{
    m_bAutoHeadLogItems = bEnable;
}
//------------------------------------------------------------------------------------------------------------------
inline bool Logable::IsAutoHeadLogItemsEnabled () const
{
    return m_bAutoHeadLogItems;
}
//------------------------------------------------------------------------------------------------------------------
inline const String& Logable::GetLogFilename () const
{
    return m_qLogFilename;
}
//------------------------------------------------------------------------------------------------------------------
inline void Logable::EnableAutoFlushLog (bool bEnable)
{
    m_bAutoFlushLog = bEnable;
}
//------------------------------------------------------------------------------------------------------------------
inline bool Logable::GetAutoFlushLog () const
{
    return m_bAutoFlushLog;
}
//------------------------------------------------------------------------------------------------------------------
inline void Logable::SetOnFatalErrorFunction (void (*oOnFatalErrorFunction)(const char* acMatter))
{
    m_oOnFatalErrorFunction = oOnFatalErrorFunction;
}
//------------------------------------------------------------------------------------------------------------------



