// simple.cpp - a simple xll
//#define EXCEL12
#include "../xll/xll.h"
#include "../xll/utility/inet.h"
#include "../xll/utility/google.h"

using namespace xll;

int xll_simple(void)
{
	OPERX o;
	o = XLL_XLF(GetWorkspace, OPERX(23)); // startup dir
	o = XLL_XLF(GetWorkspace, OPERX(24)); // alt dir
	o = XLL_XLF(GetWorkspace, OPERX(48)); // library dir
	o = o;

	xll_alert_level |= XLL_ALERT_LOG;
	XLL_WARNING("testing");

	return 1;
}
static Auto<OpenAfterX> xao_simple(xll_simple);
