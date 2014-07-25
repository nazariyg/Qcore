//------------------------------------------------------------------------------------------------------------------
template <class KeyT, class ValueT, class HasherT, class EquatorT>
HashMapIterator<KeyT,ValueT,HasherT,EquatorT>::HashMapIterator ()
{
}
//------------------------------------------------------------------------------------------------------------------
template <class KeyT, class ValueT, class HasherT, class EquatorT>
HashMapIterator<KeyT,ValueT,HasherT,EquatorT>::HashMapIterator (const MapT* pqMap, RecordContainerT* pqRecord,
    bool bPassForward)
    :
    m_pqMap(pqMap),
    m_pqRecord(pqRecord)
{
    if ( bPassForward )
        PassEmptyAndRemovedForward();
}
//------------------------------------------------------------------------------------------------------------------
template <class KeyT, class ValueT, class HasherT, class EquatorT>
Link<KeyT,ValueT>& HashMapIterator<KeyT,ValueT,HasherT,EquatorT>::operator* () const
{
    assert( m_pqMap->m_aqRecord <= m_pqRecord && m_pqRecord < m_pqMap->m_aqRecord + m_pqMap->m_iBucketQuantity );
    return m_pqRecord->Record;
}
//------------------------------------------------------------------------------------------------------------------
template <class KeyT, class ValueT, class HasherT, class EquatorT>
Link<KeyT,ValueT>* HashMapIterator<KeyT,ValueT,HasherT,EquatorT>::operator-> () const
{
    return &(operator*());
}
//------------------------------------------------------------------------------------------------------------------
template <class KeyT, class ValueT, class HasherT, class EquatorT>
bool HashMapIterator<KeyT,ValueT,HasherT,EquatorT>::operator== (const IteratorT& rqIterator) const
{
    return ( m_pqRecord == rqIterator.m_pqRecord );
}
//------------------------------------------------------------------------------------------------------------------
template <class KeyT, class ValueT, class HasherT, class EquatorT>
bool HashMapIterator<KeyT,ValueT,HasherT,EquatorT>::operator!= (const IteratorT& rqIterator) const
{
    return ( m_pqRecord != rqIterator.m_pqRecord );
}
//------------------------------------------------------------------------------------------------------------------
template <class KeyT, class ValueT, class HasherT, class EquatorT>
HashMapIterator<KeyT,ValueT,HasherT,EquatorT> HashMapIterator<KeyT,ValueT,HasherT,EquatorT>::operator++ ()
{
    assert( m_pqMap->m_aqRecord <= m_pqRecord && m_pqRecord < m_pqMap->m_aqRecord + m_pqMap->m_iBucketQuantity );

    m_pqRecord++;
    PassEmptyAndRemovedForward();
    return *this;
}
//------------------------------------------------------------------------------------------------------------------
template <class KeyT, class ValueT, class HasherT, class EquatorT>
HashMapIterator<KeyT,ValueT,HasherT,EquatorT> HashMapIterator<KeyT,ValueT,HasherT,EquatorT>::operator++ (int)
{
    IteratorT qPos = *this;
    ++*this;
    return qPos;
}
//------------------------------------------------------------------------------------------------------------------
template <class KeyT, class ValueT, class HasherT, class EquatorT>
HashMapIterator<KeyT,ValueT,HasherT,EquatorT> HashMapIterator<KeyT,ValueT,HasherT,EquatorT>::operator-- ()
{
    assert( m_pqMap->m_aqRecord <= m_pqRecord && m_pqRecord <= m_pqMap->m_aqRecord + m_pqMap->m_iBucketQuantity );

    m_pqRecord--;
    PassEmptyAndRemovedBackward();
    return *this;
}
//------------------------------------------------------------------------------------------------------------------
template <class KeyT, class ValueT, class HasherT, class EquatorT>
HashMapIterator<KeyT,ValueT,HasherT,EquatorT> HashMapIterator<KeyT,ValueT,HasherT,EquatorT>::operator-- (int)
{
    IteratorT qPos = *this;
    --*this;
    return qPos;
}
//------------------------------------------------------------------------------------------------------------------
template <class KeyT, class ValueT, class HasherT, class EquatorT>
HashMapIterator<KeyT,ValueT,HasherT,EquatorT>
HashMapIterator<KeyT,ValueT,HasherT,EquatorT>::operator+ (int iQuantity) const
{
    assert( iQuantity >= 0 );

    IteratorT qPos = *this;
    for (int i = 0; i < iQuantity; i++)
        ++qPos;
    return qPos;
}
//------------------------------------------------------------------------------------------------------------------
template <class KeyT, class ValueT, class HasherT, class EquatorT>
HashMapIterator<KeyT,ValueT,HasherT,EquatorT>
HashMapIterator<KeyT,ValueT,HasherT,EquatorT>::operator- (int iQuantity) const
{
    assert( iQuantity >= 0 );

    IteratorT qPos = *this;
    for (int i = 0; i < iQuantity; i++)
        --qPos;
    return qPos;
}
//------------------------------------------------------------------------------------------------------------------
template <class KeyT, class ValueT, class HasherT, class EquatorT>
HashMapIterator<KeyT,ValueT,HasherT,EquatorT>&
HashMapIterator<KeyT,ValueT,HasherT,EquatorT>::operator+= (int iQuantity)
{
    assert( iQuantity >= 0 );

    for (int i = 0; i < iQuantity; i++)
        ++*this;
    return *this;
}
//------------------------------------------------------------------------------------------------------------------
template <class KeyT, class ValueT, class HasherT, class EquatorT>
HashMapIterator<KeyT,ValueT,HasherT,EquatorT>&
HashMapIterator<KeyT,ValueT,HasherT,EquatorT>::operator-= (int iQuantity)
{
    assert( iQuantity >= 0 );

    for (int i = 0; i < iQuantity; i++)
        --*this;
    return *this;
}
//------------------------------------------------------------------------------------------------------------------
template <class KeyT, class ValueT, class HasherT, class EquatorT>
void HashMapIterator<KeyT,ValueT,HasherT,EquatorT>::PassEmptyAndRemovedForward ()
{
    RecordContainerT* pqEndRecord = m_pqMap->m_aqRecord + m_pqMap->m_iBucketQuantity;
    while ( m_pqRecord < pqEndRecord && (m_pqMap->TestEmpty(*this) || m_pqMap->TestRemoved(*this)) )
        m_pqRecord++;
}
//------------------------------------------------------------------------------------------------------------------
template <class KeyT, class ValueT, class HasherT, class EquatorT>
void HashMapIterator<KeyT,ValueT,HasherT,EquatorT>::PassEmptyAndRemovedBackward ()
{
    RecordContainerT* pqBeginRecord = m_pqMap->m_aqRecord;
    while ( m_pqRecord >= pqBeginRecord && (m_pqMap->TestEmpty(*this) || m_pqMap->TestRemoved(*this)) )
        m_pqRecord--;
}
//------------------------------------------------------------------------------------------------------------------



