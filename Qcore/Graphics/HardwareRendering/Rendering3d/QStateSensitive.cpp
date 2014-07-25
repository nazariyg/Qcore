#include "QHardwareRendering.h"
using namespace Q;

//------------------------------------------------------------------------------------------------------------------
StateSensitive::StateSensitive ()
    :
    m_spqTextureStatef(0),
    m_spqTextureStated(0),
    m_bReceiveAllLights(true)
{
}
//------------------------------------------------------------------------------------------------------------------
int StateSensitive::BindState (const SmartRttiPointer& rspqState)
{
    assert( !rspqState->IsExactly(TextureStatef::Class)
        &&  !rspqState->IsExactly(TextureStated::Class) );

    return m_qStates.Push(rspqState);
}
//------------------------------------------------------------------------------------------------------------------
const SmartRttiPointer& StateSensitive::GetState (int i) const
{
    return m_qStates[i];
}
//------------------------------------------------------------------------------------------------------------------
void StateSensitive::UnbindState (int i)
{
    m_qStates.Remove(i);
}
//------------------------------------------------------------------------------------------------------------------
void StateSensitive::UnbindTextureState ()
{
    // not both types simultaneously
    assert( (m_spqTextureStatef && !m_spqTextureStated)
        ||  (!m_spqTextureStatef && m_spqTextureStated) );

    if ( m_spqTextureStatef )
    {
        // notify the Hardware Renderer
        if ( HardwareRendering::ms_pqItself )
        {
            for (int i = 0; i < m_spqTextureStatef->GetTextureQuantity(); i++)
                HardwareRendering::ms_pqItself->OnBufferableLeave(*m_spqTextureStatef->GetTexture(i));
        }

        m_spqTextureStatef = 0;
    }
    else
    {
        // notify the Hardware Renderer
        if ( HardwareRendering::ms_pqItself )
        {
            for (int i = 0; i < m_spqTextureStated->GetTextureQuantity(); i++)
                HardwareRendering::ms_pqItself->OnBufferableLeave(*m_spqTextureStated->GetTexture(i));
        }

        m_spqTextureStated = 0;
    }
}
//------------------------------------------------------------------------------------------------------------------



