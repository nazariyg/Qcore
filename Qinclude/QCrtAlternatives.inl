// This page contains rewritten standard C run-time library replacement routines that originate from the
// University of California.  The copyright notice follows below.

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

/*
Copyright (c) 1992, 1993
The Regents of the University of California. All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions
are met:
1. Redistributions of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in the
   documentation and/or other materials provided with the distribution.
3. All advertising materials mentioning features or use of this software
   must display the following acknowledgement:
     This product includes software developed by the University of
     California, Berkeley and its contributors.
4. Neither the name of the University nor the names of its contributors
   may be used to endorse or promote products derived from this software
   without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
SUCH DAMAGE.
*/

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

//------------------------------------------------------------------------------------------------------------------
inline int MemCmp (const void* pvBuffer0, const void* pvBuffer1, size_t iQuantity)
{
#ifndef Q_USE_CRT_ASM
    if ( iQuantity == 0 )
        return 0;
    char* pcBuffer0 = (char*)pvBuffer0;
    char* pcBuffer1 = (char*)pvBuffer1;
    while ( *pcBuffer0 == *pcBuffer1 && --iQuantity != 0 )
    {
        pcBuffer0++;
        pcBuffer1++;
    }
    return *pcBuffer0 - *pcBuffer1;
#else
    return memcmp(pvBuffer0,pvBuffer1);
#endif
}
//------------------------------------------------------------------------------------------------------------------
inline void* MemCpy (void* pvDst, const void* pvSrc, size_t iQuantity)
{
#ifndef Q_USE_CRT_ASM
    char* acDst = (char*)pvDst;
    char* acSrc = (char*)pvSrc;
    while ( iQuantity-- != 0 )
        acDst[iQuantity] = acSrc[iQuantity];
    return pvDst;
#else
    return memcpy(pvDst,pvSrc,iQuantity);
#endif
}
//------------------------------------------------------------------------------------------------------------------
inline void* MemMove (void* pvDst, const void* pvSrc, size_t iQuantity)
{
#ifndef Q_USE_CRT_ASM
    size_t i;
    char* acDst = (char*)pvDst;
    char* acSrc = (char*)pvSrc;
    if ( pvDst > pvSrc )
    {
        for (i = iQuantity-1; i >= 0; i--)
            acDst[i] = acSrc[i];
    }
    else
    {
        for (i = 0; i < iQuantity; i++)
            acDst[i] = acSrc[i];
    }
    return pvDst;
#else
    return memmove(pvDst,pvSrc,iQuantity);
#endif
}
//------------------------------------------------------------------------------------------------------------------
inline void* MemSet (void* pvDst, int iValue, size_t iQuantity)
{
#ifndef Q_USE_CRT_ASM
    char* acDst = (char*)pvDst;
    while ( iQuantity-- != 0 )
        acDst[iQuantity] = iValue;
    return pvDst;
#else
    return memset(pvDst,iValue,iQuantity);
#endif
}
//------------------------------------------------------------------------------------------------------------------
inline char* StrCat (char* acDst, const char* acSrc)
{
#ifndef Q_USE_CRT_ASM
    char* pcChar = acDst;
    while ( *pcChar != 0 )
        pcChar++;
    while ( *acSrc != 0 )
        *pcChar++ = *acSrc++;
    *pcChar = 0;
    return acDst;
#else
    return strcat(acDst,acSrc);
#endif
}
//------------------------------------------------------------------------------------------------------------------
inline char* StrChr (const char* acStr, int iChar)
{
#ifndef Q_USE_CRT_ASM
    while ( *acStr != 0 )
    {
        if ( *acStr == iChar )
            return (char*)acStr;
        acStr++;
    }
    return 0;
#else
    return strchr(acStr,iChar);
#endif
}
//------------------------------------------------------------------------------------------------------------------
inline int StrCmp (const char* acStr0, const char* acStr1)
{
#ifndef Q_USE_CRT_ASM
    while ( *acStr0 == *acStr1 && *acStr0 != 0 && *acStr1 != 0 )
    {
        acStr0++;
        acStr1++;
    }
    return *acStr0 - *acStr1;
#else
    return strcmp(acStr0,acStr1);
#endif
}
//------------------------------------------------------------------------------------------------------------------
inline char* StrCpy (char* acDst, const char* acSrc)
{
#ifndef Q_USE_CRT_ASM
    char* pcChar = acDst;
    while ( *acSrc != 0 )
        *pcChar++ = *acSrc++;
    *pcChar = 0;
    return acDst;
#else
    return strcpy(acDst,acSrc);
#endif
}
//------------------------------------------------------------------------------------------------------------------
inline size_t StrLen (const char* acString)
{
#ifndef Q_USE_CRT_ASM
    const char* pcChar = acString;
    while ( *pcChar != 0 )
        pcChar++;
    return pcChar - acString;
#else
    return strlen(acString);
#endif
}
//------------------------------------------------------------------------------------------------------------------
inline int StrNCmp (const char* acStr0, const char* acStr1, size_t iQuantity)
{
#ifndef Q_USE_CRT_ASM
    if ( iQuantity == 0 )
        return 0;
    while ( *acStr0 == *acStr1 && --iQuantity != 0 )
    {
        acStr0++;
        acStr1++;
    }
    return *acStr0 - *acStr1;
#else
    return strncmp(acStr0,acStr1,iQuantity);
#endif
}
//------------------------------------------------------------------------------------------------------------------
inline char* StrNCpy (char* acDst, const char* acSrc, size_t iQuantity)
{
#ifndef Q_USE_CRT_ASM
    char* pcChar = acDst;
    while ( *acSrc != 0 && iQuantity != 0 )
    {
        *pcChar++ = *acSrc++;
        iQuantity--;
    }
    while ( iQuantity-- != 0 )
        *pcChar++ = 0;
    return acDst;
#else
    return strncpy(acDst,acSrc,iQuantity);
#endif
}
//------------------------------------------------------------------------------------------------------------------
inline int StrNICmp (const char* acStr0, const char* acStr1, size_t iQuantity)
{
#ifndef Q_USE_CRT_ASM
    if ( iQuantity == 0 )
        return 0;
    char cLUDist = 'a' - 'A';
    while ( ( 'A' <= *acStr0 && *acStr0 <= 'Z' ? *acStr0 + cLUDist : *acStr0 ) ==
            ( 'A' <= *acStr1 && *acStr1 <= 'Z' ? *acStr1 + cLUDist : *acStr1 )
       &&   --iQuantity != 0 )
    {
        acStr0++;
        acStr1++;
    }
    return *acStr0 - *acStr1;
#else
    return strnicmp(acStr0,acStr1,iQuantity);
#endif
}
//------------------------------------------------------------------------------------------------------------------
inline char* StrPBrk (const char* acStr, const char* acCharSet)
{
#ifndef Q_USE_CRT_ASM
    const char* pcChar;
    while ( *acStr != 0 )
    {
        pcChar = acCharSet;
        while ( *pcChar != 0 )
        {
            if ( *acStr == *pcChar )
                return (char*)acStr;
            pcChar++;
        }
        acStr++;
    }
    return 0;
#else
    return strpbrk(acStr,acCharSet);
#endif
}
//------------------------------------------------------------------------------------------------------------------
inline char* StrStr (const char* acStr, const char* acCharSet)
{
#ifndef Q_USE_CRT_ASM
    while ( *acStr != 0 )
    {
        int i;
        for (i = 0; acCharSet[i] != 0; i++)
        {
            if ( acStr[i] != acCharSet[i] )
                break;
        }
        if ( acCharSet[i] == 0 )
            return (char*)acStr;
        acStr++;
    }
    return 0;
#else
    return strstr(acStr,acCharSet);
#endif
}
//------------------------------------------------------------------------------------------------------------------
inline int ToLower (int iChar)
{
#ifndef Q_USE_CRT_ASM
    if ( 'A' <= iChar && iChar <= 'Z' )
        iChar += 'a' - 'A';
    return iChar;
#else
    return tolower(iChar);
#endif
}
//------------------------------------------------------------------------------------------------------------------
inline int ToUpper (int iChar)
{
#ifndef Q_USE_CRT_ASM
    if ( 'a' <= iChar && iChar <= 'z' )
        iChar += 'A' - 'a';
    return iChar;
#else
    return toupper(iChar);
#endif
}
//------------------------------------------------------------------------------------------------------------------



