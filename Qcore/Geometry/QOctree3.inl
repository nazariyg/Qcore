//------------------------------------------------------------------------------------------------------------------
template <class Real, class ElementT>
Octree3<Real,ElementT>::Octree3 (const AxisAlignedBox3<Real>& rqOverallBound, int iSplitDepth)
    :
    m_qOverallBound(rqOverallBound),
    m_iSplitDepth(iSplitDepth),
    m_qOverallSplitRecord(rqOverallBound)
{
    assert( iSplitDepth >= 0 );
}
//------------------------------------------------------------------------------------------------------------------
template <class Real, class ElementT>
const AxisAlignedBox3<Real>& Octree3<Real,ElementT>::GetOverallBound () const
{
    return m_qOverallBound;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real, class ElementT>
int Octree3<Real,ElementT>::GetSplitDepth () const
{
    return m_iSplitDepth;
}
//------------------------------------------------------------------------------------------------------------------
template <class Real, class ElementT>
void Octree3<Real,ElementT>::InsertElement (const Vector3<Real>& rqGuidePoint, const ElementT& rtElement)
{
    assert( m_qOverallBound.Contains(rqGuidePoint) );

    SplitRecord* pqSplitRecord = &m_qOverallSplitRecord;
    int iDepth = 0;
    AxisAlignedBox3<Real> aqSplitBound[8];
    Real fMiddleX, fMiddleY, fMiddleZ;
    for (/**/; /**/; /**/)
    {
        if ( iDepth++ == m_iSplitDepth )
        {
            pqSplitRecord->Content.Push(rtElement);
            break;
        }

        fMiddleX = ((Real)0.5)*(pqSplitRecord->Bound.Min[0] + pqSplitRecord->Bound.Max[0]);
        fMiddleY = ((Real)0.5)*(pqSplitRecord->Bound.Min[1] + pqSplitRecord->Bound.Max[1]);
        fMiddleZ = ((Real)0.5)*(pqSplitRecord->Bound.Min[2] + pqSplitRecord->Bound.Max[2]);

        aqSplitBound[0].Min[0] = pqSplitRecord->Bound.Min[0];
        aqSplitBound[0].Max[0] = fMiddleX;
        aqSplitBound[0].Min[1] = pqSplitRecord->Bound.Min[1];
        aqSplitBound[0].Max[1] = fMiddleY;
        aqSplitBound[0].Min[2] = pqSplitRecord->Bound.Min[2];
        aqSplitBound[0].Max[2] = fMiddleZ;

        aqSplitBound[1].Min[0] = pqSplitRecord->Bound.Min[0];
        aqSplitBound[1].Max[0] = fMiddleX;
        aqSplitBound[1].Min[1] = fMiddleY;
        aqSplitBound[1].Max[1] = pqSplitRecord->Bound.Max[1];
        aqSplitBound[1].Min[2] = pqSplitRecord->Bound.Min[2];
        aqSplitBound[1].Max[2] = fMiddleZ;

        aqSplitBound[2].Min[0] = fMiddleX;
        aqSplitBound[2].Max[0] = pqSplitRecord->Bound.Max[0];
        aqSplitBound[2].Min[1] = fMiddleY;
        aqSplitBound[2].Max[1] = pqSplitRecord->Bound.Max[1];
        aqSplitBound[2].Min[2] = pqSplitRecord->Bound.Min[2];
        aqSplitBound[2].Max[2] = fMiddleZ;

        aqSplitBound[3].Min[0] = fMiddleX;
        aqSplitBound[3].Max[0] = pqSplitRecord->Bound.Max[0];
        aqSplitBound[3].Min[1] = pqSplitRecord->Bound.Min[1];
        aqSplitBound[3].Max[1] = fMiddleY;
        aqSplitBound[3].Min[2] = pqSplitRecord->Bound.Min[2];
        aqSplitBound[3].Max[2] = fMiddleZ;

        aqSplitBound[4].Min[0] = pqSplitRecord->Bound.Min[0];
        aqSplitBound[4].Max[0] = fMiddleX;
        aqSplitBound[4].Min[1] = pqSplitRecord->Bound.Min[1];
        aqSplitBound[4].Max[1] = fMiddleY;
        aqSplitBound[4].Min[2] = fMiddleZ;
        aqSplitBound[4].Max[2] = pqSplitRecord->Bound.Max[2];

        aqSplitBound[5].Min[0] = pqSplitRecord->Bound.Min[0];
        aqSplitBound[5].Max[0] = fMiddleX;
        aqSplitBound[5].Min[1] = fMiddleY;
        aqSplitBound[5].Max[1] = pqSplitRecord->Bound.Max[1];
        aqSplitBound[5].Min[2] = fMiddleZ;
        aqSplitBound[5].Max[2] = pqSplitRecord->Bound.Max[2];

        aqSplitBound[6].Min[0] = fMiddleX;
        aqSplitBound[6].Max[0] = pqSplitRecord->Bound.Max[0];
        aqSplitBound[6].Min[1] = fMiddleY;
        aqSplitBound[6].Max[1] = pqSplitRecord->Bound.Max[1];
        aqSplitBound[6].Min[2] = fMiddleZ;
        aqSplitBound[6].Max[2] = pqSplitRecord->Bound.Max[2];

        aqSplitBound[7].Min[0] = fMiddleX;
        aqSplitBound[7].Max[0] = pqSplitRecord->Bound.Max[0];
        aqSplitBound[7].Min[1] = pqSplitRecord->Bound.Min[1];
        aqSplitBound[7].Max[1] = fMiddleY;
        aqSplitBound[7].Min[2] = fMiddleZ;
        aqSplitBound[7].Max[2] = pqSplitRecord->Bound.Max[2];

        for (int i = 0; i < 8; i++)
        {
            if ( aqSplitBound[i].Contains(rqGuidePoint) )
            {
                if ( !pqSplitRecord->ChildSplits[i] )
                    pqSplitRecord->ChildSplits[i] = new SplitRecord(aqSplitBound[i]);

                pqSplitRecord = pqSplitRecord->ChildSplits[i];
                break;
            }
        }
    }
}
//------------------------------------------------------------------------------------------------------------------



