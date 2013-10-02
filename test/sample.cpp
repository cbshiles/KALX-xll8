#include <cmath> // for sqrt
#include "xll/xll.h"

using namespace xll;

static AddIn xai_sqrt(
	Function(XLL_LPOPER, "?xll_sqrt", "XLL.SQRT")
	.Arg(XLL_DOUBLE, "Number", "is a floating point number.")
	.Category("XLL")
	.FunctionHelp("Return the square root of Number or #NUM! if less than 0. ")
);
LPOPER WINAPI xll_sqrt(double num)
{
#pragma XLLEXPORT
	static OPER o;

	if (num >= 0)
		o = sqrt(num);
	else
		o = OPER(xlerr::Num);

	return &o;
}
