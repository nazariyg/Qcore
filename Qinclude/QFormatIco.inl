//------------------------------------------------------------------------------------------------------------------
inline FormatIco::FileResourceType FormatIco::GetFileResourceType () const
{
    return m_eFileResource;
}
//------------------------------------------------------------------------------------------------------------------
inline int FormatIco::GetImageQuantity () const
{
    return m_iImageQuantity;
}
//------------------------------------------------------------------------------------------------------------------
inline const FormatIco::ImageRecord& FormatIco::GetImageRecord (int i) const
{
    return m_qImageRecords[i];
}
//------------------------------------------------------------------------------------------------------------------



