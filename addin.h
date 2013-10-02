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

		XAddIn(const XAddIn&);
		XAddIn& operator=(const XAddIn&);
	public:
		XAddIn(XArgs<X>&& args)
			: args_(args)
		{ }
		// macro
		XAddIn(xcstr proc, xcstr text)
			: args_(XArgs<X>(proc, text))
		{
			List().push_back(this);
		}
		// function
		XAddIn(xcstr proc, xcstr type, xcstr func, xcstr args = nullptr, xcstr cat = nullptr)
			: args_(XArgs<X>(proc, type, func, args, cat))
		{
			List().push_back(this);
		}
		// general
		XAddIn(const XArgs<X>& args)
			: args_(args)
		{
			List().push_back(this);
		}
		~XAddIn()
		{ }

		XOPER<X> Register(void)
		{
			std::vector<X*> pargs(args_.size());

			// ModuleText
			args_[0] = Excel<X>(xlGetName);

			for (xword i = 0; i < pargs.size(); ++i)
				pargs[i] = &args_[i];

			// add a space to last arg description
			if (pargs.size() >= static_cast<size_t>(RegisterArg::Max)) {
				XOPER<X>* pa = static_cast<XOPER<X>*>(pargs.back());
				if (pa->xltype == xltypeStr && *(pa->val.str + pa->val.str[0]) != ' ') {
					xchar sp(' ');
					pa->append(&sp, 1);
				}
			}

			// check for aliases!!!
			LXOPER<X> x;
			int ret = traits<X>::Excelv(xlfRegister, &x, args_.size(), &pargs[0]);
			if (ret != xlretSuccess) {
				MessageBox(GetHwnd(), _T("Failed to register function."), _T("Error"), MB_OK);
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
				MessageBoxA(GetHwnd(), ex.what(), "Error", MB_OK);

				return 0;
			}

			return 1;
		}

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

	private:
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

#ifdef _DEBUG
//#include "utility/random.h"
#endif