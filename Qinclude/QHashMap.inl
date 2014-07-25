// This page contains rewritten code of Google's hash map.  The copyright notice follows below.

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

// Copyright (c) 2005, Google Inc.
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
//     * Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above
// copyright notice, this list of conditions and the following disclaimer
// in the documentation and/or other materials provided with the
// distribution.
//     * Neither the name of Google Inc. nor the names of its
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

// minimal number of buckets; must be a power of two >= 4
template <class KeyT, class ValueT, class HasherT, class EquatorT>
const int HashMap<KeyT,ValueT,HasherT,EquatorT>::ms_iMinBucketQuantity(32);

// indicates how full table gets before growth
template <class KeyT, class ValueT, class HasherT, class EquatorT>
const float HashMap<KeyT,ValueT,HasherT,EquatorT>::ms_fOccupancy(0.5f);

// indicates how full table gets before shrinking
template <class KeyT, class ValueT, class HasherT, class EquatorT>
const float HashMap<KeyT,ValueT,HasherT,EquatorT>::ms_fEmptiness(0.4f*
HashMap<KeyT,ValueT,HasherT,EquatorT>::ms_fOccupancy);

// -1 is never legal, so use it
template <class KeyT, class ValueT, class HasherT, class EquatorT>
const int HashMap<KeyT,ValueT,HasherT,EquatorT>::ms_iIllegalBucket(-1);

