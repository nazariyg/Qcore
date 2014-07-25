#ifndef QHASHMAPITERATOR_H
#define QHASHMAPITERATOR_H

namespace Q
{

// to avoid circular include
template <class KeyT, class ValueT, class HasherT, class EquatorT>
class HashMap;

template <class KeyT, class ValueT, class HasherT, class EquatorT>
class HashMapIterator
{
private:
    typedef HashMap<KeyT,ValueT,HasherT,EquatorT> MapT;
    typedef typename MapT::RecordContainer RecordContainerT;
    typedef HashMapIterator<KeyT,ValueT,HasherT,EquatorT> IteratorT;

public:
    // construction
    HashMapIterator ();  // uninitialized
    HashMapIterator (const MapT* pqMap, RecordContainerT* pqRecord, bool bPassForward);

    // record access
    Link<KeyT,ValueT>& operator* () const;
    Link<KeyT,ValueT>* operator-> () const;

    // comparison
    bool operator== (const IteratorT& rqIterator) const;
    bool operator!= (const IteratorT& rqIterator) const;

    // stepping
    IteratorT operator++ ();
    IteratorT operator++ (int);
    IteratorT operator-- ();
    IteratorT operator-- (int);

    // jumping
    IteratorT operator+ (int iQuantity) const;
    IteratorT operator- (int iQuantity) const;
    IteratorT& operator+= (int iQuantity);
    IteratorT& operator-= (int iQuantity);

private:
    template <class KeyT, class ValueT, class HasherT, class EquatorT>
    friend class HashMap;

    void PassEmptyAndRemovedForward ();
    void PassEmptyAndRemovedBackward ();

    const MapT* m_pqMap;
    RecordContainerT* m_pqRecord;
};

#include "QHashMapIterator.inl"

}

#endif



