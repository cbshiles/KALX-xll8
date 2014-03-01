// depends.cpp - specify Excel calculation order
// Copyright (c) 2011 KALX, LLC. All rights reserved. No warranty is made.
#include "xll.h"

using namespace xll;

static AddIn xai_depends(
	Function(XLL_LPXLOPER, "_xll_depends@8", "DEPENDS")
	.Arg(XLL_LPXLOPER, "Value", "is the value to be returned.")
	.Arg(XLL_LPXLOPER, "Dependent", "is a reference to a cell that is required to be computed before Value.")
	.Category("XLL")
	.FunctionHelp("Return Value after Dependent has been computed.")
	.Documentation(
		_T("Excel calculation order is unspecified for independent formulas. This function ")
	    _T("can be used to make calculation order deterministic. The value of ")
		_T("<codeInline>Dependent</codeInline> is not used, only that it has been calculated prior to ")
		_T("returning <codeInline>Value</codeInline>. Alias <codeInline>DEPENDS</codeInline>. ")
	)
//	.Alias("DEPENDS")
);
extern "C" LPXLOPER __declspec(dllexport) WINAPI xll_depends(LPXLOPER pRef, LPXLOPER pDep)
{
	return xlretUncalced == traits<XLOPER>::Excel(xlCoerce, 0, 1, pDep) ? 0 : pRef;
}

static AddIn12 xai_depends12(
	Function12(XLL_LPXLOPER12, L"_xll_depends12@8", L"DEPENDS")
	.Arg(XLL_LPXLOPER12, L"Value", L"is the value to be returned.")
	.Arg(XLL_LPXLOPER12, L"Dependent", L"is a reference to a cell that is required to be computed before Value.")
	.Category(L"XLL")
	.FunctionHelp(L"Return Value after Dependent has been computed.")
	/*
	.Documentation(
		_T("Excel calculation order is unspecified for independent formulas. This function ")
	    _T("can be used to make calculation order deterministic. The value of ")
		_T("<codeInline>Dependent</codeInline> is not used, only that it has been calculated prior to ")
		_T("returning <codeInline>Value</codeInline>. Alias <codeInline>DEPENDS</codeInline>. ")
	)
	.Alias("DEPENDS")
	*/
);
extern "C" LPXLOPER12 __declspec(dllexport) WINAPI xll_depends12(LPXLOPER12 pRef, LPXLOPER12 pDep)
{
	return xlretUncalced == traits<XLOPER12>::Excel(xlCoerce, 0, 1, pDep) ? 0 : pRef;
}
