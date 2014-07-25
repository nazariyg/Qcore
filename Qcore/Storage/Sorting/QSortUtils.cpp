#include "QSortUtils.h"
using namespace Q;

//------------------------------------------------------------------------------------------------------------------
// explicit instantiation
//------------------------------------------------------------------------------------------------------------------
// native
template
class SortUtils<int,DefaultComparator<int> >;

// aggregate
template
class SortUtils<Link<int,int>,DefaultComparator<Link<int,int> > >;
//------------------------------------------------------------------------------------------------------------------



