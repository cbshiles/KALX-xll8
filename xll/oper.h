// oper.cpp - Memory managed XLOPER
// Copyright (c) KALX, LLC. All rights reserved. No warranty made.
// included by loper.h
#include <algorithm>
#include <utility>

using namespace std::rel_ops; //??? bad idea???

// in global namespace, just like XLOPER
template<class X>
class XOPER : public X {
	typedef typename xll::traits<X>::xchar xchar;
	typedef typename xll::traits<X>::xcstr xcstr;
	typedef typename xll::traits<X>::xstring xstring;
	typedef typename xll::traits<X>::xword xword;
public:
	XOPER()
	{
		alloc(xltype::Nil);
	}
	XOPER(const XOPER& o)
	{
		alloc(xltype::Nil);
		alloc(o);
	}
/*	XOPER(XOPER&& o)
	{
		xltype = o.xltype;
		val = o.val;

		o.xltype = xltypeNil;
	}
*/	XOPER& operator=(const XOPER& o)
	{
		if (this != &o) {
			if (xltype == o.xltype) {
				realloc(o);
			}
			else {
				alloc(o);
			}
		}

		return *this;
	}
/*	XOPER& operator=(XOPER&& o)
	{
		xltype = o.xltype;
		val = o.val;

		o.xltype = xltypeNil;

		return *this;
	}
*/	~XOPER()
	{
		free();
	}

	XOPER(const X& x)
	{
		alloc(x);
	}
	XOPER& operator=(const X& x)
	{
		alloc(x);

		return *this;
	}

	// For thread-safe programs:
	// LPOPERX WINAPI foo(...) { OPERX* px = new(OPERX(...)); ... ; return px->DLLFree(); }
	XOPER* DLLFree()
	{
		xltype |= xlbitDLLFree;

		return this;
	}

	// forward to XLOPER
	xword rows() const
	{
		return ::rows<X>(*this);
	}
	xword columns() const
	{
		return ::columns<X>(*this);
	}
	xword size() const
	{
		return ::size<X>(*this);
	}

	// 1-d index
	XOPER& operator[](xword i)
	{
		return static_cast<XOPER&>(::index<X>(*this, i));
	}
	const XOPER& operator[](xword i) const
	{
		return static_cast<const XOPER&>(::index<X>(*this, i));
	}
	// 2-d index
	XOPER& operator()(xword r, xword c)
	{
		return static_cast<XOPER&>(::index<X>(*this, r, c));
	}
	const XOPER& operator()(xword r, xword c) const
	{
		return static_cast<const XOPER&>(::index<X>(*this, r, c));
	}

	// STL friendly
	XOPER* begin()
	{
		return static_cast<XOPER*>(::begin<X>(*this));
	}
	XOPER* end()
	{
		return static_cast<XOPER*>(::end<X>(*this));
	}
	const XOPER* begin() const
	{
		return static_cast<const XOPER*>(::begin<X>(*this));
	}
	const XOPER* end() const
	{
		return static_cast<const XOPER*>(::end<X>(*this));
	}

	// Num
	explicit XOPER(double num)
	{
		xltype = xltypeNum;
		val.num = num;
	}
	operator double() const
	{
		return to_double(*this);
	}
	XOPER& operator=(double num)
	{
		free();

		xltype = xltypeNum;
		val.num = num;

		return *this;
	}
	bool operator==(double num) const
	{
		return xltype == xltypeNum && val.num == num;
	}
	XOPER& operator+=(const XOPER& o)
	{
		if (xltype != xltypeNum)
			*this = to_double(*this);

		val.num += to_double(o);

		return *this;
	}
	explicit XOPER(int i)
	{
		xltype = xltypeNum;
		val.num = i;
	}
	XOPER& operator=(int i)
	{
		free();

		xltype = xltypeNum;
		val.num = i;

		return *this;
	}
	bool operator==(int i) const
	{
		return xltype == xltypeNum && val.num == i;
	}

