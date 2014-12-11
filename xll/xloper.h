// xloper.h - global functions involving XLOPER's in the Microsoft SDK
// Copyright (c) KALX, LLC. All rights reserved. No warranty made.
// included by traits.h

// scoped and strongly typed enums
enum class xltype {
	Num = xltypeNum,        // IEEE 64-bit floating point
	Str = xltypeStr,        // Counted string
	Bool = xltypeBool,      // Boolean value
	Ref = xltypeRef,        // Reference to multiple ranges
	Err = xltypeErr,        // Excel error type
	Flow = xltypeFlow,      // not used
	Multi = xltypeMulti,    // A range of OPERs
	Missing = xltypeMissing,// Argument not specified
	Nil = xltypeNil,        // Empty data type
	SRef = xltypeSRef,      // Single reference to a range
	Int = xltypeInt,        // Integer type that never occurs
	BigData = xltypeBigData,// Binary data
};

enum class xlbit {
	XLFree = xlbitXLFree,   // Excel calls xlFree
	DLLFree = xlbitDLLFree, // Excel calls xlAutoFree
};

enum class xlerr {
	Null = xlerrNull,  // Intersection of ranges is empty
	Div0 = xlerrDiv0,  // Division by 0
	Value = xlerrValue,// Problem with add-in argument
	Ref = xlerrRef,    // Invalid reference
	Name = xlerrName,  // Defined name not found
	Num = xlerrNum,    // Invalid numeric type
	NA = xlerrNA,      // General not available error
	GettingData = xlerrGettingData,
};

enum class xlret {
	Success = xlretSuccess,
	Abort = xlretAbort,         // macro halted
	InvXlfn = xlretInvXlfn,     // invalid function number
	InvCount = xlretInvCount,   // invalid number of arguments
	InvXloper = xlretInvXloper, // invalid OPER structure
	StackOvfl = xlretStackOvfl, // stack overflow
	Failed = xlretFailed,       // command failed
	Uncalced = xlretUncalced,   // uncalced cell
	NotThreadSafe = xlretNotThreadSafe,// not allowed during multi-threaded calc
	InvAsynchronousContext = xlretInvAsynchronousContext,// invalid asynchronous function handle"
	NotClusterSafe = xlretNotClusterSafe,// not supported on cluster
};

namespace xll {
	template<class X>
	inline double to_double(const typename xll::traits<X>::type& x)
	{
		switch (x.xltype) {
		case xltypeNum:
			return x.val.num;
		case xltypeStr:
			return x.val.str[0]; // length???
		case xltypeBool:
			return x.val.xbool;
		case xltypeInt:
			return x.val.w;
		case xltypeErr: // so ensure (x) fails in this case
		case xltypeMissing:
		case xltypeNil:
			return 0;
		}

		return std::numeric_limits<double>::quiet_NaN();
	}
	/*
	template<class X>
	inline typename xll::traits<X>::xstring to_string(const typename xll::traits<X>::type& x)
	{
		typedef xll::traits<X>::xstring xstring;
		typedef xll::traits<X>::xword xword;

		switch (x.xltype) {
		case xltypeNum:
			return xll::traits<X>::to_string(x.val.num);
		case xltypeStr:
			return xstring(x.val.str + 1, x.val.str[0]);
		case xltypeBool:
			return xll::traits<X>::to_string(x.val.xbool ? true : false);
		case xltypeMulti: 
			{
				xstring s;

				for (xword i = 0; i < size(x); ++i)
					s.append(to_string(index(x, i)));

				return s;
			}
		}

		return xstring('?', 1);
	}
	*/
	template<class X>
	inline typename xll::traits<X>::xword rows(const typename xll::traits<X>::type& x)
	{
		switch (x.xltype) {
		case xltypeMulti:
			return x.val.array.rows;
			break;
		case xltypeSRef:
			return x.val.sref.ref.rwLast - x.val.sref.ref.rwFirst + 1;
			break;
		case xltypeNil:
			return 0;
			break;
		default:
			return 1;
		}
	}
	template<class X>
	inline typename xll::traits<X>::xword columns(const typename xll::traits<X>::type& x)
	{
		switch (x.xltype) {
		case xltypeMulti:
			return x.val.array.columns;
			break;
		case xltypeSRef:
			return x.val.sref.ref.colLast - x.val.sref.ref.colFirst + 1;
			break;
		case xltypeNil:
			return 0;
			break;
		default:
			return 1;
		}
	}
	template<class X>
	inline typename xll::traits<X>::xword size(const typename xll::traits<X>::type& x)
	{
		return rows<X>(x) * columns<X>(x);
	}

	// 1-d index
	template<class X>
	inline typename xll::traits<X>::type& index(typename xll::traits<X>::type& x, typename xll::traits<X>::xword r)
	{
		if (x.xltype == xltypeMulti) {
			ensure (r < size<X>(x));
			return x.val.array.lparray[r];
		}

		ensure (r == 0);

		return x;
	}
	template<class X>
	inline const typename xll::traits<X>::type& index(const typename xll::traits<X>::type& x, typename xll::traits<X>::xword r)
	{
		if (x.xltype == xltypeMulti) {
			ensure (r < size<X>(x));
			return x.val.array.lparray[r];
		}

		ensure (r == 0);

		return x;
	}
	// 2-d index
	template<class X>
	inline typename xll::traits<X>::type& index(typename xll::traits<X>::type& x, typename xll::traits<X>::xword r, typename xll::traits<X>::xword c)
	{
		return ::index<X>(x, r*x.val.array.columns + c);
	}
	template<class X>
	inline const typename xll::traits<X>::type& index(const typename xll::traits<X>::type& x, typename xll::traits<X>::xword r, typename xll::traits<X>::xword c)
	{
		return ::index<X>(x, r*x.val.array.columns + c);
	}

