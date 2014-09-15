#include "../xll8/xll/xll.h"

static xll::AddIn xai_normal_cdf(
	xll::Function(XLL_DOUBLE, "?xll_normal_cdf", "NORMAL.CDF")
	.Arg(XLL_DOUBLE, "x", "is the argument.")
	.Category("XLL")
	.FunctionHelp("Return the normal cumulative distribution function.")
	.Documentation(R"(
		Implementation of NORMSDIST using erf.
	)")
);
double WINAPI xll_normal_cdf(double x)
{
#pragma XLLEXPORT

	return (1 + erf(x/sqrt(2)))/2;
}