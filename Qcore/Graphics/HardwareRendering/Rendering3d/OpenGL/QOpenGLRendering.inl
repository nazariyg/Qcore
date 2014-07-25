//------------------------------------------------------------------------------------------------------------------
inline int OpenGLRendering::GetContextQuantity () const
{
    return m_qIdToContext.GetQuantity();
}
//------------------------------------------------------------------------------------------------------------------
inline int OpenGLRendering::GetCurrentContext () const
{
    assert( m_iCurrentContextId != 0 );
    return m_iCurrentContextId;
}
//------------------------------------------------------------------------------------------------------------------
inline const String& OpenGLRendering::GetGLVendorInfo () const
{
    return m_qGLVendorInfo;
}
//------------------------------------------------------------------------------------------------------------------
inline const String& OpenGLRendering::GetGLRendererInfo () const
{
    return m_qGLRendererInfo;
}
//------------------------------------------------------------------------------------------------------------------
inline const String& OpenGLRendering::GetGLVersionInfo () const
{
    return m_qGLVersionInfo;
}
//------------------------------------------------------------------------------------------------------------------
inline const String& OpenGLRendering::GetGLExtensionInfo () const
{
    return m_qGLExtensionInfo;
}
//------------------------------------------------------------------------------------------------------------------
inline void OpenGLRendering::SetViewport (int iX, int iY, int iWidth, int iHeight)
{
    assert( m_pqCurrentContext );
    m_pqCurrentContext->Viewport = Recti(iX,iY,iWidth,iHeight);
}
//------------------------------------------------------------------------------------------------------------------
inline void OpenGLRendering::SetViewport (const Recti& rqViewport)
{
    assert( m_pqCurrentContext );
    m_pqCurrentContext->Viewport = rqViewport;
}
//------------------------------------------------------------------------------------------------------------------
inline const Recti& OpenGLRendering::GetViewport () const
{
    assert( m_pqCurrentContext );
    return m_pqCurrentContext->Viewport;
}
//------------------------------------------------------------------------------------------------------------------
inline void OpenGLRendering::SetDoColorBufferClear (bool bDo)
{
    assert( m_pqCurrentContext );
    m_pqCurrentContext->DoColorBufferClear = bDo;
}
//------------------------------------------------------------------------------------------------------------------
inline bool OpenGLRendering::GetDoColorBufferClear () const
{
    assert( m_pqCurrentContext );
    return m_pqCurrentContext->DoColorBufferClear;
}
//------------------------------------------------------------------------------------------------------------------
inline void OpenGLRendering::SetDoDepthBufferClear (bool bDo)
{
    assert( m_pqCurrentContext );
    m_pqCurrentContext->DoDepthBufferClear = bDo;
}
//------------------------------------------------------------------------------------------------------------------
inline bool OpenGLRendering::GetDoDepthBufferClear () const
{
    assert( m_pqCurrentContext );
    return m_pqCurrentContext->DoDepthBufferClear;
}
//------------------------------------------------------------------------------------------------------------------
inline void OpenGLRendering::SetBackgroundColor (const ColorRgba& rqColor)
{
    assert( m_pqCurrentContext );
    m_pqCurrentContext->BackgroundColor = rqColor;
}
//------------------------------------------------------------------------------------------------------------------
inline const ColorRgba& OpenGLRendering::GetBackgroundColor () const
{
    assert( m_pqCurrentContext );
    return m_pqCurrentContext->BackgroundColor;
}
//------------------------------------------------------------------------------------------------------------------
inline void OpenGLRendering::SetDepthBufferClearValue (float fValue)
{
    assert( m_pqCurrentContext );
    m_pqCurrentContext->DepthBufferClearValue = fValue;
}
//------------------------------------------------------------------------------------------------------------------
inline float OpenGLRendering::GetDepthBufferClearValue () const
{
    assert( m_pqCurrentContext );
    return m_pqCurrentContext->DepthBufferClearValue;
}
//------------------------------------------------------------------------------------------------------------------
inline void OpenGLRendering::SetCameraf (const HRCamera3fPtr& rspqCamera)
{
    assert( m_pqCurrentContext );

    m_pqCurrentContext->Camerad = 0;
    m_pqCurrentContext->Cameraf = rspqCamera;
}
//------------------------------------------------------------------------------------------------------------------
inline const HRCamera3fPtr& OpenGLRendering::GetCameraf () const
{
    assert( m_pqCurrentContext );
    return m_pqCurrentContext->Cameraf;
}
//------------------------------------------------------------------------------------------------------------------
inline void OpenGLRendering::SetCamerad (const HRCamera3dPtr& rspqCamera)
{
    assert( m_pqCurrentContext );

    m_pqCurrentContext->Cameraf = 0;
    m_pqCurrentContext->Camerad = rspqCamera;
}
//------------------------------------------------------------------------------------------------------------------
inline const HRCamera3dPtr& OpenGLRendering::GetCamerad () const
{
    assert( m_pqCurrentContext );
    return m_pqCurrentContext->Camerad;
}
//------------------------------------------------------------------------------------------------------------------
inline int OpenGLRendering::GetGlobalStateQuantity () const
{
    return m_qGlobalStates.GetStateQuantity();
}
//------------------------------------------------------------------------------------------------------------------
inline const SmartRttiPointer& OpenGLRendering::GetGlobalState (int i) const
{
    return m_qGlobalStates.GetState(i);
}
//------------------------------------------------------------------------------------------------------------------
inline bool OpenGLRendering::IsBlendColorPresent () const
{
    assert( m_bInitialized );
    return ( QGLExists_GL_EXT_blend_color == QGL_EXT_FULLY_EXISTS );
}
//------------------------------------------------------------------------------------------------------------------
inline bool OpenGLRendering::IsBlendEquationSeparatePresent () const
{
    assert( m_bInitialized );
    return ( QGLExists_GL_EXT_blend_equation_separate == QGL_EXT_FULLY_EXISTS );
}
//------------------------------------------------------------------------------------------------------------------
inline bool OpenGLRendering::IsBlendFuncSeparatePresent () const
{
    assert( m_bInitialized );
    return ( QGLExists_GL_EXT_blend_func_separate == QGL_EXT_FULLY_EXISTS );
}
//------------------------------------------------------------------------------------------------------------------
inline bool OpenGLRendering::IsBlendLogicOpPresent () const
{
    assert( m_bInitialized );
    return ( QGLExists_GL_EXT_blend_logic_op == QGL_EXT_FULLY_EXISTS );
}
//------------------------------------------------------------------------------------------------------------------
inline bool OpenGLRendering::IsBlendMinmaxPresent () const
{
    assert( m_bInitialized );
    return ( QGLExists_GL_EXT_blend_minmax == QGL_EXT_FULLY_EXISTS );
}
//------------------------------------------------------------------------------------------------------------------
inline bool OpenGLRendering::IsBlendSquarePresent () const
{
    assert( m_bInitialized );
    return ( QGLExists_GL_NV_blend_square == QGL_EXT_FULLY_EXISTS );
}
//------------------------------------------------------------------------------------------------------------------
inline bool OpenGLRendering::IsBlendSubtractPresent () const
{
    assert( m_bInitialized );
    return ( QGLExists_GL_EXT_blend_subtract == QGL_EXT_FULLY_EXISTS );
}
//------------------------------------------------------------------------------------------------------------------
inline bool OpenGLRendering::IsColorMatrixPresent () const
{
    assert( m_bInitialized );
    return ( QGLExists_GL_EXT_color_matrix == QGL_EXT_FULLY_EXISTS );
}
//------------------------------------------------------------------------------------------------------------------
inline bool OpenGLRendering::IsColorTablePresent () const
{
    assert( m_bInitialized );
    return ( QGLExists_GL_SGI_color_table == QGL_EXT_FULLY_EXISTS );
}
//------------------------------------------------------------------------------------------------------------------
inline bool OpenGLRendering::IsConvolutionPresent () const
{
    assert( m_bInitialized );
    return ( QGLExists_GL_EXT_convolution == QGL_EXT_FULLY_EXISTS );
}
//------------------------------------------------------------------------------------------------------------------
inline bool OpenGLRendering::IsDepthTexturePresent () const
{
    assert( m_bInitialized );
    return ( QGLExists_GL_ARB_depth_texture == QGL_EXT_FULLY_EXISTS );
}
//------------------------------------------------------------------------------------------------------------------
inline bool OpenGLRendering::IsFogCoordPresent () const
{
    assert( m_bInitialized );
    return ( QGLExists_GL_EXT_fog_coord == QGL_EXT_FULLY_EXISTS );
}
//------------------------------------------------------------------------------------------------------------------
inline bool OpenGLRendering::IsGenerateMipmapPresent () const
{
    assert( m_bInitialized );
    return ( QGLExists_GL_SGIS_generate_mipmap == QGL_EXT_FULLY_EXISTS );
}
//------------------------------------------------------------------------------------------------------------------
inline bool OpenGLRendering::IsMultisamplePresent () const
{
    assert( m_bInitialized );
    return ( QGLExists_GL_ARB_multisample == QGL_EXT_FULLY_EXISTS );
}
//------------------------------------------------------------------------------------------------------------------
inline bool OpenGLRendering::IsMultitexturePresent () const
{
    assert( m_bInitialized );
    return ( QGLExists_GL_ARB_multitexture == QGL_EXT_FULLY_EXISTS );
}
//------------------------------------------------------------------------------------------------------------------
inline bool OpenGLRendering::IsPackedPixelsPresent () const
{
    assert( m_bInitialized );
    return ( QGLExists_GL_EXT_packed_pixels == QGL_EXT_FULLY_EXISTS );
}
//------------------------------------------------------------------------------------------------------------------
inline bool OpenGLRendering::IsPointParametersPresent () const
{
    assert( m_bInitialized );
    return ( QGLExists_GL_EXT_point_parameters == QGL_EXT_FULLY_EXISTS );
}
//------------------------------------------------------------------------------------------------------------------
inline bool OpenGLRendering::IsPointSpritePresent () const
{
    assert( m_bInitialized );
    return ( QGLExists_GL_NV_point_sprite == QGL_EXT_FULLY_EXISTS );
}
//------------------------------------------------------------------------------------------------------------------
inline bool OpenGLRendering::IsSecondaryColorPresent () const
{
    assert( m_bInitialized );
    return ( QGLExists_GL_EXT_secondary_color == QGL_EXT_FULLY_EXISTS );
}
//------------------------------------------------------------------------------------------------------------------
inline bool OpenGLRendering::IsSeparateSpecularColorPresent () const
{
    assert( m_bInitialized );
    return ( QGLExists_GL_EXT_separate_specular_color == QGL_EXT_FULLY_EXISTS );
}
//------------------------------------------------------------------------------------------------------------------
inline bool OpenGLRendering::IsSeparateStencilPresent () const
{
    assert( m_bInitialized );
    return ( QGLExists_GL_ATI_separate_stencil == QGL_EXT_FULLY_EXISTS );
}
//------------------------------------------------------------------------------------------------------------------
inline bool OpenGLRendering::IsShadowFuncsPresent () const
{
    assert( m_bInitialized );
    return ( QGLExists_GL_EXT_shadow_funcs == QGL_EXT_FULLY_EXISTS );
}
//------------------------------------------------------------------------------------------------------------------
inline bool OpenGLRendering::IsShadowPresent () const
{
    assert( m_bInitialized );
    return ( QGLExists_GL_ARB_shadow == QGL_EXT_FULLY_EXISTS );
}
//------------------------------------------------------------------------------------------------------------------
inline bool OpenGLRendering::IsStencilMaskSeparatePresent () const
{
    assert( m_bInitialized );
    return ( glStencilMaskSeparate != 0 );
}
//------------------------------------------------------------------------------------------------------------------
inline bool OpenGLRendering::IsTexgenReflectionPresent () const
{
    assert( m_bInitialized );
    return ( QGLExists_GL_NV_texgen_reflection == QGL_EXT_FULLY_EXISTS );
}
//------------------------------------------------------------------------------------------------------------------
inline bool OpenGLRendering::IsTexParameterLodBiasPresent () const
{
    assert( m_bInitialized );
    return ( QGLExists_GL_VERSION_1_4 == QGL_EXT_FULLY_EXISTS );
}
//------------------------------------------------------------------------------------------------------------------
inline bool OpenGLRendering::IsTexture3dPresent () const
{
    assert( m_bInitialized );
    return ( QGLExists_GL_EXT_texture3D == QGL_EXT_FULLY_EXISTS );
}
//------------------------------------------------------------------------------------------------------------------
inline bool OpenGLRendering::IsTextureBorderClampPresent () const
{
    assert( m_bInitialized );
    return ( QGLExists_GL_ARB_texture_border_clamp == QGL_EXT_FULLY_EXISTS );
}
//------------------------------------------------------------------------------------------------------------------
inline bool OpenGLRendering::IsTextureCompressionFxt1Present () const
{
    assert( m_bInitialized );
    return ( QGLExists_GL_3DFX_texture_compression_FXT1 == QGL_EXT_FULLY_EXISTS );
}
//------------------------------------------------------------------------------------------------------------------
inline bool OpenGLRendering::IsTextureCompressionPresent () const
{
    assert( m_bInitialized );
    return ( QGLExists_GL_ARB_texture_compression == QGL_EXT_FULLY_EXISTS );
}
//------------------------------------------------------------------------------------------------------------------
inline bool OpenGLRendering::IsTextureCompressionS3tcPresent () const
{
    assert( m_bInitialized );
    return ( QGLExists_GL_EXT_texture_compression_s3tc == QGL_EXT_FULLY_EXISTS );
}
//------------------------------------------------------------------------------------------------------------------
inline bool OpenGLRendering::IsTextureCubeMapPresent () const
{
    assert( m_bInitialized );
    return ( QGLExists_GL_EXT_texture_cube_map == QGL_EXT_FULLY_EXISTS );
}
//------------------------------------------------------------------------------------------------------------------
inline bool OpenGLRendering::IsTextureEdgeClampPresent () const
{
    assert( m_bInitialized );
    return ( QGLExists_GL_SGIS_texture_edge_clamp == QGL_EXT_FULLY_EXISTS );
}
//------------------------------------------------------------------------------------------------------------------
inline bool OpenGLRendering::IsTextureEnvAddPresent () const
{
    assert( m_bInitialized );
    return ( QGLExists_GL_ARB_texture_env_add == QGL_EXT_FULLY_EXISTS );
}
//------------------------------------------------------------------------------------------------------------------
inline bool OpenGLRendering::IsTextureEnvCombinePresent () const
{
    assert( m_bInitialized );
    return ( QGLExists_GL_ARB_texture_env_combine == QGL_EXT_FULLY_EXISTS );
}
//------------------------------------------------------------------------------------------------------------------
inline bool OpenGLRendering::IsTextureEnvCrossbarPresent () const
{
    assert( m_bInitialized );
    return ( QGLExists_GL_ARB_texture_env_crossbar == QGL_EXT_FULLY_EXISTS );
}
//------------------------------------------------------------------------------------------------------------------
inline bool OpenGLRendering::IsTextureEnvDot3Present () const
{
    assert( m_bInitialized );
    return ( QGLExists_GL_ARB_texture_env_dot3 == QGL_EXT_FULLY_EXISTS );
}
//------------------------------------------------------------------------------------------------------------------
inline bool OpenGLRendering::IsTextureLodPresent () const
{
    assert( m_bInitialized );
    return ( QGLExists_GL_SGIS_texture_lod == QGL_EXT_FULLY_EXISTS );
}
//------------------------------------------------------------------------------------------------------------------
inline bool OpenGLRendering::IsTextureMirroredRepeatPresent () const
{
    assert( m_bInitialized );
    return ( QGLExists_GL_IBM_texture_mirrored_repeat == QGL_EXT_FULLY_EXISTS );
}
//------------------------------------------------------------------------------------------------------------------
inline bool OpenGLRendering::IsTextureNonPowerOfTwoPresent () const
{
    assert( m_bInitialized );
    return ( QGLExists_GL_ARB_texture_non_power_of_two == QGL_EXT_FULLY_EXISTS );
}
//------------------------------------------------------------------------------------------------------------------
inline bool OpenGLRendering::IsTypeUnsignedShort565Present () const
{
    assert( m_bInitialized );
    return ( QGLExists_GL_VERSION_1_2 == QGL_EXT_FULLY_EXISTS );
}
//------------------------------------------------------------------------------------------------------------------
inline bool OpenGLRendering::IsWindowPosPresent () const
{
    assert( m_bInitialized );
    return ( QGLExists_GL_ARB_window_pos == QGL_EXT_FULLY_EXISTS );
}
//------------------------------------------------------------------------------------------------------------------



