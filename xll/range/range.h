// range.h - universal range functions
//XLL_DLLIMPEXP xll::handle<OPER> handle_oper;
//XLL_DLLIMPEXP xll::handle<OPER12> handle_oper12;
#pragma once
#include <algorithm>
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
	inline XOPER<X> take(const XOPER<X>& o, long n)
	{
		XOPER<X> o_;

		if (n > 0) {
			n = (std::min<long>)(n, o.size());

			if (o.rows() == 1)
				o_.resize(1, n);
			else
				o_.resize(n, 1);

			std::copy(o.begin(), o.begin() + n, o_.begin());
		}
		else if (n < 0) {
			n = (std::min<long>)(-n, o.size());

			if (o.rows() == 1)
				o_.resize(1, n);
			else
				o_.resize(n, 1);

			std::copy(o.end() - n, o.end(), o_.begin());
		}
		else {
			o_ = XOPER<X>();
		}

		return o_;
	}

	// drop elements of a range
	template<class X>
	inline XOPER<X> drop(const XOPER<X>& o, long n)
	{
		XOPER<X> o_;

		if (n > 0) {
			n = (std::min<long>)(n, o.size());

			if (o.rows() == 1)
				o_.resize(1, o.size() - n);
			else
				o_.resize(o.size() - n, 1);

			std::copy(o.begin() + n, o.end(), o_.begin());
		}
		else if (n < 0) {
			n = (std::min<long>)(-n, o.size());

			if (o.rows() == 1)
				o_.resize(1, o.size() - n);
			else
				o_.resize(o.size() - n, 1);

			std::copy(o.begin(), o.end() - n, o_.begin());
		}
		else {
			o_ = o;
		}

		return o_;
	}
	// trim empty elements from front and back of range
	template<class X>
	inline XOPER<X> trim(const XOPER<X>& o, bool contiguous = false)
	{
		XOPER<X> o_;

		ensure (o.size() > 0);

		auto nil = [](const XOPER<X>& x) { return x.xltype == xltypeNil || (x.xltype == xltypeStr && x.val.str[0] == 0); };
		const XOPER<X>* lb = std::find_if_not(o.begin(), o.end(), nil);

		if (lb == o.end())
			return XOPER<X>(xlerr::NA);

		const XOPER<X>* ub = contiguous 
			? std::lower_bound(lb, o.end(), XOPER<X>(), [&nil](const XOPER<X>& x, const XOPER<X>& y) { return !nil(x) && nil(y); })
			: std::find_if_not(o.rbegin(), o.rend(), nil).base();

		ptrdiff_t n = std::distance(lb, ub);
		ensure (n > 0);

		if (o.rows() == 1)
			o_.resize(1, n);
		else
			o_.resize(n, 1);

		std::copy(lb, ub, o_.begin());

		return o_;
	}

} // range