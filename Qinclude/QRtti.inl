//------------------------------------------------------------------------------------------------------------------
inline const ClassLabel& Rtti::GetClass () const
{
    return Class;
}
//------------------------------------------------------------------------------------------------------------------
inline bool Rtti::IsExactly (const ClassLabel& rqClass) const
{
    return Class.IsExactly(rqClass);
}
//------------------------------------------------------------------------------------------------------------------
inline bool Rtti::IsDerived (const ClassLabel& rqClass) const
{
    return Class.IsDerived(rqClass);
}
//------------------------------------------------------------------------------------------------------------------
inline bool Rtti::IsExactlyClassOf (const Rtti* pqObject) const
{
    return ( pqObject && Class.IsExactly(pqObject->GetClass()) );
}
//------------------------------------------------------------------------------------------------------------------
inline bool Rtti::IsDerivedClassOf (const Rtti* pqObject) const
{
    return ( pqObject && Class.IsDerived(pqObject->GetClass()) );
}
//------------------------------------------------------------------------------------------------------------------



