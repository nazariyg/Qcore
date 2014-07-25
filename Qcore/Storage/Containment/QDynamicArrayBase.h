#ifndef QDYNAMICARRAYBASE_H
#define QDYNAMICARRAYBASE_H

// The DynamicArrayBase class adds features such as pushing, popping, and insertion above the StillArray class
// in order to serve as the base for dynamic array classes.  The layer of the class brings in a new array
// property 'max_quantity' to indicate the number of allocated element slots; 'max_quantity' is always greater
// than or equal to 'quantity' that means the number of occupied slots.  While Pop member function is
// premeditated the same for all DynamicArrayBase derivatives, Push is to be implemented by every derived class
// using an individual approach determining how much more memory to allocate when 'quantity' is equal to
// 'max_quantity' and a new element needs to be pushed.

#include "QStillArray.h"

namespace Q
{

template <class ElementT>
class DynamicArrayBase : public StillArray<ElementT>
{

Q_DECLARE_STREAM;

protected:
    // construction
    DynamicArrayBase (int iQuantity, int iMaxQuantity);
    DynamicArrayBase (const DynamicArrayBase& rqArray);

public:
    // the number of allocated element slots
    int GetMaxQuantity () const;

    // resizing
    virtual void SetQuantity (int iNewQuantity, bool bCopy = true);
    void SetMaxQuantity (int iNewMaxQuantity, bool bCopy = true);    // changes the number of reserved slots
    void Compact ();                                                 // removes all reserved slots

    // Insertion.  An index of insertion position must be in the range [0, quantity]; i = quantity is
    // equivalent to pushing of the element.  (The List class may offer faster element insertion however in
    // cost of some memory.)
    void Insert (int i, const ElementT& rtElement);

    // Removal.  To facilitate further element pushing, all the functions except RemoveAll keep allocated
    // memory at the same amount.  A freed slot is assigned with the default instance of the template type.
    // For the range-removal function below, parameter iFrom specifies the position of the first element, while
    // parameter iTo indicates the position next to the last element in the range to be removed.
    void Pop ();                       // removes the last element (keeping the memory)
    void Remove (int i);               // removes an element at a specific position
    void Remove (int iFrom, int iTo);  // removes a range of elements; iFrom must be <= iTo
    virtual void RemoveAll ();         // frees the allocated memory

    // delete present and own a native array
    virtual void BecomeOwner (ElementT* atElement, int iQuantity);

    // assignment
    virtual DynamicArrayBase& operator= (const DynamicArrayBase& rqArray);

    // swapping
    virtual void Swap (DynamicArrayBase& rqArray);

protected:
    int m_iMaxQuantity;
};

#include "QDynamicArrayBase.inl"

}

#endif



