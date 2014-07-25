//------------------------------------------------------------------------------------------------------------------
inline int StateSensitive::GetStateQuantity () const
{
    return m_qStates.GetQuantity();
}
//------------------------------------------------------------------------------------------------------------------
inline void StateSensitive::BindTextureStatef (const TextureStatefPtr& rspqTextureState)
{
    // not both types simultaneously
    assert( !m_spqTextureStated );

    m_spqTextureStatef = rspqTextureState;
}
//------------------------------------------------------------------------------------------------------------------
inline void StateSensitive::BindTextureStated (const TextureStatedPtr& rspqTextureState)
{
    // not both types simultaneously
    assert( !m_spqTextureStatef );

    m_spqTextureStated = rspqTextureState;
}
//------------------------------------------------------------------------------------------------------------------
inline const TextureStatefPtr& StateSensitive::GetTextureStatef () const
{
    return m_spqTextureStatef;
}
//------------------------------------------------------------------------------------------------------------------
inline const TextureStatedPtr& StateSensitive::GetTextureStated () const
{
    return m_spqTextureStated;
}
//------------------------------------------------------------------------------------------------------------------
inline void StateSensitive::SetReceiveAllLights (bool bSet)
{
    m_bReceiveAllLights = bSet;
}
//------------------------------------------------------------------------------------------------------------------
inline bool StateSensitive::GetReceiveAllLights () const
{
    return m_bReceiveAllLights;
}
//------------------------------------------------------------------------------------------------------------------



