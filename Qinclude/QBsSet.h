#ifndef QBSSET_H
#define QBSSET_H

// The BsSet class is for unique element storage and lookup.  The lookup is based on the binary search
// algorithm.  The algorithm however requires the set to be sorted, so linear search is involved when the set
// is unsorted.  The BsSet class is designed for the cases when a large number of elements are being inserted
// without interruptions for lookup, and then pure and intensive lookup is being performed; element sorting is
// to happen between these phases.  This class exists as alternative to the Set class that is oriented at mixed
// insert/lookup operations.  For a set with presumably low number of elements, usage of linear search only may
// be the better choice as the sorting is somewhat expensive to start.  The ElementT template parameter is
// either a native data type or a class that has the next member functions:
//   Class::Class ();
//   Class::Class (const Class& ...);
//   Class& Class::operator= (const Class& ...);
// where Class is the same as ElementT, the 'const' and '&' are optional.  As a general rule, the default
// comparator to be used in sorting is suitable enough; it calls for the next member function:
//   bool Class::operator<  (const Class& ...);
// If usage of an inter-set comparison operator is implied, the ElementT class may require to be equipped with
// the following member function:
//   bool Class::operator== (const Class& ...);

#include "QSortUtils.h"

namespace Q
{

template <class ElementT, class ComparatorT = DefaultComparator<ElementT> >
class BsSet
{

Q_DECLARE_STREAM;

public:
    // construction
    BsSet (int iQuantity = 0, int iMaxQuantity = 0);
    BsSet (const BsSet& rqSet);

    // member access
    int GetQuantity () const;
    int GetMaxQuantity () const;
    bool IsEmpty () const;
    ElementT* GetElements ();
    const ElementT* GetElements () const;
    operator ElementT* ();
    operator const ElementT* () const;
    ElementT& operator[] (int i);
    const ElementT& operator[] (int i) const;

    // Insertion of an element.  NOTE. The set becomes unsorted.
    bool Insert (const ElementT& rtElement);         // returns success ('true' if the element was new for the set)
    void InsertNoCheck (const ElementT& rtElement);  // if the element is known to be new for the set; faster

    // Sorting of the set.  Should be called before intensive element lookup.
    void PrepareBinarySearch ();
    void SetIsSorted ();

    // Lookup.  If the set is sorted, fast binary search is used, but slow linear search otherwise.
    bool IsPresent (const ElementT& rtElement) const;           // retrieves the presence status only
    bool Find (const ElementT& rtElement, int& riIndex) const;  // retrieves the presence status and the position

    // Removal.  For the Remove function, a freed slot is filled by the default object of the element class.
    void Remove (const ElementT& rtElement);  // removes an element by its value
    void RemoveAll ();                        // frees the allocated memory

    // resizing
    void SetQuantity (int iNewQuantity, bool bCopy);
    void SetMaxQuantity (int iNewMaxQuantity, bool bCopy);

    // assignment
    BsSet& operator= (const BsSet& rqSet);

    // comparison
    bool operator== (const BsSet& rqSet) const;
    bool operator!= (const BsSet& rqSet) const;
    bool operator<  (const BsSet& rqSet) const;
    bool operator<= (const BsSet& rqSet) const;
    bool operator>  (const BsSet& rqSet) const;
    bool operator>= (const BsSet& rqSet) const;

    // swapping
    void Swap (BsSet& rqSet);

private:
    bool FindLinearly (const ElementT& rtElement, int& riIndex) const;
    bool FindBinary (const ElementT& rtElement, int& riIndex) const;

    Array<ElementT> m_qArray;
    bool m_bSorted;  // default: false
    ComparatorT m_tComparator;
};

#include "QBsSet.inl"

}

#endif



