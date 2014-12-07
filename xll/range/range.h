// range.h - universal range functions
//XLL_DLLIMPEXP xll::handle<OPER> handle_oper;
//XLL_DLLIMPEXP xll::handle<OPER12> handle_oper12;
#pragma once
#include <iterator>
#include "../xll.h"

typedef xll::traits<XLOPERX>::xword xword;

namespace range {

	template<class X>
	inline bool is_scalar(const XOPER<X>& o)
	{
		return o.xltype == xltypeMulti && (o.rows() == 1 && o.columns() == 1);
	}
	template<class X>
	inline bool is_vector(const XOPER<X>& o)
	{
		return o.xltype == xltypeMulti && (o.rows() == 1 || o.columns() == 1);
	}

	// get range pointer even if handle
	template<class X>
	inline const XOPER<X>* ptr(const XOPER<X>& x)
	{
		// throws if not a valid handle
		if (is_scalar(x) && x.val.array.lparray[0].xltype == xltypeNum) {
			try {
				return xll::handle<XOPER<X>>(x.val.array.lparray[0].val.num).ptr();
			}
			catch (...) {
				return 0;
			}
		}

		return &x;
	}

	// select elements of o based on mask
	template<class X>
	inline XOPER<X> mask(const XOPER<X>& o, const XOPER<X>& m)
	{
		ensure (is_vector(o));
		ensure (o.size() == m.size());

		XOPER<X> o_;

		for (xword i = 0; i < size<X>(m); ++i) {
			if (m[i]) {
				o_.push_back(o[i]);
			}
		}

		if (o_.size()) {
			if (o.rows() == 1)
				o_.resize(1, o_.size());
			else
				o_.resize(o_.size(), 1);
		}
		else {
			o_ = XOPER<X>(xlerr::NA);
		}

		return o_;
	}

#pragma warning(disable: 4244 4996)

	// take elements of a range
	template<class X>
	inline void take(XOPER<X>& o, long n)
	{
		if (o.rows() > 1) {
			if (n > 0) {
				xword r = (std::min<xword>)(o.rows(), static_cast<xword>(n));
				o.resize(r, o.columns());
			}
			else if (n < 0) {
				xword r = (std::min<xword>)(o.rows(), static_cast<xword>(-n));
				std::copy(o.end() - r*o.columns(), o.end(), o.begin());
				o.resize(r, o.columns());
			}
		}
		else {
			if (n > 0) {
				xword c = (std::min<xword>)(o.size(), static_cast<xword>(n));
				std::copy(o.end() - c, o.end(), o.begin());
				o.resize(1, c);
			}
			else if (n < 0) {
				xword c = (std::min<xword>)(o.size(), static_cast<xword>(-n));
				o.resize(1, c);
			}
		}
	}

} // range