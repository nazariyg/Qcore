#ifndef QEIGEN_H
#define QEIGEN_H

#include "QMatrix2.h"
#include "QMatrix3.h"
#include "QMatrix4.h"
#include "QGMatrix.h"

namespace Q
{

template <class Real>
class Q_ITEM Eigen
{
public:
    Eigen (int iSize);
    Eigen (const Matrix2<Real>& rqM);
    Eigen (const Matrix3<Real>& rqM);
    Eigen (const Matrix4<Real>& rqM);
    Eigen (const GMatrix<Real>& rqM);
    ~Eigen ();

    // set the matrix for eigensolving
    Real& operator() (int iRow, int iCol);
    Eigen& operator= (const Matrix2<Real>& rqM);
    Eigen& operator= (const Matrix3<Real>& rqM);
    Eigen& operator= (const Matrix4<Real>& rqM);
    Eigen& operator= (const GMatrix<Real>& rqM);

    // Get the eigenresults (eigenvectors are columns of eigenmatrix).  The GetEigenvector calls involving
    // Vector2, Vector3, and Vector4 should only be called if you know that the eigenmatrix is of the
    // appropriate size.
    Real GetEigenvalue (int i) const;
    const Real* GetEigenvalues () const;
    void GetEigenvector (int i, Vector2<Real>& rqV) const;
    void GetEigenvector (int i, Vector3<Real>& rqV) const;
    void GetEigenvector (int i, Vector4<Real>& rqV) const;
    GVector<Real> GetEigenvector (int i) const;
    const GMatrix<Real>& GetEigenvectors () const;

    // solve eigensystem
    void EigenStuff2 ();
    void EigenStuff3 ();
    void EigenStuff4 ();
    void EigenStuffN ();
    void EigenStuff  ();

    // solve eigensystem, use decreasing sort on eigenvalues
    void DecrSortEigenStuff2 ();
    void DecrSortEigenStuff3 ();
    void DecrSortEigenStuff4 ();
    void DecrSortEigenStuffN ();
    void DecrSortEigenStuff  ();

    // solve eigensystem, use increasing sort on eigenvalues
    void IncrSortEigenStuff2 ();
    void IncrSortEigenStuff3 ();
    void IncrSortEigenStuff4 ();
    void IncrSortEigenStuffN ();
    void IncrSortEigenStuff  ();

private:
    int m_iSize;
    GMatrix<Real> m_qMat;
    Real* m_afDiag;
    Real* m_afSubd;

    // For odd size matrices, the Householder reduction involves an odd number of reflections.  The product of
    // these is a reflection.  The QL algorithm uses rotations for further reductions.  The final orthogonal
    // matrix whose columns are the eigenvectors is a reflection, so its determinant is -1.  For even size
    // matrices, the Householder reduction involves an even number of reflections whose product is a rotation.
    // The final orthogonal matrix has determinant +1.  Many algorithms that need an eigendecomposition want a
    // rotation matrix.  We want to guarantee this is the case, so m_bRotation keeps track of this.  The
    // DecrSort and IncrSort further complicate the issue since they swap columns of the orthogonal matrix,
    // causing the matrix to toggle between rotation and reflection.  The value m_bRotation must be toggled
    // accordingly.
    bool m_bIsRotation;
    void GuaranteeRotation ();

    // Householder reduction to tridiagonal form
    void Tridiagonal2 ();
    void Tridiagonal3 ();
    void Tridiagonal4 ();
    void TridiagonalN ();

    // QL algorithm with implicit shifting, applies to tridiagonal matrices
    bool QLAlgorithm ();

    // sort eigenvalues from largest to smallest
    void DecreasingSort ();

    // sort eigenvalues from smallest to largest
    void IncreasingSort ();
};

typedef Eigen<float> Eigenf;
typedef Eigen<double> Eigend;

}

#endif



