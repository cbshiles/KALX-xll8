// this.cpp
#include "xll.h"

using namespace xll;

static AddIn xai_this(
	XLL_DECORATE("xll_this",0), XLL_LPXLOPER XLL_UNCALCED, 
	"THIS", "",
	"XLL", "Return the contents of the calling cell."
);
extern "C" LPXLOPER __declspec(dllexport) WINAPI xll_this(void)
{
	static LOPER x;

	x = Excel<XLOPER>(xlCoerce, Excel<XLOPER>(xlfCaller));

	return x.XLFree();
}

static AddIn12 xai_this12(
	XLL_DECORATE(L"xll_this12",0), XLL_LPXLOPER12 XLL_UNCALCED12,
	L"THIS", L"",
	L"XLL", L"Return the contents of the calling cell."
);
extern "C" LPXLOPER12 __declspec(dllexport) WINAPI xll_this12(void)
{
	static LOPER12 x;

	x = Excel<XLOPER12>(xlCoerce, Excel<XLOPER12>(xlfCaller));

	return x.XLFree();
}

