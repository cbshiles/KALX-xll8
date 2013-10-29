// handle.cpp - test handle class
#include "xll/xll.h"
#include "xll/handle.h"

using namespace xll;

class Base {
	int i_;
public:
	Base(int i)
		: i_(i)
	{ }
	virtual ~Base()
	{ }
	int value(void) const
	{
		return i_;
	}
};

static AddInX xai_base(
	FunctionX(XLL_HANDLEX, _T("?xll_base"), _T("XLL.BASE"))
	.Arg(XLL_LONGX, _T("Value"), _T("is an integer value."))
	.Uncalced()
	.Category(_T("XLL"))
);
HANDLEX WINAPI xll_base(LONG v)
{
#pragma XLLEXPORT
		HANDLEX h(0);

		try {
			handle<Base> h_(new Base(v));

			h = h_.get();
		}
		catch (const std::exception& ex) {
			XLL_ERROR(ex.what());
		}

		return h;
}

static AddInX xai_base_value(
	FunctionX(XLL_LONGX, "?xll_base_value", "XLL.BASE.VALUE")
	.Arg(XLL_HANDLEX, _T("Handle"), _T("is a handle returned by XLL.BASE."))
	.Category(_T("XLL"))
	.FunctionHelp(_T("Return the value of base class."))
);
LONG WINAPI xll_base_value(HANDLEX h)
{
#pragma XLLEXPORT
	LONG v(0);

	try {
		handle<Base> h_(h);

		v = h_->value();
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return v;
}

