//------------------------------------------------------------------------------------------------------------------
inline const char* ClassLabel::GetName () const
{
    return m_acName;
}
//------------------------------------------------------------------------------------------------------------------
inline bool ClassLabel::IsExactly (const ClassLabel& rqClass) const
{
    return ( &rqClass == this );
}
//------------------------------------------------------------------------------------------------------------------



