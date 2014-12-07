// mask.cpp - mask ranges
#include "range.h"

using namespace xll;


static AddIn xai_range_mask(
	Function(XLL_LPOPER, XLL_DECORATE("xll_range_mask",8), "RANGE.MASK")
	.Arg(XLL_LPOPER, "Range", "is a range or handle to a range.")
	.Arg(XLL_LPOPER, "Mask", "is a range.")
	.Category("XL")
	.FunctionHelp("Mask elements of Range.")
);
extern "C" LPOPER __declspec(dllexport) WINAPI xll_range_mask(LPOPER pr, const LPOPER pm)
{
	static OPER o;

	try {
		const OPER* po = range::ptr(*pr);
		o = range::mask(po ? *po : *pr, *pm);
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
		const OPER12* po = range::ptr(*pr);
		o = range::mask(po ? *po : *pr, *pm);
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return &o;
}

#ifdef _DEBUG

template<class X>
void xll_test_range_mask_(void)
{
//	XOPER<X> o { XOPER<X>('a'), XOPER<X>('a'), XOPER<X>('a') };
//	ensure (o[0] == 0);
}
int xll_test_range_mask(void)
{
	try {
		xll_test_range_mask_<XLOPER>();
		xll_test_range_mask_<XLOPER12>();
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());

		return 0;
	}

	return 1;
}

static Auto<OpenAfter> xao_test_range_mask(xll_test_range_mask);

#endif // _DEBUG