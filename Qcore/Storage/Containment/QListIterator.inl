//------------------------------------------------------------------------------------------------------------------
template <class ElementT>
ListIterator<ElementT>::ListIterator ()
{
}
//------------------------------------------------------------------------------------------------------------------
template <class ElementT>
ListIterator<ElementT>::ListIterator (const ListT* pqList, ElementNodeT* pqElementNode)
    :
    m_pqList(pqList),
    m_pqElementNode(pqElementNode)
{
}
//------------------------------------------------------------------------------------------------------------------
template <class ElementT>
ElementT& ListIterator<ElementT>::operator* () const
{
    assert( m_pqElementNode );
    return m_pqElementNode->GetElement();
}
//------------------------------------------------------------------------------------------------------------------
template <class ElementT>
ElementT* ListIterator<ElementT>::operator-> () const
{
    return &(operator*());
}
//------------------------------------------------------------------------------------------------------------------
template <class ElementT>
bool ListIterator<ElementT>::operator== (const IteratorT& rqIterator) const
{
    return ( m_pqElementNode == rqIterator.m_pqElementNode );
}
//------------------------------------------------------------------------------------------------------------------
template <class ElementT>
bool ListIterator<ElementT>::operator!= (const IteratorT& rqIterator) const
{
    return ( m_pqElementNode != rqIterator.m_pqElementNode );
}
//------------------------------------------------------------------------------------------------------------------
template <class ElementT>
ListIterator<ElementT> ListIterator<ElementT>::operator++ ()
{
    assert( m_pqElementNode );

    m_pqElementNode = m_pqElementNode->GetNextNode();
    return *this;
}
//------------------------------------------------------------------------------------------------------------------
template <class ElementT>
ListIterator<ElementT> ListIterator<ElementT>::operator++ (int)
{
    IteratorT qPos = *this;
    ++*this;
    return qPos;
}
//------------------------------------------------------------------------------------------------------------------
template <class ElementT>
ListIterator<ElementT> ListIterator<ElementT>::operator-- ()
{
    if ( m_pqElementNode )
        m_pqElementNode = m_pqElementNode->GetPriorNode();
    else
        m_pqElementNode = m_pqList->m_pqTailNode;
    return *this;
}
//------------------------------------------------------------------------------------------------------------------
template <class ElementT>
ListIterator<ElementT> ListIterator<ElementT>::operator-- (int)
{
    IteratorT qPos = *this;
    --*this;
    return qPos;
}
//------------------------------------------------------------------------------------------------------------------
template <class ElementT>
ListIterator<ElementT> ListIterator<ElementT>::operator+ (int iQuantity) const
{
    assert( iQuantity >= 0 );

    IteratorT qPos = *this;
    for (int i = 0; i < iQuantity; i++)
        ++qPos;
    return qPos;
}
//------------------------------------------------------------------------------------------------------------------
template <class ElementT>
ListIterator<ElementT> ListIterator<ElementT>::operator- (int iQuantity) const
{
    assert( iQuantity >= 0 );

    IteratorT qPos = *this;
    for (int i = 0; i < iQuantity; i++)
        --qPos;
    return qPos;
}
//------------------------------------------------------------------------------------------------------------------
template <class ElementT>
ListIterator<ElementT>& ListIterator<ElementT>::operator+= (int iQuantity)
{
    assert( iQuantity >= 0 );

    for (int i = 0; i < iQuantity; i++)
        ++*this;
    return *this;
}
//------------------------------------------------------------------------------------------------------------------
template <class ElementT>
ListIterator<ElementT>& ListIterator<ElementT>::operator-= (int iQuantity)
{
    assert( iQuantity >= 0 );

    for (int i = 0; i < iQuantity; i++)
        --*this;
    return *this;
}
//------------------------------------------------------------------------------------------------------------------



