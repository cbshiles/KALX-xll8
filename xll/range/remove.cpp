// remove.cpp - remove items from range based on xltype
#include "range.h"

using namespace xll;

static AddIn xai_xltype_num(
	Function(XLL_WORD, XLL_DECORATE("xll_alert_error",0), "XLYPE_NUM")
	.Category("XLL")
	.FunctionHelp("Return enumeration for floating point numbers.")
);
extern "C" int __declspec(dllexport) WINAPI xll_xltype_num(void)
{
	return xltypeNum;
}

static AddIn xai_range_remove(
	Function(XLL_LPOPER, XLL_DECORATE("xll_range_remove",8), "RANGE.REMOVE")
	.Arg(XLL_LONG, "Type", "is a mask of XLTYPE_* enumerations.")
	.Arg(XLL_LPOPER, "Range", "is a range or handle to a range.")
	.Category("XLL")
	.FunctionHelp("Remove Type elements of Range.")
);
extern "C" LPOPER __declspec(dllexport) WINAPI xll_range_remove(LONG n, const LPOPER pr)
{
	static OPER o;

	try {
		handle<OPER> po(o = *pr, false);

		if (po)
			*po = range::remove(*po, n);
		else
			o = range::remove(*pr, n);
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return &o;
}

static AddIn12 xai_range_remove12(
	Function12(XLL_LPOPER12, XLL_DECORATE12(L"xll_range_remove12",8), L"RANGE.REMOVE")
	.Arg(XLL_LONG12, L"Type", L"is a mask of XLTYPE_* enumerations.")
	.Arg(XLL_LPOPER12, L"Range", L"is a range or handle to a range.")
	.Category(L"XLL")
	.FunctionHelp(L"Remove Type elements of Range.")
);
extern "C" LPOPER12 __declspec(dllexport) WINAPI xll_range_remove12(LONG n, const LPOPER12 pr)
{
	static OPER12 o;

	try {
		handle<OPER12> po(o = *pr, false);

		if (po)
			*po = range::remove(*po, n);
		else
			o = range::remove(*pr, n);
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return &o;
}
