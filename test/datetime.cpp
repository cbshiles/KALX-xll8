// datetime.cpp - test date and time routines
#include "../xll/xll.h"

inline int uniform_int(int a, int b)
{
	OPERX o = XLL_XLF(Randbetween, OPERX(a), OPERX(b));

	return static_cast<int>(o.val.num);
}
using namespace xll;

void test_excel2time_t(void)
{
	OPERX y, m, d, h, n, s;

	for (int i = 0; i < 1000; ++i) {
		y = uniform_int(1970, 2070);
		m = uniform_int(1, 12);
		d = uniform_int(1, 30);
		h = uniform_int(0, 23);
		n = uniform_int(0,59);
		s = uniform_int(0,59);

		OPERX xDate = XLL_XLF(Date, y, m, d);
		xDate += XLL_XLF(Time, h, n, s);
		ensure (xDate.xltype == xltypeNum);
		time_t t = xll::excel2time_t(xDate.val.num, true); // no DST

		tm tm;
		ZeroMemory(&tm, sizeof(tm));
		tm.tm_year = static_cast<int>(y - 1900);
		tm.tm_mon = static_cast<int>(m - 1);
		tm.tm_mday = static_cast<int>(d);
		tm.tm_hour = static_cast<int>(h);
		tm.tm_min = static_cast<int>(n);
		tm.tm_sec = static_cast<int>(s);
		time_t te = ::mktime(&tm);
		ensure (t == te);
	}
}

int xll_test_datetime()
{
	try {
		test_excel2time_t();
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());

		return 0;
	}

	return 1;
}
static Auto<OpenAfterX> xao_test_datetime(xll_test_datetime);