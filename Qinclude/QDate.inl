//------------------------------------------------------------------------------------------------------------------
inline int Date::GetYear () const
{
    return m_iYear;
}
//------------------------------------------------------------------------------------------------------------------
inline int Date::GetMonth () const
{
    return m_iMonth;
}
//------------------------------------------------------------------------------------------------------------------
inline int Date::GetDay () const
{
    return m_iDay;
}
//------------------------------------------------------------------------------------------------------------------
inline int Date::GetHour () const
{
    return m_iHour;
}
//------------------------------------------------------------------------------------------------------------------
inline int Date::GetMinute () const
{
    return m_iMinute;
}
//------------------------------------------------------------------------------------------------------------------
inline int Date::GetSecond () const
{
    return m_iSecond;
}
//------------------------------------------------------------------------------------------------------------------
inline int Date::GetMillisecond () const
{
    return m_iMillisecond;
}
//------------------------------------------------------------------------------------------------------------------
inline Date::DayOfWeekType Date::GetDayOfWeek () const
{
    return m_eDayOfWeek;
}
//------------------------------------------------------------------------------------------------------------------
inline long Date::GetEpochSeconds () const
{
    return m_lEpochSeconds;
}
//------------------------------------------------------------------------------------------------------------------
inline Date::DayOfWeekType Date::DowNumToEnum (int iDow) const
{
    switch ( iDow )
    {
    case 1:
        return DOWT_SUNDAY;
    case 2:
        return DOWT_MONDAY;
    case 3:
        return DOWT_TUESDAY;
    case 4:
        return DOWT_WEDNESDAY;
    case 5:
        return DOWT_THURSDAY;
    case 6:
        return DOWT_FRIDAY;
    default:  // 7
        return DOWT_SATURDAY;
    }
}
//------------------------------------------------------------------------------------------------------------------
inline int Date::DowEnumToNum (DayOfWeekType eDow) const
{
    switch ( eDow )
    {
    case DOWT_SUNDAY:
        return 1;
    case DOWT_MONDAY:
        return 2;
    case DOWT_TUESDAY:
        return 3;
    case DOWT_WEDNESDAY:
        return 4;
    case DOWT_THURSDAY:
        return 5;
    case DOWT_FRIDAY:
        return 6;
    default:  // DOWT_SATURDAY
        return 7;
    }
}
//------------------------------------------------------------------------------------------------------------------



