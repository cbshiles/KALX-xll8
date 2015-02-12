// excel.h - Wrapper functions for Excel* functions.
// Copyright (c) KALX, LLC. All rights reserved. No warranty made.
// included by loper.h
#include <array>

namespace xll {

	// requires variadic templates and due amazement
	template<class X, typename... Args>
	inline LXOPER<X> Excel(int xlf, const Args&... args)
	{
		LXOPER<X> x;
		
		ensure (xlretSuccess == traits<X>::Excel(xlf, &x, sizeof...(args), &args...));

		return x;
	}
}

#include "oper.h"
