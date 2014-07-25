//------------------------------------------------------------------------------------------------------------------
#define CONC_PROCESS_STREAM_SUCCESS_FOR_LOADING(success,concession) \
{ \
    if ( !success ) \
    { \
        if ( m_eStreaming != Stream::ST_READING_FROM_DYNAMIC_BYTESTREAM ) \
        { \
            if ( !concession ) \
                PROCESS_STREAM_SUCCESS_FOR_LOADING(success) \
            else \
                goto Exit; \
        } \
        else \
        { \
            return false; \
        } \
    } \
}
//------------------------------------------------------------------------------------------------------------------
#define ELSE_IF_COMMENT_OR_APPLICATION_EXTENSION(concession) \
    else if ( ucByte == 254 || ucByte == 255 ) \
    { \
        if ( ucByte == 255 ) \
            iNextReadPos += 12; \
        Stream qFirstBlockSizeStream(m_eStreaming,m_acStreamSrcParam,iNextReadPos++,1,true,bSuccess, \
            iBytestreamSize); \
        CONC_PROCESS_STREAM_SUCCESS_FOR_LOADING(bSuccess,concession) \
        qFirstBlockSizeStream.Read(ucByte); \
        qFirstBlockSizeStream.Finalize(); \
        if ( ucByte != 0 ) \
        { \
            for (/**/; /**/; /**/) \
            { \
                iNextReadPos += ucByte; \
                Stream qBlockSizeStream(m_eStreaming,m_acStreamSrcParam,iNextReadPos++,1,true,bSuccess, \
                    iBytestreamSize); \
                CONC_PROCESS_STREAM_SUCCESS_FOR_LOADING(bSuccess,concession) \
                qBlockSizeStream.Read(ucByte); \
                qBlockSizeStream.Finalize(); \
                if ( ucByte == 0 ) \
                    break; \
            } \
        } \
        else \
        { \
            Stream qBlockSizeStream(m_eStreaming,m_acStreamSrcParam,iNextReadPos++,1,true,bSuccess, \
                iBytestreamSize); \
            CONC_PROCESS_STREAM_SUCCESS_FOR_LOADING(bSuccess,concession) \
            qBlockSizeStream.Read(ucByte); \
            qBlockSizeStream.Finalize(); \
            if ( !(ucByte == 0 || ucByte == 59) ) \
            { \
                if ( !concession ) \
                    THROW_EXCEPTION(ET_FORMAT_MISMATCH) \
                else \
                    goto Exit; \
            } \
        } \
    }
