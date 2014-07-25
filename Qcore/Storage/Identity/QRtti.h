#ifndef QRTTI_H
#define QRTTI_H

// The run-time type (class) information.  A class which is necessary to be RTTI-operational must derive from
// the Rtti class directly or indirectly.  Only single-inheritance allowed.  Besides the derivation, by means
// of the macros an RTTI class must declare its own 'static const' class-label together with the associated
// get-function (defined in-place) and implement this class-label.  A ClassLabel object is the one which
// defines uniqueness of the RTTI featured class it was "stuck" to.  The uniqueness consists of the memory
// address of that ClassLabel object and of the classname, but only the address is decisive factor for RTTI.
// Within the RTTI system a classname is composed of the namespace where the class is declared plus dot and
// plus the actual name of the class.  For plane classes the macros Q_DECLARE_RTTI and Q_IMPLEMENT_RTTI are to
// be used, and Q_DECLARE_TEMPLATE_RTTI with Q_IMPLEMENT_TEMPLATE_RTTI are ones for template classes.  So, an
// implemental macro takes as input the namespace, the name of the base class so as to get the address of its
// class-label for being able to trace kinships, and the name of the class.

#include "QClassLabel.h"
#include "QRtti.mcr"

namespace Q
{

class Q_ITEM Rtti
{
protected:
    // construction
    Rtti ();

public:
    // the classname is "Q.Rtti", the base class-label pointer is equal to 0 since Rtti is the root
    static const ClassLabel Class;

    // to be reimplemented by derivatives
    virtual const ClassLabel& GetClass () const;

    // the functions IsDerived and IsDerivedClassOf return 'true' when the same classes are being compared
    bool IsExactly (const ClassLabel& rqClass) const;  // NOTE. The function name is referenced by Q_IF_NOT_EXISTS.
    bool IsDerived (const ClassLabel& rqClass) const;
    bool IsExactlyClassOf (const Rtti* pqObject) const;
    bool IsDerivedClassOf (const Rtti* pqObject) const;
};

#include "QRtti.inl"

}

#endif



