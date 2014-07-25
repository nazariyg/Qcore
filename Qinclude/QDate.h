#ifndef QDATE_H
#define QDATE_H

// The Date class serves for memorizing of time instants.  In addition to the seven time levels, a valid Date
// object also knows the corresponding day-of-week and the number of seconds elapsed since January 1, 1970,
// 00:00.  Values of the time levels are in the ranges given below.
//   year:        [1970, 2037]
//   month:       [1, 12]
//   day:         [1, 31]
//   hour:        [0, 23]
//   minute:      [0, 59]
//   second:      [0, 59]
//   millisecond: [0, 999]

#include "QString.h"
#include "QTimeDuration.h"

namespace Q
{

class Q_ITEM Date
{

Q_DECLARE_STREAM;

public:
    enum TimeReferenceType
    {
        TRT_LOCAL,
        TRT_UTC
    };

    enum DayOfWeekType
    {
        DOWT_SUNDAY,
        DOWT_MONDAY,
        DOWT_TUESDAY,
        DOWT_WEDNESDAY,
        DOWT_THURSDAY,
        DOWT_FRIDAY,
        DOWT_SATURDAY
    };

    // Construction.  As uninitialized.
    Date ();

    // Construction.  From the time levels.
    Date (int iYear, int iMonth, int iDay, int iHour, int iMinute, int iSecond, int iMillisecond = 0);

    // Construction.  From a number of seconds elapsed since January 1, 1970 00:00, relatively to either local
    // or UTC time.
    Date (TimeReferenceType eTimeType, long lEpochSeconds, int iMillisecond = 0);

    // Construction.  From a textual string (not necessary null-terminated) formatted in the 19-char standard:
    // yyyy:mm:dd hh:mm:ss
    Date (const char* acTimeString, int iMillisecond = 0);

    // getting of a current time
    static Date GetCurrentLocal ();
    static Date GetCurrentUtc ();

    // member access
    int GetYear () const;
    int GetMonth () const;
    int GetDay () const;
    int GetHour () const;
    int GetMinute () const;
    int GetSecond () const;
    int GetMillisecond () const;
    DayOfWeekType GetDayOfWeek () const;
    long GetEpochSeconds () const;

    // transformation into the textual string formatted in the 19-char standard:
    // yyyy:mm:dd hh:mm:ss
    void ToString (String& rqString) const;

    // comparison
    bool operator== (const Date& rqDate) const;
    bool operator!= (const Date& rqDate) const;
    bool operator<  (const Date& rqDate) const;
    bool operator<= (const Date& rqDate) const;
    bool operator>  (const Date& rqDate) const;
    bool operator>= (const Date& rqDate) const;

    // time duration since another date
    TimeDuration operator- (const Date& rqDate) const;

    // arithmetics with time duration
    Date operator+ (const TimeDuration& rTD) const;
    Date operator- (const TimeDuration& rTD) const;

    // updates with time duration
    Date& operator+= (const TimeDuration& rTD);
    Date& operator-= (const TimeDuration& rTD);

private:
    DayOfWeekType DowNumToEnum (int iDow) const;
    int DowEnumToNum (DayOfWeekType eDow) const;
    void FindOutDayOfWeek ();
    void FindOutEpochSeconds ();

    int m_iYear, m_iMonth, m_iDay, m_iHour, m_iMinute, m_iSecond, m_iMillisecond;
    DayOfWeekType m_eDayOfWeek;
    long m_lEpochSeconds;
};

#include "QDate.inl"

}

#endif



