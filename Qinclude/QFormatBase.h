#ifndef QFORMATBASE_H
#define QFORMATBASE_H

// The base class for all format classes.

#include "QString.h"
#include "QExptWarnBase.h"
#include "QHeap.h"

namespace Q
{

class Q_ITEM FormatBase : public ExptWarnBase, public Heap
{
protected:
    // construction
    FormatBase ();  // for saving (m_eStreaming is to be uninitialized)
    FormatBase (const char* acFilename);  // for loading
    FormatBase (bool bBytestreamIsStatic, const char* acBytestream, int iStaticBytestreamSize);  // for loading

    // destruction
    ~FormatBase ();

    virtual void OnException (int iExceptionType);

    Stream::StreamingType m_eStreaming;
    String m_qFilename;
    FILE* m_pqFile;
    const char* m_acBytestream;
    int m_iStaticBytestreamSize;
    const char* m_acStreamSrcParam;
};

}

#endif



