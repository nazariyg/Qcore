#ifndef QMAP_H
#define QMAP_H

// The class is for mapping of type instances, called keys, to other type instances, called values; a key is
// unique within the map.  Key-value pairs, namely map records, are stored and looked-up by means of the
// red-black tree algorithm.  The Map class exists as alternative to the HashMap class that has more efficient
// behavior if an instance of the key type occupies in memory 4 or less bytes.  The KeyT and ValueT template
// parameters are either native data types or classes that have the next member function:
//   Class::Class (const Class& ...);
// where Class is the same as KeyT or ValueT, the 'const' and '&' are optional.  ValueT additionally needs the
// next one:
//   Class& Class::operator= (const Class& ...);
// As a general rule, the default eq-comparator to be used by the red-black tree algorithm is suitable enough;
// it wants the next member functions to be maintained by KeyT:
//   bool Class::operator== (const Class& ...);
//   bool Class::operator<  (const Class& ...);
// If usage of an inter-map comparison operator is implied, the KeyT and ValueT template parameters may require
// to be equipped with one of the following member functions:
//   bool Class::operator== (const Class& ...);
//   bool Class::operator<  (const Class& ...);
// (For compatibility purposes, the class was being designed with respect to features, interface, and behavior
// peculiar to the Standard Template Library.)

#include "QArray.h"
#include "QBspNode.h"
#include "QMapIterator.h"

namespace Q
{

template <class KeyT, class ValueT, class EqComparatorT = DefaultEqComparator<KeyT> >
class Map
{

Q_DECLARE_STREAM;

public:
    typedef MapIterator<KeyT,ValueT,EqComparatorT> IteratorT;  // iterator type
    typedef Link<KeyT,ValueT> RecordT;                         // record type

    // construction and destruction
    Map ();
    Map (const Map& rqMap);
    ~Map ();

    // fullness
    int GetQuantity () const;
    bool IsEmpty () const;

    // Insertion of a record.  For the two alternative functions below, a returned pair associates an iterator
    // with key introduction success: it contains the iterator designating the inserted record with 'true' if
    // the key was new for the map and the record has been inserted, and the iterator designating the already
    // present same-key record with 'false' if such record's value has been overwritten but the tree has not
    // changed.  NOTE. The square-brackets operator is to be used for record insertion only, but not for
    // value-by-key access.
    Link<IteratorT,bool> Insert (const KeyT& rtKey, const ValueT& rtValue);
    Link<IteratorT,bool> Insert (const RecordT& rqRecord);
    ValueT& operator[] (const KeyT& rtKey);

    // lookup
    bool IsPresent (const KeyT& rtKey) const;
    IteratorT Find (const KeyT& rtKey) const;

    // outermost record access
    IteratorT GetBegin () const;  // if the map is empty, returns the end-iterator
    IteratorT GetEnd () const;

    // Removal.  In the iterator-oriented functions below a returned iterator designates the record that was
    // next (in the positive direction) to the last record has been removed, or it is the end-iterator if no
    // such record exists.  For the range removal function, qTo designates the non-inclusive record up to which
    // removal is to be performed.
    int Remove (const KeyT& rtKey);                                     // removes a record by its key
    IteratorT Remove (const IteratorT& rqPos);                          // removes a record by its position
    IteratorT Remove (const IteratorT& rqFrom, const IteratorT& rqTo);  // removes a range of records
    void RemoveAll ();                                                  // clears the map

    // assignment
    Map& operator= (const Map& rqMap);

    // comparison
    bool operator== (const Map& rqMap);
    bool operator!= (const Map& rqMap);
    bool operator<  (const Map& rqMap);
    bool operator<= (const Map& rqMap);
    bool operator>  (const Map& rqMap);
    bool operator>= (const Map& rqMap);

    // swapping
    void Swap (Map& rqMap);

private:
    template <class KeyT, class ValueT, class EqComparatorT>
    friend class MapIterator;

    class RecordContainer
    {
    public:
        enum RecordColorType
        {
            RCT_RED,
            RCT_BLACK
        };

        RecordContainer (const Link<KeyT,ValueT>& rqRecord, RecordColorType eColor)
            :
            Record(rqRecord),
            Color(eColor)
        {
        };

        Link<KeyT,ValueT> Record;
        RecordColorType Color;
    };
    typedef BspNode<RecordContainer> RecordNodeT;

    void RotateNodeNegative (RecordNodeT* pqRecordNode);
    void RotateNodePositive (RecordNodeT* pqRecordNode);
    void OnInsert (RecordNodeT* pqRecordNode);
    void OnRemove (RecordNodeT* pqRecordNode);

    int m_iQuantity;
    RecordNodeT* m_pqRootRecordNode;
    EqComparatorT m_tEqComparator;
};

#include "QMap.inl"

}

#endif



