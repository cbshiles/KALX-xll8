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

#include <cmath>

static AddInX xai_normal_cdf(
	FunctionX(XLL_DOUBLEX, _T("?xll_normal_cdf"), _T("NORMAL.CDF"))
	.Arg(XLL_DOUBLEX, _T("x"), _T("is a number"))
	.Category(_T("FMS"))
	.FunctionHelp(_T("Return the cumulative normal distribution at x."))
);
double WINAPI xll_normal_cdf(double x)
{
#pragma XLLEXPORT
	return 0.5*(1 + erf(x/sqrt(2)));
}