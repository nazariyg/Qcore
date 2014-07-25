#include "QFormatBase.h"
using namespace Q;

//------------------------------------------------------------------------------------------------------------------
FormatBase::FormatBase ()
    :
    m_pqFile(0)
{
}
//------------------------------------------------------------------------------------------------------------------
FormatBase::FormatBase (const char* acFilename)
    :
    m_eStreaming(Stream::ST_READING_FROM_FILE),
    m_qFilename(acFilename)
{
    m_acStreamSrcParam = m_qFilename;

    // lock the file
    m_pqFile = fopen(acFilename,"rb");
    assert( m_pqFile );
}
//------------------------------------------------------------------------------------------------------------------
FormatBase::FormatBase (bool bBytestreamIsStatic, const char* acBytestream, int iStaticBytestreamSize)
    :
    m_acBytestream(acBytestream),
    m_iStaticBytestreamSize(iStaticBytestreamSize),
    m_acStreamSrcParam(acBytestream),
    m_pqFile(0)
{
    assert( iStaticBytestreamSize == -1 || iStaticBytestreamSize >= 0 );

    if ( bBytestreamIsStatic )
        m_eStreaming = Stream::ST_READING_FROM_STATIC_BYTESTREAM;
    else
        m_eStreaming = Stream::ST_READING_FROM_DYNAMIC_BYTESTREAM;
}
//------------------------------------------------------------------------------------------------------------------
FormatBase::~FormatBase ()
{
    if ( m_pqFile )
    {
        // unlock the file
        fclose(m_pqFile);
    }
}
//------------------------------------------------------------------------------------------------------------------
void FormatBase::OnException (int iExceptionType)
{
    ExptWarnBase::OnException(iExceptionType);

    if ( m_pqFile )
    {
        // unlock the file
        if ( fclose(m_pqFile) != 0 )
            assert( false );
    }
}
//------------------------------------------------------------------------------------------------------------------



