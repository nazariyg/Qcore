#include "QComplex.h"
using namespace Q;

template<> const Complex<float> Complex<float>::I(0.0f,1.0f);
template<> const Complex<float> Complex<float>::ZERO(0.0f,0.0f);

template<> const Complex<double> Complex<double>::I(0.0,1.0);
template<> const Complex<double> Complex<double>::ZERO(0.0,0.0);

//------------------------------------------------------------------------------------------------------------------
// explicit instantiation
//------------------------------------------------------------------------------------------------------------------
// float
template
class Complex<float>;

// double
template
class Complex<double>;
//------------------------------------------------------------------------------------------------------------------



