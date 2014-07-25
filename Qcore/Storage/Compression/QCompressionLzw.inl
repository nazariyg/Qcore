//------------------------------------------------------------------------------------------------------------------
inline void CompressionLzw::SetSymbolLength (int iSymbolLength)
{
    assert( m_eEncoding == ET_FREE || m_eEncoding == ET_GIF );
    assert( iSymbolLength >= 1 );
    m_iSymbolLength = iSymbolLength;
}
//------------------------------------------------------------------------------------------------------------------
inline void CompressionLzw::SetMaximalCodeLength (int iMaximalCodeLength)
{
    assert( m_eEncoding == ET_FREE );
    assert( iMaximalCodeLength >= 2 );
    m_iMaximalCodeLength = iMaximalCodeLength;
}
//------------------------------------------------------------------------------------------------------------------
inline void CompressionLzw::SetDynamicalDecompressionMode ()
{
    m_bDynamicalDecompressionMode = true;
}
//------------------------------------------------------------------------------------------------------------------
inline bool CompressionLzw::HasDecompressionStateRecord () const
{
    assert( m_bDynamicalDecompressionMode );
    return m_bHasDecompressionStateRecord;
}
//------------------------------------------------------------------------------------------------------------------
inline int CompressionLzw::GetDecodedSymbolQuantity () const
{
    assert( m_bDynamicalDecompressionMode && m_bHasDecompressionStateRecord );
    return m_qIDSR.DecodedSymbolQuantity;
}
//------------------------------------------------------------------------------------------------------------------
inline int CompressionLzw::Concatenate (int iPrefixStringIndex, int iExtensionSymbol) const
{
    return iExtensionSymbol | (iPrefixStringIndex << m_iSymbolLength);
}
//------------------------------------------------------------------------------------------------------------------



