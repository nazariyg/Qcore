#ifndef QSTREAM_H
#define QSTREAM_H

// The main purpose of the Stream class is writing and reading files, file parts, and bytestreams by
// involvement of a "head".  In addition to that, the class may serve as a memory buffer.  There are two sorts
// of bytestreams.  A static bytestream is a block of working memory which keeps motionless in its size.  On
// the contrary, a dynamic bytestream is a block of working memory with gradually rising number of available
// bytes.  The Stream class processes destination/source file data indirectly, i.e. in the working memory.  In
// particular, all kinds of writing at first memorize the data to be outputted by means of adaptive enlargement
// of the Stream's memory buffer (the enlargement is not to happen when patching existing data), and, on the
// completion phase, flushing occurs.  NOTE. Since in writing mode the Stream's memory buffer may reallocate in
// order to become bigger, the user must not store the buffer's address at any point of data feeding.
//
// The class also provides the feature of restoring of smart pointers if they were saved together with pointed
// objects into the same data being read.  Only that smart pointer can be saved and restored which points to an
// object derived from the Id class.  The restoration mechanism is described next.  Every Id class derived
// object stores its id when its Save member function is called and every smart pointer on saving stores the id
// of the object it points to.  On loading, each Id derived object reads the former id (but without assignment
// of course) and with the Stream's AddId member function makes the Stream object remember the id together with
// the pointer to that object; from the other side, each smart pointer being read extracts the former id of the
// previously pointed object and via the Stream's MapId member function maps the id to the address of the
// native pointer used inside that smart pointer object.  On the Stream object's finalizing every added id
// finds through the map the address of the correlative native pointer and then the native pointer found in
// that way is assigned by the modern-context address.
//
// In the class' constructions below the bLittleEndian flag makes sense only if it's implied writing/reading of
// byte-based data units with byte sizes greater than one, that is not for cases when bitfield or one-byte data
// operations are the only to be performed during the session.  A Stream object before its destruction should
// be "manually" finalized with the Finalize member function (see the comments by).  However the finalization
// is just encouraged and may be omitted if there is no other way.
//
// Any class can have built-in support for streaming.  For that it should use the Q_DECLARE_STREAM macro in
// order to declare Save and Load member functions; a Stream object is the single parameter for them.

#include "QStream.mcr"

// aggregate classes: the only classes supported to be written and read in the manner of native data types
#include "QMatrix2.h"
#include "QMatrix3.h"
#include "QMatrix4.h"
#include "QColorRgb.h"
#include "QColorRgba.h"

namespace Q
{

class Q_ITEM Stream
{
public:
    enum StreamingType
    {
        ST_WRITING_TO_FILE,
        ST_WRITING_TO_STATIC_BYTESTREAM,
        ST_READING_FROM_FILE,
        ST_READING_FROM_STATIC_BYTESTREAM,
        ST_READING_FROM_DYNAMIC_BYTESTREAM
    };

    enum GeneralStreamingType
    {
        GST_WRITING,
        GST_READING
    };

    // Construction.  For writing of a file as a whole.
    Stream (const char* acFilename, bool bLittleEndian);

    // Construction.  For writing into the own memory buffer.
    Stream (bool bLittleEndian);

    // Construction.  For writing into a still memory block which was allocated independently.
    Stream (char* acStaticBytestream, int iStaticBytestreamSize, bool bLittleEndian);

    // Construction.  For reading of a file as a whole.
    Stream (const char* acFilename, bool bLittleEndian, bool& rbSuccess);

    // Construction.  For writing/reading of some part of a file or a memory block.  For both file writing and
    // file reading the file must exist at the time.  The iOffset parameter defines the number of bytes to be
    // skipped from the beginning of the file or from the provided address in memory.  For writing, the
    // iByteQuantity parameter means the size of the data to be patched; for reading, it is the number of bytes
    // in the data portion needed to be inspected.  In the case of a dynamic bytestream the
    // iDynamicBytestreamSize parameter defines the number of bytes currently available within the block and if
    // iByteQuantity < 0 or iOffset + iByteQuantity > iDynamicBytestreamSize, under-transmission is understood,
    // rbSuccess gets 'false' without occurrence of assert-false, the function returns, and further tries may
    // be done after a time.
    Stream (StreamingType eStreaming, const char* acFilenameOrBytestream, int iOffset, int iByteQuantity,
        bool bLittleEndian, bool& rbSuccess, int iDynamicBytestreamSize = -1);

