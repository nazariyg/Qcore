//------------------------------------------------------------------------------------------------------------------
#define BEFORE_CHUNK_DATA_WRITE(typebytes) \
{ \
    rqStream.SetByteQuantity(rqStream.GetBytePosition()+4); \
    rqStream.OffsetBytePosition(4); \
    rqStream.Write(4,typebytes); \
    iChunkDataBeginningPos = rqStream.GetBytePosition(); \
}
//------------------------------------------------------------------------------------------------------------------
#define AFTER_CHUNK_DATA_WRITE \
{ \
    uiLength = rqStream.GetBytePosition() - iChunkDataBeginningPos; \
    rqStream.PushPositionState(); \
    rqStream.SetBytePosition(rqStream.GetBytePosition()-uiLength-8); \
    rqStream.Write(uiLength); \
    rqStream.PopPositionState(); \
    uiCrc = Storage::GetCrc32Checksum(rqStream.GetBytesUpPosition()-uiLength-4,4+uiLength); \
    rqStream.Write(uiCrc); \
}
//------------------------------------------------------------------------------------------------------------------
#define CHECK_LATIN1_NONEMPTY_LIMITED80_STRING_FOR_SAVING(string) \
{ \
    assert( 1 <= string.GetLength() && string.GetLength() <= 79 ); \
    unsigned char ucChar, ucPrevChar = 32; \
    for (int iC = 0; iC < string.GetLength(); iC++) \
    { \
        ucChar = string[iC]; \
        assert( !(ucChar == 32 && ucPrevChar == 32) ); \
        assert( (32 <= ucChar && ucChar <= 126) || (161 <= ucChar && ucChar <= 255) ); \
        ucPrevChar = ucChar; \
    } \
    assert( !(!string.IsEmpty() && string[string.GetLength()-1] == 32) ); \
}
//------------------------------------------------------------------------------------------------------------------
#define CHECK_TEXT_FOR_SAVING(string) \
{ \
    unsigned char ucChar; \
    for (int iC = 0; iC < string.GetLength(); iC++) \
    { \
        ucChar = string[iC]; \
        assert( ucChar == 9 || ucChar == 10 || ucChar == 12 || ucChar == 13 \
            ||  (32 <= ucChar && ucChar <= 126) || (161 <= ucChar && ucChar <= 255) ); \
    } \
}
//------------------------------------------------------------------------------------------------------------------
#define COMPUTE_EXPECTED_IMAGE_SIZE_AND_RELATED_VALUES \
{ \
    if ( m_eInterlaceMethod == IMT_NONE ) \
    { \
        int iRowSizeInBits = m_iWidth*m_iBitsPerPixel; \
        m_iRowSizeWithFB = ( (iRowSizeInBits & 7) == 0 ? iRowSizeInBits/8 : iRowSizeInBits/8 + 1 ) + 1; \
        m_iExpectedImageSize = m_iHeight*m_iRowSizeWithFB; \
    } \
    else \
    { \
        m_aiWidthInPass[0] = (m_iWidth + 7)/8; \
        m_aiWidthInPass[1] = (m_iWidth + 3)/8; \
        m_aiWidthInPass[2] = (m_iWidth + 3)/4; \
        m_aiWidthInPass[3] = (m_iWidth + 1)/4; \
        m_aiWidthInPass[4] = (m_iWidth + 1)/2; \
        m_aiWidthInPass[5] = (m_iWidth + 0)/2; \
        m_aiWidthInPass[6] = (m_iWidth + 0)/1; \
        int iRSIB0 = m_iBitsPerPixel*m_aiWidthInPass[0]; \
        int iRSIB1 = m_iBitsPerPixel*m_aiWidthInPass[1]; \
        int iRSIB2 = m_iBitsPerPixel*m_aiWidthInPass[2]; \
        int iRSIB3 = m_iBitsPerPixel*m_aiWidthInPass[3]; \
        int iRSIB4 = m_iBitsPerPixel*m_aiWidthInPass[4]; \
        int iRSIB5 = m_iBitsPerPixel*m_aiWidthInPass[5]; \
        int iRSIB6 = m_iBitsPerPixel*m_aiWidthInPass[6]; \
        m_aiRowSizeWithFBInPass[0] = ( (iRSIB0 & 7) == 0 ? iRSIB0/8 : iRSIB0/8 + 1 ) + ( m_iWidth > 0 ? 1 : 0 ); \
        m_aiRowSizeWithFBInPass[1] = ( (iRSIB1 & 7) == 0 ? iRSIB1/8 : iRSIB1/8 + 1 ) + ( m_iWidth > 4 ? 1 : 0 ); \
        m_aiRowSizeWithFBInPass[2] = ( (iRSIB2 & 7) == 0 ? iRSIB2/8 : iRSIB2/8 + 1 ) + ( m_iWidth > 0 ? 1 : 0 ); \
        m_aiRowSizeWithFBInPass[3] = ( (iRSIB3 & 7) == 0 ? iRSIB3/8 : iRSIB3/8 + 1 ) + ( m_iWidth > 2 ? 1 : 0 ); \
        m_aiRowSizeWithFBInPass[4] = ( (iRSIB4 & 7) == 0 ? iRSIB4/8 : iRSIB4/8 + 1 ) + ( m_iWidth > 0 ? 1 : 0 ); \
        m_aiRowSizeWithFBInPass[5] = ( (iRSIB5 & 7) == 0 ? iRSIB5/8 : iRSIB5/8 + 1 ) + ( m_iWidth > 1 ? 1 : 0 ); \
        m_aiRowSizeWithFBInPass[6] = ( (iRSIB6 & 7) == 0 ? iRSIB6/8 : iRSIB6/8 + 1 ) + ( m_iWidth > 0 ? 1 : 0 ); \
        m_aiHeightInPass[0] = (m_iHeight + 7)/8; \
        m_aiHeightInPass[1] = (m_iHeight + 7)/8; \
        m_aiHeightInPass[2] = (m_iHeight + 3)/8; \
        m_aiHeightInPass[3] = (m_iHeight + 3)/4; \
        m_aiHeightInPass[4] = (m_iHeight + 1)/4; \
        m_aiHeightInPass[5] = (m_iHeight + 1)/2; \
        m_aiHeightInPass[6] = (m_iHeight + 0)/2; \
        m_aiPassSize[0] = m_aiRowSizeWithFBInPass[0]*m_aiHeightInPass[0]; \
        m_aiPassSize[1] = m_aiRowSizeWithFBInPass[1]*m_aiHeightInPass[1]; \
        m_aiPassSize[2] = m_aiRowSizeWithFBInPass[2]*m_aiHeightInPass[2]; \
        m_aiPassSize[3] = m_aiRowSizeWithFBInPass[3]*m_aiHeightInPass[3]; \
        m_aiPassSize[4] = m_aiRowSizeWithFBInPass[4]*m_aiHeightInPass[4]; \
        m_aiPassSize[5] = m_aiRowSizeWithFBInPass[5]*m_aiHeightInPass[5]; \
        m_aiPassSize[6] = m_aiRowSizeWithFBInPass[6]*m_aiHeightInPass[6]; \
        m_iExpectedImageSize = 0; \
        for (int iP = 0; iP < 7; iP++) \
            m_iExpectedImageSize += m_aiPassSize[iP]; \
    } \
}
//------------------------------------------------------------------------------------------------------------------
#define PROCESS_FILTERING_FOR_SAVING(height,firstrowptr,rowsizewithfb) \
{ \
    StillArray<int> qFilterTypes; \
    if ( m_eColorModel != CMT_INDEXED ) \
    { \
        GetFilterTypesForRows(qFilterTypes,height,firstrowptr,rowsizewithfb); \
    } \
    else \
    { \
        qFilterTypes.SetQuantity(height); \
        MemSet(qFilterTypes,0,height*4); \
    } \
 \
    unsigned char* pucRow = firstrowptr; \
    StillArray<unsigned char> qOrigUpRow(rowsizewithfb); \
    StillArray<unsigned char> qOrigRow(rowsizewithfb); \
    for (int iFY = 0; iFY < height; iFY++) \
    { \
        MemCpy(qOrigRow,pucRow,rowsizewithfb); \
        AssignFBAndFilterRow(qFilterTypes[iFY],iFY,pucRow,qOrigRow,qOrigUpRow,rowsizewithfb); \
 \
        pucRow += rowsizewithfb; \
        qOrigUpRow = qOrigRow; \
    } \
}
//------------------------------------------------------------------------------------------------------------------
#define LEAVE_ANCILLARY_CHUNK(warning) \
{ \
    THROW_WARNING(warning); \
    goto DoneWithThisChunk; \
}
//------------------------------------------------------------------------------------------------------------------
#define CONDITIONAL_LEAVE_ANCILLARY_CHUNK(expression,warning) \
{ \
    if ( !expression ) \
        LEAVE_ANCILLARY_CHUNK(warning); \
}
//------------------------------------------------------------------------------------------------------------------
#define CHECK_CRC_FOR_ANCILLARY_CHUNK(warning) \
{ \
    qChunkStream.OffsetBytePosition(uiLength); \
    qChunkStream.Read(uiCrc); \
    CONDITIONAL_LEAVE_ANCILLARY_CHUNK(( uiCrc == Storage::GetCrc32Checksum(qChunkStream,4+uiLength) ),warning); \
    qChunkStream.SetBytePosition(4); \
}
//------------------------------------------------------------------------------------------------------------------
#define READ_AND_PROCESS_LATIN1_NONEMPTY_LIMITED80_STRING_FOR_ANCILLARY_CHUNK(stringoffset,string,warning) \
{ \
    bNullFound = false; \
    pcNullSearchStart = qChunkStream + stringoffset; \
    pcChar = pcNullSearchStart; \
    iVar = uiLength - (stringoffset - 4); \
    for (int iC = 0; iC < 80 && iC < iVar; iC++) \
    { \
        if ( *pcChar++ == 0 ) \
        { \
            iNullPos = iC; \
            bNullFound = true; \
            break; \
        } \
    } \
    CONDITIONAL_LEAVE_ANCILLARY_CHUNK(( bNullFound && iNullPos != 0 ),warning); \
    string.SetLength(iNullPos); \
    MemCpy(string,pcNullSearchStart,iNullPos); \
    if ( ms_bCorrectLatin1Limited80StringsForLoading ) \
    { \
        unsigned char ucPrevChar = 32; \
        for (int iC = 0; iC < string.GetLength(); iC++) \
        { \
            ucChar = string[iC]; \
            if ( ucChar == 32 && ucPrevChar == 32 ) \
            { \
                string.Remove(iC,1); \
                iC--; \
                continue; \
            } \
            if ( !((32 <= ucChar && ucChar <= 126) || (161 <= ucChar && ucChar <= 255)) ) \
                string[iC] = 63; \
            ucPrevChar = ucChar; \
        } \
        if ( !string.IsEmpty() && string[string.GetLength()-1] == 32 ) \
            string.Remove(string.GetLength()-1,1); \
    } \
    qChunkStream.SetBytePosition(stringoffset+iNullPos+1); \
}
//------------------------------------------------------------------------------------------------------------------
#define PROCESS_TEXT_FOR_LOADING(string) \
{ \
    if ( ms_bCorrectTextsForLoading ) \
    { \
        for (int iC = 0; iC < string.GetLength(); iC++) \
        { \
            ucChar = string[iC]; \
            if ( !(ucChar == 9 || ucChar == 10 || ucChar == 12 || ucChar == 13 \
            ||   (32 <= ucChar && ucChar <= 126) || (161 <= ucChar && ucChar <= 255)) ) \
            { \
                string[iC] = 63; \
            } \
        } \
    } \
}
//------------------------------------------------------------------------------------------------------------------
#define READ_tIME_CHUNK \
{ \
    CHECK_CRC_FOR_ANCILLARY_CHUNK(WT_ANCILLARY_CHUNK_tIME_IS_INCORRECT); \
    CONDITIONAL_LEAVE_ANCILLARY_CHUNK(( uiLength == 7 ),WT_ANCILLARY_CHUNK_tIME_IS_INCORRECT); \
 \
    unsigned short usYear; \
    unsigned char ucMonth, ucDay, ucHour, ucMinute, ucSecond; \
    qChunkStream.Read(usYear); \
    qChunkStream.Read(ucMonth); \
    qChunkStream.Read(ucDay); \
    qChunkStream.Read(ucHour); \
    qChunkStream.Read(ucMinute); \
    qChunkStream.Read(ucSecond); \
    CONDITIONAL_LEAVE_ANCILLARY_CHUNK(( \
         1 <= ucMonth && ucMonth <= 12 \
    &&   1 <= ucDay && ucDay <= 31 \
    &&   0 <= ucHour && ucHour <= 23 \
    &&   0 <= ucMinute && ucMinute <= 59 \
    &&   0 <= ucSecond && ucSecond <= 60 ),WT_ANCILLARY_CHUNK_tIME_IS_INCORRECT); \
 \
    m_qLastModificationTime = Date(usYear,ucMonth,ucDay,ucHour,ucMinute,( ucSecond != 60 ? ucSecond : 59 )); \
 \
    m_bHas_tIME_Chunk = true; \
}
//------------------------------------------------------------------------------------------------------------------
#define READ_tEXt_CHUNK \
{ \
    CHECK_CRC_FOR_ANCILLARY_CHUNK(WT_ONE_OR_MORE_ANCILLARY_CHUNKS_tEXt_ARE_INCORRECT); \
    CONDITIONAL_LEAVE_ANCILLARY_CHUNK(( uiLength >= 2 ),WT_ONE_OR_MORE_ANCILLARY_CHUNKS_tEXt_ARE_INCORRECT); \
 \
    TextualData qTextualData; \
 \
    READ_AND_PROCESS_LATIN1_NONEMPTY_LIMITED80_STRING_FOR_ANCILLARY_CHUNK(4,qTextualData.Keyword, \
        WT_ONE_OR_MORE_ANCILLARY_CHUNKS_tEXt_ARE_INCORRECT); \
 \
    int iRemainder = uiLength - (qChunkStream.GetBytePosition() - 4); \
    CONDITIONAL_LEAVE_ANCILLARY_CHUNK(( iRemainder >= 0 ),WT_ONE_OR_MORE_ANCILLARY_CHUNKS_tEXt_ARE_INCORRECT); \
    qTextualData.Text.SetLength(iRemainder); \
    qChunkStream.Read(iRemainder,qTextualData.Text); \
 \
    PROCESS_TEXT_FOR_LOADING(qTextualData.Text); \
 \
    m_qTextualDataArray.Push(qTextualData); \
}
//------------------------------------------------------------------------------------------------------------------
#define READ_zTXt_CHUNK \
{ \
    CHECK_CRC_FOR_ANCILLARY_CHUNK(WT_ONE_OR_MORE_ANCILLARY_CHUNKS_zTXt_ARE_INCORRECT); \
    CONDITIONAL_LEAVE_ANCILLARY_CHUNK(( uiLength >= 3 ),WT_ONE_OR_MORE_ANCILLARY_CHUNKS_zTXt_ARE_INCORRECT); \
 \
    TextualData qTextualData; \
 \
    READ_AND_PROCESS_LATIN1_NONEMPTY_LIMITED80_STRING_FOR_ANCILLARY_CHUNK(4,qTextualData.Keyword, \
        WT_ONE_OR_MORE_ANCILLARY_CHUNKS_zTXt_ARE_INCORRECT); \
 \
    unsigned char ucCompression; \
    qChunkStream.Read(ucCompression); \
    CONDITIONAL_LEAVE_ANCILLARY_CHUNK(( ucCompression == 0 ), \
        WT_ONE_OR_MORE_ANCILLARY_CHUNKS_zTXt_ARE_INCORRECT); \
 \
    int iRemainder = uiLength - (qChunkStream.GetBytePosition() - 4); \
    CONDITIONAL_LEAVE_ANCILLARY_CHUNK(( iRemainder >= 0 ),WT_ONE_OR_MORE_ANCILLARY_CHUNKS_zTXt_ARE_INCORRECT); \
    StillArray<char> qDText; \
    CompressionZLib qZLib; \
    CONDITIONAL_LEAVE_ANCILLARY_CHUNK( \
        qZLib.Decompress(qChunkStream.GetBytesUpPosition(),iRemainder,qDText), \
        WT_ONE_OR_MORE_ANCILLARY_CHUNKS_zTXt_ARE_INCORRECT); \
    qTextualData.Text.SetLength(qDText.GetQuantity()); \
    MemCpy(qTextualData.Text,qDText,qDText.GetQuantity()); \
 \
    PROCESS_TEXT_FOR_LOADING(qTextualData.Text); \
 \
    m_qTextualDataArray.Push(qTextualData); \
}
//------------------------------------------------------------------------------------------------------------------



