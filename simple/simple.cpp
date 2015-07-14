// simple.cpp - a simple xll
//#define EXCEL12
#include "../xll/udp.h"
#include "../xll/xll.h"

using namespace xll;

AddIn12 xai_foo(
	L"?xll_foo", XLL_DOUBLE12 XLL_DOUBLE12,
	L"FOO", L"x"
);
double WINAPI xll_foo(double x)
{
#pragma XLLEXPORT
	udp::sendto(__FUNCTION__);

	return 2*x;
}
