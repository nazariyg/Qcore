#include "QStillArray.h"
using namespace Q;

//------------------------------------------------------------------------------------------------------------------
// explicit instantiation
//------------------------------------------------------------------------------------------------------------------
// native
template
class StillArray<int>;

// aggregate
template
class StillArray<Link<int,int> >;
//------------------------------------------------------------------------------------------------------------------



