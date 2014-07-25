//------------------------------------------------------------------------------------------------------------------
template <class ElementT>
BspNode<ElementT>::BspNode ()
    :
    m_pqParent(0),
    m_pqNegative(0),
    m_pqPositive(0)
{
}
//------------------------------------------------------------------------------------------------------------------
template <class ElementT>
BspNode<ElementT>::BspNode (const ElementT& rtElement, BspNode* pqParent)
    :
    m_tElement(rtElement),
    m_pqParent(pqParent),
    m_pqNegative(0),
    m_pqPositive(0)
{
}
//------------------------------------------------------------------------------------------------------------------
template <class ElementT>
void BspNode<ElementT>::SetParentNode (BspNode* pqParent)
{
    m_pqParent = pqParent;
}
//------------------------------------------------------------------------------------------------------------------
template <class ElementT>
void BspNode<ElementT>::AttachNegativeChild (BspNode* pqChild)
{
    m_pqNegative = pqChild;
}
//------------------------------------------------------------------------------------------------------------------
template <class ElementT>
void BspNode<ElementT>::AttachPositiveChild (BspNode* pqChild)
{
    m_pqPositive = pqChild;
}
//------------------------------------------------------------------------------------------------------------------
template <class ElementT>
void BspNode<ElementT>::DetachNegativeChild ()
{
    m_pqNegative = 0;
}
//------------------------------------------------------------------------------------------------------------------
template <class ElementT>
void BspNode<ElementT>::DetachPositiveChild ()
{
    m_pqPositive = 0;
}
//------------------------------------------------------------------------------------------------------------------
template <class ElementT>
ElementT& BspNode<ElementT>::GetElement ()
{
    return m_tElement;
}
//------------------------------------------------------------------------------------------------------------------
template <class ElementT>
const ElementT& BspNode<ElementT>::GetElement () const
{
    return m_tElement;
}
//------------------------------------------------------------------------------------------------------------------
template <class ElementT>
BspNode<ElementT>* BspNode<ElementT>::GetParentNode ()
{
    return m_pqParent;
}
//------------------------------------------------------------------------------------------------------------------
template <class ElementT>
BspNode<ElementT>* BspNode<ElementT>::GetNegativeChild ()
{
    return m_pqNegative;
}
//------------------------------------------------------------------------------------------------------------------
template <class ElementT>
BspNode<ElementT>* BspNode<ElementT>::GetPositiveChild ()
{
    return m_pqPositive;
}
//------------------------------------------------------------------------------------------------------------------
template <class ElementT>
BspNode<ElementT>* BspNode<ElementT>::GetNegativemostChild ()
{
    BspNode* pqNegativemost = this;
    while ( pqNegativemost->m_pqNegative )
        pqNegativemost = pqNegativemost->m_pqNegative;
    return pqNegativemost;
}
//------------------------------------------------------------------------------------------------------------------
template <class ElementT>
BspNode<ElementT>* BspNode<ElementT>::GetPositivemostChild ()
{
    BspNode* pqPositivemost = this;
    while ( pqPositivemost->m_pqPositive )
        pqPositivemost = pqPositivemost->m_pqPositive;
    return pqPositivemost;
}
//------------------------------------------------------------------------------------------------------------------
template <class ElementT>
BspNode<ElementT>* BspNode<ElementT>::GetNextNode ()
{
    if ( m_pqPositive )
    {
        return m_pqPositive->GetNegativemostChild();
    }
    else
    {
        BspNode* pqNode = this;
        while ( pqNode->m_pqParent && pqNode == pqNode->m_pqParent->m_pqPositive )
        {
            // right type of linking, move up
            pqNode = pqNode->m_pqParent;
        }
        return pqNode->m_pqParent;
    }
}
//------------------------------------------------------------------------------------------------------------------
template <class ElementT>
BspNode<ElementT>* BspNode<ElementT>::GetPriorNode ()
{
    if ( m_pqNegative )
    {
        return m_pqNegative->GetPositivemostChild();
    }
    else
    {
        BspNode* pqNode = this;
        while ( pqNode->m_pqParent && pqNode == pqNode->m_pqParent->m_pqNegative )
        {
            // left type of linking, move up
            pqNode = pqNode->m_pqParent;
        }
        return pqNode->m_pqParent;
    }
}
//------------------------------------------------------------------------------------------------------------------



