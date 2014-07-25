#ifndef QPOLYNOMIAL_H
#define QPOLYNOMIAL_H

#include "QMath.h"

namespace Q
{

template <class Real>
class Polynomial
{
public:
    // construction and destruction
    Polynomial (int iDegree = -1);
    Polynomial (const Polynomial& rqPoly);
    ~Polynomial ();

    // member access
    void SetDegree (int iDegree);
    int GetDegree () const;
    operator const Real* () const;
    operator Real* ();
    Real operator[] (int i) const;
    Real& operator[] (int i);

    // assignment
    Polynomial& operator= (const Polynomial& rqPoly);

    // evaluation
    Real operator() (Real fT) const;

    // arithmetic operations
    Polynomial operator+ (const Polynomial& rqPoly) const;
    Polynomial operator- (const Polynomial& rqPoly) const;
    Polynomial operator* (const Polynomial& rqPoly) const;
    Polynomial operator+ (Real fScalar) const;  // input is degree 0 poly
    Polynomial operator- (Real fScalar) const;  // input is degree 0 poly
    Polynomial operator* (Real fScalar) const;
    Polynomial operator/ (Real fScalar) const;
    Polynomial operator- () const;

    // arithmetic updates
    Polynomial& operator+= (const Polynomial& rqPoly);
    Polynomial& operator-= (const Polynomial& rqPoly);
    Polynomial& operator*= (const Polynomial& rqPoly);
    Polynomial& operator+= (Real fScalar);  // input is degree 0 poly
    Polynomial& operator-= (Real fScalar);  // input is degree 0 poly
    Polynomial& operator*= (Real fScalar);
    Polynomial& operator/= (Real fScalar);

    // derivation
    Polynomial GetDerivative () const;

    // inversion ( invpoly[i] = poly[degree-i] for 0 <= i <= degree )
    Polynomial GetInversion () const;

    // Reduce degree by eliminating all (nearly) zero leading coefficients and by making the leading
    // coefficient one.  The input parameter is the threshold for specifying that a coefficient is effectively
    // zero.
    void Compress (Real fEpsilon);

    // If 'this' is P(t) and the divisor is D(t) with degree(P) >= degree(D), then P(t) = Q(t)*D(t)+R(t) where
    // Q(t) is the quotient with degree(Q) = degree(P) - degree(D) and R(t) is the remainder with
    // degree(R) < degree(D).  If this routine is called with degree(P) < degree(D), then Q = 0 and R = P are
    // returned.  The value of epsilon is used as a threshold on the coefficients of the remainder polynomial.
    // If smaller, the coefficient is assumed to be zero.
    void Divide (const Polynomial& rqDiv, Polynomial& rqQuot, Polynomial& rqRem, Real fEpsilon) const;

protected:
    int m_iDegree;
    Real* m_afCoeff;
};

template <class Real>
Polynomial<Real> operator* (Real fScalar, const Polynomial<Real>& rqPoly);

#include "QPolynomial.inl"

typedef Polynomial<float> Polynomialf;
typedef Polynomial<double> Polynomiald;

}

#endif



