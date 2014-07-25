#ifndef QSYSTEM_H
#define QSYSTEM_H

// Platform-specific information.  The defines to control which platform is included are:
//   MINGW     : Minimalist GNU for Windows
//   _WIN32    : Microsoft Windows 9x/2000/XP/Vista
//   __APPLE__ : Macintosh OS X (10.2.3 or higher required)
//   <none>    : Linux

//------------------------------------------------------------------------------------------------------------------
// Minimalist GNU for Windows
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#if defined(MINGW)

// Macro used for Microsoft Windows systems to support dynamic link libraries.  Not needed for MINGW.
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

// many CRT functions at Visual C++ 8.0 have suffix '_s' and are encouraged by the IDE, but for the reason of
// portability the established ones still remain in use
#pragma warning( disable : 4996 )

#endif

// The use of Q_ITEM to export an entire class generates warnings when there occur member data or functions
// that involve templates.  To avoid the warning, Q_ITEM can be applied only to those items that really need to
// be exported.  The second directive disables the warning "non dll-interface class <a class> used as base for
// dll-interface class <a class>".
#pragma warning( disable : 4251 )
#pragma warning( disable : 4275 )

// Enable the warning: "loop control variable declared in the for-loop is used outside the for-loop scope".
// The default level 3 warnings do not enable this (level 4 does), but should since allowing the outside scope
// is a Microsoft extension.
#pragma warning( error : 4289 )

typedef __int64 Int64;

#endif

// Specialized instantiation of static members in template classes before or after the class itself is
// instantiated is not a problem with Visual Studio .NET 7.1 and above, but VC 6 likes the specialized
// instantiation to occur after the class instantiation.
// #define Q_INSTANTIATE_BEFORE

//------------------------------------------------------------------------------------------------------------------
// Macintosh OS X platform
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#elif defined(__APPLE__)

#define Q_BIG_ENDIAN

// Macro used for Microsoft Windows systems to support dynamic link libraries.  Not needed for the Macintosh.
#define Q_ITEM

#include <stdint.h>
typedef int64_t Int64;

// g++ wants specialized template instantiations to occur after the explicit class instantiations.  CodeWarrior
// wants them to occur before.
#ifdef __MWERKS__
#define Q_INSTANTIATE_BEFORE
#endif

//------------------------------------------------------------------------------------------------------------------
// Linux platform
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#else

// Macro used for Microsoft Windows systems to support dynamic link libraries.  Not needed for Linux.
#define Q_ITEM

#include <stdint.h>
typedef int64_t Int64;

// Linux on a PC.  Red Hat 8.x g++ has problems with specialized instantiation of static members in template
// classes *before* the class itself is explicitly instantiated.  The problem is not consistent, so the
// following macro is *not* defined for this platform.
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



