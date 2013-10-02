// loper.h - Return values from Excel* functions that call xlFree in the destructor.
// Copyright (c) KALX, LLC. All rights reserved. No warranty made.
// included by auto.h

template<class X>
class LXOPER : public X {
	LXOPER(const LXOPER&);
	LXOPER& operator=(const LXOPER&);
	LXOPER(const X& x);
	LXOPER& operator=(const X& x);
public:
	LXOPER()
	{
		ZeroMemory(this, sizeof(*this));
		xltype = xltypeNil;
	}
	LXOPER(LXOPER&& o)
	{
		xltype = o.xltype;
		val = o.val;

//		ZeroMemory(&o, sizeof(o));
		o.xltype = xltypeMissing;
	}
	LXOPER& operator=(LXOPER&& o)
	{
		xltype = o.xltype;
		val = o.val;

//		ZeroMemory(&o, sizeof(o));
		o.xltype = xltypeMissing;

		return *this;
	}
	~LXOPER()
	{
		if (!(xltype&xlbitXLFree))
			ExcelX(xlFree, 0, 1, this);
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
	bool operator==(xlret type) const
	{
		return xltype == xltypeInt && val.w == static_cast<xll::traits<X>::xint>(type);
	}

	// For thread-safe functions:
	// LPXLOPERX WINAPI foo(...) { static LOPERX o; ... ; return o.XLFree() }
	X* XLFree()
	{
		xltype |= xlbitXLFree;

		return static_cast<X*>(this);
	}
};

typedef LXOPER<XLOPER>   LOPER;
typedef LXOPER<XLOPER12> LOPER12;
typedef LXOPER<XLOPERX>  LOPERX;

#include "excel.h"