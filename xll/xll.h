// xll.h - Top level header file for xll library.
// Copyright (c) KALX, LLC. All rights reserved. No warranty made.
// #define EXCEL12 for Excel 2007 and later.
#ifndef XLL_H
#define XLL_H

#ifdef EXCEL12

// standard i18n defines
#undef _MBCS
#ifndef _UNICODE
#define _UNICODE
#endif
#ifndef UNICODE
#define UNICODE
#endif

#else

#ifndef _MBCS
#define _MBCS
#endif
#undef _UNICODE
#undef UNICODE

#endif

#pragma warning(disable: 4996)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <tchar.h>
extern "C" {
#include "XLCALL.H"
}

// like assert but throws std::runtime_error
#include "ensure.h"

// try { ... ensure(...); } catch (const std::exception& ex) { XLL_ERROR|WARNING|INFO(ex.what()); }
#include "error.h"

/// Excel instance handle
extern HMODULE xllInstance;

/// Excel window handle.
HWND GetHwnd(void);

// include chain to AddIn class
#include "defines.h"

// extras
#include "on.h"

#endif // XLL_H