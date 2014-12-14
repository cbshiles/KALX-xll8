// take.cpp - take from front or back of range
#include "range.h"

using namespace xll;

static AddIn xai_range_take(
	Function(XLL_LPOPER, XLL_DECORATE("xll_range_take",8), "RANGE.TAKE")
	.Arg(XLL_LONG, "Count", "is the number of elements to take.")
	.Arg(XLL_LPOPER, "Range", "is a range or handle to a range.")
	.Category("XLL")
	.FunctionHelp("take elements of Range.")
);
extern "C" LPOPER __declspec(dllexport) WINAPI xll_range_take(LONG n, const LPOPER pr)
{
	static OPER o;

	try {
		handle<OPER> po(o = *pr, false);

		if (po)
			*po = range::take(*po, n);
		else
			o = range::take(*pr, n);
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return &o;
}

static AddIn12 xai_range_take12(
	Function12(XLL_LPOPER12, XLL_DECORATE12(L"xll_range_take12",8), L"RANGE.TAKE")
	.Arg(XLL_LONG12, L"Count", L"is the number of elements to take.")
	.Arg(XLL_LPOPER12, L"Range", L"is a range or handle to a range.")
	.Category(L"XLL")
	.FunctionHelp(L"Take elements of Range.")
);
extern "C" LPOPER12 __declspec(dllexport) WINAPI xll_range_take12(LONG n, const LPOPER12 pr)
{
	static OPER12 o;

	try {
		handle<OPER12> po(o = *pr, false);

		if (po)
			*po = range::take(*po, n);
		else
			o = range::take(*pr, n);
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return &o;
}

#ifdef _DEBUG

int xll_test_range_take()
{
	try {
		OPERX o {OPERX(1.23), OPERX(_T("abc")), OPERX(true)};
	
		OPERX o2 = range::take(o, 0);
		ensure (o2 == OPERX());
	
		OPERX o3 = range::take(o, 1);
		ensure (o3.size() == 1);
		ensure (o3[0] == o[0]);

		OPERX o4 = range::take(o, 3);
		ensure (o4 == o);

		OPERX o5 = range::take(o, -2);
		ensure (o5.size() == 2);
		ensure (o5[0] == o[1]);
		ensure (o5[1] == o[2]);
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());

		return 0;
	}

	return 1;
}
static Auto<OpenX> xao_test_range_take(xll_test_range_take);

#endif // _DEBUG