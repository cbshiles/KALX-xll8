	// traits.cpp - test xll::traits
// Copyright (c) KALX, LLC. All rights reserved. No warranty made.
#include "../xll/xll.h"

using namespace xll;

int xll_test_traits(void)
{
	try {
		ensure (0 == traits<XLOPER>::strnicmp("Foobar", "fOOhar", 3));
		ensure (0 != traits<XLOPER>::strnicmp("Foobar", "fOOHar", 4));
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());

		return 0;
	}

	return 1;
}
static Auto<Open> xao_test_traits(xll_test_traits);