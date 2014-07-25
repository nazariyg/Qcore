#include "QHRImage.h"
using namespace Q;

Q_IMPLEMENT_TEMPLATE_RTTI(Q,Rtti,HRImagef);
Q_IMPLEMENT_TEMPLATE_RTTI(Q,Rtti,HRImaged);

//------------------------------------------------------------------------------------------------------------------
// explicit instantiation
//------------------------------------------------------------------------------------------------------------------
// float
template
class HRImage<float>;

// double
template
class HRImage<double>;
//------------------------------------------------------------------------------------------------------------------



