//------------------------------------------------------------------------------------------------------------------
inline int TimeDuration::GetSecondQuantity () const
{
    return m_iSecondQuantity;
}
//------------------------------------------------------------------------------------------------------------------
inline int TimeDuration::GetMillisecondFraction () const
{
    return m_iMillisecondFraction;
}
//------------------------------------------------------------------------------------------------------------------
inline void TimeDuration::GetAsGranulated (int& riMin, int& riSec) const
{
    riMin = m_iSecondQuantity / 60;
    riSec = m_iSecondQuantity % 60;
}
//------------------------------------------------------------------------------------------------------------------
inline void TimeDuration::GetAsGranulated (int& riHour, int& riMin, int& riSec) const
{
    riHour = m_iSecondQuantity / 3600;
    riMin = (m_iSecondQuantity % 3600)/60;
    riSec = m_iSecondQuantity % 60;
}
//------------------------------------------------------------------------------------------------------------------
inline void TimeDuration::GetAsGranulated (int& riDay, int& riHour, int& riMin, int& riSec) const
{
    riDay = m_iSecondQuantity / 86400;
    riHour = (m_iSecondQuantity % 86400)/3600;
    riMin = (m_iSecondQuantity % 3600)/60;
    riSec = m_iSecondQuantity % 60;
}
//------------------------------------------------------------------------------------------------------------------
inline int TimeDuration::GetInMilliseconds () const
{
    return m_iSecondQuantity*1000 + m_iMillisecondFraction;
}
//------------------------------------------------------------------------------------------------------------------
inline double TimeDuration::GetInSeconds () const
{
    return m_iSecondQuantity + ((double)m_iMillisecondFraction)/1000;
}
//------------------------------------------------------------------------------------------------------------------
inline double TimeDuration::GetInMinutes () const
{
    return (m_iSecondQuantity + ((double)m_iMillisecondFraction)/1000)/60;
}
//------------------------------------------------------------------------------------------------------------------
inline double TimeDuration::GetInHours () const
{
    return (m_iSecondQuantity + ((double)m_iMillisecondFraction)/1000)/3600;
}
//------------------------------------------------------------------------------------------------------------------
inline double TimeDuration::GetInDays () const
{
    return (m_iSecondQuantity + ((double)m_iMillisecondFraction)/1000)/86400;
}
//------------------------------------------------------------------------------------------------------------------
inline bool TimeDuration::operator== (const TimeDuration& rTD) const
{
    return ( m_iSecondQuantity == rTD.m_iSecondQuantity && m_iMillisecondFraction == rTD.m_iMillisecondFraction );
}
//------------------------------------------------------------------------------------------------------------------
inline bool TimeDuration::operator!= (const TimeDuration& rTD) const
{
    return ( m_iSecondQuantity != rTD.m_iSecondQuantity || m_iMillisecondFraction != rTD.m_iMillisecondFraction );
}
//------------------------------------------------------------------------------------------------------------------
inline bool TimeDuration::operator< (const TimeDuration& rTD) const
{
    if ( m_iSecondQuantity < rTD.m_iSecondQuantity )
        return true;
    if ( m_iSecondQuantity == rTD.m_iSecondQuantity )
        return ( m_iMillisecondFraction < rTD.m_iMillisecondFraction );
    return false;
}
//------------------------------------------------------------------------------------------------------------------
inline bool TimeDuration::operator<= (const TimeDuration& rTD) const
{
    if ( m_iSecondQuantity < rTD.m_iSecondQuantity )
        return true;
    if ( m_iSecondQuantity == rTD.m_iSecondQuantity )
        return ( m_iMillisecondFraction <= rTD.m_iMillisecondFraction );
    return false;
}
//------------------------------------------------------------------------------------------------------------------
inline bool TimeDuration::operator> (const TimeDuration& rTD) const
{
    if ( m_iSecondQuantity > rTD.m_iSecondQuantity )
        return true;
    if ( m_iSecondQuantity == rTD.m_iSecondQuantity )
        return ( m_iMillisecondFraction > rTD.m_iMillisecondFraction );
    return false;
}
//------------------------------------------------------------------------------------------------------------------
inline bool TimeDuration::operator>= (const TimeDuration& rTD) const
{
    if ( m_iSecondQuantity > rTD.m_iSecondQuantity )
        return true;
    if ( m_iSecondQuantity == rTD.m_iSecondQuantity )
        return ( m_iMillisecondFraction >= rTD.m_iMillisecondFraction );
    return false;
}
//------------------------------------------------------------------------------------------------------------------
inline TimeDuration TimeDuration::operator+ (const TimeDuration& rTD) const
{
    int iSecondQuantity = m_iSecondQuantity + rTD.m_iSecondQuantity;
    int iMillisecondFraction = m_iMillisecondFraction + rTD.m_iMillisecondFraction;
    if ( iMillisecondFraction > 999 )
    {
        iSecondQuantity++;
        iMillisecondFraction -= 1000;
    }
    return TimeDuration(iSecondQuantity,iMillisecondFraction);
}
//------------------------------------------------------------------------------------------------------------------
inline TimeDuration TimeDuration::operator- (const TimeDuration& rTD) const
{
    assert( *this >= rTD );
    int iSecondQuantity = m_iSecondQuantity - rTD.m_iSecondQuantity;
    int iMillisecondFraction = m_iMillisecondFraction - rTD.m_iMillisecondFraction;
    if ( iMillisecondFraction < 0 )
    {
        iSecondQuantity--;
        assert( iSecondQuantity >= 0 );
        iMillisecondFraction += 1000;
    }
    return TimeDuration(iSecondQuantity,iMillisecondFraction);
}
//------------------------------------------------------------------------------------------------------------------
inline TimeDuration& TimeDuration::operator+= (const TimeDuration& rTD)
{
    m_iSecondQuantity += rTD.m_iSecondQuantity;
    m_iMillisecondFraction += rTD.m_iMillisecondFraction;
    if ( m_iMillisecondFraction > 999 )
    {
        m_iSecondQuantity++;
        m_iMillisecondFraction -= 1000;
    }
    return *this;
}
//------------------------------------------------------------------------------------------------------------------
inline TimeDuration& TimeDuration::operator-= (const TimeDuration& rTD)
{
    assert( *this >= rTD );
    m_iSecondQuantity -= rTD.m_iSecondQuantity;
    m_iMillisecondFraction -= rTD.m_iMillisecondFraction;
    if ( m_iMillisecondFraction < 0 )
    {
        m_iSecondQuantity--;
        assert( m_iSecondQuantity >= 0 );
        m_iMillisecondFraction += 1000;
    }
    return *this;
}
//------------------------------------------------------------------------------------------------------------------