	// Str
	XOPER(xcstr str, xchar str0 = 0)
	{
		alloc(xltype::Nil);
		alloc(str, str0);
	}
	XOPER(const xstring& s)
	{
		alloc(xltype::Nil);
		alloc(s.c_str(), static_cast<xchar>(s.length()));
	}
	XOPER& operator=(xcstr str)
	{
		if (xltype == xltypeStr) {
			realloc(str);
		}
		else {
			free();
			alloc(str);
		}

		return *this;
	}
	XOPER& append(xcstr str, xchar n = 0)
	{
		if (xltype == xltypeMissing || xltype == xltypeNil) {
			alloc(str);
		}
		else if (xltype == xltypeStr) {
			if (n == 0)
				n = static_cast<xchar>(xll::traits<X>::strlen(str));
			val.str = static_cast<xchar*>(::realloc(val.str, sizeof(xchar)*(val.str[0] + n + 1)));
			ensure (val.str);
			xll::traits<X>::strncpy(val.str + val.str[0] + 1, str, n);
			val.str[0] += static_cast<xchar>(n);
		}
		else {
			throw std::runtime_error("XOPER::append must be str or empty");
		}

		return *this;
	}
	bool operator==(xcstr str) const
	{
		return xltype == xltypeStr 
			&& val.str[0] == static_cast<xchar>(xll::traits<X>::strlen(str))
			&& 0 == xll::traits<X>::strnicmp(val.str + 1, str, val.str[0]);	
	}
	static xstring to_string(const X& x)
	{
		static xchar name[] = { 6, '_', 'n', 'A', 'm', 'E', '_' };
		static X Name = { 0, xltypeStr};

		Name.val.str = name;

		// Get/Set name converts "str" to "=\"str\""
		// lock to make thread safe???
		Excel<X>(xlfSetName, Name, x);
		XOPER<X> o = Excel<X>(xlfGetName, Name);
		Excel<X>(xlfSetName, Name);

		ensure (o.xltype == xltypeStr);

		return xstring(o.val.str + 1, o.val.str[0]);
	}
	xstring to_string(void) const
	{
		return to_string(*this);
	}

	// Bool
	explicit XOPER(bool b)
	{
		xltype = xltypeBool;
		val.xbool = b;
	}
	
	XOPER operator=(bool b)
	{
		free();

		xltype = xltypeBool;
		val.xbool = b;

		return *this;
	}
	
	bool operator==(bool b) const
	{
		return val.xbool == static_cast<xll::traits<X>::xbool>(b);
	}
	bool operator!=(bool b) const
	{
		return val.xbool != static_cast<xll::traits<X>::xbool>(b);
	}

	// Ref ???

	// Err
	explicit XOPER(xlerr err)
	{
		alloc(xltype::Err);
		val.err = static_cast<xword>(err);
	}
	bool operator==(xlerr err) const
	{
		return xltype == xltypeErr && val.err == static_cast<xword>(err);
	}
	bool operator!=(xlerr err) const
	{
		return !operator==(err);
	}

	// Multi
	XOPER(xword r, xword c)
	{
		alloc(xltype::Nil);
		alloc(r, c);
	}
	XOPER& resize(xword r, xword c)
	{
		if (xltype == xltypeMulti) {
			realloc(r, c);
		}
		else {
			XOPER tmp(*this);
			alloc(r, c);
			if (r*c != 0)
				operator[](0) = tmp;
		}

		return *this;
	}
	XOPER& push_back(const X& x)
	{
		xword r = (std::max<xword>)(1, ::rows(x));
		xword c = (std::max<xword>)(1, ::columns(x));

		if (xltype == xltypeNil) {
			operator=(x);

			return resize(r, c);
		}

		if (xltype != xltypeMulti)
			resize(1,1);

		if (c == 1) { // favor columns
			resize(rows() + r, 1);
			std::copy(::begin(x), ::end(x), end() - r);
		}
		else if (r == 1) {
			resize(1, columns() + c);
			std::copy(::begin(x), ::end(x), end() - c);
		}
		else {
			ensure (columns() == c);
			resize(rows() + r, c);
			std::copy(::begin(x), ::end(x), end() - r*c);
		}

		return *this;
	}


	// Missing, Nil
	XOPER(::xltype type)
	{
		ensure (type == xltype::Missing || type == xltype::Nil);

		alloc(type);
	}

	// sref

	// Int - converts to double, like Excel

	// emplace, emplace_back, emplace_front, emplace_hint!!!

private:
	void free()
	{
		if (xltype == xltypeStr) {
			::free(val.str);
		}
		else if (xltype == xltypeMulti) {
			std::for_each(begin(), end(), [](XOPER& x) { x.free(); });
			::free(val.array.lparray);
		}

		alloc(xltype::Nil); // xltype::Missing?
	}

	// call free before this if necessary
	void alloc(::xltype type)
	{
		ZeroMemory(this, sizeof(*this));
		xltype = static_cast<WORD>(type);
	}


