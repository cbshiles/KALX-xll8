// traits.h - parameterize based on XLOPER and XLOPER12 from the Microsoft SDK.
// Copyright (c) KALX, LLC. All rights reserved. No warranty made.
// included by defines.h

namespace xll {

	template<class X>
	struct traits {};

	template<>
	struct traits<XLOPER> {
		typedef WORD xbool;
		typedef CHAR xchar;
		typedef WORD xrw;
		typedef WORD xcol;
		typedef LPSTR xstr;
		typedef LPCSTR xcstr;
		typedef std::basic_string<xchar> xstring;
		typedef xrw xword; // use for array indices
		typedef short int xint;
		typedef _FP xfp;

		static const int strmax = 0xFF; // maximum number of characters in a str
		static xcstr null() { return ""; }
		static int strnicmp(xcstr a, xcstr b, size_t n) { return ::_strnicmp(a, b, n); }
		static xchar strlen(xcstr s) 
		{ 
			size_t n = ::strlen(s);
			ensure (n < strmax);

			return static_cast<xchar>(n); 
		}
		static xstr strncpy(xstr s, xcstr t, size_t n) { return ::strncpy(s, t, n); }
		static int Excelv(int f, LPXLOPER res, int n, LPXLOPER args[])
		{
			return ::Excel4v(f, res, n, args);
		}
		static int Excel(int f, LPXLOPER res, int n, ...) 
		{
			if (n < 0 || n >= 32)
				return xlretInvCount;

			LPXLOPER args[32];
			va_list ap;

			va_start(ap, n);
			for (int i = 0; i < n; ++i)
				args[i] = va_arg(ap, LPXLOPER);
			va_end(ap);

			return Excelv(f, res, n, &args[0]);
		}
	};

	template<>
	struct traits<XLOPER12> {
		typedef BOOL xbool;
		typedef XCHAR xchar;
		typedef RW xrw;
		typedef COL xcol;
		typedef XCHAR* xstr;
		typedef const XCHAR* xcstr;
		typedef std::basic_string<xchar> xstring;
		typedef xrw xword; // use for array indices
		typedef int xint;
		typedef _FP12 xfp;

		static const int strmax = 0x7FFF;
		static xcstr null() { return L""; }
		static int strnicmp(xcstr s, xcstr t, size_t n) { return ::_wcsnicmp(s, t, n); }
		static xchar strlen(xcstr s) 
		{
			size_t n = ::wcslen(s); 
			ensure (n < strmax);

			return static_cast<xchar>(n);
		}
		static xstr strncpy(xstr s, xcstr t, size_t n) { return wcsncpy(s, t, n); }
		static int Excelv(int f, LPXLOPER12 res, int n, LPXLOPER12 args[])
		{
			return ::Excel12v(f, res, n, args);
		}
		static int Excel(int f, LPXLOPER12 res, int n, ...) 
		{
			if (n < 0 || n >= 32)
				return xlretInvCount;

			LPXLOPER12 args[32];
			va_list ap;

			va_start(ap, n);
			for (int i = 0; i < n; ++i)
				args[i] = va_arg(ap, LPXLOPER12);
			va_end(ap);

			return Excelv(f, res, n, &args[0]);
		}
	};
/*
	WideCharToMultiByte();
	MultiByteToWideChar();
*/
	// construct with char* but convertable to wchar_t*
	class string {
		const char* s_;
		int n_;
		string(const string&);
		string& operator=(const string&);
	public:
		string(const char* s, int n = -1)
			: s_(s), n_(n)
		{ }
		~string()
		{ }
		operator traits<XLOPER>::xstring()
		{
			return n_ > 0 ? traits<XLOPER>::xstring(s_, n_) : traits<XLOPER>::xstring(s_);
		}
		operator traits<XLOPER12>::xstring()
		{
			int n(n_);

			if (n <= 0) {
				n = -1;
				n = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, s_, -1, nullptr, 0);
			}

			traits<XLOPER12>::xstring s;
			s.reserve(n);
			ensure (MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, s_, n_, &s[0], n));

			return s;
		}
	};
}

#include "types.h"