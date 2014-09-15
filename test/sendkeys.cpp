// sendkeys.cpp - xlcSendKeys replacement
//#define EXCEL12
#include "../xll/xll.h"
#include "../xll/utility/sendkeys.h"

using namespace xll;

static AddInX xai_send_keys(_T("?xll_send_keys"), _T("XLL.SEND.KEYS"));
int WINAPI xll_send_keys(void)
{
#pragma XLLEXPORT
	// CalculateFullRebuild
	// Ctrl-Alt-F9
//	return Win::SendKeys({/*VK_CONTROL, VK_MENU,*/ VK_F9});
	return Win::SendString("aBc");
}