#include "QHashMap.h"
using namespace Q;

//------------------------------------------------------------------------------------------------------------------
// explicit instantiation
//------------------------------------------------------------------------------------------------------------------
// native
template
class HashMapIterator<int,int,DefaultHasher<int>,DefaultEquator<int> >;

// aggregate
template
class HashMapIterator<Link<int,int>,Link<int,int>,DefaultHasher<Link<int,int> >,DefaultEquator<Link<int,int> > >;
//------------------------------------------------------------------------------------------------------------------



