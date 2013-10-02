// oper.cpp - Memory managed XLOPER
// Copyright (c) KALX, LLC. All rights reserved. No warranty made.
// included by loper.h
#include <algorithm>
#include <utility>

using namespace std::rel_ops;

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
	XOPER(XOPER&& o)
	{
		xltype = o.xltype;
		val = o.val;

		o.xltype = xltypeNil;
	}
	XOPER& operator=(const XOPER& o)
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
	XOPER& operator=(XOPER&& o)
	{
		xltype = o.xltype;
		val = o.val;

		o.xltype = xltypeNil;

		return *this;
	}
	~XOPER()
	{
		free();
	}

	XOPER(const X& x)
	{
		alloc(x);
	}
	XOPER& operator=(const X& x)
	{
		if (this != &x) {
			alloc(x);
		}

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
		if (size() == 0) {
			free();
			alloc(str);
		}
		else if (xltype == xltypeStr) {
			if (n == 0)
				n = static_cast<xchar>(_tcslen(str));
			val.str = static_cast<xchar*>(::realloc(val.str, val.str[0] + n + 1));
			ensure (val.str);
			_tcsnccat(val.str + val.str[0] + 1, str, n);
		}
		else {
			throw std::runtime_error("XOPER::append must be str or empty");
		}

		return *this;
	}
	bool operator==(xcstr str) const
	{
		return xltype == xltypeStr 
			&& val.str[0] == static_cast<xchar>(_tcsclen(str))
			&& 0 == _tcsnicmp(val.str + 1, str, val.str[0]);	
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
	void resize(xword r, xword c)
	{
		if (xltype == xltypeMulti)
			realloc(r, c, begin(), (std::min<xword>)(size(), r*c));
		else
			alloc(r, c);
	}
	void push_back(const X& x)
	{
		realloc(::rows(x), ::columns(x), ::begin(x), ::size(x), true); // append
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

		alloc(xltype::Missing); // like xlFree
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

			realloc(::rows(x), ::columns(x), ::begin(x), ::size(x));
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
			realloc(o.rows(), o.columns(), o.begin(), o.size());
		}
		else {
			val = o.val;
		}
	}

	// Str
	void alloc(xcstr str, xchar str0 = 0)
	{
		xltype = xltypeStr;
		val.str = nullptr;

		realloc(str, str0);
	}
	void realloc(xcstr str, xchar str0 = 0)
	{
		ensure (xltype == xltypeStr);

		if (str0 == 0) {
			size_t n = _tcslen(str);
			ensure (n < xll::traits<X>::strmax);
			str0 = static_cast<xchar>(n);
		}

		val.str = static_cast<xchar*>(::realloc(val.str, (1 + str0)*sizeof(xchar)));
		ensure (val.str);
		_tcsncpy(val.str + 1, str, str0);
		val.str[0] = str0;
	}

	// Multi
	void alloc(xword r, xword c, const X* px = 0, xword n = 0)
	{
		xltype = xltypeMulti;
		val.array.rows = 0;
		val.array.columns = 0;
		val.array.lparray = nullptr;

		realloc(r, c, px, n);
	}
	void realloc(xword r, xword c, const X* px = 0, xword n = 0, bool append = false)
	{
		ensure (!append || px);

		// index juggling
		if (!append && xltype == xltypeMulti && size() == r*c) {
			val.array.rows = r;
			val.array.columns = c;

			return;
		}

		// append means replace for nil and missing
		if (append && size() == 0) {
			if (px)
				operator=(*px); // ignore n??

			return;
		}

		if (append && size() == 1 && px && xltype != xltypeMulti) {
			XOPER x(*this); // make temp, could be more efficient
			alloc(1, 1, &x, 1);
		}

		ensure (xltype == xltypeMulti);

		xword offset = 0;
		if (append) {
			offset = size();
			if (columns() == 1) // 1x1 defaults to column vector
				r = rows() + n;
			else if (rows() == 1 && columns() != c) // row vector
				c = columns() + n;
			else
				r = rows() + 1 + (n - 1)/columns(); // pad to full column
		}

		// throw if assigning multi to index of itself
		ensure (!(px <= this && this < px + n));

		if (r*c != size()) { // r*c == 0 calls ::free
			val.array.lparray = static_cast<X*>(::realloc(val.array.lparray, r*c*sizeof(X)));
			ensure (val.array.lparray);
		}

		val.array.rows = r;
		val.array.columns = c;

		ensure (size() >= offset + n);

		xword i = 0;

		if (px) {
			while (i < n) {
				operator[](offset + i).alloc(px[i]);
				++i;
			}
		}
		while (offset + i < size()) {
			operator[](offset + i++) = XOPER();
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