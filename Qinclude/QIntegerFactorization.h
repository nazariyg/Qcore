#ifndef QINTEGERFACTORIZATION_H
#define QINTEGERFACTORIZATION_H

#include "QModestArray.h"

namespace Q
{

class Q_ITEM IntegerFactorization
{
public:
    // construction
    IntegerFactorization (int iNumber);

    // factorization
    bool FactorizeFermat ();
    bool FactorizePrism ();

    // factor access
    int GetFactorQuantity () const;
    int GetFactor (int i) const;

private:
    int m_iNumber;
    ModestArray<int> m_qFactors;
};

#include "QIntegerFactorization.inl"

}

#endif



