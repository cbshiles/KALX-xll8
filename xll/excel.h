// excel.h - Wrapper functions for Excel* functions.
// Copyright (c) KALX, LLC. All rights reserved. No warranty made.
// included by loper.h
/*
template<class.. Ts> void fun(Ts... vs)
{
	// capture args and do something with them later
	auto g = [&vs...] { return gun(vs...); }
	g();
}
*/

// Convenience wrappers for Excel calls
#define XLL_XLF(fn, ...) Excel<XLOPERX>(xlf##fn, __VA_ARGS__)
#define XLL_XLC(fn, ...) Excel<XLOPERX>(xlc##fn, __VA_ARGS__)
#define XLL_XL_(fn, ...) Excel<XLOPERX>(xl##fn, __VA_ARGS__)

namespace xll {

	// requires variadic templates and due amazement
	template<class X, typename... Args>
	inline LXOPER<X> Excel(int xlf, const Args&... args)
	{
		LXOPER<X> x;
		
		int ret = traits<X>::Excel(xlf, &x, sizeof...(args), &args...);
		if (ret != xlretSuccess)
			return LXOPER<X>(static_cast<xlret>(ret));

		return x;
	}

}

#include "oper.h"
