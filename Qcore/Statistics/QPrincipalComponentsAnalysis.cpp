#include "QPrincipalComponentsAnalysis.h"
#include "QEigen.h"
using namespace Q;

//------------------------------------------------------------------------------------------------------------------
template <class Real>
PrincipalComponentsAnalysis<Real>::PrincipalComponentsAnalysis (int iN, int iQuantity, bool bSampling)
    :
    m_iN(iN),
    m_iQuantity(iQuantity),
    m_bSampling(bSampling),
    m_qMeans(iN),
    m_qEigenvalues(iN),
    m_qFeatureVector(iN,iN)
{
    assert( iN >= 2 && iQuantity >= 2 );

    // allocate the data table
    m_qTable.SetQuantity(iN);
    for (int i = 0; i < iN; i++)
        m_qTable[i].SetQuantity(iQuantity);
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
void PrincipalComponentsAnalysis<Real>::SetValue (int iDim, int i, Real fValue)
{
    m_qTable[iDim][i] = fValue;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
void PrincipalComponentsAnalysis<Real>::Analyze ()
{
    Real fInvQ = ((Real)1.0)/m_iQuantity;
    Real fInvQCov = ( m_bSampling ? ((Real)1.0)/(m_iQuantity - 1) : fInvQ );

    // adjust data by subtracting the dimension's mean from each data element in that dimension
    Real fMean;
    for (int i0 = 0; i0 < m_iN; i0++)
    {
        StillArray<Real>& rqElements = m_qTable[i0];

        // calculate the mean
        fMean = (Real)0.0;
        for (int i1 = 0; i1 < m_iQuantity; i1++)
            fMean += rqElements[i1];
        fMean *= fInvQ;

        m_qMeans[i0] = fMean;

        // adjust
        for (int i1 = 0; i1 < m_iQuantity; i1++)
            rqElements[i1] -= fMean;
    }

    // build the covariance matrix; since it is symmetrical about the main diagonal, only diagonal elements and
    // elements above the main diagonal are required for computing
    GMatrix<Real> qCovMat(m_iN,m_iN);
    Real fCov;
    for (int i0 = 0; i0 < m_iN; i0++)
    {
        const StillArray<Real>& rqElements0 = m_qTable[i0];

        for (int i1 = i0; i1 < m_iN; i1++)
        {
            const StillArray<Real>& rqElements1 = m_qTable[i1];

            // calculate the covariance
            fCov = (Real)0.0;
            for (int i2 = 0; i2 < m_iQuantity; i2++)
                fCov += rqElements0[i2]*rqElements1[i2];
            fCov *= fInvQCov;

            // set an element
            qCovMat(i0,i1) = fCov;
            if ( i0 != i1 )
            {
                // reflect
                qCovMat(i1,i0) = fCov;
            }
        }
    }

    // for the given covariance matrix, find its eigenvalues together with eigenvectors and form resulting
    // feature vector in eigenvalue-decreasing order
    if ( m_iN == 2 )
    {
        Matrix2<Real> qM;
        for (int i0 = 0; i0 < 2; i0++)
        {
            for (int i1 = 0; i1 < 2; i1++)
                qM(i0,i1) = qCovMat(i0,i1);
        }

        // solve the eigensystem
        Eigen<Real> qEigen(qM);
        qEigen.DecrSortEigenStuff2();

        // eigenvalues
        for (int i = 0; i < 2; i++)
            m_qEigenvalues[i] = qEigen.GetEigenvalue(i);

        // feature vector
        Vector2<Real> qV;
        for (int i0 = 0; i0 < 2; i0++)
        {
            qEigen.GetEigenvector(i0,qV);
            for (int i1 = 0; i1 < 2; i1++)
                m_qFeatureVector(i1,i0) = qV[i1];
        }
    }
    else if ( m_iN == 3 )
    {
        Matrix3<Real> qM;
        for (int i0 = 0; i0 < 3; i0++)
        {
            for (int i1 = 0; i1 < 3; i1++)
                qM(i0,i1) = qCovMat(i0,i1);
        }

        // solve the eigensystem
        Eigen<Real> qEigen(qM);
        qEigen.DecrSortEigenStuff3();

        // eigenvalues
        for (int i = 0; i < 3; i++)
            m_qEigenvalues[i] = qEigen.GetEigenvalue(i);

        // feature vector
        Vector3<Real> qV;
        for (int i0 = 0; i0 < 3; i0++)
        {
            qEigen.GetEigenvector(i0,qV);
            for (int i1 = 0; i1 < 3; i1++)
                m_qFeatureVector(i1,i0) = qV[i1];
        }
    }
    else if ( m_iN == 4 )
    {
        Matrix4<Real> qM;
        for (int i0 = 0; i0 < 4; i0++)
        {
            for (int i1 = 0; i1 < 4; i1++)
                qM(i0,i1) = qCovMat(i0,i1);
        }

        // solve the eigensystem
        Eigen<Real> qEigen(qM);
        qEigen.DecrSortEigenStuff4();

        // eigenvalues
        for (int i = 0; i < 4; i++)
            m_qEigenvalues[i] = qEigen.GetEigenvalue(i);

        // feature vector
        Vector4<Real> qV;
        for (int i0 = 0; i0 < 4; i0++)
        {
            qEigen.GetEigenvector(i0,qV);
            for (int i1 = 0; i1 < 4; i1++)
                m_qFeatureVector(i1,i0) = qV[i1];
        }
    }
    else  // N > 4
    {
        // solve the eigensystem
        Eigen<Real> qEigen(qCovMat);
        qEigen.DecrSortEigenStuffN();

        // eigenvalues
        for (int i = 0; i < m_iN; i++)
            m_qEigenvalues[i] = qEigen.GetEigenvalue(i);

        // feature vector
        for (int i = 0; i < m_iN; i++)
            m_qFeatureVector.SetColumn(i,qEigen.GetEigenvector(i));
    }
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Real PrincipalComponentsAnalysis<Real>::GetMean (int iDim) const
{
    return m_qMeans[iDim];
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Real PrincipalComponentsAnalysis<Real>::GetPrincipalEigenvalue () const
{
    return m_qEigenvalues.GetFirst();
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
Real PrincipalComponentsAnalysis<Real>::GetEigenvalue (int i) const
{
    return m_qEigenvalues[i];
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
const Real* PrincipalComponentsAnalysis<Real>::GetEigenvalues () const
{
    return m_qEigenvalues;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
void PrincipalComponentsAnalysis<Real>::GetPrincipalComponent (Vector2<Real>& rqV) const
{
    assert( m_iN == 2 );

    for (int i = 0; i < 2; i++)
        rqV[i] = m_qFeatureVector(i,0);
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
void PrincipalComponentsAnalysis<Real>::GetPrincipalComponent (Vector3<Real>& rqV) const
{
    assert( m_iN == 3 );

    for (int i = 0; i < 3; i++)
        rqV[i] = m_qFeatureVector(i,0);
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
void PrincipalComponentsAnalysis<Real>::GetPrincipalComponent (Vector4<Real>& rqV) const
{
    assert( m_iN == 4 );

    for (int i = 0; i < 4; i++)
        rqV[i] = m_qFeatureVector(i,0);
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
GVector<Real> PrincipalComponentsAnalysis<Real>::GetPrincipalComponent () const
{
    return m_qFeatureVector.GetColumn(0);
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
void PrincipalComponentsAnalysis<Real>::GetComponent (int i, Vector2<Real>& rqV) const
{
    assert( m_iN == 2 );

    for (int j = 0; j < 2; j++)
        rqV[j] = m_qFeatureVector(j,i);
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
void PrincipalComponentsAnalysis<Real>::GetComponent (int i, Vector3<Real>& rqV) const
{
    assert( m_iN == 3 );

    for (int j = 0; j < 3; j++)
        rqV[j] = m_qFeatureVector(j,i);
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
void PrincipalComponentsAnalysis<Real>::GetComponent (int i, Vector4<Real>& rqV) const
{
    assert( m_iN == 4 );

    for (int j = 0; j < 4; j++)
        rqV[j] = m_qFeatureVector(j,i);
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
GVector<Real> PrincipalComponentsAnalysis<Real>::GetComponent (int i) const
{
    return m_qFeatureVector.GetColumn(i);
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
void PrincipalComponentsAnalysis<Real>::GetFeatureVector (Matrix2<Real>& rqM) const
{
    assert( m_iN == 2 );

    for (int i0 = 0; i0 < 2; i0++)
    {
        for (int i1 = 0; i1 < 2; i1++)
            rqM(i0,i1) = m_qFeatureVector(i0,i1);
    }
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
void PrincipalComponentsAnalysis<Real>::GetFeatureVector (Matrix3<Real>& rqM) const
{
    assert( m_iN == 3 );

    for (int i0 = 0; i0 < 3; i0++)
    {
        for (int i1 = 0; i1 < 3; i1++)
            rqM(i0,i1) = m_qFeatureVector(i0,i1);
    }
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
void PrincipalComponentsAnalysis<Real>::GetFeatureVector (Matrix4<Real>& rqM) const
{
    assert( m_iN == 4 );

    for (int i0 = 0; i0 < 4; i0++)
    {
        for (int i1 = 0; i1 < 4; i1++)
            rqM(i0,i1) = m_qFeatureVector(i0,i1);
    }
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
const GMatrix<Real>& PrincipalComponentsAnalysis<Real>::GetFeatureVector () const
{
    return m_qFeatureVector;
}
//------------------------------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------------------------------
// explicit instantiation
//------------------------------------------------------------------------------------------------------------------
// float
template
class PrincipalComponentsAnalysis<float>;

// double
template
class PrincipalComponentsAnalysis<double>;
//------------------------------------------------------------------------------------------------------------------



