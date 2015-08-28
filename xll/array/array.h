// array.h - FP arrays of doubles
#pragma once
#include "../xll.h"

typedef xll::traits<XLOPER>::xfp xfp;
typedef xll::traits<XLOPER12>::xfp xfp12;
typedef xll::traits<XLOPERX>::xfp xfpx;

#pragma comment(linker, "/include:" XLL_DECORATE("xll_array_mask", 8))
#pragma comment(linker, "/include:" XLL_DECORATE("xll_array_mask12", 8))
#pragma comment(linker, "/include:" XLL_DECORATE("xll_array_get12", 8))
#pragma comment(linker, "/include:" XLL_DECORATE("xll_array_sequence12", 24))
#pragma comment(linker, "/include:" XLL_DECORATE("xll_array_set12", 4))

namespace array {

	template<class X>
	using xfp = typename xll::traits<X>::xfp;

	template<class X>
	inline xll::XFP<X> mask(const xfp<X>& a, const xfp<X>& m)
	{
		xll::XFP<X> a_;

		if (columns(a) > 1 && rows(a) > 1) {
			ensure (rows(a) == size(m));
			for (xword i = 0; i < rows(a); ++i)
				if (m.array[i])
					a_.push_back(a.array + i*columns(a), a.array + (i+1)*columns(a));
		}
		else {
			ensure (size(a) == size(m));
			for (xword i = 0; i < rows(a); ++i)
				if (m.array[i])
					a_.push_back(a.array[i]);

			if (rows(a) == 1)
				a_.reshape(1, a_.size());
		}

		return a_;
	}

} // array
