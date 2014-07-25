#include "QHardwareRendering.h"
using namespace Q;

//------------------------------------------------------------------------------------------------------------------
HRBufferable::HRBufferable ()
    :
    NeedRebuffer(true),
    BufferId(0),
    m_eBufferProfile(PT_IMMEDIATE)
{
}
//------------------------------------------------------------------------------------------------------------------
#ifdef _DEBUG
HRBufferable::~HRBufferable ()
{
    // must be unbufferd
    assert( BufferId == 0 );
}
#endif
//------------------------------------------------------------------------------------------------------------------
void HRBufferable::SetBufferProfile (BufferProfileType eBufferProfile)
{
    if ( m_eBufferProfile == PT_BUFFER && eBufferProfile == PT_IMMEDIATE )
    {
        // notify the Hardware Renderer
        if ( HardwareRendering::ms_pqItself )
            HardwareRendering::ms_pqItself->OnBufferableLeave(*this);
    }

    m_eBufferProfile = eBufferProfile;
}
//------------------------------------------------------------------------------------------------------------------
void HRBufferable::Unbuffer ()
{
    if ( HardwareRendering::ms_pqItself )
        HardwareRendering::ms_pqItself->OnBufferableLeave(*this);
}
//------------------------------------------------------------------------------------------------------------------