    // The Finalize function calls AlignBytes from the beginning in order to move the "head" beyond the last
    // significant byte; no moving happens if bit_ratio is 0.  If the Stream object was created for writing
    // into a file, the function flushes the data up to the active byte upon which the "head" is positioned and
    // returns the success of the flushing.  For reading, it restores smart pointers (if any) and returns
    // 'true' if the number of added ids (from the side of Id derived objects) corresponds with the number of
    // mapped ids (from the side of smart pointers) and 'false' otherwise.  Although there is no obvious motive
    // for calling the function in the cases when non-file writing happens or the user knows that there are no
    // smart pointers stored in the data being read, the calling is nevertheless recommended for all cases due
    // to the reason of uniformity and also for the purpose to free some heap memory before actual destruction
    // ('true' is always returned in such situations).
    bool Finalize ();

    // destruction
    ~Stream ();

    // member access
    StreamingType GetStreamingType () const;
    GeneralStreamingType GetGeneralStreaming () const;
    int GetByteQuantity () const;  // size of the buffer (including unused parts)
    int GetBytePosition () const;  // the byte position of the "head"
    int GetBitRatio () const;  // the bit position of the "head" within the active byte; the range is [0, 7]
    void SetBytePosition (int iBytePosition, bool bResetBitRatio = true);
    void OffsetBytePosition (int iOffset);
    void SetBitRatio (int iRatio);
    void OffsetBitPosition (int iOffset);  // any value can be provided
    const char* GetBytes () const;  // the address of the buffer
    operator const char* () const;  // the address of the buffer
    const char* GetBytesUpPosition () const;  // the address of the active byte
    void SetDoNotFreeBufferAfterComplete ();  // if called, the buffer will not be freed on destruction

    // Pushing and popping position records.  A position record contains numerical byte_position as well as
    // bit_ratio.  The stack must be empty on destruction.
    void PushPositionState ();
    void PopPositionState ();

    // operating with bitfields (you may look into Storage class header for the related information)
    void WriteBits (int iValue, int iNumBits);
    bool ReadUBits (int& riValue, int iNumBits);
    bool ReadSBits (int& riValue, int iNumBits);

    // leads to the effect only if bit_ratio is not 0
    void AlignBytes ();

    // writing of native data types
    void Write (bool bValue);
    void Write (int iQuantity, const bool* abValue);
    void Write (char cValue);
    void Write (int iQuantity, const char* acValue);
    void Write (unsigned char ucValue);
    void Write (int iQuantity, const unsigned char* aucValue);
    void Write (short sValue);
    void Write (int iQuantity, const short* asValue);
    void Write (unsigned short usValue);
    void Write (int iQuantity, const unsigned short* ausValue);
    void Write (int iValue);
    void Write (int iQuantity, const int* aiValue);
    void Write (unsigned int uiValue);
    void Write (int iQuantity, const unsigned int* auiValue);
    void Write (long lValue);                                   // 8 bytes per item if compile on a 64-bit machine
    void Write (int iQuantity, const long* alValue);            // 8 bytes per item if compile on a 64-bit machine
    void Write (unsigned long ulValue);                         // 8 bytes per item if compile on a 64-bit machine
    void Write (int iQuantity, const unsigned long* aulValue);  // 8 bytes per item if compile on a 64-bit machine
    void Write (Int64 iValue);
    void Write (int iQuantity, const Int64* aiValue);
    void Write (float fValue);
    void Write (int iQuantity, const float* afValue);
    void Write (double dValue);
    void Write (int iQuantity, const double* adValue);

    // reading of native data types
    bool Read (bool& rbValue);
    bool Read (int iQuantity, bool* abValue);
    bool Read (char& rcValue);
    bool Read (int iQuantity, char* acValue);
    bool Read (unsigned char& rucValue);
    bool Read (int iQuantity, unsigned char* aucValue);
    bool Read (short& rsValue);
    bool Read (int iQuantity, short* asValue);
    bool Read (unsigned short& rusValue);
    bool Read (int iQuantity, unsigned short* ausValue);
    bool Read (int& riValue);
    bool Read (int iQuantity, int* aiValue);
    bool Read (unsigned int& ruiValue);
    bool Read (int iQuantity, unsigned int* auiValue);
    bool Read (long& rlValue);                                  // 8 bytes per item if compile on a 64-bit machine
    bool Read (int iQuantity, long* alValue);                   // 8 bytes per item if compile on a 64-bit machine
    bool Read (unsigned long& rulValue);                        // 8 bytes per item if compile on a 64-bit machine
    bool Read (int iQuantity, unsigned long* aulValue);         // 8 bytes per item if compile on a 64-bit machine
    bool Read (Int64& riValue);
    bool Read (int iQuantity, Int64* aiValue);
    bool Read (float& rfValue);
    bool Read (int iQuantity, float* afValue);
    bool Read (double& rdValue);
    bool Read (int iQuantity, double* adValue);

