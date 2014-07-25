//------------------------------------------------------------------------------------------------------------------
template <class Real>
Matrix4<Real>::Matrix4 (bool bZero)
{
    if ( bZero )
        MakeZero();
    else
        MakeIdentity();
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Matrix4<Real>::Matrix4 (const Matrix4& rqM)
{
    MemCpy(m_afEntry,rqM.m_afEntry,16*sizeof(Real));
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Matrix4<Real>::Matrix4 (Real fM00, Real fM01, Real fM02, Real fM03,
    Real fM10, Real fM11, Real fM12, Real fM13, Real fM20, Real fM21,
    Real fM22, Real fM23, Real fM30, Real fM31, Real fM32, Real fM33)
{
    m_afEntry[ 0] = fM00;
    m_afEntry[ 1] = fM01;
    m_afEntry[ 2] = fM02;
    m_afEntry[ 3] = fM03;
    m_afEntry[ 4] = fM10;
    m_afEntry[ 5] = fM11;
    m_afEntry[ 6] = fM12;
    m_afEntry[ 7] = fM13;
    m_afEntry[ 8] = fM20;
    m_afEntry[ 9] = fM21;
    m_afEntry[10] = fM22;
    m_afEntry[11] = fM23;
    m_afEntry[12] = fM30;
    m_afEntry[13] = fM31;
    m_afEntry[14] = fM32;
    m_afEntry[15] = fM33;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Matrix4<Real>::Matrix4 (const Real afEntry[16], bool bRowMajor)
{
    if ( bRowMajor )
    {
        MemCpy(m_afEntry,afEntry,16*sizeof(Real));
    }
    else
    {
        m_afEntry[ 0] = afEntry[ 0];
        m_afEntry[ 1] = afEntry[ 4];
        m_afEntry[ 2] = afEntry[ 8];
        m_afEntry[ 3] = afEntry[12];
        m_afEntry[ 4] = afEntry[ 1];
        m_afEntry[ 5] = afEntry[ 5];
        m_afEntry[ 6] = afEntry[ 9];
        m_afEntry[ 7] = afEntry[13];
        m_afEntry[ 8] = afEntry[ 2];
        m_afEntry[ 9] = afEntry[ 6];
        m_afEntry[10] = afEntry[10];
        m_afEntry[11] = afEntry[14];
        m_afEntry[12] = afEntry[ 3];
        m_afEntry[13] = afEntry[ 7];
        m_afEntry[14] = afEntry[11];
        m_afEntry[15] = afEntry[15];
    }
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Matrix4<Real>::operator Real* ()
{
    return m_afEntry;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Matrix4<Real>::operator const Real* () const
{
    return m_afEntry;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
const Real* Matrix4<Real>::operator[] (int iRow) const
{
    return &m_afEntry[4*iRow];
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Real* Matrix4<Real>::operator[] (int iRow)
{
    return &m_afEntry[4*iRow];
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Real Matrix4<Real>::operator() (int iRow, int iCol) const
{
    return m_afEntry[I(iRow,iCol)];
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Real& Matrix4<Real>::operator() (int iRow, int iCol)
{
    return m_afEntry[I(iRow,iCol)];
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
int Matrix4<Real>::I (int iRow, int iCol)
{
    assert( 0 <= iRow && iRow < 4 && 0 <= iCol && iCol < 4 );
    return iCol + 4*iRow;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
void Matrix4<Real>::MakeZero ()
{
    MemSet(m_afEntry,0,16*sizeof(Real));
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
void Matrix4<Real>::MakeIdentity ()
{
    m_afEntry[ 0] = (Real)1.0;
    m_afEntry[ 1] = (Real)0.0;
    m_afEntry[ 2] = (Real)0.0;
    m_afEntry[ 3] = (Real)0.0;
    m_afEntry[ 4] = (Real)0.0;
    m_afEntry[ 5] = (Real)1.0;
    m_afEntry[ 6] = (Real)0.0;
    m_afEntry[ 7] = (Real)0.0;
    m_afEntry[ 8] = (Real)0.0;
    m_afEntry[ 9] = (Real)0.0;
    m_afEntry[10] = (Real)1.0;
    m_afEntry[11] = (Real)0.0;
    m_afEntry[12] = (Real)0.0;
    m_afEntry[13] = (Real)0.0;
    m_afEntry[14] = (Real)0.0;
    m_afEntry[15] = (Real)1.0;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
void Matrix4<Real>::SetRow (int iRow, const Vector4<Real>& rqV)
{
    assert( 0 <= iRow && iRow < 4 );
    for (int iCol = 0, i = 4*iRow; iCol < 4; iCol++, i++)
        m_afEntry[i] = rqV[iCol];
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Vector4<Real> Matrix4<Real>::GetRow (int iRow) const
{
    assert( 0 <= iRow && iRow < 4 );
    Vector4<Real> qV;
    for (int iCol = 0, i = 4*iRow; iCol < 4; iCol++, i++)
        qV[iCol] = m_afEntry[i];
    return qV;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
void Matrix4<Real>::SetColumn (int iCol, const Vector4<Real>& rqV)
{
    assert( 0 <= iCol && iCol < 4 );
    for (int iRow = 0, i = iCol; iRow < 4; iRow++, i += 4)
        m_afEntry[i] = rqV[iRow];
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Vector4<Real> Matrix4<Real>::GetColumn (int iCol) const
{
    assert( 0 <= iCol && iCol < 4 );
    Vector4<Real> qV;
    for (int iRow = 0, i = iCol; iRow < 4; iRow++, i += 4)
        qV[iRow] = m_afEntry[i];
    return qV;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
void Matrix4<Real>::GetColumnMajor (Real* afCMajor) const
{
    for (int iRow = 0, i = 0; iRow < 4; iRow++)
    {
        for (int iCol = 0; iCol < 4; iCol++)
            afCMajor[i++] = m_afEntry[I(iCol,iRow)];
    }
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Matrix4<Real>& Matrix4<Real>::operator= (const Matrix4& rqM)
{
    MemCpy(m_afEntry,rqM.m_afEntry,16*sizeof(Real));
    return *this;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
bool Matrix4<Real>::operator== (const Matrix4& rqM) const
{
    return ( MemCmp(m_afEntry,rqM.m_afEntry,16*sizeof(Real)) == 0 );
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
bool Matrix4<Real>::operator!= (const Matrix4& rqM) const
{
    return ( MemCmp(m_afEntry,rqM.m_afEntry,16*sizeof(Real)) != 0 );
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
bool Matrix4<Real>::operator< (const Matrix4& rqM) const
{
    return ( MemCmp(m_afEntry,rqM.m_afEntry,16*sizeof(Real)) < 0 );
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
bool Matrix4<Real>::operator<= (const Matrix4& rqM) const
{
    return ( MemCmp(m_afEntry,rqM.m_afEntry,16*sizeof(Real)) <= 0 );
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
bool Matrix4<Real>::operator> (const Matrix4& rqM) const
{
    return ( MemCmp(m_afEntry,rqM.m_afEntry,16*sizeof(Real)) > 0 );
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
bool Matrix4<Real>::operator>= (const Matrix4& rqM) const
{
    return ( MemCmp(m_afEntry,rqM.m_afEntry,16*sizeof(Real)) >= 0 );
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Matrix4<Real> Matrix4<Real>::operator+ (const Matrix4& rqM) const
{
    Matrix4 qSum;
    for (int i = 0; i < 16; i++)
        qSum.m_afEntry[i] = m_afEntry[i] + rqM.m_afEntry[i];
    return qSum;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Matrix4<Real> Matrix4<Real>::operator- (const Matrix4& rqM) const
{
    Matrix4 qDiff;
    for (int i = 0; i < 16; i++)
        qDiff.m_afEntry[i] = m_afEntry[i] - rqM.m_afEntry[i];
    return qDiff;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Matrix4<Real> Matrix4<Real>::operator* (const Matrix4& rqM) const
{
    Matrix4 qProd;
    for (int iRow = 0; iRow < 4; iRow++)
    {
        for (int iCol = 0; iCol < 4; iCol++)
        {
            int i = I(iRow,iCol);
            qProd.m_afEntry[i] = (Real)0.0;
            for (int iMid = 0; iMid < 4; iMid++)
                qProd.m_afEntry[i] += m_afEntry[I(iRow,iMid)]*rqM.m_afEntry[I(iMid,iCol)];
        }
    }
    return qProd;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Matrix4<Real> Matrix4<Real>::operator* (Real fScalar) const
{
    Matrix4 qProd;
    for (int i = 0; i < 16; i++)
        qProd.m_afEntry[i] = fScalar*m_afEntry[i];
    return qProd;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Matrix4<Real> Matrix4<Real>::operator/ (Real fScalar) const
{
    Matrix4 qQuot;
    int i;

    if ( fScalar != (Real)0.0 )
    {
        Real fInvScalar = ((Real)1.0)/fScalar;
        for (i = 0; i < 16; i++)
            qQuot.m_afEntry[i] = fInvScalar*m_afEntry[i];
    }
    else
    {
        for (i = 0; i < 16; i++)
            qQuot.m_afEntry[i] = Math<Real>::MAX_REAL;
    }

    return qQuot;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Matrix4<Real> Matrix4<Real>::operator- () const
{
    Matrix4 qNeg;
    for (int i = 0; i < 16; i++)
        qNeg.m_afEntry[i] = -m_afEntry[i];
    return qNeg;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Matrix4<Real>& Matrix4<Real>::operator+= (const Matrix4& rqM)
{
    for (int i = 0; i < 16; i++)
        m_afEntry[i] += rqM.m_afEntry[i];
    return *this;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Matrix4<Real>& Matrix4<Real>::operator-= (const Matrix4& rqM)
{
    for (int i = 0; i < 16; i++)
        m_afEntry[i] -= rqM.m_afEntry[i];
    return *this;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Matrix4<Real>& Matrix4<Real>::operator*= (Real fScalar)
{
    for (int i = 0; i < 16; i++)
        m_afEntry[i] *= fScalar;
    return *this;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Matrix4<Real>& Matrix4<Real>::operator/= (Real fScalar)
{
    int i;

    if ( fScalar != (Real)0.0 )
    {
        Real fInvScalar = ((Real)1.0)/fScalar;
        for (i = 0; i < 16; i++)
            m_afEntry[i] *= fInvScalar;
    }
    else
    {
        for (i = 0; i < 16; i++)
            m_afEntry[i] = Math<Real>::MAX_REAL;
    }

    return *this;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Vector4<Real> Matrix4<Real>::operator* (const Vector4<Real>& rqV) const
{
    Vector4<Real> qProd;
    for (int iRow = 0; iRow < 4; iRow++)
    {
        qProd[iRow] = (Real)0.0;
        for (int iCol = 0; iCol < 4; iCol++)
            qProd[iRow] += m_afEntry[I(iRow,iCol)]*rqV[iCol];
    }
    return qProd;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Matrix4<Real> Matrix4<Real>::GetTransposed () const
{
    Matrix4 qTranspose;
    for (int iRow = 0; iRow < 4; iRow++)
    {
        for (int iCol = 0; iCol < 4; iCol++)
            qTranspose.m_afEntry[I(iRow,iCol)] = m_afEntry[I(iCol,iRow)];
    }
    return qTranspose;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Matrix4<Real> Matrix4<Real>::GetTransposedTimes (const Matrix4& rqM) const
{
    // P = A^T*B, P[r][c] = sum_m A[m][r]*B[m][c]
    Matrix4 qProd;
    for (int iRow = 0; iRow < 4; iRow++)
    {
        for (int iCol = 0; iCol < 4; iCol++)
        {
            int i = I(iRow,iCol);
            qProd.m_afEntry[i] = (Real)0.0;
            for (int iMid = 0; iMid < 4; iMid++)
                qProd.m_afEntry[i] += m_afEntry[I(iMid,iRow)]*rqM.m_afEntry[I(iMid,iCol)];
        }
    }
    return qProd;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Matrix4<Real> Matrix4<Real>::GetTimesTransposed (const Matrix4& rqM) const
{
    // P = A*B^T, P[r][c] = sum_m A[r][m]*B[c][m]
    Matrix4 qProd;
    for (int iRow = 0; iRow < 4; iRow++)
    {
        for (int iCol = 0; iCol < 4; iCol++)
        {
            int i = I(iRow,iCol);
            qProd.m_afEntry[i] = (Real)0.0;
            for (int iMid = 0; iMid < 4; iMid++)
                qProd.m_afEntry[i] += m_afEntry[I(iRow,iMid)]*rqM.m_afEntry[I(iCol,iMid)];
        }
    }
    return qProd;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Matrix4<Real> Matrix4<Real>::GetInversed () const
{
    Real fA0 = m_afEntry[ 0]*m_afEntry[ 5] - m_afEntry[ 1]*m_afEntry[ 4];
    Real fA1 = m_afEntry[ 0]*m_afEntry[ 6] - m_afEntry[ 2]*m_afEntry[ 4];
    Real fA2 = m_afEntry[ 0]*m_afEntry[ 7] - m_afEntry[ 3]*m_afEntry[ 4];
    Real fA3 = m_afEntry[ 1]*m_afEntry[ 6] - m_afEntry[ 2]*m_afEntry[ 5];
    Real fA4 = m_afEntry[ 1]*m_afEntry[ 7] - m_afEntry[ 3]*m_afEntry[ 5];
    Real fA5 = m_afEntry[ 2]*m_afEntry[ 7] - m_afEntry[ 3]*m_afEntry[ 6];
    Real fB0 = m_afEntry[ 8]*m_afEntry[13] - m_afEntry[ 9]*m_afEntry[12];
    Real fB1 = m_afEntry[ 8]*m_afEntry[14] - m_afEntry[10]*m_afEntry[12];
    Real fB2 = m_afEntry[ 8]*m_afEntry[15] - m_afEntry[11]*m_afEntry[12];
    Real fB3 = m_afEntry[ 9]*m_afEntry[14] - m_afEntry[10]*m_afEntry[13];
    Real fB4 = m_afEntry[ 9]*m_afEntry[15] - m_afEntry[11]*m_afEntry[13];
    Real fB5 = m_afEntry[10]*m_afEntry[15] - m_afEntry[11]*m_afEntry[14];

    Real fDet = fA0*fB5 - fA1*fB4 + fA2*fB3 + fA3*fB2 - fA4*fB1 + fA5*fB0;
    if ( Math<Real>::FAbs(fDet) <= Math<Real>::ZERO_TOLERANCE )
        return Matrix4<Real>::ZERO;

    Matrix4 qInv;
    qInv[0][0] = + m_afEntry[ 5]*fB5 - m_afEntry[ 6]*fB4 + m_afEntry[ 7]*fB3;
    qInv[1][0] = - m_afEntry[ 4]*fB5 + m_afEntry[ 6]*fB2 - m_afEntry[ 7]*fB1;
    qInv[2][0] = + m_afEntry[ 4]*fB4 - m_afEntry[ 5]*fB2 + m_afEntry[ 7]*fB0;
    qInv[3][0] = - m_afEntry[ 4]*fB3 + m_afEntry[ 5]*fB1 - m_afEntry[ 6]*fB0;
    qInv[0][1] = - m_afEntry[ 1]*fB5 + m_afEntry[ 2]*fB4 - m_afEntry[ 3]*fB3;
    qInv[1][1] = + m_afEntry[ 0]*fB5 - m_afEntry[ 2]*fB2 + m_afEntry[ 3]*fB1;
    qInv[2][1] = - m_afEntry[ 0]*fB4 + m_afEntry[ 1]*fB2 - m_afEntry[ 3]*fB0;
    qInv[3][1] = + m_afEntry[ 0]*fB3 - m_afEntry[ 1]*fB1 + m_afEntry[ 2]*fB0;
    qInv[0][2] = + m_afEntry[13]*fA5 - m_afEntry[14]*fA4 + m_afEntry[15]*fA3;
    qInv[1][2] = - m_afEntry[12]*fA5 + m_afEntry[14]*fA2 - m_afEntry[15]*fA1;
    qInv[2][2] = + m_afEntry[12]*fA4 - m_afEntry[13]*fA2 + m_afEntry[15]*fA0;
    qInv[3][2] = - m_afEntry[12]*fA3 + m_afEntry[13]*fA1 - m_afEntry[14]*fA0;
    qInv[0][3] = - m_afEntry[ 9]*fA5 + m_afEntry[10]*fA4 - m_afEntry[11]*fA3;
    qInv[1][3] = + m_afEntry[ 8]*fA5 - m_afEntry[10]*fA2 + m_afEntry[11]*fA1;
    qInv[2][3] = - m_afEntry[ 8]*fA4 + m_afEntry[ 9]*fA2 - m_afEntry[11]*fA0;
    qInv[3][3] = + m_afEntry[ 8]*fA3 - m_afEntry[ 9]*fA1 + m_afEntry[10]*fA0;

    Real fInvDet = ((Real)1.0)/fDet;
    for (int iRow = 0; iRow < 4; iRow++)
    {
        for (int iCol = 0; iCol < 4; iCol++)
            qInv[iRow][iCol] *= fInvDet;
    }

    return qInv;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Matrix4<Real> Matrix4<Real>::GetAdjointed () const
{
    Real fA0 = m_afEntry[ 0]*m_afEntry[ 5] - m_afEntry[ 1]*m_afEntry[ 4];
    Real fA1 = m_afEntry[ 0]*m_afEntry[ 6] - m_afEntry[ 2]*m_afEntry[ 4];
    Real fA2 = m_afEntry[ 0]*m_afEntry[ 7] - m_afEntry[ 3]*m_afEntry[ 4];
    Real fA3 = m_afEntry[ 1]*m_afEntry[ 6] - m_afEntry[ 2]*m_afEntry[ 5];
    Real fA4 = m_afEntry[ 1]*m_afEntry[ 7] - m_afEntry[ 3]*m_afEntry[ 5];
    Real fA5 = m_afEntry[ 2]*m_afEntry[ 7] - m_afEntry[ 3]*m_afEntry[ 6];
    Real fB0 = m_afEntry[ 8]*m_afEntry[13] - m_afEntry[ 9]*m_afEntry[12];
    Real fB1 = m_afEntry[ 8]*m_afEntry[14] - m_afEntry[10]*m_afEntry[12];
    Real fB2 = m_afEntry[ 8]*m_afEntry[15] - m_afEntry[11]*m_afEntry[12];
    Real fB3 = m_afEntry[ 9]*m_afEntry[14] - m_afEntry[10]*m_afEntry[13];
    Real fB4 = m_afEntry[ 9]*m_afEntry[15] - m_afEntry[11]*m_afEntry[13];
    Real fB5 = m_afEntry[10]*m_afEntry[15] - m_afEntry[11]*m_afEntry[14];

    Matrix4 qAdj;
    qAdj[0][0] = + m_afEntry[ 5]*fB5 - m_afEntry[ 6]*fB4 + m_afEntry[ 7]*fB3;
    qAdj[1][0] = - m_afEntry[ 4]*fB5 + m_afEntry[ 6]*fB2 - m_afEntry[ 7]*fB1;
    qAdj[2][0] = + m_afEntry[ 4]*fB4 - m_afEntry[ 5]*fB2 + m_afEntry[ 7]*fB0;
    qAdj[3][0] = - m_afEntry[ 4]*fB3 + m_afEntry[ 5]*fB1 - m_afEntry[ 6]*fB0;
    qAdj[0][1] = - m_afEntry[ 1]*fB5 + m_afEntry[ 2]*fB4 - m_afEntry[ 3]*fB3;
    qAdj[1][1] = + m_afEntry[ 0]*fB5 - m_afEntry[ 2]*fB2 + m_afEntry[ 3]*fB1;
    qAdj[2][1] = - m_afEntry[ 0]*fB4 + m_afEntry[ 1]*fB2 - m_afEntry[ 3]*fB0;
    qAdj[3][1] = + m_afEntry[ 0]*fB3 - m_afEntry[ 1]*fB1 + m_afEntry[ 2]*fB0;
    qAdj[0][2] = + m_afEntry[13]*fA5 - m_afEntry[14]*fA4 + m_afEntry[15]*fA3;
    qAdj[1][2] = - m_afEntry[12]*fA5 + m_afEntry[14]*fA2 - m_afEntry[15]*fA1;
    qAdj[2][2] = + m_afEntry[12]*fA4 - m_afEntry[13]*fA2 + m_afEntry[15]*fA0;
    qAdj[3][2] = - m_afEntry[12]*fA3 + m_afEntry[13]*fA1 - m_afEntry[14]*fA0;
    qAdj[0][3] = - m_afEntry[ 9]*fA5 + m_afEntry[10]*fA4 - m_afEntry[11]*fA3;
    qAdj[1][3] = + m_afEntry[ 8]*fA5 - m_afEntry[10]*fA2 + m_afEntry[11]*fA1;
    qAdj[2][3] = - m_afEntry[ 8]*fA4 + m_afEntry[ 9]*fA2 - m_afEntry[11]*fA0;
    qAdj[3][3] = + m_afEntry[ 8]*fA3 - m_afEntry[ 9]*fA1 + m_afEntry[10]*fA0;

    return qAdj;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Real Matrix4<Real>::GetDeterminant () const
{
    Real fA0 = m_afEntry[ 0]*m_afEntry[ 5] - m_afEntry[ 1]*m_afEntry[ 4];
    Real fA1 = m_afEntry[ 0]*m_afEntry[ 6] - m_afEntry[ 2]*m_afEntry[ 4];
    Real fA2 = m_afEntry[ 0]*m_afEntry[ 7] - m_afEntry[ 3]*m_afEntry[ 4];
    Real fA3 = m_afEntry[ 1]*m_afEntry[ 6] - m_afEntry[ 2]*m_afEntry[ 5];
    Real fA4 = m_afEntry[ 1]*m_afEntry[ 7] - m_afEntry[ 3]*m_afEntry[ 5];
    Real fA5 = m_afEntry[ 2]*m_afEntry[ 7] - m_afEntry[ 3]*m_afEntry[ 6];
    Real fB0 = m_afEntry[ 8]*m_afEntry[13] - m_afEntry[ 9]*m_afEntry[12];
    Real fB1 = m_afEntry[ 8]*m_afEntry[14] - m_afEntry[10]*m_afEntry[12];
    Real fB2 = m_afEntry[ 8]*m_afEntry[15] - m_afEntry[11]*m_afEntry[12];
    Real fB3 = m_afEntry[ 9]*m_afEntry[14] - m_afEntry[10]*m_afEntry[13];
    Real fB4 = m_afEntry[ 9]*m_afEntry[15] - m_afEntry[11]*m_afEntry[13];
    Real fB5 = m_afEntry[10]*m_afEntry[15] - m_afEntry[11]*m_afEntry[14];
    Real fDet = fA0*fB5 - fA1*fB4 + fA2*fB3 + fA3*fB2 - fA4*fB1 + fA5*fB0;
    return fDet;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Real Matrix4<Real>::GetQForm (const Vector4<Real>& rqU, const Vector4<Real>& rqV) const
{
    return rqU.Dot((*this)*rqV);
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
void Matrix4<Real>::MakeObliqueProjection (const Vector3<Real>& rqNormal, const Vector3<Real>& rqPoint,
    const Vector3<Real>& rqDirection)
{
    // The projection plane is Dot(N,X-P) = 0 where N is a 3-by-1 unit-length normal vector and P is a 3-by-1
    // point on the plane.  The projection is oblique to the plane, in the direction of the 3-by-1 vector D.
    // Necessarily Dot(N,D) is not zero for this projection to make sense.  Given a 3-by-1 point U, compute the
    // intersection of the line U+t*D with the plane to obtain t = -Dot(N,U-P)/Dot(N,D).  Then
    //
    //   projection(U) = P + [I - D*N^T/Dot(N,D)]*(U-P)
    //
    // A 4-by-4 homogeneous transformation representing the projection is
    //
    //       +-                               -+
    //   M = | D*N^T - Dot(N,D)*I   -Dot(N,P)D |
    //       |          0^T          -Dot(N,D) |
    //       +-                               -+
    //
    // where M applies to [U^T 1]^T by M*[U^T 1]^T.  The matrix is chosen so that M[3][3] > 0 whenever Dot(N,D) < 0
    // (projection is onto the "positive side" of the plane).

    Real fNdD = rqNormal.Dot(rqDirection);
    Real fNdP = rqNormal.Dot(rqPoint);
    m_afEntry[ 0] = rqDirection[0]*rqNormal[0] - fNdD;
    m_afEntry[ 1] = rqDirection[0]*rqNormal[1];
    m_afEntry[ 2] = rqDirection[0]*rqNormal[2];
    m_afEntry[ 3] = -fNdP*rqDirection[0];
    m_afEntry[ 4] = rqDirection[1]*rqNormal[0];
    m_afEntry[ 5] = rqDirection[1]*rqNormal[1] - fNdD;
    m_afEntry[ 6] = rqDirection[1]*rqNormal[2];
    m_afEntry[ 7] = -fNdP*rqDirection[1];
    m_afEntry[ 8] = rqDirection[2]*rqNormal[0];
    m_afEntry[ 9] = rqDirection[2]*rqNormal[1];
    m_afEntry[10] = rqDirection[2]*rqNormal[2] - fNdD;
    m_afEntry[11] = -fNdP*rqDirection[2];
    m_afEntry[12] = (Real)0.0;
    m_afEntry[13] = (Real)0.0;
    m_afEntry[14] = (Real)0.0;
    m_afEntry[15] = -fNdD;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
void Matrix4<Real>::MakePerspectiveProjection (const Vector3<Real>& rqNormal, const Vector3<Real>& rqPoint,
    const Vector3<Real>& rqEye)
{
    //     +-                                                 -+
    // M = | Dot(N,E-P)*I - E*N^T    -(Dot(N,E-P)*I - E*N^T)*E |
    //     |        -N^t                      Dot(N,E)         |
    //     +-                                                 -+
    //
    // where E is the eye point, P is a point on the plane, and N is a unit-length plane normal.

    Real fNdEmP = rqNormal.Dot(rqEye-rqPoint);

    m_afEntry[ 0] = fNdEmP - rqEye[0]*rqNormal[0];
    m_afEntry[ 1] = -rqEye[0]*rqNormal[1];
    m_afEntry[ 2] = -rqEye[0]*rqNormal[2];
    m_afEntry[ 3] = -(m_afEntry[0]*rqEye[0] + m_afEntry[1]*rqEye[1] + m_afEntry[2]*rqEye[2]);
    m_afEntry[ 4] = -rqEye[1]*rqNormal[0];
    m_afEntry[ 5] = fNdEmP - rqEye[1]*rqNormal[1];
    m_afEntry[ 6] = -rqEye[1]*rqNormal[2];
    m_afEntry[ 7] = -(m_afEntry[4]*rqEye[0] + m_afEntry[5]*rqEye[1] + m_afEntry[6]*rqEye[2]);
    m_afEntry[ 8] = -rqEye[2]*rqNormal[0];
    m_afEntry[ 9] = -rqEye[2]*rqNormal[1];
    m_afEntry[10] = fNdEmP - rqEye[2]*rqNormal[2];
    m_afEntry[11] = -(m_afEntry[8]*rqEye[0] + m_afEntry[9]*rqEye[1] + m_afEntry[10]*rqEye[2]);
    m_afEntry[12] = -rqNormal[0];
    m_afEntry[13] = -rqNormal[1];
    m_afEntry[14] = -rqNormal[2];
    m_afEntry[15] = rqNormal.Dot(rqEye);
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
void Matrix4<Real>::MakeReflection (const Vector3<Real>& rqNormal, const Vector3<Real>& rqPoint)
{
    //     +-                         -+
    // M = | I-2*N*N^T    2*Dot(N,P)*N |
    //     |     0^T            1      |
    //     +-                         -+
    //
    // where P is a point on the plane and N is a unit-length plane normal.

    Real fTwoNdP = ((Real)2.0)*(rqNormal.Dot(rqPoint));

    m_afEntry[ 0] = (Real)1.0 - ((Real)2.0)*rqNormal[0]*rqNormal[0];
    m_afEntry[ 1] = -((Real)2.0)*rqNormal[0]*rqNormal[1];
    m_afEntry[ 2] = -((Real)2.0)*rqNormal[0]*rqNormal[2];
    m_afEntry[ 3] = fTwoNdP*rqNormal[0];
    m_afEntry[ 4] = -((Real)2.0)*rqNormal[1]*rqNormal[0];
    m_afEntry[ 5] = (Real)1.0 - ((Real)2.0)*rqNormal[1]*rqNormal[1];
    m_afEntry[ 6] = -((Real)2.0)*rqNormal[1]*rqNormal[2];
    m_afEntry[ 7] = fTwoNdP*rqNormal[1];
    m_afEntry[ 8] = -((Real)2.0)*rqNormal[2]*rqNormal[0];
    m_afEntry[ 9] = -((Real)2.0)*rqNormal[2]*rqNormal[1];
    m_afEntry[10] = (Real)1.0 - ((Real)2.0)*rqNormal[2]*rqNormal[2];
    m_afEntry[11] = fTwoNdP*rqNormal[2];
    m_afEntry[12] = (Real)0.0;
    m_afEntry[13] = (Real)0.0;
    m_afEntry[14] = (Real)0.0;
    m_afEntry[15] = (Real)1.0;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Matrix4<Real> operator* (Real fScalar, const Matrix4<Real>& rqM)
{
    return rqM*fScalar;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Vector4<Real> operator* (const Vector4<Real>& rqV, const Matrix4<Real>& rqM)
{
    return Vector4<Real>(
        rqV[0]*rqM[0][0]+rqV[1]*rqM[1][0]+rqV[2]*rqM[2][0]+rqV[3]*rqM[3][0],
        rqV[0]*rqM[0][1]+rqV[1]*rqM[1][1]+rqV[2]*rqM[2][1]+rqV[3]*rqM[3][1],
        rqV[0]*rqM[0][2]+rqV[1]*rqM[1][2]+rqV[2]*rqM[2][2]+rqV[3]*rqM[3][2],
        rqV[0]*rqM[0][3]+rqV[1]*rqM[1][3]+rqV[2]*rqM[2][3]+rqV[3]*rqM[3][3]);
}
//------------------------------------------------------------------------------------------------------------------



