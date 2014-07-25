//------------------------------------------------------------------------------------------------------------------
template <class ElementT>
UspNode<ElementT>::UspNode ()
    :
    m_pqPrior(0),
    m_pqNext(0)
{
}
//------------------------------------------------------------------------------------------------------------------
template <class ElementT>
UspNode<ElementT>::UspNode (const ElementT& rtElement, UspNode* pqPrior)
    :
    m_tElement(rtElement),
    m_pqPrior(pqPrior),
    m_pqNext(0)
{
}
//------------------------------------------------------------------------------------------------------------------
template <class ElementT>
void UspNode<ElementT>::SetPriorNode (UspNode* pqPrior)
{
    m_pqPrior = pqPrior;
}
//------------------------------------------------------------------------------------------------------------------
template <class ElementT>
void UspNode<ElementT>::AttachNextNode (UspNode* pqNext)
{
    m_pqNext = pqNext;
}
//------------------------------------------------------------------------------------------------------------------
template <class ElementT>
void UspNode<ElementT>::DetachNextNode ()
{
    m_pqNext = 0;
}
//------------------------------------------------------------------------------------------------------------------
template <class ElementT>
ElementT& UspNode<ElementT>::GetElement ()
{
    return m_tElement;
}
//------------------------------------------------------------------------------------------------------------------
template <class ElementT>
const ElementT& UspNode<ElementT>::GetElement () const
{
    return m_tElement;
}
//------------------------------------------------------------------------------------------------------------------
template <class ElementT>
UspNode<ElementT>* UspNode<ElementT>::GetPriorNode ()
{
    return m_pqPrior;
}
//------------------------------------------------------------------------------------------------------------------
template <class ElementT>
UspNode<ElementT>* UspNode<ElementT>::GetNextNode ()
{
    return m_pqNext;
}
//------------------------------------------------------------------------------------------------------------------



