#ifndef QMATH_H
#define QMATH_H

#include "QMathi.h"

namespace Q
{

template <class Real>
class Math
{
public:
    static Real Min (Real fValue0, Real fValue1);
    static Real Max (Real fValue0, Real fValue1);
    static Real Min3 (Real fValue0, Real fValue1, Real fValue2);
    static Real Max3 (Real fValue0, Real fValue1, Real fValue2);
    static void MinMax (Real fValue0, Real fValue1, Real& rfMin, Real& rfMax);
    static void MinMax3 (Real fValue0, Real fValue1, Real fValue2, Real& rfMin, Real& rfMax);

    static int RoundToInt (Real fValue);

    // Wrappers to hide implementations of function.  The ACos and ASin function clamp the input argument to
    // [-1,1] to avoid NaN issues when the input is slightly larger than 1 or slightly smaller than -1.  Other
    // function have the potential for using a fast and approximate algorithm rather than calling the standard
    // math library function.
    static Real ACos (Real fValue);
    static Real ASin (Real fValue);
    static Real ATan (Real fValue);
    static Real ATan2 (Real fY, Real fX);
    static Real Ceil (Real fValue);
    static Real Cos (Real fValue);
    static Real Exp (Real fValue);
    static Real FAbs (Real fValue);
    static Real Floor (Real fValue);
    static Real FMod (Real fX, Real fY);
    static Real InvSqrt (Real fValue);
    static Real Log (Real fValue);
    static Real Pow (Real fBase, Real fExponent);
    static Real Sin (Real fValue);
    static Real Sqr (Real fValue);
    static Real Sqrt (Real fValue);
    static Real Tan (Real fValue);

    // Return -1 if the input is negative, 0 if the input is zero, and +1 if the input is positive.
    static Real Sign (Real fValue);

    // Generate a random number in [0,1).  The random number generator may be seeded by a first call to
    // UnitRandom with a positive seed.
    static Real UnitRandom (unsigned int uiSeed = 0);

    // Generate a random number in [-1,1).  The random number generator may be seeded by a first call to
    // SymmetricRandom with a positive seed.
    static Real SymmetricRandom (unsigned int uiSeed = 0);

    // Generate a random number in [min,max).  The random number generator may be seeded by a first call to
    // IntervalRandom with a positive seed.
    static Real IntervalRandom (Real fMin, Real fMax, unsigned int uiSeed = 0);

    // Fast evaluation of trigonometric and inverse trigonometric function using polynomial approximations.

    // The input must be in [0,pi/2].
    // max error sin0 = 1.7e-04, speed up = 4.0
    // max error sin1 = 1.9e-08, speed up = 2.8
    static Real FastSin0 (Real fAngle);
    static Real FastSin1 (Real fAngle);

    // The input must be in [0,pi/2]
    // max error cos0 = 1.2e-03, speed up = 4.5
    // max error cos1 = 6.5e-09, speed up = 2.8
    static Real FastCos0 (Real fAngle);
    static Real FastCos1 (Real fAngle);

    // The input must be in [0,pi/4].
    // max error tan0 = 8.1e-04, speed up = 5.6
    // max error tan1 = 1.9e-08, speed up = 3.4
    static Real FastTan0 (Real fAngle);
    static Real FastTan1 (Real fAngle);

    // The input must be in [0,1].
    // max error invsin0 = 6.8e-05, speed up = 7.5
    // max error invsin1 = 1.4e-07, speed up = 5.5
    static Real FastInvSin0 (Real fValue);
    static Real FastInvSin1 (Real fValue);

    // The input must be in [0,1].
    // max error invcos0 = 6.8e-05, speed up = 7.5
    // max error invcos1 = 1.4e-07, speed up = 5.7
    static Real FastInvCos0 (Real fValue);
    static Real FastInvCos1 (Real fValue);

    // The input must be in [-1,1].
    // max error invtan0 = 1.2e-05, speed up = 2.8
    // max error invtan1 = 2.3e-08, speed up = 1.8
    static Real FastInvTan0 (Real fValue);
    static Real FastInvTan1 (Real fValue);

    // A fast approximation to 1/sqrt.
    Q_ITEM static Real FastInvSqrt (Real fValue);

    // common constants
    Q_ITEM static const Real EPSILON;
    Q_ITEM static const Real ZERO_TOLERANCE;
    Q_ITEM static const Real MIN_REAL;
    Q_ITEM static const Real MAX_REAL;
    Q_ITEM static const Real PI;
    Q_ITEM static const Real TWO_PI;
    Q_ITEM static const Real HALF_PI;
    Q_ITEM static const Real INV_PI;
    Q_ITEM static const Real INV_TWO_PI;
    Q_ITEM static const Real DEG_TO_RAD;
    Q_ITEM static const Real RAD_TO_DEG;
};

#include "QMath.inl"
#include "QMath.mcr"

typedef Math<float> Mathf;
typedef Math<double> Mathd;

}

#endif



