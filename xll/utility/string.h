// string.h - string manipulation
#pragma once
#include <string>
#include "Stringapiset.h"

inline std::basic_string<wchar_t> MultiByteToWideChar(const char* s, int n = 0)
{

	if (n == 0)
		n = static_cast<int>(::strlen(s));
		
	int n_ = MultiByteToWideChar(CP_ACP, 0, s, n, 0, 0);
	std::basic_string<wchar_t> s_(n, 0);
	MultiByteToWideChar(CP_ACP, 0, s, n, &s_[0], n_);

	return s_;
}

inline std::basic_string<char> WideCharToMultiByte(const wchar_t* s, int n = 0)
{
	if (n == 0)
		n = static_cast<int>(::wcslen(s));
		
	int n_ = WideCharToMultiByte(CP_ACP, 0, s, n, 0, 0, 0, 0);
	std::string s_(n_, 0);
	WideCharToMultiByte(CP_ACP, 0, s, n, &s_[0], n_, 0, 0);

	return s_;
}

#ifdef _DEBUG
#include <cassert>

inline void test_string(void)
{
	std::wstring s = MultiByteToWideChar("foo");
	assert (s == L"foo");

	std::string t = WideCharToMultiByte(L"bar");
	assert (t == "bar");

}

#endif // _DEBUG