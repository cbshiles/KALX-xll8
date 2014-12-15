// drop.cpp - drop elements from beginning or end of range
#include "range.h"

using namespace xll;

static AddIn xai_range_drop(
	Function(XLL_LPOPER, XLL_DECORATE("xll_range_drop",8), "RANGE.DROP")
	.Arg(XLL_LONG, "Count", "is the number of elements to drop.")
	.Arg(XLL_LPOPER, "Range", "is a range or handle to a range.")
	.Category("XLL")
	.FunctionHelp("drop elements of Range.")
);
extern "C" LPOPER __declspec(dllexport) WINAPI xll_range_drop(LONG n, const LPOPER pr)
{
	static OPER o;

	try {
		handle<OPER> po(o = *pr, false);

		if (po)
			*po = range::drop(*po, n);
		else
			o = range::drop(*pr, n);
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return &o;
}
static AddIn12 xai_range_drop12(
	Function12(XLL_LPOPER12, XLL_DECORATE12(L"xll_range_drop12",8), L"RANGE.DROP")
	.Arg(XLL_LONG12, L"Count", L"is the number of elements to drop.")
	.Arg(XLL_LPOPER12, L"Range", L"is a range or handle to a range.")
	.Category(L"XLL")
	.FunctionHelp(L"drop elements of Range.")
);
extern "C" LPOPER12 __declspec(dllexport) WINAPI xll_range_drop12(LONG n, const LPOPER12 pr)
{
	static OPER12 o;

	try {
		handle<OPER12> po(o = *pr, false);

		if (po)
			*po = range::drop(*po, n);
		else
			o = range::drop(*pr, n);
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return &o;
}

#ifdef _DEBUG

int xll_test_range_drop()
{
	try {
		OPERX o {OPERX(1.23), OPERX(_T("abc")), OPERX(true)};
	
		OPERX o2 = range::drop(o, 0);
		ensure (o2 == o);
	
		OPERX o3 = range::drop(o, 1);
		ensure (o3.size() == 2);
		ensure (o3[0] == o[1]);
		ensure (o3[1] == o[2]);

		OPERX o4 = range::drop(o, 3);
		ensure (o4 == OPERX());

		OPERX o5 = range::drop(o, -2);
		ensure (o5.size() == 1);
		ensure (o5[0] == o[0]);
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());

		return 0;
	}

	return 1;
}
static Auto<OpenX> xao_test_range_drop(xll_test_range_drop);

#endif // _DEBUG