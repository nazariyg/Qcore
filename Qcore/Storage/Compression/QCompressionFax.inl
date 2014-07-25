//------------------------------------------------------------------------------------------------------------------
inline void CompressionFax::SetImageDimensions (int iWidth, int iHeight)
{
    assert( iWidth >= 1 && iHeight >= 1 );
    m_iWidth = iWidth;
    m_iHeight = iHeight;
}
//------------------------------------------------------------------------------------------------------------------
inline void CompressionFax::SetKFactor (int iK)
{
    assert( m_bForPacking && m_eEncoding == ET_GROUP_3_2D );
    assert( iK > 0 );
    m_iK = iK;
}
//------------------------------------------------------------------------------------------------------------------
inline void CompressionFax::SetUncompressedModeEncoding (UncompressedModeEncodingType eUncompressedModeEncoding)
{
    assert( !m_bForPacking && (m_eEncoding == ET_GROUP_3_2D || m_eEncoding == ET_GROUP_4) );
    m_eUncompressedModeEncoding = eUncompressedModeEncoding;
}
//------------------------------------------------------------------------------------------------------------------



