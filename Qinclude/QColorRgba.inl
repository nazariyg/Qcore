//------------------------------------------------------------------------------------------------------------------
inline ColorRgba::operator float* ()
{
    return m_afTuple;
}
//------------------------------------------------------------------------------------------------------------------
inline ColorRgba::operator const float* () const
{
    return m_afTuple;
}
//------------------------------------------------------------------------------------------------------------------
inline float& ColorRgba::operator[] (int i)
{
    assert( 0 <= i && i <= 3 );
    return m_afTuple[i];
}
//------------------------------------------------------------------------------------------------------------------
inline float ColorRgba::operator[] (int i) const
{
    assert( 0 <= i && i <= 3 );
    return m_afTuple[i];
}
//------------------------------------------------------------------------------------------------------------------
inline float ColorRgba::R () const
{
    return m_afTuple[0];
}
//------------------------------------------------------------------------------------------------------------------
inline float& ColorRgba::R ()
{
    return m_afTuple[0];
}
//------------------------------------------------------------------------------------------------------------------
inline float ColorRgba::G () const
{
    return m_afTuple[1];
}
//------------------------------------------------------------------------------------------------------------------
inline float& ColorRgba::G ()
{
    return m_afTuple[1];
}
//------------------------------------------------------------------------------------------------------------------
inline float ColorRgba::B () const
{
    return m_afTuple[2];
}
//------------------------------------------------------------------------------------------------------------------
inline float& ColorRgba::B ()
{
    return m_afTuple[2];
}
//------------------------------------------------------------------------------------------------------------------
inline float ColorRgba::A () const
{
    return m_afTuple[3];
}
//------------------------------------------------------------------------------------------------------------------
inline float& ColorRgba::A ()
{
    return m_afTuple[3];
}
//------------------------------------------------------------------------------------------------------------------



