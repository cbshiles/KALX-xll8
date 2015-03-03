// conversion.cpp - test conversions
#include <random>
#include "test.h"

using namespace xll;

int xll_test_conversion(void)
{
	try {
		std::default_random_engine dre;
		std::uniform_real_distribution<> urd(-1,1);

		// num
		for (int i = 0; i < 100; ++i) {
			double x = 1/urd(dre);
			OPERX X(x);
			OPERX X_ = xll::name<XLOPERX>(X);
			X_ = XLL_XLF(Evaluate, X_);

//			ensure (X_ == X);
		}
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());

		return FALSE;
	}

	return TRUE;
}

static Auto<OpenAfterX> xao_test_converstion(xll_test_conversion);