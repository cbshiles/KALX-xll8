// error.h - Error reporting functions
// Copyright (c) 2005-2011 KALX, LLC. All rights reserved.
#pragma once
#include "utility/registry.h"
#include "utility/log.h"

/*
// !!!Doesn't work with Excel 2007 and above.
inline bool in_function_wizard(void)
{
	return !Excel<XLOPER>(xlfGetTool, OPER(4), OPER("Standard"), OPER(1));
}
*/

extern Reg::Object<char, DWORD> xll_alert_level;
extern Log::EventSource xll_log;

#ifndef _LIB

#pragma comment(linker, "/include:?xll_alert_level@@3V?$Object@DK@Reg@@A")
#pragma comment(linker, "/include:?log@xll@@YA_NH@Z")

#endif // _LIB

enum { 
	XLL_ALERT_ERROR   = 1,
	XLL_ALERT_WARNING = 2, 
	XLL_ALERT_INFO    = 4,
	XLL_ALERT_LOG     = 8	// turn on logging (not yet implemented)
};

// OKCANCEL message box. Cancel turns off alert bit
int XLL_ERROR(const char* e, bool force = false);
int XLL_WARNING(const char* e, bool force = false);
int XLL_INFO(const char* e, bool force = false);
inline int XLL_INFORMATION(const char* e, bool force = false) { return XLL_INFO(e, force); }

namespace xll {
	// -1 report, 0 off, 1 on
	inline bool log(int on)
	{
		static bool on_ = false;

		bool result = on_;
		if (on == 0)
			on_ = false;
		else if (on == 1)
			on_ = true;

		return result;
	}
}
