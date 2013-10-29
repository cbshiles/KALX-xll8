// types.h - Excel OPER types
// Copyright (c) KALX, LLC. All rights reserved. No warranty made.
// included by traits.h

namespace xll {

	template<class X>
	struct Num {
	};

	template<>
	struct Num<XLOPER> {
		typedef double type;
		const char* name() { return "Number"; }
		const char* help() { return "is a number."; }
		const char* code() { return XLL_DOUBLE; }
	};
	template<>

	struct Num<XLOPER12> {
		typedef double type;
		const XCHAR* name() { return L"Number"; }
		const XCHAR* help() { return L"is a number."; }
		const XCHAR* code() { return XLL_DOUBLE12; }
	};

} // namespace xll

#include "xloper.h"

