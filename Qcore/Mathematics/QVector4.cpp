#include "QVector4.h"
using namespace Q;

template<> const Vector4<float> Vector4<float>::ZERO(0.0f,0.0f,0.0f,0.0f);
template<> const Vector4<float> Vector4<float>::UNIT_X(1.0f,0.0f,0.0f,0.0f);
template<> const Vector4<float> Vector4<float>::UNIT_Y(0.0f,1.0f,0.0f,0.0f);
template<> const Vector4<float> Vector4<float>::UNIT_Z(0.0f,0.0f,1.0f,0.0f);
template<> const Vector4<float> Vector4<float>::UNIT_W(0.0f,0.0f,0.0f,1.0f);

template<> const Vector4<double> Vector4<double>::ZERO(0.0,0.0,0.0,0.0);
template<> const Vector4<double> Vector4<double>::UNIT_X(1.0,0.0,0.0,0.0);
template<> const Vector4<double> Vector4<double>::UNIT_Y(0.0,1.0,0.0,0.0);
template<> const Vector4<double> Vector4<double>::UNIT_Z(0.0,0.0,1.0,0.0);
template<> const Vector4<double> Vector4<double>::UNIT_W(0.0,0.0,0.0,1.0);

//------------------------------------------------------------------------------------------------------------------
// explicit instantiation
//------------------------------------------------------------------------------------------------------------------
// float
template
class Vector4<float>;

// double
template
class Vector4<double>;
//------------------------------------------------------------------------------------------------------------------



