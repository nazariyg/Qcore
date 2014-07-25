#include "QLogable.h"
using namespace Q;

//------------------------------------------------------------------------------------------------------------------
Logable::Logable ()
    :
    m_bEnabled(true),
    m_bAutoHeadLogItems(true),
    m_iStartFlushingLogItem(0),
    m_bAutoFlushLog(false),
    m_oOnFatalErrorFunction(0)
{
}
//------------------------------------------------------------------------------------------------------------------
void Logable::SetLogFilename (const char* acFilename)
{
    m_qLogFilename = acFilename;
    assert( !m_qLogFilename.IsEmpty() );
    FILE* pqFile = fopen(acFilename,"w");
    if ( pqFile == 0 )
        LogError("Cannot open the log file");
    fclose(pqFile);
}
//------------------------------------------------------------------------------------------------------------------
void Logable::FlushLog ()
{
    if ( m_bEnabled )
    {
        assert( !m_qLogFilename.IsEmpty() );
        FILE* pqFile = fopen(m_qLogFilename,"a");
        if ( pqFile == 0 )
        {
            LogError("Cannot open the log file");
            return;
        }
        for (int i = m_iStartFlushingLogItem; i < m_qLog.GetQuantity(); i++)
        {
            if ( i != 0 )
                fputs("\n",pqFile);
            fputs(m_qLog[i].Matter,pqFile);
        }

        fclose(pqFile);
        m_iStartFlushingLogItem = m_qLog.GetQuantity();
    }
}
//------------------------------------------------------------------------------------------------------------------
void Logable::LogInfo (const char* acMatter)
{
    if ( m_bEnabled )
        m_qLog.Push(LogItem(LogItem::LIT_INFO,acMatter,false));

    if ( m_bAutoFlushLog )
        FlushLog();
}
//------------------------------------------------------------------------------------------------------------------
void Logable::LogWarning (const char* acMatter)
{
    if ( m_bEnabled )
    {
        if ( m_bAutoHeadLogItems )
            m_qLog.Push(LogItem(LogItem::LIT_WARNING,String("WARNING: ")+acMatter,false));
        else
            m_qLog.Push(LogItem(LogItem::LIT_WARNING,acMatter,false));
    }

    if ( m_bAutoFlushLog )
        FlushLog();
}
//------------------------------------------------------------------------------------------------------------------
void Logable::LogError (const char* acMatter)
{
    if ( m_bEnabled )
    {
        if ( m_bAutoHeadLogItems )
            m_qLog.Push(LogItem(LogItem::LIT_ERROR,String("ERROR: ")+acMatter,false));
        else
            m_qLog.Push(LogItem(LogItem::LIT_ERROR,acMatter,false));
    }

    if ( m_bAutoFlushLog )
        FlushLog();
}
//------------------------------------------------------------------------------------------------------------------
void Logable::LogAndThrowFatalError (const char* acMatter)
{
    if ( m_bEnabled )
    {
        if ( m_bAutoHeadLogItems )
            m_qLog.Push(LogItem(LogItem::LIT_ERROR,String("FATAL ERROR: ")+acMatter,true));
        else
            m_qLog.Push(LogItem(LogItem::LIT_ERROR,acMatter,true));
    }

    if ( m_bAutoFlushLog )
        FlushLog();

    if ( m_oOnFatalErrorFunction )
        m_oOnFatalErrorFunction(acMatter);
}
//------------------------------------------------------------------------------------------------------------------
void Logable::LogToLastItem (const char* acMatter, bool bWhiteSpace)
{
    if ( m_bEnabled )
    {
        assert( !m_qLog.IsEmpty() );
        String qMatter;
        if ( bWhiteSpace )
            qMatter = " ";
        qMatter += acMatter;
        m_qLog.GetLast().Matter += qMatter;

        if ( m_bAutoFlushLog )
        {
            assert( !m_qLogFilename.IsEmpty() );
            FILE* pqFile = fopen(m_qLogFilename,"a");
            if ( pqFile == 0 )
            {
                LogError("Cannot open the log file");
                return;
            }
            fputs(qMatter,pqFile);
            fclose(pqFile);
        }
    }
}
//------------------------------------------------------------------------------------------------------------------



