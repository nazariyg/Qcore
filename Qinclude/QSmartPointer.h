#ifndef QSMARTPOINTER_H
#define QSMARTPOINTER_H

// The smart pointer class.  (You may look into Heap class header for the related information.)

#include "QSmartRttiPointer.h"

namespace Q
{

template <class ObjectT>
class SmartPointer
{

Q_DECLARE_STREAM;

public:
    // construction and destruction
    SmartPointer (ObjectT* pqObject = 0);
    SmartPointer (const SmartPointer& rqPointer);
    ~SmartPointer ();

    // implicit conversions
    operator ObjectT* () const;
    ObjectT& operator* () const;
    ObjectT* operator-> () const;

    // camouflaging into SmartRttiPointer
    operator SmartRttiPointer () const;

    // assignment
    SmartPointer& operator= (ObjectT* pqObject);
    SmartPointer& operator= (const SmartPointer& rqReference);

    // comparison
    bool operator== (ObjectT* pqObject) const;
    bool operator!= (ObjectT* pqObject) const;
    bool operator== (const SmartPointer& rqReference) const;
    bool operator!= (const SmartPointer& rqReference) const;

private:
    ObjectT* m_pqObject;
};

#include "QSmartPointer.inl"

}

#endif



