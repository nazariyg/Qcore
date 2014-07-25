#ifndef QLISTITERATOR_H
#define QLISTITERATOR_H

namespace Q
{

// to avoid circular include
template <class ElementT>
class List;

template <class ElementT>
class ListIterator
{
private:
    typedef List<ElementT> ListT;
    typedef UspNode<ElementT> ElementNodeT;
    typedef ListIterator<ElementT> IteratorT;

public:
    // construction
    ListIterator ();  // uninitialized
    ListIterator (const ListT* pqList, ElementNodeT* pqElementNode);

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
    template <class ElementT>
    friend class List;

    const ListT* m_pqList;
    ElementNodeT* m_pqElementNode;
};

#include "QListIterator.inl"

}

#endif



