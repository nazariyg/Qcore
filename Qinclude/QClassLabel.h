#ifndef QCLASSLABEL_H
#define QCLASSLABEL_H

// The RTTI system participator.  (You may look into Rtti class header for the related information.)

#include "QString.h"

namespace Q
{

class Q_ITEM ClassLabel
{
public:
    // Construction.  Parameter acName is the classname and pqBaseClass is the address of the class-label of
    // the base class.
    ClassLabel (const char* acName, const ClassLabel* pqBaseClass);

    // classname
    const char* GetName () const;

    // finding of the kinship with another class-label and thus among the classes
    bool IsExactly (const ClassLabel& rqClass) const;
    bool IsDerived (const ClassLabel& rqClass) const;  // returns 'true' when the same classes are being compared

private:
    const char* m_acName;
    const ClassLabel* m_pqBaseClass;
};

#include "QClassLabel.inl"

}

#endif



