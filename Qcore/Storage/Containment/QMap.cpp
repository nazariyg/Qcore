#include "QMap.h"
using namespace Q;

//------------------------------------------------------------------------------------------------------------------
// explicit instantiation
//------------------------------------------------------------------------------------------------------------------
// native
template
class Map<int,int,DefaultEqComparator<int> >;

// aggregate
template
class Map<Link<int,int>,Link<int,int>,DefaultEqComparator<Link<int,int> > >;
//------------------------------------------------------------------------------------------------------------------



