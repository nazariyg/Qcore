#include "QProjectionState.h"
using namespace Q;

Q_IMPLEMENT_TEMPLATE_RTTI(Q,HeapRtti,ProjectionStatef);
Q_IMPLEMENT_TEMPLATE_RTTI(Q,HeapRtti,ProjectionStated);

//------------------------------------------------------------------------------------------------------------------
// explicit instantiation
//------------------------------------------------------------------------------------------------------------------
// float
template
class ProjectionState<float>;

// double
template
class ProjectionState<double>;
//------------------------------------------------------------------------------------------------------------------



