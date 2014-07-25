//------------------------------------------------------------------------------------------------------------------
inline void CompressionDxt::SetDxt1aThreshold (int iThreshold)
{
    assert( m_eCompression == CT_DXT_1 );
    assert( 0 <= iThreshold && iThreshold <= 255 );

    m_iDxt1aThreshold = iThreshold;
}
//------------------------------------------------------------------------------------------------------------------



