//------------------------------------------------------------------------------------------------------------------
inline void FormatDds::SetDxtQuality (DxtCompressionQualityType eDxtQuality)
{
    assert( m_bDxtCompression );
    m_eDxtQuality = eDxtQuality;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatDds::SetDxtWeightingFactors (float fFcrR, float fFcrG, float fFcrB)
{
    assert( m_bDxtCompression );

    m_fDxtWeigFcrR = fFcrR;
    m_fDxtWeigFcrG = fFcrG;
    m_fDxtWeigFcrB = fFcrB;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatDds::SetDxt1aThreshold (int iThreshold)
{
    assert( m_bDxtCompression );
    m_iDxt1aThreshold = iThreshold;
}
//------------------------------------------------------------------------------------------------------------------
inline FormatDds::ResourceType FormatDds::GetResourceType () const
{
    return m_eResource;
}
//------------------------------------------------------------------------------------------------------------------
inline bool FormatDds::AreMainImageDimensionsPowerOfTwo () const
{
    return ( IsPowerOfTwo(m_iMainImageWidth) && IsPowerOfTwo(m_iMainImageHeight) );
}
//------------------------------------------------------------------------------------------------------------------
inline int FormatDds::GetMainImageWidth () const
{
    return m_iMainImageWidth;
}
//------------------------------------------------------------------------------------------------------------------
inline int FormatDds::GetMainImageHeight () const
{
    return m_iMainImageHeight;
}
//------------------------------------------------------------------------------------------------------------------
inline int FormatDds::GetBitsPerPixel () const
{
    return m_iBitsPerPixel;
}
//------------------------------------------------------------------------------------------------------------------
inline FormatDds::ColorModelType FormatDds::GetColorModel () const
{
    return m_eColorModel;
}
//------------------------------------------------------------------------------------------------------------------
inline FormatDds::CompressionType FormatDds::GetCompression () const
{
    return m_eCompression;
}
//------------------------------------------------------------------------------------------------------------------
inline bool FormatDds::HasNonFullResolutionImages () const
{
    return m_bHasNonFullResImages;
}
//------------------------------------------------------------------------------------------------------------------
inline int FormatDds::GetResolutionLevelQuantity () const
{
    return m_iResolutionLevelQuantity;
}
//------------------------------------------------------------------------------------------------------------------
inline bool FormatDds::HasAllFaces () const
{
    assert( m_eResource == RT_CUBIC_ENVIRONMENT_MAP );

    return ( m_bHasFacePositiveX && m_bHasFaceNegativeX
        &&   m_bHasFacePositiveY && m_bHasFaceNegativeY
        &&   m_bHasFacePositiveZ && m_bHasFaceNegativeZ );
}
//------------------------------------------------------------------------------------------------------------------
inline bool FormatDds::HasFacePositiveX () const
{
    assert( m_eResource == RT_CUBIC_ENVIRONMENT_MAP );
    return m_bHasFacePositiveX;
}
//------------------------------------------------------------------------------------------------------------------
inline bool FormatDds::HasFaceNegativeX () const
{
    assert( m_eResource == RT_CUBIC_ENVIRONMENT_MAP );
    return m_bHasFaceNegativeX;
}
//------------------------------------------------------------------------------------------------------------------
inline bool FormatDds::HasFacePositiveY () const
{
    assert( m_eResource == RT_CUBIC_ENVIRONMENT_MAP );
    return m_bHasFacePositiveY;
}
//------------------------------------------------------------------------------------------------------------------
inline bool FormatDds::HasFaceNegativeY () const
{
    assert( m_eResource == RT_CUBIC_ENVIRONMENT_MAP );
    return m_bHasFaceNegativeY;
}
//------------------------------------------------------------------------------------------------------------------
inline bool FormatDds::HasFacePositiveZ () const
{
    assert( m_eResource == RT_CUBIC_ENVIRONMENT_MAP );
    return m_bHasFacePositiveZ;
}
//------------------------------------------------------------------------------------------------------------------
inline bool FormatDds::HasFaceNegativeZ () const
{
    assert( m_eResource == RT_CUBIC_ENVIRONMENT_MAP );
    return m_bHasFaceNegativeZ;
}
//------------------------------------------------------------------------------------------------------------------
inline int FormatDds::GetVolumeTextureDepth () const
{
    assert( m_eResource == RT_VOLUME_TEXTURE );
    return m_iVolumeTextureDepth;
}
//------------------------------------------------------------------------------------------------------------------



