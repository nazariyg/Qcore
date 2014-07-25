#ifndef QGVECTOR_H
#define QGVECTOR_H

#include "QMath.h"

namespace Q
{

template <class Real>
class GVector
{
public:
    // construction and destruction
    GVector (int iSize = 0);
    GVector (int iSize, const Real* afTuple);
    GVector (const GVector& rqV);
    ~GVector ();

    // coordinate access
    void SetSize (int iSize);
    int GetSize () const;
    operator const Real* () const;
    operator Real* ();
    Real operator[] (int i) const;
    Real& operator[] (int i);

    // assignment
    GVector& operator= (const GVector& rqV);

    // comparison
    bool operator== (const GVector& rqV) const;
    bool operator!= (const GVector& rqV) const;
    bool operator<  (const GVector& rqV) const;
    bool operator<= (const GVector& rqV) const;
    bool operator>  (const GVector& rqV) const;
    bool operator>= (const GVector& rqV) const;

    // arithmetic operations
    GVector operator+ (const GVector& rqV) const;
    GVector operator- (const GVector& rqV) const;
    GVector operator* (Real fScalar) const;
    GVector operator/ (Real fScalar) const;
    GVector operator- () const;

    // arithmetic updates
    GVector& operator+= (const GVector& rqV);
    GVector& operator-= (const GVector& rqV);
    GVector& operator*= (Real fScalar);
    GVector& operator/= (Real fScalar);

    // vector operations
    Real Length () const;
    Real SquaredLength () const;
    Real Dot (const GVector& rqV) const;
    Real Normalize ();

protected:
    // support for comparisons
    int CompareArrays (const GVector& rqV) const;

    int m_iSize;
    Real* m_afTuple;
};

template <class Real>
GVector<Real> operator* (Real fScalar, const GVector<Real>& rqV);

#include "QGVector.inl"

typedef GVector<float> GVectorf;
typedef GVector<double> GVectord;

}

#endif



