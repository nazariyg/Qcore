#ifndef QTIMEDURATION_H
#define QTIMEDURATION_H

// The class is for representing of temporal durations.  A time-duration is defined by a number of passed
// seconds and a fraction of passed milliseconds in the range [0, 999].

#include "QSystem.h"

namespace Q
{

class Q_ITEM TimeDuration
{
public:
    // construction
    TimeDuration (int iSecondQuantity = 0, int iMillisecondFraction = 0);

    // since 1970
    static TimeDuration GetEpochTimeDuration ();

    // member access
    int GetSecondQuantity () const;
    int GetMillisecondFraction () const;

    // Granulation into integer components.  Values to be assigned to the first parameter in each function come
    // back as not bound by the standard date limits.
    void GetAsGranulated (int& riMin, int& riSec) const;
    void GetAsGranulated (int& riHour, int& riMin, int& riSec) const;
    void GetAsGranulated (int& riDay, int& riHour, int& riMin, int& riSec) const;

    // conversion into a single-component representation
    int GetInMilliseconds () const;  // for relatively short durations
    double GetInSeconds () const;
    double GetInMinutes () const;
    double GetInHours () const;
    double GetInDays () const;

    // comparison
    bool operator== (const TimeDuration& rTD) const;
    bool operator!= (const TimeDuration& rTD) const;
    bool operator<  (const TimeDuration& rTD) const;
    bool operator<= (const TimeDuration& rTD) const;
    bool operator>  (const TimeDuration& rTD) const;
    bool operator>= (const TimeDuration& rTD) const;

    // arithmetic operations
    TimeDuration operator+ (const TimeDuration& rTD) const;
    TimeDuration operator- (const TimeDuration& rTD) const;
    TimeDuration& operator+= (const TimeDuration& rTD);
    TimeDuration& operator-= (const TimeDuration& rTD);

private:
    int m_iSecondQuantity, m_iMillisecondFraction;
};

#include "QTimeDuration.inl"

}

#endif



