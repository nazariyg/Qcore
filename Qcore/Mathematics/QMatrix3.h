#ifndef QMATRIX3_H
#define QMATRIX3_H

// Matrix operations are applied on the left.  For example, given a matrix M and a vector V,
// matrix-times-vector is M*V.  That is, V is treated as a column vector.
//
// The (x,y,z) coordinate system is assumed to be right-handed.  Coordinate axis rotation matrices are of the
// form
//   RX =    1       0       0
//           0     cos(t) -sin(t)
//           0     sin(t)  cos(t)
// where t > 0 indicates a counterclockwise rotation in the yz-plane
//   RY =  cos(t)    0     sin(t)
//           0       1       0
//        -sin(t)    0     cos(t)
// where t > 0 indicates a counterclockwise rotation in the zx-plane
//   RZ =  cos(t) -sin(t)    0
//         sin(t)  cos(t)    0
//           0       0       1
// where t > 0 indicates a counterclockwise rotation in the xy-plane.

#include "QVector3.h"

namespace Q
{

template <class Real>
class Matrix3
{
public:
    // Construction.

    // If bZero is true, create the zero matrix.  Otherwise, create the identity matrix.
    Matrix3 (bool bZero = true);

    // copy constructor
    Matrix3 (const Matrix3& rqM);

    // input Mrc is in row r, column c.
    Matrix3 (Real fM00, Real fM01, Real fM02,
             Real fM10, Real fM11, Real fM12,
             Real fM20, Real fM21, Real fM22);

    // Create a matrix from an array of numbers.  The input array is interpreted based on the Boolean input as
    //   true:  entry[0..8]={m00,m01,m02,m10,m11,m12,m20,m21,m22} [row major]
    //   false: entry[0..8]={m00,m10,m20,m01,m11,m21,m02,m12,m22} [col major]
    Matrix3 (const Real afEntry[9], bool bRowMajor);

    // Create matrices based on vector input.  The Boolean is interpreted as
    //   true: vectors are columns of the matrix
    //   false: vectors are rows of the matrix
    Matrix3 (const Vector3<Real>& rqU, const Vector3<Real>& rqV, const Vector3<Real>& rqW, bool bColumns);
    Matrix3 (const Vector3<Real>* aqV, bool bColumns);

    // create a diagonal matrix
    Matrix3 (Real fM00, Real fM11, Real fM22);

    // Create rotation matrices (positive angle - counterclockwise).  The angle must be in radians, not
    // degrees.
    Matrix3 (const Vector3<Real>& rqAxis, Real fAngle);

    // create a tensor product U*V^T
    Matrix3 (const Vector3<Real>& rqU, const Vector3<Real>& rqV);

    // create various matrices
    Matrix3& MakeZero ();
    Matrix3& MakeIdentity ();
    Matrix3& MakeDiagonal (Real fM00, Real fM11, Real fM22);
    Matrix3& FromAxisAngle (const Vector3<Real>& rqAxis, Real fAngle);
    Matrix3& MakeTensorProduct (const Vector3<Real>& rqU, const Vector3<Real>& rqV);

    // member access
    operator Real* ();
    operator const Real* () const;
    const Real* operator[] (int iRow) const;
    Real* operator[] (int iRow);
    Real operator() (int iRow, int iCol) const;
    Real& operator() (int iRow, int iCol);
    void SetRow (int iRow, const Vector3<Real>& rqV);
    Vector3<Real> GetRow (int iRow) const;
    void SetColumn (int iCol, const Vector3<Real>& rqV);
    Vector3<Real> GetColumn (int iCol) const;
    void GetColumnMajor (Real* afCMajor) const;

    // assignment
    Matrix3& operator= (const Matrix3& rqM);

    // comparison
    bool operator== (const Matrix3& rqM) const;
    bool operator!= (const Matrix3& rqM) const;
    bool operator<  (const Matrix3& rqM) const;
    bool operator<= (const Matrix3& rqM) const;
    bool operator>  (const Matrix3& rqM) const;
    bool operator>= (const Matrix3& rqM) const;

    // arithmetic operations
    Matrix3 operator+ (const Matrix3& rqM) const;
    Matrix3 operator- (const Matrix3& rqM) const;
    Matrix3 operator* (const Matrix3& rqM) const;
    Matrix3 operator* (Real fScalar) const;
    Matrix3 operator/ (Real fScalar) const;
    Matrix3 operator- () const;

    // arithmetic updates
    Matrix3& operator+= (const Matrix3& rqM);
    Matrix3& operator-= (const Matrix3& rqM);
    Matrix3& operator*= (Real fScalar);
    Matrix3& operator/= (Real fScalar);

    // matrix times vector
    Vector3<Real> operator* (const Vector3<Real>& rqV) const;  // M * v

    // other operations
    Matrix3 GetTransposed () const;  // M^T
    Matrix3 GetTransposedTimes (const Matrix3& rqM) const;  // this^T * M
    Matrix3 GetTimesTransposed (const Matrix3& rqM) const;  // this * M^T
    Matrix3 GetInversed () const;
    Matrix3 GetAdjointed () const;
    Real GetDeterminant () const;
    Real GetQForm (const Vector3<Real>& rqU, const Vector3<Real>& rqV) const;  // u^T*M*v
    Matrix3 GetTimesDiagonal (const Vector3<Real>& rqDiag) const;  // M*D
    Matrix3 GetDiagonalTimes (const Vector3<Real>& rqDiag) const;  // D*M

