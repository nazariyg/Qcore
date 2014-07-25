#ifndef QMAPITERATOR_H
#define QMAPITERATOR_H

namespace Q
{

// to avoid circular include
template <class KeyT, class ValueT, class EqComparatorT>
class Map;

template <class KeyT, class ValueT, class EqComparatorT>
class MapIterator
{
private:
    typedef Map<KeyT,ValueT,EqComparatorT> MapT;
    typedef typename MapT::RecordContainer RecordContainerT;
    typedef BspNode<RecordContainerT> RecordNodeT;
    typedef MapIterator<KeyT,ValueT,EqComparatorT> IteratorT;

public:
    // construction
    MapIterator ();  // uninitialized
    MapIterator (const MapT* pqMap, RecordNodeT* pqNode);

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
    template <class KeyT, class ValueT, class EqComparatorT>
    friend class Map;

    const MapT* m_pqMap;
    RecordNodeT* m_pqRecordNode;
};

#include "QMapIterator.inl"

}

#endif



