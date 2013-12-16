// xll.h - Top level header file for xll library.
// Copyright (c) KALX, LLC. All rights reserved. No warranty made.
// #define EXCEL12 for Excel 2007 and later.
#pragma once
#include <ctime>

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

// xll_alert_level, XLL_ERROR|WARNING|INFO
// try { ... ensure(...); ... } catch (const std::exception& ex) { XLL_ERROR|WARNING|INFO(ex.what()); }
#include "error.h"

/// Excel instance handle from DllMain
extern HMODULE xll_Instance;

/// Excel window handle.
HWND xll_GetHwnd(void);


// include chain to AddIn class
#include "defines.h"

// extras
#include "dt.h"
#include "entities.h"
#include "enum.h"
#include "fp.h"
#include "handle.h"
#include "on.h"

#pragma comment(linker, "/include:_xll_caller@0")
#pragma comment(linker, "/include:_xll_caller12@0")
#pragma comment(linker, "/include:_xll_this@0")
#pragma comment(linker, "/include:_xll_this12@0")
#pragma comment(linker, "/include:_xll_throttle@4")
#pragma comment(linker, "/include:_xll_throttle12@4")
