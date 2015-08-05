// sfhb.cpp - Sandcastle Help File Builder
// Create files for building help files.
#include <iostream>
#include <fstream>
#include <map>
#define EXCEL12
#include "../xll.h"

using namespace std;
using namespace xll;

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

using namespace xll;

template<class X>
void xll_make_one(const XAddIn<X>* pai)
{

}

extern "C" int __declspec(dllexport) WINAPI xll_make_doc()
{
	try {
		map<string,map<string,const XAddIn<XLOPER>*>> cat_fun_map;
		// get list of all addins
		for (const auto& i : XAddIn<XLOPER>::List()) {
			cat_fun_map[i->Args().Category().string()][i->Args().FunctionText().string()] = i;
		}

		auto list12 = XAddIn<XLOPER12>::List();
		for (const auto& i : list12)
			xll_make_one<XLOPER12>(i);

		// generate shfbproj file
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());

		return 0;
	}

	return 1;
}