// simple.cpp - a simple xll
#define EXCEL12
#include "../xll/xll.h"

using namespace xll;

int xll_test_simple()
{
	OPERX o;
	traits<XLOPERX>::xstring s;

	s = o.to_string();
	ensure (s == _T("=\"\""));
	o = OPERX(s);
	ensure (o.xltype == xltypeStr && o.val.str[0] == 3);
	o = Excel<XLOPERX>(xlfEvaluate, OPERX(s));
	ensure (o == _T(""));

	return 1;
}
static Auto<OpenX> xao_test_simple(xll_test_simple);

static AddInX xai_foo(_T("?xll_foo"), XLL_SHORTX XLL_SHORTX, _T("SIMPLE.FOO"), _T("int"));
SHORT WINAPI xll_foo(SHORT i)
{
#pragma XLLEXPORT

	return xyz_foo(i);
}