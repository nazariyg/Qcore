//------------------------------------------------------------------------------------------------------------------
inline bool ExptWarnBase::GetExceptionFlag () const
{
    return m_bExceptionFlag;
}
//------------------------------------------------------------------------------------------------------------------
inline int ExptWarnBase::GetExceptionType () const
{
    assert( m_bExceptionFlag );
    return m_iExceptionType;
}
//------------------------------------------------------------------------------------------------------------------
inline int ExptWarnBase::GetWarningQuantity () const
{
    return m_qWarnings.GetQuantity();
}
//------------------------------------------------------------------------------------------------------------------
inline int ExptWarnBase::GetWarningType (int i) const
{
    return m_qWarnings[i];
}
//------------------------------------------------------------------------------------------------------------------
inline void ExptWarnBase::OnException (int iExceptionType)
{
    m_iExceptionType = iExceptionType;
    m_bExceptionFlag = true;
}
//------------------------------------------------------------------------------------------------------------------
inline void ExptWarnBase::OnWarning (int iWarningType)
{
    bool bWarnIsAlreadyIn = false;
    for (int iWarn = 0; iWarn < m_qWarnings.GetQuantity(); iWarn++)
    {
        if ( iWarningType == m_qWarnings[iWarn] )
        {
            bWarnIsAlreadyIn = true;
            break;
        }
    }
    if ( !bWarnIsAlreadyIn )
        m_qWarnings.Push(iWarningType);
}
//------------------------------------------------------------------------------------------------------------------



