#ifndef QSETITERATOR_H
#define QSETITERATOR_H

namespace Q
{

// to avoid circular include
template <class ElementT, class EqComparatorT>
class Set;

template <class ElementT, class EqComparatorT>
class SetIterator
{
private:
    typedef Set<ElementT,EqComparatorT> SetT;
    typedef typename SetT::RecordContainer RecordContainerT;
    typedef BspNode<RecordContainerT> RecordNodeT;
    typedef SetIterator<ElementT,EqComparatorT> IteratorT;

public:
    // construction
    SetIterator (const SetT* pqSet, RecordNodeT* pqNode);

    // element access
    ElementT& operator* () const;
    ElementT* operator-> () const;

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
    template <class ElementT, class EqComparatorT>
    friend class Set;

    const SetT* m_pqSet;
    RecordNodeT* m_pqRecordNode;
};

#include "QSetIterator.inl"

}

#endif



