#include "QTimeDuration.h"
using namespace Q;

#ifdef __APPLE__
#include <sys/time.h>
#else
#include <sys/timeb.h>
#endif

//------------------------------------------------------------------------------------------------------------------
TimeDuration::TimeDuration (int iSecondQuantity, int iMillisecondFraction)
    :
    m_iSecondQuantity(iSecondQuantity),
    m_iMillisecondFraction(iMillisecondFraction)
{
    assert( iSecondQuantity >= 0 && 0 <= iMillisecondFraction && iMillisecondFraction <= 999 );
}
//------------------------------------------------------------------------------------------------------------------
TimeDuration TimeDuration::GetEpochTimeDuration ()
{
#ifdef __APPLE__
    struct timeval qCurrent;
    gettimeofday(&qCurrent,0);
    long lUtc = (long)qCurrent.tv_sec;
    int iMillisecond = (int)qCurrent.tv_usec;
#else
    struct timeb qCurrent;
    ftime(&qCurrent);
    long lUtc = (long)qCurrent.time;
    int iMillisecond = (int)qCurrent.millitm;
#endif
    return TimeDuration(lUtc,iMillisecond);
}
//------------------------------------------------------------------------------------------------------------------



