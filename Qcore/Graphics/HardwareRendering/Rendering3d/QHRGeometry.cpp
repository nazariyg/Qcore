#include "QHRGeometry.h"
using namespace Q;

Q_IMPLEMENT_TEMPLATE_RTTI(Q,Rtti,HRGeometryf);
Q_IMPLEMENT_TEMPLATE_RTTI(Q,Rtti,HRGeometryd);

//------------------------------------------------------------------------------------------------------------------
// explicit instantiation
//------------------------------------------------------------------------------------------------------------------
// float
template
class HRGeometry<float>;

// double
template
class HRGeometry<double>;
//------------------------------------------------------------------------------------------------------------------



