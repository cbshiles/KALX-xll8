// eval.cpp - evaluate an Excel expression
#include "xll.h"

using namespace xll;

static AddIn xai_eval(
	XLL_DECORATE("xll_eval",8), XLL_LPXLOPER XLL_LPXLOPER XLL_BOOL XLL_UNCALCED,
	"EVAL", "Ref, ?Formula", 
	"XLL", "Call xlfEvaluate on Ref."
);
extern "C" LPXLOPER __declspec(dllexport) WINAPI xll_eval(LPXLOPER po, BOOL formula)
{
	static LOPER x;

	try {
		x = Excel<XLOPER>(xlfEvaluate, formula ? Excel<XLOPER>(xlfGetCell, OPER(6), *po) : *po);
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return x.XLFree();
}
static AddIn12 xai_eval12(
	XLL_DECORATE12(L"xll_eval12",8), XLL_LPXLOPER12 XLL_LPXLOPER12 XLL_BOOL12 XLL_UNCALCED12,
	L"EVAL", L"Ref, ?Formula", 
	L"XLL", L"Call xlfEvaluate on Ref.");
extern "C" LPXLOPER12 __declspec(dllexport) WINAPI xll_eval12(LPXLOPER12 po, BOOL formula)
{
	static LOPER12 x;

	try {
		x = Excel<XLOPER12>(xlfEvaluate, formula ? Excel<XLOPER12>(xlfGetCell, OPER12(6), *po) : *po);
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return x.XLFree();
}

