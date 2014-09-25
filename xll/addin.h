// register.h - Register functions and macros.
// Copyright (c) KALX, LLC. All rights reserved. No warranty made.
// included by args.h
#include <sstream>
#include <vector>

namespace xll {

	template<class X>
	class XAddIn {
		typedef typename traits<X>::xchar xchar;
		typedef typename traits<X>::xcstr xcstr;
		typedef typename traits<X>::xword xword;
		typedef typename traits<X>::xstring xstring;

		XArgs<X> args_;
		double regid_;

		XAddIn& operator=(const XAddIn&);
	public:
		// macro
		XAddIn(xcstr proc, xcstr text)
			: args_(XArgs<X>(proc, text)), regid_(0)
		{
			List().push_back(this);
		}
		// function
		XAddIn(xcstr proc, xcstr type, xcstr func, xcstr args = 0, xcstr cat = 0, xcstr doc = 0)
			: args_(XArgs<X>(proc, type, func, args, cat, doc)), regid_(0)
		{
			List().push_back(this);
		}
		// general
		XAddIn(const XArgs<X>& args)
			: args_(args), regid_(0)
		{
			List().push_back(this);
		}
		~XAddIn()
		{ }

		const XArgs<X>& Args() const
		{
			return args_;
		}
		double RegisterId() const
		{
			return regid_;
		}

		XOPER<X> Register(void)
		{
			// use constexpr to demangle name and check argumens
			std::vector<X*> pargs(args_.size());

			// ModuleText
			args_[0] = Excel<X>(xlGetName);

			for (xword i = 0; i < args_.size(); ++i)
				pargs[i] = &args_[i];

			// add a space to last arg description
//			if (pargs.size() > static_cast<size_t>(RegisterArg::Max)) {
				XOPER<X>* pa = static_cast<XOPER<X>*>(pargs.back());
				if (pa->xltype == xltypeStr && *(pa->val.str + pa->val.str[0]) != ' ') {
					xchar sp(' ');
					pa->append(&sp, 1);
				}
//			}

			// check for aliases!!!
			LXOPER<X> x;
			int ret = traits<X>::Excelv(xlfRegister, &x, args_.size(), &pargs[0]);
			if (ret != xlretSuccess) {
				std::string err("Failed to register function: ");
				err.append(xll::traits<X>::string(args_.FunctionText()));
				MessageBoxA(xll_GetHwnd(), err.c_str(), "Error", MB_OK);
			}
			else if (!x) {
				std::string err("Failed to register function: ");
				err.append(xll::traits<X>::string(args_.FunctionText()));
				err.append("\nPerhaps you forgot to use #pragma XLLEXPORT or told Excel the wrong name for your function?"); 
				MessageBoxA(xll_GetHwnd(), err.c_str(), "Warning", MB_OK);
			}
			else {
				ensure (x.xltype == xltypeNum);
				regid_ = x.val.num;
			}

			return x;
		}

		static int RegisterAll(void)
		{
			try {
				for (auto ai : List())
					ai->Register();
			}
			catch (const std::exception& ex) {
				MessageBoxA(xll_GetHwnd(), ex.what(), "Error", MB_OK);

				return 0;
			}

			return 1;
		}

		static const XAddIn<X>* Find(const XOPER<X>& regid)
		{
			for (auto ai : List()) {
				if (ai->regid_ == regid || ai->Args().FunctionText() == regid)
					return ai;
			}

			return 0;
		}

/*
		// Return the C++ declaration.
		xstring Declaration(void)
		{
			std::ostringstream oss;

			// oss << args_.ReturnType();
			oss << args_.Procedure() << _T("(");
			// for (auto a : args_.ArgTypes()) oss << a << _T(", ");
			// chop last two chars???
			oss << _T(");");

			return oss.str();
		}
*/
//	private:
		static std::vector<XAddIn<X>*>& List(void)
		{
			static std::vector<XAddIn<X>*> l_;

			return l_;
		}

	};

	typedef XAddIn<XLOPER>   AddIn;
	typedef XAddIn<XLOPER12> AddIn12;
	typedef XAddIn<XLOPERX>  AddInX;

} // namespace xll

// Enumerated type XLL_ENUM(C_VALUE, ExcelName, "Category", "Description")
#define XLL_ENUM(value, name, cat, desc) static xll::AddIn xai_##name(   \
	XLL_DECORATE("xll_" #name, 0), XLL_LPOPER, #name, "", cat, desc); \
    extern "C" __declspec(dllexport) LPOPER WINAPI xll_##name(void)      \
	{ static OPER o(value); return &o; }
#define XLL_ENUM12(value, name, cat, desc) static xll::AddIn12 xai_##name##12(   \
	XLL_DECORATE12(L"xll_" L#name L"12", 0), XLL_LPOPER12, L#name, L"", L#cat, L#desc); \
	extern "C" __declspec(dllexport) LPOPER12 WINAPI xll_##name##12(void)      \
	{ static OPER12 o(value); return &o; }
#ifdef EXCEL12
#define XLL_ENUMX XLL_ENUM12
#else
#define XLL_ENUMX XLL_ENUM
#endif

#define XLL_ENUM_DOC(value, name, cat, desc, doc) static xll::AddIn xai_##name(   \
	FunctionX(XLL_LPOPERX, ENSURE_STRZ_(_xll_##name##@0), #name) \
	.Category(cat).FunctionHelp(desc).Documentation(doc)); \
    extern "C" __declspec(dllexport) LPOPER WINAPI xll_##name(void)      \
	{ static OPER o(value); return &o; }
#define XLL_ENUM_DOC12(value, name, cat, desc, doc) static xll::AddIn12 xai_##name##12(   \
	FunctionX(XLL_LPOPER12, ENSURE_STRZ12_(_xll_##name##12@0), L#name) \
	.Category(L#cat).FunctionHelp(L#desc).Documentation(L#doc)); \
	extern "C" __declspec(dllexport) LPOPER12 WINAPI xll_##name##12(void)      \
	{ static OPER12 o(value); return &o; }
#ifdef EXCEL12
#define XLL_ENUM_DOCX XLL_ENUM_DOC12
#else
#define XLL_ENUM_DOCX XLL_ENUM_DOC
#endif
/*
#define XLL_ENUM_SORT(group, value, name, cat, desc, doc) static xll::AddInX xai_##name(   \
    FunctionX(XLL_LPOPERX, _T(ENSURE_STRZ_(xll_##name)), _T(#name)) \
	.Category(cat).FunctionHelp(desc _T(" ")).Documentation(doc) \
	.Sort(_T(#group), OPERX(value))); \
    extern "C" __declspec(dllexport) LPOPERX WINAPI xll_##name(void)      \
	{ static OPERX o(value); return &o; }
*/

//#include "error.h"

#ifdef _DEBUG
//#include "utility/random.h"
#endif