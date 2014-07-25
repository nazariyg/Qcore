#include "QMatrix2.h"
using namespace Q;

template<> const Matrix2<float> Matrix2<float>::ZERO(
    0.0f,0.0f,
    0.0f,0.0f);
template<> const Matrix2<float> Matrix2<float>::IDENTITY(
    1.0f,0.0f,
    0.0f,1.0f);

template<> const Matrix2<double> Matrix2<double>::ZERO(
    0.0,0.0,
    0.0,0.0);
template<> const Matrix2<double> Matrix2<double>::IDENTITY(
    1.0,0.0,
    0.0,1.0);

//------------------------------------------------------------------------------------------------------------------
// explicit instantiation
//------------------------------------------------------------------------------------------------------------------
// float
template
class Matrix2<float>;

// double
template
class Matrix2<double>;
//------------------------------------------------------------------------------------------------------------------



