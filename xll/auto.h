// auto.h - export xlAutoXXX functions
// Copyright (c) KALX, LLC. All rights reserved. No warranty made.
// included by xloper.h
#include <functional>
#include <list>

// Export well known functions implemented by xll lib.
#pragma comment(linker, "/include:" XLL_DECORATE("DllMain", 12))
#pragma comment(linker, "/export:" XLL_DECORATE("XLCallVer", 0))
#pragma comment(linker, "/export:xlAutoOpen" XLL_X32("@0=xlAutoOpen"))
#pragma comment(linker, "/export:xlAutoClose" XLL_X32("@0=xlAutoClose"))
#pragma comment(linker, "/export:xlAutoAdd" XLL_X32("@0=xlAutoAdd"))
#pragma comment(linker, "/export:xlAutoRemove" XLL_X32("@0=xlAutoRemove"))
#pragma comment(linker, "/export:xlAutoFree" XLL_X32("@4=xlAutoFree"))
#pragma comment(linker, "/export:xlAutoFree12" XLL_X32("@4=xlAutoFree12"))
#pragma comment(linker, "/export:xlAutoRegister" XLL_X32("@4=xlAutoRegister"))
#pragma comment(linker, "/export:xlAutoRegister12" XLL_X32("@4=xlAutoRegister12"))
//#pragma comment(linker, "/export:xlAddInManagerInfo@4=xlAddInManagerInfo") // default is add-in name
//#pragma comment(linker, "/export:xlAddInManagerInfo12@4=xlAddInManagerInfo12")
#ifdef _DEBUG
#pragma comment(linker, "/export:?crtDbg@@3UCrtDbg@@A")
#endif


// Use Auto<XXX> xao_foo(xll_foo) to run xll_foo on event XXX
namespace xll {

	class Open {};
	class Open12 {};
	class OpenAfter {};
	class OpenAfter12 {};
	class Close {};
	class Close12 {};
	class Add {};
	class Add12 {};
	class RemoveBefore {};
	class RemoveBefore12 {};
	class Remove {};
	class Remove12 {};
	
#ifndef EXCEL12
	typedef Open         OpenX;
	typedef OpenAfter    OpenAfterX;
	typedef Close        CloseX;
	typedef Add          AddX;
	typedef RemoveBefore RemoveBeforeX;
	typedef Remove       RemoveX;
#else
	typedef Open12         OpenX;
	typedef OpenAfter12    OpenAfterX;
	typedef Close12        CloseX;
	typedef Add12          AddX;
	typedef RemoveBefore12 RemoveBeforeX;
	typedef Remove12       RemoveX;
#endif

	// Register macros to be called in xlAuto functions.
	template<class T>
	class Auto {
	public:
		typedef std::function<int(void)> macro;
		Auto(macro m)
		{
			macros().push_back(m); // run in the order they are constructed
		}
		static int Call(void)
		{
			int result(1);

			macro_list& m(macros());
			for (macro_iter i = m.begin(); i != m.end(); ++i) {
				result = (*i)();
				if (!result)
					return result;
			}

			return result;
		}
	private:
		typedef std::list<macro> macro_list;
		typedef macro_list::iterator macro_iter;
		static macro_list& macros(void)
		{
			static macro_list macros_;

			return macros_;
		}
	};
} // namespace xll

#include "loper.h"