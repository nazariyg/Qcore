#include "QString.h"
using namespace Q;

// buffer size for transformations
const int String::ms_iTrnMaxChars(64);

//------------------------------------------------------------------------------------------------------------------
String::String (const char* acText)
{
    if ( acText )
    {
        m_iLength = (int)StrLen(acText);
        m_acText = new char[m_iLength+1];
        StrNCpy(m_acText,acText,m_iLength);
        m_acText[m_iLength] = 0;
    }
    else
    {
        m_iLength = 0;
        m_acText = new char[1];
        m_acText[0] = 0;
    }
}
//------------------------------------------------------------------------------------------------------------------
String::String (const String& rqString)
{
    m_acText = 0;
    *this = rqString;
}
//------------------------------------------------------------------------------------------------------------------
String::String (int iNum)
{
    char* acNum = new char[ms_iTrnMaxChars];
    sprintf(acNum,"%i",iNum);
    m_iLength = (int)StrLen(acNum);
    m_acText = new char[m_iLength+1];
    StrNCpy(m_acText,acNum,m_iLength);
    m_acText[m_iLength] = 0;
    delete[] acNum;
}
//------------------------------------------------------------------------------------------------------------------
String::String (double dNum, bool bExp)
{
    char* acNum = new char[ms_iTrnMaxChars];
    if ( !bExp )
    {
        sprintf(acNum,"%#f",dNum);
        m_iLength = (int)StrLen(acNum);

        // cut off trailing zero-chars
        while ( acNum[m_iLength-1] == '0' )
            m_iLength--;
        if ( acNum[m_iLength-1] == '.' )
            m_iLength++;

        m_acText = new char[m_iLength+1];
        StrNCpy(m_acText,acNum,m_iLength);
        m_acText[m_iLength] = 0;
    }
    else
    {
        sprintf(acNum,"%#e",dNum);
        int iLength = (int)StrLen(acNum);
        int iLengthB = iLength - 5;

        // cut off trailing zero-chars
        while ( acNum[iLengthB-1] == '0' )
            iLengthB--;
        if ( acNum[iLengthB-1] == '.' )
            iLengthB++;

        int iLM3 = iLength - 3;
        m_iLength = iLengthB + ( acNum[iLM3] == '0' ? 4 : 5 );
        m_acText = new char[m_iLength+1];
        int i0;
        for (i0 = 0; i0 < iLengthB; i0++)
            m_acText[i0] = acNum[i0];
        for (int i1 = iLength-5; i1 < iLength; i1++)
        {
            if ( !(i1 == iLM3 && acNum[iLM3] == '0') )
                m_acText[i0++] = acNum[i1];
        }
        m_acText[m_iLength] = 0;
    }
    delete[] acNum;
}
//------------------------------------------------------------------------------------------------------------------
String::~String ()
{
    delete[] m_acText;
}
//------------------------------------------------------------------------------------------------------------------
void String::SetLength (int iNewLength)
{
    assert( iNewLength >= 0 );

    char* acNewText = new char[iNewLength+1];
    StrNCpy(acNewText,m_acText,( iNewLength < m_iLength ? iNewLength : m_iLength ));
    acNewText[iNewLength] = 0;
    m_iLength = iNewLength;
    delete[] m_acText;
    m_acText = acNewText;
}
//------------------------------------------------------------------------------------------------------------------
String String::operator+ (const String& rqString) const
{
    int iNewLength = m_iLength + rqString.m_iLength;
    char* acNew = new char[iNewLength+1];
    StrNCpy(acNew,m_acText,m_iLength);
    StrNCpy(acNew+m_iLength,rqString.m_acText,rqString.m_iLength);
    acNew[iNewLength] = 0;
    String qNew(acNew);
    delete[] acNew;
    return qNew;
}
//------------------------------------------------------------------------------------------------------------------
String& String::operator+= (const String& rqString)
{
    int iNewLength = m_iLength + rqString.m_iLength;
    char* acNew = new char[iNewLength+1];
    StrNCpy(acNew,m_acText,m_iLength);
    StrNCpy(acNew+m_iLength,rqString.m_acText,rqString.m_iLength);
    acNew[iNewLength] = 0;
    m_iLength = iNewLength;
    delete[] m_acText;
    m_acText = acNew;
    return *this;
}
//------------------------------------------------------------------------------------------------------------------
void String::Insert (int i, const String& rqString)
{
    assert( 0 <= i && i <= m_iLength );

    int iNewLength = m_iLength + rqString.m_iLength;
    char* acNew = new char[iNewLength+1];
    StrNCpy(acNew,m_acText,i);
    StrNCpy(acNew+i,rqString.m_acText,rqString.m_iLength);
    StrNCpy(acNew+i+rqString.m_iLength,m_acText+i,m_iLength-i);
    acNew[iNewLength] = 0;
    m_iLength = iNewLength;
    delete[] m_acText;
    m_acText = acNew;
}
//------------------------------------------------------------------------------------------------------------------
bool String::Trim ()
{
    int iTrailingSymbolQuantity = 0;
    for (int i = m_iLength-1; i >= 0; i--)
    {
        if ( m_acText[i] == ' ' || m_acText[i] == 0 )
            iTrailingSymbolQuantity++;
        else
            break;
    }
    SetLength(m_iLength-iTrailingSymbolQuantity);
    return ( iTrailingSymbolQuantity != 0 );
}
//------------------------------------------------------------------------------------------------------------------
void String::Remove (int i, int iLength)
{
    if ( iLength == -1 )
        iLength = m_iLength - i;

    assert( 0 <= i && i < m_iLength && iLength >= 0 && i + iLength <= m_iLength );

    int iNewLength = m_iLength - iLength;
    char* acNew = new char[iNewLength+1];
    StrNCpy(acNew,m_acText,i);
    int iSpan = i + iLength;
    StrNCpy(acNew+i,m_acText+iSpan,m_iLength-iSpan);
    acNew[iNewLength] = 0;
    m_iLength = iNewLength;
    delete[] m_acText;
    m_acText = acNew;
}
//------------------------------------------------------------------------------------------------------------------
void String::Erase ()
{
    m_iLength = 0;
    delete[] m_acText;
    m_acText = new char[1];
    m_acText[0] = 0;
}
//------------------------------------------------------------------------------------------------------------------
void String::ToLowerCase ()
{
    for (int i = 0; i < m_iLength; i++)
        m_acText[i] = (char)ToLower(m_acText[i]);
}
//------------------------------------------------------------------------------------------------------------------
void String::ToUpperCase ()
{
    for (int i = 0; i < m_iLength; i++)
        m_acText[i] = (char)ToUpper(m_acText[i]);
}
//------------------------------------------------------------------------------------------------------------------
int String::ToInt () const
{
    return atoi(m_acText);
}
//------------------------------------------------------------------------------------------------------------------
double String::ToDouble () const
{
    return atof(m_acText);
}
//------------------------------------------------------------------------------------------------------------------
String String::GetSubstringByLength (int i, int iLength) const
{
    if ( iLength == -1 )
        iLength = m_iLength - i;

    assert( 0 <= i && i < m_iLength && iLength >= 0 && i + iLength <= m_iLength );

    String qString;
    delete[] qString.m_acText;
    qString.m_acText = new char[iLength+1];
    StrNCpy(qString.m_acText,m_acText+i,iLength);
    qString.m_acText[iLength] = 0;
    qString.m_iLength = iLength;
    return qString;
}
//------------------------------------------------------------------------------------------------------------------
String String::GetSubstringByRange (int iFrom, int iTo) const
{
    assert( 0 <= iFrom && iFrom < m_iLength && 0 <= iTo && iTo <= m_iLength && iFrom <= iTo );

    String qString;
    delete[] qString.m_acText;
    int iLength = iTo - iFrom;
    qString.m_acText = new char[iLength+1];
    StrNCpy(qString.m_acText,m_acText+iFrom,iLength);
    qString.m_acText[iLength] = 0;
    qString.m_iLength = iLength;
    return qString;
}
//------------------------------------------------------------------------------------------------------------------
void String::GetSplitsByChar (char cDelimiter, Array<String>& rqSplits) const
{
    int iLength;
    char* acSub;
    char* pcFResult = StrChr(m_acText,cDelimiter);
    char* pcStart = m_acText;
    while ( pcFResult )
    {
        iLength = (int)(pcFResult - pcStart);
        acSub = new char[iLength+1];
        StrNCpy(acSub,pcStart,iLength);
        acSub[iLength] = 0;
        rqSplits.Push(String(acSub));
        delete[] acSub;
        pcStart = pcFResult + 1;
        pcFResult = StrChr(pcStart,cDelimiter);
    }

    if ( pcFResult )
        iLength = (int)(pcFResult - pcStart);
    else
        iLength = m_iLength;
    acSub = new char[iLength+1];
    StrNCpy(acSub,pcStart,iLength);
    acSub[iLength] = 0;
    rqSplits.Push(String(acSub));
    delete[] acSub;
}
//------------------------------------------------------------------------------------------------------------------
void String::GetSplitsByCharSet (const String& rqDelimiterSet, Array<String>& rqSplits) const
{
    int iLength;
    char* acSub;
    char* pcFResult = StrPBrk(m_acText,rqDelimiterSet.m_acText);
    char* pcStart = m_acText;
    while ( pcFResult )
    {
        iLength = (int)(pcFResult - pcStart);
        acSub = new char[iLength+1];
        StrNCpy(acSub,pcStart,iLength);
        acSub[iLength] = 0;
        rqSplits.Push(String(acSub));
        delete[] acSub;
        pcStart = pcFResult + 1;
        pcFResult = StrPBrk(pcStart,rqDelimiterSet.m_acText);
    }

    if ( pcFResult )
        iLength = (int)(pcFResult - pcStart);
    else
        iLength = m_iLength;
    acSub = new char[iLength+1];
    StrNCpy(acSub,pcStart,iLength);
    acSub[iLength] = 0;
    rqSplits.Push(String(acSub));
    delete[] acSub;
}
//------------------------------------------------------------------------------------------------------------------
void String::GetSplitsByString (const String& rqDelimiter, Array<String>& rqSplits) const
{
    if ( rqDelimiter.m_iLength != 0 )
    {
        int iLength;
        char* acSub;
        char* pcFResult = StrStr(m_acText,rqDelimiter.m_acText);
        char* pcStart = m_acText;
        while ( pcFResult )
        {
            iLength = (int)(pcFResult - pcStart);
            acSub = new char[iLength+1];
            StrNCpy(acSub,pcStart,iLength);
            acSub[iLength] = 0;
            rqSplits.Push(String(acSub));
            delete[] acSub;
            pcStart = pcFResult + rqDelimiter.m_iLength;
            pcFResult = StrStr(pcStart,rqDelimiter.m_acText);
        }

        if ( pcFResult )
            iLength = (int)(pcFResult - pcStart);
        else
            iLength = m_iLength;
        acSub = new char[iLength+1];
        StrNCpy(acSub,pcStart,iLength);
        acSub[iLength] = 0;
        rqSplits.Push(String(acSub));
        delete[] acSub;
    }
    else
    {
        rqSplits.SetQuantity(m_iLength);
        for (int i = 0; i < m_iLength; i++)
        {
            rqSplits[i].SetLength(1);
            rqSplits[i].m_acText[0] = m_acText[i];
        }
    }
}
//------------------------------------------------------------------------------------------------------------------
bool String::BeginsWith (const String& rqString, bool bCase) const
{
    if ( bCase )
        return ( StrNCmp(m_acText,rqString.m_acText,rqString.m_iLength) == 0 );
    else
        return ( StrNICmp(m_acText,rqString.m_acText,rqString.m_iLength) == 0 );
}
//------------------------------------------------------------------------------------------------------------------
bool String::EndsWith (const String& rqString, bool bCase) const
{
    if ( m_iLength < rqString.m_iLength )
        return false;

    if ( bCase )
        return ( StrNCmp(m_acText+m_iLength-rqString.m_iLength,rqString.m_acText,rqString.m_iLength) == 0 );
    else
        return ( StrNICmp(m_acText+m_iLength-rqString.m_iLength,rqString.m_acText,rqString.m_iLength) == 0 );
}
//------------------------------------------------------------------------------------------------------------------
Link<int,bool> String::Find (const String& rqString, const MatchOptions& rqOptions) const
{
    assert( 0 <= rqOptions.From && rqOptions.From < m_iLength );
    assert( rqOptions.To == -1 || (0 <= rqOptions.To && rqOptions.To <= m_iLength) );
    assert( rqOptions.To == -1 || rqOptions.From <= rqOptions.To );

    char* pcFResult;
    if ( rqOptions.Case )
        pcFResult = StrStr(m_acText+rqOptions.From,rqString.m_acText);
    else
        pcFResult = FindStringCaseInsensitive(m_acText+rqOptions.From,rqString.m_acText);

    if ( pcFResult && (rqOptions.To == -1 || pcFResult < m_acText + rqOptions.To) )
        return Link<int,bool>((int)(pcFResult-m_acText),true);
    else
        return Link<int,bool>(0,false);
}
//------------------------------------------------------------------------------------------------------------------
int String::Replace (const String& rqWhat, const String& rqBy, MatchOptions qOptions)
{
    assert( 0 <= qOptions.From && qOptions.From < m_iLength );
    assert( qOptions.To == -1 || (0 <= qOptions.To && qOptions.To <= m_iLength) );
    assert( qOptions.To == -1 || qOptions.From <= qOptions.To );
    assert( rqWhat.m_iLength != 0 );

    if ( qOptions.To != -1 )
    {
        if ( qOptions.To < 0 )
            qOptions.To = 0;
        if ( qOptions.To > m_iLength )
            qOptions.To = m_iLength;
    }

    Array<char*> qFPositions;
    char* pcTo = ( qOptions.To == -1 ? m_acText + m_iLength : m_acText + qOptions.To );

    char* pcFResult;
    if ( qOptions.Case )
    {
        pcFResult = StrStr(m_acText+qOptions.From,rqWhat.m_acText);
        while ( pcFResult && pcFResult < pcTo )
        {
            qFPositions.Push(pcFResult);
            pcFResult = StrStr(pcFResult+1,rqWhat.m_acText);
        }
    }
    else
    {
        pcFResult = FindStringCaseInsensitive(m_acText+qOptions.From,rqWhat.m_acText,rqWhat.m_iLength);
        while ( pcFResult && pcFResult < pcTo )
        {
            qFPositions.Push(pcFResult);
            pcFResult = FindStringCaseInsensitive(pcFResult+1,rqWhat.m_acText,rqWhat.m_iLength);
        }
    }

    if ( !qFPositions.IsEmpty() )
    {
        int iNewLength = m_iLength + qFPositions.GetQuantity()*(rqBy.m_iLength - rqWhat.m_iLength);
        char* acNew = new char[iNewLength+1];
        char* pcPosNew = acNew;
        char* pcPosOld = m_acText;
        int iLeft;
        for (int i = 0; i < qFPositions.GetQuantity(); i++)
        {
            iLeft = (int)(qFPositions[i] - pcPosOld);
            StrNCpy(pcPosNew,pcPosOld,iLeft);
            pcPosNew += iLeft;
            StrNCpy(pcPosNew,rqBy.m_acText,rqBy.m_iLength);
            pcPosNew += rqBy.m_iLength;
            pcPosOld += iLeft + rqWhat.m_iLength;
        }
        iLeft = (int)(m_acText + m_iLength - pcPosOld);
        StrNCpy(pcPosNew,pcPosOld,iLeft);
        acNew[iNewLength] = 0;

        m_iLength = iNewLength;
        delete[] m_acText;
        m_acText = acNew;
    }
    return qFPositions.GetQuantity();
}
//------------------------------------------------------------------------------------------------------------------
String& String::operator= (const String& rqString)
{
    if ( &rqString == this )
        return *this;

    m_iLength = rqString.m_iLength;
    delete[] m_acText;
    m_acText = new char[m_iLength+1];
    StrNCpy(m_acText,rqString.m_acText,m_iLength);
    m_acText[m_iLength] = 0;
    return *this;
}
//------------------------------------------------------------------------------------------------------------------
bool String::operator== (const char* acText) const
{
    return StrCmp(m_acText,acText) == 0;
}
//------------------------------------------------------------------------------------------------------------------
bool String::operator!= (const char* acText) const
{
    return StrCmp(m_acText,acText) != 0;
}
//------------------------------------------------------------------------------------------------------------------
bool String::operator< (const char* acText) const
{
    return StrCmp(m_acText,acText) < 0;
}
//------------------------------------------------------------------------------------------------------------------
bool String::operator<= (const char* acText) const
{
    return StrCmp(m_acText,acText) <= 0;
}
//------------------------------------------------------------------------------------------------------------------
bool String::operator> (const char* acText) const
{
    return StrCmp(m_acText,acText) > 0;
}
//------------------------------------------------------------------------------------------------------------------
bool String::operator>= (const char* acText) const
{
    return StrCmp(m_acText,acText) >= 0;
}
//------------------------------------------------------------------------------------------------------------------
bool String::operator== (const String& rqString) const
{
    return ( m_iLength == rqString.m_iLength && StrNCmp(m_acText,rqString.m_acText,m_iLength) == 0 );
}
//------------------------------------------------------------------------------------------------------------------
bool String::operator!= (const String& rqString) const
{
    return ( m_iLength != rqString.m_iLength || StrNCmp(m_acText,rqString.m_acText,m_iLength) != 0 );
}
//------------------------------------------------------------------------------------------------------------------
bool String::operator< (const String& rqString) const
{
    return ( m_iLength == rqString.m_iLength && StrNCmp(m_acText,rqString.m_acText,m_iLength) < 0 );
}
//------------------------------------------------------------------------------------------------------------------
bool String::operator<= (const String& rqString) const
{
    return ( m_iLength == rqString.m_iLength && StrNCmp(m_acText,rqString.m_acText,m_iLength) <= 0 );
}
//------------------------------------------------------------------------------------------------------------------
bool String::operator> (const String& rqString) const
{
    return ( m_iLength == rqString.m_iLength && StrNCmp(m_acText,rqString.m_acText,m_iLength) > 0 );
}
//------------------------------------------------------------------------------------------------------------------
bool String::operator>= (const String& rqString) const
{
    return ( m_iLength == rqString.m_iLength && StrNCmp(m_acText,rqString.m_acText,m_iLength) >= 0 );
}
//------------------------------------------------------------------------------------------------------------------
bool String::CompareCaseInsensitively (const String& rqString) const
{
    return ( m_iLength == rqString.m_iLength && StrNICmp(m_acText,rqString.m_acText,m_iLength) == 0 );
}
//------------------------------------------------------------------------------------------------------------------
char* String::FindStringCaseInsensitive (const char* acMainText, const char* acText, int iLength) const
{
    if ( iLength == -1 )
        iLength = (int)StrLen(acText);

    char* pcFPos = (char*)acMainText;
    char cFirstL = (char)ToLower(*acText);
    char cFirstU = (char)ToUpper(*acText);
    char* pcFResultL;
    char* pcFResultU;

    for (/**/; /**/; /**/)
    {
        pcFResultL = StrChr(pcFPos,cFirstL);
        if ( pcFResultL )
        {
            if ( StrNICmp(pcFPos,acText,iLength) == 0 )
                return pcFPos;
            else
                pcFPos++;
        }
        else
        {
            pcFResultU = StrChr(pcFPos,cFirstU);
            if ( pcFResultU )
            {
                if ( StrNICmp(pcFPos,acText,iLength) == 0 )
                    return pcFPos;
                else
                    pcFPos++;
            }
            else
            {
                return 0;
            }
        }
    }
}
//------------------------------------------------------------------------------------------------------------------
void String::Save (Stream& rqStream) const
{
    rqStream.Write(m_iLength);
    rqStream.Write(m_iLength,m_acText);
}
//------------------------------------------------------------------------------------------------------------------
void String::Load (Stream& rqStream)
{
    rqStream.Read(m_iLength);

    // allocate heap
    m_acText = new char[m_iLength];

    // assign heap
    rqStream.Read(m_iLength,m_acText);
}
//------------------------------------------------------------------------------------------------------------------



