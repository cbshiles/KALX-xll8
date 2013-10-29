// dual.cpp - test dual numbers
#include "dual.h"
#include "../xll/xll.h"

using namespace dual;
using namespace xll;

void xll_test_array_wrapper(void)
{
	double a_[] = {0, 1, 2};
	array_wrap<double> a(a_, 3);
	ensure (a.data() == a_);
	ensure (a.size() == 3);
	ensure (a.begin() == a.data());
	ensure (a.end() == a.begin() + a.size());
	ensure (a[0] == a_[0]);
	ensure (a[1] == a_[1]);
	ensure (a[2] == a_[2]);

	a[0] = 3;
	ensure (a[0] == 3);

	array_wrap<const double> b = {1,2,3};
	ensure (b.size() == 3);
	ensure (b[0] == 1);
	ensure (b[1] == 2);
	ensure (b[2] == 3);
}

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