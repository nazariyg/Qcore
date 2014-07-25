//------------------------------------------------------------------------------------------------------------------
template <class KeyT, class ValueT, class EqComparatorT>
MapIterator<KeyT,ValueT,EqComparatorT>::MapIterator ()
{
}
//------------------------------------------------------------------------------------------------------------------
template <class KeyT, class ValueT, class EqComparatorT>
MapIterator<KeyT,ValueT,EqComparatorT>::MapIterator (const MapT* pqMap, RecordNodeT* pqNode)
    :
    m_pqMap(pqMap),
    m_pqRecordNode(pqNode)
{
}
//------------------------------------------------------------------------------------------------------------------
template <class KeyT, class ValueT, class EqComparatorT>
Link<KeyT,ValueT>& MapIterator<KeyT,ValueT,EqComparatorT>::operator* () const
{
    assert( m_pqRecordNode );
    return m_pqRecordNode->GetElement().Record;
}
//------------------------------------------------------------------------------------------------------------------
template <class KeyT, class ValueT, class EqComparatorT>
Link<KeyT,ValueT>* MapIterator<KeyT,ValueT,EqComparatorT>::operator-> () const
{
    return &(operator*());
}
//------------------------------------------------------------------------------------------------------------------
template <class KeyT, class ValueT, class EqComparatorT>
bool MapIterator<KeyT,ValueT,EqComparatorT>::operator== (const IteratorT& rqIterator) const
{
    return ( m_pqRecordNode == rqIterator.m_pqRecordNode );
}
//------------------------------------------------------------------------------------------------------------------
template <class KeyT, class ValueT, class EqComparatorT>
bool MapIterator<KeyT,ValueT,EqComparatorT>::operator!= (const IteratorT& rqIterator) const
{
    return ( m_pqRecordNode != rqIterator.m_pqRecordNode );
}
//------------------------------------------------------------------------------------------------------------------
template <class KeyT, class ValueT, class EqComparatorT>
MapIterator<KeyT,ValueT,EqComparatorT> MapIterator<KeyT,ValueT,EqComparatorT>::operator++ ()
{
    assert( m_pqRecordNode );

    m_pqRecordNode = m_pqRecordNode->GetNextNode();
    return *this;
}
//------------------------------------------------------------------------------------------------------------------
template <class KeyT, class ValueT, class EqComparatorT>
MapIterator<KeyT,ValueT,EqComparatorT> MapIterator<KeyT,ValueT,EqComparatorT>::operator++ (int)
{
    IteratorT qPos = *this;
    ++*this;
    return qPos;
}
//------------------------------------------------------------------------------------------------------------------
template <class KeyT, class ValueT, class EqComparatorT>
MapIterator<KeyT,ValueT,EqComparatorT> MapIterator<KeyT,ValueT,EqComparatorT>::operator-- ()
{
    if ( m_pqRecordNode )
        m_pqRecordNode = m_pqRecordNode->GetPriorNode();
    else
        m_pqRecordNode = m_pqMap->m_pqRootRecordNode->GetPositivemostChild();
    return *this;
}
//------------------------------------------------------------------------------------------------------------------
template <class KeyT, class ValueT, class EqComparatorT>
MapIterator<KeyT,ValueT,EqComparatorT> MapIterator<KeyT,ValueT,EqComparatorT>::operator-- (int)
{
    IteratorT qPos = *this;
    --*this;
    return qPos;
}
//------------------------------------------------------------------------------------------------------------------
template <class KeyT, class ValueT, class EqComparatorT>
MapIterator<KeyT,ValueT,EqComparatorT> MapIterator<KeyT,ValueT,EqComparatorT>::operator+ (int iQuantity) const
{
    assert( iQuantity >= 0 );

    IteratorT qPos = *this;
    for (int i = 0; i < iQuantity; i++)
        ++qPos;
    return qPos;
}
//------------------------------------------------------------------------------------------------------------------
template <class KeyT, class ValueT, class EqComparatorT>
MapIterator<KeyT,ValueT,EqComparatorT> MapIterator<KeyT,ValueT,EqComparatorT>::operator- (int iQuantity) const
{
    assert( iQuantity >= 0 );

    IteratorT qPos = *this;
    for (int i = 0; i < iQuantity; i++)
        --qPos;
    return qPos;
}
//------------------------------------------------------------------------------------------------------------------
template <class KeyT, class ValueT, class EqComparatorT>
MapIterator<KeyT,ValueT,EqComparatorT>& MapIterator<KeyT,ValueT,EqComparatorT>::operator+= (int iQuantity)
{
    assert( iQuantity >= 0 );

    for (int i = 0; i < iQuantity; i++)
        ++*this;
    return *this;
}
//------------------------------------------------------------------------------------------------------------------
template <class KeyT, class ValueT, class EqComparatorT>
MapIterator<KeyT,ValueT,EqComparatorT>& MapIterator<KeyT,ValueT,EqComparatorT>::operator-= (int iQuantity)
{
    assert( iQuantity >= 0 );

    for (int i = 0; i < iQuantity; i++)
        --*this;
    return *this;
}
//------------------------------------------------------------------------------------------------------------------



