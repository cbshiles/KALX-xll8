// paste.cpp - paste functions into Excel
#include "xll.h"

using namespace xll;

static AddIn xai_paste_basic(XLL_DECORATE("xll_paste_basic",0), "XLL.PASTE.BASIC");
extern "C" int __declspec(dllexport) WINAPI xll_paste_basic(void)
{
	try {
		OPER ac,co;
		ac = Excel<XLOPER>(xlfActiveCell);
		co = Excel<XLOPER>(xlCoerce, ac);
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());

		return FALSE;
	}

	return TRUE;
}
// Ctrl-Shift-B (after Ctrl-Shift-A)
static On<Key> xok_paste_basic(_T("^+B"), _T("XLL.PASTE.BASIC"));
