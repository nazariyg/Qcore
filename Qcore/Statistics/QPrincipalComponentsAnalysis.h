#ifndef QPRINCIPALCOMPONENTSANALYSIS_H
#define QPRINCIPALCOMPONENTSANALYSIS_H

#include "QStillArray.h"
#include "QGMatrix.h"

namespace Q
{

template <class Real>
class Q_ITEM PrincipalComponentsAnalysis
{
public:
    // Construction.  Parameter iN is number of dimensions and iQuantity is number of elements in each
    // dimension; both must be >= 2.  Parameter bSampling indicates if properties of whole data set will be
    // extracted using its sample subset or via examining entire information (influences calculation of a
    // covariance).
    PrincipalComponentsAnalysis (int iN, int iQuantity, bool bSampling);

    // setting of data to be analyzed
    void SetValue (int iDim, int i, Real fValue);

    // Analysis.  The process takes into account special cases of 2-, 3-, and 4-dimensional eigensolving.
    void Analyze ();

    // Getting the result.  The N-dimensional feature vector is a matrix with eigenvectors forming its columns
    // in decreasing order accordingly to eigenvalues.
    Real GetMean (int iDim) const;  // may be useful
    Real GetPrincipalEigenvalue () const;
    Real GetEigenvalue (int i) const;
    const Real* GetEigenvalues () const;
    void GetPrincipalComponent (Vector2<Real>& rqV) const;
    void GetPrincipalComponent (Vector3<Real>& rqV) const;
    void GetPrincipalComponent (Vector4<Real>& rqV) const;
    GVector<Real> GetPrincipalComponent () const;
    void GetComponent (int i, Vector2<Real>& rqV) const;
    void GetComponent (int i, Vector3<Real>& rqV) const;
    void GetComponent (int i, Vector4<Real>& rqV) const;
    GVector<Real> GetComponent (int i) const;
    void GetFeatureVector (Matrix2<Real>& rqM) const;
    void GetFeatureVector (Matrix3<Real>& rqM) const;
    void GetFeatureVector (Matrix4<Real>& rqM) const;
    const GMatrix<Real>& GetFeatureVector () const;

private:
    int m_iN, m_iQuantity;
    bool m_bSampling;
    StillArray<StillArray<Real> > m_qTable;
    StillArray<Real> m_qMeans;
    StillArray<Real> m_qEigenvalues;
    GMatrix<Real> m_qFeatureVector;
};

typedef PrincipalComponentsAnalysis<float> PrincipalComponentsAnalysisf;
typedef PrincipalComponentsAnalysis<double> PrincipalComponentsAnalysisd;

}

#endif



