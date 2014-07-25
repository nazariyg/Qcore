#include "QHardwareRendering.h"
using namespace Q;

Q_IMPLEMENT_TEMPLATE_RTTI(Q,HeapRtti,TextureStatef);
Q_IMPLEMENT_TEMPLATE_RTTI(Q,HeapRtti,TextureStated);

//------------------------------------------------------------------------------------------------------------------
// explicit instantiation
//------------------------------------------------------------------------------------------------------------------
// float
template
class TextureState<float>;

// double
template
class TextureState<double>;
//------------------------------------------------------------------------------------------------------------------



