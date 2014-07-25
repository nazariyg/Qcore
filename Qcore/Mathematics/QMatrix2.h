#ifndef QMATRIX2_H
#define QMATRIX2_H

// Matrix operations are applied on the left.  For example, given a matrix M and a vector V,
// matrix-times-vector is M*V.  That is, V is treated as a column vector.
//
// Rotation matrices are of the form
//   R = cos(t) -sin(t)
//       sin(t)  cos(t)
// where t > 0 indicates a counterclockwise rotation in the xy-plane.

#include "QVector2.h"

namespace Q
{

template <class Real>
class Matrix2
{
public:
    // Construction.

    // If bZero is true, create the zero matrix.  Otherwise, create the identity matrix.
    Matrix2 (bool bZero = true);

    // copy constructor
    Matrix2 (const Matrix2& rqM);

    // input Mrc is in row r, column c.
    Matrix2 (Real fM00, Real fM01, Real fM10, Real fM11);

    // Create a matrix from an array of numbers.  The input array is interpreted based on the Boolean input as
    //   true:  entry[0..3] = {m00,m01,m10,m11}  [row major]
    //   false: entry[0..3] = {m00,m10,m01,m11}  [column major]
    Matrix2 (const Real afEntry[4], bool bRowMajor);

    // Create matrices based on vector input.  The Boolean is interpreted as
    //   true: vectors are columns of the matrix
    //   false: vectors are rows of the matrix
    Matrix2 (const Vector2<Real>& rqU, const Vector2<Real>& rqV, bool bColumns);
    Matrix2 (const Vector2<Real>* aqV, bool bColumns);

    // create a diagonal matrix
    Matrix2 (Real fM00, Real fM11);

    // create a rotation matrix (positive angle - counterclockwise)
    Matrix2 (Real fAngle);

    // create a tensor product U*V^T
    Matrix2 (const Vector2<Real>& rqU, const Vector2<Real>& rqV);

    // create various matrices
    void MakeZero ();
    void MakeIdentity ();
    void MakeDiagonal (Real fM00, Real fM11);
    void FromAngle (Real fAngle);
    void MakeTensorProduct (const Vector2<Real>& rqU, const Vector2<Real>& rqV);

    // member access
    operator Real* ();
    operator const Real* () const;
    const Real* operator[] (int iRow) const;
    Real* operator[] (int iRow);
    Real operator() (int iRow, int iCol) const;
    Real& operator() (int iRow, int iCol);
    void SetRow (int iRow, const Vector2<Real>& rqV);
    Vector2<Real> GetRow (int iRow) const;
    void SetColumn (int iCol, const Vector2<Real>& rqV);
    Vector2<Real> GetColumn (int iCol) const;
    void GetColumnMajor (Real* afCMajor) const;

    // assignment
    Matrix2& operator= (const Matrix2& rqM);

    // comparison
    bool operator== (const Matrix2& rqM) const;
    bool operator!= (const Matrix2& rqM) const;
    bool operator<  (const Matrix2& rqM) const;
    bool operator<= (const Matrix2& rqM) const;
    bool operator>  (const Matrix2& rqM) const;
    bool operator>= (const Matrix2& rqM) const;

    // arithmetic operations
    Matrix2 operator+ (const Matrix2& rqM) const;
    Matrix2 operator- (const Matrix2& rqM) const;
    Matrix2 operator* (const Matrix2& rqM) const;
    Matrix2 operator* (Real fScalar) const;
    Matrix2 operator/ (Real fScalar) const;
    Matrix2 operator- () const;

    // arithmetic updates
    Matrix2& operator+= (const Matrix2& rqM);
    Matrix2& operator-= (const Matrix2& rqM);
    Matrix2& operator*= (Real fScalar);
    Matrix2& operator/= (Real fScalar);

    // matrix times vector
    Vector2<Real> operator* (const Vector2<Real>& rqV) const;  // M * v

    // other operations
    Matrix2 GetTransposed () const;  // M^T
    Matrix2 GetTransposedTimes (const Matrix2& rqM) const;  // this^T * M
    Matrix2 GetTimesTransposed (const Matrix2& rqM) const;  // this * M^T
    Matrix2 GetInversed () const;
    Matrix2 GetAdjointed () const;
    Real GetDeterminant () const;
    Real GetQForm (const Vector2<Real>& rqU, const Vector2<Real>& rqV) const;  // u^T*M*v

    // The matrix must be a rotation for these function to be valid.  The last function uses Gram-Schmidt
    // orthonormalization applied to the columns of the rotation matrix.  The angle must be in radians, not
    // degrees.
    void ToAngle (Real& rfAngle) const;
    void Orthonormalize ();

    // The matrix must be symmetric.  Factor M = R * D * R^T where R = [u0|u1] is a rotation matrix with
    // columns u0 and u1 and D = diag(d0,d1) is a diagonal matrix whose diagonal entries are d0 and d1.  The
    // eigenvector u[i] corresponds to eigenvector d[i].  The eigenvalues are ordered as d0 <= d1.
    void EigenDecomposition (Matrix2& rqRot, Matrix2& rqDiag) const;

    Q_ITEM static const Matrix2 ZERO;
    Q_ITEM static const Matrix2 IDENTITY;

private:
    // for indexing into the 1D array of the matrix, iCol+N*iRow
    static int I (int iRow, int iCol);

    // matrix stored in row-major order
    Real m_afEntry[4];
};

// c * M
template <class Real>
Matrix2<Real> operator* (Real fScalar, const Matrix2<Real>& rqM);

// v^T * M
template <class Real>
Vector2<Real> operator* (const Vector2<Real>& rqV, const Matrix2<Real>& rqM);

#include "QMatrix2.inl"

typedef Matrix2<float> Matrix2f;
typedef Matrix2<double> Matrix2d;

}

#endif



