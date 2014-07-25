#include "QSmartRttiPointer.h"
using namespace Q;

//------------------------------------------------------------------------------------------------------------------
SmartRttiPointer::SmartRttiPointer (HeapRtti* pqHeapRtti)
    :
    m_pqHeapRtti(pqHeapRtti)
{
    if ( m_pqHeapRtti )
        m_pqHeapRtti->IncrementReferences((void**)&m_pqHeapRtti);
}
//------------------------------------------------------------------------------------------------------------------
SmartRttiPointer::SmartRttiPointer (const SmartRttiPointer& rqPointer)
    :
    m_pqHeapRtti(rqPointer.m_pqHeapRtti)
{
    if ( m_pqHeapRtti )
        m_pqHeapRtti->IncrementReferences((void**)&m_pqHeapRtti);
}
//------------------------------------------------------------------------------------------------------------------
SmartRttiPointer::~SmartRttiPointer ()
{
    if ( m_pqHeapRtti )
        m_pqHeapRtti->DecrementReferences((void**)&m_pqHeapRtti);
}
//------------------------------------------------------------------------------------------------------------------
SmartRttiPointer& SmartRttiPointer::operator= (HeapRtti* pqHeapRtti)
{
    if ( m_pqHeapRtti != pqHeapRtti )
    {
        if ( pqHeapRtti )
            pqHeapRtti->IncrementReferences((void**)&m_pqHeapRtti);
        if ( m_pqHeapRtti )
            m_pqHeapRtti->DecrementReferences((void**)&m_pqHeapRtti);
        m_pqHeapRtti = pqHeapRtti;
    }
    return *this;
}
//------------------------------------------------------------------------------------------------------------------
SmartRttiPointer& SmartRttiPointer::operator= (const SmartRttiPointer& rqPointer)
{
    if ( m_pqHeapRtti != rqPointer.m_pqHeapRtti )
    {
        if ( rqPointer.m_pqHeapRtti )
            rqPointer.m_pqHeapRtti->IncrementReferences((void**)&m_pqHeapRtti);
        if ( m_pqHeapRtti )
            m_pqHeapRtti->DecrementReferences((void**)&m_pqHeapRtti);
        m_pqHeapRtti = rqPointer.m_pqHeapRtti;
    }
    return *this;
}
//------------------------------------------------------------------------------------------------------------------
bool SmartRttiPointer::operator== (HeapRtti* pqHeapRtti) const
{
    return ( m_pqHeapRtti == pqHeapRtti );
}
//------------------------------------------------------------------------------------------------------------------
bool SmartRttiPointer::operator!= (HeapRtti* pqHeapRtti) const
{
    return ( m_pqHeapRtti != pqHeapRtti );
}
//------------------------------------------------------------------------------------------------------------------
bool SmartRttiPointer::operator== (const SmartRttiPointer& rqPointer) const
{
    return ( m_pqHeapRtti == rqPointer.m_pqHeapRtti );
}
//------------------------------------------------------------------------------------------------------------------
bool SmartRttiPointer::operator!= (const SmartRttiPointer& rqPointer) const
{
    return ( m_pqHeapRtti != rqPointer.m_pqHeapRtti );
}
//------------------------------------------------------------------------------------------------------------------



