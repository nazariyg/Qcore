//------------------------------------------------------------------------------------------------------------------
template <class KeyT, class ValueT, class EqComparatorT>
Map<KeyT,ValueT,EqComparatorT>::Map ()
    :
    m_iQuantity(0),
    m_pqRootRecordNode(0)
{
}
//------------------------------------------------------------------------------------------------------------------
template <class KeyT, class ValueT, class EqComparatorT>
Map<KeyT,ValueT,EqComparatorT>::Map (const Map& rqMap)
{
    m_iQuantity = 0;
    *this = rqMap;
}
//------------------------------------------------------------------------------------------------------------------
template <class KeyT, class ValueT, class EqComparatorT>
Map<KeyT,ValueT,EqComparatorT>::~Map ()
{
    RemoveAll();
}
//------------------------------------------------------------------------------------------------------------------
template <class KeyT, class ValueT, class EqComparatorT>
int Map<KeyT,ValueT,EqComparatorT>::GetQuantity () const
{
    return m_iQuantity;
}
//------------------------------------------------------------------------------------------------------------------
template <class KeyT, class ValueT, class EqComparatorT>
bool Map<KeyT,ValueT,EqComparatorT>::IsEmpty () const
{
    return ( m_iQuantity == 0 );
}
//------------------------------------------------------------------------------------------------------------------
template <class KeyT, class ValueT, class EqComparatorT>
Link<MapIterator<KeyT,ValueT,EqComparatorT>,bool> Map<KeyT,ValueT,EqComparatorT>::Insert (const KeyT& rtKey,
    const ValueT& rtValue)
{
    return Insert(RecordT(rtKey,rtValue));
}
//------------------------------------------------------------------------------------------------------------------
template <class KeyT, class ValueT, class EqComparatorT>
Link<MapIterator<KeyT,ValueT,EqComparatorT>,bool> Map<KeyT,ValueT,EqComparatorT>::Insert (const RecordT& rqRecord)
{
    if ( m_iQuantity != 0 )
    {
        RecordNodeT* pqRecordNode = m_pqRootRecordNode;
        int iEqCompareResult;
        for (/**/; /**/; /**/)
        {
            iEqCompareResult = m_tEqComparator(rqRecord.First,pqRecordNode->GetElement().Record.First);
            if ( iEqCompareResult == -1 )
            {
                // less
                if ( !pqRecordNode->GetNegativeChild() )
                {
                    pqRecordNode->AttachNegativeChild(new RecordNodeT(RecordContainer(rqRecord,
                        RecordContainer::RCT_RED),pqRecordNode));
                    pqRecordNode = pqRecordNode->GetNegativeChild();
                    OnInsert(pqRecordNode);
                    m_iQuantity++;
                    return Link<IteratorT,bool>(IteratorT(this,pqRecordNode),true);
                }
                pqRecordNode = pqRecordNode->GetNegativeChild();
            }
            else if ( iEqCompareResult == 1 )
            {
                // greater
                if ( !pqRecordNode->GetPositiveChild() )
                {
                    pqRecordNode->AttachPositiveChild(new RecordNodeT(RecordContainer(rqRecord,
                        RecordContainer::RCT_RED),pqRecordNode));
                    pqRecordNode = pqRecordNode->GetPositiveChild();
                    OnInsert(pqRecordNode);
                    m_iQuantity++;
                    return Link<IteratorT,bool>(IteratorT(this,pqRecordNode),true);
                }
                pqRecordNode = pqRecordNode->GetPositiveChild();
            }
            else
            {
                // equal, overwrite
                pqRecordNode->GetElement().Record.Second = rqRecord.Second;
                return Link<IteratorT,bool>(IteratorT(this,pqRecordNode),false);
            }
        }
    }
    else
    {
        // 0: root node has no parent
        m_pqRootRecordNode = new RecordNodeT(RecordContainer(rqRecord,RecordContainer::RCT_BLACK),0);
        m_iQuantity = 1;
        return Link<IteratorT,bool>(IteratorT(this,m_pqRootRecordNode),true);
    }
}
//------------------------------------------------------------------------------------------------------------------
template <class KeyT, class ValueT, class EqComparatorT>
ValueT& Map<KeyT,ValueT,EqComparatorT>::operator[] (const KeyT& rtKey)
{
    return (*((Insert(RecordT(rtKey,ValueT()))).First)).Second;
}
//------------------------------------------------------------------------------------------------------------------
template <class KeyT, class ValueT, class EqComparatorT>
bool Map<KeyT,ValueT,EqComparatorT>::IsPresent (const KeyT& rtKey) const
{
    return ( Find(rtKey) != GetEnd() );
}
//------------------------------------------------------------------------------------------------------------------
template <class KeyT, class ValueT, class EqComparatorT>
MapIterator<KeyT,ValueT,EqComparatorT> Map<KeyT,ValueT,EqComparatorT>::Find (const KeyT& rtKey) const
{
    if ( m_iQuantity == 0 )
        return GetEnd();

    RecordNodeT* pqRecordNode = m_pqRootRecordNode;
    int iEqCompareResult;
    for (/**/; /**/; /**/)
    {
        iEqCompareResult = m_tEqComparator(rtKey,pqRecordNode->GetElement().Record.First);
        if ( iEqCompareResult == -1 )
        {
            // less
            if ( !pqRecordNode->GetNegativeChild() )
                return GetEnd();
            pqRecordNode = pqRecordNode->GetNegativeChild();
        }
        else if ( iEqCompareResult == 1 )
        {
            // greater
            if ( !pqRecordNode->GetPositiveChild() )
                return GetEnd();
            pqRecordNode = pqRecordNode->GetPositiveChild();
        }
        else
        {
            // equal
            return IteratorT(this,pqRecordNode);
        }
    }
}
//------------------------------------------------------------------------------------------------------------------
template <class KeyT, class ValueT, class EqComparatorT>
MapIterator<KeyT,ValueT,EqComparatorT> Map<KeyT,ValueT,EqComparatorT>::GetBegin () const
{
    if ( m_iQuantity != 0 )
        return IteratorT(this,m_pqRootRecordNode->GetNegativemostChild());
    else
        return IteratorT(this,0);
}
//------------------------------------------------------------------------------------------------------------------
template <class KeyT, class ValueT, class EqComparatorT>
MapIterator<KeyT,ValueT,EqComparatorT> Map<KeyT,ValueT,EqComparatorT>::GetEnd () const
{
    return IteratorT(this,0);
}
//------------------------------------------------------------------------------------------------------------------
template <class KeyT, class ValueT, class EqComparatorT>
int Map<KeyT,ValueT,EqComparatorT>::Remove (const KeyT& rtKey)
{
    IteratorT qPos = Find(rtKey);
    if ( qPos != GetEnd() )
    {
        Remove(qPos);
        return 1;
    }
    else
    {
        return 0;
    }
}
//------------------------------------------------------------------------------------------------------------------
template <class KeyT, class ValueT, class EqComparatorT>
MapIterator<KeyT,ValueT,EqComparatorT> Map<KeyT,ValueT,EqComparatorT>::Remove (const IteratorT& rqPos)
{
    assert( rqPos.m_pqRecordNode );

    IteratorT qRet = rqPos + 1;
    RecordNodeT* pqRecordNode0;
    RecordNodeT* pqRecordNode1;

    if ( !rqPos.m_pqRecordNode->GetNegativeChild()
    ||   !rqPos.m_pqRecordNode->GetPositiveChild() )
    {
        pqRecordNode1 = rqPos.m_pqRecordNode;
    }
    else
    {
        pqRecordNode1 = rqPos.m_pqRecordNode->GetPositiveChild()->GetNegativemostChild();
    }
    if ( pqRecordNode1->GetNegativeChild() )
        pqRecordNode0 = pqRecordNode1->GetNegativeChild();
    else
        pqRecordNode0 = pqRecordNode1->GetPositiveChild();

    if ( pqRecordNode0 )
        pqRecordNode0->SetParentNode(pqRecordNode1->GetParentNode());

    if ( pqRecordNode1->GetParentNode() )
    {
        if ( pqRecordNode1 == pqRecordNode1->GetParentNode()->GetNegativeChild() )
            pqRecordNode1->GetParentNode()->AttachNegativeChild(pqRecordNode0);
        else
            pqRecordNode1->GetParentNode()->AttachPositiveChild(pqRecordNode0);
    }
    else
    {
        m_pqRootRecordNode = pqRecordNode0;
    }
    if ( pqRecordNode1 != rqPos.m_pqRecordNode )
        rqPos.m_pqRecordNode->GetElement().Record = pqRecordNode1->GetElement().Record;

    if ( pqRecordNode0 && pqRecordNode1->GetElement().Color == RecordContainer::RCT_BLACK )
        OnRemove(pqRecordNode0);

    delete pqRecordNode1;
    m_iQuantity--;
    return qRet;
}
//------------------------------------------------------------------------------------------------------------------
template <class KeyT, class ValueT, class EqComparatorT>
MapIterator<KeyT,ValueT,EqComparatorT> Map<KeyT,ValueT,EqComparatorT>::Remove (const IteratorT& rqFrom,
    const IteratorT& rqTo)
{
    assert( rqFrom.m_pqRecordNode );

    IteratorT qIt = rqFrom;
    while ( qIt != rqTo )
        qIt = Remove(qIt);
    return rqTo;
}
//------------------------------------------------------------------------------------------------------------------
template <class KeyT, class ValueT, class EqComparatorT>
void Map<KeyT,ValueT,EqComparatorT>::RemoveAll ()
{
    if ( m_iQuantity != 0 )
    {
        // emulate stack
        Array<RecordNodeT*> qRecordNodeStack;
        qRecordNodeStack.Push(m_pqRootRecordNode);
        RecordNodeT* pqLast;
        while ( !qRecordNodeStack.IsEmpty() )
        {
            pqLast = qRecordNodeStack.GetLast();
            if ( pqLast->GetNegativeChild() )
            {
                qRecordNodeStack.Push(pqLast->GetNegativeChild());
            }
            else if ( pqLast->GetPositiveChild() )
            {
                qRecordNodeStack.Push(pqLast->GetPositiveChild());
            }
            else
            {
                if ( pqLast->GetParentNode() )
                {
                    // set child's pointer to 0
                    if ( pqLast == pqLast->GetParentNode()->GetNegativeChild() )
                        pqLast->GetParentNode()->DetachNegativeChild();
                    else
                        pqLast->GetParentNode()->DetachPositiveChild();
                }
                delete pqLast;
                qRecordNodeStack.Pop();
            }
        }
        m_iQuantity = 0;
        m_pqRootRecordNode = 0;
    }
}
//------------------------------------------------------------------------------------------------------------------
template <class KeyT, class ValueT, class EqComparatorT>
Map<KeyT,ValueT,EqComparatorT>& Map<KeyT,ValueT,EqComparatorT>::operator= (const Map& rqMap)
{
    if ( &rqMap == this )
        return *this;

    RemoveAll();
    m_pqRootRecordNode = new RecordNodeT(rqMap.m_pqRootRecordNode->GetElement(),0);

    // emulate stack
    Array<RecordNodeT*> qRecordNodeStack0;
    Array<RecordNodeT*> qRecordNodeStack1;
    qRecordNodeStack0.Push(m_pqRootRecordNode);
    qRecordNodeStack1.Push(rqMap.m_pqRootRecordNode);
    RecordNodeT* pqLast0;
    RecordNodeT* pqLast1;
    while ( !qRecordNodeStack1.IsEmpty() )
    {
        pqLast0 = qRecordNodeStack0.GetLast();
        pqLast1 = qRecordNodeStack1.GetLast();
        if ( pqLast1->GetNegativeChild() && !pqLast0->GetNegativeChild() )
        {
            pqLast0->AttachNegativeChild(new RecordNodeT(pqLast1->GetNegativeChild()->GetElement(),pqLast0));
            qRecordNodeStack0.Push(pqLast0->GetNegativeChild());
            qRecordNodeStack1.Push(pqLast1->GetNegativeChild());
        }
        else if ( pqLast1->GetPositiveChild() && !pqLast0->GetPositiveChild() )
        {
            pqLast0->AttachPositiveChild(new RecordNodeT(pqLast1->GetPositiveChild()->GetElement(),pqLast0));
            qRecordNodeStack0.Push(pqLast0->GetPositiveChild());
            qRecordNodeStack1.Push(pqLast1->GetPositiveChild());
        }
        else
        {
            qRecordNodeStack0.Pop();
            qRecordNodeStack1.Pop();
        }
    }
    m_iQuantity = rqMap.m_iQuantity;
    m_tEqComparator = rqMap.m_tEqComparator;
    return *this;
}
//------------------------------------------------------------------------------------------------------------------
template <class KeyT, class ValueT, class EqComparatorT>
bool Map<KeyT,ValueT,EqComparatorT>::operator== (const Map& rqMap)
{
    if ( m_iQuantity != rqMap.m_iQuantity )
        return false;

    if ( m_iQuantity != 0 )
    {
        IteratorT qPos0 = GetBegin();
        IteratorT qPos1 = rqMap.GetBegin();
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
template <class KeyT, class ValueT, class EqComparatorT>
bool Map<KeyT,ValueT,EqComparatorT>::operator!= (const Map& rqMap)
{
    return ( !(*this == rqMap) );
}
//------------------------------------------------------------------------------------------------------------------
template <class KeyT, class ValueT, class EqComparatorT>
bool Map<KeyT,ValueT,EqComparatorT>::operator< (const Map& rqMap)
{
    if ( m_iQuantity == rqMap.m_iQuantity )
    {
        if ( m_iQuantity != 0 )
        {
            IteratorT qPos0 = GetBegin();
            IteratorT qPos1 = rqMap.GetBegin();
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
    else if ( m_iQuantity < rqMap.m_iQuantity )
    {
        return true;
    }
    else
    {
        return false;
    }
}
//------------------------------------------------------------------------------------------------------------------
template <class KeyT, class ValueT, class EqComparatorT>
bool Map<KeyT,ValueT,EqComparatorT>::operator<= (const Map& rqMap)
{
    if ( m_iQuantity == rqMap.m_iQuantity )
    {
        if ( m_iQuantity != 0 )
        {
            IteratorT qPos0 = GetBegin();
            IteratorT qPos1 = rqMap.GetBegin();
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
    else if ( m_iQuantity < rqMap.m_iQuantity )
    {
        return true;
    }
    else
    {
        return false;
    }
}
//------------------------------------------------------------------------------------------------------------------
template <class KeyT, class ValueT, class EqComparatorT>
bool Map<KeyT,ValueT,EqComparatorT>::operator> (const Map& rqMap)
{
    if ( m_iQuantity == rqMap.m_iQuantity )
    {
        if ( m_iQuantity != 0 )
        {
            IteratorT qPos0 = GetBegin();
            IteratorT qPos1 = rqMap.GetBegin();
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
    else if ( m_iQuantity > rqMap.m_iQuantity )
    {
        return true;
    }
    else
    {
        return false;
    }
}
//------------------------------------------------------------------------------------------------------------------
template <class KeyT, class ValueT, class EqComparatorT>
bool Map<KeyT,ValueT,EqComparatorT>::operator>= (const Map& rqMap)
{
    if ( m_iQuantity == rqMap.m_iQuantity )
    {
        if ( m_iQuantity != 0 )
        {
            IteratorT qPos0 = GetBegin();
            IteratorT qPos1 = rqMap.GetBegin();
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
    else if ( m_iQuantity > rqMap.m_iQuantity )
    {
        return true;
    }
    else
    {
        return false;
    }
}
//------------------------------------------------------------------------------------------------------------------
template <class KeyT, class ValueT, class EqComparatorT>
void Map<KeyT,ValueT,EqComparatorT>::Swap (Map& rqMap)
{
    SwapObjects(m_iQuantity,rqMap.m_iQuantity);
    SwapObjects(m_pqRootRecordNode,rqMap.m_pqRootRecordNode);
    SwapObjects(m_tEqComparator,rqMap.m_tEqComparator);
}
//------------------------------------------------------------------------------------------------------------------
template <class KeyT, class ValueT, class EqComparatorT>
void Map<KeyT,ValueT,EqComparatorT>::RotateNodeNegative (RecordNodeT* pqRecordNode)
{
    RecordNodeT* pqPositive = pqRecordNode->GetPositiveChild();
    pqRecordNode->AttachPositiveChild(pqPositive->GetNegativeChild());

    if ( pqPositive->GetNegativeChild() )
        pqPositive->GetNegativeChild()->SetParentNode(pqRecordNode);
    if ( pqPositive )
        pqPositive->SetParentNode(pqRecordNode->GetParentNode());
    if ( pqRecordNode->GetParentNode() )
    {
        if ( pqRecordNode == pqRecordNode->GetParentNode()->GetNegativeChild() )
            pqRecordNode->GetParentNode()->AttachNegativeChild(pqPositive);
        else
            pqRecordNode->GetParentNode()->AttachPositiveChild(pqPositive);
    }
    else
    {
        m_pqRootRecordNode = pqPositive;
    }
    pqPositive->AttachNegativeChild(pqRecordNode);
    pqRecordNode->SetParentNode(pqPositive);
}
//------------------------------------------------------------------------------------------------------------------
template <class KeyT, class ValueT, class EqComparatorT>
void Map<KeyT,ValueT,EqComparatorT>::RotateNodePositive (RecordNodeT* pqRecordNode)
{
    RecordNodeT* pqNegative = pqRecordNode->GetNegativeChild();
    pqRecordNode->AttachNegativeChild(pqNegative->GetPositiveChild());
    if ( pqNegative->GetPositiveChild() )
        pqNegative->GetPositiveChild()->SetParentNode(pqRecordNode);
    if ( pqNegative )
        pqNegative->SetParentNode(pqRecordNode->GetParentNode());
    if ( pqRecordNode->GetParentNode() )
    {
        if ( pqRecordNode == pqRecordNode->GetParentNode()->GetPositiveChild() )
            pqRecordNode->GetParentNode()->AttachPositiveChild(pqNegative);
        else
            pqRecordNode->GetParentNode()->AttachNegativeChild(pqNegative);
    }
    else
    {
        m_pqRootRecordNode = pqNegative;
    }
    pqNegative->AttachPositiveChild(pqRecordNode);
    pqRecordNode->SetParentNode(pqNegative);
}
//------------------------------------------------------------------------------------------------------------------
template <class KeyT, class ValueT, class EqComparatorT>
void Map<KeyT,ValueT,EqComparatorT>::OnInsert (RecordNodeT* pqRecordNode)
{
    RecordNodeT* pqParent = pqRecordNode->GetParentNode();
    if ( pqParent )
    {
        if ( pqParent->GetElement().Color != RecordContainer::RCT_BLACK )
        {
            // parent is red
            RecordNodeT* pqGrandparent = pqParent->GetParentNode();
            RecordNodeT* pqUncle;
            bool bGNegative;
            if ( pqGrandparent->GetNegativeChild() == pqParent )
            {
                bGNegative = true;
                pqUncle = pqGrandparent->GetPositiveChild();
            }
            else
            {
                bGNegative = false;
                pqUncle = pqGrandparent->GetNegativeChild();
            }
            if ( pqUncle && pqUncle->GetElement().Color == RecordContainer::RCT_RED )
            {
                // repaint only
                pqParent->GetElement().Color = RecordContainer::RCT_BLACK;
                pqUncle->GetElement().Color = RecordContainer::RCT_BLACK;
                pqGrandparent->GetElement().Color = RecordContainer::RCT_RED;
                OnInsert(pqGrandparent);
            }
            else
            {
                // apply repaint and rotations
                bool bPNegative = ( pqRecordNode == pqParent->GetNegativeChild() ? true : false );
                if ( bGNegative != bPNegative )
                {
                    // prepeare rotation
                    if ( bPNegative )
                        RotateNodePositive(pqParent);
                    else
                        RotateNodeNegative(pqParent);
                    pqRecordNode->GetElement().Color = RecordContainer::RCT_BLACK;
                }
                else
                {
                    pqParent->GetElement().Color = RecordContainer::RCT_BLACK;
                }
                pqGrandparent->GetElement().Color = RecordContainer::RCT_RED;

                // rotate
                if ( bGNegative )
                    RotateNodePositive(pqGrandparent);
                else
                    RotateNodeNegative(pqGrandparent);
            }
        }
    }
    else
    {
        pqRecordNode->GetElement().Color = RecordContainer::RCT_BLACK;
    }
}
//------------------------------------------------------------------------------------------------------------------
template <class KeyT, class ValueT, class EqComparatorT>
void Map<KeyT,ValueT,EqComparatorT>::OnRemove (RecordNodeT* pqRecordNode)
{
    while ( pqRecordNode != m_pqRootRecordNode && pqRecordNode->GetElement().Color == RecordContainer::RCT_BLACK )
    {
        if ( pqRecordNode == pqRecordNode->GetParentNode()->GetNegativeChild() )
        {
            RecordNodeT* pqSibling = pqRecordNode->GetParentNode()->GetPositiveChild();
            if ( pqSibling->GetElement().Color == RecordContainer::RCT_RED )
            {
                pqSibling->GetElement().Color = RecordContainer::RCT_BLACK;
                pqRecordNode->GetParentNode()->GetElement().Color = RecordContainer::RCT_RED;
                RotateNodeNegative(pqRecordNode->GetParentNode());
                pqSibling = pqRecordNode->GetParentNode()->GetPositiveChild();
            }
            if ( pqSibling->GetNegativeChild()->GetElement().Color == RecordContainer::RCT_BLACK
            &&   pqSibling->GetPositiveChild()->GetElement().Color == RecordContainer::RCT_BLACK )
            {
                pqSibling->GetElement().Color = RecordContainer::RCT_RED;
                pqRecordNode = pqRecordNode->GetParentNode();
            }
            else
            {
                if ( pqSibling->GetPositiveChild()->GetElement().Color == RecordContainer::RCT_BLACK )
                {
                    pqSibling->GetNegativeChild()->GetElement().Color = RecordContainer::RCT_BLACK;
                    pqSibling->GetElement().Color = RecordContainer::RCT_RED;
                    RotateNodePositive(pqSibling);
                    pqSibling = pqRecordNode->GetParentNode()->GetPositiveChild();
                }
                pqSibling->GetElement().Color = pqRecordNode->GetParentNode()->GetElement().Color;
                pqRecordNode->GetParentNode()->GetElement().Color = RecordContainer::RCT_BLACK;
                pqSibling->GetPositiveChild()->GetElement().Color = RecordContainer::RCT_BLACK;
                RotateNodeNegative(pqRecordNode->GetParentNode());
                pqRecordNode = m_pqRootRecordNode;
            }
        }
        else
        {
            RecordNodeT* pqSibling = pqRecordNode->GetParentNode()->GetNegativeChild();
            if ( pqSibling->GetElement().Color == RecordContainer::RCT_RED )
            {
                pqSibling->GetElement().Color = RecordContainer::RCT_BLACK;
                pqRecordNode->GetParentNode()->GetElement().Color = RecordContainer::RCT_RED;
                RotateNodePositive(pqRecordNode->GetParentNode());
                pqSibling = pqRecordNode->GetParentNode()->GetNegativeChild();
            }
            if ( pqSibling->GetPositiveChild()->GetElement().Color == RecordContainer::RCT_BLACK
            &&   pqSibling->GetNegativeChild()->GetElement().Color == RecordContainer::RCT_BLACK )
            {
                pqSibling->GetElement().Color = RecordContainer::RCT_RED;
                pqRecordNode = pqRecordNode->GetParentNode();
            }
            else
            {
                if ( pqSibling->GetNegativeChild()->GetElement().Color == RecordContainer::RCT_BLACK )
                {
                    pqSibling->GetPositiveChild()->GetElement().Color = RecordContainer::RCT_BLACK;
                    pqSibling->GetElement().Color = RecordContainer::RCT_RED;
                    RotateNodeNegative(pqSibling);
                    pqSibling = pqRecordNode->GetParentNode()->GetNegativeChild();
                }
                pqSibling->GetElement().Color = pqRecordNode->GetParentNode()->GetElement().Color;
                pqRecordNode->GetParentNode()->GetElement().Color = RecordContainer::RCT_BLACK;
                pqSibling->GetNegativeChild()->GetElement().Color = RecordContainer::RCT_BLACK;
                RotateNodePositive(pqRecordNode->GetParentNode());
                pqRecordNode = m_pqRootRecordNode;
            }
        }
    }
    pqRecordNode->GetElement().Color = RecordContainer::RCT_BLACK;
}
//------------------------------------------------------------------------------------------------------------------
template <class KeyT, class ValueT, class EqComparatorT>
void Map<KeyT,ValueT,EqComparatorT>::Save (Stream& rqStream) const
{
    rqStream.Write(m_iQuantity);
    RecordNodeT* pqNode = m_pqRootRecordNode->GetNegativemostChild();
    for (int i = 0; i < m_iQuantity; i++)
    {
        pqNode->GetElement().Record.Save(rqStream);
        pqNode = pqNode->GetNextNode();
    }
}
//------------------------------------------------------------------------------------------------------------------
template <class KeyT, class ValueT, class EqComparatorT>
void Map<KeyT,ValueT,EqComparatorT>::Load (Stream& rqStream)
{
    assert( m_iQuantity == 0 );

    int iQuantity;
    rqStream.Read(iQuantity);
    RecordT qRecord;
    for (int i = 0; i < iQuantity; i++)
    {
        qRecord.Load(rqStream);
        Insert(qRecord);
    }
}
//------------------------------------------------------------------------------------------------------------------



