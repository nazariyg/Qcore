#ifndef QSTATEBASE_H
#define QSTATEBASE_H

#include "QHeapRtti.h"

namespace Q
{

class Q_ITEM StateBase : public HeapRtti
{
protected:
    // construction
    StateBase ();

public:
    void SetOn (bool bOn);

    bool On;  // default: true
};

#include "QStateBase.inl"

}

#endif



