// range.h - universal range functions
//XLL_DLLIMPEXP xll::handle<OPER> handle_oper;
//XLL_DLLIMPEXP xll::handle<OPER12> handle_oper12;
#pragma once
#include <iterator>
#include "xll.h"

typedef xll::traits<XLOPERX>::xword xword;

namespace range {

	template<class X>
	class row_iterator : public std::iterator<std::forward_iterator_tag, X> {
		const X* x_;
		X i_;
	public:
		row_iterator(const X* x)
			: x_(x), i_(x)
		{
			i_.val.array.rows = 1;
		}
		~row_iterator()
		{ }
		const X& operator*() const
		{
			return i_;
		}
		const X& operator++()
		{
			i_.val.array.lparray += i_.val.array.columns;

			return i_;
		}
	};

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

	// select elements of o based on mask
	template<class X>
	inline XOPER<X> mask(const XOPER<X>& o, const XOPER<X>& m)
	{
		ensure (is_vector(o));
		ensure (o.size() == m.size());

		XOPER<X> o_;

		for (xword j = 0; j < size<X>(m); ++j) {
			if (m[j]) {
				o_.push_back(o[j]);
			}
		}

		if (o_.xltype == xltypeNil)
			o_ = XOPER<X>(xlerr::NA);

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