// sfhb.cpp - Sandcastle Help File Builder
// Create files for building help files.
#include <iostream>
#include <fstream>
#include <map>
#define EXCEL12
#include "../xll.h"

using namespace std;
using namespace xll;

// category -> function name -> AddInX pointer
template<class X>
using cat_fun_map = map<typename traits<X>::xstring,map<typename traits<X>::xstring,const XAddIn<X>*>>;

/*
#include "shfbproj.txt"

namespace shfb {

	inline int proj(const char* name)
	{
		std::string projname(name);
		projname.append(".shfbproj");

		std::ifstream ifs("proj.shfbproj");

		return 1;
	}

} // shfb
*/

template<class X>
void xll_make_cfm(const cat_fun_map<X>& cfm)
{

}

extern "C" int __declspec(dllexport) WINAPI xll_make_doc()
{
	try {
		cat_fun_map<XLOPER> cfm;
		// get list of all addins
		for (const auto& i : XAddIn<XLOPER>::List()) {
			cfm[i->Args().Category().string()].insert(
				std::make_pair(i->Args().FunctionText().string(), i));
		}
		xll_make_cfm<XLOPER>(cfm);

		cat_fun_map<XLOPER12> cfm12;
		// get list of all addins
		for (const auto& i : XAddIn<XLOPER12>::List()) {
			cfm12[i->Args().Category().string()].insert(
				std::make_pair(i->Args().FunctionText().string(), i));
		}
		xll_make_cfm<XLOPER12>(cfm12);

		// generate shfbproj file
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());

		return 0;
	}

	return 1;
}