//------------------------------------------------------------------------------------------------------------------
template <class ElementT>
ModestArray<ElementT>::ModestArray (int iQuantity, int iGrowBy)
    :
    DynamicArrayBase<ElementT>(iQuantity,iQuantity),
    m_iGrowBy(iGrowBy)
{
    assert( iGrowBy > 0 );
}
//------------------------------------------------------------------------------------------------------------------
template <class ElementT>
ModestArray<ElementT>::ModestArray (int iQuantity, int iMaxQuantity, int iGrowBy)
    :
    DynamicArrayBase<ElementT>(iQuantity,iMaxQuantity),
    m_iGrowBy(iGrowBy)
{
    assert( iGrowBy > 0 );
}
//------------------------------------------------------------------------------------------------------------------
template <class ElementT>
ModestArray<ElementT>::ModestArray (const ModestArray& rqArray)
    :
    DynamicArrayBase<ElementT>(rqArray),
    m_iGrowBy(rqArray.m_iGrowBy)
{
}
//------------------------------------------------------------------------------------------------------------------
template <class ElementT>
void ModestArray<ElementT>::SetGrowBy (int iGrowBy)
{
    assert( iGrowBy > 0 );
    m_iGrowBy = iGrowBy;
}
//------------------------------------------------------------------------------------------------------------------
template <class ElementT>
int ModestArray<ElementT>::GetGrowBy () const
{
    return m_iGrowBy;
}
//------------------------------------------------------------------------------------------------------------------
template <class ElementT>
int ModestArray<ElementT>::Push (const ElementT& rtElement)
{
    ConsiderGrowing();
    m_atElement[m_iQuantity] = rtElement;
    return m_iQuantity++;
}
//------------------------------------------------------------------------------------------------------------------
template <class ElementT>
ElementT& ModestArray<ElementT>::GetNewLast ()
{
    ConsiderGrowing();
    return m_atElement[m_iQuantity++];
}
//------------------------------------------------------------------------------------------------------------------
template <class ElementT>
void ModestArray<ElementT>::ConsiderGrowing ()
{
    if ( m_iQuantity == m_iMaxQuantity )
    {
        // increase the number of available slots by 'grow_by'
        SetMaxQuantity(m_iMaxQuantity+m_iGrowBy,true);
    }
}
//------------------------------------------------------------------------------------------------------------------
template <class ElementT>
void ModestArray<ElementT>::Save (Stream& rqStream) const
{
    DynamicArrayBase<ElementT>::Save(rqStream);
    rqStream.Write(m_iGrowBy);
}
//------------------------------------------------------------------------------------------------------------------
template <class ElementT>
void ModestArray<ElementT>::Load (Stream& rqStream)
{
    DynamicArrayBase<ElementT>::Load(rqStream);
    rqStream.Read(m_iGrowBy);
}
//------------------------------------------------------------------------------------------------------------------



