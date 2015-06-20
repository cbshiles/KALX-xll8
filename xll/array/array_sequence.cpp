// array_sequence.cpp - sequence of numbers
#include "array.h"

using namespace xll;

static AddIn12 xai_array_sequence(
	Function12(XLL_FP12, XLL_DECORATE12(L"xll_array_sequence12",24), L"ARRAY.SEQUENCE")
	.Arg(XLL_DOUBLE12, L"start", L"is the first number in the sequence.")
	.Arg(XLL_DOUBLE12, L"step", L"is size of each increment in the sequence.")
	.Arg(XLL_WORD12, L"rows", L"is the number of rows in the resulting array.")
	.Arg(XLL_WORD12, L"_columns", L"is the optional number of columns. Default is 1.")
	.Category(L"XLL")
	.FunctionHelp(L"Return an array of numbers.")
	.Documentation()
);
extern "C" __declspec(dllexport) xfp12* WINAPI xll_array_sequence12(double s, double ds, INT32 r, INT32 c)
{
	xfp12* pa{0};

	try {
		if (r == 0)
			r = 1;
		if (c == 0)
			c = 1;

		static xll::FP12 a;
		a.reshape(r, c);
		for (xword i = 0; i < a.size(); ++i) {
			a[i] = s + i*ds;
		}

		pa = a.get();
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
		pa = 0;
	}

	return pa;
}
