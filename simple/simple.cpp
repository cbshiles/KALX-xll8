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

static AddInX xai_select_end(
	Function(_T(">"), _T("?xll_select_end"), _T("XLL.SELECT.END"))
	.Arg(XLL_LPXLOPERX, _T("Range"), _T("is a range"))
	.Uncalced()
	.Category(_T("XL_T("))
);
LPOPERX WINAPI xll_select_end(LPXLOPERX po)
{
#pragma XLLEXPORT
	static OPERX o; 
	
	o = *po;

	return &o;
}
