// simple.cpp - a simple xll
//#define EXCEL12
#include "../xll/xll.h"

using namespace xll;

int xll_simple(void)
{
	try {
		OPERX o;
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());

		return FALSE;
	}

	return TRUE;
}
static Auto<OpenAfterX> xao_simple(xll_simple);
