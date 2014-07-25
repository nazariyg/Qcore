#include "QHeap.h"
using namespace Q;

//------------------------------------------------------------------------------------------------------------------
Heap::Heap ()
{
}
//------------------------------------------------------------------------------------------------------------------
Heap::~Heap ()
{
    // empty
}
//------------------------------------------------------------------------------------------------------------------
void Heap::Delete ()
{
    // set all referencing pointers to zero
    for (int i = 0; i < m_qReferences.GetQuantity(); i++)
        *m_qReferences[i] = 0;

    // destruct
    delete this;
}
//------------------------------------------------------------------------------------------------------------------
void Heap::IncrementReferences (void** ppvReference)
{
    // has to be new
    m_qReferences.Push(ppvReference);
}
//------------------------------------------------------------------------------------------------------------------
void Heap::DecrementReferences (void** ppvReference)
{
    // linear search through the registry
    for (int i = 0; i < m_qReferences.GetQuantity(); i++)
    {
        if ( m_qReferences[i] == ppvReference )
        {
            // have to be unique
            m_qReferences.Remove(i);
            break;
        }
    }

    if ( m_qReferences.IsEmpty() )
    {
        // it was the last reference, so destruct
        delete this;
    }
}
//------------------------------------------------------------------------------------------------------------------



