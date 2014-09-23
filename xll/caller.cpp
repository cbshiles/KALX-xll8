// caller.cpp
#include "xll.h"

using namespace xll;

static AddIn xai_caller(
	XLL_DECORATE("xll_caller",0), XLL_BOOL XLL_UNCALCED,
	"CALLING", "",
	"XLL", "Return TRUE if the cell is being called by F2 Enter."
);
extern "C" BOOL __declspec(dllexport) WINAPI xll_caller(void)
{
	LOPER x;

	x = Excel<XLOPER>(xlCoerce, Excel<XLOPER>(xlfCaller));

	return x;
}
static AddIn12 xai_caller12(
	XLL_DECORATE12(L"xll_caller12",0), XLL_BOOL12 XLL_UNCALCED12,
	L"CALLING", L"",
	L"XLL", L"Return TRUE if the cell is being called by F2 Enter."
);
extern "C" BOOL __declspec(dllexport) WINAPI
xll_caller12(void)
{
	LOPER12 x;

	x = Excel<XLOPER12>(xlCoerce, Excel<XLOPER12>(xlfCaller));

	return x;
}

