#include "QDate.h"
using namespace Q;

#ifdef __APPLE__
#include <sys/time.h>
#else
#include <sys/timeb.h>
#endif

// the standard time formatting
static const char* gs_acStandardTimeFormatPattern = "%Y:%m:%d %H:%M:%S";

//------------------------------------------------------------------------------------------------------------------
Date::Date ()
{
}
//------------------------------------------------------------------------------------------------------------------
Date::Date (int iYear, int iMonth, int iDay, int iHour, int iMinute, int iSecond, int iMillisecond)
    :
    m_iYear(iYear),
    m_iMonth(iMonth),
    m_iDay(iDay),
    m_iHour(iHour),
    m_iMinute(iMinute),
    m_iSecond(iSecond),
    m_iMillisecond(iMillisecond)
{
    assert( 1970 <= iYear && iYear <= 2037 );
    assert( 1 <= iMonth && iMonth <= 12 );
    assert( 1 <= iDay && iDay <= 31 );
    assert( 0 <= iHour && iHour <= 23 );
    assert( 0 <= iMinute && iMinute <= 59 );
    assert( 0 <= iSecond && iSecond <= 59 );
    assert( 0 <= iMillisecond && iMillisecond <= 999 );

    FindOutDayOfWeek();
    FindOutEpochSeconds();
}
//------------------------------------------------------------------------------------------------------------------
Date::Date (TimeReferenceType eTimeType, long lEpochSeconds, int iMillisecond)
{
    assert( lEpochSeconds >= 0 );
    assert( 0 <= iMillisecond && iMillisecond <= 999 );

    struct tm* pqTM;
    if ( eTimeType == TRT_LOCAL )
        pqTM = localtime((const time_t*)&lEpochSeconds);
    else  // eTimeType = TRT_UTC
        pqTM = gmtime((const time_t*)&lEpochSeconds);

    m_iYear = pqTM->tm_year + 1900;
    m_iMonth = pqTM->tm_mon + 1;
    m_iDay = pqTM->tm_mday;
    m_iHour = pqTM->tm_hour;
    m_iMinute = pqTM->tm_min;
    m_iSecond = pqTM->tm_sec;
    m_iMillisecond = iMillisecond;
    m_eDayOfWeek = DowNumToEnum(pqTM->tm_wday+1);
    m_lEpochSeconds = lEpochSeconds;
}
//------------------------------------------------------------------------------------------------------------------
Date::Date (const char* acTimeString, int iMillisecond)
    :
    m_iMillisecond(iMillisecond)
{
    String qYear, qMon, qDay, qHour, qMin, qSec;
    qYear.SetLength(4);
    qMon.SetLength(2);
    qDay.SetLength(2);
    qHour.SetLength(2);
    qMin.SetLength(2);
    qSec.SetLength(2);
    for (int i = 0; i < 4; i++)
        qYear[i] = acTimeString[i];
    for (int i = 0; i < 2; i++)
        qMon[i] = acTimeString[i+5];
    for (int i = 0; i < 2; i++)
        qDay[i] = acTimeString[i+8];
    for (int i = 0; i < 2; i++)
        qHour[i] = acTimeString[i+11];
    for (int i = 0; i < 2; i++)
        qMin[i] = acTimeString[i+14];
    for (int i = 0; i < 2; i++)
        qSec[i] = acTimeString[i+17];

    m_iYear = qYear.ToInt();
    m_iMonth = qMon.ToInt();
    m_iDay = qDay.ToInt();
    m_iHour = qHour.ToInt();
    m_iMinute = qMin.ToInt();
    m_iSecond = qSec.ToInt();

    FindOutDayOfWeek();
    FindOutEpochSeconds();
}
//------------------------------------------------------------------------------------------------------------------
Date Date::GetCurrentLocal ()
{
#ifndef __APPLE__
    struct timeb qCurrent;
    ftime(&qCurrent);
    long lEpochSeconds = (long)qCurrent.time;
    int iMillisecond = (int)qCurrent.millitm;
#else
    struct timeval qCurrent;
    gettimeofday(&qCurrent,0);
    long lEpochSeconds = (long)qCurrent.tv_sec;
    int iMillisecond = (int)qCurrent.tv_usec;
#endif
    return Date(TRT_LOCAL,lEpochSeconds,iMillisecond);
}
//------------------------------------------------------------------------------------------------------------------
Date Date::GetCurrentUtc ()
{
#ifndef __APPLE__
    struct timeb qCurrent;
    ftime(&qCurrent);
    long lEpochSeconds = (long)qCurrent.time;
    int iMillisecond = (int)qCurrent.millitm;
#else
    struct timeval qCurrent;
    gettimeofday(&qCurrent,0);
    long lEpochSeconds = (long)qCurrent.tv_sec;
    int iMillisecond = (int)qCurrent.tv_usec;
#endif
    return Date(TRT_UTC,lEpochSeconds,iMillisecond);
}
//------------------------------------------------------------------------------------------------------------------
void Date::ToString (String& rqString) const
{
    struct tm qTM;
    qTM.tm_year = m_iYear - 1900;
    qTM.tm_mon = m_iMonth - 1;
    qTM.tm_mday = m_iDay;
    qTM.tm_hour = m_iHour;
    qTM.tm_min = m_iMinute;
    qTM.tm_sec = m_iSecond;
    qTM.tm_wday = DowEnumToNum(m_eDayOfWeek) - 1;

    static const int s_iDateStringMaxLength = 64;
    char* acTimeString = new char[s_iDateStringMaxLength];
    strftime(acTimeString,s_iDateStringMaxLength,gs_acStandardTimeFormatPattern,&qTM);
    rqString = acTimeString;
    delete[] acTimeString;
}
//------------------------------------------------------------------------------------------------------------------
bool Date::operator== (const Date& rqDate) const
{
    return ( m_lEpochSeconds == rqDate.m_lEpochSeconds && m_iMillisecond == rqDate.m_iMillisecond );
}
//------------------------------------------------------------------------------------------------------------------
bool Date::operator!= (const Date& rqDate) const
{
    return ( m_lEpochSeconds != rqDate.m_lEpochSeconds || m_iMillisecond != rqDate.m_iMillisecond );
}
//------------------------------------------------------------------------------------------------------------------
bool Date::operator< (const Date& rqDate) const
{
    if ( m_lEpochSeconds < rqDate.m_lEpochSeconds )
        return true;
    if ( m_lEpochSeconds == rqDate.m_lEpochSeconds )
        return ( m_iMillisecond < rqDate.m_iMillisecond );
    return false;
}
//------------------------------------------------------------------------------------------------------------------
bool Date::operator<= (const Date& rqDate) const
{
    if ( m_lEpochSeconds < rqDate.m_lEpochSeconds )
        return true;
    if ( m_lEpochSeconds == rqDate.m_lEpochSeconds )
        return ( m_iMillisecond <= rqDate.m_iMillisecond );
    return false;
}
//------------------------------------------------------------------------------------------------------------------
bool Date::operator> (const Date& rqDate) const
{
    if ( m_lEpochSeconds > rqDate.m_lEpochSeconds )
        return true;
    if ( m_lEpochSeconds == rqDate.m_lEpochSeconds )
        return ( m_iMillisecond > rqDate.m_iMillisecond );
    return false;
}
//------------------------------------------------------------------------------------------------------------------
bool Date::operator>= (const Date& rqDate) const
{
    if ( m_lEpochSeconds > rqDate.m_lEpochSeconds )
        return true;
    if ( m_lEpochSeconds == rqDate.m_lEpochSeconds )
        return ( m_iMillisecond >= rqDate.m_iMillisecond );
    return false;
}
//------------------------------------------------------------------------------------------------------------------
TimeDuration Date::operator- (const Date& rqDate) const
{
    assert( *this >= rqDate );
    int iSecondDiff = m_lEpochSeconds - rqDate.m_lEpochSeconds;
    int iMillisecondDiff = m_iMillisecond - rqDate.m_iMillisecond;
    if ( iMillisecondDiff < 0 )
    {
        iSecondDiff--;
        assert( iSecondDiff >= 0 );
        iMillisecondDiff += 1000;
    }
    return TimeDuration(iSecondDiff,iMillisecondDiff);
}
//------------------------------------------------------------------------------------------------------------------
Date Date::operator+ (const TimeDuration& rTD) const
{
    long lEpochSeconds = m_lEpochSeconds + rTD.GetSecondQuantity();
    int iMillisecond = m_iMillisecond + rTD.GetMillisecondFraction();
    if ( iMillisecond > 999 )
    {
        lEpochSeconds++;
        iMillisecond -= 1000;
    }
    struct tm* pqTM;
    pqTM = localtime((const time_t*)&lEpochSeconds);
    return Date(pqTM->tm_year+1900,pqTM->tm_mon+1,pqTM->tm_mday,pqTM->tm_hour,pqTM->tm_min,pqTM->tm_sec,
        iMillisecond);
}
//------------------------------------------------------------------------------------------------------------------
Date Date::operator- (const TimeDuration& rTD) const
{
    long lEpochSeconds = m_lEpochSeconds - rTD.GetSecondQuantity();
    int iMillisecond = m_iMillisecond - rTD.GetMillisecondFraction();
    if ( iMillisecond < 0 )
    {
        lEpochSeconds--;
        assert( lEpochSeconds >= 0 );
        iMillisecond += 1000;
    }
    struct tm* pqTM;
    pqTM = localtime((const time_t*)&lEpochSeconds);
    return Date(pqTM->tm_year+1900,pqTM->tm_mon+1,pqTM->tm_mday,pqTM->tm_hour,pqTM->tm_min,pqTM->tm_sec,
        iMillisecond);
}
//------------------------------------------------------------------------------------------------------------------
Date& Date::operator+= (const TimeDuration& rTD)
{
    long lEpochSeconds = m_lEpochSeconds + rTD.GetSecondQuantity();
    int iMillisecond = m_iMillisecond + rTD.GetMillisecondFraction();
    if ( iMillisecond > 999 )
    {
        lEpochSeconds++;
        iMillisecond -= 1000;
    }
    struct tm* pqTM;
    pqTM = localtime((const time_t*)&lEpochSeconds);

    m_iYear = pqTM->tm_year + 1900;
    m_iMonth = pqTM->tm_mon + 1;
    m_iDay = pqTM->tm_mday;
    m_iHour = pqTM->tm_hour;
    m_iMinute = pqTM->tm_min;
    m_iSecond = pqTM->tm_sec;
    m_iMillisecond = iMillisecond;
    m_eDayOfWeek = DowNumToEnum(pqTM->tm_wday+1);
    m_lEpochSeconds = lEpochSeconds;

    return *this;
}
//------------------------------------------------------------------------------------------------------------------
Date& Date::operator-= (const TimeDuration& rTD)
{
    long lEpochSeconds = m_lEpochSeconds - rTD.GetSecondQuantity();
    int iMillisecond = m_iMillisecond - rTD.GetMillisecondFraction();
    if ( iMillisecond < 0 )
    {
        lEpochSeconds--;
        assert( lEpochSeconds >= 0 );
        iMillisecond += 1000;
    }
    struct tm* pqTM;
    pqTM = localtime((const time_t*)&lEpochSeconds);

    m_iYear = pqTM->tm_year + 1900;
    m_iMonth = pqTM->tm_mon + 1;
    m_iDay = pqTM->tm_mday;
    m_iHour = pqTM->tm_hour;
    m_iMinute = pqTM->tm_min;
    m_iSecond = pqTM->tm_sec;
    m_iMillisecond = iMillisecond;
    m_eDayOfWeek = DowNumToEnum(pqTM->tm_wday+1);
    m_lEpochSeconds = lEpochSeconds;

    return *this;
}
//------------------------------------------------------------------------------------------------------------------
void Date::FindOutDayOfWeek ()
{
    // using the Zeller's rule
	int iYearTwoDFirst = m_iYear/100;
	int iYearTwoDLast;
	int iZMonth;
	if ( m_iMonth > 2 )
	{
		iYearTwoDLast = m_iYear % 100;
		iZMonth = m_iMonth - 2;
	}
	else
	{
		iYearTwoDLast = (m_iYear - 1) % 100;
		iZMonth = m_iMonth + 10;
	}
	int iF = -2*iYearTwoDFirst + iYearTwoDFirst/4 + iYearTwoDLast + iYearTwoDLast/4 + (13*iZMonth - 1)/5 +
        m_iDay;
    static const double s_dInv7 = 1.0/7;
    int iDow = iF - 7*((int)Mathd::Floor(s_dInv7*iF)) + 1;
    m_eDayOfWeek = DowNumToEnum(iDow);
}
//------------------------------------------------------------------------------------------------------------------
void Date::FindOutEpochSeconds ()
{
    struct tm qTM;
    qTM.tm_year = m_iYear - 1900;
    qTM.tm_mon = m_iMonth - 1;
    qTM.tm_mday = m_iDay;
    qTM.tm_hour = m_iHour;
    qTM.tm_min = m_iMinute;
    qTM.tm_sec = m_iSecond;
    m_lEpochSeconds = (long)mktime(&qTM);
}
//------------------------------------------------------------------------------------------------------------------
void Date::Save (Stream& rqStream) const
{
    rqStream.Write(m_iYear);
    rqStream.Write(m_iMonth);
    rqStream.Write(m_iDay);
    rqStream.Write(m_iHour);
    rqStream.Write(m_iMinute);
    rqStream.Write(m_iSecond);
    rqStream.Write(m_iMillisecond);
    int iDow = (int)m_eDayOfWeek;
    rqStream.Write(iDow);
    rqStream.Write(m_lEpochSeconds);
}
//------------------------------------------------------------------------------------------------------------------
void Date::Load (Stream& rqStream)
{
    rqStream.Read(m_iYear);
    rqStream.Read(m_iMonth);
    rqStream.Read(m_iDay);
    rqStream.Read(m_iHour);
    rqStream.Read(m_iMinute);
    rqStream.Read(m_iSecond);
    rqStream.Read(m_iMillisecond);
    int iDow;
    rqStream.Read(iDow);
    m_eDayOfWeek = (DayOfWeekType)iDow;
    rqStream.Read(m_lEpochSeconds);
}
//------------------------------------------------------------------------------------------------------------------



