#include "QStream.h"
#include "QStorage.h"
#include "QHashMap.h"
#include "QArray.h"
#include "QModestArray.h"
#include "QHeap.h"
using namespace Q;

typedef HashMap<unsigned int,void**> MapT;
typedef Array<Link<unsigned int,void*> > ArrayT;
typedef ModestArray<Link<int,int> > StackT;

// indicates how much the buffer's size is to grow above needed on reallocation caused by a lack of available
// bytes
const int Stream::ms_iAdvanceQuantityDivider(3);  // on 1/3 of actually required size

//------------------------------------------------------------------------------------------------------------------
Stream::Stream (const char* acFilename, bool bLittleEndian)
    :
    m_eStreaming(ST_WRITING_TO_FILE),
    m_eGeneralStreaming(GST_WRITING),
    m_bWholeFile(true),
    m_bFinalized(false),
    m_bFreeBufferAfterComplete(true),
    m_bLittleEndian(bLittleEndian),
    m_bOwner(true),
    m_iBytePosition(0),
    m_iBitRatio(0),
    m_pvIdToPPointerMap(0),
    m_pvIdWithPointerArray(0),
    m_pvPositionStack(new StackT)
{
    m_acFilename = new char[StrLen(acFilename)+1];
    StrCpy(m_acFilename,acFilename);

    // initially one byte in the buffer
    m_iByteQuantity = 1;
    m_acByte = new char[1];
}
//------------------------------------------------------------------------------------------------------------------
Stream::Stream (bool bLittleEndian)
    :
    m_eStreaming(ST_WRITING_TO_STATIC_BYTESTREAM),
    m_eGeneralStreaming(GST_WRITING),
    m_bFinalized(false),
    m_bFreeBufferAfterComplete(true),
    m_bLittleEndian(bLittleEndian),
    m_bOwner(true),
    m_iBytePosition(0),
    m_iBitRatio(0),
    m_acFilename(0),
    m_pvIdToPPointerMap(0),
    m_pvIdWithPointerArray(0),
    m_pvPositionStack(new StackT)
{
    // initially one byte in the buffer
    m_iByteQuantity = 1;
    m_acByte = new char[1];
}
//------------------------------------------------------------------------------------------------------------------
Stream::Stream (char* acStaticBytestream, int iStaticBytestreamSize, bool bLittleEndian)
    :
    m_eStreaming(ST_WRITING_TO_STATIC_BYTESTREAM),
    m_eGeneralStreaming(GST_WRITING),
    m_bFinalized(false),
    m_bLittleEndian(bLittleEndian),
    m_bOwner(false),
    m_iByteQuantity(iStaticBytestreamSize),
    m_iBytePosition(0),
    m_iBitRatio(0),
    m_acFilename(0),
    m_acByte(acStaticBytestream),
    m_pvIdToPPointerMap(0),
    m_pvIdWithPointerArray(0),
    m_pvPositionStack(new StackT)
{
    assert( iStaticBytestreamSize >= 0 );
}
//------------------------------------------------------------------------------------------------------------------
Stream::Stream (const char* acFilename, bool bLittleEndian, bool& rbSuccess)
    :
    m_eStreaming(ST_READING_FROM_FILE),
    m_eGeneralStreaming(GST_READING),
    m_bWholeFile(true),
    m_bFinalized(false),
    m_bFreeBufferAfterComplete(true),
    m_bLittleEndian(bLittleEndian),
    m_bOwner(true),
    m_iBytePosition(0),
    m_iBitRatio(0),
    m_pvIdToPPointerMap(new MapT),
    m_pvIdWithPointerArray(new ArrayT),
    m_pvPositionStack(new StackT)
{
    m_acFilename = new char[StrLen(acFilename)+1];
    StrCpy(m_acFilename,acFilename);

    // allocate appropriate amount of memory
    if ( !Storage::GetFileSize(m_iByteQuantity,acFilename) )
    {
        assert( false );
        rbSuccess = false;
        m_acByte = 0;
        return;
    }
    m_acByte = new char[m_iByteQuantity];

    // load data
    FILE* pqFile = fopen(acFilename,"rb");
    if ( !pqFile )
    {
        assert( false );
        rbSuccess = false;
        return;
    }
    int iRead = (int)fread(m_acByte,1,m_iByteQuantity,pqFile);
    if ( fclose(pqFile) != 0 || iRead != m_iByteQuantity )
    {
        assert( false );
        rbSuccess = false;
        return;
    }

    rbSuccess = true;
}
//------------------------------------------------------------------------------------------------------------------
Stream::Stream (StreamingType eStreaming, const char* acFilenameOrBytestream, int iOffset, int iByteQuantity,
    bool bLittleEndian, bool& rbSuccess, int iDynamicBytestreamSize)
    :
    m_eStreaming(eStreaming),
    m_bWholeFile(false),
    m_bFinalized(false),
    m_bFreeBufferAfterComplete(true),
    m_bLittleEndian(bLittleEndian),
    m_iBytePosition(0),
    m_iBitRatio(0),
    m_pvIdToPPointerMap(0),
    m_pvIdWithPointerArray(0),
    m_pvPositionStack(new StackT)
{
    assert( iOffset >= 0 && !(eStreaming != ST_READING_FROM_DYNAMIC_BYTESTREAM && iByteQuantity < 0) );
    assert( !(eStreaming == ST_READING_FROM_DYNAMIC_BYTESTREAM && iDynamicBytestreamSize < 0) );

    switch ( eStreaming )
    {
    case ST_WRITING_TO_FILE:
        m_eGeneralStreaming = GST_WRITING;
        break;
    case ST_WRITING_TO_STATIC_BYTESTREAM:
        m_eGeneralStreaming = GST_WRITING;
        break;
    case ST_READING_FROM_FILE:
        m_eGeneralStreaming = GST_READING;
        break;
    case ST_READING_FROM_STATIC_BYTESTREAM:
        m_eGeneralStreaming = GST_READING;
        break;
    case ST_READING_FROM_DYNAMIC_BYTESTREAM:
        m_eGeneralStreaming = GST_READING;
        break;
    }

    if ( eStreaming == ST_WRITING_TO_FILE || eStreaming == ST_READING_FROM_FILE )
    {
        // operating with a file
        m_bOwner = true;
        m_acFilename = new char[StrLen(acFilenameOrBytestream)+1];
        StrCpy(m_acFilename,acFilenameOrBytestream);
        m_acByte = new char[iByteQuantity];
    }
    else
    {
        // operating with a memory block
        m_bOwner = false;
        m_acFilename = 0;

        if ( eStreaming == ST_READING_FROM_DYNAMIC_BYTESTREAM
        &&   (iByteQuantity < 0 || iOffset + iByteQuantity > iDynamicBytestreamSize) )
        {
            // not enough of transmitted data
            rbSuccess = false;
            return;
        }

        m_acByte = ((char*)acFilenameOrBytestream) + iOffset;
    }
    m_iOffset = iOffset;
    m_iByteQuantity = iByteQuantity;

    if ( eStreaming == ST_READING_FROM_FILE )
    {
        // open the file
        FILE* pqFile = fopen(acFilenameOrBytestream,"rb");
        if ( !pqFile )
        {
            assert( false );
            rbSuccess = false;
            return;
        }

        // load data and close the file
        if ( iOffset != 0 )
        {
            if ( fseek(pqFile,iOffset,SEEK_SET) != 0 )
            {
                assert( false );
                fclose(pqFile);  // no assertion
                rbSuccess = false;
                return;
            }
        }
        int iRead = (int)fread(m_acByte,1,iByteQuantity,pqFile);
        if ( fclose(pqFile) != 0 || iRead != iByteQuantity )
        {
            assert( false );
            rbSuccess = false;
            return;
        }
    }

    if ( m_eGeneralStreaming == GST_READING )
    {
        m_pvIdToPPointerMap = new MapT;
        m_pvIdWithPointerArray = new ArrayT;
    }

    rbSuccess = true;
}
//------------------------------------------------------------------------------------------------------------------
bool Stream::Finalize ()
{
    assert( !m_bFinalized );

    AlignBytes();

    bool bSuccess = true;
    if ( m_eStreaming == ST_WRITING_TO_FILE )
    {
        // flush data into the file
        FILE* pqFile;
        if ( m_bWholeFile )
        {
            pqFile = fopen(m_acFilename,"wb");
            if ( !pqFile )
            {
                assert( false );
                return false;
            }
        }
        else
        {
            pqFile = fopen(m_acFilename,"r+b");
            if ( !pqFile )
            {
                assert( false );
                return false;
            }
            if ( fseek(pqFile,m_iOffset,SEEK_SET) != 0 )
            {
                assert( false );
                fclose(pqFile);  // no assertion
                return false;
            }
        }

        int iWrittenQuantity = (int)fwrite(m_acByte,1,m_iBytePosition,pqFile);
        if ( fclose(pqFile) != 0 || iWrittenQuantity != m_iBytePosition )
        {
            assert( false );
            return false;
        }
    }

    if ( m_eGeneralStreaming == GST_READING )
    {
        // restore smart pointers if any
        MapT* pqIdToPPointerMap = (MapT*)m_pvIdToPPointerMap;
        ArrayT* pqIdWithPointerArray = (ArrayT*)m_pvIdWithPointerArray;
        const Link<unsigned int,void*>* pLink;
        HashMap<unsigned int,void**>::IteratorT qIt;
        int iRestored = 0;
        for (int i = 0; i < pqIdWithPointerArray->GetQuantity(); i++)
        {
            pLink = &(*pqIdWithPointerArray)[i];
            qIt = pqIdToPPointerMap->Find(pLink->First);
            if ( qIt != pqIdToPPointerMap->GetEnd() )
            {
                *qIt->Second = pLink->Second;
                ((Heap*)pLink->Second)->IncrementReferences(qIt->Second);

                iRestored++;
            }
        }
        if ( iRestored != pqIdToPPointerMap->GetQuantity() )
            bSuccess = false;

        delete (MapT*)m_pvIdToPPointerMap;
        delete (ArrayT*)m_pvIdWithPointerArray;
        m_pvIdToPPointerMap = 0;
        m_pvIdWithPointerArray = 0;
    }

    if ( m_eStreaming == ST_WRITING_TO_FILE || m_eStreaming == ST_READING_FROM_FILE )
    {
        delete[] m_acFilename;
        m_acFilename = 0;
    }
    if ( m_bOwner && m_bFreeBufferAfterComplete )
    {
        delete[] m_acByte;
        m_acByte = 0;
    }
    assert( ((StackT*)m_pvPositionStack)->IsEmpty() );
    delete (StackT*)m_pvPositionStack;
    m_pvPositionStack = 0;

    m_bFinalized = true;
    assert( bSuccess );
    return bSuccess;
}
//------------------------------------------------------------------------------------------------------------------
Stream::~Stream ()
{
    if ( m_bOwner && m_bFreeBufferAfterComplete )
        delete[] m_acByte;
    delete[] m_acFilename;
    delete (MapT*)m_pvIdToPPointerMap;
    delete (ArrayT*)m_pvIdWithPointerArray;
    if ( m_pvPositionStack )
    {
        assert( ((StackT*)m_pvPositionStack)->IsEmpty() );
        delete (StackT*)m_pvPositionStack;
    }
}
//------------------------------------------------------------------------------------------------------------------
void Stream::PushPositionState ()
{
    ((StackT*)m_pvPositionStack)->Push(MakeLink(m_iBytePosition,m_iBitRatio));
}
//------------------------------------------------------------------------------------------------------------------
void Stream::PopPositionState ()
{
    StackT* pqPS = (StackT*)m_pvPositionStack;
    m_iBytePosition = pqPS->GetLast().First;
    m_iBitRatio = pqPS->GetLast().Second;
    pqPS->Pop();
}
//------------------------------------------------------------------------------------------------------------------
void Stream::WriteBits (int iValue, int iNumBits)
{
    assert( m_iBytePosition >= 0 );

    int iBitSum = m_iBitRatio + iNumBits;
    int iBSD8 = iBitSum/8;
    int iNewByteQuantity = m_iBytePosition + ( (iBitSum & 7) != 0 ? iBSD8 + 1 : iBSD8 );
    if ( iNewByteQuantity > m_iByteQuantity )
    {
        if ( m_bOwner )
            SetByteQuantity(iNewByteQuantity);
        else
            assert( false );
    }
    Storage::WriteBits(m_acByte+m_iBytePosition,m_iBitRatio,iNumBits,iValue);
    m_iBytePosition += iBSD8;
}
//------------------------------------------------------------------------------------------------------------------
bool Stream::ReadUBits (int& riValue, int iNumBits)
{
    assert( m_iBytePosition >= 0 );

    int iBitSum = m_iBitRatio + iNumBits;
    int iBSD8 = iBitSum/8;
    int iNewByteQuantity = m_iBytePosition + ( (iBitSum & 7) != 0 ? iBSD8 + 1 : iBSD8 );
    if ( iNewByteQuantity > m_iByteQuantity )
    {
        assert( m_eStreaming == ST_READING_FROM_DYNAMIC_BYTESTREAM );
        return false;
    }
    Storage::ReadUBits(m_acByte+m_iBytePosition,m_iBitRatio,iNumBits,riValue);
    m_iBytePosition += iBSD8;
    return true;
}
//------------------------------------------------------------------------------------------------------------------
bool Stream::ReadSBits (int& riValue, int iNumBits)
{
    assert( m_iBytePosition >= 0 );

    int iBitSum = m_iBitRatio + iNumBits;
    int iBSD8 = iBitSum/8;
    int iNewByteQuantity = m_iBytePosition + ( (iBitSum & 7) != 0 ? iBSD8 + 1 : iBSD8 );
    if ( iNewByteQuantity > m_iByteQuantity )
    {
        assert( m_eStreaming == ST_READING_FROM_DYNAMIC_BYTESTREAM );
        return false;
    }
    Storage::ReadSBits(m_acByte+m_iBytePosition,m_iBitRatio,iNumBits,riValue);
    m_iBytePosition += iBSD8;
    return true;
}
//------------------------------------------------------------------------------------------------------------------
void Stream::AlignBytes ()
{
    assert( m_iBytePosition >= 0 );

    if ( m_iBitRatio != 0 )
    {
        int iNewByteQuantity = m_iBytePosition + 1;
        if ( iNewByteQuantity > m_iByteQuantity )
        {
            if ( m_eGeneralStreaming == GST_WRITING && m_bOwner )
                SetByteQuantity(iNewByteQuantity);
            else if ( m_eStreaming != ST_READING_FROM_DYNAMIC_BYTESTREAM )
                assert( false );
        }
        m_iBytePosition = iNewByteQuantity;
        m_iBitRatio = 0;
    }
}
//------------------------------------------------------------------------------------------------------------------
void Stream::Write (bool bValue)
{
    assert( m_iBytePosition >= 0 && m_iBitRatio == 0 );

    int iNewByteQuantity = m_iBytePosition + 1;
    if ( iNewByteQuantity > m_iByteQuantity )
    {
        if ( m_bOwner )
            SetByteQuantity(iNewByteQuantity);
        else
            assert( false );
    }
    *(m_acByte + m_iBytePosition) = ( bValue ? 1 : 0 );
    m_iBytePosition = iNewByteQuantity;
}
//------------------------------------------------------------------------------------------------------------------
void Stream::Write (int iQuantity, const bool* abValue)
{
    assert( m_iBytePosition >= 0 && m_iBitRatio == 0 );

    assert( iQuantity >= 0 );

    int iNewByteQuantity = m_iBytePosition + iQuantity;
    if ( iNewByteQuantity > m_iByteQuantity )
    {
        if ( m_bOwner )
            SetByteQuantity(iNewByteQuantity);
        else
            assert( false );
    }
    char* pcByte = m_acByte + m_iBytePosition;
    const bool* pbValue = abValue;
    for (int i = 0; i < iQuantity; i++)
        *pcByte++ = ( *pbValue++ ? 1 : 0 );
    m_iBytePosition = iNewByteQuantity;
}
//------------------------------------------------------------------------------------------------------------------
void Stream::Write (char cValue)
{
    assert( m_iBytePosition >= 0 && m_iBitRatio == 0 );

    int iNewByteQuantity = m_iBytePosition + 1;
    if ( iNewByteQuantity > m_iByteQuantity )
    {
        if ( m_bOwner )
            SetByteQuantity(iNewByteQuantity);
        else
            assert( false );
    }
    *(m_acByte + m_iBytePosition) = cValue;
    m_iBytePosition = iNewByteQuantity;
}
//------------------------------------------------------------------------------------------------------------------
void Stream::Write (int iQuantity, const char* acValue)
{
    assert( m_iBytePosition >= 0 && m_iBitRatio == 0 );

    assert( iQuantity >= 0 );

    int iNewByteQuantity = m_iBytePosition + iQuantity;
    if ( iNewByteQuantity > m_iByteQuantity )
    {
        if ( m_bOwner )
            SetByteQuantity(iNewByteQuantity);
        else
            assert( false );
    }
    MemCpy(m_acByte+m_iBytePosition,acValue,iQuantity);
    m_iBytePosition = iNewByteQuantity;
}
//------------------------------------------------------------------------------------------------------------------
void Stream::Write (unsigned char ucValue)
{
    assert( m_iBytePosition >= 0 && m_iBitRatio == 0 );

    int iNewByteQuantity = m_iBytePosition + 1;
    if ( iNewByteQuantity > m_iByteQuantity )
    {
        if ( m_bOwner )
            SetByteQuantity(iNewByteQuantity);
        else
            assert( false );
    }
    *(m_acByte + m_iBytePosition) = ucValue;
    m_iBytePosition = iNewByteQuantity;
}
//------------------------------------------------------------------------------------------------------------------
void Stream::Write (int iQuantity, const unsigned char* aucValue)
{
    assert( m_iBytePosition >= 0 && m_iBitRatio == 0 );

    assert( iQuantity >= 0 );

    int iNewByteQuantity = m_iBytePosition + iQuantity;
    if ( iNewByteQuantity > m_iByteQuantity )
    {
        if ( m_bOwner )
            SetByteQuantity(iNewByteQuantity);
        else
            assert( false );
    }
    MemCpy(m_acByte+m_iBytePosition,aucValue,iQuantity);
    m_iBytePosition = iNewByteQuantity;
}
//------------------------------------------------------------------------------------------------------------------
void Stream::Write (short sValue)
{
    assert( m_iBytePosition >= 0 && m_iBitRatio == 0 );

    int iNewByteQuantity = m_iBytePosition + 2;
    if ( iNewByteQuantity > m_iByteQuantity )
    {
        if ( m_bOwner )
            SetByteQuantity(iNewByteQuantity);
        else
            assert( false );
    }
    if ( m_bLittleEndian )
        Storage::Write2le(m_acByte+m_iBytePosition,1,&sValue);
    else
        Storage::Write2be(m_acByte+m_iBytePosition,1,&sValue);
    m_iBytePosition = iNewByteQuantity;
}
//------------------------------------------------------------------------------------------------------------------
void Stream::Write (int iQuantity, const short* asValue)
{
    assert( m_iBytePosition >= 0 && m_iBitRatio == 0 );

    int iNewByteQuantity = m_iBytePosition + 2*iQuantity;
    if ( iNewByteQuantity > m_iByteQuantity )
    {
        if ( m_bOwner )
            SetByteQuantity(iNewByteQuantity);
        else
            assert( false );
    }
    if ( m_bLittleEndian )
        Storage::Write2le(m_acByte+m_iBytePosition,iQuantity,asValue);
    else
        Storage::Write2be(m_acByte+m_iBytePosition,iQuantity,asValue);
    m_iBytePosition = iNewByteQuantity;
}
//------------------------------------------------------------------------------------------------------------------
void Stream::Write (unsigned short usValue)
{
    assert( m_iBytePosition >= 0 && m_iBitRatio == 0 );

    int iNewByteQuantity = m_iBytePosition + 2;
    if ( iNewByteQuantity > m_iByteQuantity )
    {
        if ( m_bOwner )
            SetByteQuantity(iNewByteQuantity);
        else
            assert( false );
    }
    if ( m_bLittleEndian )
        Storage::Write2le(m_acByte+m_iBytePosition,1,&usValue);
    else
        Storage::Write2be(m_acByte+m_iBytePosition,1,&usValue);
    m_iBytePosition = iNewByteQuantity;
}
//------------------------------------------------------------------------------------------------------------------
void Stream::Write (int iQuantity, const unsigned short* ausValue)
{
    assert( m_iBytePosition >= 0 && m_iBitRatio == 0 );

    int iNewByteQuantity = m_iBytePosition + 2*iQuantity;
    if ( iNewByteQuantity > m_iByteQuantity )
    {
        if ( m_bOwner )
            SetByteQuantity(iNewByteQuantity);
        else
            assert( false );
    }
    if ( m_bLittleEndian )
        Storage::Write2le(m_acByte+m_iBytePosition,iQuantity,ausValue);
    else
        Storage::Write2be(m_acByte+m_iBytePosition,iQuantity,ausValue);
    m_iBytePosition = iNewByteQuantity;
}
//------------------------------------------------------------------------------------------------------------------
void Stream::Write (int iValue)
{
    assert( m_iBytePosition >= 0 && m_iBitRatio == 0 );

    int iNewByteQuantity = m_iBytePosition + 4;
    if ( iNewByteQuantity > m_iByteQuantity )
    {
        if ( m_bOwner )
            SetByteQuantity(iNewByteQuantity);
        else
            assert( false );
    }
    if ( m_bLittleEndian )
        Storage::Write4le(m_acByte+m_iBytePosition,1,&iValue);
    else
        Storage::Write4be(m_acByte+m_iBytePosition,1,&iValue);
    m_iBytePosition = iNewByteQuantity;
}
//------------------------------------------------------------------------------------------------------------------
void Stream::Write (int iQuantity, const int* aiValue)
{
    assert( m_iBytePosition >= 0 && m_iBitRatio == 0 );

    int iNewByteQuantity = m_iBytePosition + 4*iQuantity;
    if ( iNewByteQuantity > m_iByteQuantity )
    {
        if ( m_bOwner )
            SetByteQuantity(iNewByteQuantity);
        else
            assert( false );
    }
    if ( m_bLittleEndian )
        Storage::Write4le(m_acByte+m_iBytePosition,iQuantity,aiValue);
    else
        Storage::Write4be(m_acByte+m_iBytePosition,iQuantity,aiValue);
    m_iBytePosition = iNewByteQuantity;
}
//------------------------------------------------------------------------------------------------------------------
void Stream::Write (unsigned int uiValue)
{
    assert( m_iBytePosition >= 0 && m_iBitRatio == 0 );

    int iNewByteQuantity = m_iBytePosition + 4;
    if ( iNewByteQuantity > m_iByteQuantity )
    {
        if ( m_bOwner )
            SetByteQuantity(iNewByteQuantity);
        else
            assert( false );
    }
    if ( m_bLittleEndian )
        Storage::Write4le(m_acByte+m_iBytePosition,1,&uiValue);
    else
        Storage::Write4be(m_acByte+m_iBytePosition,1,&uiValue);
    m_iBytePosition = iNewByteQuantity;
}
//------------------------------------------------------------------------------------------------------------------
void Stream::Write (int iQuantity, const unsigned int* auiValue)
{
    assert( m_iBytePosition >= 0 && m_iBitRatio == 0 );

    int iNewByteQuantity = m_iBytePosition + 4*iQuantity;
    if ( iNewByteQuantity > m_iByteQuantity )
    {
        if ( m_bOwner )
            SetByteQuantity(iNewByteQuantity);
        else
            assert( false );
    }
    if ( m_bLittleEndian )
        Storage::Write4le(m_acByte+m_iBytePosition,iQuantity,auiValue);
    else
        Storage::Write4be(m_acByte+m_iBytePosition,iQuantity,auiValue);
    m_iBytePosition = iNewByteQuantity;
}
//------------------------------------------------------------------------------------------------------------------
void Stream::Write (long lValue)
{
    assert( m_iBytePosition >= 0 && m_iBitRatio == 0 );

    int iNewByteQuantity = m_iBytePosition + ( sizeof(void*) == 4 ? 4 : 8 );
    if ( iNewByteQuantity > m_iByteQuantity )
    {
        if ( m_bOwner )
            SetByteQuantity(iNewByteQuantity);
        else
            assert( false );
    }
    if ( sizeof(void*) == 4 )
    {
        if ( m_bLittleEndian )
            Storage::Write4le(m_acByte+m_iBytePosition,1,&lValue);
        else
            Storage::Write4be(m_acByte+m_iBytePosition,1,&lValue);
    }
    else
    {
        if ( m_bLittleEndian )
            Storage::Write8le(m_acByte+m_iBytePosition,1,&lValue);
        else
            Storage::Write8be(m_acByte+m_iBytePosition,1,&lValue);
    }
    m_iBytePosition = iNewByteQuantity;
}
//------------------------------------------------------------------------------------------------------------------
void Stream::Write (int iQuantity, const long* alValue)
{
    assert( m_iBytePosition >= 0 && m_iBitRatio == 0 );

    int iNewByteQuantity = m_iBytePosition + (iQuantity << ( sizeof(void*) == 4 ? 2 : 3 ));
    if ( iNewByteQuantity > m_iByteQuantity )
    {
        if ( m_bOwner )
            SetByteQuantity(iNewByteQuantity);
        else
            assert( false );
    }
    if ( sizeof(void*) == 4 )
    {
        if ( m_bLittleEndian )
            Storage::Write4le(m_acByte+m_iBytePosition,iQuantity,alValue);
        else
            Storage::Write4be(m_acByte+m_iBytePosition,iQuantity,alValue);
    }
    else
    {
        if ( m_bLittleEndian )
            Storage::Write8le(m_acByte+m_iBytePosition,iQuantity,alValue);
        else
            Storage::Write8be(m_acByte+m_iBytePosition,iQuantity,alValue);
    }
    m_iBytePosition = iNewByteQuantity;
}
//------------------------------------------------------------------------------------------------------------------
void Stream::Write (unsigned long ulValue)
{
    assert( m_iBytePosition >= 0 && m_iBitRatio == 0 );

    int iNewByteQuantity = m_iBytePosition + ( sizeof(void*) == 4 ? 4 : 8 );
    if ( iNewByteQuantity > m_iByteQuantity )
    {
        if ( m_bOwner )
            SetByteQuantity(iNewByteQuantity);
        else
            assert( false );
    }
    if ( sizeof(void*) == 4 )
    {
        if ( m_bLittleEndian )
            Storage::Write4le(m_acByte+m_iBytePosition,1,&ulValue);
        else
            Storage::Write4be(m_acByte+m_iBytePosition,1,&ulValue);
    }
    else
    {
        if ( m_bLittleEndian )
            Storage::Write8le(m_acByte+m_iBytePosition,1,&ulValue);
        else
            Storage::Write8be(m_acByte+m_iBytePosition,1,&ulValue);
    }
    m_iBytePosition = iNewByteQuantity;
}
//------------------------------------------------------------------------------------------------------------------
void Stream::Write (int iQuantity, const unsigned long* aulValue)
{
    assert( m_iBytePosition >= 0 && m_iBitRatio == 0 );

    int iNewByteQuantity = m_iBytePosition + (iQuantity << ( sizeof(void*) == 4 ? 2 : 3 ));
    if ( iNewByteQuantity > m_iByteQuantity )
    {
        if ( m_bOwner )
            SetByteQuantity(iNewByteQuantity);
        else
            assert( false );
    }
    if ( sizeof(void*) == 4 )
    {
        if ( m_bLittleEndian )
            Storage::Write4le(m_acByte+m_iBytePosition,iQuantity,aulValue);
        else
            Storage::Write4be(m_acByte+m_iBytePosition,iQuantity,aulValue);
    }
    else
    {
        if ( m_bLittleEndian )
            Storage::Write8le(m_acByte+m_iBytePosition,iQuantity,aulValue);
        else
            Storage::Write8be(m_acByte+m_iBytePosition,iQuantity,aulValue);
    }
    m_iBytePosition = iNewByteQuantity;
}
//------------------------------------------------------------------------------------------------------------------
void Stream::Write (Int64 iValue)
{
    assert( m_iBytePosition >= 0 && m_iBitRatio == 0 );

    int iNewByteQuantity = m_iBytePosition + 8;
    if ( iNewByteQuantity > m_iByteQuantity )
    {
        if ( m_bOwner )
            SetByteQuantity(iNewByteQuantity);
        else
            assert( false );
    }
    if ( m_bLittleEndian )
        Storage::Write8le(m_acByte+m_iBytePosition,1,&iValue);
    else
        Storage::Write8be(m_acByte+m_iBytePosition,1,&iValue);
    m_iBytePosition = iNewByteQuantity;
}
//------------------------------------------------------------------------------------------------------------------
void Stream::Write (int iQuantity, const Int64* aiValue)
{
    assert( m_iBytePosition >= 0 && m_iBitRatio == 0 );

    int iNewByteQuantity = m_iBytePosition + 8*iQuantity;
    if ( iNewByteQuantity > m_iByteQuantity )
    {
        if ( m_bOwner )
            SetByteQuantity(iNewByteQuantity);
        else
            assert( false );
    }
    if ( m_bLittleEndian )
        Storage::Write8le(m_acByte+m_iBytePosition,iQuantity,aiValue);
    else
        Storage::Write8be(m_acByte+m_iBytePosition,iQuantity,aiValue);
    m_iBytePosition = iNewByteQuantity;
}
//------------------------------------------------------------------------------------------------------------------
void Stream::Write (float fValue)
{
    assert( m_iBytePosition >= 0 && m_iBitRatio == 0 );

    int iNewByteQuantity = m_iBytePosition + 4;
    if ( iNewByteQuantity > m_iByteQuantity )
    {
        if ( m_bOwner )
            SetByteQuantity(iNewByteQuantity);
        else
            assert( false );
    }
    if ( m_bLittleEndian )
        Storage::Write4le(m_acByte+m_iBytePosition,1,&fValue);
    else
        Storage::Write4be(m_acByte+m_iBytePosition,1,&fValue);
    m_iBytePosition = iNewByteQuantity;
}
//------------------------------------------------------------------------------------------------------------------
void Stream::Write (int iQuantity, const float* afValue)
{
    assert( m_iBytePosition >= 0 && m_iBitRatio == 0 );

    int iNewByteQuantity = m_iBytePosition + 4*iQuantity;
    if ( iNewByteQuantity > m_iByteQuantity )
    {
        if ( m_bOwner )
            SetByteQuantity(iNewByteQuantity);
        else
            assert( false );
    }
    if ( m_bLittleEndian )
        Storage::Write4le(m_acByte+m_iBytePosition,iQuantity,afValue);
    else
        Storage::Write4be(m_acByte+m_iBytePosition,iQuantity,afValue);
    m_iBytePosition = iNewByteQuantity;
}
//------------------------------------------------------------------------------------------------------------------
void Stream::Write (double dValue)
{
    assert( m_iBytePosition >= 0 && m_iBitRatio == 0 );

    int iNewByteQuantity = m_iBytePosition + 8;
    if ( iNewByteQuantity > m_iByteQuantity )
    {
        if ( m_bOwner )
            SetByteQuantity(iNewByteQuantity);
        else
            assert( false );
    }
    if ( m_bLittleEndian )
        Storage::Write8le(m_acByte+m_iBytePosition,1,&dValue);
    else
        Storage::Write8be(m_acByte+m_iBytePosition,1,&dValue);
    m_iBytePosition = iNewByteQuantity;
}
//------------------------------------------------------------------------------------------------------------------
void Stream::Write (int iQuantity, const double* adValue)
{
    assert( m_iBytePosition >= 0 && m_iBitRatio == 0 );

    int iNewByteQuantity = m_iBytePosition + 8*iQuantity;
    if ( iNewByteQuantity > m_iByteQuantity )
    {
        if ( m_bOwner )
            SetByteQuantity(iNewByteQuantity);
        else
            assert( false );
    }
    if ( m_bLittleEndian )
        Storage::Write8le(m_acByte+m_iBytePosition,iQuantity,adValue);
    else
        Storage::Write8be(m_acByte+m_iBytePosition,iQuantity,adValue);
    m_iBytePosition = iNewByteQuantity;
}
//------------------------------------------------------------------------------------------------------------------
bool Stream::Read (bool& rbValue)
{
    assert( m_iBytePosition >= 0 && m_iBitRatio == 0 );

    int iNewByteQuantity = m_iBytePosition + 1;
    if ( iNewByteQuantity > m_iByteQuantity )
    {
        assert( m_eStreaming == ST_READING_FROM_DYNAMIC_BYTESTREAM );
        return false;
    }
    rbValue = ( *(m_acByte + m_iBytePosition) != 0 ? true : false );
    m_iBytePosition = iNewByteQuantity;
    return true;
}
//------------------------------------------------------------------------------------------------------------------
bool Stream::Read (int iQuantity, bool* abValue)
{
    assert( m_iBytePosition >= 0 && m_iBitRatio == 0 );

    assert( iQuantity >= 0 );

    int iNewByteQuantity = m_iBytePosition + iQuantity;
    if ( iNewByteQuantity > m_iByteQuantity )
    {
        assert( m_eStreaming == ST_READING_FROM_DYNAMIC_BYTESTREAM );
        return false;
    }
    char* pcByte = m_acByte + m_iBytePosition;
    bool* pbValue = abValue;
    for (int i = 0; i < iQuantity; i++)
        *pbValue++ = ( *pcByte++ != 0 ? true : false );
    m_iBytePosition = iNewByteQuantity;
    return true;
}
//------------------------------------------------------------------------------------------------------------------
bool Stream::Read (char& rcValue)
{
    assert( m_iBytePosition >= 0 && m_iBitRatio == 0 );

    int iNewByteQuantity = m_iBytePosition + 1;
    if ( iNewByteQuantity > m_iByteQuantity )
    {
        assert( m_eStreaming == ST_READING_FROM_DYNAMIC_BYTESTREAM );
        return false;
    }
    rcValue = *(m_acByte + m_iBytePosition);
    m_iBytePosition = iNewByteQuantity;
    return true;
}
//------------------------------------------------------------------------------------------------------------------
bool Stream::Read (int iQuantity, char* acValue)
{
    assert( m_iBytePosition >= 0 && m_iBitRatio == 0 );

    assert( iQuantity >= 0 );

    int iNewByteQuantity = m_iBytePosition + iQuantity;
    if ( iNewByteQuantity > m_iByteQuantity )
    {
        assert( m_eStreaming == ST_READING_FROM_DYNAMIC_BYTESTREAM );
        return false;
    }
    MemCpy(acValue,m_acByte+m_iBytePosition,iQuantity);
    m_iBytePosition = iNewByteQuantity;
    return true;
}
//------------------------------------------------------------------------------------------------------------------
bool Stream::Read (unsigned char& rucValue)
{
    assert( m_iBytePosition >= 0 && m_iBitRatio == 0 );

    int iNewByteQuantity = m_iBytePosition + 1;
    if ( iNewByteQuantity > m_iByteQuantity )
    {
        assert( m_eStreaming == ST_READING_FROM_DYNAMIC_BYTESTREAM );
        return false;
    }
    rucValue = *(m_acByte + m_iBytePosition);
    m_iBytePosition = iNewByteQuantity;
    return true;
}
//------------------------------------------------------------------------------------------------------------------
bool Stream::Read (int iQuantity, unsigned char* aucValue)
{
    assert( m_iBytePosition >= 0 && m_iBitRatio == 0 );

    assert( iQuantity >= 0 );

    int iNewByteQuantity = m_iBytePosition + iQuantity;
    if ( iNewByteQuantity > m_iByteQuantity )
    {
        assert( m_eStreaming == ST_READING_FROM_DYNAMIC_BYTESTREAM );
        return false;
    }
    MemCpy(aucValue,m_acByte+m_iBytePosition,iQuantity);
    m_iBytePosition = iNewByteQuantity;
    return true;
}
//------------------------------------------------------------------------------------------------------------------
bool Stream::Read (short& rsValue)
{
    assert( m_iBytePosition >= 0 && m_iBitRatio == 0 );

    int iNewByteQuantity = m_iBytePosition + 2;
    if ( iNewByteQuantity > m_iByteQuantity )
    {
        assert( m_eStreaming == ST_READING_FROM_DYNAMIC_BYTESTREAM );
        return false;
    }
    if ( m_bLittleEndian )
        Storage::Read2le(m_acByte+m_iBytePosition,1,&rsValue);
    else
        Storage::Read2be(m_acByte+m_iBytePosition,1,&rsValue);
    m_iBytePosition = iNewByteQuantity;
    return true;
}
//------------------------------------------------------------------------------------------------------------------
bool Stream::Read (int iQuantity, short* asValue)
{
    assert( m_iBytePosition >= 0 && m_iBitRatio == 0 );

    int iNewByteQuantity = m_iBytePosition + 2*iQuantity;
    if ( iNewByteQuantity > m_iByteQuantity )
    {
        assert( m_eStreaming == ST_READING_FROM_DYNAMIC_BYTESTREAM );
        return false;
    }
    if ( m_bLittleEndian )
        Storage::Read2le(m_acByte+m_iBytePosition,iQuantity,asValue);
    else
        Storage::Read2be(m_acByte+m_iBytePosition,iQuantity,asValue);
    m_iBytePosition = iNewByteQuantity;
    return true;
}
//------------------------------------------------------------------------------------------------------------------
bool Stream::Read (unsigned short& rusValue)
{
    assert( m_iBytePosition >= 0 && m_iBitRatio == 0 );

    int iNewByteQuantity = m_iBytePosition + 2;
    if ( iNewByteQuantity > m_iByteQuantity )
    {
        assert( m_eStreaming == ST_READING_FROM_DYNAMIC_BYTESTREAM );
        return false;
    }
    if ( m_bLittleEndian )
        Storage::Read2le(m_acByte+m_iBytePosition,1,&rusValue);
    else
        Storage::Read2be(m_acByte+m_iBytePosition,1,&rusValue);
    m_iBytePosition = iNewByteQuantity;
    return true;
}
//------------------------------------------------------------------------------------------------------------------
bool Stream::Read (int iQuantity, unsigned short* ausValue)
{
    assert( m_iBytePosition >= 0 && m_iBitRatio == 0 );

    int iNewByteQuantity = m_iBytePosition + 2*iQuantity;
    if ( iNewByteQuantity > m_iByteQuantity )
    {
        assert( m_eStreaming == ST_READING_FROM_DYNAMIC_BYTESTREAM );
        return false;
    }
    if ( m_bLittleEndian )
        Storage::Read2le(m_acByte+m_iBytePosition,iQuantity,ausValue);
    else
        Storage::Read2be(m_acByte+m_iBytePosition,iQuantity,ausValue);
    m_iBytePosition = iNewByteQuantity;
    return true;
}
//------------------------------------------------------------------------------------------------------------------
bool Stream::Read (int& riValue)
{
    assert( m_iBytePosition >= 0 && m_iBitRatio == 0 );

    int iNewByteQuantity = m_iBytePosition + 4;
    if ( iNewByteQuantity > m_iByteQuantity )
    {
        assert( m_eStreaming == ST_READING_FROM_DYNAMIC_BYTESTREAM );
        return false;
    }
    if ( m_bLittleEndian )
        Storage::Read4le(m_acByte+m_iBytePosition,1,&riValue);
    else
        Storage::Read4be(m_acByte+m_iBytePosition,1,&riValue);
    m_iBytePosition = iNewByteQuantity;
    return true;
}
//------------------------------------------------------------------------------------------------------------------
bool Stream::Read (int iQuantity, int* aiValue)
{
    assert( m_iBytePosition >= 0 && m_iBitRatio == 0 );

    int iNewByteQuantity = m_iBytePosition + 4*iQuantity;
    if ( iNewByteQuantity > m_iByteQuantity )
    {
        assert( m_eStreaming == ST_READING_FROM_DYNAMIC_BYTESTREAM );
        return false;
    }
    if ( m_bLittleEndian )
        Storage::Read4le(m_acByte+m_iBytePosition,iQuantity,aiValue);
    else
        Storage::Read4be(m_acByte+m_iBytePosition,iQuantity,aiValue);
    m_iBytePosition = iNewByteQuantity;
    return true;
}
//------------------------------------------------------------------------------------------------------------------
bool Stream::Read (unsigned int& ruiValue)
{
    assert( m_iBytePosition >= 0 && m_iBitRatio == 0 );

    int iNewByteQuantity = m_iBytePosition + 4;
    if ( iNewByteQuantity > m_iByteQuantity )
    {
        assert( m_eStreaming == ST_READING_FROM_DYNAMIC_BYTESTREAM );
        return false;
    }
    if ( m_bLittleEndian )
        Storage::Read4le(m_acByte+m_iBytePosition,1,&ruiValue);
    else
        Storage::Read4be(m_acByte+m_iBytePosition,1,&ruiValue);
    m_iBytePosition = iNewByteQuantity;
    return true;
}
//------------------------------------------------------------------------------------------------------------------
bool Stream::Read (int iQuantity, unsigned int* auiValue)
{
    assert( m_iBytePosition >= 0 && m_iBitRatio == 0 );

    int iNewByteQuantity = m_iBytePosition + 4*iQuantity;
    if ( iNewByteQuantity > m_iByteQuantity )
    {
        assert( m_eStreaming == ST_READING_FROM_DYNAMIC_BYTESTREAM );
        return false;
    }
    if ( m_bLittleEndian )
        Storage::Read4le(m_acByte+m_iBytePosition,iQuantity,auiValue);
    else
        Storage::Read4be(m_acByte+m_iBytePosition,iQuantity,auiValue);
    m_iBytePosition = iNewByteQuantity;
    return true;
}
//------------------------------------------------------------------------------------------------------------------
bool Stream::Read (long& rlValue)
{
    assert( m_iBytePosition >= 0 && m_iBitRatio == 0 );

    int iNewByteQuantity = m_iBytePosition + ( sizeof(void*) == 4 ? 4 : 8 );
    if ( iNewByteQuantity > m_iByteQuantity )
    {
        assert( m_eStreaming == ST_READING_FROM_DYNAMIC_BYTESTREAM );
        return false;
    }
    if ( sizeof(void*) == 4 )
    {
        if ( m_bLittleEndian )
            Storage::Read4le(m_acByte+m_iBytePosition,1,&rlValue);
        else
            Storage::Read4be(m_acByte+m_iBytePosition,1,&rlValue);
    }
    else
    {
        if ( m_bLittleEndian )
            Storage::Read8le(m_acByte+m_iBytePosition,1,&rlValue);
        else
            Storage::Read8be(m_acByte+m_iBytePosition,1,&rlValue);
    }
    m_iBytePosition = iNewByteQuantity;
    return true;
}
//------------------------------------------------------------------------------------------------------------------
bool Stream::Read (int iQuantity, long* alValue)
{
    assert( m_iBytePosition >= 0 && m_iBitRatio == 0 );

    int iNewByteQuantity = m_iBytePosition + (iQuantity << ( sizeof(void*) == 4 ? 2 : 3 ));
    if ( iNewByteQuantity > m_iByteQuantity )
    {
        assert( m_eStreaming == ST_READING_FROM_DYNAMIC_BYTESTREAM );
        return false;
    }
    if ( sizeof(void*) == 4 )
    {
        if ( m_bLittleEndian )
            Storage::Read4le(m_acByte+m_iBytePosition,iQuantity,alValue);
        else
            Storage::Read4be(m_acByte+m_iBytePosition,iQuantity,alValue);
    }
    else
    {
        if ( m_bLittleEndian )
            Storage::Read8le(m_acByte+m_iBytePosition,iQuantity,alValue);
        else
            Storage::Read8be(m_acByte+m_iBytePosition,iQuantity,alValue);
    }
    m_iBytePosition = iNewByteQuantity;
    return true;
}
//------------------------------------------------------------------------------------------------------------------
bool Stream::Read (unsigned long& rulValue)
{
    assert( m_iBytePosition >= 0 && m_iBitRatio == 0 );

    int iNewByteQuantity = m_iBytePosition + ( sizeof(void*) == 4 ? 4 : 8 );
    if ( iNewByteQuantity > m_iByteQuantity )
    {
        assert( m_eStreaming == ST_READING_FROM_DYNAMIC_BYTESTREAM );
        return false;
    }
    if ( sizeof(void*) == 4 )
    {
        if ( m_bLittleEndian )
            Storage::Read4le(m_acByte+m_iBytePosition,1,&rulValue);
        else
            Storage::Read4be(m_acByte+m_iBytePosition,1,&rulValue);
    }
    else
    {
        if ( m_bLittleEndian )
            Storage::Read8le(m_acByte+m_iBytePosition,1,&rulValue);
        else
            Storage::Read8be(m_acByte+m_iBytePosition,1,&rulValue);
    }
    m_iBytePosition = iNewByteQuantity;
    return true;
}
//------------------------------------------------------------------------------------------------------------------
bool Stream::Read (int iQuantity, unsigned long* aulValue)
{
    assert( m_iBytePosition >= 0 && m_iBitRatio == 0 );

    int iNewByteQuantity = m_iBytePosition + (iQuantity << ( sizeof(void*) == 4 ? 2 : 3 ));
    if ( iNewByteQuantity > m_iByteQuantity )
    {
        assert( m_eStreaming == ST_READING_FROM_DYNAMIC_BYTESTREAM );
        return false;
    }
    if ( sizeof(void*) == 4 )
    {
        if ( m_bLittleEndian )
            Storage::Read4le(m_acByte+m_iBytePosition,iQuantity,aulValue);
        else
            Storage::Read4be(m_acByte+m_iBytePosition,iQuantity,aulValue);
    }
    else
    {
        if ( m_bLittleEndian )
            Storage::Read8le(m_acByte+m_iBytePosition,iQuantity,aulValue);
        else
            Storage::Read8be(m_acByte+m_iBytePosition,iQuantity,aulValue);
    }
    m_iBytePosition = iNewByteQuantity;
    return true;
}
//------------------------------------------------------------------------------------------------------------------
bool Stream::Read (Int64& riValue)
{
    assert( m_iBytePosition >= 0 && m_iBitRatio == 0 );

    int iNewByteQuantity = m_iBytePosition + 8;
    if ( iNewByteQuantity > m_iByteQuantity )
    {
        assert( m_eStreaming == ST_READING_FROM_DYNAMIC_BYTESTREAM );
        return false;
    }
    if ( m_bLittleEndian )
        Storage::Read8le(m_acByte+m_iBytePosition,1,&riValue);
    else
        Storage::Read8be(m_acByte+m_iBytePosition,1,&riValue);
    m_iBytePosition = iNewByteQuantity;
    return true;
}
//------------------------------------------------------------------------------------------------------------------
bool Stream::Read (int iQuantity, Int64* aiValue)
{
    assert( m_iBytePosition >= 0 && m_iBitRatio == 0 );

    int iNewByteQuantity = m_iBytePosition + 8*iQuantity;
    if ( iNewByteQuantity > m_iByteQuantity )
    {
        assert( m_eStreaming == ST_READING_FROM_DYNAMIC_BYTESTREAM );
        return false;
    }
    if ( m_bLittleEndian )
        Storage::Read8le(m_acByte+m_iBytePosition,iQuantity,aiValue);
    else
        Storage::Read8be(m_acByte+m_iBytePosition,iQuantity,aiValue);
    m_iBytePosition = iNewByteQuantity;
    return true;
}
//------------------------------------------------------------------------------------------------------------------
bool Stream::Read (float& rfValue)
{
    assert( m_iBytePosition >= 0 && m_iBitRatio == 0 );

    int iNewByteQuantity = m_iBytePosition + 4;
    if ( iNewByteQuantity > m_iByteQuantity )
    {
        assert( m_eStreaming == ST_READING_FROM_DYNAMIC_BYTESTREAM );
        return false;
    }
    if ( m_bLittleEndian )
        Storage::Read4le(m_acByte+m_iBytePosition,1,&rfValue);
    else
        Storage::Read4be(m_acByte+m_iBytePosition,1,&rfValue);
    m_iBytePosition = iNewByteQuantity;
    return true;
}
//------------------------------------------------------------------------------------------------------------------
bool Stream::Read (int iQuantity, float* afValue)
{
    assert( m_iBytePosition >= 0 && m_iBitRatio == 0 );

    int iNewByteQuantity = m_iBytePosition + 4*iQuantity;
    if ( iNewByteQuantity > m_iByteQuantity )
    {
        assert( m_eStreaming == ST_READING_FROM_DYNAMIC_BYTESTREAM );
        return false;
    }
    if ( m_bLittleEndian )
        Storage::Read4le(m_acByte+m_iBytePosition,iQuantity,afValue);
    else
        Storage::Read4be(m_acByte+m_iBytePosition,iQuantity,afValue);
    m_iBytePosition = iNewByteQuantity;
    return true;
}
//------------------------------------------------------------------------------------------------------------------
bool Stream::Read (double& rdValue)
{
    assert( m_iBytePosition >= 0 && m_iBitRatio == 0 );

    int iNewByteQuantity = m_iBytePosition + 8;
    if ( iNewByteQuantity > m_iByteQuantity )
    {
        assert( m_eStreaming == ST_READING_FROM_DYNAMIC_BYTESTREAM );
        return false;
    }
    if ( m_bLittleEndian )
        Storage::Read8le(m_acByte+m_iBytePosition,1,&rdValue);
    else
        Storage::Read8be(m_acByte+m_iBytePosition,1,&rdValue);
    m_iBytePosition = iNewByteQuantity;
    return true;
}
//------------------------------------------------------------------------------------------------------------------
bool Stream::Read (int iQuantity, double* adValue)
{
    assert( m_iBytePosition >= 0 && m_iBitRatio == 0 );

    int iNewByteQuantity = m_iBytePosition + 8*iQuantity;
    if ( iNewByteQuantity > m_iByteQuantity )
    {
        assert( m_eStreaming == ST_READING_FROM_DYNAMIC_BYTESTREAM );
        return false;
    }
    if ( m_bLittleEndian )
        Storage::Read8le(m_acByte+m_iBytePosition,iQuantity,adValue);
    else
        Storage::Read8be(m_acByte+m_iBytePosition,iQuantity,adValue);
    m_iBytePosition = iNewByteQuantity;
    return true;
}
//------------------------------------------------------------------------------------------------------------------
void Stream::Write (const Vector2f& rqValue)
{
    assert( m_iBytePosition >= 0 && m_iBitRatio == 0 );

    int iNewByteQuantity = m_iBytePosition + 8;
    if ( iNewByteQuantity > m_iByteQuantity )
    {
        if ( m_bOwner )
            SetByteQuantity(iNewByteQuantity);
        else
            assert( false );
    }
    Storage::Write4le(m_acByte+m_iBytePosition,2,rqValue);
    m_iBytePosition = iNewByteQuantity;
}
//------------------------------------------------------------------------------------------------------------------
void Stream::Write (int iQuantity, const Vector2f* aqValue)
{
    assert( m_iBytePosition >= 0 && m_iBitRatio == 0 );

    int iNewByteQuantity = m_iBytePosition + 8*iQuantity;
    if ( iNewByteQuantity > m_iByteQuantity )
    {
        if ( m_bOwner )
            SetByteQuantity(iNewByteQuantity);
        else
            assert( false );
    }
    Storage::Write4le(m_acByte+m_iBytePosition,2*iQuantity,aqValue);
    m_iBytePosition = iNewByteQuantity;
}
//------------------------------------------------------------------------------------------------------------------
void Stream::Write (const Vector3f& rqValue)
{
    assert( m_iBytePosition >= 0 && m_iBitRatio == 0 );

    int iNewByteQuantity = m_iBytePosition + 12;
    if ( iNewByteQuantity > m_iByteQuantity )
    {
        if ( m_bOwner )
            SetByteQuantity(iNewByteQuantity);
        else
            assert( false );
    }
    Storage::Write4le(m_acByte+m_iBytePosition,3,rqValue);
    m_iBytePosition = iNewByteQuantity;
}
//------------------------------------------------------------------------------------------------------------------
void Stream::Write (int iQuantity, const Vector3f* aqValue)
{
    assert( m_iBytePosition >= 0 && m_iBitRatio == 0 );

    int iNewByteQuantity = m_iBytePosition + 12*iQuantity;
    if ( iNewByteQuantity > m_iByteQuantity )
    {
        if ( m_bOwner )
            SetByteQuantity(iNewByteQuantity);
        else
            assert( false );
    }
    Storage::Write4le(m_acByte+m_iBytePosition,3*iQuantity,aqValue);
    m_iBytePosition = iNewByteQuantity;
}
//------------------------------------------------------------------------------------------------------------------
void Stream::Write (const Vector4f& rqValue)
{
    assert( m_iBytePosition >= 0 && m_iBitRatio == 0 );

    int iNewByteQuantity = m_iBytePosition + 16;
    if ( iNewByteQuantity > m_iByteQuantity )
    {
        if ( m_bOwner )
            SetByteQuantity(iNewByteQuantity);
        else
            assert( false );
    }
    Storage::Write4le(m_acByte+m_iBytePosition,4,rqValue);
    m_iBytePosition = iNewByteQuantity;
}
//------------------------------------------------------------------------------------------------------------------
void Stream::Write (int iQuantity, const Vector4f* aqValue)
{
    assert( m_iBytePosition >= 0 && m_iBitRatio == 0 );

    int iNewByteQuantity = m_iBytePosition + 16*iQuantity;
    if ( iNewByteQuantity > m_iByteQuantity )
    {
        if ( m_bOwner )
            SetByteQuantity(iNewByteQuantity);
        else
            assert( false );
    }
    Storage::Write4le(m_acByte+m_iBytePosition,4*iQuantity,aqValue);
    m_iBytePosition = iNewByteQuantity;
}
//------------------------------------------------------------------------------------------------------------------
void Stream::Write (const Vector2d& rqValue)
{
    assert( m_iBytePosition >= 0 && m_iBitRatio == 0 );

    int iNewByteQuantity = m_iBytePosition + 16;
    if ( iNewByteQuantity > m_iByteQuantity )
    {
        if ( m_bOwner )
            SetByteQuantity(iNewByteQuantity);
        else
            assert( false );
    }
    Storage::Write8le(m_acByte+m_iBytePosition,2,rqValue);
    m_iBytePosition = iNewByteQuantity;
}
//------------------------------------------------------------------------------------------------------------------
void Stream::Write (int iQuantity, const Vector2d* aqValue)
{
    assert( m_iBytePosition >= 0 && m_iBitRatio == 0 );

    int iNewByteQuantity = m_iBytePosition + 16*iQuantity;
    if ( iNewByteQuantity > m_iByteQuantity )
    {
        if ( m_bOwner )
            SetByteQuantity(iNewByteQuantity);
        else
            assert( false );
    }
    Storage::Write8le(m_acByte+m_iBytePosition,2*iQuantity,aqValue);
    m_iBytePosition = iNewByteQuantity;
}
//------------------------------------------------------------------------------------------------------------------
void Stream::Write (const Vector3d& rqValue)
{
    assert( m_iBytePosition >= 0 && m_iBitRatio == 0 );

    int iNewByteQuantity = m_iBytePosition + 24;
    if ( iNewByteQuantity > m_iByteQuantity )
    {
        if ( m_bOwner )
            SetByteQuantity(iNewByteQuantity);
        else
            assert( false );
    }
    Storage::Write8le(m_acByte+m_iBytePosition,3,rqValue);
    m_iBytePosition = iNewByteQuantity;
}
//------------------------------------------------------------------------------------------------------------------
void Stream::Write (int iQuantity, const Vector3d* aqValue)
{
    assert( m_iBytePosition >= 0 && m_iBitRatio == 0 );

    int iNewByteQuantity = m_iBytePosition + 24*iQuantity;
    if ( iNewByteQuantity > m_iByteQuantity )
    {
        if ( m_bOwner )
            SetByteQuantity(iNewByteQuantity);
        else
            assert( false );
    }
    Storage::Write8le(m_acByte+m_iBytePosition,3*iQuantity,aqValue);
    m_iBytePosition = iNewByteQuantity;
}
//------------------------------------------------------------------------------------------------------------------
void Stream::Write (const Vector4d& rqValue)
{
    assert( m_iBytePosition >= 0 && m_iBitRatio == 0 );

    int iNewByteQuantity = m_iBytePosition + 32;
    if ( iNewByteQuantity > m_iByteQuantity )
    {
        if ( m_bOwner )
            SetByteQuantity(iNewByteQuantity);
        else
            assert( false );
    }
    Storage::Write8le(m_acByte+m_iBytePosition,4,rqValue);
    m_iBytePosition = iNewByteQuantity;
}
//------------------------------------------------------------------------------------------------------------------
void Stream::Write (int iQuantity, const Vector4d* aqValue)
{
    assert( m_iBytePosition >= 0 && m_iBitRatio == 0 );

    int iNewByteQuantity = m_iBytePosition + 32*iQuantity;
    if ( iNewByteQuantity > m_iByteQuantity )
    {
        if ( m_bOwner )
            SetByteQuantity(iNewByteQuantity);
        else
            assert( false );
    }
    Storage::Write8le(m_acByte+m_iBytePosition,4*iQuantity,aqValue);
    m_iBytePosition = iNewByteQuantity;
}
//------------------------------------------------------------------------------------------------------------------
void Stream::Write (const Matrix2f& rqValue)
{
    assert( m_iBytePosition >= 0 && m_iBitRatio == 0 );

    int iNewByteQuantity = m_iBytePosition + 16;
    if ( iNewByteQuantity > m_iByteQuantity )
    {
        if ( m_bOwner )
            SetByteQuantity(iNewByteQuantity);
        else
            assert( false );
    }
    Storage::Write4le(m_acByte+m_iBytePosition,4,rqValue);
    m_iBytePosition = iNewByteQuantity;
}
//------------------------------------------------------------------------------------------------------------------
void Stream::Write (int iQuantity, const Matrix2f* aqValue)
{
    assert( m_iBytePosition >= 0 && m_iBitRatio == 0 );

    int iNewByteQuantity = m_iBytePosition + 16*iQuantity;
    if ( iNewByteQuantity > m_iByteQuantity )
    {
        if ( m_bOwner )
            SetByteQuantity(iNewByteQuantity);
        else
            assert( false );
    }
    Storage::Write4le(m_acByte+m_iBytePosition,4*iQuantity,aqValue);
    m_iBytePosition = iNewByteQuantity;
}
//------------------------------------------------------------------------------------------------------------------
void Stream::Write (const Matrix3f& rqValue)
{
    assert( m_iBytePosition >= 0 && m_iBitRatio == 0 );

    int iNewByteQuantity = m_iBytePosition + 36;
    if ( iNewByteQuantity > m_iByteQuantity )
    {
        if ( m_bOwner )
            SetByteQuantity(iNewByteQuantity);
        else
            assert( false );
    }
    Storage::Write4le(m_acByte+m_iBytePosition,9,rqValue);
    m_iBytePosition = iNewByteQuantity;
}
//------------------------------------------------------------------------------------------------------------------
void Stream::Write (int iQuantity, const Matrix3f* aqValue)
{
    assert( m_iBytePosition >= 0 && m_iBitRatio == 0 );

    int iNewByteQuantity = m_iBytePosition + 36*iQuantity;
    if ( iNewByteQuantity > m_iByteQuantity )
    {
        if ( m_bOwner )
            SetByteQuantity(iNewByteQuantity);
        else
            assert( false );
    }
    Storage::Write4le(m_acByte+m_iBytePosition,9*iQuantity,aqValue);
    m_iBytePosition = iNewByteQuantity;
}
//------------------------------------------------------------------------------------------------------------------
void Stream::Write (const Matrix4f& rqValue)
{
    assert( m_iBytePosition >= 0 && m_iBitRatio == 0 );

    int iNewByteQuantity = m_iBytePosition + 64;
    if ( iNewByteQuantity > m_iByteQuantity )
    {
        if ( m_bOwner )
            SetByteQuantity(iNewByteQuantity);
        else
            assert( false );
    }
    Storage::Write4le(m_acByte+m_iBytePosition,16,rqValue);
    m_iBytePosition = iNewByteQuantity;
}
//------------------------------------------------------------------------------------------------------------------
void Stream::Write (int iQuantity, const Matrix4f* aqValue)
{
    assert( m_iBytePosition >= 0 && m_iBitRatio == 0 );

    int iNewByteQuantity = m_iBytePosition + 64*iQuantity;
    if ( iNewByteQuantity > m_iByteQuantity )
    {
        if ( m_bOwner )
            SetByteQuantity(iNewByteQuantity);
        else
            assert( false );
    }
    Storage::Write4le(m_acByte+m_iBytePosition,16*iQuantity,aqValue);
    m_iBytePosition = iNewByteQuantity;
}
//------------------------------------------------------------------------------------------------------------------
void Stream::Write (const Matrix2d& rqValue)
{
    assert( m_iBytePosition >= 0 && m_iBitRatio == 0 );

    int iNewByteQuantity = m_iBytePosition + 32;
    if ( iNewByteQuantity > m_iByteQuantity )
    {
        if ( m_bOwner )
            SetByteQuantity(iNewByteQuantity);
        else
            assert( false );
    }
    Storage::Write8le(m_acByte+m_iBytePosition,4,rqValue);
    m_iBytePosition = iNewByteQuantity;
}
//------------------------------------------------------------------------------------------------------------------
void Stream::Write (int iQuantity, const Matrix2d* aqValue)
{
    assert( m_iBytePosition >= 0 && m_iBitRatio == 0 );

    int iNewByteQuantity = m_iBytePosition + 32*iQuantity;
    if ( iNewByteQuantity > m_iByteQuantity )
    {
        if ( m_bOwner )
            SetByteQuantity(iNewByteQuantity);
        else
            assert( false );
    }
    Storage::Write8le(m_acByte+m_iBytePosition,4*iQuantity,aqValue);
    m_iBytePosition = iNewByteQuantity;
}
//------------------------------------------------------------------------------------------------------------------
void Stream::Write (const Matrix3d& rqValue)
{
    assert( m_iBytePosition >= 0 && m_iBitRatio == 0 );

    int iNewByteQuantity = m_iBytePosition + 72;
    if ( iNewByteQuantity > m_iByteQuantity )
    {
        if ( m_bOwner )
            SetByteQuantity(iNewByteQuantity);
        else
            assert( false );
    }
    Storage::Write8le(m_acByte+m_iBytePosition,9,rqValue);
    m_iBytePosition = iNewByteQuantity;
}
//------------------------------------------------------------------------------------------------------------------
void Stream::Write (int iQuantity, const Matrix3d* aqValue)
{
    assert( m_iBytePosition >= 0 && m_iBitRatio == 0 );

    int iNewByteQuantity = m_iBytePosition + 72*iQuantity;
    if ( iNewByteQuantity > m_iByteQuantity )
    {
        if ( m_bOwner )
            SetByteQuantity(iNewByteQuantity);
        else
            assert( false );
    }
    Storage::Write8le(m_acByte+m_iBytePosition,9*iQuantity,aqValue);
    m_iBytePosition = iNewByteQuantity;
}
//------------------------------------------------------------------------------------------------------------------
void Stream::Write (const Matrix4d& rqValue)
{
    assert( m_iBytePosition >= 0 && m_iBitRatio == 0 );

    int iNewByteQuantity = m_iBytePosition + 128;
    if ( iNewByteQuantity > m_iByteQuantity )
    {
        if ( m_bOwner )
            SetByteQuantity(iNewByteQuantity);
        else
            assert( false );
    }
    Storage::Write8le(m_acByte+m_iBytePosition,16,rqValue);
    m_iBytePosition = iNewByteQuantity;
}
//------------------------------------------------------------------------------------------------------------------
void Stream::Write (int iQuantity, const Matrix4d* aqValue)
{
    assert( m_iBytePosition >= 0 && m_iBitRatio == 0 );

    int iNewByteQuantity = m_iBytePosition + 128*iQuantity;
    if ( iNewByteQuantity > m_iByteQuantity )
    {
        if ( m_bOwner )
            SetByteQuantity(iNewByteQuantity);
        else
            assert( false );
    }
    Storage::Write8le(m_acByte+m_iBytePosition,16*iQuantity,aqValue);
    m_iBytePosition = iNewByteQuantity;
}
//------------------------------------------------------------------------------------------------------------------
void Stream::Write (const ColorRgb& rqValue)
{
    assert( m_iBytePosition >= 0 && m_iBitRatio == 0 );

    int iNewByteQuantity = m_iBytePosition + 12;
    if ( iNewByteQuantity > m_iByteQuantity )
    {
        if ( m_bOwner )
            SetByteQuantity(iNewByteQuantity);
        else
            assert( false );
    }
    Storage::Write4le(m_acByte+m_iBytePosition,3,rqValue);
    m_iBytePosition = iNewByteQuantity;
}
//------------------------------------------------------------------------------------------------------------------
void Stream::Write (int iQuantity, const ColorRgb* aqValue)
{
    assert( m_iBytePosition >= 0 && m_iBitRatio == 0 );

    int iNewByteQuantity = m_iBytePosition + 12*iQuantity;
    if ( iNewByteQuantity > m_iByteQuantity )
    {
        if ( m_bOwner )
            SetByteQuantity(iNewByteQuantity);
        else
            assert( false );
    }
    Storage::Write4le(m_acByte+m_iBytePosition,3*iQuantity,aqValue);
    m_iBytePosition = iNewByteQuantity;
}
//------------------------------------------------------------------------------------------------------------------
void Stream::Write (const ColorRgba& rqValue)
{
    assert( m_iBytePosition >= 0 && m_iBitRatio == 0 );

    int iNewByteQuantity = m_iBytePosition + 16;
    if ( iNewByteQuantity > m_iByteQuantity )
    {
        if ( m_bOwner )
            SetByteQuantity(iNewByteQuantity);
        else
            assert( false );
    }
    Storage::Write4le(m_acByte+m_iBytePosition,4,rqValue);
    m_iBytePosition = iNewByteQuantity;
}
//------------------------------------------------------------------------------------------------------------------
void Stream::Write (int iQuantity, const ColorRgba* aqValue)
{
    assert( m_iBytePosition >= 0 && m_iBitRatio == 0 );

    int iNewByteQuantity = m_iBytePosition + 16*iQuantity;
    if ( iNewByteQuantity > m_iByteQuantity )
    {
        if ( m_bOwner )
            SetByteQuantity(iNewByteQuantity);
        else
            assert( false );
    }
    Storage::Write4le(m_acByte+m_iBytePosition,4*iQuantity,aqValue);
    m_iBytePosition = iNewByteQuantity;
}
//------------------------------------------------------------------------------------------------------------------
bool Stream::Read (Vector2f& rqValue)
{
    assert( m_iBytePosition >= 0 && m_iBitRatio == 0 );

    int iNewByteQuantity = m_iBytePosition + 8;
    if ( iNewByteQuantity > m_iByteQuantity )
    {
        assert( m_eStreaming == ST_READING_FROM_DYNAMIC_BYTESTREAM );
        return false;
    }
    Storage::Read4le(m_acByte+m_iBytePosition,2,rqValue);
    m_iBytePosition = iNewByteQuantity;
    return true;
}
//------------------------------------------------------------------------------------------------------------------
bool Stream::Read (int iQuantity, Vector2f* aqValue)
{
    assert( m_iBytePosition >= 0 && m_iBitRatio == 0 );

    int iNewByteQuantity = m_iBytePosition + 8*iQuantity;
    if ( iNewByteQuantity > m_iByteQuantity )
    {
        assert( m_eStreaming == ST_READING_FROM_DYNAMIC_BYTESTREAM );
        return false;
    }
    Storage::Read4le(m_acByte+m_iBytePosition,2*iQuantity,aqValue);
    m_iBytePosition = iNewByteQuantity;
    return true;
}
//------------------------------------------------------------------------------------------------------------------
bool Stream::Read (Vector3f& rqValue)
{
    assert( m_iBytePosition >= 0 && m_iBitRatio == 0 );

    int iNewByteQuantity = m_iBytePosition + 12;
    if ( iNewByteQuantity > m_iByteQuantity )
    {
        assert( m_eStreaming == ST_READING_FROM_DYNAMIC_BYTESTREAM );
        return false;
    }
    Storage::Read4le(m_acByte+m_iBytePosition,3,rqValue);
    m_iBytePosition = iNewByteQuantity;
    return true;
}
//------------------------------------------------------------------------------------------------------------------
bool Stream::Read (int iQuantity, Vector3f* aqValue)
{
    assert( m_iBytePosition >= 0 && m_iBitRatio == 0 );

    int iNewByteQuantity = m_iBytePosition + 12*iQuantity;
    if ( iNewByteQuantity > m_iByteQuantity )
    {
        assert( m_eStreaming == ST_READING_FROM_DYNAMIC_BYTESTREAM );
        return false;
    }
    Storage::Read4le(m_acByte+m_iBytePosition,3*iQuantity,aqValue);
    m_iBytePosition = iNewByteQuantity;
    return true;
}
//------------------------------------------------------------------------------------------------------------------
bool Stream::Read (Vector4f& rqValue)
{
    assert( m_iBytePosition >= 0 && m_iBitRatio == 0 );

    int iNewByteQuantity = m_iBytePosition + 16;
    if ( iNewByteQuantity > m_iByteQuantity )
    {
        assert( m_eStreaming == ST_READING_FROM_DYNAMIC_BYTESTREAM );
        return false;
    }
    Storage::Read4le(m_acByte+m_iBytePosition,4,rqValue);
    m_iBytePosition = iNewByteQuantity;
    return true;
}
//------------------------------------------------------------------------------------------------------------------
bool Stream::Read (int iQuantity, Vector4f* aqValue)
{
    assert( m_iBytePosition >= 0 && m_iBitRatio == 0 );

    int iNewByteQuantity = m_iBytePosition + 16*iQuantity;
    if ( iNewByteQuantity > m_iByteQuantity )
    {
        assert( m_eStreaming == ST_READING_FROM_DYNAMIC_BYTESTREAM );
        return false;
    }
    Storage::Read4le(m_acByte+m_iBytePosition,4*iQuantity,aqValue);
    m_iBytePosition = iNewByteQuantity;
    return true;
}
//------------------------------------------------------------------------------------------------------------------
bool Stream::Read (Vector2d& rqValue)
{
    assert( m_iBytePosition >= 0 && m_iBitRatio == 0 );

    int iNewByteQuantity = m_iBytePosition + 16;
    if ( iNewByteQuantity > m_iByteQuantity )
    {
        assert( m_eStreaming == ST_READING_FROM_DYNAMIC_BYTESTREAM );
        return false;
    }
    Storage::Read8le(m_acByte+m_iBytePosition,2,rqValue);
    m_iBytePosition = iNewByteQuantity;
    return true;
}
//------------------------------------------------------------------------------------------------------------------
bool Stream::Read (int iQuantity, Vector2d* aqValue)
{
    assert( m_iBytePosition >= 0 && m_iBitRatio == 0 );

    int iNewByteQuantity = m_iBytePosition + 16*iQuantity;
    if ( iNewByteQuantity > m_iByteQuantity )
    {
        assert( m_eStreaming == ST_READING_FROM_DYNAMIC_BYTESTREAM );
        return false;
    }
    Storage::Read8le(m_acByte+m_iBytePosition,2*iQuantity,aqValue);
    m_iBytePosition = iNewByteQuantity;
    return true;
}
//------------------------------------------------------------------------------------------------------------------
bool Stream::Read (Vector3d& rqValue)
{
    assert( m_iBytePosition >= 0 && m_iBitRatio == 0 );

    int iNewByteQuantity = m_iBytePosition + 24;
    if ( iNewByteQuantity > m_iByteQuantity )
    {
        assert( m_eStreaming == ST_READING_FROM_DYNAMIC_BYTESTREAM );
        return false;
    }
    Storage::Read8le(m_acByte+m_iBytePosition,3,rqValue);
    m_iBytePosition = iNewByteQuantity;
    return true;
}
//------------------------------------------------------------------------------------------------------------------
bool Stream::Read (int iQuantity, Vector3d* aqValue)
{
    assert( m_iBytePosition >= 0 && m_iBitRatio == 0 );

    int iNewByteQuantity = m_iBytePosition + 24*iQuantity;
    if ( iNewByteQuantity > m_iByteQuantity )
    {
        assert( m_eStreaming == ST_READING_FROM_DYNAMIC_BYTESTREAM );
        return false;
    }
    Storage::Read8le(m_acByte+m_iBytePosition,3*iQuantity,aqValue);
    m_iBytePosition = iNewByteQuantity;
    return true;
}
//------------------------------------------------------------------------------------------------------------------
bool Stream::Read (Vector4d& rqValue)
{
    assert( m_iBytePosition >= 0 && m_iBitRatio == 0 );

    int iNewByteQuantity = m_iBytePosition + 32;
    if ( iNewByteQuantity > m_iByteQuantity )
    {
        assert( m_eStreaming == ST_READING_FROM_DYNAMIC_BYTESTREAM );
        return false;
    }
    Storage::Read8le(m_acByte+m_iBytePosition,4,rqValue);
    m_iBytePosition = iNewByteQuantity;
    return true;
}
//------------------------------------------------------------------------------------------------------------------
bool Stream::Read (int iQuantity, Vector4d* aqValue)
{
    assert( m_iBytePosition >= 0 && m_iBitRatio == 0 );

    int iNewByteQuantity = m_iBytePosition + 32*iQuantity;
    if ( iNewByteQuantity > m_iByteQuantity )
    {
        assert( m_eStreaming == ST_READING_FROM_DYNAMIC_BYTESTREAM );
        return false;
    }
    Storage::Read8le(m_acByte+m_iBytePosition,4*iQuantity,aqValue);
    m_iBytePosition = iNewByteQuantity;
    return true;
}
//------------------------------------------------------------------------------------------------------------------
bool Stream::Read (Matrix2f& rqValue)
{
    assert( m_iBytePosition >= 0 && m_iBitRatio == 0 );

    int iNewByteQuantity = m_iBytePosition + 16;
    if ( iNewByteQuantity > m_iByteQuantity )
    {
        assert( m_eStreaming == ST_READING_FROM_DYNAMIC_BYTESTREAM );
        return false;
    }
    Storage::Read4le(m_acByte+m_iBytePosition,4,rqValue);
    m_iBytePosition = iNewByteQuantity;
    return true;
}
//------------------------------------------------------------------------------------------------------------------
bool Stream::Read (int iQuantity, Matrix2f* aqValue)
{
    assert( m_iBytePosition >= 0 && m_iBitRatio == 0 );

    int iNewByteQuantity = m_iBytePosition + 16*iQuantity;
    if ( iNewByteQuantity > m_iByteQuantity )
    {
        assert( m_eStreaming == ST_READING_FROM_DYNAMIC_BYTESTREAM );
        return false;
    }
    Storage::Read4le(m_acByte+m_iBytePosition,4*iQuantity,aqValue);
    m_iBytePosition = iNewByteQuantity;
    return true;
}
//------------------------------------------------------------------------------------------------------------------
bool Stream::Read (Matrix3f& rqValue)
{
    assert( m_iBytePosition >= 0 && m_iBitRatio == 0 );

    int iNewByteQuantity = m_iBytePosition + 36;
    if ( iNewByteQuantity > m_iByteQuantity )
    {
        assert( m_eStreaming == ST_READING_FROM_DYNAMIC_BYTESTREAM );
        return false;
    }
    Storage::Read4le(m_acByte+m_iBytePosition,9,rqValue);
    m_iBytePosition = iNewByteQuantity;
    return true;
}
//------------------------------------------------------------------------------------------------------------------
bool Stream::Read (int iQuantity, Matrix3f* aqValue)
{
    assert( m_iBytePosition >= 0 && m_iBitRatio == 0 );

    int iNewByteQuantity = m_iBytePosition + 36*iQuantity;
    if ( iNewByteQuantity > m_iByteQuantity )
    {
        assert( m_eStreaming == ST_READING_FROM_DYNAMIC_BYTESTREAM );
        return false;
    }
    Storage::Read4le(m_acByte+m_iBytePosition,9*iQuantity,aqValue);
    m_iBytePosition = iNewByteQuantity;
    return true;
}
//------------------------------------------------------------------------------------------------------------------
bool Stream::Read (Matrix4f& rqValue)
{
    assert( m_iBytePosition >= 0 && m_iBitRatio == 0 );

    int iNewByteQuantity = m_iBytePosition + 64;
    if ( iNewByteQuantity > m_iByteQuantity )
    {
        assert( m_eStreaming == ST_READING_FROM_DYNAMIC_BYTESTREAM );
        return false;
    }
    Storage::Read4le(m_acByte+m_iBytePosition,16,rqValue);
    m_iBytePosition = iNewByteQuantity;
    return true;
}
//------------------------------------------------------------------------------------------------------------------
bool Stream::Read (int iQuantity, Matrix4f* aqValue)
{
    assert( m_iBytePosition >= 0 && m_iBitRatio == 0 );

    int iNewByteQuantity = m_iBytePosition + 64*iQuantity;
    if ( iNewByteQuantity > m_iByteQuantity )
    {
        assert( m_eStreaming == ST_READING_FROM_DYNAMIC_BYTESTREAM );
        return false;
    }
    Storage::Read4le(m_acByte+m_iBytePosition,16*iQuantity,aqValue);
    m_iBytePosition = iNewByteQuantity;
    return true;
}
//------------------------------------------------------------------------------------------------------------------
bool Stream::Read (Matrix2d& rqValue)
{
    assert( m_iBytePosition >= 0 && m_iBitRatio == 0 );

    int iNewByteQuantity = m_iBytePosition + 32;
    if ( iNewByteQuantity > m_iByteQuantity )
    {
        assert( m_eStreaming == ST_READING_FROM_DYNAMIC_BYTESTREAM );
        return false;
    }
    Storage::Read8le(m_acByte+m_iBytePosition,4,rqValue);
    m_iBytePosition = iNewByteQuantity;
    return true;
}
//------------------------------------------------------------------------------------------------------------------
bool Stream::Read (int iQuantity, Matrix2d* aqValue)
{
    assert( m_iBytePosition >= 0 && m_iBitRatio == 0 );

    int iNewByteQuantity = m_iBytePosition + 32*iQuantity;
    if ( iNewByteQuantity > m_iByteQuantity )
    {
        assert( m_eStreaming == ST_READING_FROM_DYNAMIC_BYTESTREAM );
        return false;
    }
    Storage::Read8le(m_acByte+m_iBytePosition,4*iQuantity,aqValue);
    m_iBytePosition = iNewByteQuantity;
    return true;
}
//------------------------------------------------------------------------------------------------------------------
bool Stream::Read (Matrix3d& rqValue)
{
    assert( m_iBytePosition >= 0 && m_iBitRatio == 0 );

    int iNewByteQuantity = m_iBytePosition + 72;
    if ( iNewByteQuantity > m_iByteQuantity )
    {
        assert( m_eStreaming == ST_READING_FROM_DYNAMIC_BYTESTREAM );
        return false;
    }
    Storage::Read8le(m_acByte+m_iBytePosition,9,rqValue);
    m_iBytePosition = iNewByteQuantity;
    return true;
}
//------------------------------------------------------------------------------------------------------------------
bool Stream::Read (int iQuantity, Matrix3d* aqValue)
{
    assert( m_iBytePosition >= 0 && m_iBitRatio == 0 );

    int iNewByteQuantity = m_iBytePosition + 72*iQuantity;
    if ( iNewByteQuantity > m_iByteQuantity )
    {
        assert( m_eStreaming == ST_READING_FROM_DYNAMIC_BYTESTREAM );
        return false;
    }
    Storage::Read8le(m_acByte+m_iBytePosition,9*iQuantity,aqValue);
    m_iBytePosition = iNewByteQuantity;
    return true;
}
//------------------------------------------------------------------------------------------------------------------
bool Stream::Read (Matrix4d& rqValue)
{
    assert( m_iBytePosition >= 0 && m_iBitRatio == 0 );

    int iNewByteQuantity = m_iBytePosition + 128;
    if ( iNewByteQuantity > m_iByteQuantity )
    {
        assert( m_eStreaming == ST_READING_FROM_DYNAMIC_BYTESTREAM );
        return false;
    }
    Storage::Read8le(m_acByte+m_iBytePosition,16,rqValue);
    m_iBytePosition = iNewByteQuantity;
    return true;
}
//------------------------------------------------------------------------------------------------------------------
bool Stream::Read (int iQuantity, Matrix4d* aqValue)
{
    assert( m_iBytePosition >= 0 && m_iBitRatio == 0 );

    int iNewByteQuantity = m_iBytePosition + 128*iQuantity;
    if ( iNewByteQuantity > m_iByteQuantity )
    {
        assert( m_eStreaming == ST_READING_FROM_DYNAMIC_BYTESTREAM );
        return false;
    }
    Storage::Read8le(m_acByte+m_iBytePosition,16*iQuantity,aqValue);
    m_iBytePosition = iNewByteQuantity;
    return true;
}
//------------------------------------------------------------------------------------------------------------------
bool Stream::Read (ColorRgb& rqValue)
{
    assert( m_iBytePosition >= 0 && m_iBitRatio == 0 );

    int iNewByteQuantity = m_iBytePosition + 12;
    if ( iNewByteQuantity > m_iByteQuantity )
    {
        assert( m_eStreaming == ST_READING_FROM_DYNAMIC_BYTESTREAM );
        return false;
    }
    Storage::Read4le(m_acByte+m_iBytePosition,3,rqValue);
    m_iBytePosition = iNewByteQuantity;
    return true;
}
//------------------------------------------------------------------------------------------------------------------
bool Stream::Read (int iQuantity, ColorRgb* aqValue)
{
    assert( m_iBytePosition >= 0 && m_iBitRatio == 0 );

    int iNewByteQuantity = m_iBytePosition + 12*iQuantity;
    if ( iNewByteQuantity > m_iByteQuantity )
    {
        assert( m_eStreaming == ST_READING_FROM_DYNAMIC_BYTESTREAM );
        return false;
    }
    Storage::Read4le(m_acByte+m_iBytePosition,3*iQuantity,aqValue);
    m_iBytePosition = iNewByteQuantity;
    return true;
}
//------------------------------------------------------------------------------------------------------------------
bool Stream::Read (ColorRgba& rqValue)
{
    assert( m_iBytePosition >= 0 && m_iBitRatio == 0 );

    int iNewByteQuantity = m_iBytePosition + 16;
    if ( iNewByteQuantity > m_iByteQuantity )
    {
        assert( m_eStreaming == ST_READING_FROM_DYNAMIC_BYTESTREAM );
        return false;
    }
    Storage::Read4le(m_acByte+m_iBytePosition,4,rqValue);
    m_iBytePosition = iNewByteQuantity;
    return true;
}
//------------------------------------------------------------------------------------------------------------------
bool Stream::Read (int iQuantity, ColorRgba* aqValue)
{
    assert( m_iBytePosition >= 0 && m_iBitRatio == 0 );

    int iNewByteQuantity = m_iBytePosition + 16*iQuantity;
    if ( iNewByteQuantity > m_iByteQuantity )
    {
        assert( m_eStreaming == ST_READING_FROM_DYNAMIC_BYTESTREAM );
        return false;
    }
    Storage::Read4le(m_acByte+m_iBytePosition,4*iQuantity,aqValue);
    m_iBytePosition = iNewByteQuantity;
    return true;
}
//------------------------------------------------------------------------------------------------------------------
void Stream::Write (...)
{
    assert( false );
}
//------------------------------------------------------------------------------------------------------------------
void Stream::Read (...)
{
    assert( false );
}
//------------------------------------------------------------------------------------------------------------------
void Stream::Write (void* pvValue)
{
    assert( false );
}
//------------------------------------------------------------------------------------------------------------------
void Stream::Read (void* pvValue)
{
    assert( false );
}
//------------------------------------------------------------------------------------------------------------------
void Stream::SetByteQuantity (int iNeededByteQuantity)
{
    if ( iNeededByteQuantity <= m_iByteQuantity )
        return;

    int iNewByteQuantity = iNeededByteQuantity + iNeededByteQuantity/ms_iAdvanceQuantityDivider;
    char* acNewByte = new char[iNewByteQuantity];
    MemCpy(acNewByte,m_acByte,m_iByteQuantity);
    delete[] m_acByte;
    m_acByte = acNewByte;
    m_iByteQuantity = iNewByteQuantity;
}
//------------------------------------------------------------------------------------------------------------------
void Stream::AddId (unsigned int uiId, void* pvPointer)
{
    ((ArrayT*)m_pvIdWithPointerArray)->Push(Link<unsigned int,void*>(uiId,pvPointer));
}
//------------------------------------------------------------------------------------------------------------------
void Stream::MapId (unsigned int uiId, void** ppvPointer)
{
    ((MapT*)m_pvIdToPPointerMap)->Insert(uiId,ppvPointer);
}
//------------------------------------------------------------------------------------------------------------------



