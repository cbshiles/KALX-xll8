// slice.cpp - std::slice
#include "range.h"

using namespace xll;

static AddIn xai_range_slice(
	Function(XLL_LPOPER, XLL_DECORATE("xll_range_slice",16), "RANGE.SLICE")
	.Arg(XLL_LPOPER, "Range", "is a range or handle to a range.")
	.Arg(XLL_LONG, "Start", "is the 0-based first element.")
	.Arg(XLL_LONG, "_Stride", "is the optional step increment. Default is 1.")
	.Arg(XLL_LONG, "_Size", "is the optional number of elements to slice. Default is all.")
	.Category("XLL")
	.FunctionHelp("Slice elements from a Range.")
);
extern "C" LPOPER __declspec(dllexport) WINAPI xll_range_slice(const LPOPER pr, LONG start, LONG stride, LONG size)
{
	static OPER o;

	try {
		handle<OPER> po(o = *pr, false);

		if (po)
			*po = range::slice(*po, start, size, stride);
		else
			o = range::slice(*pr, start, size, stride);
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return &o;
}
static AddIn12 xai_range_slice12(
	Function12(XLL_LPOPER12, XLL_DECORATE12(L"xll_range_slice12",16), L"RANGE.SLICE")
	.Arg(XLL_LPOPER12, L"Range", L"is a range or handle to a range.")
	.Arg(XLL_LONG12, L"Start", L"is the 0-based first element.")
	.Arg(XLL_LONG12, L"_Size", L"is the optional number of elements to slice. Default is all.")
	.Arg(XLL_LONG12, L"_Stride", L"is the optional step increment. Default is 1.")
	.Category(L"XLL")
	.FunctionHelp(L"Slice elements from a Range.")
);
extern "C" LPOPER12 __declspec(dllexport) WINAPI xll_range_slice12(const LPOPER12 pr, LONG start, LONG size, LONG stride)
{
	static OPER12 o;

	try {
		handle<OPER12> po(o = *pr, false);

		if (po)
			*po = range::slice(*po, start, size, stride);
		else
			o = range::slice(*pr, start, size, stride);
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return &o;
}

#ifdef _DEBUG

int xll_test_range_slice()
{
	try {
		OPERX o(1,6);
		for (xword i = 0; i < 6; ++i)
			o[i] = i;

		OPERX o1 = range::slice(o,0,0,0);
		ensure (o1 == o);
		
		o.resize(6,1);	
		o1 = range::slice(o,0,0,0);
		ensure (o1 == o);

		o1 = range::slice(o,1,0,0);
		ensure (o1.rows() == 5 && o1.columns() == 1);
		ensure (o1[0] == 1 && o1[4] == 5);

		o1 = range::slice(o,1,0,2);
		ensure (o1.rows() == 3 && o1.columns() == 1);
		ensure (o1[0] == 1 && o1[2] == 5);

		o1 = range::slice(o,-1,0,-2);
		ensure (o1.rows() == 3 && o1.columns() == 1);
		ensure (o1[0] == 5 && o1[2] == 1);
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());

		return 0;
	}

	return 1;
}
static Auto<OpenX> xao_test_range_slice(xll_test_range_slice);

#endif // _DEBUG