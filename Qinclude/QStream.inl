//------------------------------------------------------------------------------------------------------------------
inline Stream::StreamingType Stream::GetStreamingType () const
{
    return m_eStreaming;
}
//------------------------------------------------------------------------------------------------------------------
inline Stream::GeneralStreamingType Stream::GetGeneralStreaming () const
{
    return m_eGeneralStreaming;
}
//------------------------------------------------------------------------------------------------------------------
inline int Stream::GetByteQuantity () const
{
    return m_iByteQuantity;
}
//------------------------------------------------------------------------------------------------------------------
inline int Stream::GetBytePosition () const
{
    return m_iBytePosition;
}
//------------------------------------------------------------------------------------------------------------------
inline int Stream::GetBitRatio () const
{
    return m_iBitRatio;
}
//------------------------------------------------------------------------------------------------------------------
inline void Stream::SetBytePosition (int iBytePosition, bool bResetBitRatio)
{
    assert( 0 <= iBytePosition && iBytePosition <= m_iByteQuantity );
    m_iBytePosition = iBytePosition;
    if ( bResetBitRatio )
        m_iBitRatio = 0;
}
//------------------------------------------------------------------------------------------------------------------
inline void Stream::OffsetBytePosition (int iOffset)
{
    m_iBytePosition += iOffset;
}
//------------------------------------------------------------------------------------------------------------------
inline void Stream::SetBitRatio (int iRatio)
{
    assert( 0 <= iRatio && iRatio < 8 );
    m_iBitRatio = iRatio;
}
//------------------------------------------------------------------------------------------------------------------
inline void Stream::OffsetBitPosition (int iOffset)
{
    int iBitSum = m_iBitRatio + iOffset;
    if ( iBitSum >= 0 )
    {
        m_iBytePosition += iBitSum/8;
        m_iBitRatio = iBitSum & 7;
    }
    else
    {
        iBitSum = -iBitSum;
        int iMod8 = iBitSum & 7;
        if ( iMod8 != 0 )
        {
            m_iBytePosition -= iBitSum/8 + 1;
            m_iBitRatio = 8 - iMod8;
        }
        else
        {
            m_iBytePosition -= iBitSum/8;
            m_iBitRatio = 0;
        }
    }
    assert( 0 <= m_iBytePosition && m_iBytePosition <= m_iByteQuantity );
}
//------------------------------------------------------------------------------------------------------------------
inline const char* Stream::GetBytes () const
{
    return m_acByte;
}
//------------------------------------------------------------------------------------------------------------------
inline Stream::operator const char* () const
{
    return m_acByte;
}
//------------------------------------------------------------------------------------------------------------------
inline const char* Stream::GetBytesUpPosition () const
{
    return m_acByte + m_iBytePosition;
}
//------------------------------------------------------------------------------------------------------------------
inline void Stream::SetDoNotFreeBufferAfterComplete ()
{
    m_bFreeBufferAfterComplete = false;
}
//------------------------------------------------------------------------------------------------------------------



