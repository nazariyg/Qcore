//------------------------------------------------------------------------------------------------------------------
inline RasterImage::ImageType RasterImageSequence::GetFrameImageType () const
{
    return m_eFrameImageType;
}
//------------------------------------------------------------------------------------------------------------------
inline int RasterImageSequence::GetScreenWidth () const
{
    return m_iScreenWidth;
}
//------------------------------------------------------------------------------------------------------------------
inline int RasterImageSequence::GetScreenHeight () const
{
    return m_iScreenHeight;
}
//------------------------------------------------------------------------------------------------------------------
inline int RasterImageSequence::GetFrameQuantity () const
{
    return m_qFrames.GetQuantity();
}
//------------------------------------------------------------------------------------------------------------------
inline void RasterImageSequence::SetFrameImage (const RasterImagePtr& rspqImage, int i)
{
    assert( rspqImage->GetType() == m_eFrameImageType );
    assert( rspqImage->GetWidth() == m_iScreenWidth && rspqImage->GetHeight() == m_iScreenHeight );

    m_qFrames[i] = rspqImage;
}
//------------------------------------------------------------------------------------------------------------------
inline const RasterImagePtr& RasterImageSequence::GetFrameImage (int i) const
{
    return m_qFrames[i];
}
//------------------------------------------------------------------------------------------------------------------
inline void RasterImageSequence::SetFrameQuantity (int iFrameQuantity)
{
    m_qFrames.SetQuantity(iFrameQuantity);
}
//------------------------------------------------------------------------------------------------------------------



