#ifndef QID_H
#define QID_H

// Every Id derived object will get a unique numerical identification on construction.

#include "QStream.h"

namespace Q
{

class Q_ITEM Id
{

Q_DECLARE_STREAM;

protected:
    // construction
    Id ();

public:
    unsigned int GetId () const;
    static unsigned int GetNextId ();

private:
    unsigned int m_uiId;
    static unsigned int ms_uiNextId;  // the global counter
};

#include "QId.inl"

}

#endif



