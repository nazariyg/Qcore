//------------------------------------------------------------------------------------------------------------------
inline void FormatSct::SetDpi (int iHorizontalDpi, int iVerticalDpi)
{
    assert( iHorizontalDpi > 0 && iVerticalDpi > 0 );

    m_iHorizontalDpi = iHorizontalDpi;
    m_iVerticalDpi = iVerticalDpi;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatSct::SetOrientation (OrientationType eOrientation)
{
    m_eOrientation = eOrientation;
}
//------------------------------------------------------------------------------------------------------------------
inline void FormatSct::SetNameComment (const String& rqNameComment)
{
    assert( rqNameComment.GetLength() <= 80 );
    m_qNameComment = rqNameComment;
}
//------------------------------------------------------------------------------------------------------------------
inline int FormatSct::GetWidth () const
{
    return m_iWidth;
}
//------------------------------------------------------------------------------------------------------------------
inline int FormatSct::GetHeight () const
{
    return m_iHeight;
}
//------------------------------------------------------------------------------------------------------------------
inline FormatSct::ColorModelType FormatSct::GetColorModel () const
{
    return m_eColorModel;
}
//------------------------------------------------------------------------------------------------------------------
inline FormatSct::OrientationType FormatSct::GetOrientation () const
{
    return m_eOrientation;
}
//------------------------------------------------------------------------------------------------------------------
inline int FormatSct::GetHorizontalDpi () const
{
    return m_iHorizontalDpi;
}
//------------------------------------------------------------------------------------------------------------------
inline int FormatSct::GetVerticalDpi () const
{
    return m_iVerticalDpi;
}
//------------------------------------------------------------------------------------------------------------------
inline const String& FormatSct::GetNameComment () const
{
    return m_qNameComment;
}
//------------------------------------------------------------------------------------------------------------------



