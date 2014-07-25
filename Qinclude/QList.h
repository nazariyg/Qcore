#ifndef QLIST_H
#define QLIST_H

// The List class is an array container and arranges elements in the chain-like manner.  A List object is able
// to be pushed with elements at both endings and also has the distinguishing feature of fast
// arbitrary-position element insertion, which is in contrast to slower but more memory-dense behavior of the
// array classes.  The ElementT template parameter is either a native data type or a class that has the next
// member function:
//   Class::Class (const Class& ...);
// where Class is the same as ElementT, the 'const' and '&' are optional.  If usage of an inter-list comparison
// operator is implied, the ElementT class may require to be equipped with one of the following member
// functions:
//   bool Class::operator== (const Class& ...);
//   bool Class::operator<  (const Class& ...);
// (For compatibility purposes, the class was being designed with respect to features, interface, and behavior
// peculiar to the Standard Template Library.)

#include "QStrgCnmtUtils.h"
#include "QUspNode.h"
#include "QListIterator.h"

namespace Q
{

template <class ElementT>
class List
{

Q_DECLARE_STREAM;

public:
    typedef ListIterator<ElementT> IteratorT;  // iterator type

    // construction and destruction
    List ();
    List (const List& rqList);
    ~List ();

    // fullness
    int GetQuantity () const;
    bool IsEmpty () const;

    // Adding of an element.  For the Insert function, the element that was at rqPos becomes next (in the
    // positive direction) to the inserted element; the end-iterator is allowed for insertion position; the
    // function returns the iterator designating the element has been inserted.
    void PushHead (const ElementT& rtElement);
    void PushTail (const ElementT& rtElement);
    IteratorT Insert (const IteratorT& rqPos, const ElementT& rtElement);

    // outermost element access
    ElementT& GetFirst ();
    const ElementT& GetFirst () const;
    ElementT& GetLast ();
    const ElementT& GetLast () const;
    IteratorT GetBegin () const;  // if the list is empty the end-iterator is returned
    IteratorT GetEnd () const;

    // Removal.  In the iterator-oriented functions below, a returned iterator designates the element that was
    // next in the positive direction to the last element has been removed, or it is the end-iterator if no
    // such element exists.  For the range removal function, qTo designates the non-inclusive element up to
    // which removal is to be performed.
    void PopHead ();
    void PopTail ();
    IteratorT Remove (const IteratorT& rqPos);                          // removes an element by its position
    IteratorT Remove (const IteratorT& rqFrom, const IteratorT& rqTo);  // removes a range of elements
    void RemoveAll ();                                                  // clears the list

    // operations
    void Merge (List& rqList);  // Concatenates a list from the right.  NOTE. The parameter list becomes empty.
    void Reverse ();            // reverses the element order

    // assignment
    List& operator= (const List& rqList);

    // comparison
    bool operator== (const List& rqList) const;
    bool operator!= (const List& rqList) const;
    bool operator<  (const List& rqList) const;
    bool operator<= (const List& rqList) const;
    bool operator>  (const List& rqList) const;
    bool operator>= (const List& rqList) const;

    // swapping
    void Swap (List& rqList);

private:
    template <class ElementT>
    friend class ListIterator;

    typedef UspNode<ElementT> ElementNodeT;

    int m_iQuantity;
    ElementNodeT* m_pqHeadNode;
    ElementNodeT* m_pqTailNode;
};

#include "QList.inl"

}

#endif



