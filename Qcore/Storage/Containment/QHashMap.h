#ifndef QHASHMAP_H
#define QHASHMAP_H

// The class is for mapping of type instances, called keys, to other type instances, called values; a key is
// unique within the map.  Key-value pairs, namely map records, are stored and looked-up by means of the
// hashing algorithm.  A hash code is 4 bytes in size.  The HashMap class exists as alternative to the Map
// class that may be more efficient if an instance of the key type occupies in memory more bytes than a hash
// code.  The KeyT and ValueT template parameters are either native data types or classes that have the next
// member functions:
//   Class::Class ();
//   Class::Class (const Class& ...);
//   Class& Class::operator= (const Class& ...);
// where Class is the same as KeyT or ValueT, the 'const' and '&' are optional.  As a general rule, the default
// equator to be used in searching among collided records is suitable enough; it wants the next member function
// to be maintained by KeyT:
//   bool Class::operator== (const Class& ...);
// If usage of an inter-map comparison operator is implied, the KeyT and ValueT template parameters may require
// to be equipped with one of the following member functions:
//   bool Class::operator== (const Class& ...);
//   bool Class::operator<  (const Class& ...);
// (For compatibility purposes, the class was being designed with respect to features, interface, and behavior
// peculiar to the Standard Template Library.)

#include "QStrgCnmtUtils.h"
#include "QHashMapIterator.h"

namespace Q
{

template <class KeyT, class ValueT, class HasherT = DefaultHasher<KeyT>, class EquatorT = DefaultEquator<KeyT> >
class HashMap
{

Q_DECLARE_STREAM;

public:
    typedef HashMapIterator<KeyT,ValueT,HasherT,EquatorT> IteratorT;  // iterator type
    typedef Link<KeyT,ValueT> RecordT;                                // record type

    // construction and destruction
    HashMap ();
    HashMap (const HashMap& rqMap, int iMinBucketsWanted = 0);
    ~HashMap ();

    // fullness
    int GetQuantity () const;
    bool IsEmpty () const;

    // Insertion of a record.  For the two alternative functions below, a returned pair associates an iterator
    // with key introduction success: it contains the iterator designating the inserted record with 'true' if
    // the key was new for the map and the record has been inserted, and the iterator designating the already
    // present same-key record with 'false' if such record's value has been overwritten.  NOTE. The
    // square-brackets operator is to be used for record insertion only, but not for value-by-key access.
    Link<IteratorT,bool> Insert (const KeyT& rtKey, const ValueT& rtValue);
    Link<IteratorT,bool> Insert (const RecordT& rqRecord);
    ValueT& operator[] (const KeyT& rtKey);

    // lookup
    bool IsPresent (const KeyT& rtKey) const;
    IteratorT Find (const KeyT& rtKey) const;

    // outermost record access
    IteratorT GetBegin () const;  // if the map is empty, returns the end-iterator
    IteratorT GetEnd () const;

    // Removal.  In the iterator-oriented functions below, a returned iterator designates the record that was
    // next (in the positive direction) to the last record has been removed, or it is the end-iterator if no
    // such record exists.  For the range removal function, qTo designates the non-inclusive record up to which
    // removal is to be performed.  A freed record slot will be filled by the record put together from the
    // default values.
    int Remove (const KeyT& rtKey);                                     // removes a record by its key
    IteratorT Remove (const IteratorT& rqPos);                          // removes a record by its position
    IteratorT Remove (const IteratorT& rqFrom, const IteratorT& rqTo);  // removes a range of records
    void RemoveAll ();                                                  // clears the map

    // assignment
    HashMap& operator= (const HashMap& rqMap);

    // comparison
    bool operator== (const HashMap& rqMap);
    bool operator!= (const HashMap& rqMap);
    bool operator<  (const HashMap& rqMap);
    bool operator<= (const HashMap& rqMap);
    bool operator>  (const HashMap& rqMap);
    bool operator>= (const HashMap& rqMap);

    // swapping
    void Swap (HashMap& rqMap);

private:
    template <class KeyT, class ValueT, class HasherT, class EquatorT>
    friend class HashMapIterator;

    class RecordContainer
    {
    public:
        enum RecordStateType
        {
            RST_OCCUPIED,
            RST_EMPTY,
            RST_REMOVED
        };

        RecordContainer ()
            :
            State(RST_OCCUPIED)
        {
        };
        RecordContainer (const Link<KeyT,ValueT>& rqRecord)
            :
            Record(rqRecord),
            State(RST_OCCUPIED)
        {
        };

        Link<KeyT,ValueT> Record;
        RecordStateType State;
    };

    static int ProbeJump (int iProbeQuantity);  // the probing method functions
    bool TestRemoved (int i) const;
    bool TestRemoved (const IteratorT& rqIterator) const;
    bool SetRemoved (const IteratorT& rqIterator);
    bool ClearRemoved (const IteratorT& rqIterator);
    void SquashRemoved ();
    bool TestEmpty (int i) const;
    bool TestEmpty (const IteratorT& rqIterator) const;
    void SetEmpty (int i);
    void SetEmpty (int iFrom, int iTo);
    int GetMinQuantity (int iRecordQuantity, int iMinBucketsWanted) const;
    void TryShrink ();
    void SetQuantityDelta (int iDelta, int iMinBucketsWanted = 0);
    void CopyFrom (const HashMap& rqMap, int iMinBucketsWanted = 0);
    Link<int,int> FindPosition (const KeyT& rtKey) const;
    Link<IteratorT,bool> InsertNoResize (const RecordT& rqRecord);
    void ResetThresholds ();

    int m_iQuantity, m_iBucketQuantity, m_iRemovedQuantity;
    RecordContainer* m_aqRecord;
    HasherT m_tHasher;
    EquatorT m_tEquator;
    int m_iShrinkThreshold, m_iEnlargeThreshold;
    bool m_bConsiderShrink;

    static const int ms_iMinBucketQuantity;  // 32; minimal number of buckets; must be a power of two >= 4
    static const float ms_fOccupancy;  // 0.5; indicates how full table gets before growth
    static const float ms_fEmptiness;  // 0.4*ms_fOccupancy; indicates how full table gets before shrinking
    static const int ms_iIllegalBucket;  // -1; -1 is never legal, so use it
};

#include "QHashMap.inl"

}

#endif



