//------------------------------------------------------------------------------------------------------------------
template <class ElementT>
HeapArray<ElementT>::HeapArray (int iQuantity)
    :
    StillArray<ElementT>(iQuantity)
{
}
//------------------------------------------------------------------------------------------------------------------
template <class ElementT>
HeapArray<ElementT>::HeapArray (ElementT* atElement, int iQuantity)
{
    assert( iQuantity >= 0 );

    m_iQuantity = iQuantity;
    m_atElement = atElement;
}
//------------------------------------------------------------------------------------------------------------------
template <class ElementT>
HeapArray<ElementT>::HeapArray (const HeapArray& rqHeapArray)
    :
    StillArray<ElementT>(rqHeapArray)
{
}
//------------------------------------------------------------------------------------------------------------------
template <class ElementT>
void HeapArray<ElementT>::Save (Stream& rqStream) const
{
    StillArray<ElementT>::Save(rqStream);
}
//------------------------------------------------------------------------------------------------------------------
template <class ElementT>
void HeapArray<ElementT>::Load (Stream& rqStream)
{
    StillArray<ElementT>::Load(rqStream);
}
//------------------------------------------------------------------------------------------------------------------



