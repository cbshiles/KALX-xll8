// array_mask.cpp - mask an array of numbers
#include "array.h"

using namespace xll;

static AddIn xai_array_mask(
	Function(XLL_FP, XLL_DECORATE("xll_array_mask",8), "ARRAY.MASK")
	.Arg(XLL_FP, "mask", "is a array.")
	.Arg(XLL_FP, "array", "is a array or handle to a array.")
	.Category("XLL")
	.FunctionHelp("Mask elements of array.")
);
extern "C" __declspec(dllexport) xfp* WINAPI xll_array_mask(const xfp* pm, const xfp* pr)
{
	static xll::FP o;

	try {
		o = pr->array[0]; // if handle
		handle<xll::FP> po(o[0],false);

		if (po)
			*po = ::array::mask<XLOPER>(*po.ptr()->get(), *pm);
		else
			o = ::array::mask<XLOPER>(*pr, *pm);
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return o.get();
}

static AddIn12 xai_array_mask12(
	Function12(XLL_FP12, XLL_DECORATE12(L"xll_array_mask12",8), L"ARRAY.MASK")
	.Arg(XLL_FP12, L"mask", L"is a array.")
	.Arg(XLL_FP12, L"array", L"is a array or handle to a array.")
	.Category(L"XLL")
	.FunctionHelp(L"Mask elements of array.")
);
extern "C" __declspec(dllexport) xfp12*  WINAPI xll_array_mask12(const xfp12* pm, const xfp12* pr)
{
	static xll::FP12 o;

	try {
		o = pr->array[0]; // if handle
		handle<xll::FP12> po(o[0],false);

		if (po)
			*po = ::array::mask<XLOPER12>(*po.ptr()->get(), *pm);
		else
			o = ::array::mask<XLOPER12>(*pr, *pm);
		}
		catch (const std::exception& ex) {
			XLL_ERROR(ex.what());
		}

	return o.get();
}

#ifdef _DEBUG

template<class X>
void xll_test_array_mask_(void)
{
	XFP<X> o { 1.23, 0, 1 };

	XFP<X> m110{1,1,0};
	XFP<X> o2 = array::mask<X>(*o.get(), *m110.get());
	ensure (o2.size() == 2);
	ensure (o2[0] == 1.23);
	ensure (o2[1] == 0);

	XFP<X> m111{1,1,1};
	XFP<X> o3 = array::mask<X>(*o.get(), *m111.get());
	ensure (o == o3);

	XFP<X> m000{0,0,0};
	XFP<X> o4 = array::mask<X>(*o.get(), *m000.get());
	ensure (o4 == XFP<X>{});
}
int xll_test_array_mask(void)
{
	try {
		xll_test_array_mask_<XLOPER>();
		xll_test_array_mask_<XLOPER12>();
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());

		return 0;
	}

	return 1;
}

static Auto<OpenAfter> xao_test_array_mask(xll_test_array_mask);

#endif // _DEBUG