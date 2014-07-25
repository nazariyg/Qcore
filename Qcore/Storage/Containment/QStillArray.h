#ifndef QSTILLARRAY_H
#define QSTILLARRAY_H

// The "still" array container class.  The StillArray class provides the facility of auto-deallocation and is
// appropriate for storing arrays known to remain the same size or change size infrequently.  The class also
// serves as the base for the dynamic (push/pop-able) array classes, namely Array and ModestArray.  The
// ElementT template parameter is either a native data type or a class that has the next member functions:
//   Class::Class ();
//   Class::Class (const Class& ...);
//   Class& Class::operator= (const Class& ...);
// where Class is the same as ElementT, the 'const' and '&' are optional.  If usage of an inter-array
// comparison operator is implied, the ElementT class may require to be equipped with one of the following
// member functions:
//   bool Class::operator== (const Class& ...);
//   bool Class::operator<  (const Class& ...);

#include "QStrgCnmtUtils.h"

namespace Q
{

template <class ElementT>
class StillArray
{

Q_DECLARE_STREAM;

public:
    // construction and destruction
    StillArray (int iQuantity = 0);
    StillArray (const StillArray& rqArray);
    StillArray (ElementT* atElement, int iQuantity);  // to own a native array
    ~StillArray ();

    // member access
    int GetQuantity () const;
    bool IsEmpty () const;
    ElementT* GetElements ();
    const ElementT* GetElements () const;
    operator ElementT* ();
    operator const ElementT* () const;
    ElementT& operator[] (int i);
    const ElementT& operator[] (int i) const;

    // outermost element access
    ElementT& GetFirst ();
    const ElementT& GetFirst () const;
    ElementT& GetLast ();
    const ElementT& GetLast () const;

    // operations
    virtual void SetQuantity (int iNewQuantity, bool bCopy = true);  // resizes the array; default: keep elements
    void AdvanceQuantity (int iAdvance);      // resizes relatively to the current size (iAdvance can be negative)
    void Append (const StillArray& rqArray);  // concatenates an array from the right; rqArray remains the same
    void Reverse ();                          // reverses element order
    void Shuffle ();                          // randomizes element positions
    virtual void RemoveAll ();                // frees the allocated memory

    // delete present and own a native array
    virtual void BecomeOwner (ElementT* atElement, int iQuantity);

    // assignment
    virtual StillArray& operator= (const StillArray& rqArray);

    // comparison
    bool operator== (const StillArray& rqArray) const;
    bool operator!= (const StillArray& rqArray) const;
    bool operator<  (const StillArray& rqArray) const;
    bool operator<= (const StillArray& rqArray) const;
    bool operator>  (const StillArray& rqArray) const;
    bool operator>= (const StillArray& rqArray) const;

    // swapping
    virtual void Swap (StillArray& rqArray);

protected:
    int m_iQuantity;
    ElementT* m_atElement;
};

#include "QStillArray.inl"

}

#endif



