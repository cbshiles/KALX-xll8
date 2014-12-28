// trim.cpp - remove empty elements from front and back of range
#include "range.h"

using namespace xll;

static AddIn xai_range_trim(
	Function(XLL_LPOPER, XLL_DECORATE("xll_range_trim",8), "RANGE.TRIM")
	.Arg(XLL_LPOPER, "Range", "is a range.")
	.Arg(XLL_BOOL, "_Contiguous", "is an optional argument indicating data are contiguous.")
	.Uncalced()
	.Category("XLL")
	.FunctionHelp("Trim empty cells from front and back of range.")
);
extern "C" LPOPER __declspec(dllexport) WINAPI xll_range_trim(LPOPER po, bool contiguous)
{
	static OPER o;

	try {
		handle<OPER> ho(o = *po, false);

		if (ho)
			*ho = range::trim(*ho, contiguous);
		else
			o = range::trim(*po, contiguous);
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());

		o = OPER(xlerr::NA);
	}

	return &o;
}

static AddIn12 xai_range_trim12(
	Function12(XLL_LPOPER12, XLL_DECORATE12(L"xll_range_trim12",8), L"RANGE.TRIM")
	.Arg(XLL_LPOPER12, L"Range", L"is a range.")
	.Arg(XLL_BOOL12, L"_Contiguous", L"is an optional argument indicating data are contiguous.")
	.Uncalced()
	.Category(L"XLL")
	.FunctionHelp(L"Trim empty cells from front and back of range.")
);
extern "C" LPOPER12 __declspec(dllexport) WINAPI xll_range_trim12(LPOPER12 po, bool contiguous)
{
	static OPER12 o;

	try {
		handle<OPER12> ho(o = *po, false);

		if (ho)
			*ho = range::trim(*ho, contiguous);
		else
			o = range::trim(*po, contiguous);
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());

		o = OPER12(xlerr::NA);
	}

	return &o;
}

#ifdef _DEBUG

int xll_test_range_trim(void)
{
	try {
		OPERX o{OPERX(), OPERX(1),  OPERX()};
		auto o2 = range::trim(o);
		ensure (o2.size() == 1);
		ensure (o2[0] == o[1]);
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());

		return 0;
	}

	return 1;
}
static Auto<OpenX> xao_test_range_trim(xll_test_range_trim);

#endif // _DEBUG