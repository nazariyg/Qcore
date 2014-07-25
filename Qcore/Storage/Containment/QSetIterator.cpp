#include "QSet.h"
using namespace Q;

//------------------------------------------------------------------------------------------------------------------
// explicit instantiation
//------------------------------------------------------------------------------------------------------------------
// native
template
class SetIterator<int,DefaultEqComparator<int> >;

// aggregate
template
class SetIterator<Link<int,int>,DefaultEqComparator<Link<int,int> > >;
//------------------------------------------------------------------------------------------------------------------



