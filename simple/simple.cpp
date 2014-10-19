// simple.cpp - a simple xll
//#define EXCEL12
#include "../xll/xll.h"

using namespace xll;

int xll_simple(void)
{
	try {
		OPERX o;
		ensure (o.xltype == xltypeNil);
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());

		return FALSE;
	}

	return TRUE;
}
static Auto<OpenAfterX> xao_simple(xll_simple);

static AddIn12 xai_select_end(
	Function12(L">", L"?xll_select_end", L"XLL.SELECT.END")
	.Arg(XLL_LPXLOPER12, L"Range", L"is a range")
	.Uncalced()
	.Category(L"XLL")
);
LPOPER12 WINAPI xll_select_end(LPXLOPER12 po)
{
#pragma XLLEXPORT
	static OPER12 o; 
	
	o = *po;

	return &o;
}
