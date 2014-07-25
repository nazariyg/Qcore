#ifndef QNAME_H
#define QNAME_H

// The name keeping class.

#include "QString.h"

namespace Q
{

class Q_ITEM Name
{

Q_DECLARE_STREAM;

// construction
protected:
    Name (const char* acName = 0);

public:
    void SetName (const String& rqName);
    const String& GetName () const;

private:
    String m_qName;
};

#include "QName.inl"

}

#endif



