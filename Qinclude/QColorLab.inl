//------------------------------------------------------------------------------------------------------------------
inline ColorLab::operator float* ()
{
    return m_afTuple;
}
//------------------------------------------------------------------------------------------------------------------
inline ColorLab::operator const float* () const
{
    return m_afTuple;
}
//------------------------------------------------------------------------------------------------------------------
inline float& ColorLab::operator[] (int i)
{
    assert( 0 <= i && i <= 2 );
    return m_afTuple[i];
}
//------------------------------------------------------------------------------------------------------------------
inline float ColorLab::operator[] (int i) const
{
    assert( 0 <= i && i <= 2 );
    return m_afTuple[i];
}
//------------------------------------------------------------------------------------------------------------------
inline float ColorLab::L () const
{
    return m_afTuple[0];
}
//------------------------------------------------------------------------------------------------------------------
inline float& ColorLab::L ()
{
    return m_afTuple[0];
}
//------------------------------------------------------------------------------------------------------------------
inline float ColorLab::A () const
{
    return m_afTuple[1];
}
//------------------------------------------------------------------------------------------------------------------
inline float& ColorLab::A ()
{
    return m_afTuple[1];
}
//------------------------------------------------------------------------------------------------------------------
inline float ColorLab::B () const
{
    return m_afTuple[2];
}
//------------------------------------------------------------------------------------------------------------------
inline float& ColorLab::B ()
{
    return m_afTuple[2];
}
//------------------------------------------------------------------------------------------------------------------



