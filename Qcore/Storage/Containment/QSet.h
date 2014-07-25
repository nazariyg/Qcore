#ifndef QSET_H
#define QSET_H

// The container class to store only unique elements inside; new element insertion and further lookup is
// performed by means of the red-black tree algorithm that is good for mixed insert/lookup operations.  This
// class exists as alternative to the BsSet class that is optimal for cases when insertion and lookup are
// separate phases.  The ElementT template parameter is either a native data type or a class that has the next
// member function:
//   Class::Class (const Class& ...);
// where Class is the same as ElementT, the 'const' and '&' are optional.  As a general rule, the default
// eq-comparator to be used by the red-black tree algorithm is suitable enough; it calls for the next member
// functions:
//   bool Class::operator== (const Class& ...);
//   bool Class::operator<  (const Class& ...);
// (For compatibility purposes, the class was being designed with respect to features, interface, and behavior
// peculiar to the Standard Template Library.)

#include "QArray.h"
#include "QBspNode.h"
#include "QSetIterator.h"

namespace Q
{

template <class ElementT, class EqComparatorT = DefaultEqComparator<ElementT> >
class Set
{

Q_DECLARE_STREAM;

public:
    // iterator type
    typedef SetIterator<ElementT,EqComparatorT> IteratorT;

    // construction and destruction
    Set ();
    Set (const Set& rqSet);
    ~Set ();

    // fullness
    int GetQuantity () const;
    bool IsEmpty () const;

    // Insertion of an element.  A returned pair associates an iterator with insertion success: it contains the
    // iterator designating the inserted element with 'true' if the element's value was new for the set, and
    // the iterator designating the already present same-value element with 'false' if it was not.
    Link<IteratorT,bool> Insert (const ElementT& rqElement);

    // lookup
    bool IsPresent (const ElementT& rtElement) const;
    IteratorT Find (const ElementT& rtElement) const;

    // outermost element access
    IteratorT GetBegin () const;  // if the set is empty, returns the end-iterator
    IteratorT GetEnd () const;

    // Removal.  In the iterator-oriented functions below, a returned iterator designates the element that was
    // next (in the positive direction) to the last element has been removed, or it is the end-iterator if no
    // such element exists.  For the range removal function, qTo designates the non-inclusive element up to
    // which removal is to be performed.
    bool Remove (const ElementT& rtElement);                            // removes an element by its value
    IteratorT Remove (const IteratorT& rqPos);                          // removes an element by its position
    IteratorT Remove (const IteratorT& rqFrom, const IteratorT& rqTo);  // removes a range of elements
    void RemoveAll ();                                                  // clears the set

    // assignment
    Set& operator= (const Set& rqSet);

    // comparison
    bool operator== (const Set& rqSet);
    bool operator!= (const Set& rqSet);
    bool operator<  (const Set& rqSet);
    bool operator<= (const Set& rqSet);
    bool operator>  (const Set& rqSet);
    bool operator>= (const Set& rqSet);

    // swapping
    void Swap (Set& rqSet);

private:
    template <class ElementT, class EqComparatorT>
    friend class SetIterator;

    class RecordContainer
    {
    public:
        enum ElementColorType
        {
            ECT_RED,
            ECT_BLACK
        };

        RecordContainer (const ElementT& rqElement, ElementColorType eColor)
            :
            Element(rqElement),
            Color(eColor)
        {
        };

        ElementT Element;
        ElementColorType Color;
    };
    typedef BspNode<RecordContainer> RecordNodeT;

    void RotateNodeNegative (RecordNodeT* pqRecordNode);
    void RotateNodePositive (RecordNodeT* pqRecordNode);
    void OnInsert (RecordNodeT* pqRecordNode);
    void OnRemove (RecordNodeT* pqRecordNode);

    int m_iQuantity;
    RecordNodeT* m_pqRootRecordNode;
    EqComparatorT m_tEqComparator;
};

#include "QSet.inl"

}

#endif



