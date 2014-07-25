#ifndef QCRTALTERNATIVES_H
#define QCRTALTERNATIVES_H

// Performance-purposed replacements for some standard C run-time library routines.

#ifdef Q_USE_CRT_ASM
#include <cstring>
#endif

namespace Q
{

int MemCmp (const void* pvBuffer0, const void* pvBuffer1, size_t iQuantity);
void* MemCpy (void* pvDst, const void* pvSrc, size_t iQuantity);
void* MemMove (void* pvDst, const void* pvSrc, size_t iQuantity);
void* MemSet (void* pvDst, int iValue, size_t iQuantity);
char* StrCat (char* acDst, const char* acSrc);
char* StrChr (const char* acStr, int iChar);
int StrCmp (const char* acStr0, const char* acStr1);
char* StrCpy (char* acDst, const char* acSrc);
size_t StrLen (const char* acString);
int StrNCmp (const char* acStr0, const char* acStr1, size_t iQuantity);
char* StrNCpy (char* acDst, const char* acSrc, size_t iQuantity);
int StrNICmp (const char* acStr0, const char* acStr1, size_t iQuantity);
char* StrPBrk (const char* acStr, const char* acCharSet);
char* StrStr (const char* acStr, const char* acCharSet);
int ToLower (int iChar);
int ToUpper (int iChar);

#include "QCrtAlternatives.inl"

}

#endif



