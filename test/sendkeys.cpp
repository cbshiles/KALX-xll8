// sendkeys.cpp - xlcSendKeys replacement
//#define EXCEL12
#include "../xll/xll.h"

using namespace xll;

//SHORT virtKey = VkKeyScan((TCHAR) chr);
//input[nInput].ki.wVk = LOBYTE(virtKey);

struct keybdinput : public INPUT {
	keybdinput(WORD vk = 0, DWORD flags = 0)
	{
		type = INPUT_KEYBOARD;
		ki.wVk = vk;
		ki.wScan = 0;
		ki.dwFlags = flags;
		ki.time = 0;
		ki.dwExtraInfo = 0;
	}
	~keybdinput()
	{ }
	void vk(WORD k)
	{
		ki.wVk = k;
	}
	keybdinput& operator=(WORD k)
	{
		vk(k);

		return *this;
	}
	void flags(DWORD f)
	{
		ki.dwFlags = f;
	}
	keybdinput& operator|=(DWORD f)
	{
		ki.dwFlags |= f;

		return *this;
	}
};

class Input {
	std::vector<keybdinput> i_;
public:
	Input& Type(WORD vk)
	{
		i_.push_back(keybdinput(vk));
		i_.push_back(keybdinput(vk) |= KEYEVENTF_KEYUP);

		return *this;
	}
};

static AddInX xai_send_keys(_T("?xll_send_keys"), _T("XLL.SEND.KEYS"));
int WINAPI xll_send_keys(void)
{
#pragma XLLEXPORT
	// CalculateFullRebuild
	// Ctrl-Alt-F9
	keybdinput input[6];

	input[0] = VK_CONTROL;
	input[1] = VK_MENU;
	input[2] = VK_F9;
	input[3] = VK_F9;
	input[3] |= KEYEVENTF_KEYUP;
	input[4] = VK_MENU;
	input[4] |= KEYEVENTF_KEYUP;
	input[5] = VK_CONTROL;
	input[5] |= KEYEVENTF_KEYUP;

	int retval = SendInput(6, input, sizeof(INPUT));

	return retval > 0;
}