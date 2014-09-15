// handle.cpp - test handle class
#include "xll/xll.h"

using namespace xll;

class Base {
	int i_;
public:
	Base(int i)
		: i_(i)
	{ }
	virtual ~Base() // for RTTI
	{ }
	int value(void) const
	{
		return i_;
	}
};

static AddInX xai_base(
	FunctionX(XLL_HANDLEX, _T("?xll_base"), _T("XLL.BASE"))
	.Arg(XLL_LONGX, _T("Value"), _T("is an integer value."))
	.FunctionHelp(_T("Return a handle to a Base object."))
	.Uncalced()
	.Category(_T("XLL"))
);
HANDLEX WINAPI xll_base(LONG v)
{
#pragma XLLEXPORT
		handlex h; // a HANDLEX that defaults to a NaN that returns #NUM!

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

class Derived : public Base {
	int i2_;
public:
	Derived(int i, int i2)
		: Base(i), i2_(i2)
	{ }
	int value2(void) const
	{
		return i2_;
	}
};

// Put derived classes into the Base bucket.
static AddInX xai_derived(
	FunctionX(XLL_HANDLEX, _T("?xll_derived"), _T("XLL.DERIVED"))
	.Arg(XLL_LONGX, _T("Value"), _T("is an integer value for the base class."))
	.Arg(XLL_LONGX, _T("Value2"), _T("is an integer value for the derived class."))
	.Uncalced()
	.Category(_T("XLL"))
	.FunctionHelp(_T("Return a handle to a Derived object."))
);
HANDLEX WINAPI xll_derived(LONG v, LONG v2)
{
#pragma XLLEXPORT
		handlex h;

		try {
			// put derived into Base bag
			handle<Base> h_(new Derived(v, v2));

			h = h_.get();
		}
		catch (const std::exception& ex) {
			XLL_ERROR(ex.what());
		}

		return h;
}

// Use dynamic cast to get the Derived from the Base.
static AddInX xai_derived_value(
	FunctionX(XLL_LONGX, "?xll_derived_value2", "XLL.DERIVED.VALUE2")
	.Arg(XLL_HANDLEX, _T("Handle"), _T("is a handle returned by XLL.DERIVED."))
	.Category(_T("XLL"))
	.FunctionHelp(_T("Return the second value of derived class."))
);
LONG WINAPI xll_derived_value2(HANDLEX h)
{
#pragma XLLEXPORT
	LONG v(0);

	try {
		handle<Base> h_(h);
		Derived *pd = dynamic_cast<Derived*>(h_.ptr());
		ensure (pd != 0);

		v = pd->value2();
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return v;
}
