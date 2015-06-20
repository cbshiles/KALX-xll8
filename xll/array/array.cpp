// array.cpp - FP data type 2-d arrays of doubles
#include "array.h"

using namespace xll;

static AddIn12 xai_array_get12(
	Function12(XLL_FP12, XLL_DECORATE12(L"xll_array_get12",8), L"ARRAY.GET")
	.Arg(XLL_HANDLE12, L"handle", L"is a handle to an array.")
	.Category(L"XLL")
	.FunctionHelp(L"Return an array given a handle.")
);
extern "C" __declspec(dllexport) xfp12*  WINAPI xll_array_get12(HANDLEX h)
{
	xfp12* pa{0};

	try {
		handle<xll::FP12> ha(h);
		pa = ha.ptr()->get();
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return pa;
}

static AddIn12 xai_array_set12(
	Function12(XLL_HANDLE12, XLL_DECORATE12(L"xll_array_set12",4), L"ARRAY.SET")
	.Arg(XLL_FP12, L"array", L"is an array of numbers.")
	.Uncalced()
	.Category(L"XLL")
	.FunctionHelp(L"Return a handle given an array.")
);
extern "C" __declspec(dllexport) HANDLEX WINAPI xll_array_set12(const xfp12* pa)
{
	handlex h;

	try {
		handle<xll::FP12> ha(new xll::FP12(*pa));
		h = ha.get();
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return h;
}

#ifdef _DEBUG

template<class X>
void xll_test_array_(void)
{
}
int xll_test_array(void)
{
	try {
		xll_test_array_<XLOPER>();
		xll_test_array_<XLOPER12>();
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());

		return 0;
	}

	return 1;
}

static Auto<OpenAfter> xao_test_array_mask(xll_test_array);

#endif // _DEBUG