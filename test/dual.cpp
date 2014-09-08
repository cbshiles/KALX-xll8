// dual.cpp - test dual numbers
#include "dual.h"
#include "../xll/xll.h"

using namespace dual;
using namespace xll;

template<class T>
void xll_test_constructor(void)
{
	dual::number<int> x{1,2,3};

	ensure (x.size() == 3);
	ensure (x[0] == 1);
	ensure (x[1] == 2);
	ensure (x[2] == 3);

	x[1] = 4;
	ensure (x[1] == 4);

}

template<class T>
void xll_test_dual_add(void)
{
	T x[] = {1, 2, 3};
	T y[] = {4, 5, 6};
	
	op_add<T>(3, x, 1);
	ensure (x[0] == 2);
	ensure (x[1] == 3);
	ensure (x[2] == 4);
	op_add<T>(3, x, -1);

	op_add(3, x, y);
	ensure (x[0] == 5);
	ensure (x[1] == 7);
	ensure (x[2] == 9);
}
template<class T>
void xll_test_dual_mul(void)
{
	T x[] = {1, 2, 3};
	T y[] = {4, 5, 6};

	op_mul(3, x, 2);
	ensure (x[0] == 2);
	ensure (x[1] == 4);
	ensure (x[2] == 6);
	op_mul(3, x, 0.5);
	ensure (x[0] == 1);
	ensure (x[1] == 2);
	ensure (x[2] == 3);

	op_mul(3, x, 0);
	ensure (x[0] == 1);
	ensure (x[1] == 2);
	ensure (x[2] == 3);

	op_mul(3, x, y, 1);
	ensure (x[0] == 4);
	ensure (x[1] == 8);
	ensure (x[2] == 12);
	op_mul(3, x, 0.25);

	op_mul(3, x, y);
	ensure (x[0] == 1*4);
	ensure (x[1] == 1*5 + 2*4);
	ensure (x[2] == 1*6 + 2*5 + 3*4);

}
int xll_test_dual(void)
{
	try {
		xll_test_constructor<double>();
		xll_test_dual_add<double>();
//		xll_test_dual_mul<double>();
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());

		return 0;
	}

	return 1;
}
static Auto<OpenX> xao_test_dual(xll_test_dual);