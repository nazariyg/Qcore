#ifndef QHEAP_H
#define QHEAP_H

// The Heap base class and the SmartPointer class template together make up the smart pointer system being in
// service.  The system provides the comfort of object sharing without the requirement for tracking the
// runtime-earliest point in the program code (or creating a special one) where a shared object and its
// allocations are safe to be deleted from memory.  This allows a program code to be of any necessary
// complexity about the relations between objects and their sharers as well as to change such relations freely.
// In use, smart pointers are like native C++ pointers: a smart pointer evaluates to the address of the object
// it points to, its '->' operator is for accessing the members of the object, '*' operator is for
// dereferencing, '=' operator is for assigning, '==' and '!=' operators are for comparison.
//
// Generally, the smart pointer system is activated and works as follows: a class whose objects are likely both
// to have substantial heap allocations and to be shared is derived from the Heap class; now, an object of the
// class can be pointed by a smart pointer templated with the type of that class; while in runtime an object of
// the class is shared by means of smart pointers, the layer of the Heap class takes into account every smart
// pointer attaching or detaching, and, when it detects that the last smart pointer has detached, the object
// destructs automatically with subsequent memory deallocation.  The state of being detached always means for a
// smart pointer to be null i.e. to have the address-value of zero, and vice versa.  The detaching can be
// caused by destruction of the smart pointer object, by assigning the value of zero to it (nullification), or
// by calling Delete member function of the pointed object, in which case all associated smart pointers become
// null.

#include "QStream.h"
#include "QArray.h"

namespace Q
{

template <class ObjectT>
class SmartPointer;

class SmartRttiPointer;

class Q_ITEM Heap
{

protected:
    // construction
    Heap ();

    // destruction
    virtual ~Heap ();

public:
    // nullifies all associated smart pointers, then forces destruction
    void Delete ();

private:
    template <class ObjectT>
    friend class SmartPointer;

    friend class Stream;

    friend class SmartRttiPointer;

    void IncrementReferences (void** ppvReference);
    void DecrementReferences (void** ppvReference);

    Array<void**> m_qReferences;
};

}

#endif



