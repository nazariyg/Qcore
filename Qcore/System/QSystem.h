#ifndef QSYSTEM_H
#define QSYSTEM_H

//   MINGW     : Minimalist GNU for Windows
//   _WIN32    : Microsoft Windows 9x/2000/XP/Vista
//   __APPLE__ : Macintosh OS X (10.2.3 or higher required)
//   <none>    : Linux

//------------------------------------------------------------------------------------------------------------------
// Minimalist GNU for Windows
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#if defined(MINGW)

// To support dynamic link libraries.
#define Q_ITEM

typedef long long Int64;

//------------------------------------------------------------------------------------------------------------------
// Microsoft Windows 9x/2000/XP/Vista platform
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#elif defined(_WIN32)

// for a DLL library
#if defined(Q_DLL_EXPORT)
#define Q_ITEM __declspec(dllexport)

// for a client of the DLL library
#elif defined(Q_DLL_IMPORT)
#define Q_ITEM __declspec(dllimport)

// for a static library
#else
#define Q_ITEM

#endif

#if defined(_MSC_VER)

// Microsoft Visual C++ specific pragmas
#if _MSC_VER < 1300
#define Q_USING_VC6
#elif _MSC_VER < 1310
#define Q_USING_VC70
#elif _MSC_VER < 1400
#define Q_USING_VC71
#else
#define Q_USING_VC80

#pragma warning( disable : 4996 )

#endif

#pragma warning( disable : 4251 )
#pragma warning( disable : 4275 )

#pragma warning( error : 4289 )

typedef __int64 Int64;

#endif

//------------------------------------------------------------------------------------------------------------------
// Macintosh OS X platform
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#elif defined(__APPLE__)

#define Q_BIG_ENDIAN

// To support dynamic link libraries.
#define Q_ITEM

#include <stdint.h>
typedef int64_t Int64;

#ifdef __MWERKS__
#define Q_INSTANTIATE_BEFORE
#endif

//------------------------------------------------------------------------------------------------------------------
// Linux platform
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#else

// To support dynamic link libraries.
#define Q_ITEM

#include <stdint.h>
typedef int64_t Int64;

// #define Q_INSTANTIATE_BEFORE

#endif

//------------------------------------------------------------------------------------------------------------------
// Platform-independent
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#define Q_IF_EXISTS __if_exists
#define Q_IF_NOT_EXISTS __if_not_exists

// common standard library headers
#include <cassert>
#include <cctype>
#include <cfloat>
#include <cmath>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>

// standard library replacements
#include "QCrtAlternatives.h"

//------------------------------------------------------------------------------------------------------------------

#endif



