#ifndef QIDENUTILS_H
#define QIDENUTILS_H

// Static and dynamic type casting.

#include "QRtti.h"

namespace Q
{

template <class ObjectT>
ObjectT* StaticCast (Rtti* pqObject);

template <class ObjectT>
const ObjectT* StaticCast (const Rtti* pqObject);

template <class ObjectT>
ObjectT* DynamicCast (Rtti* pqObject);

template <class ObjectT>
const ObjectT* DynamicCast (const Rtti* pqObject);

#include "QIdenUtils.inl"

}

#endif



