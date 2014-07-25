//------------------------------------------------------------------------------------------------------------------
template <class ElementT>
DynamicArrayBase<ElementT>::DynamicArrayBase (int iQuantity, int iMaxQuantity)
    :
    m_iMaxQuantity(iMaxQuantity)
{
    assert( iQuantity >= 0 && iMaxQuantity >= 0 && iQuantity <= iMaxQuantity );

    m_iQuantity = iQuantity;
    m_atElement = ( iMaxQuantity != 0 ? new ElementT[iMaxQuantity] : 0 );
}
//------------------------------------------------------------------------------------------------------------------
template <class ElementT>
DynamicArrayBase<ElementT>::DynamicArrayBase (const DynamicArrayBase& rqArray)
    :
    StillArray<ElementT>(rqArray)
{
    m_iMaxQuantity = rqArray.m_iQuantity;
}
//------------------------------------------------------------------------------------------------------------------
template <class ElementT>
int DynamicArrayBase<ElementT>::GetMaxQuantity () const
{
    return m_iMaxQuantity;
}
//------------------------------------------------------------------------------------------------------------------
template <class ElementT>
void DynamicArrayBase<ElementT>::SetQuantity (int iNewQuantity, bool bCopy)
{
    StillArray<ElementT>::SetQuantity(iNewQuantity,bCopy);
    m_iMaxQuantity = m_iQuantity;
}
//------------------------------------------------------------------------------------------------------------------
template <class ElementT>
void DynamicArrayBase<ElementT>::SetMaxQuantity (int iNewMaxQuantity, bool bCopy)
{
    assert( iNewMaxQuantity >= 0 );

    if ( iNewMaxQuantity == 0 )
    {
        m_iQuantity = 0;
        m_iMaxQuantity = 0;
        delete[] m_atElement;
        m_atElement = 0;
        return;
    }

    ElementT* atNewElement = new ElementT[iNewMaxQuantity];
    if ( bCopy )
    {
        if ( m_iQuantity < iNewMaxQuantity )
        {
            for (int i = 0; i < m_iQuantity; i++)
                atNewElement[i] = m_atElement[i];
        }
        else
        {
            for (int i = 0; i < iNewMaxQuantity; i++)
                atNewElement[i] = m_atElement[i];

            m_iQuantity = iNewMaxQuantity;
        }
    }
    m_iMaxQuantity = iNewMaxQuantity;
    delete[] m_atElement;
    m_atElement = atNewElement;
}
//------------------------------------------------------------------------------------------------------------------
template <class ElementT>
void DynamicArrayBase<ElementT>::Compact ()
{
    if ( m_iMaxQuantity != m_iQuantity )
        SetMaxQuantity(m_iQuantity,true);
}
//------------------------------------------------------------------------------------------------------------------
template <class ElementT>
void DynamicArrayBase<ElementT>::Insert (int i, const ElementT& rtElement)
{
    assert( 0 <= i && i <= m_iQuantity );

    if ( m_iQuantity + 1 > m_iMaxQuantity )
        SetMaxQuantity(m_iMaxQuantity+1,true);

    ElementT tSave;
    ElementT tPrior = m_atElement[i];
    m_atElement[i] = rtElement;
    m_iQuantity++;
    for (int i0 = i+1; i0 < m_iQuantity; i0++)
    {
        tSave = m_atElement[i0];
        m_atElement[i0] = tPrior;
        tPrior = tSave;
    }
}
//------------------------------------------------------------------------------------------------------------------
template <class ElementT>
void DynamicArrayBase<ElementT>::Pop ()
{
    assert( m_iQuantity != 0 );
    m_atElement[--m_iQuantity] = ElementT();
}
//------------------------------------------------------------------------------------------------------------------
template <class ElementT>
void DynamicArrayBase<ElementT>::Remove (int i)
{
    assert( 0 <= i && i < m_iQuantity );

    for (int j = i+1; j < m_iQuantity; i = j++)
        m_atElement[i] = m_atElement[j];
    m_atElement[--m_iQuantity] = ElementT();
}
//------------------------------------------------------------------------------------------------------------------
template <class ElementT>
void DynamicArrayBase<ElementT>::Remove (int iFrom, int iTo)
{
    assert( 0 <= iFrom && iFrom < m_iQuantity && 0 <= iTo && iTo <= m_iQuantity && iFrom <= iTo );

    for (int i0 = iFrom, i1 = iTo; i1 < m_iQuantity; i0++, i1++)
        m_atElement[i0] = m_atElement[i1];
    for (int i = m_iQuantity-iTo+iFrom; i < m_iQuantity; i++)
        m_atElement[i] = ElementT();
    m_iQuantity -= iTo - iFrom;
}
//------------------------------------------------------------------------------------------------------------------
template <class ElementT>
void DynamicArrayBase<ElementT>::RemoveAll ()
{
    StillArray<ElementT>::RemoveAll();
    m_iMaxQuantity = 0;
}
//------------------------------------------------------------------------------------------------------------------
template <class ElementT>
void DynamicArrayBase<ElementT>::BecomeOwner (ElementT* atElement, int iQuantity)
{
    StillArray<ElementT>::BecomeOwner(atElement,iQuantity);
    m_iMaxQuantity = iQuantity;
}
//------------------------------------------------------------------------------------------------------------------
template <class ElementT>
DynamicArrayBase<ElementT>& DynamicArrayBase<ElementT>::operator= (const DynamicArrayBase& rqArray)
{
    StillArray<ElementT>::operator=(rqArray);
    m_iMaxQuantity = rqArray.m_iQuantity;
    return *this;
}
//------------------------------------------------------------------------------------------------------------------
template <class ElementT>
void DynamicArrayBase<ElementT>::Swap (DynamicArrayBase& rqArray)
{
    StillArray<ElementT>::Swap(rqArray);
    SwapObjects(m_iMaxQuantity,rqArray.m_iMaxQuantity);
}
//------------------------------------------------------------------------------------------------------------------
template <class ElementT>
void DynamicArrayBase<ElementT>::Save (Stream& rqStream) const
{
    StillArray<ElementT>::Save(rqStream);
}
//------------------------------------------------------------------------------------------------------------------
template <class ElementT>
void DynamicArrayBase<ElementT>::Load (Stream& rqStream)
{
    StillArray<ElementT>::Load(rqStream);
    m_iMaxQuantity = m_iQuantity;
}
//------------------------------------------------------------------------------------------------------------------



