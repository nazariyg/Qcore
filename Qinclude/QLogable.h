#ifndef QLOGABLE_H
#define QLOGABLE_H

#include "QString.h"

namespace Q
{

class Q_ITEM Logable
{
protected:
    // construction
    Logable ();

public:
    // enable/disable both types of logging
    void EnableLogging (bool bEnable);
    bool IsLoggingEnabled () const;

    // Setting of log filename enables feeding into the file additionaly to workings memory; initially
    // creates/clears the file.
    void SetLogFilename (const char* acFilename);
    const String& GetLogFilename () const;

    // auto-heading
    void EnableAutoHeadLogItems (bool bEnable);
    bool IsAutoHeadLogItemsEnabled () const;

    // only unflushed before log items flush into the file
    void FlushLog ();

    // if enabled, messages issued by a derivative class flush into the file immediately as appear
    void EnableAutoFlushLog (bool bEnable);
    bool GetAutoFlushLog () const;

    // what to do on a fatal error
    void SetOnFatalErrorFunction (void (*oOnFatalErrorFunction)(const char* acMatter));

protected:
    // to be used by derivatives
    void LogInfo (const char* acMatter);
    void LogWarning (const char* acMatter);
    void LogError (const char* acMatter);
    void LogAndThrowFatalError (const char* acMatter);
    void LogToLastItem (const char* acMatter, bool bWhiteSpace = true);

    char m_acLogTmp[1024];

private:
    class LogItem
    {
    public:
        enum LogItemType
        {
            LIT_INFO,
            LIT_WARNING,
            LIT_ERROR
        };

        LogItem () {};
        LogItem (LogItemType eType, const char* acMatter, bool bFatal)
            :
            Type(eType),
            Matter(acMatter),
            Fatal(bFatal)
        {
        };

        LogItemType Type;
        String Matter;
        bool Fatal;
    };

    bool m_bEnabled;  // default: true
    bool m_bAutoHeadLogItems;
    Array<LogItem> m_qLog;
    String m_qLogFilename;
    int m_iStartFlushingLogItem;
    bool m_bAutoFlushLog;  // default: false
    void (*m_oOnFatalErrorFunction)(const char* acMatter);
};

#include "QLogable.inl"

}

#endif



