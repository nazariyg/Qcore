//------------------------------------------------------------------------------------------------------------------
inline void CompressionZLib::SetSpeedSizeBalance (float fBalance)
{
    assert( 0.0f <= fBalance && fBalance <= 1.0f );
    m_fSpeedSizeBalance = fBalance;
}
//------------------------------------------------------------------------------------------------------------------
inline void CompressionZLib::SetCompressionStrategy (CompressionStrategyType eStrategy)
{
    m_eCompressionStrategy = eStrategy;
}
//------------------------------------------------------------------------------------------------------------------
inline void CompressionZLib::SetDynamicalDecompressionMode ()
{
    m_bDynamicalDecompressionMode = true;
}
//------------------------------------------------------------------------------------------------------------------



