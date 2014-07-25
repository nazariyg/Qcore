//------------------------------------------------------------------------------------------------------------------
template <class ObjectT>
void SwapObjects (ObjectT& rtObject0, ObjectT& rtObject1)
{
    ObjectT tSave(rtObject0);
    rtObject0 = rtObject1;
    rtObject1 = tSave;
}
//------------------------------------------------------------------------------------------------------------------
template <class FirstT, class SecondT>
Link<FirstT,SecondT> MakeLink (const FirstT& rtFirst, const SecondT& rtSecond)
{
    return Link<FirstT,SecondT>(rtFirst,rtSecond);
}
//------------------------------------------------------------------------------------------------------------------
template<class ObjectT>
bool DefaultEquator<ObjectT>::operator() (const ObjectT& rtObject0, const ObjectT& rtObject1) const
{
    return ( rtObject0 == rtObject1 );
}
//------------------------------------------------------------------------------------------------------------------
template<class ObjectT>
bool DefaultComparator<ObjectT>::operator() (const ObjectT& rtObject0, const ObjectT& rtObject1) const
{
    return ( rtObject0 < rtObject1 );
}
//------------------------------------------------------------------------------------------------------------------
template<class ObjectT>
int DefaultEqComparator<ObjectT>::operator() (const ObjectT& rtObject0, const ObjectT& rtObject1) const
{
    if ( rtObject0 == rtObject1 )
        return 0;
    return ( rtObject0 < rtObject1 ? -1 : 1 );
}
//------------------------------------------------------------------------------------------------------------------
template<class KeyT>
int DefaultHasher<KeyT>::operator() (const KeyT& rtKey) const
{
    int iKey = 0;
    MemCpy(&iKey,&rtKey,( sizeof(iKey) < sizeof(KeyT) ? sizeof(iKey) : sizeof(KeyT) ));
    return iKey;
}
//------------------------------------------------------------------------------------------------------------------



