// samplex.cpp - Single source dual use code.
#include <cmath> // for sqrt
// Uncomment to build for Excel 2007 and above
// #define EXCEL12
#include "xll/xll.h"

using namespace xll;

static AddInX xai_sqrtx(
	FunctionX(XLL_LPOPERX, _T("?xll_sqrt"), TX_("XLL.SQRT"))
	.Num(_T("Number"), _T("is a floating point number."))
	.Category(_T("XLL"))
	.FunctionHelp(_T("Return the square root of Number or #NUM! if less than 0."))
);
LPOPERX WINAPI X_(xll_sqrt)(double num)
{
#pragma XLLEXPORT
	static OPERX o;

	if (num >= 0)
		o = sqrt(num);
	else
		o = OPERX(xlerr::Num);

	return &o;
}


