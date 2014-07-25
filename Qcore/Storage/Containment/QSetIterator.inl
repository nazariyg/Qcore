//------------------------------------------------------------------------------------------------------------------
template <class ElementT, class EqComparatorT>
SetIterator<ElementT,EqComparatorT>::SetIterator (const SetT* pqSet, RecordNodeT* pqNode)
    :
    m_pqSet(pqSet),
    m_pqRecordNode(pqNode)
{
}
//------------------------------------------------------------------------------------------------------------------
template <class ElementT, class EqComparatorT>
ElementT& SetIterator<ElementT,EqComparatorT>::operator* () const
{
    assert( m_pqRecordNode );
    return m_pqRecordNode->GetElement().Element;
}
//------------------------------------------------------------------------------------------------------------------
template <class ElementT, class EqComparatorT>
ElementT* SetIterator<ElementT,EqComparatorT>::operator-> () const
{
    return &(operator*());
}
//------------------------------------------------------------------------------------------------------------------
template <class ElementT, class EqComparatorT>
bool SetIterator<ElementT,EqComparatorT>::operator== (const IteratorT& rqIterator) const
{
    return ( m_pqRecordNode == rqIterator.m_pqRecordNode );
}
//------------------------------------------------------------------------------------------------------------------
template <class ElementT, class EqComparatorT>
bool SetIterator<ElementT,EqComparatorT>::operator!= (const IteratorT& rqIterator) const
{
    return ( m_pqRecordNode != rqIterator.m_pqRecordNode );
}
//------------------------------------------------------------------------------------------------------------------
template <class ElementT, class EqComparatorT>
SetIterator<ElementT,EqComparatorT> SetIterator<ElementT,EqComparatorT>::operator++ ()
{
    assert( m_pqRecordNode );

    m_pqRecordNode = m_pqRecordNode->GetNextNode();
    return *this;
}
//------------------------------------------------------------------------------------------------------------------
template <class ElementT, class EqComparatorT>
SetIterator<ElementT,EqComparatorT> SetIterator<ElementT,EqComparatorT>::operator++ (int)
{
    IteratorT qPos = *this;
    ++*this;
    return qPos;
}
//------------------------------------------------------------------------------------------------------------------
template <class ElementT, class EqComparatorT>
SetIterator<ElementT,EqComparatorT> SetIterator<ElementT,EqComparatorT>::operator-- ()
{
    if ( m_pqRecordNode )
        m_pqRecordNode = m_pqRecordNode->GetPriorNode();
    else
        m_pqRecordNode = m_pqSet->m_pqRootRecordNode->GetPositivemostChild();
    return *this;
}
//------------------------------------------------------------------------------------------------------------------
template <class ElementT, class EqComparatorT>
SetIterator<ElementT,EqComparatorT> SetIterator<ElementT,EqComparatorT>::operator-- (int)
{
    IteratorT qPos = *this;
    --*this;
    return qPos;
}
//------------------------------------------------------------------------------------------------------------------
template <class ElementT, class EqComparatorT>
SetIterator<ElementT,EqComparatorT> SetIterator<ElementT,EqComparatorT>::operator+ (int iQuantity) const
{
    assert( iQuantity >= 0 );

    IteratorT qPos = *this;
    for (int i = 0; i < iQuantity; i++)
        ++qPos;
    return qPos;
}
//------------------------------------------------------------------------------------------------------------------
template <class ElementT, class EqComparatorT>
SetIterator<ElementT,EqComparatorT> SetIterator<ElementT,EqComparatorT>::operator- (int iQuantity) const
{
    assert( iQuantity >= 0 );

    IteratorT qPos = *this;
    for (int i = 0; i < iQuantity; i++)
        --qPos;
    return qPos;
}
//------------------------------------------------------------------------------------------------------------------
template <class ElementT, class EqComparatorT>
SetIterator<ElementT,EqComparatorT>& SetIterator<ElementT,EqComparatorT>::operator+= (int iQuantity)
{
    assert( iQuantity >= 0 );

    for (int i = 0; i < iQuantity; i++)
        ++*this;
    return *this;
}
//------------------------------------------------------------------------------------------------------------------
template <class ElementT, class EqComparatorT>
SetIterator<ElementT,EqComparatorT>& SetIterator<ElementT,EqComparatorT>::operator-= (int iQuantity)
{
    assert( iQuantity >= 0 );

    for (int i = 0; i < iQuantity; i++)
        --*this;
    return *this;
}
//------------------------------------------------------------------------------------------------------------------



