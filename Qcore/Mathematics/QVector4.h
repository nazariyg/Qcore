#ifndef QVECTOR4_H
#define QVECTOR4_H

#include "QMath.h"

namespace Q
{

template <class Real>
class Vector4
{
public:
    // construction
    Vector4 ();  // uninitialized
    Vector4 (Real fX, Real fY, Real fZ, Real fW);
    Vector4 (const Vector4& rqV);

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
    Real W () const;
    Real& W ();

    // assignment
    Vector4& operator= (const Vector4& rqV);

    // comparison
    bool operator== (const Vector4& rqV) const;
    bool operator!= (const Vector4& rqV) const;
    bool operator<  (const Vector4& rqV) const;
    bool operator<= (const Vector4& rqV) const;
    bool operator>  (const Vector4& rqV) const;
    bool operator>= (const Vector4& rqV) const;

    // arithmetic operations
    Vector4 operator+ (const Vector4& rqV) const;
    Vector4 operator- (const Vector4& rqV) const;
    Vector4 operator* (Real fScalar) const;
    Vector4 operator/ (Real fScalar) const;
    Vector4 operator- () const;

    // arithmetic updates
    Vector4& operator+= (const Vector4& rqV);
    Vector4& operator-= (const Vector4& rqV);
    Vector4& operator*= (Real fScalar);
    Vector4& operator/= (Real fScalar);

    // vector operations
    Real Length () const;
    Real SquaredLength () const;
    Real Dot (const Vector4& rqV) const;
    Real Normalize ();

    // special vectors
    Q_ITEM static const Vector4 ZERO;
    Q_ITEM static const Vector4 UNIT_X;
    Q_ITEM static const Vector4 UNIT_Y;
    Q_ITEM static const Vector4 UNIT_Z;
    Q_ITEM static const Vector4 UNIT_W;

private:
    Real m_afTuple[4];
};

template <class Real>
Vector4<Real> operator* (Real fScalar, const Vector4<Real>& rqV);

#include "QVector4.inl"

typedef Vector4<float> Vector4f;
typedef Vector4<double> Vector4d;

}

#endif



