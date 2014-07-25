#ifndef QPOLYNOMIALROOTSOLVING_H
#define QPOLYNOMIALROOTSOLVING_H

// Methods are
//
// A: algebraic using closed-form expressions (fast, typically not robust)
// B: bisection (after root bounding, slow and robust)
// N: Newton's/bisection hybrid (after root bounding, medium and robust)
// E: eigenvalues of a companion matrix (fast and robust)

// Root bounds:
//
// For a monic polynomial
//   x^n + a[n-1]*x^{n-1} + ... + a[1]*x + a[0]
// the Cauchy bound is
//   M = 1 + max{|a[0]|,...,|a[n-1]|}.
// All real-value roots must lie in the interval [-M,M].  For a non-monic polynomial,
//   b[n]*x^n + b[n-1]*x^{n-1} + ... + b[1]*x + b[0],
// if b[n] is not zero, divide through by it and calculate Cauchy's bound:
//   1 + max{|b[0]/b[n]|,...,|b[n-1]/b[n]|}.

#include "QGMatrix.h"
#include "QPolynomial.h"
#include "QModestArray.h"

namespace Q
{

template <class Real>
class Q_ITEM PolynomialRootSolving
{
public:
    // construction and destruction
    PolynomialRootSolving (Real fEpsilon);
    ~PolynomialRootSolving ();

    // member access
    int GetCount () const;
    const Real* GetRoots () const;
    Real GetRoot (int i) const;
    Real& Epsilon ();
    Real Epsilon () const;

    // for FindE functions, default is 128
    int& MaxIterations ();
    int MaxIterations () const;

    // linear equations:  c1*x+c0 = 0
    bool FindA (Real fC0, Real fC1);
    Real GetBound (Real fC0, Real fC1);

    // quadratic equations: c2*x^2+c1*x+c0 = 0
    bool FindA (Real fC0, Real fC1, Real fC2);
    Real GetBound (Real fC0, Real fC1, Real fC2);

    // cubic equations: c3*x^3+c2*x^2+c1*x+c0 = 0
    bool FindA (Real fC0, Real fC1, Real fC2, Real fC3);
    bool FindE (Real fC0, Real fC1, Real fC2, Real fC3, bool bDoBalancing);
    Real GetBound (Real fC0, Real fC1, Real fC2, Real fC3);

    // Solve A*r^3 + B*r = C where A > 0 and B > 0.  This equation always has exactly one root.
    Real SpecialCubic (Real fA, Real fB, Real fC);

    // quartic equations: c4*x^4+c3*x^3+c2*x^2+c1*x+c0 = 0
    bool FindA (Real fC0, Real fC1, Real fC2, Real fC3, Real fC4);
    bool FindE (Real fC0, Real fC1, Real fC2, Real fC3, Real fC4, bool bDoBalancing);
    Real GetBound (Real fC0, Real fC1, Real fC2, Real fC3, Real fC4);

    // general equations: sum_{i=0}^{degree} c(i)*x^i = 0
    bool FindB (const Polynomial<Real>& rqPoly, int iDigits);
    bool FindN (const Polynomial<Real>& rqPoly, int iDigits);
    bool FindE (const Polynomial<Real>& rqPoly, bool bDoBalancing);
    Real GetBound (const Polynomial<Real>& rqPoly);

    // find roots on specified intervals
    bool FindB (const Polynomial<Real>& rqPoly, Real fXMin, Real fXMax, int iDigits);
    bool FindN (const Polynomial<Real>& rqPoly, Real fXMin, Real fXMax, int iDigits);

    bool AllRealPartsNegative (const Polynomial<Real>& rqPoly);
    bool AllRealPartsPositive (const Polynomial<Real>& rqPoly);

    // Count the number of roots on [t0,t1].  Uses Sturm sequences to do the counting.  It is allowed to pass
    // in t0 = -Math<Real>::MAX_REAL or t1 = Math<Real>::MAX_REAL.  The value of m_fEpsilon is used as a
    // threshold on the value of a Sturm polynomial at an end point.  If smaller, that value is assumed to be
    // zero.  The return value is the number of roots.  If there are infinitely many, -1 is returned.
    int GetRootCount (const Polynomial<Real>& rqPoly, Real fT0, Real fT1);

private:
    // support for FindB
    bool Bisection (const Polynomial<Real>& rqPoly, Real fXMin, Real fXMax, int iDigitsAccuracy, Real& rfRoot);

    // support for FindE
    void GetHouseholderVector (int iSize, const Vector3<Real>& rqU, Vector3<Real>& rqV);

    void PremultiplyHouseholder (GMatrix<Real>& rqMat, GVector<Real>& rqW, int iRMin, int iRMax, int iCMin,
        int iCMax, int iVSize, const Vector3<Real>& rqV);

    void PostmultiplyHouseholder (GMatrix<Real>& rqMat, GVector<Real>& rqW, int iRMin, int iRMax, int iCMin,
        int iCMax, int iVSize, const Vector3<Real>& rqV);

    void FrancisQRStep (GMatrix<Real>& rqH, GVector<Real>& rqW);

    Real GetRowNorm (int iRow, GMatrix<Real>& rqMat);
    Real GetColNorm (int iCol, GMatrix<Real>& rqMat);
    void ScaleRow (int iRow, Real fScale, GMatrix<Real>& rqMat);
    void ScaleCol (int iCol, Real fScale, GMatrix<Real>& rqMat);
    void Balance3 (GMatrix<Real>& rqMat);
    bool IsBalanced3 (GMatrix<Real>& rqMat);
    void BalanceCompanion3 (GMatrix<Real>& rqMat);
    bool IsBalancedCompanion3 (Real fA10, Real fA21, Real fA02, Real fA12, Real fA22);
    bool QRIteration3 (GMatrix<Real>& rqMat);

    void BalanceCompanion4 (GMatrix<Real>& rqMat);
    bool IsBalancedCompanion4 (Real fA10, Real fA21, Real fA32, Real fA03, Real fA13, Real fA23, Real fA33);
    bool QRIteration4 (GMatrix<Real>& rqMat);

    // support for testing if all roots have negative real parts
    bool AllRealPartsNegative (int iDegree, Real* afCoeff);

    Real m_fEpsilon;
    int m_iCount, m_iMaxRoot;
    Real* m_afRoot;
    int m_iMaxIterations;
};

typedef PolynomialRootSolving<float> PolynomialRootSolvingf;
typedef PolynomialRootSolving<double> PolynomialRootSolvingd;

}

#endif



