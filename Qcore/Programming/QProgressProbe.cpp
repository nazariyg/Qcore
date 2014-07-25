#include "QProgressProbe.h"
#include "QMath.h"
using namespace Q;

const int ProgressProbe::ms_iDoneDeltaQuantity(100);
const float ProgressProbe::ms_fInvDoneDeltaQuantity(1.0f/ms_iDoneDeltaQuantity);

//------------------------------------------------------------------------------------------------------------------
ProgressProbe::ProgressProbe (ProgressBase* pqProgress, float fAlreadyDone)
    :
    m_pqProgress(pqProgress)
{
    if ( fAlreadyDone < 0.0f )
        fAlreadyDone = 0.0f;
    else if ( fAlreadyDone > 1.0f )
        fAlreadyDone = 1.0f;
    m_fDone = Mathf::Floor(fAlreadyDone*ms_iDoneDeltaQuantity)*ms_fInvDoneDeltaQuantity;

    // in this order
    OnUpdate();
    pqProgress->m_bDoes = true;
}
//------------------------------------------------------------------------------------------------------------------
ProgressProbe::~ProgressProbe ()
{
    m_pqProgress->m_bDoes = false;
}
//------------------------------------------------------------------------------------------------------------------
void ProgressProbe::MakeAdvance ()
{
    SetDone(m_fDone+m_fAdvance);
}
//------------------------------------------------------------------------------------------------------------------
void ProgressProbe::MakeOneTimeAdvance (float fAdvance)
{
    SetDone(m_fDone+fAdvance);
}
//------------------------------------------------------------------------------------------------------------------
void ProgressProbe::SetDone (float fDone)
{
    if ( fDone < 0.0f )
        fDone = 0.0f;
    else if ( fDone > 1.0f )
        fDone = 1.0f;
    fDone = Mathf::Floor(fDone*ms_iDoneDeltaQuantity)*ms_fInvDoneDeltaQuantity;

    if ( fDone >= m_fDone + ms_fInvDoneDeltaQuantity )
    {
        m_fDone = fDone;
        OnUpdate();
    }
}
//------------------------------------------------------------------------------------------------------------------
void ProgressProbe::OnUpdate () const
{
    m_pqProgress->m_fDone = m_fDone;

    if ( m_pqProgress->m_oCallback )
        (*m_pqProgress->m_oCallback)(m_fDone);

    if ( m_pqProgress->m_bRedirMode )
    {
        m_pqProgress->m_pqRedirProbe->SetDone(
            m_pqProgress->m_fRedirDoneBefore+m_pqProgress->m_fRedirContribFactor*m_fDone);
    }
}
//------------------------------------------------------------------------------------------------------------------