	template<class X>
	inline bool operator_equal(const typename xll::traits<X>::type& x, typename traits<X>::xcstr s, size_t n)
	{
		typedef xll::traits<X>::xchar xchar;

		return x.xltype == xltypeStr && x.val.str[0] == static_cast<xchar>(n) && xll::traits<X>::strnicmp(x.val.str + 1, s, n) == 0;
	}
	template<class X>
	inline bool operator_less(const typename xll::traits<X>::type& x, typename traits<X>::xcstr s, size_t n)
	{
		typedef xll::traits<X>::xchar xchar;

		return std::lexicographical_compare(x.val.str + 1, x.val.str + 1 + x.val.str[0], s, s + n,
			[](xchar a, xchar b) { return ::tolower(a) < ::towlower(b); });
	}
	template<class X>
	inline bool operator_equal(const typename xll::traits<X>::type& x, const typename xll::traits<X>::type& y)
	{
		if (&x == &y)
			return true;

		if (x.xltype != y.xltype)
			return false;

		switch (x.xltype) {
		case xltypeNum:
			return x.val.num == y.val.num;
		case xltypeStr:
			return operator_equal<X>(x, y.val.str + 1, y.val.str[0]);
		case xltypeBool:
			return x.val.xbool == y.val.xbool;
		//case xltypeRef:
		case xltypeErr:
			return x.val.err == y.val.err;
		case xltypeMulti:
			return x.val.array.rows == y.val.array.rows && x.val.array.columns == y.val.array.columns
				&& std::equal(begin<X>(x), end<X>(x), begin<X>(y));
		//case xltypeSRef:
		case xltypeInt:
			return x.val.w == y.val.w;
		default:
			return true; // Nil and Missing
		}
	}

	template<class X>
	inline bool operator_less(const typename xll::traits<X>::type& x, const typename xll::traits<X>::type& y)
	{
		if (&x == &y)
			return false;

		if (x.xltype < y.xltype)
			return true;

		if (x.xltype > y.xltype)
			return false;

		switch (x.xltype) {
		case xltypeNum:
			return x.val.num < y.val.num;
		case xltypeStr:
			return operator_less<X>(x, y.val.str + 1, y.val.str[0]);
		case xltypeBool:
			return x.val.xbool < y.val.xbool;
		//case xltypeRef:
		case xltypeErr:
			return x.val.err < y.val.err;
		case xltypeMulti:
			return std::lexicographical_compare(xll::begin<X>(x), xll::end<X>(x), xll::begin<X>(y), xll::end<X>(y)); 
		//case xltypeSRef:
		case xltypeInt:
			return x.val.w < y.val.w;
		default:
			return false; // Nil and Missing
		}
	}

	template<class X>
	typename traits<X>::type* begin(typename traits<X>::type& x)
	{
		return x.xltype == xltypeMulti ? x.val.array.lparray : &x;
	}
	template<class X>
	typename traits<X>::type* end(typename traits<X>::type& x)
	{
		return x.xltype == xltypeMulti ? x.val.array.lparray + size<X>(x) : &x;
	}
	template<class X>
	const typename traits<X>::type* begin(const typename traits<X>::type& x)
	{
		return x.xltype == xltypeMulti ? x.val.array.lparray : &x;
	}
	template<class X>
	const typename traits<X>::type* end(const typename traits<X>::type& x)
	{
		return x.xltype == xltypeMulti ? x.val.array.lparray + size<X>(x) : &x;
	}

} // namespace xll

template<class X>
inline bool operator!(const typename xll::traits<X>::type& x)
{
	return xll::to_double<X>(x) != 0;
}

inline bool operator==(const XLOPER12& x, const XLOPER12& y)
{
	return xll::operator_equal<XLOPER12>(x, y);
}
inline bool operator!=(const XLOPER12& x, const XLOPER12& y)
{
	return !xll::operator_equal<XLOPER12>(x, y);
}
inline bool operator<(const XLOPER12& x, const XLOPER12& y)
{
	return xll::operator_less<XLOPER12>(x, y);
}
inline bool operator>=(const XLOPER12& x, const XLOPER12& y)
{
	return !xll::operator_less<XLOPER12>(x, y);
}
inline bool operator<=(const XLOPER12& x, const XLOPER12& y)
{
	return xll::operator_less<XLOPER12>(x, y) || xll::operator_equal<XLOPER12>(x, y);
}
inline bool operator>(const XLOPER12& x, const XLOPER12& y)
{
	return !(x <= y);
}

inline bool operator==(const XLOPER& x, const XLOPER& y)
{
	return xll::operator_equal<XLOPER>(x, y);
}
inline bool operator!=(const XLOPER& x, const XLOPER& y)
{
	return !xll::operator_equal<XLOPER>(x, y);
}
inline bool operator<(const XLOPER& x, const XLOPER& y)
{
	return xll::operator_less<XLOPER>(x, y);
}
inline bool operator>=(const XLOPER& x, const XLOPER& y)
{
	return !xll::operator_less<XLOPER>(x, y);
}
inline bool operator<=(const XLOPER& x, const XLOPER& y)
{
	return xll::operator_less<XLOPER>(x, y) || xll::operator_equal<XLOPER>(x, y);
}
inline bool operator>(const XLOPER& x, const XLOPER& y)
{
	return !operator<=(x, y);
}

#include "auto.h"