//------------------------------------------------------------------------------------------------------------------
#define REACH_IMAGE_DESCRIPTOR_AND_READ_IT_WITH_LOCAL_COLOR_TABLE(concession) \
{ \
    for (/**/; /**/; /**/) \
    { \
        Stream qByteStream(m_eStreaming,m_acStreamSrcParam,iNextReadPos++,1,true,bSuccess,iBytestreamSize); \
        CONC_PROCESS_STREAM_SUCCESS_FOR_LOADING(bSuccess,concession) \
        qByteStream.Read(ucByte); \
        qByteStream.Finalize(); \
        if ( ucByte == 0 ) \
        { \
        } \
        else if ( ucByte == 33 ) \
        { \
            Stream qLabelStream(m_eStreaming,m_acStreamSrcParam,iNextReadPos++,1,true,bSuccess,iBytestreamSize); \
            CONC_PROCESS_STREAM_SUCCESS_FOR_LOADING(bSuccess,concession) \
            qLabelStream.Read(ucByte); \
            qLabelStream.Finalize(); \
            if ( ucByte == 1 ) \
            { \
                if ( !concession ) \
                    THROW_EXCEPTION(ET_PLAIN_TEXT_EXTENSION_NOT_SUPPORTED) \
                else \
                    goto Exit; \
            } \
            ELSE_IF_COMMENT_OR_APPLICATION_EXTENSION(concession) \
            else \
            { \
                if ( !concession ) \
                    THROW_EXCEPTION(ET_FORMAT_MISMATCH) \
                else \
                    goto Exit; \
            } \
        } \
        else if ( ucByte == 44 ) \
        { \
            iNextReadPos--; \
            break; \
        } \
        else \
        { \
            if ( !concession ) \
                THROW_EXCEPTION(ET_FORMAT_MISMATCH) \
            else \
                goto Exit; \
        } \
    } \
 \
    iNextReadPos++; \
    Stream qIDStream(m_eStreaming,m_acStreamSrcParam,iNextReadPos,9,true,bSuccess,iBytestreamSize); \
    CONC_PROCESS_STREAM_SUCCESS_FOR_LOADING(bSuccess,concession) \
 \
    bFitsLSD = true; \
 \
    qIDStream.Read(usImageLeftPosition); \
    qIDStream.Read(usImageTopPosition); \
    qIDStream.Read(usImageWidth); \
    qIDStream.Read(usImageHeight); \
    if ( usImageWidth == 0 || usImageHeight == 0 \
    ||   usImageLeftPosition >= m_iLogicalScreenWidth \
    ||   usImageTopPosition >= m_iLogicalScreenHeight ) \
    { \
        if ( !concession ) \
            THROW_EXCEPTION(ET_FORMAT_MISMATCH) \
        else \
            goto Exit; \
    } \
    if ( usImageLeftPosition + usImageWidth > m_iLogicalScreenWidth \
    ||   usImageTopPosition + usImageHeight > m_iLogicalScreenHeight ) \
    { \
        bFitsLSD = false; \
    } \
 \
    int iLocalColorTableFlag; \
    qIDStream.ReadUBits(iLocalColorTableFlag,1); \
    bHasLocalColorTable = ( iLocalColorTableFlag == 1 ); \
 \
    int iInterlaceFlag; \
    qIDStream.ReadUBits(iInterlaceFlag,1); \
    bInterlaced = ( iInterlaceFlag == 1 ); \
 \
    qIDStream.OffsetBitPosition(3); \
 \
    int iSizeOfLocalColorTable; \
    qIDStream.ReadUBits(iSizeOfLocalColorTable,3); \
    int iLocalColorTableSize; \
    if ( bHasLocalColorTable ) \
        iLocalColorTableSize = 1 << (iSizeOfLocalColorTable + 1); \
 \
    qIDStream.Finalize(); \
 \
    iNextReadPos += 9; \
 \
    if ( bHasLocalColorTable ) \
    { \
        qLocalColorTable.SetQuantity(iLocalColorTableSize); \
        int iSizeInBytes = iLocalColorTableSize*3; \
        Stream qLCTStream(m_eStreaming,m_acStreamSrcParam,iNextReadPos,iSizeInBytes,true,bSuccess, \
            iBytestreamSize); \
        CONC_PROCESS_STREAM_SUCCESS_FOR_LOADING(bSuccess,concession) \
        for (int i = 0; i < iLocalColorTableSize; i++) \
        { \
            PaletteColor& rqPaletteColor = qLocalColorTable[i]; \
 \
            qLCTStream.Read(rqPaletteColor.R()); \
            qLCTStream.Read(rqPaletteColor.G()); \
            qLCTStream.Read(rqPaletteColor.B()); \
        } \
        qLCTStream.Finalize(); \
 \
        iNextReadPos += iSizeInBytes; \
    } \
    else \
    { \
        if ( !m_bHasGlobalColorTable ) \
        { \
            if ( !concession ) \
                THROW_EXCEPTION(ET_FORMAT_MISMATCH) \
            else \
                goto Exit; \
        } \
    } \
}
//------------------------------------------------------------------------------------------------------------------
#define ANIM_PROCESS_STREAM_SUCCESS_FOR_LOADING(success) \
{ \
    if ( !success ) \
    { \
        if ( m_eStreaming != Stream::ST_READING_FROM_DYNAMIC_BYTESTREAM ) \
            goto Exit; \
        else \
            return false; \
    } \
}
//------------------------------------------------------------------------------------------------------------------
#define UPDATE_DONE \
{ \
    if ( m_eStreaming != Stream::ST_READING_FROM_DYNAMIC_BYTESTREAM ) \
    { \
        qProgress.MakeAdvance(); \
    } \
    else \
    { \
        if ( !bInterlaced ) \
            qProgress.SetDone(((float)m_iDevelopedRowQuantity)/iHeight); \
        else \
            qProgress.SetDone(((float)m_iCurrentPassIndex+((float)m_iDevelopedRowQuantity)/iHeight)/4); \
    } \
}
//------------------------------------------------------------------------------------------------------------------



