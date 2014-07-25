//------------------------------------------------------------------------------------------------------------------
inline void FormatGif::SetIsTitleImageInterlaced (bool bInterlaced)
{
    m_bTitleImageIsInterlaced = bInterlaced;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatGif::SetLoopQuantity (int iQuantity)
{
    assert( 0 <= iQuantity && iQuantity <= 65535 );
    m_iLoopQuantity = iQuantity;
}
//------------------------------------------------------------------------------------------------------------------
inline int FormatGif::GetLogicalScreenWidth () const
{
    return m_iLogicalScreenWidth;
}
//------------------------------------------------------------------------------------------------------------------
inline int FormatGif::GetLogicalScreenHeight () const
{
    return m_iLogicalScreenHeight;
}
//------------------------------------------------------------------------------------------------------------------
inline bool FormatGif::IsAnimated () const
{
    return m_bIsAnimated;
}
//------------------------------------------------------------------------------------------------------------------
inline bool FormatGif::IsTitleImageInterlaced () const
{
    return m_bTitleImageIsInterlaced;
}
//------------------------------------------------------------------------------------------------------------------
inline int FormatGif::GetLoopQuantity () const
{
    return m_iLoopQuantity;
}
//------------------------------------------------------------------------------------------------------------------
inline int FormatGif::GetDevelopedRowQuantity () const
{
    assert( m_eStreaming == Stream::ST_READING_FROM_DYNAMIC_BYTESTREAM );
    return m_iDevelopedRowQuantity;
}
//------------------------------------------------------------------------------------------------------------------
inline int FormatGif::GetCurrentPassIndex () const
{
    assert( m_eStreaming == Stream::ST_READING_FROM_DYNAMIC_BYTESTREAM );
    return m_iCurrentPassIndex;
}
//------------------------------------------------------------------------------------------------------------------
inline int FormatGif::GetIrlcRow (int iPass, int iRowWithinPass) const
{
    return ms_aiIrlcStrt[iPass] + iRowWithinPass*ms_aiIrlcStep[iPass];
}
//------------------------------------------------------------------------------------------------------------------