    // The matrix must be a rotation for these function to be valid.  The last function uses Gram-Schmidt
    // orthonormalization applied to the columns of the rotation matrix.  The angle must be in radians, not
    // degrees.
    void ToAxisAngle (Vector3<Real>& rqAxis, Real& rfAngle) const;
    void Orthonormalize ();

    // The matrix must be symmetric.  Factor M = R * D * R^T where R = [u0|u1|u2] is a rotation matrix with
    // columns u0, u1, and u2 and D = diag(d0,d1,d2) is a diagonal matrix whose diagonal entries are d0, d1,
    // and d2.  The eigenvector u[i] corresponds to eigenvector d[i].  The eigenvalues are ordered as
    // d0 <= d1 <= d2.
    void EigenDecomposition (Matrix3& rqRot, Matrix3& rqDiag) const;

    // The matrix must be orthonormal.  The decomposition is yaw*pitch*roll where yaw is rotation about the Up
    // vector, pitch is rotation about the Right axis, and roll is rotation about the Direction axis.
    Matrix3& FromEulerAnglesXYZ (Real fYAngle, Real fPAngle, Real fRAngle);
    Matrix3& FromEulerAnglesXZY (Real fYAngle, Real fPAngle, Real fRAngle);
    Matrix3& FromEulerAnglesYXZ (Real fYAngle, Real fPAngle, Real fRAngle);
    Matrix3& FromEulerAnglesYZX (Real fYAngle, Real fPAngle, Real fRAngle);
    Matrix3& FromEulerAnglesZXY (Real fYAngle, Real fPAngle, Real fRAngle);
    Matrix3& FromEulerAnglesZYX (Real fYAngle, Real fPAngle, Real fRAngle);
    bool ToEulerAnglesXYZ (Real& rfYAngle, Real& rfPAngle, Real& rfRAngle) const;
    bool ToEulerAnglesXZY (Real& rfYAngle, Real& rfPAngle, Real& rfRAngle) const;
    bool ToEulerAnglesYXZ (Real& rfYAngle, Real& rfPAngle, Real& rfRAngle) const;
    bool ToEulerAnglesYZX (Real& rfYAngle, Real& rfPAngle, Real& rfRAngle) const;
    bool ToEulerAnglesZXY (Real& rfYAngle, Real& rfPAngle, Real& rfRAngle) const;
    bool ToEulerAnglesZYX (Real& rfYAngle, Real& rfPAngle, Real& rfRAngle) const;

    // SLERP (spherical linear interpolation) without quaternions.  Computes R(t) = R0*(Transpose(R0)*R1)^t.
    // If Q is a rotation matrix with unit-length axis U and angle A, then Q^t is a rotation matrix with
    // unit-length axis U and rotation angle t*A.
    static Matrix3 Slerp (Real fT, const Matrix3& rqR0, const Matrix3& rqR1);

    // Singular value decomposition, M = L*S*R, where L and R are orthogonal and S is a diagonal matrix whose
    // diagonal entries are nonnegative.
    void SingularValueDecomposition (Matrix3& rqL, Matrix3& rqS, Matrix3& rqR) const;
    void SingularValueComposition (const Matrix3& rqL, const Matrix3& rqS, const Matrix3& rqR);

    // factor M = Q*D*U with orthogonal Q, diagonal D, upper triangular U
    void QDUDecomposition (Matrix3& rqQ, Matrix3& rqD, Matrix3& rqU) const;

    // special matrices
    Q_ITEM static const Matrix3 ZERO;
    Q_ITEM static const Matrix3 IDENTITY;

private:
    // Support for eigendecomposition.  The Tridiagonalize function applies a Householder transformation to the
    // matrix.  If that transformation is the identity (the matrix is already tridiagonal), then the return
    // value is 'false'.  Otherwise, the transformation is a reflection and the return value is 'true'.  The
    // QLAlgorithm returns 'true' iff the QL iteration scheme converged.
    bool Tridiagonalize (Real afDiag[3], Real afSubd[2]);
    bool QLAlgorithm (Real afDiag[3], Real afSubd[2]);

    // support for singular value decomposition
    static void Bidiagonalize (Matrix3& rqA, Matrix3& rqL, Matrix3& rqR);
    static void GolubKahanStep (Matrix3& rqA, Matrix3& rqL, Matrix3& rqR);

    // for indexing into the 1D array of the matrix, iCol+N*iRow
    static int I (int iRow, int iCol);

    Real m_afEntry[9];
};

// c * M
template <class Real>
Matrix3<Real> operator* (Real fScalar, const Matrix3<Real>& rqM);

// v^T * M
template <class Real>
Vector3<Real> operator* (const Vector3<Real>& rqV, const Matrix3<Real>& rqM);

#include "QMatrix3.inl"

typedef Matrix3<float> Matrix3f;
typedef Matrix3<double> Matrix3d;

}

#endif



