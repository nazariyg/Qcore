//------------------------------------------------------------------------------------------------------------------
inline int Mathi::Min (int iValue0, int iValue1)
{
    return ( iValue0 < iValue1 ? iValue0 : iValue1 );
}
//------------------------------------------------------------------------------------------------------------------
inline int Mathi::Max (int iValue0, int iValue1)
{
    return ( iValue0 > iValue1 ? iValue0 : iValue1 );
}
//------------------------------------------------------------------------------------------------------------------
inline int Mathi::Min3 (int iValue0, int iValue1, int iValue2)
{
    if ( iValue0 < iValue1 )
    {
        if ( iValue0 < iValue2 )
            return iValue0;
        else
            return iValue2;
    }
    else
    {
        if ( iValue1 < iValue2 )
            return iValue1;
        else
            return iValue2;
    }
}
//------------------------------------------------------------------------------------------------------------------
inline int Mathi::Max3 (int iValue0, int iValue1, int iValue2)
{
    if ( iValue0 > iValue1 )
    {
        if ( iValue0 > iValue2 )
            return iValue0;
        else
            return iValue2;
    }
    else
    {
        if ( iValue1 > iValue2 )
            return iValue1;
        else
            return iValue2;
    }
}
//------------------------------------------------------------------------------------------------------------------
inline void Mathi::MinMax (int iValue0, int iValue1, int& riMin, int& riMax)
{
    if ( iValue0 < iValue1 )
    {
        riMin = iValue0;
        riMax = iValue1;
    }
    else
    {
        riMin = iValue1;
        riMax = iValue0;
    }
}
//------------------------------------------------------------------------------------------------------------------
inline void Mathi::MinMax3 (int iValue0, int iValue1, int iValue2, int& riMin, int& riMax)
{
    if ( iValue0 < iValue1 )
    {
        riMin = iValue0;
        riMax = iValue1;
    }
    else
    {
        riMin = iValue1;
        riMax = iValue0;
    }
    if ( iValue2 < riMin )
        riMin = iValue2;
    else if ( iValue2 > riMax )
        riMax = iValue2;
}
//------------------------------------------------------------------------------------------------------------------
inline int Mathi::Sqr (int iValue)
{
    return iValue*iValue;
}
//------------------------------------------------------------------------------------------------------------------
inline int Mathi::Pow (int iValue, int iExponent)
{
    int iProd = 1;
    for (int i = 0; i < iExponent; i++)
        iProd *= iValue;
    return iProd;
}
//------------------------------------------------------------------------------------------------------------------
inline int Mathi::Sqrt (int iValue)
{
    assert( iValue >= 0 );

    static const int s_iHighestPowerOf4 = 1 << 30;

    int iSqrt = 0;
    int iPo4 = s_iHighestPowerOf4;
    int iSum;
    while ( iPo4 != 0 )
    {
        iSum = iSqrt | iPo4;
        iSqrt >>= 1;
        if ( iValue >= iSum )
        {
            iValue -= iSum;
            iSqrt |= iPo4;
        }
        iPo4 >>= 2;
    }
    return iSqrt;
}
//------------------------------------------------------------------------------------------------------------------
inline Int64 Mathi::Sqrt (Int64 iValue)
{
    assert( iValue >= 0 );

    static const Int64 s_iHighestPowerOf4 = (Int64)1 << 62;

    Int64 iSqrt = 0;
    Int64 iPo4 = s_iHighestPowerOf4;
    Int64 iSum;
    while ( iPo4 != 0 )
    {
        iSum = iSqrt | iPo4;
        iSqrt >>= 1;
        if ( iValue >= iSum )
        {
            iValue -= iSum;
            iSqrt |= iPo4;
        }
        iPo4 >>= 2;
    }
    return iSqrt;
}
//------------------------------------------------------------------------------------------------------------------
inline int Mathi::Sign (int iValue)
{
    if ( iValue > 0 )
        return 1;
    if ( iValue < 0 )
        return -1;
    return 0;
}
//------------------------------------------------------------------------------------------------------------------
inline int Mathi::Abs (int iValue)
{
    return ( iValue >= 0 ? iValue : -iValue );
}
//------------------------------------------------------------------------------------------------------------------



