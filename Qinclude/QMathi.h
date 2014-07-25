#ifndef QMATHI_H
#define QMATHI_H

#include "QSystem.h"

namespace Q
{

class Q_ITEM Mathi
{
public:
    static int Min (int iValue0, int iValue1);
    static int Max (int iValue0, int iValue1);
    static int Min3 (int iValue0, int iValue1, int iValue2);
    static int Max3 (int iValue0, int iValue1, int iValue2);
    static void MinMax (int iValue0, int iValue1, int& riMin, int& riMax);
    static void MinMax3 (int iValue0, int iValue1, int iValue2, int& riMin, int& riMax);

    static int Sqr (int iValue);
    static int Pow (int iValue, int iExponent);

    // floor(sqrt(value))
    static int Sqrt (int iValue);
    static Int64 Sqrt (Int64 iValue);

    // returns -1 if the input is negative, 0 if the input is zero, and +1 if the input is positive
    static int Sign (int iValue);

    static int Abs (int iValue);

    static const int MIN_INT;
    static const int MAX_INT;
};

#include "QMathi.inl"

}

#endif



