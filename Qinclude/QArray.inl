// indicates how much an array is to grow depending on the number of previously allocated slots
template <class ElementT>
const int Array<ElementT>::ms_iGrowQuantityDivider(3);  // on 1/3 each time

//------------------------------------------------------------------------------------------------------------------
template <class ElementT>
Array<ElementT>::Array (int iQuantity)
    :
    DynamicArrayBase<ElementT>(iQuantity,iQuantity)
{
}
//------------------------------------------------------------------------------------------------------------------
template <class ElementT>
Array<ElementT>::Array (int iQuantity, int iMaxQuantity)
    :
    DynamicArrayBase<ElementT>(iQuantity,iMaxQuantity)
{
}
//------------------------------------------------------------------------------------------------------------------
template <class ElementT>
Array<ElementT>::Array (const Array& rqArray)
    :
    DynamicArrayBase<ElementT>(rqArray)
{
}
//------------------------------------------------------------------------------------------------------------------
template <class ElementT>
int Array<ElementT>::Push (const ElementT& rtElement)
{
    ConsiderGrowing();
    m_atElement[m_iQuantity] = rtElement;
    return m_iQuantity++;
}
//------------------------------------------------------------------------------------------------------------------
template <class ElementT>
ElementT& Array<ElementT>::GetNewLast ()
{
    ConsiderGrowing();
    return m_atElement[m_iQuantity++];
}
//------------------------------------------------------------------------------------------------------------------
template <class ElementT>
void Array<ElementT>::ConsiderGrowing ()
{
    // consider increasing of 'max_quantity'
    if ( m_iQuantity == m_iMaxQuantity )
        SetMaxQuantity(m_iMaxQuantity+m_iMaxQuantity/ms_iGrowQuantityDivider+1,true);
}
//------------------------------------------------------------------------------------------------------------------



