//------------------------------------------------------------------------------------------------------------------
template <class ElementT>
StillArray<ElementT>::StillArray (int iQuantity)
    :
    m_iQuantity(iQuantity)
{
    assert( iQuantity >= 0 );
    m_atElement = ( iQuantity != 0 ? new ElementT[iQuantity] : 0 );
}
//------------------------------------------------------------------------------------------------------------------
template <class ElementT>
StillArray<ElementT>::StillArray (const StillArray& rqArray)
    :
    m_atElement(0)
{
    *this = rqArray;
}
//------------------------------------------------------------------------------------------------------------------
template <class ElementT>
StillArray<ElementT>::StillArray (ElementT* atElement, int iQuantity)
    :
    m_atElement(atElement),
    m_iQuantity(iQuantity)
{
    assert( iQuantity >= 0 );
}
//------------------------------------------------------------------------------------------------------------------
template <class ElementT>
StillArray<ElementT>::~StillArray ()
{
    delete[] m_atElement;
}
//------------------------------------------------------------------------------------------------------------------
template <class ElementT>
int StillArray<ElementT>::GetQuantity () const
{
    return m_iQuantity;
}
//------------------------------------------------------------------------------------------------------------------
template <class ElementT>
bool StillArray<ElementT>::IsEmpty () const
{
    return ( m_iQuantity == 0 );
}
//------------------------------------------------------------------------------------------------------------------
template <class ElementT>
ElementT* StillArray<ElementT>::GetElements ()
{
    return m_atElement;
}
//------------------------------------------------------------------------------------------------------------------
template <class ElementT>
const ElementT* StillArray<ElementT>::GetElements () const
{
    return m_atElement;
}
//------------------------------------------------------------------------------------------------------------------
template <class ElementT>
StillArray<ElementT>::operator ElementT* ()
{
    return m_atElement;
}
//------------------------------------------------------------------------------------------------------------------
template <class ElementT>
StillArray<ElementT>::operator const ElementT* () const
{
    return m_atElement;
}
//------------------------------------------------------------------------------------------------------------------
template <class ElementT>
ElementT& StillArray<ElementT>::operator[] (int i)
{
    assert( 0 <= i && i < m_iQuantity );
    return m_atElement[i];
}
//------------------------------------------------------------------------------------------------------------------
template <class ElementT>
const ElementT& StillArray<ElementT>::operator[] (int i) const
{
    assert( 0 <= i && i < m_iQuantity );
    return m_atElement[i];
}
//------------------------------------------------------------------------------------------------------------------
template <class ElementT>
ElementT& StillArray<ElementT>::GetFirst ()
{
    assert( m_iQuantity != 0 );
    return m_atElement[0];
}
//------------------------------------------------------------------------------------------------------------------
template <class ElementT>
const ElementT& StillArray<ElementT>::GetFirst () const
{
    assert( m_iQuantity != 0 );
    return m_atElement[0];
}
//------------------------------------------------------------------------------------------------------------------
template <class ElementT>
ElementT& StillArray<ElementT>::GetLast ()
{
    assert( m_iQuantity != 0 );
    return m_atElement[m_iQuantity-1];
}
//------------------------------------------------------------------------------------------------------------------
template <class ElementT>
const ElementT& StillArray<ElementT>::GetLast () const
{
    assert( m_iQuantity != 0 );
    return m_atElement[m_iQuantity-1];
}
//------------------------------------------------------------------------------------------------------------------
template <class ElementT>
void StillArray<ElementT>::SetQuantity (int iNewQuantity, bool bCopy)
{
    assert( iNewQuantity >= 0 );

    if ( iNewQuantity == 0 )
    {
        m_iQuantity = 0;
        delete[] m_atElement;
        m_atElement = 0;
        return;
    }

    ElementT* atNewElement = new ElementT[iNewQuantity];
    if ( bCopy )
    {
        int iCopyQuantity = ( m_iQuantity < iNewQuantity ? m_iQuantity : iNewQuantity );
        for (int i = 0; i < iCopyQuantity; i++)
            atNewElement[i] = m_atElement[i];
    }
    m_iQuantity = iNewQuantity;
    delete[] m_atElement;
    m_atElement = atNewElement;
}
//------------------------------------------------------------------------------------------------------------------
template <class ElementT>
void StillArray<ElementT>::AdvanceQuantity (int iAdvance)
{
    SetQuantity(m_iQuantity+iAdvance,true);
}
//------------------------------------------------------------------------------------------------------------------
template <class ElementT>
void StillArray<ElementT>::Append (const StillArray& rqArray)
{
    int iPrevQuantity = m_iQuantity;
    SetQuantity(m_iQuantity+rqArray.m_iQuantity,true);
    for (int i = 0; i < rqArray.m_iQuantity; i++)
        m_atElement[iPrevQuantity+i] = rqArray.m_atElement[i];
}
//------------------------------------------------------------------------------------------------------------------
template <class ElementT>
void StillArray<ElementT>::Reverse ()
{
    if ( m_iQuantity <= 1 )
        return;

    ElementT tSave;
    int iQD2 = m_iQuantity/2;
    for (int i0 = 0, i1 = m_iQuantity-1; i0 < iQD2; i0++, i1--)
    {
        tSave = m_atElement[i0];
        m_atElement[i0] = m_atElement[i1];
        m_atElement[i1] = tSave;
    }
}
//------------------------------------------------------------------------------------------------------------------
template <class ElementT>
void StillArray<ElementT>::Shuffle ()
{
    if ( m_iQuantity <= 1 )
        return;

    // the Fisher-Yates algorithm
    ElementT tSave;
    int iRem;
    int iQMO = m_iQuantity - 1;
    for (int i = 0; i < iQMO; i++)
    {
        iRem = i + rand()/(RAND_MAX/(m_iQuantity - i) + 1);
        tSave = m_atElement[iRem];
        m_atElement[iRem] = m_atElement[i];
        m_atElement[i] = tSave;
    }
}
//------------------------------------------------------------------------------------------------------------------
template <class ElementT>
void StillArray<ElementT>::RemoveAll ()
{
    m_iQuantity = 0;
    delete[] m_atElement;
    m_atElement = 0;
}
//------------------------------------------------------------------------------------------------------------------
template <class ElementT>
void StillArray<ElementT>::BecomeOwner (ElementT* atElement, int iQuantity)
{
    assert( iQuantity >= 0 );

    delete[] m_atElement;
    m_atElement = atElement;
    m_iQuantity = iQuantity;
}
//------------------------------------------------------------------------------------------------------------------
template <class ElementT>
StillArray<ElementT>& StillArray<ElementT>::operator= (const StillArray& rqArray)
{
    if ( &rqArray == this )
        return *this;

    delete[] m_atElement;
    m_atElement = new ElementT[rqArray.m_iQuantity];
    for (int i = 0; i < rqArray.m_iQuantity; i++)
        m_atElement[i] = rqArray.m_atElement[i];
    m_iQuantity = rqArray.m_iQuantity;
    return *this;
}
//------------------------------------------------------------------------------------------------------------------
template <class ElementT>
bool StillArray<ElementT>::operator== (const StillArray& rqArray) const
{
    if ( m_iQuantity != rqArray.m_iQuantity )
        return false;

    if ( m_iQuantity != 0 )
    {
        for (int i = 0; i < m_iQuantity; i++)
        {
            if ( !(m_atElement[i] == rqArray.m_atElement[i]) )
                return false;
        }
    }  // else: both are empty, so they are equal
    return true;
}
//------------------------------------------------------------------------------------------------------------------
template <class ElementT>
bool StillArray<ElementT>::operator!= (const StillArray& rqArray) const
{
    return ( !(*this == rqArray) );
}
//------------------------------------------------------------------------------------------------------------------
template <class ElementT>
bool StillArray<ElementT>::operator< (const StillArray& rqArray) const
{
    if ( m_iQuantity == rqArray.m_iQuantity )
    {
        if ( m_iQuantity != 0 )
        {
            for (int i = 0; i < m_iQuantity; i++)
            {
                if ( !(m_atElement[i] < rqArray.m_atElement[i]) )
                    return false;
            }
            return true;
        }
        else
        {
            return false;
        }
    }
    else if ( m_iQuantity < rqArray.m_iQuantity )
    {
        return true;
    }
    else
    {
        return false;
    }
}
//------------------------------------------------------------------------------------------------------------------
template <class ElementT>
bool StillArray<ElementT>::operator<= (const StillArray& rqArray) const
{
    if ( m_iQuantity == rqArray.m_iQuantity )
    {
        if ( m_iQuantity != 0 )
        {
            for (int i = 0; i < m_iQuantity; i++)
            {
                if ( rqArray.m_atElement[i] < m_atElement[i] )
                    return false;
            }
        }
        return true;
    }
    else if ( m_iQuantity < rqArray.m_iQuantity )
    {
        return true;
    }
    else
    {
        return false;
    }
}
//------------------------------------------------------------------------------------------------------------------
template <class ElementT>
bool StillArray<ElementT>::operator> (const StillArray& rqArray) const
{
    if ( m_iQuantity == rqArray.m_iQuantity )
    {
        if ( m_iQuantity != 0 )
        {
            for (int i = 0; i < m_iQuantity; i++)
            {
                if ( !(rqArray.m_atElement[i] < m_atElement[i]) )
                    return false;
            }
            return true;
        }
        else
        {
            return false;
        }
    }
    else if ( m_iQuantity > rqArray.m_iQuantity )
    {
        return true;
    }
    else
    {
        return false;
    }
}
//------------------------------------------------------------------------------------------------------------------
template <class ElementT>
bool StillArray<ElementT>::operator>= (const StillArray& rqArray) const
{
    if ( m_iQuantity == rqArray.m_iQuantity )
    {
        if ( m_iQuantity != 0 )
        {
            for (int i = 0; i < m_iQuantity; i++)
            {
                if ( m_atElement[i] < rqArray.m_atElement[i] )
                    return false;
            }
        }
        return true;
    }
    else if ( m_iQuantity > rqArray.m_iQuantity )
    {
        return true;
    }
    else
    {
        return false;
    }
}
//------------------------------------------------------------------------------------------------------------------
template <class ElementT>
void StillArray<ElementT>::Swap (StillArray& rqArray)
{
    SwapObjects(m_iQuantity,rqArray.m_iQuantity);
    SwapObjects(m_atElement,rqArray.m_atElement);
}
//------------------------------------------------------------------------------------------------------------------
template <class ElementT>
void StillArray<ElementT>::Save (Stream& rqStream) const
{
    rqStream.Write(m_iQuantity);
    Q_IF_EXISTS ( ElementT::Save )
    {
        // has the special member function
        for (int i = 0; i < m_iQuantity; i++)
            m_atElement[i].Save(rqStream);
    }
    Q_IF_NOT_EXISTS ( ElementT::Save )
    {
        // native or aggregate
        rqStream.Write(m_iQuantity,m_atElement);
    }
}
//------------------------------------------------------------------------------------------------------------------
template <class ElementT>
void StillArray<ElementT>::Load (Stream& rqStream)
{
    rqStream.Read(m_iQuantity);

    // allocate heap
    m_atElement = new ElementT[m_iQuantity];

    // assign heap
    Q_IF_EXISTS ( ElementT::Load )
    {
        // has the special member function
        for (int i = 0; i < m_iQuantity; i++)
            m_atElement[i].Load(rqStream);
    }
    Q_IF_NOT_EXISTS ( ElementT::Load )
    {
        // native or aggregate
        rqStream.Read(m_iQuantity,m_atElement);
    }
}
//------------------------------------------------------------------------------------------------------------------



