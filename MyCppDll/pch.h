// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#ifndef PCH_H
#define PCH_H

// add headers that you want to pre-compile here
#include "framework.h"

// I hate implicit CString constructors that can cause subtle bugs;
// so better making those bogus CString constructors *explicit*
#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS

// Convenient ATL stuff
#include <atlbase.h>
#include <atldef.h>
#include <atlstr.h>

#endif //PCH_H
