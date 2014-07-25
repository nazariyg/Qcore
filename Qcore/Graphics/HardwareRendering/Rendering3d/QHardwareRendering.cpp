#include "QHardwareRendering.h"
using namespace Q;

// to be used by unrelated classes
HardwareRendering* HardwareRendering::ms_pqItself(0);

//------------------------------------------------------------------------------------------------------------------
HardwareRendering::HardwareRendering (const FramebufferParams& rqRequestedFramebufferParams)
    :
    m_qRequestedFramebufferParams(rqRequestedFramebufferParams),
    m_bDoVerticalSync(false),
    m_bInitialized(false)
{
    // only one instance allowed
    assert( !ms_pqItself );

    ms_pqItself = this;
}
//------------------------------------------------------------------------------------------------------------------
HardwareRendering::~HardwareRendering ()
{
    ms_pqItself = 0;
}
//------------------------------------------------------------------------------------------------------------------



