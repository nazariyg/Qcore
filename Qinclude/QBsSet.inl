//------------------------------------------------------------------------------------------------------------------
template <class ElementT, class ComparatorT>
BsSet<ElementT,ComparatorT>::BsSet (int iQuantity, int iMaxQuantity)
    :
    m_qArray(iQuantity,iMaxQuantity),
    m_bSorted(false)
{
}
//------------------------------------------------------------------------------------------------------------------
template <class ElementT, class ComparatorT>
BsSet<ElementT,ComparatorT>::BsSet (const BsSet& rqSet)
    :
    m_qArray(rqSet.m_qArray),
    m_bSorted(rqSet.m_bSorted),
    m_tComparator(rqSet.m_tComparator)
{
}
//------------------------------------------------------------------------------------------------------------------
template <class ElementT, class ComparatorT>
int BsSet<ElementT,ComparatorT>::GetQuantity () const
{
    return m_qArray.GetQuantity();
}
//------------------------------------------------------------------------------------------------------------------
template <class ElementT, class ComparatorT>
int BsSet<ElementT,ComparatorT>::GetMaxQuantity () const
{
    return m_qArray.GetMaxQuantity();
}
//------------------------------------------------------------------------------------------------------------------
template <class ElementT, class ComparatorT>
bool BsSet<ElementT,ComparatorT>::IsEmpty () const
{
    return m_qArray.IsEmpty();
}
//------------------------------------------------------------------------------------------------------------------
template <class ElementT, class ComparatorT>
ElementT* BsSet<ElementT,ComparatorT>::GetElements ()
{
    return m_qArray.GetElements();
}
//------------------------------------------------------------------------------------------------------------------
template <class ElementT, class ComparatorT>
const ElementT* BsSet<ElementT,ComparatorT>::GetElements () const
{
    return m_qArray.GetElements();
}
//------------------------------------------------------------------------------------------------------------------
template <class ElementT, class ComparatorT>
BsSet<ElementT,ComparatorT>::operator ElementT* ()
{
    return m_qArray;
}
//------------------------------------------------------------------------------------------------------------------
template <class ElementT, class ComparatorT>
BsSet<ElementT,ComparatorT>::operator const ElementT* () const
{
    return m_qArray;
}
//------------------------------------------------------------------------------------------------------------------
template <class ElementT, class ComparatorT>
ElementT& BsSet<ElementT,ComparatorT>::operator[] (int i)
{
    return m_qArray[i];
}
//------------------------------------------------------------------------------------------------------------------
template <class ElementT, class ComparatorT>
const ElementT& BsSet<ElementT,ComparatorT>::operator[] (int i) const
{
    return m_qArray[i];
}
//------------------------------------------------------------------------------------------------------------------
template <class ElementT, class ComparatorT>
bool BsSet<ElementT,ComparatorT>::Insert (const ElementT& rtElement)
{
    int iIndex;
    if ( FindLinearly(rtElement,iIndex) )
        return false;
    m_qArray.Push(rtElement);
    m_bSorted = false;
    return true;
}
//------------------------------------------------------------------------------------------------------------------
template <class ElementT, class ComparatorT>
void BsSet<ElementT,ComparatorT>::InsertNoCheck (const ElementT& rtElement)
{
    m_qArray.Push(rtElement);
    m_bSorted = false;
}
//------------------------------------------------------------------------------------------------------------------
template <class ElementT, class ComparatorT>
void BsSet<ElementT,ComparatorT>::PrepareBinarySearch ()
{
    if ( !m_bSorted )
    {
        SortUtils<ElementT,ComparatorT>::Sort(m_qArray);
        m_bSorted = true;
    }
}
//------------------------------------------------------------------------------------------------------------------
template <class ElementT, class ComparatorT>
void BsSet<ElementT,ComparatorT>::SetIsSorted ()
{
    m_bSorted = true;
}
//------------------------------------------------------------------------------------------------------------------
template <class ElementT, class ComparatorT>
bool BsSet<ElementT,ComparatorT>::IsPresent (const ElementT& rtElement) const
{
    int iIndex;
    return Find(rtElement,iIndex);
}
//------------------------------------------------------------------------------------------------------------------
template <class ElementT, class ComparatorT>
bool BsSet<ElementT,ComparatorT>::Find (const ElementT& rtElement, int& riIndex) const
{
    if ( !m_bSorted )
        return FindLinearly(rtElement,riIndex);
    else
        return FindBinary(rtElement,riIndex);
}
//------------------------------------------------------------------------------------------------------------------
template <class ElementT, class ComparatorT>
void BsSet<ElementT,ComparatorT>::Remove (const ElementT& rtElement)
{
    int iIndex;
    if ( Find(rtElement,iIndex) )
        m_qArray.Remove(iIndex);
}
//------------------------------------------------------------------------------------------------------------------
template <class ElementT, class ComparatorT>
void BsSet<ElementT,ComparatorT>::RemoveAll ()
{
    m_qArray.RemoveAll();
}
//------------------------------------------------------------------------------------------------------------------
template <class ElementT, class ComparatorT>
void BsSet<ElementT,ComparatorT>::SetQuantity (int iNewQuantity, bool bCopy)
{
    m_qArray.SetQuantity(iNewQuantity,bCopy);
}
//------------------------------------------------------------------------------------------------------------------
template <class ElementT, class ComparatorT>
void BsSet<ElementT,ComparatorT>::SetMaxQuantity (int iNewMaxQuantity, bool bCopy)
{
    m_qArray.SetMaxQuantity(iNewMaxQuantity,bCopy);
}
//------------------------------------------------------------------------------------------------------------------
template <class ElementT, class ComparatorT>
BsSet<ElementT,ComparatorT>& BsSet<ElementT,ComparatorT>::operator= (const BsSet& rqSet)
{
    if ( &rqSet == this )
        return *this;

    m_qArray = rqSet.m_qArray;
    m_bSorted = rqSet.m_bSorted;
    m_tComparator = rqSet.m_tComparator;
    return *this;
}
//------------------------------------------------------------------------------------------------------------------
template <class ElementT, class ComparatorT>
bool BsSet<ElementT,ComparatorT>::operator== (const BsSet& rqSet) const
{
    return ( m_qArray == rqSet.m_qArray );
}
//------------------------------------------------------------------------------------------------------------------
template <class ElementT, class ComparatorT>
bool BsSet<ElementT,ComparatorT>::operator!= (const BsSet& rqSet) const
{
    return ( m_qArray != rqSet.m_qArray );
}
//------------------------------------------------------------------------------------------------------------------
template <class ElementT, class ComparatorT>
bool BsSet<ElementT,ComparatorT>::operator< (const BsSet& rqSet) const
{
    return ( m_qArray < rqSet.m_qArray );
}
//------------------------------------------------------------------------------------------------------------------
template <class ElementT, class ComparatorT>
bool BsSet<ElementT,ComparatorT>::operator<= (const BsSet& rqSet) const
{
    return ( m_qArray <= rqSet.m_qArray );
}
//------------------------------------------------------------------------------------------------------------------
template <class ElementT, class ComparatorT>
bool BsSet<ElementT,ComparatorT>::operator> (const BsSet& rqSet) const
{
    return ( m_qArray > rqSet.m_qArray );
}
//------------------------------------------------------------------------------------------------------------------
template <class ElementT, class ComparatorT>
bool BsSet<ElementT,ComparatorT>::operator>= (const BsSet& rqSet) const
{
    return ( m_qArray >= rqSet.m_qArray );
}
//------------------------------------------------------------------------------------------------------------------
template <class ElementT, class ComparatorT>
void BsSet<ElementT,ComparatorT>::Swap (BsSet& rqSet)
{
    m_qArray.Swap(rqSet.m_qArray);
    SwapObjects(m_bSorted,rqSet.m_bSorted);
    SwapObjects(m_tComparator,rqSet.m_tComparator);
}
//------------------------------------------------------------------------------------------------------------------
template <class ElementT, class ComparatorT>
bool BsSet<ElementT,ComparatorT>::FindLinearly (const ElementT& rtElement, int& riIndex) const
{
    for (int i = 0; i < m_qArray.GetQuantity(); i++)
    {
        if ( rtElement == m_qArray[i] )
        {
            riIndex = i;
            return true;
        }
    }
    return false;
}
//------------------------------------------------------------------------------------------------------------------
template <class ElementT, class ComparatorT>
bool BsSet<ElementT,ComparatorT>::FindBinary (const ElementT& rtElement, int& riIndex) const
{
    if ( m_qArray.IsEmpty() )
        return false;

    // special case
    if ( !m_tComparator(rtElement,m_qArray.GetLast()) )
    {
        if ( rtElement == m_qArray.GetLast() )
        {
            riIndex = m_qArray.GetQuantity() - 1;
            return true;
        }
        else
        {
            return false;
        }
    }

    int iFrom = 0;
    int iTo = m_qArray.GetQuantity();
    int iDist, iPivot;
    for (/**/; /**/; /**/)
    {
        iDist = iTo - iFrom;
        if ( iDist <= 1 )
            break;
        iPivot = iFrom + iDist/2;
        if ( m_tComparator(rtElement,m_qArray[iPivot]) )
            iTo = iPivot;
        else
            iFrom = iPivot;
    }
    if ( rtElement == m_qArray[iFrom] )
    {
        riIndex = iFrom;
        return true;
    }
    else
    {
        return false;
    }
}
//------------------------------------------------------------------------------------------------------------------
template <class ElementT, class ComparatorT>
void BsSet<ElementT,ComparatorT>::Save (Stream& rqStream) const
{
    m_qArray.Save(rqStream);
    rqStream.Write(m_bSorted);
}
//------------------------------------------------------------------------------------------------------------------
template <class ElementT, class ComparatorT>
void BsSet<ElementT,ComparatorT>::Load (Stream& rqStream)
{
    m_qArray.Load(rqStream);
    rqStream.Read(m_bSorted);
}
//------------------------------------------------------------------------------------------------------------------



