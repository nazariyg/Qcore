#ifndef QSORTUTILS_H
#define QSORTUTILS_H

// The class' member functions use the Quick Sort algorithm with stack emulation to organize large arrays and
// the Insertion Sort algorithm for short arrays (and subarrays).  The ElementT template parameter is the type
// of array elements.  With the purpose of performing comparison of two array elements, the ComparatorT
// template parameter must be a class with overloaded operator '()' and to look like this:
//   bool operator() (const Class& ..., const Class& ...) const;
// where Class is the same as ElementT, the 'const' and '&' are optional.  The function is expected to return
// 'true' if in the sorted array the first comparative must appear before the second one and 'false' otherwise.
// In sub-sorting "To" indicates the index (position for lists) of the array element which goes just after
// the last element participating in sorting process; surely, if sorting is to be applied from some element to
// the very end, "To" index/position is imaginary.

#include "QArray.h"
#include "QList.h"

namespace Q
{

template <class ElementT, class ComparatorT = DefaultComparator<ElementT> >
class SortUtils
{
public:
    // For native arrays.  iFrom must be <= iTo.
    static void Sort (ElementT* atElement, int iFrom, int iTo);
    static void Sort (int iQuantity, ElementT* atElement);

    // For StillArray objects.  iFrom must be <= iTo.
    static void Sort (StillArray<ElementT>& rqArray, int iFrom, int iTo);
    static void Sort (StillArray<ElementT>& rqArray);

    // For List objects.  qFrom must point lefter than qTo, qFrom = qTo allowed.
    static void Sort (ListIterator<ElementT> qFrom, ListIterator<ElementT> qTo);
    static void Sort (List<ElementT>& rqList);

private:
    static const ComparatorT ms_tComparator;
};

#include "QSortUtils.inl"

}

#endif



