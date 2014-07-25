#include "QIntegerFactorization.h"
#include "QArray.h"
#include "QSortUtils.h"
using namespace Q;

//------------------------------------------------------------------------------------------------------------------
IntegerFactorization::IntegerFactorization (int iNumber)
    :
    m_iNumber(iNumber)
{
    assert( iNumber >= 0 );
}
//------------------------------------------------------------------------------------------------------------------
bool IntegerFactorization::FactorizeFermat ()
{
    if ( m_iNumber == 0 || m_iNumber == 1 )
    {
        m_qFactors.Push(m_iNumber);
        return false;
    }

    static const int s_iTrialPrimeQt = 4;
    static const int s_aiTrialPrime[s_iTrialPrimeQt] = {3, 5, 7, 11};

    Array<Int64> qProcNumbers;
    qProcNumbers.Push(m_iNumber);
    Int64 iNum, iDivRes, iA, iDelta, iSqrB, iB, iC, iD;
    int iPrime;
    bool bTrialSucc;
    for (int i0 = 0; i0 < qProcNumbers.GetQuantity(); i0++)
    {
        iNum = qProcNumbers[i0];

        // division by 2
        if ( (iNum & 1) == 0 )
        {
            m_qFactors.Push(2);
            iDivRes = iNum >> 1;
            if ( iDivRes != 1 )
                qProcNumbers.Push(iDivRes);
            continue;
        }

        // trial division by the primes
        bTrialSucc = false;
        for (int i1 = 0; i1 < s_iTrialPrimeQt; i1++)
        {
            iPrime = s_aiTrialPrime[i1];
            iDivRes = iNum/iPrime;
            if ( iDivRes*iPrime == iNum )
            {
                m_qFactors.Push(iPrime);
                if ( iDivRes != 1 )
                    qProcNumbers.Push(iDivRes);
                bTrialSucc = true;
                break;
            }
        }
        if ( bTrialSucc )
            continue;

        // the Fermat's method
        iA = Mathi::Sqrt(iNum);
        iDelta = (iA << 1) + 1;
        iSqrB = iA*iA - iNum;
        for (/**/; /**/; /**/)
        {
            if ( iSqrB >= 0 )
            {
                // if a square is divisible by 2, it also should be divisible by 4
                if ( !((iSqrB & 1) == 0 && (iSqrB & 3) != 0) )
                {
                    iB = Mathi::Sqrt(iSqrB);
                    if ( iB*iB == iSqrB )
                        break;
                }
            }

            iA++;
            iSqrB += iDelta;
            iDelta += 2;
        }
        iC = iA + iB;
        iD = iA - iB;
        if ( iD != 1 )
        {
            qProcNumbers.Push(iC);
            qProcNumbers.Push(iD);
        }
        else
        {
            m_qFactors.Push((int)iC);
        }
    }

    // sort ascending
    SortUtils<int>::Sort(m_qFactors);

    return ( m_qFactors.GetQuantity() > 1 );
}
//------------------------------------------------------------------------------------------------------------------



