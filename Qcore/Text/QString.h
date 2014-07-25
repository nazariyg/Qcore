#ifndef QSTRING_H
#define QSTRING_H

// The String class is the string of ASCII characters.  The string appears in memory as null-terminated, even
// if empty.

#include "QArray.h"

namespace Q
{

class Q_ITEM String
{

Q_DECLARE_STREAM;

public:
    // Supporting class for find/replace operations.  The member data of the class have the following roles:
    // From specifies the position of the first character in the range to be searched, To indicates the
    // position of the character next to the last character in the range (points to an imaginary character if
    // the range includes the last character in the string), and Case flags whether the search is
    // case-sensitive.
    // Default construction: find/replace in the whole string, case-sensitively.
    class MatchOptions
    {
    public:
        MatchOptions ()
            :
            From(0),
            To(-1),
            Case(true)
        {
        };

        int From, To;
        bool Case;
    };

    // Construction.  A construction from a numerical value generates its textual representation styled
    // appropriate to the standards.  Both for integer and floating-point numbers, a resulting string never
    // begins with "+" or multiple zeroes.  In particular, a string created from a floating-point number has
    // the following characteristics: the radix point is preceded at least by one digit, the digits of the
    // fractional part after the radix point never end with multiple zeroes; if the scientific notation is
    // flagged with bExp parameter, the radix point is always preceded by a single digit, the concluding
    // exponential part appears left-to-right composed of: the lowercase "e", a sign-character ("-" or "+"),
    // and two or three digits of an exponent (from "00" and up to "999").
    String (const char* acText = 0);          // from a native string or as empty
    String (const String& rqString);          // from another String object
    String (int iNum);                        // making the text from an integer value
    String (double dNum, bool bExp = false);  // making the text from a floating-point value

    // destruction
    ~String ();

    // text access
    int GetLength () const;     // (not counting the null-terminator)
    int GetMemLength () const;  // (counting the null-terminator)
    bool IsEmpty () const;
    char* GetChars ();
    const char* GetChars () const;
    operator char* ();
    operator const char* () const;
    char operator [] (int i) const;
    char& operator [] (int i);

    // operations
    void SetLength (int iNewLength);                  // resizing; new characters appear uninitialized
    String operator+ (const String& rqString) const;  // performs concatenation and returns the result
    String& operator+= (const String& rqString);      // changes the string via concatenation
    void Insert (int i, const String& rqString);      // inserts a string at a specified position
    bool Trim ();                                     // removes trailing spaces, returns changes-flag
    void Remove (int i, int iLength = -1);            // removes a range by start-and-length; default: to the end
    void Erase ();                                    // makes the string empty
    void ToLowerCase ();                              // makes the string lowercase
    void ToUpperCase ();                              // makes the string uppercase
    int ToInt () const;                               // only if the string represents an integer value
    double ToDouble () const;                         // only if the string represents a floating-point value

    // Retrieval.  For GetSubstringByRange function, parameter iFrom specifies the position of the first
    // character in the range of the substring, while parameter iTo indicates the position of the character
    // next to the last character in the range (iTo points to an imaginary character if the range includes the
    // last character in the string).
    String GetSubstringByLength (int i, int iLength = -1) const;  // default: copy up to the end
    String GetSubstringByRange (int iFrom, int iTo) const;
    void GetSplitsByChar (char cDelimiter, Array<String>& rqSplits) const;
    void GetSplitsByCharSet (const String& rqDelimiterSet, Array<String>& rqSplits) const;
    void GetSplitsByString (const String& rqDelimiter, Array<String>& rqSplits) const;

    // Examination and search.  Element First of a link returned from Find function indicates the position of
    // the first character of the substring has been found, but only if element Second is 'true'.
    bool BeginsWith (const String& rqString, bool bCase = true) const;
    bool EndsWith (const String& rqString, bool bCase = true) const;
    Link<int,bool> Find (const String& rqString, const MatchOptions& rqOptions = MatchOptions()) const;

    // Replacement.  Parameter rqWhat must be a non-empty string; returns the number of replacements done.
    int Replace (const String& rqWhat, const String& rqBy, MatchOptions qOptions = MatchOptions());

    // assignment
    String& operator= (const String& rqString);

    // comparison
    bool operator== (const char* acText) const;
    bool operator!= (const char* acText) const;
    bool operator<  (const char* acText) const;
    bool operator<= (const char* acText) const;
    bool operator>  (const char* acText) const;
    bool operator>= (const char* acText) const;
    bool operator== (const String& rqString) const;
    bool operator!= (const String& rqString) const;
    bool operator<  (const String& rqString) const;
    bool operator<= (const String& rqString) const;
    bool operator>  (const String& rqString) const;
    bool operator>= (const String& rqString) const;
    bool CompareCaseInsensitively (const String& rqString) const;  // acts like operator == but case-insensitively

    // getting the character from an ASCII code
    static char Chr (int iCode);

private:
    char* FindStringCaseInsensitive (const char* acMainText, const char* acText, int iLength = -1) const;

    int m_iLength;
    char* m_acText;

    // size of the transformation buffer
    static const int ms_iTrnMaxChars;  // 64
};

#include "QString.inl"

}

#endif



