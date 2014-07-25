//------------------------------------------------------------------------------------------------------------------
inline bool ProgressBase::GetProgressFlag () const
{
    return m_bDoes;
}
//------------------------------------------------------------------------------------------------------------------
inline float ProgressBase::GetProgressState () const
{
    float fDone = m_fDone;
    if ( fDone < 0.0f )
        fDone = 0.0f;
    else if ( fDone > 1.0f )
        fDone = 1.0f;
    return fDone;
}
//------------------------------------------------------------------------------------------------------------------
inline void ProgressBase::SetProgressCallback (void (*oCallback)(float))
{
    m_oCallback = oCallback;
}
//------------------------------------------------------------------------------------------------------------------
inline void ProgressBase::RedirectProgressTo (ProgressBase* pqObject, ProgressProbe& rqProbe, float fContribFactor)
{
    m_pqRedirProbe = &rqProbe;
    m_fRedirDoneBefore = pqObject->m_fDone;
    m_fRedirContribFactor = fContribFactor;
    m_bRedirMode = true;
}
//------------------------------------------------------------------------------------------------------------------



