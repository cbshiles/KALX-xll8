// range.h - universal range functions
#pragma once
#include <algorithm>
#include "../xll.h"

typedef xll::traits<XLOPERX>::xword xword;

#pragma comment(linker, "/include:" XLL_DECORATE("xll_range_set", 4))
#pragma comment(linker, "/include:" XLL_DECORATE("xll_range_get", 8))
#pragma comment(linker, "/include:" XLL_DECORATE("xll_range_set12", 4))
#pragma comment(linker, "/include:" XLL_DECORATE("xll_range_get12", 8))
#pragma comment(linker, "/include:" XLL_DECORATE("xll_range_mask", 8))
#pragma comment(linker, "/include:" XLL_DECORATE("xll_range_mask12", 8))
#pragma comment(linker, "/include:" XLL_DECORATE("xll_range_take", 8))
#pragma comment(linker, "/include:" XLL_DECORATE("xll_range_take12", 8))
#pragma comment(linker, "/include:" XLL_DECORATE("xll_range_drop", 8))
#pragma comment(linker, "/include:" XLL_DECORATE("xll_range_drop12", 8))
#pragma comment(linker, "/include:" XLL_DECORATE("xll_range_trim", 8))
#pragma comment(linker, "/include:" XLL_DECORATE("xll_range_trim12", 8))
#pragma comment(linker, "/include:" XLL_DECORATE("xll_range_reshape", 12))
#pragma comment(linker, "/include:" XLL_DECORATE("xll_range_reshape12", 12))
#pragma comment(linker, "/include:" XLL_DECORATE("xll_range_slice", 16))
#pragma comment(linker, "/include:" XLL_DECORATE("xll_range_slice12", 16))


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
	template<class X>
	inline xll::handle<XOPER<X>> handle(const XOPER<X>& o)
	{
		return xll::handle<XOPER<X>>(o[0].val.num, false);
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

		if (o_.size() == 0)
			o_ = XOPER<X>(xlerr::Null);
		else if (o.rows() == 1)
			o_.resize(1, o_.size());

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
			o_ = XOPER<X>(xlerr::Null);
		}

		return o_;
	}

	// drop elements of a range
	template<class X>
	inline XOPER<X> drop(const XOPER<X>& o, long n)
	{
		XOPER<X> o_;

		if (n > 0) {
			if (n >= static_cast<long>(o.size())) {
				o_ = XOPER<X>(xlerr::Null);
			}
			else {
				if (o.rows() == 1)
					o_.resize(1, o.size() - n);
				else
					o_.resize(o.size() - n, 1);

				std::copy(o.begin() + n, o.end(), o_.begin());
			}
		}
		else if (n < 0) {
			if (-n >= static_cast<long>(o.size())) {
				o_ = XOPER<X>(xlerr::Null);
			}
			else {
				if (o.rows() == 1)
					o_.resize(1, o.size() + n);
				else
					o_.resize(o.size() + n, 1);

				std::copy(o.begin(), o.end() + n, o_.begin());
			}
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

	// remove xltypes in mask
	template<class X>
	inline XOPER<X> remove(const XOPER<X>& o, int mask)
	{
		XOPER<X> o_;

		for (xword i = 0; i < o.size(); ++i) {
			if (!(o[i].xltype&mask))
				o_.push_back(o[i]);
		}

		if (o_.size() == 0)
			o_ = XOPER<X>(xlerr::Null);
		else if (o.rows() == 1)
			o_.resize(1, o_.size());

		return o_;
	}

	template<class X>
	inline XOPER<X> slice(const XOPER<X>& o, int start, int stride, int size)
	{
		XOPER<X> o_;

		int osize = static_cast<int>(o.size());

		if (start < 0)
			start = osize + start;
		if (start < 0 || start >= osize)
			return XOPER<X>(xlerr::Value);

		if (stride == 0)
			stride = 1;

		if (size < 0 || size > 0 && start + (size - 1)*stride >= osize)
			return XOPER<X>(xlerr::Value);

		for (int i = start; size ? i < start + size*stride : (0 <= i && i < osize); i += stride)
			o_.push_back(o[i]);

		if (o_.size() == 0)
			o_ = XOPER<X>(xlerr::Null);
		else if (o.rows() == 1)
			o_.resize(1, o_.size());

		return o_;
	}

} // range