#ifndef QHEAPRTTI_H
#define QHEAPRTTI_H

#include "QHeap.h"
#include "QRtti.h"

namespace Q
{

class HeapRtti : public Heap, public Rtti
{
protected:
    // construction
    HeapRtti () {};
};

}

#endif



