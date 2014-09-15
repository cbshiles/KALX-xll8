// traits.h - parameterize based on XLOPER and XLOPER12 from the Microsoft SDK.
// Copyright (c) KALX, LLC. All rights reserved. No warranty made.
// included by defines.h
// Permit a single xll to use either old or new types.

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

		static xcstr UNCALCED() { return XLL_UNCALCED; }
		static xcstr VOLATILE() { return XLL_VOLATILE; }
		static xcstr COMMA() { return ", "; }

		// The total number of available columns in Excel
		static const BYTE colmax = 0xFF;
		// The total number of available rows in Excel
		static const WORD rwmax = 0xFFFF;
		// The number of characters that can be stored and displayed in a cell formatted as Text
		static const UCHAR strmax = 0xFF;
		//Maximum number of arguments to a function
		static const int argmax = 30;

		static std::string string(xcstr s, xchar n = 0)
		{
			return std::string(s, s + (n ? n : strlen(s)));
		}
		static std::string string(const XLOPER& s)
		{
			ensure (s.xltype == xltypeStr);
			
			return string(s.val.str + 1, s.val.str[0]);
		}
		static int strnicmp(xcstr a, xcstr b, size_t n) 
		{ 
			return ::_strnicmp(a, b, n);
		}
		static xchar strlen(xcstr s) 
		{ 
			size_t n = ::strlen(s);
			ensure (n <= strmax);

			return static_cast<xchar>(n); 
		}
		static xstr strncpy(xstr s, xcstr t, size_t n) 
		{
			return ::strncpy(s, t, n); 
		}
		static int Excelv(int f, LPXLOPER res, int n, LPXLOPER args[])
		{
			return ::Excel4v(f, res, n, args);
		}
		template<typename T>
		static std::string to_string(T t)
		{
			return std::to_string(t);
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

		static xcstr UNCALCED() { return XLL_UNCALCED12; }
		static xcstr VOLATILE() { return XLL_VOLATILE12; }
		static xcstr COMMA() { return L", "; }

		// The total number of available columns.
		static const COL colmax = 0x3FFF;
		// The total number of available rows.
		static const RW rwmax = 0xFFFFF;
		// The number of characters that can be stored in a cell formatted as Text
		static const XCHAR strmax = 0x7FFF;
		//Maximum number of arguments to a function
		static const int argmax = 255;

		static std::string string(xcstr s, int n = 0)
		{
			if (n == 0)
				n = static_cast<int>(wcslen(s));
		
			int n_ = WideCharToMultiByte(CP_ACP, 0, s, n, 0, 0, 0, 0);
			std::string s_(n_, 0);
			WideCharToMultiByte(CP_ACP, 0, s, n, &s_[0], n_, 0, 0);

			return s_;
		}
		static std::string string(const XLOPER12& s)
		{
			ensure (s.xltype == xltypeStr);
			
			return string(s.val.str + 1, s.val.str[0]);
		}
		static int strnicmp(xcstr s, xcstr t, size_t n) 
		{ 
			return ::_wcsnicmp(s, t, n); 
		}
		static xchar strlen(xcstr s) 
		{
			size_t n = ::wcslen(s); 
			ensure (n <= strmax);

			return static_cast<xchar>(n);
		}
		static xstr strncpy(xstr s, xcstr t, size_t n) 
		{ 
			return wcsncpy(s, t, n); 
		}
		template<typename T>
		static std::wstring to_string(T t)
		{
			return std::to_wstring(t);
		}
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
//	template<class X>
//	inline std::string to_string(xll::traits<X>::xcstr s);
	// convert from wchar_t to char strings
//	template<class X>
//	inline std::string as_string(traits<X>::xcstr s, size_t n = 0);
/*
	template<>
	inline std::string to_string<XLOPER>(traits<XLOPER>::xcstr s, size_t n)
	{
		return std::string(s, s + (n ? n : traits<XLOPER>::strlen(s)));
	}
	template<>
	inline std::string to_string<XLOPER12>(traits<XLOPER12>::xcstr s, size_t n)
	{
		if (n == 0)
			n = wcslen(s);
		
		size_t n_ = WideCharToMultiByte(CP_ACP, 0, s, n, 0, 0, 0, 0);
		std::string s_(n_);
		size_t n_ = WideCharToMultiByte(CP_ACP, 0, s, n, &s_[0], n_, 0, 0);

		return s_;
	}
	*/
}

#include "types.h"