// iterator.cpp - test iterator class
#include "../xll/xll.h"
#include "iterator.h"

using namespace xll;

void xll_test_input(void)
{
	double a[] = {0, 1, 2, 3, 4, 5};
	iterator::input<double> i(a, 2); // 2 x 3 range
	ensure (*i == *iterator::input<double>(a, 2));
	++i;
	ensure (*i == *iterator::input<double>(a + 2, 2));
	++i;
	ensure (*i == *iterator::input<double>(a + 4, 2));
	*i = iterator::input<double>(a, 2);
	ensure (a[4] == 0);
	ensure (a[5] == 1);

}

int xll_test_iterator(void)
{
	try {
		xll_test_input();
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());

		return 0;
	}

	return 1;
}
static Auto<OpenAfterX> xao_test_iterator(xll_test_iterator);