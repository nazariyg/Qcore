//------------------------------------------------------------------------------------------------------------------
template <class ElementT>
List<ElementT>::List ()
    :
    m_iQuantity(0),
    m_pqHeadNode(0),
    m_pqTailNode(0)
{
}
//------------------------------------------------------------------------------------------------------------------
template <class ElementT>
List<ElementT>::List (const List& rqList)
    :
    m_iQuantity(0)
{
    *this = rqList;
}
//------------------------------------------------------------------------------------------------------------------
template <class ElementT>
List<ElementT>::~List ()
{
    RemoveAll();
}
//------------------------------------------------------------------------------------------------------------------
template <class ElementT>
int List<ElementT>::GetQuantity () const
{
    return m_iQuantity;
}
//------------------------------------------------------------------------------------------------------------------
template <class ElementT>
bool List<ElementT>::IsEmpty () const
{
    return ( m_iQuantity == 0 );
}
//------------------------------------------------------------------------------------------------------------------
template <class ElementT>
void List<ElementT>::PushHead (const ElementT& rtElement)
{
    if ( m_iQuantity != 0 )
    {
        m_iQuantity++;
        m_pqHeadNode->SetPriorNode(new ElementNodeT(rtElement,0));
        m_pqHeadNode->GetPriorNode()->AttachNextNode(m_pqHeadNode);
        m_pqHeadNode = m_pqHeadNode->GetPriorNode();
    }
    else
    {
        m_iQuantity = 1;
        m_pqHeadNode = new ElementNodeT(rtElement,0);
        m_pqTailNode = m_pqHeadNode;
    }
}
//------------------------------------------------------------------------------------------------------------------
template <class ElementT>
void List<ElementT>::PushTail (const ElementT& rtElement)
{
    if ( m_iQuantity != 0 )
    {
        m_iQuantity++;
        m_pqTailNode->AttachNextNode(new ElementNodeT(rtElement,m_pqTailNode));
        m_pqTailNode = m_pqTailNode->GetNextNode();
    }
    else
    {
        m_iQuantity = 1;
        m_pqHeadNode = new ElementNodeT(rtElement,0);
        m_pqTailNode = m_pqHeadNode;
    }
}
//------------------------------------------------------------------------------------------------------------------
template <class ElementT>
ListIterator<ElementT> List<ElementT>::Insert (const IteratorT& rqPos, const ElementT& rtElement)
{
    m_iQuantity++;
    if ( rqPos.m_pqElementNode )
    {
        if ( rqPos.m_pqElementNode != m_pqHeadNode )
        {
            rqPos.m_pqElementNode->GetPriorNode()->AttachNextNode(new ElementNodeT(rtElement,
                rqPos.m_pqElementNode->GetPriorNode()));
            rqPos.m_pqElementNode->GetPriorNode()->GetNextNode()->AttachNextNode(rqPos.m_pqElementNode);
            rqPos.m_pqElementNode->SetPriorNode(rqPos.m_pqElementNode->GetPriorNode()->GetNextNode());
            return IteratorT(this,rqPos.m_pqElementNode->GetPriorNode()->GetNextNode());
        }
        else
        {
            PushHead(rtElement);
            return IteratorT(this,m_pqHeadNode);
        }
    }
    else
    {
        // iterator points to the end
        PushTail(rtElement);
        return IteratorT(this,m_pqTailNode);
    }
}
//------------------------------------------------------------------------------------------------------------------
template <class ElementT>
ElementT& List<ElementT>::GetFirst ()
{
    assert( m_iQuantity != 0 );
    return m_pqHeadNode->GetElement();
}
//------------------------------------------------------------------------------------------------------------------
template <class ElementT>
const ElementT& List<ElementT>::GetFirst () const
{
    assert( m_iQuantity != 0 );
    return m_pqHeadNode->GetElement();
}
//------------------------------------------------------------------------------------------------------------------
template <class ElementT>
ElementT& List<ElementT>::GetLast ()
{
    assert( m_iQuantity != 0 );
    return m_pqTailNode->GetElement();
}
//------------------------------------------------------------------------------------------------------------------
template <class ElementT>
const ElementT& List<ElementT>::GetLast () const
{
    assert( m_iQuantity != 0 );
    return m_pqTailNode->GetElement();
}
//------------------------------------------------------------------------------------------------------------------
template <class ElementT>
ListIterator<ElementT> List<ElementT>::GetBegin () const
{
    if ( m_iQuantity != 0 )
        return IteratorT(this,m_pqHeadNode);
    else
        return IteratorT(this,0);
}
//------------------------------------------------------------------------------------------------------------------
template <class ElementT>
ListIterator<ElementT> List<ElementT>::GetEnd () const
{
    return IteratorT(this,0);
}
//------------------------------------------------------------------------------------------------------------------
template <class ElementT>
void List<ElementT>::PopHead ()
{
    assert( m_iQuantity != 0 );

    if ( m_iQuantity != 1 )
    {
        m_iQuantity--;
        m_pqHeadNode = m_pqHeadNode->GetNextNode();
        delete m_pqHeadNode->GetPriorNode();
        m_pqHeadNode->SetPriorNode(0);
    }
    else
    {
        delete m_pqHeadNode;
        m_iQuantity = 0;
        m_pqHeadNode = 0;
        m_pqTailNode = 0;
    }
}
//------------------------------------------------------------------------------------------------------------------
template <class ElementT>
void List<ElementT>::PopTail ()
{
    assert( m_iQuantity != 0 );

    if ( m_iQuantity != 1 )
    {
        m_iQuantity--;
        m_pqTailNode = m_pqTailNode->GetPriorNode();
        delete m_pqTailNode->GetNextNode();
        m_pqTailNode->AttachNextNode(0);
    }
    else
    {
        delete m_pqHeadNode;
        m_iQuantity = 0;
        m_pqHeadNode = 0;
        m_pqTailNode = 0;
    }
}
//------------------------------------------------------------------------------------------------------------------
template <class ElementT>
ListIterator<ElementT> List<ElementT>::Remove (const IteratorT& rqPos)
{
    assert( rqPos.m_pqElementNode );

    IteratorT qRet = rqPos + 1;
    if ( m_iQuantity != 1 )
    {
        if ( rqPos.m_pqElementNode == m_pqHeadNode )
        {
            PopHead();
        }
        else if ( rqPos.m_pqElementNode == m_pqTailNode )
        {
            PopTail();
        }
        else
        {
            m_iQuantity--;
            rqPos.m_pqElementNode->GetPriorNode()->AttachNextNode(rqPos.m_pqElementNode->GetNextNode());
            rqPos.m_pqElementNode->GetNextNode()->SetPriorNode(rqPos.m_pqElementNode->GetPriorNode());
            delete rqPos.m_pqElementNode;
        }
    }
    else
    {
        delete m_pqHeadNode;
        m_iQuantity = 0;
        m_pqHeadNode = 0;
        m_pqTailNode = 0;
    }
    return qRet;
}
//------------------------------------------------------------------------------------------------------------------
template <class ElementT>
ListIterator<ElementT> List<ElementT>::Remove (const IteratorT& rqFrom, const IteratorT& rqTo)
{
    assert( rqFrom.m_pqElementNode );

    IteratorT qIt = rqFrom;
    while ( qIt != rqTo )
        qIt = Remove(qIt);
    return rqTo;
}
//------------------------------------------------------------------------------------------------------------------
template <class ElementT>
void List<ElementT>::RemoveAll ()
{
    ElementNodeT* pqNode = m_pqHeadNode;
    ElementNodeT* pqSaveNext;
    while ( pqNode )
    {
        pqSaveNext = pqNode->GetNextNode();
        delete pqNode;
        pqNode = pqSaveNext;
    }
    m_iQuantity = 0;
    m_pqHeadNode = 0;
    m_pqTailNode = 0;
}
//------------------------------------------------------------------------------------------------------------------
template <class ElementT>
void List<ElementT>::Merge (List& rqList)
{
    ElementNodeT* pqPos = rqList.m_pqHeadNode;
    ElementNodeT* pqSaveNext;
    for (int i = 0; i < rqList.m_iQuantity; i++)
    {
        PushTail(pqPos->GetElement());
        pqSaveNext = pqPos->GetNextNode();
        delete pqPos;
        pqPos = pqSaveNext;
    }
    rqList.m_iQuantity = 0;
    rqList.m_pqHeadNode = 0;
    rqList.m_pqTailNode = 0;
}
//------------------------------------------------------------------------------------------------------------------
template <class ElementT>
void List<ElementT>::Reverse ()
{
    if ( m_iQuantity > 1 )
    {
        ElementT tSave;
        IteratorT qIt0 = GetBegin();
        IteratorT qIt1 = GetEnd() - 1;
        int iQD2 = m_iQuantity/2;
        for (int i = 0; i < iQD2; i++)
        {
            tSave = *qIt0;
            *qIt0 = *qIt1;
            *qIt1 = tSave;
            ++qIt0;
            --qIt1;
        }
    }
}
//------------------------------------------------------------------------------------------------------------------
template <class ElementT>
List<ElementT>& List<ElementT>::operator= (const List& rqList)
{
    if ( &rqList == this )
        return *this;

    if ( rqList.m_iQuantity != 0 )
    {
        if ( m_iQuantity != 0 )
        {
            IteratorT qPos0 = GetBegin();
            IteratorT qPos1 = rqList.GetBegin();
            if ( m_iQuantity < rqList.m_iQuantity )
            {
                int i;
                for (i = 0; i < m_iQuantity; i++)
                {
                    *qPos0 = *qPos1;
                    ++qPos0;
                    ++qPos1;
                }
                for (/**/; i < rqList.m_iQuantity; i++)
                {
                    PushTail(*qPos1);
                    ++qPos1;
                }
            }
            else
            {
                for (int i = 0; i < rqList.m_iQuantity; i++)
                {
                    *qPos0 = *qPos1;
                    ++qPos0;
                    ++qPos1;
                }
                if ( m_iQuantity > rqList.m_iQuantity )
                    Remove(qPos0,GetEnd());
            }
        }
        else
        {
            IteratorT qPos = rqList.GetBegin();
            for (int i = 0; i < rqList.m_iQuantity; i++)
            {
                PushTail(*qPos);
                ++qPos;
            }
        }
    }
    else
    {
        RemoveAll();
    }
    return *this;
}
//------------------------------------------------------------------------------------------------------------------
template <class ElementT>
bool List<ElementT>::operator== (const List& rqList) const
{
    if ( m_iQuantity != rqList.m_iQuantity )
        return false;

    if ( m_iQuantity != 0 )
    {
        IteratorT qPos0 = GetBegin();
        IteratorT qPos1 = rqList.GetBegin();
        for (int i = 0; i < m_iQuantity; i++)
        {
            if ( !(*qPos0 == *qPos1) )
                return false;

            ++qPos0;
            ++qPos1;
        }
    }  // else: both are empty, so they are equal
    return true;
}
//------------------------------------------------------------------------------------------------------------------
template <class ElementT>
bool List<ElementT>::operator!= (const List& rqList) const
{
    return ( !(*this == rqList) );
}
//------------------------------------------------------------------------------------------------------------------
template <class ElementT>
bool List<ElementT>::operator< (const List& rqList) const
{
    if ( m_iQuantity == rqList.m_iQuantity )
    {
        if ( m_iQuantity != 0 )
        {
            IteratorT qPos0 = GetBegin();
            IteratorT qPos1 = rqList.GetBegin();
            for (int i = 0; i < m_iQuantity; i++)
            {
                if ( !(*qPos0 < *qPos1) )
                    return false;

                ++qPos0;
                ++qPos1;
            }
            return true;
        }
        else
        {
            return false;
        }
    }
    else if ( m_iQuantity < rqList.m_iQuantity )
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
bool List<ElementT>::operator<= (const List& rqList) const
{
    if ( m_iQuantity == rqList.m_iQuantity )
    {
        if ( m_iQuantity != 0 )
        {
            IteratorT qPos0 = GetBegin();
            IteratorT qPos1 = rqList.GetBegin();
            for (int i = 0; i < m_iQuantity; i++)
            {
                if ( *qPos1 < *qPos0 )
                    return false;

                ++qPos0;
                ++qPos1;
            }
        }
        return true;
    }
    else if ( m_iQuantity < rqList.m_iQuantity )
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
bool List<ElementT>::operator> (const List& rqList) const
{
    if ( m_iQuantity == rqList.m_iQuantity )
    {
        if ( m_iQuantity != 0 )
        {
            IteratorT qPos0 = GetBegin();
            IteratorT qPos1 = rqList.GetBegin();
            for (int i = 0; i < m_iQuantity; i++)
            {
                if ( !(*qPos1 < *qPos0) )
                    return false;

                ++qPos0;
                ++qPos1;
            }
            return true;
        }
        else
        {
            return false;
        }
    }
    else if ( m_iQuantity > rqList.m_iQuantity )
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
bool List<ElementT>::operator>= (const List& rqList) const
{
    if ( m_iQuantity == rqList.m_iQuantity )
    {
        if ( m_iQuantity != 0 )
        {
            IteratorT qPos0 = GetBegin();
            IteratorT qPos1 = rqList.GetBegin();
            for (int i = 0; i < m_iQuantity; i++)
            {
                if ( *qPos0 < *qPos1 )
                    return false;

                ++qPos0;
                ++qPos1;
            }
        }
        return true;
    }
    else if ( m_iQuantity > rqList.m_iQuantity )
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
void List<ElementT>::Swap (List& rqList)
{
    SwapObjects(m_iQuantity,rqList.m_iQuantity);
    SwapObjects(m_pqHeadNode,rqList.m_pqHeadNode);
    SwapObjects(m_pqTailNode,rqList.m_pqTailNode);
}
//------------------------------------------------------------------------------------------------------------------
template <class ElementT>
void List<ElementT>::Save (Stream& rqStream) const
{
    rqStream.Write(m_iQuantity);
    ElementNodeT* pNode = m_pqHeadNode;
    Q_IF_EXISTS ( ElementT::Save )
    {
        // has the special member function
        for (int i = 0; i < m_iQuantity; i++)
        {
            pNode->GetElement().Save(rqStream);
            pNode = pNode->GetNextNode();
        }
    }
    Q_IF_NOT_EXISTS ( ElementT::Save )
    {
        // native or aggregate
        for (int i = 0; i < m_iQuantity; i++)
        {
            rqStream.Write(pNode->GetElement());
            pNode = pNode->GetNextNode();
        }
    }
}
//------------------------------------------------------------------------------------------------------------------
template <class ElementT>
void List<ElementT>::Load (Stream& rqStream)
{
    assert( m_iQuantity == 0 );

    int iQuantity;
    rqStream.Read(iQuantity);
    ElementT tElement = ElementT();  // to avoid the warning "variable used without having been initialized"
    Q_IF_EXISTS ( ElementT::Load )
    {
        // has the special member function
        for (int i = 0; i < iQuantity; i++)
        {
            tElement.Load(rqStream);
            PushTail(tElement);
        }
    }
    Q_IF_NOT_EXISTS ( ElementT::Load )
    {
        // native or aggregate
        for (int i = 0; i < iQuantity; i++)
        {
            rqStream.Read(tElement);
            PushTail(tElement);
        }
    }
}
//------------------------------------------------------------------------------------------------------------------



