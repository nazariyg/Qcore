#ifndef QSMARTRTTIPOINTER_H
#define QSMARTRTTIPOINTER_H

// The class is for being able to store in a template container smart pointers which point to differently
// classed objects.

#include "QHeapRtti.h"

namespace Q
{

class Q_ITEM SmartRttiPointer
{
public:
    // construction and destruction
    SmartRttiPointer (HeapRtti* pqHeapRtti = 0);
    SmartRttiPointer (const SmartRttiPointer& rqPointer);
    ~SmartRttiPointer ();

    // implicit conversions
    operator HeapRtti* () const;
    HeapRtti* operator-> () const;

    // assignment
    SmartRttiPointer& operator= (HeapRtti* pqHeapRtti);
    SmartRttiPointer& operator= (const SmartRttiPointer& rqReference);

    // comparison
    bool operator== (HeapRtti* pqHeapRtti) const;
    bool operator!= (HeapRtti* pqHeapRtti) const;
    bool operator== (const SmartRttiPointer& rqReference) const;
    bool operator!= (const SmartRttiPointer& rqReference) const;

private:
    HeapRtti* m_pqHeapRtti;
};

#include "QSmartRttiPointer.inl"

}

#endif



