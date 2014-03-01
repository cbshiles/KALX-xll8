// eval.cpp - evaluate an Excel expression
#include "xll.h"

using namespace xll;

static AddIn xai_eval(
	"_xll_eval@4", XLL_LPXLOPER XLL_LPXLOPER XLL_UNCALCED, 
	"EVAL", "String", 
	"XLL", "Call xlfEvaluate on String."
);
extern "C" LPXLOPER __declspec(dllexport) WINAPI xll_eval(LPXLOPER po)
{
	static LOPER x;

	x = Excel<XLOPER>(xlfEvaluate, *po);

	return x.XLFree();
}
static AddIn12 xai_eval12(
	L"_xll_eval12@4", XLL_LPXLOPER12 XLL_LPXLOPER12 XLL_UNCALCED12, 
	L"EVAL", L"String", 
	L"XLL", L"Call xlfEvaluate on String.");
extern "C" LPXLOPER12 __declspec(dllexport) WINAPI xll_eval12(LPXLOPER12 po)
{
	static LOPER12 x;

	x = Excel<XLOPER12>(xlfEvaluate, *po);

	return x? x.XLFree() : po;
}

