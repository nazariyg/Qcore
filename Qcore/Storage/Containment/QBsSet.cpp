#include "QBsSet.h"
using namespace Q;

//------------------------------------------------------------------------------------------------------------------
// explicit instantiation
//------------------------------------------------------------------------------------------------------------------
// native
template
class BsSet<int>;

// aggregate
template
class BsSet<Link<int,int> >;
//------------------------------------------------------------------------------------------------------------------



