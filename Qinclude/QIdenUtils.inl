//------------------------------------------------------------------------------------------------------------------
template <class ObjectT>
ObjectT* StaticCast (Rtti* pqObject)
{
    return (ObjectT*)pqObject;
}
//------------------------------------------------------------------------------------------------------------------
template <class ObjectT>
const ObjectT* StaticCast (const Rtti* pqObject)
{
    return (const ObjectT*)pqObject;
}
//------------------------------------------------------------------------------------------------------------------
template <class ObjectT>
ObjectT* DynamicCast (Rtti* pqObject)
{
    return ( pqObject && pqObject->IsDerived(ObjectT::Class) ? (ObjectT*)pqObject : 0 );
}
//------------------------------------------------------------------------------------------------------------------
template <class ObjectT>
const ObjectT* DynamicCast (const Rtti* pqObject)
{
    return ( pqObject && pqObject->IsDerived(ObjectT::Class) ? (const ObjectT*)pqObject : 0 );
}
//------------------------------------------------------------------------------------------------------------------



