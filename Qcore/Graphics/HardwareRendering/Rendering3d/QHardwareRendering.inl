//------------------------------------------------------------------------------------------------------------------
inline const FramebufferParams& HardwareRendering::GetRequestedFramebufferParams () const
{
    return m_qRequestedFramebufferParams;
}
//------------------------------------------------------------------------------------------------------------------
inline const FramebufferParams& HardwareRendering::GetPresentFramebufferParams () const
{
    return m_qPresentFramebufferParams;
}
//------------------------------------------------------------------------------------------------------------------
inline void HardwareRendering::SetDoVerticalSync (bool bDo)
{
    m_bDoVerticalSync = bDo;
}
//------------------------------------------------------------------------------------------------------------------
inline bool HardwareRendering::GetDoVerticalSync () const
{
    return m_bDoVerticalSync;
}
//------------------------------------------------------------------------------------------------------------------
inline const HRNodefPtr& HardwareRendering::GetRootNodef () const
{
    return m_qRootNodef;
}
//------------------------------------------------------------------------------------------------------------------
inline const HRNodedPtr& HardwareRendering::GetRootNoded () const
{
    return m_qRootNoded;
}
//------------------------------------------------------------------------------------------------------------------
inline int HardwareRendering::GetMaxLights () const
{
    assert( m_bInitialized );
    return m_iMaxLights;
}
//------------------------------------------------------------------------------------------------------------------
inline int HardwareRendering::GetMaxClippingPlanes () const
{
    assert( m_bInitialized );
    return m_iMaxClippingPlanes;
}
//------------------------------------------------------------------------------------------------------------------
inline int HardwareRendering::GetMaxTextureSize () const
{
    assert( m_bInitialized );
    return m_iMaxTextureSize;
}
//------------------------------------------------------------------------------------------------------------------
inline int HardwareRendering::GetMaxModelViewStackSize () const
{
    assert( m_bInitialized );
    return m_iMaxModelViewStackSize;
}
//------------------------------------------------------------------------------------------------------------------
inline int HardwareRendering::GetMaxAttribStackSize () const
{
    assert( m_bInitialized );
    return m_iMaxAttribStackSize;
}
//------------------------------------------------------------------------------------------------------------------
inline int HardwareRendering::GetMaxPixelMapTableSize () const
{
    assert( m_bInitialized );
    return m_iMaxPixelMapTableSize;
}
//------------------------------------------------------------------------------------------------------------------
inline int HardwareRendering::GetMaxTextureCoords () const
{
    assert( m_bInitialized );
    return m_iMaxTextureCoords;
}
//------------------------------------------------------------------------------------------------------------------
inline int HardwareRendering::GetMaxConvolutionFilterWidth () const
{
    assert( m_bInitialized );
    return m_iMaxConvolutionFilterWidth;
}
//------------------------------------------------------------------------------------------------------------------
inline int HardwareRendering::GetMaxConvolutionFilterHeight () const
{
    assert( m_bInitialized );
    return m_iMaxConvolutionFilterHeight;
}
//------------------------------------------------------------------------------------------------------------------



