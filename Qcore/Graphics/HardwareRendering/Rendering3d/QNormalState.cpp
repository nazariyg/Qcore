#include "QNormalState.h"
using namespace Q;

Q_IMPLEMENT_TEMPLATE_RTTI(Q,HeapRtti,NormalStatef);
Q_IMPLEMENT_TEMPLATE_RTTI(Q,HeapRtti,NormalStated);

//------------------------------------------------------------------------------------------------------------------
// explicit instantiation
//------------------------------------------------------------------------------------------------------------------
// float
template
class NormalState<float>;

// double
template
class NormalState<double>;
//------------------------------------------------------------------------------------------------------------------



