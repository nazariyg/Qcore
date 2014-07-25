#ifndef QCOMPLEX_H
#define QCOMPLEX_H

#include "QMath.h"

namespace Q
{

template <class Real>
class Complex
{
public:
    // construction
    Complex ();  // uninitialized
    Complex (Real fRe, Real fIm);
    Complex (Real fValue);
    Complex (const Complex& rqC);

    // part access
    Real Re () const;
    Real& Re ();
    Real Im () const;
    Real& Im ();

    // assignment
    Complex& operator= (const Complex& rqC);
    Complex& operator= (Real fValue);

    // comparison
    bool operator== (const Complex& rqC) const;
    bool operator!= (const Complex& rqC) const;
    bool operator<  (const Complex& rqC) const;
    bool operator<= (const Complex& rqC) const;
    bool operator>  (const Complex& rqC) const;
    bool operator>= (const Complex& rqC) const;
    bool operator== (Real fValue) const;
    bool operator!= (Real fValue) const;

    // arithmetic operations
    Complex operator+ (const Complex& rqC) const;
    Complex operator- (const Complex& rqC) const;
    Complex operator* (const Complex& rqC) const;
    Complex operator/ (const Complex& rqC) const;
    Complex operator- () const;
    Complex operator+ (Real fValue) const;
    Complex operator- (Real fValue) const;
    Complex operator* (Real fValue) const;
    Complex operator/ (Real fValue) const;

    // arithmetic updates
    Complex& operator+= (const Complex& rqC);
    Complex& operator-= (const Complex& rqC);
    Complex& operator*= (const Complex& rqC);
    Complex& operator/= (const Complex& rqC);
    Complex& operator+= (Real fValue);
    Complex& operator-= (Real fValue);
    Complex& operator*= (Real fValue);
    Complex& operator/= (Real fValue);

    // conjugation
    Complex Conj () const;

    // properties
    Real Abs () const;
    Real Norm () const;

    Q_ITEM static const Complex I;
    Q_ITEM static const Complex ZERO;

private:
    Real m_afPart[2];
};

#include "QComplex.inl"

typedef Complex<float> Complexf;
typedef Complex<double> Complexd;

}

#endif



