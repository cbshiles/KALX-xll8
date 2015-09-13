// sfhb.cpp - Sandcastle Help File Builder
// Create files for building help files.
#include <iostream>
#include <fstream>
#include <map>
#define EXCEL12
#include "../xll.h"

// phony GUID
using namespace std;
using namespace xll;

inline LPCTSTR 
TUID(unsigned int tid)
{
	static TCHAR guid[64];

	_stprintf_s(guid, sizeof(guid)/sizeof(TCHAR), _T("%04x%04x-0000-0000-0000-000000000000"), tid>>16, tid&0xFFFF);

	return guid;
}


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
cat_fun_map<X> xll_make_cfm()
{
	cat_fun_map<X> cfm;

	for (const auto& i : XAddIn<X>::List()) {
		cfm[i->Args().Category().string()].insert(
			std::make_pair(i->Args().FunctionText().string(), i));
	}

	return cfm;
}

template<class X>
void xll_write_cfm(const cat_fun_map<X>& cfm)
{

}

extern "C" int __declspec(dllexport) WINAPI xll_make_doc()
{
	try {
		cat_fun_map<XLOPER> cfm = xll_make_cfm<XLOPER>();
		cat_fun_map<XLOPER12> cfm12 = xll_make_cfm<XLOPER12>();

		// generate shfbproj file
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());

		return 0;
	}

	return 1;
}