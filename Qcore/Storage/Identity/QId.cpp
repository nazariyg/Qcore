#include "QId.h"
using namespace Q;

unsigned int Id::ms_uiNextId = 0;

//------------------------------------------------------------------------------------------------------------------
Id::Id ()
    :
    m_uiId(ms_uiNextId++)
{
}
//------------------------------------------------------------------------------------------------------------------
void Id::Save (Stream& rqStream) const
{
    rqStream.Write(m_uiId);
}
//------------------------------------------------------------------------------------------------------------------
void Id::Load (Stream& rqStream)
{
    // only read the former id and register it for potential smart pointer restoration later
    unsigned int uiId;
    rqStream.Read(uiId);
    rqStream.AddId(uiId,(void*)this);
}
//------------------------------------------------------------------------------------------------------------------



