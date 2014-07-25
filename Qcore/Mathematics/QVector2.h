#ifndef QVECTOR2_H
#define QVECTOR2_H

#include "QMath.h"

namespace Q
{

template <class Real>
class Vector2
{
public:
    // construction
    Vector2 ();  // uninitialized
    Vector2 (Real fX, Real fY);
    Vector2 (const Vector2& rqV);

    // coordinate access
    operator Real* ();
    operator const Real* () const;
    Real operator[] (int i) const;
    Real& operator[] (int i);
    Real X () const;
    Real& X ();
    Real Y () const;
    Real& Y ();

    // assignment
    Vector2& operator= (const Vector2& rqV);

    // comparison
    bool operator== (const Vector2& rqV) const;
    bool operator!= (const Vector2& rqV) const;
    bool operator<  (const Vector2& rqV) const;
    bool operator<= (const Vector2& rqV) const;
    bool operator>  (const Vector2& rqV) const;
    bool operator>= (const Vector2& rqV) const;

    // arithmetic operations
    Vector2 operator+ (const Vector2& rqV) const;
    Vector2 operator- (const Vector2& rqV) const;
    Vector2 operator* (Real fScalar) const;
    Vector2 operator/ (Real fScalar) const;
    Vector2 operator- () const;

    // arithmetic updates
    Vector2& operator+= (const Vector2& rqV);
    Vector2& operator-= (const Vector2& rqV);
    Vector2& operator*= (Real fScalar);
    Vector2& operator/= (Real fScalar);

    // vector operations
    Real Length () const;
    Real SquaredLength () const;
    Real Dot (const Vector2& rqV) const;
    Real Normalize ();

    // returns (y,-x)
    Vector2 Perp () const;

    // returns (y,-x)/sqrt(x*x+y*y)
    Vector2 UnitPerp () const;

    // returns DotPerp((x,y),(V.x,V.y)) = x*V.y - y*V.x
    Real DotPerp (const Vector2& rqV) const;

    // Compute the barycentric coordinates of the point with respect to the triangle <V0,V1,V2>,
    // P = b0*V0 + b1*V1 + b2*V2, where b0 + b1 + b2 = 1.
    void GetBarycentrics (const Vector2<Real>& rqV0, const Vector2<Real>& rqV1, const Vector2<Real>& rqV2,
        Real afBary[3]) const;

    // Gram-Schmidt orthonormalization.  Take linearly independent vectors U and V and compute an orthonormal
    // set (unit length, mutually perpendicular).
    static void Orthonormalize (Vector2& rqU, Vector2& rqV);

    // Input V must be initialized to a nonzero vector, output is {U,V}, an orthonormal basis.  A hint is
    // provided about whether or not V is already unit length.
    static void GenerateOrthonormalBasis (Vector2& rqU, Vector2& rqV, bool bUnitLengthV);

    // special vectors
    Q_ITEM static const Vector2 ZERO;
    Q_ITEM static const Vector2 UNIT_X;
    Q_ITEM static const Vector2 UNIT_Y;

private:
    Real m_afTuple[2];
};

template <class Real>
Vector2<Real> operator* (Real fScalar, const Vector2<Real>& rqV);

#include "QVector2.inl"

typedef Vector2<float> Vector2f;
typedef Vector2<double> Vector2d;

}

#endif



