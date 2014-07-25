#ifndef QMODESTARRAY_H
#define QMODESTARRAY_H

// A push/pop-able array container class.  The ModestArray class, in contrast to the Array class, is
// appropriate for storing arrays likely to grow/shrink gradually by an unvarying and usually small element
// quantity.  (For the inherited interface and related information, you may look into the headers of the base
// classes: StillArray and DynamicArrayBase.)  The ElementT template parameter is either a native data type or
// a class that has the next member functions:
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
class ModestArray : public DynamicArrayBase<ElementT>
{

Q_DECLARE_STREAM;

public:
    // Construction.  Parameter iGrowBy specifies how much the array must grow when a push-function needs an
    // additional element slot.
    ModestArray (int iQuantity = 0, int iGrowBy = 1);
    ModestArray (int iQuantity, int iMaxQuantity, int iGrowBy = 1);
    ModestArray (const ModestArray& rqArray);

    // member access
    void SetGrowBy (int iGrowBy);
    int GetGrowBy () const;

    // element pushing
    int Push (const ElementT& rtElement);  // returns the position of the pushed element
    ElementT& GetNewLast ();               // the alternative way, when an element is expensive to assign

private:
    void ConsiderGrowing ();

    int m_iGrowBy;
};

#include "QModestArray.inl"

}

#endif



