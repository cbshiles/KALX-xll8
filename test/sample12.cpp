// sample12.cpp - code for Excel 2007 and above
/* The next two lines are equivalent to the code in this file.
#define EXCEL12
#include "samplex.cpp"
*/
#include <cmath> // for sqrt
#define EXCEL12
#include "xll/xll.h"

using namespace xll;

static AddIn12 xai_sqrt12(
	Function12(XLL_LPOPER12, L"?xll_sqrt12", L"XLL.SQRT12")
	.Num(L"Number", L"is a floating point number.")
	.Category(L"XLL")
	.FunctionHelp(L"Return the square root of Number or #NUM! if less than 0.")
);
LPOPER12 WINAPI xll_sqrt12(double num)
{
#pragma XLLEXPORT
	static OPER12 o;

	if (num >= 0)
		o = sqrt(num);
	else
		o = OPER12(xlerr::Num);

	return &o;
}


