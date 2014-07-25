//------------------------------------------------------------------------------------------------------------------
template <class Real>
GMatrix<Real>::GMatrix (int iRows, int iCols)
{
    m_afData = 0;
    m_aafEntry = 0;
    SetSize(iRows,iCols);
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
GMatrix<Real>::GMatrix (int iRows, int iCols, const Real* afEntry)
{
    m_afData = 0;
    m_aafEntry = 0;
    SetMatrix(iRows,iCols,afEntry);
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
GMatrix<Real>::GMatrix (int iRows, int iCols, const Real** aafMatrix)
{
    m_afData = 0;
    m_aafEntry = 0;
    SetMatrix(iRows,iCols,aafMatrix);
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
GMatrix<Real>::GMatrix (const GMatrix& rqM)
{
    m_iRows = 0;
    m_iCols = 0;
    m_iQuantity = 0;
    m_afData = 0;
    m_aafEntry = 0;
    *this = rqM;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
GMatrix<Real>::~GMatrix ()
{
    Deallocate();
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
void GMatrix<Real>::Allocate (bool bSetToZero)
{
    // assert:  m_iRows, m_iCols, and m_iQuantity already initialized

    m_afData = new Real[m_iQuantity];
    if ( bSetToZero )
        MemSet(m_afData,0,m_iQuantity*sizeof(Real));

    m_aafEntry = new Real*[m_iRows];
    for (int iRow = 0; iRow < m_iRows; iRow++)
        m_aafEntry[iRow] = &m_afData[iRow*m_iCols];
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
void GMatrix<Real>::Deallocate ()
{
    delete[] m_afData;
    delete[] m_aafEntry;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
void GMatrix<Real>::SetSize (int iRows, int iCols)
{
    Deallocate();
    if ( iRows > 0 && iCols > 0 )
    {
        m_iRows = iRows;
        m_iCols = iCols;
        m_iQuantity = m_iRows*m_iCols;
        Allocate(true);
    }
    else
    {
        m_iRows = 0;
        m_iCols = 0;
        m_iQuantity = 0;
        m_afData = 0;
        m_aafEntry = 0;
    }
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
void GMatrix<Real>::GetSize (int& riRows, int& riCols) const
{
    riRows = m_iRows;
    riCols = m_iCols;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
int GMatrix<Real>::GetRows () const
{
    return m_iRows;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
int GMatrix<Real>::GetColumns () const
{
    return m_iCols;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
int GMatrix<Real>::GetQuantity () const
{
    return m_iQuantity;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
GMatrix<Real>::operator const Real* () const
{
    return m_afData;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
GMatrix<Real>::operator Real* ()
{
    return m_afData;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
const Real* GMatrix<Real>::operator[] (int iRow) const
{
    assert( 0 <= iRow && iRow < m_iRows );
    return m_aafEntry[iRow];
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Real* GMatrix<Real>::operator[] (int iRow)
{
    assert( 0 <= iRow && iRow < m_iRows );
    return m_aafEntry[iRow];
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Real GMatrix<Real>::operator() (int iRow, int iCol) const
{
    return m_aafEntry[iRow][iCol];
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Real& GMatrix<Real>::operator() (int iRow, int iCol)
{
    assert( 0 <= iRow && iRow < m_iRows && 0 <= iCol && iCol <= m_iCols );
    return m_aafEntry[iRow][iCol];
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
void GMatrix<Real>::SwapRows (int iRow0, int iRow1)
{
    assert( 0 <= iRow0 && iRow0 < m_iRows && 0 <= iRow1 && iRow1 < m_iRows );
    Real* afSave = m_aafEntry[iRow0];
    m_aafEntry[iRow0] = m_aafEntry[iRow1];
    m_aafEntry[iRow1] = afSave;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
void GMatrix<Real>::SetRow (int iRow, const GVector<Real>& rqV)
{
    assert( (0 <= iRow && iRow < m_iRows) && (rqV.GetSize() == m_iCols) );
    for (int iCol = 0; iCol < m_iCols; iCol++)
        m_aafEntry[iRow][iCol] = rqV[iCol];
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
GVector<Real> GMatrix<Real>::GetRow (int iRow) const
{
    assert( 0 <= iRow && iRow < m_iRows );
    GVector<Real> qV(m_iCols);
    for (int iCol = 0; iCol < m_iCols; iCol++)
        qV[iCol] = m_aafEntry[iRow][iCol];
    return qV;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
void GMatrix<Real>::SetColumn (int iCol, const GVector<Real>& rqV)
{
    assert( (0 <= iCol && iCol < m_iCols) && (rqV.GetSize() == m_iRows) );
    for (int iRow = 0; iRow < m_iRows; iRow++)
        m_aafEntry[iRow][iCol] = rqV[iRow];
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
GVector<Real> GMatrix<Real>::GetColumn (int iCol) const
{
    assert( 0 <= iCol && iCol < m_iCols );
    GVector<Real> qV(m_iRows);
    for (int iRow = 0; iRow < m_iRows; iRow++)
        qV[iRow] = m_aafEntry[iRow][iCol];
    return qV;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
void GMatrix<Real>::SetMatrix (int iRows, int iCols, const Real* afData)
{
    Deallocate();
    if ( iRows > 0 && iCols > 0 )
    {
        m_iRows = iRows;
        m_iCols = iCols;
        m_iQuantity = m_iRows*m_iCols;
        Allocate(false);
        MemCpy(m_afData,afData,m_iQuantity*sizeof(Real));
    }
    else
    {
        m_iRows = 0;
        m_iCols = 0;
        m_iQuantity = 0;
        m_afData = 0;
        m_aafEntry = 0;
    }
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
void GMatrix<Real>::SetMatrix (int iRows, int iCols, const Real** aafEntry)
{
    Deallocate();
    if ( iRows > 0 && iCols > 0 )
    {
        m_iRows = iRows;
        m_iCols = iCols;
        m_iQuantity = m_iRows*m_iCols;
        Allocate(false);
        for (int iRow = 0; iRow < m_iRows; iRow++)
        {
            for (int iCol = 0; iCol < m_iCols; iCol++)
                m_aafEntry[iRow][iCol] = aafEntry[iRow][iCol];
        }
    }
    else
    {
        m_iRows = 0;
        m_iCols = 0;
        m_iQuantity = 0;
        m_afData = 0;
        m_aafEntry = 0;
    }
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
void GMatrix<Real>::GetColumnMajor (Real* afCMajor) const
{
    for (int iRow = 0, i = 0; iRow < m_iRows; iRow++)
    {
        for (int iCol = 0; iCol < m_iCols; iCol++)
            afCMajor[i++] = m_aafEntry[iCol][iRow];
    }
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
GMatrix<Real>& GMatrix<Real>::operator= (const GMatrix& rqM)
{
    if ( rqM.m_iQuantity > 0 )
    {
        if ( m_iRows != rqM.m_iRows || m_iCols != rqM.m_iCols )
        {
            Deallocate();
            m_iRows = rqM.m_iRows;
            m_iCols = rqM.m_iCols;
            m_iQuantity = rqM.m_iQuantity;
            Allocate(false);
        }
        for (int iRow = 0; iRow < m_iRows; iRow++)
        {
            for (int iCol = 0; iCol < m_iCols; iCol++)
                m_aafEntry[iRow][iCol] = rqM.m_aafEntry[iRow][iCol];
        }
    }
    else
    {
        Deallocate();
        m_iRows = 0;
        m_iCols = 0;
        m_iQuantity = 0;
        m_afData = 0;
        m_aafEntry = 0;
    }
    return *this;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
int GMatrix<Real>::CompareArrays (const GMatrix& rqM) const
{
    return MemCmp(m_afData,rqM.m_afData,m_iQuantity*sizeof(Real));
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
bool GMatrix<Real>::operator== (const GMatrix& rqM) const
{
    return ( CompareArrays(rqM) == 0 );
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
bool GMatrix<Real>::operator!= (const GMatrix& rqM) const
{
    return ( CompareArrays(rqM) != 0 );
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
bool GMatrix<Real>::operator< (const GMatrix& rqM) const
{
    return ( CompareArrays(rqM) < 0 );
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
bool GMatrix<Real>::operator<= (const GMatrix& rqM) const
{
    return ( CompareArrays(rqM) <= 0 );
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
bool GMatrix<Real>::operator> (const GMatrix& rqM) const
{
    return ( CompareArrays(rqM) > 0 );
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
bool GMatrix<Real>::operator>= (const GMatrix& rqM) const
{
    return ( CompareArrays(rqM) >= 0 );
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
GMatrix<Real> GMatrix<Real>::operator+ (const GMatrix& rqM) const
{
    GMatrix<Real> qSum(rqM.m_iRows,rqM.m_iCols);
    for (int i = 0; i < m_iQuantity; i++)
        qSum.m_afData[i] = m_afData[i] + rqM.m_afData[i];
    return qSum;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
GMatrix<Real> GMatrix<Real>::operator- (const GMatrix& rqM) const
{
    GMatrix<Real> qDiff(rqM.m_iRows,rqM.m_iCols);
    for (int i = 0; i < m_iQuantity; i++)
        qDiff.m_afData[i] = m_afData[i] - rqM.m_afData[i];
    return qDiff;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
GMatrix<Real> GMatrix<Real>::operator* (const GMatrix& rqM) const
{
    // 'this' is RxN, 'M' is NxC, 'product = this*M' is RxC
    assert( m_iCols == rqM.m_iRows );
    GMatrix<Real> qProd(m_iRows,rqM.m_iCols);
    for (int iRow = 0; iRow < qProd.m_iRows; iRow++)
    {
        for (int iCol = 0; iCol < qProd.m_iCols; iCol++)
        {
            for (int iMid = 0; iMid < m_iCols; iMid++)
                qProd.m_aafEntry[iRow][iCol] += m_aafEntry[iRow][iMid]*rqM.m_aafEntry[iMid][iCol];
        }
    }
    return qProd;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
GMatrix<Real> GMatrix<Real>::operator* (Real fScalar) const
{
    GMatrix<Real> qProd(m_iRows,m_iCols);
    for (int i = 0; i < m_iQuantity; i++)
        qProd.m_afData[i] = fScalar*m_afData[i];
    return qProd;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
GMatrix<Real> GMatrix<Real>::operator/ (Real fScalar) const
{
    GMatrix<Real> qQuot(m_iRows,m_iCols);
    int i;

    if ( fScalar != (Real)0.0 )
    {
        Real fInvScalar = ((Real)1.0)/fScalar;
        for (i = 0; i < m_iQuantity; i++)
            qQuot.m_afData[i] = fInvScalar*m_afData[i];
    }
    else
    {
        for (i = 0; i < m_iQuantity; i++)
            qQuot.m_afData[i] = Math<Real>::MAX_REAL;
    }

    return qQuot;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
GMatrix<Real> GMatrix<Real>::operator- () const
{
    GMatrix<Real> qNeg(m_iRows,m_iCols);
    for (int i = 0; i < m_iQuantity; i++)
        qNeg.m_afData[i] = -m_afData[i];
    return qNeg;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
GMatrix<Real> operator* (Real fScalar, const GMatrix<Real>& rqM)
{
    GMatrix<Real> qProd(rqM.GetRows(),rqM.GetColumns());
    const Real* afMEntry = rqM;
    Real* afPEntry = qProd;
    for (int i = 0; i < rqM.GetQuantity(); i++)
        afPEntry[i] = fScalar*afMEntry[i];
    return qProd;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
GMatrix<Real>& GMatrix<Real>::operator+= (const GMatrix& rqM)
{
    for (int i = 0; i < m_iQuantity; i++)
        m_afData[i] += rqM.m_afData[i];
    return *this;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
GMatrix<Real>& GMatrix<Real>::operator-= (const GMatrix& rqM)
{
    for (int i = 0; i < m_iQuantity; i++)
        m_afData[i] -= rqM.m_afData[i];
    return *this;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
GMatrix<Real>& GMatrix<Real>::operator*= (Real fScalar)
{
    for (int i = 0; i < m_iQuantity; i++)
        m_afData[i] *= fScalar;
    return *this;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
GMatrix<Real>& GMatrix<Real>::operator/= (Real fScalar)
{
    int i;

    if ( fScalar != (Real)0.0 )
    {
        Real fInvScalar = ((Real)1.0)/fScalar;
        for (i = 0; i < m_iQuantity; i++)
            m_afData[i] *= fInvScalar;
    }
    else
    {
        for (i = 0; i < m_iQuantity; i++)
            m_afData[i] = Math<Real>::MAX_REAL;
    }

    return *this;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
GMatrix<Real> GMatrix<Real>::GetTransposed () const
{
    GMatrix<Real> qTranspose(m_iCols,m_iRows);
    for (int iRow = 0; iRow < m_iRows; iRow++)
    {
        for (int iCol = 0; iCol < m_iCols; iCol++)
            qTranspose.m_aafEntry[iCol][iRow] = m_aafEntry[iRow][iCol];
    }
    return qTranspose;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
GMatrix<Real> GMatrix<Real>::GetTransposedTimes (const GMatrix& rqM) const
{
    // P = A^T*B, P[r][c] = sum_m A[m][r]*B[m][c]
    assert( m_iRows == rqM.m_iRows );
    GMatrix<Real> qProd(m_iCols,rqM.m_iCols);
    for (int iRow = 0; iRow < qProd.m_iRows; iRow++)
    {
        for (int iCol = 0; iCol < qProd.m_iCols; iCol++)
        {
            for (int iMid = 0; iMid < m_iRows; iMid++)
                qProd.m_aafEntry[iRow][iCol] += m_aafEntry[iMid][iRow]*rqM.m_aafEntry[iMid][iCol];
        }
    }
    return qProd;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
GMatrix<Real> GMatrix<Real>::GetTimesTransposed (const GMatrix& rqM) const
{
    // P = A*B^T, P[r][c] = sum_m A[r][m]*B[c][m]
    assert( m_iCols == rqM.m_iCols );
    GMatrix<Real> qProd(m_iRows,rqM.m_iRows);
    for (int iRow = 0; iRow < qProd.m_iRows; iRow++)
    {
        for (int iCol = 0; iCol < qProd.m_iCols; iCol++)
        {
            for (int iMid = 0; iMid < m_iCols; iMid++)
                qProd.m_aafEntry[iRow][iCol] += m_aafEntry[iRow][iMid]*rqM.m_aafEntry[iCol][iMid];
        }
    }
    return qProd;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
GVector<Real> GMatrix<Real>::operator* (const GVector<Real>& rqV) const
{
    assert( rqV.GetSize() == m_iCols );
    GVector<Real> qProd(m_iRows);
    for (int iRow = 0; iRow < m_iRows; iRow++)
    {
        for (int iCol = 0; iCol < m_iCols; iCol++)
            qProd[iRow] += m_aafEntry[iRow][iCol]*rqV[iCol];
    }
    return qProd;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
GVector<Real> operator* (const GVector<Real>& rqV, const GMatrix<Real>& rqM)
{
    assert( rqV.GetSize() == rqM.GetRows() );
    GVector<Real> qProd(rqM.GetColumns());
    Real* afPEntry = qProd;
    for (int iCol = 0; iCol < rqM.GetColumns(); iCol++)
    {
        for (int iRow = 0; iRow < rqM.GetRows(); iRow++)
            afPEntry[iCol] += rqV[iRow]*rqM[iRow][iCol];
    }
    return qProd;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Real GMatrix<Real>::QForm (const GVector<Real>& rqU, const GVector<Real>& rqV) const
{
    assert( rqU.GetSize() == m_iRows && rqV.GetSize() == m_iCols );
    return rqU.Dot((*this)*rqV);
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
bool GMatrix<Real>::GetInverse (GMatrix<Real>& rqInverse) const
{
    // computations are performed in-place
    if ( GetRows() > 0 && GetRows() != GetColumns() )
        return false;

    int iSize = GetRows();
    rqInverse = *this;

    int* aiColIndex = new int[iSize];
    int* aiRowIndex = new int[iSize];
    bool* abPivoted = new bool[iSize];
    MemSet(abPivoted,0,iSize);

    int i1, i2, iRow = 0, iCol = 0;
    Real fSave;

    // elimination by full pivoting
    for (int i0 = 0; i0 < iSize; i0++)
    {
        // search matrix (excluding pivoted rows) for maximum absolute entry
        Real fMax = (Real)0.0;
        for (i1 = 0; i1 < iSize; i1++)
        {
            if ( !abPivoted[i1] )
            {
                for (i2 = 0; i2 < iSize; i2++)
                {
                    if ( !abPivoted[i2] )
                    {
                        Real fAbs = Math<Real>::FAbs(rqInverse[i1][i2]);
                        if ( fAbs > fMax )
                        {
                            fMax = fAbs;
                            iRow = i1;
                            iCol = i2;
                        }
                    }
                }
            }
        }

        if ( fMax == (Real)0.0 )
        {
            // matrix is not invertible
            delete[] aiColIndex;
            delete[] aiRowIndex;
            delete[] abPivoted;
            return false;
        }

        abPivoted[iCol] = true;

        // swap rows so that A[iCol][iCol] contains the pivot entry
        if ( iRow != iCol )
            rqInverse.SwapRows(iRow,iCol);

        // keep track of the permutations of the rows
        aiRowIndex[i0] = iRow;
        aiColIndex[i0] = iCol;

        // scale the row so that the pivot entry is 1
        Real fInv = ((Real)1.0)/rqInverse[iCol][iCol];
        rqInverse[iCol][iCol] = (Real)1.0;
        for (i2 = 0; i2 < iSize; i2++)
            rqInverse[iCol][i2] *= fInv;

        // zero out the pivot column locations in the other rows
        for (i1 = 0; i1 < iSize; i1++)
        {
            if ( i1 != iCol )
            {
                fSave = rqInverse[i1][iCol];
                rqInverse[i1][iCol] = (Real)0.0;
                for (i2 = 0; i2 < iSize; i2++)
                    rqInverse[i1][i2] -= rqInverse[iCol][i2]*fSave;
            }
        }
    }

    // reorder rows so that A[][] stores the inverse of the original matrix
    for (i1 = iSize-1; i1 >= 0; i1--)
    {
        if ( aiRowIndex[i1] != aiColIndex[i1] )
        {
            for (i2 = 0; i2 < iSize; i2++)
            {
                fSave = rqInverse[i2][aiRowIndex[i1]];
                rqInverse[i2][aiRowIndex[i1]] = rqInverse[i2][aiColIndex[i1]];
                rqInverse[i2][aiColIndex[i1]] = fSave;
            }
        }
    }

    delete[] aiColIndex;
    delete[] aiRowIndex;
    delete[] abPivoted;
    return true;
}
//------------------------------------------------------------------------------------------------------------------



