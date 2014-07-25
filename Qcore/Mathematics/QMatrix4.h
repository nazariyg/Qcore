#ifndef QMATRIX4_H
#define QMATRIX4_H

// Matrix operations are applied on the left.  For example, given a matrix M and a vector V,
// matrix-times-vector is M*V.  That is, V is treated as a column vector.

#include "QVector3.h"
#include "QVector4.h"

namespace Q
{

template <class Real>
class Matrix4
{
public:
    // Construction.

    // If bZero is true, create the zero matrix.  Otherwise, create the identity matrix.
    Matrix4 (bool bZero = true);

    // copy constructor
    Matrix4 (const Matrix4& rqM);

    // input Mrc is in row r, column c.
    Matrix4 (Real fM00, Real fM01, Real fM02, Real fM03,
             Real fM10, Real fM11, Real fM12, Real fM13,
             Real fM20, Real fM21, Real fM22, Real fM23,
             Real fM30, Real fM31, Real fM32, Real fM33);

    // Create a matrix from an array of numbers.  The input array is interpreted based on the Boolean input as
    //   true:  entry[0..15]={m00,m01,m02,m03,m10,m11,m12,m13,m20,m21,m22,m23,m30,m31,m32,m33} [row major]
    //   false: entry[0..15]={m00,m10,m20,m30,m01,m11,m21,m31,m02,m12,m22,m32,m03,m13,m23,m33} [col major]
    Matrix4 (const Real afEntry[16], bool bRowMajor);

    void MakeZero ();
    void MakeIdentity ();

    // member access
    operator Real* ();
    operator const Real* () const;
    const Real* operator[] (int iRow) const;
    Real* operator[] (int iRow);
    Real operator() (int iRow, int iCol) const;
    Real& operator() (int iRow, int iCol);
    void SetRow (int iRow, const Vector4<Real>& rqV);
    Vector4<Real> GetRow (int iRow) const;
    void SetColumn (int iCol, const Vector4<Real>& rqV);
    Vector4<Real> GetColumn (int iCol) const;
    void GetColumnMajor (Real* afCMajor) const;

    // assignment
    Matrix4& operator= (const Matrix4& rqM);

    // comparison
    bool operator== (const Matrix4& rqM) const;
    bool operator!= (const Matrix4& rqM) const;
    bool operator<  (const Matrix4& rqM) const;
    bool operator<= (const Matrix4& rqM) const;
    bool operator>  (const Matrix4& rqM) const;
    bool operator>= (const Matrix4& rqM) const;

    // arithmetic operations
    Matrix4 operator+ (const Matrix4& rqM) const;
    Matrix4 operator- (const Matrix4& rqM) const;
    Matrix4 operator* (const Matrix4& rqM) const;
    Matrix4 operator* (Real fScalar) const;
    Matrix4 operator/ (Real fScalar) const;
    Matrix4 operator- () const;

    // arithmetic updates
    Matrix4& operator+= (const Matrix4& rqM);
    Matrix4& operator-= (const Matrix4& rqM);
    Matrix4& operator*= (Real fScalar);
    Matrix4& operator/= (Real fScalar);

    // matrix times vector
    Vector4<Real> operator* (const Vector4<Real>& rqV) const;  // M * v

    // other operations
    Matrix4 GetTransposed () const;  // M^T
    Matrix4 GetTransposedTimes (const Matrix4& rqM) const;  // this^T * M
    Matrix4 GetTimesTransposed (const Matrix4& rqM) const;  // this * M^T
    Matrix4 GetInversed () const;
    Matrix4 GetAdjointed () const;
    Real GetDeterminant () const;
    Real GetQForm (const Vector4<Real>& rqU, const Vector4<Real>& rqV) const;  // u^T*M*v

    // projection matrices onto a specified plane
    void MakeObliqueProjection (const Vector3<Real>& rqNormal, const Vector3<Real>& rqPoint,
        const Vector3<Real>& rqDirection);
    void MakePerspectiveProjection (const Vector3<Real>& rqNormal, const Vector3<Real>& rqPoint,
        const Vector3<Real>& rqEye);

    // reflection matrix through a specified plane
    void MakeReflection (const Vector3<Real>& rqNormal, const Vector3<Real>& rqPoint);

    // special matrices
    Q_ITEM static const Matrix4 ZERO;
    Q_ITEM static const Matrix4 IDENTITY;

private:
    // for indexing into the 1D array of the matrix, iCol+N*iRow
    static int I (int iRow, int iCol);

    Real m_afEntry[16];
};

// c * M
template <class Real>
Matrix4<Real> operator* (Real fScalar, const Matrix4<Real>& rqM);

// v^T * M
template <class Real>
Vector4<Real> operator* (const Vector4<Real>& rqV, const Matrix4<Real>& rqM);

#include "QMatrix4.inl"

typedef Matrix4<float> Matrix4f;
typedef Matrix4<double> Matrix4d;

}

#endif