//------------------------------------------------------------------------------------------------------------------
template <class KeyT, class ValueT, class HasherT, class EquatorT>
int HashMap<KeyT,ValueT,HasherT,EquatorT>::ProbeJump (int iProbeQuantity)
{
    // probing method functions
    return iProbeQuantity;  // quadratic-ish probing
    //return 1;  // linear probing
}
//------------------------------------------------------------------------------------------------------------------
template <class KeyT, class ValueT, class HasherT, class EquatorT>
HashMap<KeyT,ValueT,HasherT,EquatorT>::HashMap ()
    :
    m_iQuantity(0),
    m_iBucketQuantity(0),
    m_iRemovedQuantity(0),
    m_aqRecord(0)
{
    ResetThresholds();
}
//------------------------------------------------------------------------------------------------------------------
template <class KeyT, class ValueT, class HasherT, class EquatorT>
HashMap<KeyT,ValueT,HasherT,EquatorT>::HashMap (const HashMap& rqMap, int iMinBucketsWanted)
    :
    m_iQuantity(0),
    m_iRemovedQuantity(0),
    m_aqRecord(0),
    m_tHasher(rqMap.m_tHasher),
    m_tEquator(rqMap.m_tEquator)
{
    assert( iMinBucketsWanted >= 0 );

    m_iBucketQuantity = GetMinQuantity(0,iMinBucketsWanted);
    ResetThresholds();
    CopyFrom(rqMap,iMinBucketsWanted);
}
//------------------------------------------------------------------------------------------------------------------
template <class KeyT, class ValueT, class HasherT, class EquatorT>
HashMap<KeyT,ValueT,HasherT,EquatorT>::~HashMap ()
{
    delete[] m_aqRecord;
}
//------------------------------------------------------------------------------------------------------------------
template <class KeyT, class ValueT, class HasherT, class EquatorT>
int HashMap<KeyT,ValueT,HasherT,EquatorT>::GetQuantity () const
{
    return m_iQuantity - m_iRemovedQuantity;
}
//------------------------------------------------------------------------------------------------------------------
template <class KeyT, class ValueT, class HasherT, class EquatorT>
bool HashMap<KeyT,ValueT,HasherT,EquatorT>::IsEmpty () const
{
    return ( GetQuantity() == 0 );
}
//------------------------------------------------------------------------------------------------------------------
template <class KeyT, class ValueT, class HasherT, class EquatorT>
Link<HashMapIterator<KeyT,ValueT,HasherT,EquatorT>,bool>
HashMap<KeyT,ValueT,HasherT,EquatorT>::Insert (const KeyT& rtKey, const ValueT& rtValue)
{
    return Insert(RecordT(rtKey,rtValue));
}
//------------------------------------------------------------------------------------------------------------------
template <class KeyT, class ValueT, class HasherT, class EquatorT>
Link<HashMapIterator<KeyT,ValueT,HasherT,EquatorT>,bool>
HashMap<KeyT,ValueT,HasherT,EquatorT>::Insert (const RecordT& rqRecord)
{
    SetQuantityDelta(1);
    return InsertNoResize(rqRecord);
}
//------------------------------------------------------------------------------------------------------------------
template <class KeyT, class ValueT, class HasherT, class EquatorT>
ValueT& HashMap<KeyT,ValueT,HasherT,EquatorT>::operator[] (const KeyT& rtKey)
{
    return (*((Insert(RecordT(rtKey,ValueT()))).First)).Second;
}
//------------------------------------------------------------------------------------------------------------------
template <class KeyT, class ValueT, class HasherT, class EquatorT>
bool HashMap<KeyT,ValueT,HasherT,EquatorT>::IsPresent (const KeyT& rtKey) const
{
    Link<int,int> qRecord = FindPosition(rtKey);
    return ( qRecord.First != ms_iIllegalBucket ? true : false );
}
//------------------------------------------------------------------------------------------------------------------
template <class KeyT, class ValueT, class HasherT, class EquatorT>
HashMapIterator<KeyT,ValueT,HasherT,EquatorT> HashMap<KeyT,ValueT,HasherT,EquatorT>::Find (const KeyT& rtKey) const
{
    if ( GetQuantity() == 0 )
        return GetEnd();

    Link<int,int> qRecord = FindPosition(rtKey);
    if ( qRecord.First == ms_iIllegalBucket )
        return GetEnd();
    else
        return IteratorT(this,m_aqRecord+qRecord.First,false);
}
//------------------------------------------------------------------------------------------------------------------
template <class KeyT, class ValueT, class HasherT, class EquatorT>
HashMapIterator<KeyT,ValueT,HasherT,EquatorT> HashMap<KeyT,ValueT,HasherT,EquatorT>::GetBegin () const
{
    return IteratorT(this,m_aqRecord,true);
}
//------------------------------------------------------------------------------------------------------------------
template <class KeyT, class ValueT, class HasherT, class EquatorT>
HashMapIterator<KeyT,ValueT,HasherT,EquatorT> HashMap<KeyT,ValueT,HasherT,EquatorT>::GetEnd () const
{
    return IteratorT(this,m_aqRecord+m_iBucketQuantity,false);
}
//------------------------------------------------------------------------------------------------------------------
template <class KeyT, class ValueT, class HasherT, class EquatorT>
int HashMap<KeyT,ValueT,HasherT,EquatorT>::Remove (const KeyT& rtKey)
{
    IteratorT qPos = Find(rtKey);
    if ( qPos != GetEnd() && !TestRemoved(qPos) )
    {
        SetRemoved(qPos);
        m_iRemovedQuantity++;
        m_bConsiderShrink = true;
        return 1;
    }
    else
    {
        return 0;
    }
}
//------------------------------------------------------------------------------------------------------------------
template <class KeyT, class ValueT, class HasherT, class EquatorT>
HashMapIterator<KeyT,ValueT,HasherT,EquatorT> HashMap<KeyT,ValueT,HasherT,EquatorT>::Remove (const IteratorT& rqPos)
{
    assert( m_aqRecord <= rqPos.m_pqRecord && rqPos.m_pqRecord < m_aqRecord + m_iBucketQuantity );

    if ( SetRemoved(rqPos) )
    {
        m_iRemovedQuantity++;

        // consider shrink after next insert
        m_bConsiderShrink = true;
        return rqPos + 1;
    }
    else
    {
        return GetEnd();
    }
}
//------------------------------------------------------------------------------------------------------------------
template <class KeyT, class ValueT, class HasherT, class EquatorT>
HashMapIterator<KeyT,ValueT,HasherT,EquatorT>
HashMap<KeyT,ValueT,HasherT,EquatorT>::Remove (const IteratorT& rqFrom, const IteratorT& rqTo)
{
    assert( m_aqRecord <= rqFrom.m_pqRecord && rqFrom.m_pqRecord < m_aqRecord + m_iBucketQuantity );
    assert( m_aqRecord <= rqTo.m_pqRecord && rqTo.m_pqRecord <= m_aqRecord + m_iBucketQuantity );
    assert( rqFrom.m_pqRecord <= rqTo.m_pqRecord );

    IteratorT qIt = rqFrom;
    bool bAny = false;
    for (/**/; qIt != rqTo; ++qIt)
    {
        if ( SetRemoved(qIt) )
        {
            m_iRemovedQuantity++;
            bAny = true;
        }
    }

    // consider shrink after next insert
    m_bConsiderShrink = true;
    return ( bAny ? rqTo : GetEnd() );
}
//------------------------------------------------------------------------------------------------------------------
template <class KeyT, class ValueT, class HasherT, class EquatorT>
void HashMap<KeyT,ValueT,HasherT,EquatorT>::RemoveAll ()
{
    m_iBucketQuantity = GetMinQuantity(0,0);
    ResetThresholds();
    delete[] m_aqRecord;
    m_aqRecord = new RecordContainer[m_iBucketQuantity];
    SetEmpty(0,m_iBucketQuantity);
    m_iQuantity = 0;
    m_iRemovedQuantity = 0;
}
//------------------------------------------------------------------------------------------------------------------
template <class KeyT, class ValueT, class HasherT, class EquatorT>
HashMap<KeyT,ValueT,HasherT,EquatorT>& HashMap<KeyT,ValueT,HasherT,EquatorT>::operator= (const HashMap& rqMap)
{
    if ( &rqMap == this )
        return *this;

    RemoveAll();
    m_tHasher = rqMap.m_tHasher;
    m_tEquator = rqMap.m_tEquator;
    CopyFrom(rqMap);
    return *this;
}
//------------------------------------------------------------------------------------------------------------------
template <class KeyT, class ValueT, class HasherT, class EquatorT>
bool HashMap<KeyT,ValueT,HasherT,EquatorT>::operator== (const HashMap& rqMap)
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
template <class KeyT, class ValueT, class HasherT, class EquatorT>
bool HashMap<KeyT,ValueT,HasherT,EquatorT>::operator!= (const HashMap& rqMap)
{
    return ( !(*this == rqMap) );
}
//------------------------------------------------------------------------------------------------------------------
template <class KeyT, class ValueT, class HasherT, class EquatorT>
bool HashMap<KeyT,ValueT,HasherT,EquatorT>::operator< (const HashMap& rqMap)
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
template <class KeyT, class ValueT, class HasherT, class EquatorT>
bool HashMap<KeyT,ValueT,HasherT,EquatorT>::operator<= (const HashMap& rqMap)
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
template <class KeyT, class ValueT, class HasherT, class EquatorT>
bool HashMap<KeyT,ValueT,HasherT,EquatorT>::operator> (const HashMap& rqMap)
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
template <class KeyT, class ValueT, class HasherT, class EquatorT>
bool HashMap<KeyT,ValueT,HasherT,EquatorT>::operator>= (const HashMap& rqMap)
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
template <class KeyT, class ValueT, class HasherT, class EquatorT>
void HashMap<KeyT,ValueT,HasherT,EquatorT>::Swap (HashMap& rqMap)
{
    SwapObjects(m_iQuantity,rqMap.m_iQuantity);
    SwapObjects(m_iBucketQuantity,rqMap.m_iBucketQuantity);
    SwapObjects(m_iRemovedQuantity,rqMap.m_iRemovedQuantity);
    SwapObjects(m_aqRecord,rqMap.m_aqRecord);
    SwapObjects(m_tHasher,rqMap.m_tHasher);
    SwapObjects(m_tEquator,rqMap.m_tEquator);
    ResetThresholds();
    rqMap.ResetThresholds();
}
//------------------------------------------------------------------------------------------------------------------
template <class KeyT, class ValueT, class HasherT, class EquatorT>
bool HashMap<KeyT,ValueT,HasherT,EquatorT>::TestRemoved (int i) const
{
    return ( m_aqRecord[i].State == RecordContainer::RST_REMOVED ? true : false );
}
//------------------------------------------------------------------------------------------------------------------
template <class KeyT, class ValueT, class HasherT, class EquatorT>
bool HashMap<KeyT,ValueT,HasherT,EquatorT>::TestRemoved (const IteratorT& rqIterator) const
{
    return ( rqIterator.m_pqRecord->State == RecordContainer::RST_REMOVED ? true : false );
}
//------------------------------------------------------------------------------------------------------------------
template <class KeyT, class ValueT, class HasherT, class EquatorT>
bool HashMap<KeyT,ValueT,HasherT,EquatorT>::SetRemoved (const IteratorT& rqIterator)
{
    bool bRet = !TestRemoved(rqIterator);
    rqIterator.m_pqRecord->Record = RecordT();
    rqIterator.m_pqRecord->State = RecordContainer::RST_REMOVED;
    return bRet;
}
//------------------------------------------------------------------------------------------------------------------
template <class KeyT, class ValueT, class HasherT, class EquatorT>
bool HashMap<KeyT,ValueT,HasherT,EquatorT>::ClearRemoved (const IteratorT& rqIterator)
{
    // happens automatically
    return TestRemoved(rqIterator);
}
//------------------------------------------------------------------------------------------------------------------
template <class KeyT, class ValueT, class HasherT, class EquatorT>
void HashMap<KeyT,ValueT,HasherT,EquatorT>::SquashRemoved ()
{
    if ( m_iRemovedQuantity != 0 )
    {
        HashMap qTMap(*this);
        Swap(qTMap);
    }
    assert( m_iRemovedQuantity == 0 );
}
//------------------------------------------------------------------------------------------------------------------
template <class KeyT, class ValueT, class HasherT, class EquatorT>
bool HashMap<KeyT,ValueT,HasherT,EquatorT>::TestEmpty (int i) const
{
    return ( m_aqRecord[i].State == RecordContainer::RST_EMPTY );
}
//------------------------------------------------------------------------------------------------------------------
template <class KeyT, class ValueT, class HasherT, class EquatorT>
bool HashMap<KeyT,ValueT,HasherT,EquatorT>::TestEmpty (const IteratorT& rqIterator) const
{
    return ( rqIterator.m_pqRecord->State == RecordContainer::RST_EMPTY );
}
//------------------------------------------------------------------------------------------------------------------
template <class KeyT, class ValueT, class HasherT, class EquatorT>
void HashMap<KeyT,ValueT,HasherT,EquatorT>::SetEmpty (int i)
{
    m_aqRecord[i].State = RecordContainer::RST_EMPTY;
}
//------------------------------------------------------------------------------------------------------------------
template <class KeyT, class ValueT, class HasherT, class EquatorT>
void HashMap<KeyT,ValueT,HasherT,EquatorT>::SetEmpty (int iFrom, int iTo)
{
    for (/**/; iFrom < iTo; iFrom++)
        SetEmpty(iFrom);
}
//------------------------------------------------------------------------------------------------------------------
template <class KeyT, class ValueT, class HasherT, class EquatorT>
int HashMap<KeyT,ValueT,HasherT,EquatorT>::GetMinQuantity (int iRecordQuantity, int iMinBucketsWanted) const
{
    int iQuantity = ms_iMinBucketQuantity;
    while ( iQuantity < iMinBucketsWanted || iRecordQuantity >= iQuantity*ms_fOccupancy )
        iQuantity *= 2;
    return iQuantity;
}
//------------------------------------------------------------------------------------------------------------------
template <class KeyT, class ValueT, class HasherT, class EquatorT>
void HashMap<KeyT,ValueT,HasherT,EquatorT>::TryShrink ()
{
    assert( m_iQuantity >= m_iRemovedQuantity );

    // is a power of two
    assert( (m_iBucketQuantity & (m_iBucketQuantity - 1)) == 0 );
    assert( m_iBucketQuantity >= ms_iMinBucketQuantity );

    if ( m_iQuantity - m_iRemovedQuantity <= m_iShrinkThreshold && m_iBucketQuantity > ms_iMinBucketQuantity )
    {
        // find how much map should shrink
        int iQuantity = m_iBucketQuantity/2;
        while ( iQuantity > ms_iMinBucketQuantity && (m_iQuantity - m_iRemovedQuantity) <= iQuantity*ms_fEmptiness )
        {
            // stay a power of 2
            iQuantity >>= 1;
        }

        // do the actual resizing
        HashMap qTMap(*this,iQuantity);
        Swap(qTMap);
    }
    m_bConsiderShrink = false;
}
//------------------------------------------------------------------------------------------------------------------
template <class KeyT, class ValueT, class HasherT, class EquatorT>
void HashMap<KeyT,ValueT,HasherT,EquatorT>::SetQuantityDelta (int iDelta, int iMinBucketsWanted)
{
    if ( m_bConsiderShrink )
        TryShrink();

    if ( m_iBucketQuantity > iMinBucketsWanted && m_iQuantity + iDelta <= m_iEnlargeThreshold )
    {
        // nothing to do
        return;
    }
    int iResizeTo = GetMinQuantity(m_iQuantity+iDelta,iMinBucketsWanted);
    if ( iResizeTo > m_iBucketQuantity )
    {
        // not enough buckets
        HashMap qTMap(*this,iResizeTo);
        Swap(qTMap);
    }
}
//------------------------------------------------------------------------------------------------------------------
template <class KeyT, class ValueT, class HasherT, class EquatorT>
void HashMap<KeyT,ValueT,HasherT,EquatorT>::CopyFrom (const HashMap& rqMap, int iMinBucketsWanted)
{
    // clear and set removedQuantity to 0
    RemoveAll();

    int iResizeTo = GetMinQuantity(rqMap.GetQuantity(),iMinBucketsWanted);
    if ( iResizeTo > m_iBucketQuantity )
    {
        // not enough buckets
        RecordContainer* aqNewTable = new RecordContainer[iResizeTo];
        for (int i = 0; i < m_iBucketQuantity; i++)
            aqNewTable[i] = m_aqRecord[i];
        delete[] m_aqRecord;
        m_aqRecord = aqNewTable;

        // empty everything between
        SetEmpty(m_iBucketQuantity,iResizeTo);
        m_iBucketQuantity = iResizeTo;
        ResetThresholds();
    }

    // check if power of two
    assert( (m_iBucketQuantity & (m_iBucketQuantity - 1)) == 0 );
    for (IteratorT qIterator = rqMap.GetBegin(); qIterator != rqMap.GetEnd(); ++qIterator)
    {
        int iProbeQuantity = 0;
        int iBucketQuantity;
        int iBucketQMO = m_iBucketQuantity - 1;
        for (iBucketQuantity = m_tHasher(qIterator->First) & iBucketQMO;
            !TestEmpty(iBucketQuantity);
            iBucketQuantity = (iBucketQuantity + ProbeJump(iProbeQuantity)) & iBucketQMO)
        {
            iProbeQuantity++;
            assert( iProbeQuantity < m_iBucketQuantity );
        }
        m_aqRecord[iBucketQuantity] = *qIterator;
        m_iQuantity++;
    }
}
//------------------------------------------------------------------------------------------------------------------
template <class KeyT, class ValueT, class HasherT, class EquatorT>
Link<int,int> HashMap<KeyT,ValueT,HasherT,EquatorT>::FindPosition (const KeyT& rtKey) const
{
    int iProbeQuantity = 0;
    int iBucketQMO = m_iBucketQuantity - 1;
    int iBucketQuantity = m_tHasher(rtKey) & iBucketQMO;
    int iInsertPos = ms_iIllegalBucket;
    for (/**/; /**/; /**/)
    {
        // probe until something happens
        if ( TestEmpty(iBucketQuantity) )
        {
            // bucket is empty
            if ( iInsertPos == ms_iIllegalBucket )
            {
                // found no prior place to insert
                return Link<int,int>(ms_iIllegalBucket,iBucketQuantity);
            }
            else
            {
                return Link<int,int>(ms_iIllegalBucket,iInsertPos);
            }
        }
        else if ( TestRemoved(iBucketQuantity) )
        {
            // keep searching, but mark to insert
            if ( iInsertPos == ms_iIllegalBucket )
                iInsertPos = iBucketQuantity;
        }
        else if ( m_tEquator(rtKey,m_aqRecord[iBucketQuantity].Record.First) )
        {
            return Link<int,int>(iBucketQuantity,ms_iIllegalBucket);
        }

        // do another probe
        iProbeQuantity++;
        iBucketQuantity = (iBucketQuantity + ProbeJump(iProbeQuantity)) & iBucketQMO;
        assert( iProbeQuantity < m_iBucketQuantity );
    }
}
//------------------------------------------------------------------------------------------------------------------
template <class KeyT, class ValueT, class HasherT, class EquatorT>
Link<HashMapIterator<KeyT,ValueT,HasherT,EquatorT>,bool>
HashMap<KeyT,ValueT,HasherT,EquatorT>::InsertNoResize (const RecordT& rqRecord)
{
    Link<int,int> qRecord = FindPosition(rqRecord.First);
    if ( qRecord.First != ms_iIllegalBucket )
    {
        // record was already there, so update it; 'false': there was no insertion
        (*(m_aqRecord + qRecord.First)).Record.Second = rqRecord.Second;
        return Link<IteratorT,bool>(IteratorT(this,m_aqRecord+qRecord.First,false),false);
    }
    else
    {
        if ( TestRemoved(qRecord.Second) )
        {
            // just replace if it's been deleted
            IteratorT qDelPos(this,m_aqRecord+qRecord.Second,false);
            ClearRemoved(qDelPos);
            assert( m_iRemovedQuantity > 0 );
            m_iRemovedQuantity--;
        }
        else
        {
            // replacing an empty bucket
            m_iQuantity++;
        }
        m_aqRecord[qRecord.Second] = rqRecord;

        // 'true': there was insertion
        return Link<IteratorT,bool>(IteratorT(this,m_aqRecord+qRecord.Second,false),true);
    }
}
//------------------------------------------------------------------------------------------------------------------
template <class KeyT, class ValueT, class HasherT, class EquatorT>
void HashMap<KeyT,ValueT,HasherT,EquatorT>::ResetThresholds ()
{
    m_iEnlargeThreshold = (int)(m_iBucketQuantity*ms_fOccupancy);
    m_iShrinkThreshold = (int)(m_iBucketQuantity*ms_fEmptiness);
    m_bConsiderShrink = false;
}
//------------------------------------------------------------------------------------------------------------------
template <class KeyT, class ValueT, class HasherT, class EquatorT>
void HashMap<KeyT,ValueT,HasherT,EquatorT>::Save (Stream& rqStream) const
{
    rqStream.Write(m_iQuantity);
    IteratorT qIt = GetBegin();
    for (int i = 0; i < m_iQuantity; i++)
    {
        qIt->Save(rqStream);
        ++qIt;
    }
}
//------------------------------------------------------------------------------------------------------------------
template <class KeyT, class ValueT, class HasherT, class EquatorT>
void HashMap<KeyT,ValueT,HasherT,EquatorT>::Load (Stream& rqStream)
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



