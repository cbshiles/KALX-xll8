// loper.h - Return values from Excel* functions that call xlFree in the destructor.
// Copyright (c) KALX, LLC. All rights reserved. No warranty made.
// included by auto.h

template<class X>
class LXOPER : public X {
	LXOPER(const LXOPER&);
	LXOPER& operator=(const LXOPER&);
	LXOPER(const X& x);
	LXOPER& operator=(const X& x);
	bool owner_;
public:
	LXOPER()
		: owner_(true)
	{
		ZeroMemory(this, sizeof(*this));
		xltype = xltypeNil;
	}
	LXOPER(LXOPER&& o)
	{
		owner_ = true;
		xltype = o.xltype;
		val = o.val;

		o.owner_ = false;

	}
	LXOPER& operator=(LXOPER&& o)
	{
		owner_ = true;
		xltype = o.xltype;
		val = o.val;

		o.owner_ = false;

		return *this;
	}
	~LXOPER()
	{
		if (owner_)
			xll::traits<X>::Excel(xlFree, 0, 1, this);
	}

	// Return xlret types if not xlretSuccess
	LXOPER(xlret type)
	{
		xltype = xltypeInt; // not used elsewhere
		val.w = static_cast<xll::traits<X>::xint>(type);
	}
	bool operator!(void) const
	{
		return !static_cast<const XOPER<X>&>(*this);
	}
	operator bool() const
	{
		return !!*this;
	}
	bool operator==(double num) const
	{
		return xltype == xltypeNum && val.num == num;
	}

	// For thread-safe functions:
	// LPXLOPERX WINAPI foo(...) { static LOPERX o; ... ; return o.XLFree() }
	X* XLFree()
	{
		owner_ = false;
		xltype |= xlbitXLFree;

		return static_cast<X*>(this);
	}
};

typedef LXOPER<XLOPER>   LOPER;
typedef LXOPER<XLOPER12> LOPER12;
typedef LXOPER<XLOPERX>  LOPERX;

#include "excel.h"