    // writing of the aggregate classes
    void Write (const Vector2f& rqValue);
    void Write (int iQuantity, const Vector2f* aqValue);
    void Write (const Vector3f& rqValue);
    void Write (int iQuantity, const Vector3f* aqValue);
    void Write (const Vector4f& rqValue);
    void Write (int iQuantity, const Vector4f* aqValue);
    void Write (const Vector2d& rqValue);
    void Write (int iQuantity, const Vector2d* aqValue);
    void Write (const Vector3d& rqValue);
    void Write (int iQuantity, const Vector3d* aqValue);
    void Write (const Vector4d& rqValue);
    void Write (int iQuantity, const Vector4d* aqValue);
    void Write (const Matrix2f& rqValue);
    void Write (int iQuantity, const Matrix2f* aqValue);
    void Write (const Matrix3f& rqValue);
    void Write (int iQuantity, const Matrix3f* aqValue);
    void Write (const Matrix4f& rqValue);
    void Write (int iQuantity, const Matrix4f* aqValue);
    void Write (const Matrix2d& rqValue);
    void Write (int iQuantity, const Matrix2d* aqValue);
    void Write (const Matrix3d& rqValue);
    void Write (int iQuantity, const Matrix3d* aqValue);
    void Write (const Matrix4d& rqValue);
    void Write (int iQuantity, const Matrix4d* aqValue);
    void Write (const ColorRgb& rqValue);
    void Write (int iQuantity, const ColorRgb* aqValue);
    void Write (const ColorRgba& rqValue);
    void Write (int iQuantity, const ColorRgba* aqValue);

    // reading of the aggregate classes
    bool Read (Vector2f& rqValue);
    bool Read (int iQuantity, Vector2f* aqValue);
    bool Read (Vector3f& rqValue);
    bool Read (int iQuantity, Vector3f* aqValue);
    bool Read (Vector4f& rqValue);
    bool Read (int iQuantity, Vector4f* aqValue);
    bool Read (Vector2d& rqValue);
    bool Read (int iQuantity, Vector2d* aqValue);
    bool Read (Vector3d& rqValue);
    bool Read (int iQuantity, Vector3d* aqValue);
    bool Read (Vector4d& rqValue);
    bool Read (int iQuantity, Vector4d* aqValue);
    bool Read (Matrix2f& rqValue);
    bool Read (int iQuantity, Matrix2f* aqValue);
    bool Read (Matrix3f& rqValue);
    bool Read (int iQuantity, Matrix3f* aqValue);
    bool Read (Matrix4f& rqValue);
    bool Read (int iQuantity, Matrix4f* aqValue);
    bool Read (Matrix2d& rqValue);
    bool Read (int iQuantity, Matrix2d* aqValue);
    bool Read (Matrix3d& rqValue);
    bool Read (int iQuantity, Matrix3d* aqValue);
    bool Read (Matrix4d& rqValue);
    bool Read (int iQuantity, Matrix4d* aqValue);
    bool Read (ColorRgb& rqValue);
    bool Read (int iQuantity, ColorRgb* aqValue);
    bool Read (ColorRgba& rqValue);
    bool Read (int iQuantity, ColorRgba* aqValue);

    // Dummy functions for a container class templated with a non-aggregate class which has not a Save/Load
    // function among its member functions.  None of these functions must be called for real.
    void Write (...);
    void Read (...);
    void Write (void* pvValue);
    void Read (void* pvValue);

    // resizing of the buffer above required ("almost private" function)
    void SetByteQuantity (int iNeededByteQuantity);

private:
    friend class Id;

    template <class ObjectT>
    friend class SmartPointer;

    // support for smart pointer restoration
    void AddId (unsigned int uiId, void* pvPointer);
    void MapId (unsigned int uiId, void** ppvPointer);

    StreamingType m_eStreaming;
    GeneralStreamingType m_eGeneralStreaming;
    bool m_bOwner, m_bWholeFile;
    bool m_bFinalized;  // default: false
    bool m_bFreeBufferAfterComplete;  // default: true
    bool m_bLittleEndian;
    int m_iByteQuantity, m_iBytePosition, m_iBitRatio, m_iOffset;
    char* m_acByte;
    char* m_acFilename;
    void* m_pvIdToPPointerMap;
    void* m_pvIdWithPointerArray;
    void* m_pvPositionStack;

    // indicates how much the buffer's size will grow above needed on the reallocation caused by lack of
    // available bytes
    static const int ms_iAdvanceQuantityDivider;  // 3 (on 1/3 of actually required size)
};

#include "QStream.inl"

}

#endif



