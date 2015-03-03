// simple.cpp - a simple xll
#define EXCEL12
#include "../xll/xll.h"

using namespace xll;

int xll_test_simple()
{
	OPERX o;
	traits<XLOPERX>::xstring s;

/*	s = o.to_string();
	ensure (s == _T("=\"\""));
	o = OPERX(s);
	ensure (o.xltype == xltypeStr && o.val.str[0] == 3);
	o = Excel<XLOPERX>(xlfEvaluate, OPERX(s));
	ensure (o == _T(""));

	int i = 2;
	int j;
	j = xyz_foo(i);
*/
	return 1;
}
static Auto<OpenX> xao_test_simple(xll_test_simple);


static AddInX xai_xyz_foo(
	FunctionX(XLL_LPOPERX,_T("?xll_xyz_foo"),_T("XYZ_FOO"))
	.Arg(XLL_DOUBLEX,_T("Arg"),_T("is an argument."))
	.Category(_T(""))
	.FunctionHelp(_T("Description."))
	.Documentation(_T("Documentation."))
	);
LPOPERX WINAPI xll_xyz_foo(double arg)
{
#pragma XLLEXPORT
	static OPERX o;

	try {
		o = xyz_foo((int)arg);
	}
	catch(const std::exception& ex) {
		XLL_ERROR(ex.what());

		o = OPERX(xlerr::Num);
	}

	return &o;
}


static AddInX xai_name(
	_T("?xll_name"), XLL_LPXLOPERX XLL_LPOPERX XLL_UNCALCEDX, 	
	_T("NAME"), _T("Oper"),
	_T("XLL"), _T("Return value of xlfSetName for Oper.")
);
LPXLOPERX WINAPI xll_name(LPOPERX po)
{
#pragma XLLEXPORT

	try {
		static OPERX n(_T("_fUnKyStUff__"));
		LOPERX o;
		o = XLL_XLF(SetName, n, *po);
		o = XLL_XLF(GetName, n);
		XLL_XLF(SetName, n);

		return o.XLFree();
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());

		return 0;
	}
}
