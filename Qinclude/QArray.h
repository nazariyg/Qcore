#ifndef QARRAY_H
#define QARRAY_H

// A push/pop-able array container class.  The Array class, in contrast to the ModestArray class, is
// appropriate for storing arrays of unknown but presumably substantial size and which are likely to
// grow/shrink either time-intensively or by inconstant element quantity, or both.  (For the inherited
// interface and related information, you may look into the headers of the base classes: StillArray and
// DynamicArrayBase.)  The ElementT template parameter is either a native data type or a class that has the
// next member functions:
//   Class::Class ();
//   Class::Class (const Class& ...);
//   Class& Class::operator= (const Class& ...);
// where Class is the same as ElementT, the 'const' and '&' are optional.  If usage of an inter-array
// comparison operator is implied, the ElementT class may require to be equipped with one of the following
// member functions:
//   bool Class::operator== (const Class& ...);
//   bool Class::operator<  (const Class& ...);

#include "QDynamicArrayBase.h"

namespace Q
{

template <class ElementT>
class Array : public DynamicArrayBase<ElementT>
{
public:
    // construction
    Array (int iQuantity = 0);
    Array (int iQuantity, int iMaxQuantity);
    Array (const Array& rqArray);

    // element pushing
    int Push (const ElementT& rtElement);  // returns the position of the pushed element
    ElementT& GetNewLast ();               // alternative way if an element is expensive to assign

private:
    void ConsiderGrowing ();

    // indicates how much an array is to grow depending on the number of previously allocated slots
    static const int ms_iGrowQuantityDivider;  // 3 (on 1/3 each time)
};

#include "QArray.inl"

}

#endif



