//------------------------------------------------------------------------------------------------------------------
#define COMPLETE_AND_SAVE_IFF_ROW \
{ \
    while ( iBytePos != iIffRowByteSize ) \
        iBytePos += Storage::WriteBits(qIffRow+iBytePos,iBitRatio,1,0); \
 \
    if ( m_eCompression == CT_NONE ) \
    { \
        qSaveStream.Write(iIffRowByteSize,qIffRow); \
    } \
    else \
    { \
        StillArray<char> qCompressedIffRow; \
        CompressionPackBits::Compress(qIffRow,iIffRowByteSize,qCompressedIffRow); \
        qSaveStream.Write(qCompressedIffRow.GetQuantity(),qCompressedIffRow); \
    } \
}
//------------------------------------------------------------------------------------------------------------------
#define PROCESS_MASKING \
{ \
    if ( m_eMasking == MT_HAS_MASK ) \
    { \
        iBytePos = 0; \
        iBitRatio = 0; \
        for (int iX = 0; iX < m_iWidth; iX++) \
        { \
            iValue = ( m_spqImageSaveTo->IsPixelVisible(iX,iY) ? 1 : 0 ); \
            iBytePos += Storage::WriteBits(qIffRow+iBytePos,iBitRatio,1,iValue); \
        } \
 \
        COMPLETE_AND_SAVE_IFF_ROW; \
    } \
}
//------------------------------------------------------------------------------------------------------------------



