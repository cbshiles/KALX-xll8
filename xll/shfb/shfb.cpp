// sfhb.cpp - Sandcastle Help File Builder
// Create files for building help files.
#include <iostream>
#include <fstream>
#ifdef EXCEL12
#undef EXCEL12
#endif

#include "../xll.h"

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

extern "C" int __declspec(dllexport) WINAPI xll_make_doc()
{
	try {
		// get list of all addins
		// generate shfbproj file
		double x;
		x = 1;
		ensure (x == 1);
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());

		return 0;
	}

	return 1;
}