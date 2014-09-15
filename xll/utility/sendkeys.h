// sendkeys.cpp - xlcSendKeys replacement
#pragma once
#include <initializer_list>
#include <vector>
#include <Windows.h>
#include <Winuser.h>

namespace Win {

	// keyboard key
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

	// key[0], key[0]-up, ...
	inline UINT SendString(const char* key)
	{
		std::vector<keybdinput> k_;

		while (*key) {
			WORD K = static_cast<WORD>(::toupper(*key));

			if (::isupper(*key)) {
				k_.push_back(keybdinput(VK_SHIFT));
				k_.push_back(keybdinput(K));
				k_.push_back(keybdinput(K, KEYEVENTF_KEYUP));
				k_.push_back(keybdinput(VK_SHIFT, KEYEVENTF_KEYUP));
			}
			else {
				k_.push_back(keybdinput(K));
				k_.push_back(keybdinput(K, KEYEVENTF_KEYUP));
			}
			++key;
		}

		return SendInput(static_cast<UINT>(k_.size()), &k_[0], static_cast<int>(sizeof(keybdinput)));
	}

	using std::operator!=;

	// key[0],..., key[n-1], key[n-1]-up,..., key[0]-up
	inline UINT SendKeys(const std::initializer_list<WORD>& keys)
	{
		std::vector<keybdinput> k_;

		for (auto k = keys.begin(); k != keys.end(); ++k) {
			WORD K = static_cast<WORD>(::toupper(*k));
			k_.push_back(keybdinput(K));
		}


		for (auto k = rbegin(keys); k != rend(keys); ++k) {
			WORD K = static_cast<WORD>(::toupper(*k));
			k_.push_back(keybdinput(K, KEYEVENTF_KEYUP));
		}

		return SendInput(static_cast<UINT>(k_.size()), &k_[0], static_cast<int>(sizeof(keybdinput)));
	}
} // Win
