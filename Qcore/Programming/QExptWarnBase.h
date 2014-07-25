#ifndef QEXPTWARNBASE_H
#define QEXPTWARNBASE_H

// The ExptWarnBase class allows an object of a derived class to signal error occurrences with exceptions and
// warnings, which are to be represented by enumerands within the scope of the derived class.  An exception
// means a critical error, whereas a warning means a non-critical error.

#include "QModestArray.h"

namespace Q
{

class Q_ITEM ExptWarnBase
{
protected:
    // construction
    ExptWarnBase ();

public:
    // exception related
    bool GetExceptionFlag () const;
    int GetExceptionType () const;

    // warnings related
    int GetWarningQuantity () const;
    int GetWarningType (int i) const;

protected:
    virtual void OnException (int iExceptionType);
    void OnWarning (int iWarningType);

    bool m_bExceptionFlag;  // default: false
    int m_iExceptionType;
    ModestArray<int> m_qWarnings;
};

#include "QExptWarnBase.inl"

}

#endif



