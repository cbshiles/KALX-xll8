// range.cpp - universal range functions
#include "range.h"

using namespace xll;

static AddIn xai_range_set(
	Function(XLL_HANDLE, "_xll_range_set@4", "RANGE.SET")
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
	Function(XLL_LPOPER, "_xll_range_get@8", "RANGE.GET")
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
