//------------------------------------------------------------------------------------------------------------------
template <class Real>
Matrix2<Real>::Matrix2 (bool bZero)
{
    if ( bZero )
        MakeZero();
    else
        MakeIdentity();
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Matrix2<Real>::Matrix2 (const Matrix2& rqM)
{
    MemCpy(m_afEntry,rqM.m_afEntry,4*sizeof(Real));
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Matrix2<Real>::Matrix2 (Real fM00, Real fM01, Real fM10, Real fM11)
{
    m_afEntry[0] = fM00;
    m_afEntry[1] = fM01;
    m_afEntry[2] = fM10;
    m_afEntry[3] = fM11;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Matrix2<Real>::Matrix2 (const Real afEntry[4], bool bRowMajor)
{
    if ( bRowMajor )
    {
        MemCpy(m_afEntry,afEntry,4*sizeof(Real));
    }
    else
    {
        m_afEntry[0] = afEntry[0];
        m_afEntry[1] = afEntry[2];
        m_afEntry[2] = afEntry[1];
        m_afEntry[3] = afEntry[3];
    }
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Matrix2<Real>::Matrix2 (const Vector2<Real>& rqU, const Vector2<Real>& rqV, bool bColumns)
{
    if ( bColumns )
    {
        m_afEntry[0] = rqU[0];
        m_afEntry[1] = rqV[0];
        m_afEntry[2] = rqU[1];
        m_afEntry[3] = rqV[1];
    }
    else
    {
        m_afEntry[0] = rqU[0];
        m_afEntry[1] = rqU[1];
        m_afEntry[2] = rqV[0];
        m_afEntry[3] = rqV[1];
    }
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Matrix2<Real>::Matrix2 (const Vector2<Real>* aqV, bool bColumns)
{
    if ( bColumns )
    {
        m_afEntry[0] = aqV[0][0];
        m_afEntry[1] = aqV[1][0];
        m_afEntry[2] = aqV[0][1];
        m_afEntry[3] = aqV[1][1];
    }
    else
    {
        m_afEntry[0] = aqV[0][0];
        m_afEntry[1] = aqV[0][1];
        m_afEntry[2] = aqV[1][0];
        m_afEntry[3] = aqV[1][1];
    }
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Matrix2<Real>::Matrix2 (Real fM00, Real fM11)
{
    MakeDiagonal(fM00,fM11);
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Matrix2<Real>::Matrix2 (Real fAngle)
{
    FromAngle(fAngle);
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Matrix2<Real>::Matrix2 (const Vector2<Real>& rqU, const Vector2<Real>& rqV)
{
    MakeTensorProduct(rqU,rqV);
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Matrix2<Real>::operator Real* ()
{
    return m_afEntry;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Matrix2<Real>::operator const Real* () const
{
    return m_afEntry;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
const Real* Matrix2<Real>::operator[] (int iRow) const
{
    return &m_afEntry[2*iRow];
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Real* Matrix2<Real>::operator[] (int iRow)
{
    return &m_afEntry[2*iRow];
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Real Matrix2<Real>::operator() (int iRow, int iCol) const
{
    return m_afEntry[I(iRow,iCol)];
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Real& Matrix2<Real>::operator() (int iRow, int iCol)
{
    return m_afEntry[I(iRow,iCol)];
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
int Matrix2<Real>::I (int iRow, int iCol)
{
    assert( 0 <= iRow && iRow < 2 && 0 <= iCol && iCol < 2 );
    return iCol + 2*iRow;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
void Matrix2<Real>::MakeZero ()
{
    MemSet(m_afEntry,0,4*sizeof(Real));
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
void Matrix2<Real>::MakeIdentity ()
{
    m_afEntry[0] = (Real)1.0;
    m_afEntry[1] = (Real)0.0;
    m_afEntry[2] = (Real)0.0;
    m_afEntry[3] = (Real)1.0;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
void Matrix2<Real>::MakeDiagonal (Real fM00, Real fM11)
{
    m_afEntry[0] = fM00;
    m_afEntry[1] = (Real)0.0;
    m_afEntry[2] = (Real)0.0;
    m_afEntry[3] = fM11;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
void Matrix2<Real>::FromAngle (Real fAngle)
{
    m_afEntry[0] = Math<Real>::Cos(fAngle);
    m_afEntry[2] = Math<Real>::Sin(fAngle);
    m_afEntry[1] = -m_afEntry[2];
    m_afEntry[3] =  m_afEntry[0];
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
void Matrix2<Real>::MakeTensorProduct (const Vector2<Real>& rqU, const Vector2<Real>& rqV)
{
    m_afEntry[0] = rqU[0]*rqV[0];
    m_afEntry[1] = rqU[0]*rqV[1];
    m_afEntry[2] = rqU[1]*rqV[0];
    m_afEntry[3] = rqU[1]*rqV[1];
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
void Matrix2<Real>::SetRow (int iRow, const Vector2<Real>& rqV)
{
    assert( 0 <= iRow && iRow < 2 );
    for (int iCol = 0, i = 2*iRow; iCol < 2; iCol++, i++)
        m_afEntry[i] = rqV[iCol];
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Vector2<Real> Matrix2<Real>::GetRow (int iRow) const
{
    assert( 0 <= iRow && iRow < 2 );
    Vector2<Real> qV;
    for (int iCol = 0, i = 2*iRow; iCol < 2; iCol++, i++)
        qV[iCol] = m_afEntry[i];
    return qV;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
void Matrix2<Real>::SetColumn (int iCol, const Vector2<Real>& rqV)
{
    assert( 0 <= iCol && iCol < 2 );
    for (int iRow = 0, i = iCol; iRow < 2; iRow++, i += 2)
        m_afEntry[i] = rqV[iRow];
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Vector2<Real> Matrix2<Real>::GetColumn (int iCol) const
{
    assert( 0 <= iCol && iCol < 2 );
    Vector2<Real> qV;
    for (int iRow = 0, i = iCol; iRow < 2; iRow++, i += 2)
        qV[iRow] = m_afEntry[i];
    return qV;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
void Matrix2<Real>::GetColumnMajor (Real* afCMajor) const
{
    for (int iRow = 0, i = 0; iRow < 2; iRow++)
    {
        for (int iCol = 0; iCol < 2; iCol++)
            afCMajor[i++] = m_afEntry[I(iCol,iRow)];
    }
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Matrix2<Real>& Matrix2<Real>::operator= (const Matrix2& rqM)
{
    MemCpy(m_afEntry,rqM.m_afEntry,4*sizeof(Real));
    return *this;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
bool Matrix2<Real>::operator== (const Matrix2& rqM) const
{
    return ( MemCmp(m_afEntry,rqM.m_afEntry,4*sizeof(Real)) == 0 );
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
bool Matrix2<Real>::operator!= (const Matrix2& rqM) const
{
    return ( MemCmp(m_afEntry,rqM.m_afEntry,4*sizeof(Real)) != 0 );
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
bool Matrix2<Real>::operator< (const Matrix2& rqM) const
{
    return ( MemCmp(m_afEntry,rqM.m_afEntry,4*sizeof(Real)) < 0 );
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
bool Matrix2<Real>::operator<= (const Matrix2& rqM) const
{
    return ( MemCmp(m_afEntry,rqM.m_afEntry,4*sizeof(Real)) <= 0 );
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
bool Matrix2<Real>::operator> (const Matrix2& rqM) const
{
    return ( MemCmp(m_afEntry,rqM.m_afEntry,4*sizeof(Real)) > 0 );
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
bool Matrix2<Real>::operator>= (const Matrix2& rqM) const
{
    return ( MemCmp(m_afEntry,rqM.m_afEntry,4*sizeof(Real)) >= 0 );
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Matrix2<Real> Matrix2<Real>::operator+ (const Matrix2& rqM) const
{
    Matrix2 qSum;
    for (int i = 0; i < 4; i++)
        qSum.m_afEntry[i] = m_afEntry[i] + rqM.m_afEntry[i];
    return qSum;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Matrix2<Real> Matrix2<Real>::operator- (const Matrix2& rqM) const
{
    Matrix2 qDiff;
    for (int i = 0; i < 4; i++)
        qDiff.m_afEntry[i] = m_afEntry[i] - rqM.m_afEntry[i];
    return qDiff;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Matrix2<Real> Matrix2<Real>::operator* (const Matrix2& rqM) const
{
    Matrix2 qProd;
    for (int iRow = 0; iRow < 2; iRow++)
    {
        for (int iCol = 0; iCol < 2; iCol++)
        {
            int i = I(iRow,iCol);
            qProd.m_afEntry[i] = (Real)0.0;
            for (int iMid = 0; iMid < 2; iMid++)
                qProd.m_afEntry[i] += m_afEntry[I(iRow,iMid)]*rqM.m_afEntry[I(iMid,iCol)];
        }
    }
    return qProd;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Matrix2<Real> Matrix2<Real>::operator* (Real fScalar) const
{
    Matrix2 qProd;
    for (int i = 0; i < 4; i++)
        qProd.m_afEntry[i] = fScalar*m_afEntry[i];
    return qProd;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Matrix2<Real> Matrix2<Real>::operator/ (Real fScalar) const
{
    Matrix2 qQuot;
    int i;

    if ( fScalar != (Real)0.0 )
    {
        Real fInvScalar = ((Real)1.0)/fScalar;
        for (i = 0; i < 4; i++)
            qQuot.m_afEntry[i] = fInvScalar*m_afEntry[i];
    }
    else
    {
        for (i = 0; i < 4; i++)
            qQuot.m_afEntry[i] = Math<Real>::MAX_REAL;
    }

    return qQuot;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Matrix2<Real> Matrix2<Real>::operator- () const
{
    Matrix2 qNeg;
    for (int i = 0; i < 4; i++)
        qNeg.m_afEntry[i] = -m_afEntry[i];
    return qNeg;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Matrix2<Real>& Matrix2<Real>::operator+= (const Matrix2& rqM)
{
    for (int i = 0; i < 4; i++)
        m_afEntry[i] += rqM.m_afEntry[i];
    return *this;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Matrix2<Real>& Matrix2<Real>::operator-= (const Matrix2& rqM)
{
    for (int i = 0; i < 4; i++)
        m_afEntry[i] -= rqM.m_afEntry[i];
    return *this;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Matrix2<Real>& Matrix2<Real>::operator*= (Real fScalar)
{
    for (int i = 0; i < 4; i++)
        m_afEntry[i] *= fScalar;
    return *this;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Matrix2<Real>& Matrix2<Real>::operator/= (Real fScalar)
{
    int i;

    if ( fScalar != (Real)0.0 )
    {
        Real fInvScalar = ((Real)1.0)/fScalar;
        for (i = 0; i < 4; i++)
            m_afEntry[i] *= fInvScalar;
    }
    else
    {
        for (i = 0; i < 4; i++)
            m_afEntry[i] = Math<Real>::MAX_REAL;
    }

    return *this;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Vector2<Real> Matrix2<Real>::operator* (const Vector2<Real>& rqV) const
{
    Vector2<Real> qProd;
    for (int iRow = 0; iRow < 2; iRow++)
    {
        qProd[iRow] = (Real)0.0;
        for (int iCol = 0; iCol < 2; iCol++)
            qProd[iRow] += m_afEntry[I(iRow,iCol)]*rqV[iCol];
    }
    return qProd;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Matrix2<Real> Matrix2<Real>::GetTransposed () const
{
    Matrix2 qTranspose;
    for (int iRow = 0; iRow < 2; iRow++)
    {
        for (int iCol = 0; iCol < 2; iCol++)
            qTranspose.m_afEntry[I(iRow,iCol)] = m_afEntry[I(iCol,iRow)];
    }
    return qTranspose;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Matrix2<Real> Matrix2<Real>::GetTransposedTimes (const Matrix2& rqM) const
{
    // P = A^T*B, P[r][c] = sum_m A[m][r]*B[m][c]
    Matrix2 qProd;
    for (int iRow = 0; iRow < 2; iRow++)
    {
        for (int iCol = 0; iCol < 2; iCol++)
        {
            int i = I(iRow,iCol);
            qProd.m_afEntry[i] = (Real)0.0;
            for (int iMid = 0; iMid < 2; iMid++)
                qProd.m_afEntry[i] += m_afEntry[I(iMid,iRow)]*rqM.m_afEntry[I(iMid,iCol)];
        }
    }
    return qProd;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Matrix2<Real> Matrix2<Real>::GetTimesTransposed (const Matrix2& rqM) const
{
    // P = A*B^T, P[r][c] = sum_m A[r][m]*B[c][m]
    Matrix2 qProd;
    for (int iRow = 0; iRow < 2; iRow++)
    {
        for (int iCol = 0; iCol < 2; iCol++)
        {
            int i = I(iRow,iCol);
            qProd.m_afEntry[i] = (Real)0.0;
            for (int iMid = 0; iMid < 2; iMid++)
                qProd.m_afEntry[i] += m_afEntry[I(iRow,iMid)]*rqM.m_afEntry[I(iCol,iMid)];
        }
    }
    return qProd;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Matrix2<Real> Matrix2<Real>::GetInversed () const
{
    Matrix2 qInverse;

    Real fDet = m_afEntry[0]*m_afEntry[3] - m_afEntry[1]*m_afEntry[2];
    if ( Math<Real>::FAbs(fDet) > Math<Real>::ZERO_TOLERANCE )
    {
        Real fInvDet = ((Real)1.0)/fDet;
        qInverse[0][0] =  m_afEntry[3]*fInvDet;
        qInverse[0][1] = -m_afEntry[1]*fInvDet;
        qInverse[1][0] = -m_afEntry[2]*fInvDet;
        qInverse[1][1] =  m_afEntry[0]*fInvDet;
    }
    else
    {
        qInverse.MakeZero();
    }

    return qInverse;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Matrix2<Real> Matrix2<Real>::GetAdjointed () const
{
    return Matrix2(
         m_afEntry[3],-m_afEntry[1],
        -m_afEntry[2], m_afEntry[0]);
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Real Matrix2<Real>::GetDeterminant () const
{
    return m_afEntry[0]*m_afEntry[3] - m_afEntry[1]*m_afEntry[2];
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Real Matrix2<Real>::GetQForm (const Vector2<Real>& rqU, const Vector2<Real>& rqV) const
{
    return rqU.Dot((*this)*rqV);
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
void Matrix2<Real>::ToAngle (Real& rfAngle) const
{
    // assert:  matrix is a rotation
    rfAngle = Math<Real>::ATan2(m_afEntry[2],m_afEntry[0]);
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
void Matrix2<Real>::Orthonormalize ()
{
    // Algorithm uses Gram-Schmidt orthogonalization.  If 'this' matrix is M = [m0|m1], then orthonormal output
    // matrix is Q = [q0|q1],
    //
    //   q0 = m0/|m0|
    //   q1 = (m1-(q0*m1)q0)/|m1-(q0*m1)q0|
    //
    // where |V| indicates length of vector V and A*B indicates dot product of vectors A and B.

    // compute q0
    Real fInvLength = Math<Real>::InvSqrt(m_afEntry[0]*m_afEntry[0]+m_afEntry[2]*m_afEntry[2]);
    m_afEntry[0] *= fInvLength;
    m_afEntry[2] *= fInvLength;

    // compute q1
    Real fDot0 = m_afEntry[0]*m_afEntry[1] + m_afEntry[2]*m_afEntry[3];
    m_afEntry[1] -= fDot0*m_afEntry[0];
    m_afEntry[3] -= fDot0*m_afEntry[2];

    fInvLength = Math<Real>::InvSqrt(m_afEntry[1]*m_afEntry[1]+m_afEntry[3]*m_afEntry[3]);
    m_afEntry[1] *= fInvLength;
    m_afEntry[3] *= fInvLength;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
void Matrix2<Real>::EigenDecomposition (Matrix2& rqRot, Matrix2& rqDiag) const
{
    Real fTrace = m_afEntry[0] + m_afEntry[3];
    Real fDiff = m_afEntry[0] - m_afEntry[3];
    Real fDiscr = Math<Real>::Sqrt(fDiff*fDiff+((Real)4.0)*m_afEntry[1]*m_afEntry[1]);
    Real fEVal0 = ((Real)0.5)*(fTrace - fDiscr);
    Real fEVal1 = ((Real)0.5)*(fTrace + fDiscr);
    rqDiag.MakeDiagonal(fEVal0,fEVal1);

    Real fCos, fSin;
    if ( fDiff >= (Real)0.0 )
    {
        fCos = m_afEntry[1];
        fSin = fEVal0 - m_afEntry[0];
    }
    else
    {
        fCos = fEVal0 - m_afEntry[3];
        fSin = m_afEntry[1];
    }
    Real fTmp = Math<Real>::InvSqrt(fCos*fCos+fSin*fSin);
    fCos *= fTmp;
    fSin *= fTmp;

    rqRot.m_afEntry[0] = fCos;
    rqRot.m_afEntry[1] = -fSin;
    rqRot.m_afEntry[2] = fSin;
    rqRot.m_afEntry[3] = fCos;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Matrix2<Real> operator* (Real fScalar, const Matrix2<Real>& rqM)
{
    return rqM*fScalar;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Vector2<Real> operator* (const Vector2<Real>& rqV, const Matrix2<Real>& rqM)
{
    return Vector2<Real>(
        rqV[0]*rqM[0][0]+rqV[1]*rqM[1][0],
        rqV[0]*rqM[0][1]+rqV[1]*rqM[1][1]);
}
//------------------------------------------------------------------------------------------------------------------



