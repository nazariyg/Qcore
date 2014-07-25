#ifndef QGMATRIX_H
#define QGMATRIX_H

// Matrix operations are applied on the left.  For example, given a matrix M and a vector V,
// matrix-times-vector is M*V.  That is, V is treated as a column vector.  Some graphics APIs use V*M where V
// is treated as a row vector.  In this context the "M" matrix is really a transpose of the M.  Similarly, to
// apply two matrix operations M0 and M1, in that order, you compute M1*M0 so that the transform of a vector is
// (M1*M0)*V = M1*(M0*V).  Some graphics APIs use M0*M1, but again these matrices are the transpose of those.
//
// Matrices are stored in row-major order, matrix[row][col].

#include "QGVector.h"

namespace Q
{

template <class Real>
class GMatrix
{
public:
    // construction and destruction
    GMatrix (int iRows = 0, int iCols = 0);
    GMatrix (int iRows, int iCols, const Real* afData);
    GMatrix (int iRows, int iCols, const Real** aafEntry);
    GMatrix (const GMatrix& rqM);
    ~GMatrix ();

    // member access
    void SetSize (int iRows, int iCols);
    void GetSize (int& riRows, int& riCols) const;
    int GetRows () const;
    int GetColumns () const;
    int GetQuantity () const;
    operator const Real* () const;
    operator Real* ();
    const Real* operator[] (int iRow) const;
    Real* operator[] (int iRow);
    void SwapRows (int iRow0, int iRow1);
    Real operator() (int iRow, int iCol) const;
    Real& operator() (int iRow, int iCol);
    void SetRow (int iRow, const GVector<Real>& rqV);
    GVector<Real> GetRow (int iRow) const;
    void SetColumn (int iCol, const GVector<Real>& rqV);
    GVector<Real> GetColumn (int iCol) const;
    void SetMatrix (int iRows, int iCols, const Real* afEntry);
    void SetMatrix (int iRows, int iCols, const Real** aafMatrix);
    void GetColumnMajor (Real* afCMajor) const;

    // assignment
    GMatrix& operator= (const GMatrix& rqM);

    // comparison
    bool operator== (const GMatrix& rqM) const;
    bool operator!= (const GMatrix& rqM) const;
    bool operator<  (const GMatrix& rqM) const;
    bool operator<= (const GMatrix& rqM) const;
    bool operator>  (const GMatrix& rqM) const;
    bool operator>= (const GMatrix& rqM) const;

    // arithmetic operations
    GMatrix operator+ (const GMatrix& rqM) const;
    GMatrix operator- (const GMatrix& rqM) const;
    GMatrix operator* (const GMatrix& rqM) const;
    GMatrix operator* (Real fScalar) const;
    GMatrix operator/ (Real fScalar) const;
    GMatrix operator- () const;

    // arithmetic updates
    GMatrix& operator+= (const GMatrix& rqM);
    GMatrix& operator-= (const GMatrix& rqM);
    GMatrix& operator*= (Real fScalar);
    GMatrix& operator/= (Real fScalar);

    // matrix products
    GMatrix GetTransposed () const;  // M^T
    GMatrix GetTransposedTimes (const GMatrix& rqM) const;  // this^T * M
    GMatrix GetTimesTransposed (const GMatrix& rqM) const;  // this * M^T

    // matrix-vector operations
    GVector<Real> operator* (const GVector<Real>& rqV) const;  // M * v
    Real QForm (const GVector<Real>& rqU, const GVector<Real>& rqV) const;  // u^T*M*v

    // Inversion.  The matrix must be square.  The function returns true whenever the matrix is square and
    // invertible.
    bool GetInverse (GMatrix<Real>& rqInverse) const;

protected:
    // Support for allocation and deallocation.  The allocation call requires m_iRows, m_iCols, and m_iQuantity
    // to have already been correctly initialized.
    void Allocate (bool bSetToZero);
    void Deallocate ();

    // support for comparisons
    int CompareArrays (const GMatrix& rqM) const;

    int m_iRows, m_iCols, m_iQuantity;

    // the matrix is stored in row-major form as a 1-dimensional array
    Real* m_afData;

    // An array of pointers to the rows of the matrix.  The separation of row pointers and actual data supports
    // swapping of rows in linear algebraic algorithms such as solving linear systems of equations.
    Real** m_aafEntry;
};

// c * M
template <class Real>
GMatrix<Real> operator* (Real fScalar, const GMatrix<Real>& rqM);

// v^T * M
template <class Real>
GVector<Real> operator* (const GVector<Real>& rqV, const GMatrix<Real>& rqM);

#include "QGMatrix.inl"

typedef GMatrix<float> GMatrixf;
typedef GMatrix<double> GMatrixd;

}

#endif