	void alloc(const X& x)
	{
		free();

		xltype = x.xltype;

		if (xltype == xltypeStr) {
			val.str = nullptr;

			realloc(x.val.str + 1, x.val.str[0]);
		}
		else if (xltype == xltypeMulti) {
			val.array.rows = 0;
			val.array.columns = 0;
			val.array.lparray = nullptr;

			realloc(::rows(x), ::columns(x));
			std::copy(::begin(x), ::end(x), begin());
		}
		else {
			val = x.val;
		}
	}
	void realloc(const XOPER& o)
	{
		ensure (xltype == o.xltype);

		if (xltype == xltypeStr) {
			realloc(o.val.str + 1, o.val.str[0]);
		}
		else if (xltype == xltypeMulti) {
			realloc(o.rows(), o.columns());
			std::copy(o.begin(), o.end(), begin());
		}
		else {
			val = o.val;
		}
	}

	// Str
	void alloc(xcstr str, xchar str0 = 0)
	{
		free();

		xltype = xltypeStr;
		val.str = nullptr;

		realloc(str, str0);
	}
	void realloc(xcstr str, xchar str0 = 0)
	{
		ensure (xltype == xltypeStr);

		if (str0 == 0) {
			size_t n = xll::traits<X>::strlen(str);
			ensure (n < xll::traits<X>::strmax);
			str0 = static_cast<xchar>(n);
		}

		val.str = static_cast<xchar*>(::realloc(val.str, sizeof(xchar)*(1 + str0)));
		ensure (val.str);
		xll::traits<X>::strncpy(val.str + 1, str, str0);
		val.str[0] = str0;
	}

	// Multi
	void alloc(xword r, xword c)
	{
		free();

		xltype = xltypeMulti;
		val.array.rows = 0;
		val.array.columns = 0;
		val.array.lparray = nullptr;

		realloc(r, c);
	}
	void realloc(xword r, xword c)
	{
		// index juggling
		if (xltype == xltypeMulti && size() == r*c) {
			val.array.rows = r;
			val.array.columns = c;

			return;
		}

		ensure (xltype == xltypeMulti);
		xword n = size();

		if (r*c != size()) { // r*c == 0 calls ::free
			val.array.lparray = static_cast<X*>(::realloc(val.array.lparray, r*c*sizeof(X)));
			ensure (r*c == 0 || val.array.lparray);
		}

		val.array.rows = r;
		val.array.columns = c;

		for (xword i = n; i < r*c; ++i) {
			operator[](i) = XOPER<X>();
		}
	}
};

typedef XOPER<XLOPER>   OPER;
typedef XOPER<XLOPER12> OPER12;
typedef XOPER<XLOPERX>  OPERX;

typedef XOPER<XLOPER>*   LPOPER;
typedef XOPER<XLOPER12>* LPOPER12;
typedef XOPER<XLOPERX>*  LPOPERX;

inline bool operator==(const OPER12& x, const OPER12& y)
{
	return xll::operator_equal<XLOPER12>(x, y);
}
inline bool operator<(const OPER12& x, const OPER12& y)
{
	return xll::operator_less<XLOPER12>(x, y);
}
inline bool operator==(const OPER& x, const OPER& y)
{
	return xll::operator_equal<XLOPER>(x, y);
}
inline bool operator<(const OPER& x, const OPER& y)
{
	return xll::operator_less<XLOPER>(x, y);
}

inline bool operator==(const XLOPER12& x, const OPER12& y)
{
	return xll::operator_equal<XLOPER12>(x, y);
}
inline bool operator<(const XLOPER12& x, const OPER12& y)
{
	return xll::operator_less<XLOPER12>(x, y);
}
inline bool operator==(const XLOPER& x, const OPER& y)
{
	return xll::operator_equal<XLOPER>(x, y);
}
inline bool operator<(const XLOPER& x, const OPER& y)
{
	return xll::operator_less<XLOPER>(x, y);
}

inline bool operator==(const OPER12& x, const XLOPER12& y)
{
	return xll::operator_equal<XLOPER12>(x, y);
}
inline bool operator<(const OPER12& x, const XLOPER12& y)
{
	return xll::operator_less<XLOPER12>(x, y);
}
inline bool operator==(const OPER& x, const XLOPER& y)
{
	return xll::operator_equal<XLOPER>(x, y);
}
inline bool operator<(const OPER& x, const XLOPER& y)
{
	return xll::operator_less<XLOPER>(x, y);
}

#include "args.h"