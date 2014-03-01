// range.h - universal range functions
//XLL_DLLIMPEXP xll::handle<OPER> handle_oper;
//XLL_DLLIMPEXP xll::handle<OPER12> handle_oper12;
#pragma once
#include <iterator>
#include "xll.h"

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

} // range