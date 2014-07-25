#include "QMatrix3.h"
using namespace Q;

template<> const Matrix3<float> Matrix3<float>::ZERO(
    0.0f,0.0f,0.0f,
    0.0f,0.0f,0.0f,
    0.0f,0.0f,0.0f);
template<> const Matrix3<float> Matrix3<float>::IDENTITY(
    1.0f,0.0f,0.0f,
    0.0f,1.0f,0.0f,
    0.0f,0.0f,1.0f);

template<> const Matrix3<double> Matrix3<double>::ZERO(
    0.0,0.0,0.0,
    0.0,0.0,0.0,
    0.0,0.0,0.0);
template<> const Matrix3<double> Matrix3<double>::IDENTITY(
    1.0,0.0,0.0,
    0.0,1.0,0.0,
    0.0,0.0,1.0);

//------------------------------------------------------------------------------------------------------------------
// explicit instantiation
//------------------------------------------------------------------------------------------------------------------
// float
template
class Matrix3<float>;

// double
template
class Matrix3<double>;
//------------------------------------------------------------------------------------------------------------------



