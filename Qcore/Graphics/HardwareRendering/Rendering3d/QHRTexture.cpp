#include "QHRTexture.h"
using namespace Q;

Q_IMPLEMENT_TEMPLATE_RTTI(Q,Rtti,HRTexturef);
Q_IMPLEMENT_TEMPLATE_RTTI(Q,Rtti,HRTextured);

//------------------------------------------------------------------------------------------------------------------
// explicit instantiation
//------------------------------------------------------------------------------------------------------------------
// float
template
class HRTexture<float>;

// double
template
class HRTexture<double>;
//------------------------------------------------------------------------------------------------------------------



