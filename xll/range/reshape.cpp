// reshape.cpp - reshape a range
#include "range.h"

using namespace xll;

static AddIn xai_range_reshape(
	Function(XLL_LPOPER, XLL_DECORATE("xll_range_reshape",12), "RANGE.RESHAPE")
	.Arg(XLL_LPOPER, "Range", "is a range or handle to a range.")
	.Arg(XLL_WORD, "Rows", "is the number of rows in the output range.")
	.Arg(XLL_WORD, "Columns", "is the number of columns in the output range.")
	.Category("XLL")
	.FunctionHelp("Reshape Range to Rows and Columns.")
);
extern "C" LPOPER __declspec(dllexport) WINAPI xll_range_reshape(const LPOPER pr, WORD r, WORD c)
{
	static OPER o;

	try {
		handle<OPER> po(o = *pr, false);

		if (po) {
			po->resize(r, c);
		}
		else {
			o.resize(r,c);
		}

	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return &o;
}

static AddIn12 xai_range_reshape12(
	Function12(XLL_LPOPER12, XLL_DECORATE12(L"xll_range_reshape12",12), L"RANGE.RESHAPE")
	.Arg(XLL_LPOPER12, L"Range", L"is a range or handle to a range.")
	.Arg(XLL_WORD12, L"Rows", L"is the number of rows in the output range.")
	.Arg(XLL_WORD12, L"Columns", L"is the number of columns in the output range.")
	.Category(L"XLL")
	.FunctionHelp(L"Reshape Range to Rows and Columns.")
);
extern "C" LPOPER12 __declspec(dllexport) WINAPI xll_range_reshape12(const LPOPER12 pr, WORD r, WORD c)
{
	static OPER12 o;

	try {
		handle<OPER12> po(o = *pr, false);

		if (po) {
			po->resize(r, c);
		}
		else {
			o.resize(r,c);
		}

	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return &o;
}
