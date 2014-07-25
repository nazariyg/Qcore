//------------------------------------------------------------------------------------------------------------------
#define COMPRESS_ROW_AND_WRITE \
{ \
    StillArray<char> qCompressedRow; \
    CompressionPackBits::Compress(qDecompressedRow,iRowSize,qCompressedRow); \
    qByteCounts[iBCC++] = qCompressedRow.GetQuantity(); \
    qSaveStream.Write(qCompressedRow.GetQuantity(),qCompressedRow); \
}
//------------------------------------------------------------------------------------------------------------------
#define RETURN_AND_WRITE_ROWS_BYTE_COUNTS \
{ \
    if ( m_eCompression == CT_RLE ) \
    { \
        qSaveStream.PushPositionState(); \
        qSaveStream.SetBytePosition(iBeforeByteCounts); \
        for (int iBC = 0; iBC < iBCQuantity; iBC++) \
            qSaveStream.Write(qByteCounts[iBC]); \
        qSaveStream.PopPositionState(); \
    } \
}
//------------------------------------------------------------------------------------------------------------------
#define ASSIGN_SAMPLE \
{ \
    if ( bIsMainChannel ) \
    { \
        if ( m_eColorModel == CMT_CMYK ) \
            ucValue = 255 - ucValue; \
        pucComp[rqChannelInfo.Id] = ucValue; \
    } \
    else if ( rqLR.PosRect.Contains(PosRectToUse.X+iX,PosRectToUse.Y+iY) ) \
    { \
        if ( bInvertMask ) \
            ucValue = 255 - ucValue; \
        pucComp[iMCQ] = Mathf::RoundToInt(((float)(pucComp[iMCQ]*ucValue))/255); \
    } \
    pucComp += rqLayer.ImagePtr->GetBytesPerPixel(); \
}
//------------------------------------------------------------------------------------------------------------------



