#ifndef QVECTOR3_H
#define QVECTOR3_H

#include "QMath.h"

namespace Q
{

template <class Real>
class Vector3
{
public:
    // construction
    Vector3 ();  // uninitialized
    Vector3 (Real fX, Real fY, Real fZ);
    Vector3 (const Vector3& rqV);

    // coordinate access
    operator Real* ();
    operator const Real* () const;
    Real operator[] (int i) const;
    Real& operator[] (int i);
    Real X () const;
    Real& X ();
    Real Y () const;
    Real& Y ();
    Real Z () const;
    Real& Z ();

    // assignment
    Vector3& operator= (const Vector3& rqV);

    // comparison
    bool operator== (const Vector3& rqV) const;
    bool operator!= (const Vector3& rqV) const;
    bool operator<  (const Vector3& rqV) const;
    bool operator<= (const Vector3& rqV) const;
    bool operator>  (const Vector3& rqV) const;
    bool operator>= (const Vector3& rqV) const;

    // arithmetic operations
    Vector3 operator+ (const Vector3& rqV) const;
    Vector3 operator- (const Vector3& rqV) const;
    Vector3 operator* (Real fScalar) const;
    Vector3 operator/ (Real fScalar) const;
    Vector3 operator- () const;

    // arithmetic updates
    Vector3& operator+= (const Vector3& rqV);
    Vector3& operator-= (const Vector3& rqV);
    Vector3& operator*= (Real fScalar);
    Vector3& operator/= (Real fScalar);

    // vector operations
    Real Length () const;
    Real SquaredLength () const;
    Real Dot (const Vector3& rqV) const;
    Real Normalize ();

    // The cross products are computed using the right-handed rule.  If API expects left-handed rule, sign on
    // the vector must be changed (each component value c replaced by -c).
    Vector3 Cross (const Vector3& rqV) const;
    Vector3 UnitCross (const Vector3& rqV) const;

    // Compute the barycentric coordinates of the point with respect to the tetrahedron <V0,V1,V2,V3>,
    // P = b0*V0 + b1*V1 + b2*V2 + b3*V3, where b0 + b1 + b2 + b3 = 1.
    void GetBarycentrics (const Vector3<Real>& rqV0, const Vector3<Real>& rqV1, const Vector3<Real>& rqV2,
        const Vector3<Real>& rqV3, Real afBary[4]) const;

    // Gram-Schmidt orthonormalization.  Take linearly independent vectors U, V, and W and compute an
    // orthonormal set (unit length, mutually perpendicular).
    static void Orthonormalize (Vector3& rqU, Vector3& rqV, Vector3& rqW);
    static void Orthonormalize (Vector3* aqV);

    // Input W must be initialized to a nonzero vector, output is {U,V,W}, an orthonormal basis.  A hint is
    // provided about whether or not W is already unit length.
    static void GenerateOrthonormalBasis (Vector3& rqU, Vector3& rqV, Vector3& rqW, bool bUnitLengthW);

    // special vectors
    Q_ITEM static const Vector3 ZERO;
    Q_ITEM static const Vector3 UNIT_X;
    Q_ITEM static const Vector3 UNIT_Y;
    Q_ITEM static const Vector3 UNIT_Z;

private:
    Real m_afTuple[3];
};

template <class Real>
Vector3<Real> operator* (Real fScalar, const Vector3<Real>& rqV);

#include "QVector3.inl"

typedef Vector3<float> Vector3f;
typedef Vector3<double> Vector3d;

}

#endif



