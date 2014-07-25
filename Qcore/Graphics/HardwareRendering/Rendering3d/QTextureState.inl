//------------------------------------------------------------------------------------------------------------------
template <class Real>
TextureState<Real>::TextureState ()
    :
    PointSpriteSet(false)
{
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
void TextureState<Real>::SetPointSprite (PointSpriteCoordOriginType ePointSpriteCoordOrigin)
{
    PointSpriteCoordOrigin = ePointSpriteCoordOrigin;
    PointSpriteSet = true;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
void TextureState<Real>::UnsetPointSprite ()
{
    PointSpriteSet = false;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
int TextureState<Real>::GetTextureQuantity () const
{
    return m_qTextures.GetQuantity();
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
int TextureState<Real>::AddTexture (const SmartPointer<HRTexture<Real> >& rspqTexture)
{
    return m_qTextures.Push(rspqTexture);
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
const SmartPointer<HRTexture<Real> >& TextureState<Real>::GetTexture (int i) const
{
    return m_qTextures[i];
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
void TextureState<Real>::RemoveTexture (int i)
{
    // notify the Hardware Renderer
    if ( HardwareRendering::ms_pqItself )
        HardwareRendering::ms_pqItself->OnBufferableLeave(*m_qTextures[i]);

    m_qTextures.Remove(i);
}
//------------------------------------------------------------------------------------------------------------------



