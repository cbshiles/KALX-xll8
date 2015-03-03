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
	typedef typename xll::traits<X>::xint xint;
public:
	XOPER()
	{
		alloc(xltype::Nil);
	}
	XOPER(const XOPER& o)
	{
		alloc(o);
	}
	XOPER(XOPER&& o)
	{
		xltype = o.xltype;
		val = o.val;

		o.alloc(xltype::Nil);
	}
	explicit XOPER(const std::initializer_list<XOPER>& o)
	{
		alloc(1, static_cast<xll::traits<X>::xword>(o.size()));
		std::copy(o.begin(), o.end(), stdext::checked_array_iterator<XOPER*>(begin(), size()));
//		std::copy(o.begin(), o.end(), begin());
		ensure (xltype == xltypeMulti);
	}
	XOPER& operator=(const XOPER& o)
	{
		if (this != &o) {
			if (xltype == o.xltype) {
				realloc(o);
			}
			else {
				// can't assign multi to element of itself
				if (o.xltype == xltypeMulti) {
					ensure (this < o.begin() || o.end() <= this);
				}
				free();
				alloc(o);
			}
		}

		return *this;
	}
	XOPER& operator=(XOPER&& o)
	{
		xltype = o.xltype;
		val = o.val;

		o.alloc(xltype::Nil);

		return *this;
	}
	~XOPER()
	{
		free();
	}

	XOPER(const typename xll::traits<X>::type& x)
	{
		alloc(x);
	}
	XOPER& operator=(const typename xll::traits<X>::type& x)
	{
		free();
		alloc(x);

		return *this;
	}

	friend void swap(XOPER& x, XOPER& y)
	{
		using std::swap;

		swap(x.xltype, y.xltype);
		swap(x.val, y.val); //???
	}

	// For thread-safe programs:
	// LPOPERX WINAPI foo(...) { OPERX* px = new(OPERX(...)); ... ; return px->DLLFree(); }
	XOPER* DLLFree()
	{
		xltype |= xlbitDLLFree;

		return this;
	}
	XOPER* XLFree()
	{
		xltype |= xlbitXLFree;

		return this;
	}

	// forward to XLOPER
	xword rows() const
	{
		return xll::rows<X>(*this);
	}
	xword columns() const
	{
		return xll::columns<X>(*this);
	}
	xword size() const
	{
		return xll::size<X>(*this);
	}

	// 1-d index
	XOPER& operator[](xword i)
	{
		return static_cast<XOPER&>(xll::index<X>(*this, i));
	}
	const XOPER& operator[](xword i) const
	{
		return static_cast<const XOPER&>(xll::index<X>(*this, i));
	}
	// 2-d index
	XOPER& operator()(xword r, xword c)
	{
		return static_cast<XOPER&>(::index<X>(*this, r, c));
	}
	const XOPER& operator()(xword r, xword c) const
	{
		return static_cast<const XOPER&>(xll::index<X>(*this, r, c));
	}

	// STL friendly
	XOPER* begin()
	{
		return xltype == xltypeMulti ? static_cast<XOPER*>(val.array.lparray) : this;
	}
	XOPER* end()
	{
		return xltype == xltypeMulti ? static_cast<XOPER*>(val.array.lparray) + size() : this + 1;
	}
	const XOPER* begin() const
	{
		return xltype == xltypeMulti ? static_cast<const XOPER*>(val.array.lparray) : this;
	}
	const XOPER* end() const
	{
		return xltype == xltypeMulti ? static_cast<const XOPER*>(val.array.lparray) + size() : this + 1;
	}
	// STL friendly
	std::reverse_iterator<XOPER*> rbegin()
	{
		return std::reverse_iterator<XOPER*>((xltype == xltypeMulti ? static_cast<XOPER*>(val.array.lparray) : this) + size());
	}
	std::reverse_iterator<XOPER*> rend()
	{
		return std::reverse_iterator<XOPER*>(xltype == xltypeMulti ? static_cast<XOPER*>(val.array.lparray) : this);
	}
	const std::reverse_iterator<const XOPER*> rbegin() const
	{
		return std::reverse_iterator<const XOPER*>((xltype == xltypeMulti ? static_cast<XOPER*>(val.array.lparray) : this) + size());
	}
	const std::reverse_iterator<const XOPER*> rend() const
	{
		return std::reverse_iterator<const XOPER*>(xltype == xltypeMulti ? static_cast<XOPER*>(val.array.lparray) : this);
	}

	// Num
	explicit XOPER(double num)
	{
		xltype = xltypeNum;
		val.num = num;
	}
	operator double() const
	{
		switch (xltype) {
		case xltypeNum:
			return val.num;
			break;
		case xltypeStr:
			return val.str[0]; // non zero length string
			break;
		case xltypeBool:
			return val.xbool;
			break;
		case xltypeMulti:
			for (const auto& v : *this) {
				double x = v.operator double();
				if (x)
					return x;
			}
			return 0;
			break;
		case xltypeInt:
			return val.w;
			break;
		case xltypeNil: case xltypeMissing: case xltypeErr:
			return 0;
		}

		return std::numeric_limits<double>::quiet_NaN();
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
		ensure (xltype == xltypeNum);
		ensure (o.xltype == xltypeNum);

		val.num += to_double<X>(o);

		return *this;
	}

	// Str
	XOPER(xcstr str)
	{
		alloc(str, xll::traits<X>::strlen(str));
	}
	XOPER(xcstr str, xchar str0)
	{
		alloc(str, str0);
	}
	XOPER(const xstring& str)
	{
		alloc(str.c_str(), static_cast<xchar>(str.length()));
	}
	XOPER& operator=(xcstr str)
	{
		if (xltype == xltypeStr) {
			realloc(str, xll::traits<X>::strlen(str));
		}
		else {
			free();
			alloc(str, xll::traits<X>::strlen(str));
		}

		return *this;
	}

	XOPER& append(xcstr str)
	{
		return append(str, xll::traits<X>::strlen(str));
	}
	XOPER& append(xcstr str, xchar n)
	{
		if (xltype == xltypeNil) {
			alloc(str, n);
		}
		else if (xltype == xltypeStr) {
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
	XOPER& append(const xstring& s)
	{
		return append(s.c_str(), static_cast<xchar>(s.size()));
	}
	XOPER& append(const XOPER& o)
	{
		if (o.xltype == xltypeNil)
			return *this;

		ensure (o.xltype == xltypeStr);

		return append(o.val.str + 1, o.val.str[0]);
	}

	bool operator==(xcstr str) const
	{
		return xltype == xltypeStr 
			&& val.str[0] == static_cast<xchar>(xll::traits<X>::strlen(str))
			&& 0 == xll::traits<X>::strnicmp(val.str + 1, str, val.str[0]);	
	}
	bool operator==(const xstring& str) const
	{
		return operator==(str.c_str());
	}
	xstring to_string(void) const
	{
		XOPER<X> o = xll::name<X>(*this);
		ensure (o.xltype == xltypeStr);

		return xstring(o.val.str + 1, o.val.str + 1 + o.val.str[0]);
	}
	xstring string(void) const
	{
		ensure (xltype == xltypeStr);

		return xstring(val.str + 1, val.str + 1 + val.str[0]);
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
		return xltype == xltypeErr && val.err == static_cast<xll::traits<X>::xint>(err);
	}
	bool operator!=(xlerr err) const
	{
		return !operator==(err);
	}

	// Multi
	XOPER(xword r, xword c)
	{
		alloc(r, c);
	}
	XOPER& resize(xword r, xword c)
	{
		realloc(r, c);

		return *this;
	}
	// conforming resize
	XOPER& resize(xword n)
	{
		ensure (rows() == 1 || columns() == 1);

		return rows() == 1 ? resize(1, n) : resize(n, 1);
	}
	XOPER& front()
	{
		return operator[](0);
	}
	XOPER front() const
	{
		return operator[](0);
	}
	XOPER& back()
	{
		ensure (size() != 0);

		return operator[](size() - 1);
	}
	XOPER back() const
	{
		ensure (size() != 0);

		return operator[](size() - 1);
	}
	XOPER& push_back(const XOPER<X>& x)
	{
		// nil is nil
		if (x.xltype == xltypeNil)
			return *this;

		if (xltype == xltypeNil)
			return operator=(x);

		// overlapping ranges
		if (x.begin() < end() && begin() < x.end()) {
			return push_back(XOPER<X>(x));
		}
	
		if (xltype != xltypeMulti) {
			resize(1,1);
		}

		if (x.columns() == 1 || (columns() == x.columns())) { // favor columns
			resize(rows() + x.rows(), x.columns());
			std::copy(x.begin(), x.end(), stdext::checked_array_iterator<XOPER<X>*>(end() - x.size(), x.size()));
//			std::copy(x.begin(), x.end(), end() - x.size());
		}
		else {
			ensure (x.rows() == 1);
			resize(1, columns() + x.columns());
			std::copy(x.begin(), x.end(), stdext::checked_array_iterator<XOPER<X>*>(end() - x.columns(), x.columns()));
//			std::copy(x.begin(), x.end(), end() - x.columns());
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

	// Int
	explicit XOPER(int i)
	{
		xltype = xltypeInt;
		val.w = static_cast<xint>(i);
	}
	explicit XOPER(unsigned int i)
	{
		xltype = xltypeInt;
		val.w = static_cast<xint>(i);
	}
	XOPER& operator=(int i)
	{
		free();

		xltype = xltypeInt;
		val.w = static_cast<xint>(i);

		return *this;
	}
	bool operator==(int i) const
	{
		return xltype == xltypeInt && val.w ==i || (xltype == xltypeNum && val.num == i);
	}
	explicit XOPER(DWORD i)
	{
		xltype = xltypeInt;
		val.w = i;
	}
	XOPER& operator=(DWORD num)
	{
		free();

		xltype = xltypeInt;
		val.w = num;

		return *this;
	}
	XOPER& operator=(unsigned int num)
	{
		free();

		xltype = xltypeInt;
		val.w = num;

		return *this;
	}
	bool operator==(DWORD i) const
	{
		return xltype == xltypeInt && val.w ==i || (xltype == xltypeNum && val.num == i);
	}
	// emplace, emplace_back, emplace_front, emplace_hint!!!

private:
	void free()
	{
		if (xltype == xltypeStr) {
			::free(val.str);
		}
		else if (xltype == xltypeMulti) {
			for (xword i = 0; i < size(); ++i)
				operator[](i).free();
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

	void alloc(const typename xll::traits<X>::type& o)
	{
		xltype = o.xltype;

		if (xltype == xltypeStr) {
			val.str = 0;

			realloc(o.val.str + 1, o.val.str[0]);
		}
		else if (xltype == xltypeMulti) {
			val.array.rows = 0;
			val.array.columns = 0;
			val.array.lparray = 0;

			realloc(xll::rows<X>(o), xll::columns<X>(o));
			ensure (xll::end<X>(o) <= begin() || end() <= xll::begin<X>(o));
			for (xword i = 0; i < size(); ++i)
				operator[](i) = xll::index<X>(o, i);
		}
		else {
			val = o.val;
		}
	}
	void realloc(const typename xll::traits<X>::type& x)
	{
		ensure (xltype == x.xltype);

		if (xltype == xltypeStr) {
			realloc(x.val.str + 1, x.val.str[0]);
		}
		else if (xltype == xltypeMulti) {
			realloc(xll::rows<X>(x), xll::columns<X>(x));
//			std::copy(x.begin(), x.end(), stdext::checked_array_iterator<XOPER<X>*>(begin(), size()));
			std::copy(xll::begin<X>(x), xll::end<X>(x), begin());
		}
		else {
			val = x.val;
		}
	}

	// Str
	void alloc(xcstr str, xchar str0)
	{
		xltype = xltypeStr;
		val.str = 0;

		realloc(str, str0);
	}
	void realloc(xcstr str, xchar str0)
	{
		ensure (xltype == xltypeStr);

		ensure (0 != (val.str = static_cast<xchar*>(::realloc(val.str, sizeof(xchar)*(1 + str0)))));
		if (str0)
			xll::traits<X>::strncpy(val.str + 1, str, str0);
		val.str[0] = str0;
	}

	// Multi
	void alloc(xword r, xword c)
	{
		xltype = xltypeMulti;
		val.array.rows = 0;
		val.array.columns = 0;
		val.array.lparray = 0;

		realloc(r, c);
	}
	void realloc(xword r, xword c)
	{
		if (r*c == 0) {
			free();

			return;
		}

		// index juggling
		if (xltype == xltypeMulti && size() == r*c) {
			val.array.rows = r;
			val.array.columns = c;

			return;
		}

		OPERX this_;
		if (xltype != xltypeMulti)
			this_ = *this;

		xword n = size();

		if (r*c != size()) { // r*c == 0 calls ::free
			val.array.lparray = static_cast<X*>(::realloc(val.array.lparray, r*c*sizeof(X)));
			ensure (val.array.lparray);
		}

		xltype = xltypeMulti;
		val.array.rows = r;
		val.array.columns = c;

		for (xword i = n; i < r*c; ++i) {
			operator[](i).alloc(xltype::Nil);
		}

		operator[](0) = this_;
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
// more specializations??? XLOPER == double
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