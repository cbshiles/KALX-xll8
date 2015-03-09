// handle.cpp - create obj file for export
#include "handle.h"
/*
template<class T>
static __declspec(dllexport) std::vector<std::unique_ptr<T>>& xll::handle<T>::handles()
{
	static std::vector<std::unique_ptr<T>> hc_;

	return hc_;
}
*/

extern "C" int xyz_foo(int i)
{
	static int i_;

	if (i != 0)
		i_ = i;

	return i_;
}