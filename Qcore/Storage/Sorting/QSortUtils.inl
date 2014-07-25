template <class ElementT, class ComparatorT>
const ComparatorT SortUtils<ElementT,ComparatorT>::ms_tComparator;

//------------------------------------------------------------------------------------------------------------------
template <class ElementT, class ComparatorT>
void SortUtils<ElementT,ComparatorT>::Sort (ElementT* atElement, int iFrom, int iTo)
{
    assert( iFrom >= 0 && iTo >= 0 && iFrom <= iTo );
    if ( iTo - iFrom <= 1 )
        return;

    // array size to cause entering into Insertion Sort algorithm; should be between 8 and 64
    static const int s_iShortArraySize = 16;

    int i0, i1;
    if ( iTo - iFrom <= s_iShortArraySize )
    {
        // Insertion Sort
        ElementT tSave;
        for (i0 = iFrom+1; i0 < iTo; i0++)
        {
            tSave = atElement[i0];
            for (i1 = i0-1; i1 >= iFrom && ms_tComparator(tSave,atElement[i1]); i1--)
                atElement[i1+1] = atElement[i1];
            atElement[i1+1] = tSave;
        }
        return;
    }

    // stack emulator size; should be >= 30.
    static const int s_iStackSize = 2048;

    int iFirst, iLast, iDist, iPivot;
    ElementT tSave, tPivot;
    int aiLBStack[s_iStackSize];
    int aiUBStack[s_iStackSize];
    aiLBStack[1] = iFrom;
    aiUBStack[1] = iTo - 1;
    int iStackPos = 1;
    do
    {
        iFirst = aiLBStack[iStackPos];
        iLast = aiUBStack[iStackPos];
        iStackPos--;
        do
        {
            iDist = iLast - iFirst;
            if ( iDist < s_iShortArraySize )
            {
                // Insertion Sort
                for (i0 = iFirst+1; i0 <= iLast; i0++)
                {
                    tSave = atElement[i0];
                    for (i1 = i0-1; i1 >= iFirst && ms_tComparator(tSave,atElement[i1]); i1--)
                        atElement[i1+1] = atElement[i1];
                    atElement[i1+1] = tSave;
                }
                break;
            }

            // partition
            iPivot = iFirst + iDist/2;
            tPivot = atElement[iPivot];
            for (i0 = iFirst, i1 = iLast; /**/; i0++, i1--)
            {
                while ( ms_tComparator(atElement[i0],tPivot) )
                    i0++;
                while ( ms_tComparator(tPivot,atElement[i1]) )
                    i1--;

                if ( i0 > i1 )
                    break;

                tSave = atElement[i0];
                atElement[i0] = atElement[i1];
                atElement[i1] = tSave;
            }

            // sort the smallest partition
            if ( i0 < iPivot )
            {
                if ( i0 < iLast )
                {
                    iStackPos++;
                    aiLBStack[iStackPos] = i0;
                    aiUBStack[iStackPos] = iLast;
                }
                iLast = i1;
            }
            else
            {
                if ( i1 > iFirst )
                {
                    iStackPos++;
                    aiLBStack[iStackPos] = iFirst;
                    aiUBStack[iStackPos] = i1;
                }
                iFirst = i0;
            }
        } while ( iFirst < iLast );
    } while ( iStackPos != 0 );
}
//------------------------------------------------------------------------------------------------------------------
template <class ElementT, class ComparatorT>
void SortUtils<ElementT,ComparatorT>::Sort (int iQuantity, ElementT* atElement)
{
    Sort(atElement,0,iQuantity);
}
//------------------------------------------------------------------------------------------------------------------
template <class ElementT, class ComparatorT>
void SortUtils<ElementT,ComparatorT>::Sort (StillArray<ElementT>& rqArray, int iFrom, int iTo)
{
    assert( iFrom < rqArray.GetQuantity() && iTo <= rqArray.GetQuantity() );
    Sort(rqArray.GetElements(),iFrom,iTo);
}
//------------------------------------------------------------------------------------------------------------------
template <class ElementT, class ComparatorT>
void SortUtils<ElementT,ComparatorT>::Sort (StillArray<ElementT>& rqArray)
{
    Sort(rqArray,0,rqArray.GetQuantity());
}
//------------------------------------------------------------------------------------------------------------------
template <class ElementT, class ComparatorT>
void SortUtils<ElementT,ComparatorT>::Sort (ListIterator<ElementT> qFrom, ListIterator<ElementT> qTo)
{
    ListIterator<ElementT> qLB, qUB, qLeft, qRight, qPivot;
    ElementT tSave;

    // stack emulation
    Array<Link<ListIterator<ElementT>,ListIterator<ElementT> > > qStack;
    qStack.Push(MakeLink(qFrom,qTo));
    while ( !qStack.IsEmpty() )
    {
        qLB = qStack.GetLast().First;
        qUB = qStack.GetLast().Second;
        qStack.Pop();
        if ( qLB != qUB )
        {
            qLeft = qLB;
            qRight = qUB;
            qPivot = qLeft++;
            while ( qLeft != qRight )
            {
                if ( ms_tComparator(*qLeft,*qPivot) )
                {
                    ++qLeft;
                }
                else
                {
                    while ( qLeft != --qRight && ms_tComparator(*qPivot,*qRight) )
                        /**/;

                    tSave = *qLeft;
                    *qLeft = *qRight;
                    *qRight = tSave;
                }
            }
            --qLeft;
            tSave = *qLB;
            *qLB = *qLeft;
            *qLeft = tSave;
            qStack.Push(MakeLink(qRight,qUB));
            qStack.Push(MakeLink(qLB,qLeft));
        }
    }
}
//------------------------------------------------------------------------------------------------------------------
template <class ElementT, class ComparatorT>
void SortUtils<ElementT,ComparatorT>::Sort (List<ElementT>& rqList)
{
    Sort(rqList.GetBegin(),rqList.GetEnd());
}
//------------------------------------------------------------------------------------------------------------------



