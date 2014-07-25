#include "QMap.h"
using namespace Q;

//------------------------------------------------------------------------------------------------------------------
// explicit instantiation
//------------------------------------------------------------------------------------------------------------------
// native
template
class MapIterator<int,int,DefaultEqComparator<int> >;

// aggregate
template
class MapIterator<Link<int,int>,Link<int,int>,DefaultEqComparator<Link<int,int> > >;
//------------------------------------------------------------------------------------------------------------------



