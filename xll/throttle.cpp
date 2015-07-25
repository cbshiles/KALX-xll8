// throttle.cpp - only call on F2-Enter in cell
#include "xll.h"

using namespace xll;

static AddIn xai_throttle(
	XLL_DECORATE("xll_throttle",4), XLL_LPXLOPER XLL_LPXLOPER XLL_UNCALCED, 
	"THROTTLE", "Cell", 
	"XLL", "Only call Cell on F2-Enter."
);
extern "C" LPXLOPER __declspec(dllexport) WINAPI xll_throttle(LPXLOPER po)
{
	static OPER x;

	x = Excel<XLOPER>(xlCoerce, Excel<XLOPER>(xlfCaller));
	if (x[0].xltype == xltypeNum && x[0].val.num == 0)
		x = Excel<XLOPER>(xlCoerce, *po);

	return &x;
}
static AddIn12 xai_throttle12(
	XLL_DECORATE12(L"xll_throttle12",4), XLL_LPXLOPER12 XLL_LPXLOPER12 XLL_UNCALCED12, 
	L"THROTTLE", L"Cell", 
	L"XLL", L"Only call Cell on F2-Enter.");
extern "C" LPXLOPER12 __declspec(dllexport) WINAPI xll_throttle12(LPXLOPER12 po)
{
	static OPER12 x;

	x = Excel<XLOPER12>(xlCoerce, Excel<XLOPER12>(xlfCaller));
	if (x[0].xltype == xltypeNum && x[0].val.num == 0)
		x = Excel<XLOPER12>(xlCoerce, *po);

	return &x;
}

