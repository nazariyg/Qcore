#ifndef QHEAPARRAY_H
#define QHEAPARRAY_H

// The mergence of StillArray and Heap classes.  Brings the shareable array class capable to self-destruct when
// no more related smart pointers left.

#include "QStillArray.h"
#include "QHeap.h"

namespace Q
{

template <class ElementT>
class HeapArray : public StillArray<ElementT>, public Heap
{

Q_DECLARE_STREAM;

public:
    // construction
    HeapArray (int iQuantity = 0);
    HeapArray (ElementT* atElement, int iQuantity);  // to own an ownerless array of elements
    HeapArray (const HeapArray& rqHeapArray);
};

#include "QHeapArray.inl"

}

#endif



