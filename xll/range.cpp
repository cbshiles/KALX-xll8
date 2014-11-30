// range.cpp - universal range functions
#include "range.h"

using namespace xll;

static AddIn xai_range_set(
	Function(XLL_HANDLE, XLL_DECORATE("xll_range_set",4), "RANGE.SET")
	.Arg(XLL_LPOPER, "Range", "is a range.")
	.Uncalced()
	.Category("XLL")
	.FunctionHelp("Return a handle to a range.")
);
extern "C" HANDLEX __declspec(dllexport) WINAPI xll_range_set(LPOPER po)
{
	handle<OPER> ho(new OPER(*po));

	return ho.get();
}

static AddIn xai_range_get(
	Function(XLL_LPOPER, XLL_DECORATE("xll_range_get",8), "RANGE.GET")
	.Arg(XLL_HANDLE, "Handle", "is a handle to a range.")
	.Category("XLL")
	.FunctionHelp("Return a range corresponding to handle.")
);
extern "C" LPOPER __declspec(dllexport) WINAPI xll_range_get(HANDLEX h)
{
	LPOPER po(0);

	try {
		po = handle<OPER>(h).ptr();
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return po;
}

static AddIn12 xai_range_set12(
	Function12(XLL_HANDLE12, XLL_DECORATE12(L"xll_range_set12",4), L"RANGE.SET")
	.Arg(XLL_LPOPER12, L"Range", L"is a range.")
	.Uncalced()
	.Category(L"XLL")
	.FunctionHelp(L"Return a handle to a range.")
);
extern "C" HANDLEX __declspec(dllexport) WINAPI xll_range_set12(LPOPER12 po)
{
	handle<OPER12> ho(new OPER12(*po));

	return ho.get();
}

static AddIn12 xai_range_get12(
	Function12(XLL_LPOPER12, XLL_DECORATE12(L"xll_range_get12",8), L"RANGE.GET")
	.Arg(XLL_HANDLE12, L"Handle", L"is a handle to a range.")
	.Category(L"XLL")
	.FunctionHelp(L"Return a range corresponding to handle.")
);
extern "C" LPOPER12 __declspec(dllexport) WINAPI xll_range_get12(HANDLEX h)
{
	LPOPER12 po(0);

	try {
		po = handle<OPER12>(h).ptr();
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return po;
}


static AddIn xai_range_mask(
	Function(XLL_LPOPER, XLL_DECORATE("xll_range_mask",8), "RANGE.MASK")
	.Arg(XLL_LPOPER, "Range", "is a range or handle to a range.")
	.Arg(XLL_LPOPER, "Mask", "is a range.")
	.Category("XL")
	.FunctionHelp("Mask elements of Range.")
);
extern "C" LPOPER __declspec(dllexport) WINAPI xll_range_mask(const LPOPER pr, const LPOPER pm)
{
	static OPER o;

	try {
		o = range::mask(*pr, *pm);
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return &o;
}
static AddIn12 xai_range_mask12(
	Function12(XLL_LPOPER12, XLL_DECORATE12(L"xll_range_mask12",8), L"RANGE.MASK")
	.Arg(XLL_LPOPER12, L"Range", L"is a range or handle to a range.")
	.Arg(XLL_LPOPER12, L"Mask", L"is a range.")
	.Category(L"XLL")
	.FunctionHelp(L"Mask elements of Range.")
);
extern "C" LPOPER12 __declspec(dllexport) WINAPI xll_range_mask12(const LPOPER12 pr, const LPOPER12 pm)
{
	static OPER12 o;

	try {
		o = range::mask(*pr, *pm);
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return &o;
}
