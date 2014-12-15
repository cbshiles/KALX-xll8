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
		handle<OPER> po(*pr, false);

		if (po)
			*po = range::mask(*po, *pm);
		else
			o = range::mask(*pr, *pm);
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return &o;
}

static AddIn12 xai_range_mask12(
	Function12(XLL_LPOPER12, XLL_DECORATE12(L"xll_range_mask12",8), L"RANGE.MASK")
	.Arg(XLL_LPOPER12, L"Mask", L"is a range.")
	.Arg(XLL_LPOPER12, L"Range", L"is a range or handle to a range.")
	.Category(L"XLL")
	.FunctionHelp(L"Mask elements of Range.")
);
extern "C" LPOPER12 __declspec(dllexport) WINAPI xll_range_mask12(const LPOPER12 pm, const LPOPER12 pr)
{
	static OPER12 o;

	try {
		handle<OPER12> po(o = *pr, false);

		if (po)
			*po = range::mask(*po, *pm);
		else
			o = range::mask(*pr, *pm);
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
	XOPER<X> zero(0);
	XOPER<X> one(1);
	XOPER<X> o { XOPER<X>(1.23), XOPER<X>(xlerr::NA), XOPER<X>(true) };

	XOPER<X> o2 = range::mask(o, XOPER<X>{one,one,zero});
	ensure (o2.size() == 2);
	ensure (o2[0] == 1.23);
	ensure (o2[1] == XOPER<X>(xlerr::NA));

	XOPER<X> o3 = range::mask(o, XOPER<X>{one,one,one});
	ensure (o == o3);

	XOPER<X> o4 = range::mask(o, XOPER<X>{zero,zero,zero});
	ensure (o4 == XOPER<X>(xlerr::NA));
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