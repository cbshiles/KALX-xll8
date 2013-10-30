// dual.cpp - test dual numbers
#include "dual.h"
#include "../xll/xll.h"

using namespace dual;
using namespace xll;

void xll_test_number(void)
{
	number<std::vector<double>> a(std::vector<double>(3));
//	number<std::vector<double>> b = {1,2,3};

//	ensure (b.size() == 3);
//	a += b;

}

int xll_test_dual(void)
{
	try {
		xll_test_array_wrapper();
		xll_test_number();
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());

		return 0;
	}

	return 1;
}