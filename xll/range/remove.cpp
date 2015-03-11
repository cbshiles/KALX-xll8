// remove.cpp - remove items from range based on xltype
#include "range.h"

using namespace xll;

XLL_ENUM(xltypeNum, XLTYPE_NUM, "XLL", "Enumeration for floating point numbers")
XLL_ENUM(xltypeStr, XLTYPE_STR, "XLL", "Enumeration for strings")
XLL_ENUM(xltypeBool, XLTYPE_BOOL, "XLL", "Enumeration for booleans")
XLL_ENUM(xltypeErr, XLTYPE_ERR, "XLL", "Enumeration for errors")
XLL_ENUM(xltypeMissing, XLTYPE_MISSING, "XLL", "Enumeration for missing elements")
XLL_ENUM(xltypeNil, XLTYPE_NIL, "XLL", "Enumeration for floating point numbers")
XLL_ENUM(xltypeInt, XLTYPE_INT, "XLL", "Enumeration for integers")

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
