#ifndef QSTRGCNMTUTILS_H
#define QSTRGCNMTUTILS_H

// NOTE. The default behaviors must not be changed.

#include "QLink.h"

namespace Q
{

// swapping
template <class ObjectT>
void SwapObjects (ObjectT& rtObject0, ObjectT& rtObject1);

// construction of a link
template <class FirstT, class SecondT>
Link<FirstT,SecondT> MakeLink (const FirstT& rtFirst, const SecondT& rtSecond);

// default equating
template<class ObjectT>
struct DefaultEquator
{
    bool operator() (const ObjectT& rtObject0, const ObjectT& rtObject1) const;
};

// default boolean comparison (sorting happens in ascending order)
template<class ObjectT>
struct DefaultComparator
{
    bool operator() (const ObjectT& rtObject0, const ObjectT& rtObject1) const;
};

// default three-case comparison: -1 if less, 0 if equal, 1 if greater
template<class ObjectT>
struct DefaultEqComparator
{
    int operator() (const ObjectT& rtObject0, const ObjectT& rtObject1) const;
};

// default hash function
template<class KeyT>
struct DefaultHasher
{
    int operator() (const KeyT& rtKey) const;
};

#include "QStrgCnmtUtils.inl"

}

#endif



