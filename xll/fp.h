// fp.h - Two dimensional array of doubles.
// Copyright (c) 2011 KALX, LLC. All rights reserved. No warranty is made.
#pragma once
#include <algorithm>
#include <cstdlib>
#include <initializer_list>
#include <new>

namespace xll {

	// 1x1 NaN indicates empty array
	inline bool 
	is_empty(const _FP& fp)
	{
		return fp.rows == 1
			&& fp.columns == 1
			&& _isnan(fp.array[0]);
	}
	inline bool 
	is_empty(const _FP12& fp)
	{
		return fp.rows == 1
			&& fp.columns == 1
			&& _isnan(fp.array[0]);
	}

	inline traits<XLOPER>::xword
	rows(const _FP& fp)
	{
		return is_empty(fp) ? 0 : fp.rows;
	}
	inline traits<XLOPER12>::xword
	rows(const _FP12& fp)
	{
		return is_empty(fp) ? 0 : fp.rows;
	}

	inline traits<XLOPER>::xword
	columns(const _FP& fp)
	{
		return is_empty(fp) ? 0 : fp.columns;
	}
	inline traits<XLOPER12>::xword
	columns(const _FP12& fp)
	{
		return is_empty(fp) ? 0 : fp.columns;
	}



	inline traits<XLOPER>::xword
	size(const _FP& fp)
	{
		return is_empty(fp) ? 0 : fp.rows * fp.columns;
	}
	inline traits<XLOPER12>::xword
	size(const _FP12& fp)
	{
		return is_empty(fp) ? 0 : fp.rows * fp.columns;
	}

	inline double
	index(const _FP& fp, traits<XLOPER>::xword i)
	{
		ensure (i < size(fp));

		return fp.array[i];
	}
	inline double
	index(const _FP12& fp, traits<XLOPER12>::xword i)
	{
		ensure (i < size(fp));

		return fp.array[i];
	}

	inline double&
	index(_FP& fp, traits<XLOPER>::xword i, traits<XLOPER>::xword j)
	{
		ensure (i*fp.columns + j < size(fp));

		return fp.array[i*fp.columns + j];
	}
	inline double&
	index(_FP12& fp, traits<XLOPER12>::xword i, traits<XLOPER12>::xword j)
	{
		ensure (i*fp.columns + j < size(fp));

		return fp.array[i*fp.columns + j];
	}

	inline double
	index(const _FP& fp, traits<XLOPER>::xword i, traits<XLOPER>::xword j)
	{
		ensure (i*fp.columns + j < size(fp));

		return index(fp, i*fp.columns + j);
	}
	inline double
	index(const _FP12& fp, traits<XLOPER12>::xword i, traits<XLOPER12>::xword j)
	{
		ensure (i*fp.columns + j < size(fp));

		return index(fp, i*fp.columns + j);
	}
/*
	inline double&
	index(_FP& fp, traits<XLOPER>::xword i, traits<XLOPER>::xword j)
	{
		return index(fp, i*fp.columns + j);
	}
	inline double&
	index(_FP12& fp, traits<XLOPER12>::xword i, traits<XLOPER12>::xword j)
	{
		return index(fp, i*fp.columns + j);
	}
*/
	inline double*
	begin(_FP& fp)
	{
		return &fp.array[0];
	}
	inline double*
	begin(_FP12& fp)
	{
		return &fp.array[0];
	}
	inline double*
	end(_FP& fp)
	{
		return &fp.array[0] + size(fp);
	}
	inline double*
	end(_FP12& fp)
	{
		return &fp.array[0] + size(fp);
	}

	inline const double*
	begin(const _FP& fp)
	{
		return &fp.array[0];
	}
	inline const double*
	begin(const _FP12& fp)
	{
		return &fp.array[0];
	}
	inline const double*
	end(const _FP& fp)
	{
		return &fp.array[0] + size(fp);
	}
	inline const double*
	end(const _FP12& fp)
	{
		return &fp.array[0] + size(fp);
	}

	template<class X>
	class XFP {
	public:
		typedef double value_type;
		typedef value_type* pointer;		
		typedef const value_type* const_pointer;		
		typedef value_type& reference;		
		typedef const value_type& const_reference;		
		typedef typename traits<X>::xword size_type;
		typedef std::ptrdiff_t difference_type;

		typedef typename traits<X>::xword xword;
		typedef typename traits<X>::xfp xfp;
		XFP()
			: buf(0)
		{
			realloc(0,0);
		}
		XFP(xword r, xword c) 
			: buf(0) 
		{
			realloc(r, c);
		}
		XFP(const XFP& x) 
			: buf(0)
		{
			realloc(x.rows(), x.columns());
			copy(x.pf);
		}
		XFP(const xfp& x)
			: buf(0)
		{
			realloc(x.rows, x.columns);
			copy(&x);
		}
		XFP(std::initializer_list<double> a)
			: buf(0)
		{
			realloc(static_cast<xword>(a.size()), 1);
			copy(a.begin());
		}
		XFP& operator=(const XFP& x)
		{
			if (&x != this) {
				realloc(x.rows(), x.columns());
				copy(x.pf);
			}

			return *this;
		}
		XFP& operator=(const xfp& x)
		{
			realloc(x.rows, x.columns);
			copy(x.array);

			return *this;
		}
		~XFP()
		{
			free(buf);
		}

		XFP& operator=(double x)
		{
			resize(1,1);

			operator[](0) = x;

			return *this;
		}
		bool operator==(const xfp& x) const
		{
			return rows() == x.rows && columns() == x.columns
				&& std::equal(begin(), end(), &x.array[0]);
		}
		bool operator==(const XFP& x) const
		{
			return operator==(*x.pf);
		}
		bool operator!=(const xfp& x) const
		{
			return !operator==(x);
		}
		bool operator!=(const XFP& x) const
		{
			return !operator==(x);
		}

/*		XFP& set(xword r, xword c, const double* pa)
		{
			realloc(r, c);
			copy(pa);

			return *this;
		}
*/	private:
	public:
		xfp* get(void)
		{
			return pf;
		}
		// use when returning to Excel
		const xfp* get(void) const
		{
			return pf;
		}
		double* array()
		{
			return pf->array;
		}
		const double* array() const
		{
			return pf->array;
		}
		xword rows() const
		{
			return pf->rows;
		}
		xword columns() const
		{
			return pf->columns;
		}
		xword size() const
		{
			return buf ? (is_empty() ? 0 : pf->rows * pf->columns) : 0;
		}

		void reshape(xword r, xword c)
		{
			realloc(r, c);
		}
		void resize(xword r, xword c)
		{
			realloc(r, c);
		}
		double operator[](xword i) const
		{
			return index(*this, i);
		}
		double& operator[](xword i)
		{
			return pf->array[i];
		}
		double operator()(xword i, xword j) const
		{
			return index(*pf, i, j);
		}
		double& operator()(xword i, xword j)
		{
			return index(*pf, i, j);
		}

		double* begin()
		{
			return pf->array;
		}
		const double* begin() const
		{
			return pf->array;
		}
		double* end()
		{
			return pf->array + size();
		}
		const double* end() const
		{
			return pf->array + size();
		}
		template<class T>
		XFP& push_back(const T& t)
		{
			return push_back(&t, &t + 1);
		}
		template<class I>
		XFP& push_back(I b, I e, bool stack = false)
		{
			xword n = static_cast<xword>(std::distance(b, e));

			if (is_empty()) {
				reshape(1, n);
			}
			else if (stack) {
				ensure (columns() == 1 || columns() == n);

				if (columns() == 1)
					reshape(rows() + n, 1);
				else
					reshape(rows() + 1, n);
			}
			else if (columns() == 1) { // stack
				reshape(size() + n, 1);
			}
			else if (rows() == 1) {
				reshape(1, size() + n);
			}
			else {
				ensure (columns() == n);
				reshape(rows() + 1, columns());
			}

			std::copy(b, e, end() - n);

			return *this;
		}

		// buffer n rows
		template<class I>
		XFP& buffer(I b, I e, int n)
		{
			xword c = static_cast<xword>(std::distance(b, e));

			if (is_empty() || n == -1) {
				push_back(b, e, true);
			}
			// push_front
			else if (n == 0) {
				xword sz = size();
				ensure (c == columns());
				resize(rows() + 1, columns());
				memmove(begin() + c, pf->array, sz*sizeof(double));
				memcpy(begin(), b, c*sizeof(double));
			}
			else if (n < -1) {
				if (rows() == static_cast<xword>(-n)) {
					memmove(begin(), begin() + c, (size() - c)*sizeof(double));
					memcpy(end() - c, b, c*sizeof(double));
				}
				else {
					push_back(b, e, true);
				}
			}
			else {
				ensure (n > 0);
				if (rows() == static_cast<xword>(n)) {
					memmove(begin() + c, begin(), (size() - c)*sizeof(double));
					memcpy(begin(), b, c*sizeof(double));
				}
				else {
					xword sz = size();
					resize(rows() + 1, columns());
					memmove(begin() + c, begin(), sz*sizeof(double));
					memcpy(begin(), b, c*sizeof(double));
				}
			}

			return *this;
		}

		bool is_empty(void) const
		{
			return xll::is_empty(*pf);
		}
	private:
		void copy(const double* p)
		{
			pf->array[0] = p[0]; // could be empty array
			for (xword i = 1; i < size(); ++i)
				pf->array[i] = p[i];
		}
		void copy(const xfp* p)
		{
			ensure (pf->rows == p->rows);
			ensure (pf->columns == p->columns);

			copy(p->array);
		}
		void realloc(xword r, xword c)
		{
			if (!buf || size() != r*c) {
				void* p = ::realloc(buf, 2*sizeof(xfp) + r*c*sizeof(double));
				ensure (p != 0);
				if (p == 0)
					::free(buf);
				else
					buf = (char*)p;
				pf = reinterpret_cast<xfp*>(new (buf) xfp);
			}
//			memset(buf, 0, sizeof(xfp) + r*c*sizeof(double));
			//!!! check size
			pf->rows = r;
			pf->columns = c;

			// empty array
			if (r*c == 0) {
				pf->rows = 1;
				pf->columns = 1;
				pf->array[0] = std::numeric_limits<double>::quiet_NaN();
			}
			else if (is_empty()) {
				pf->array[0] = 0; // so resize(1,1) on empty array is not empty
			}
		}
		char* buf;
		xfp* pf;
	};

	typedef XFP<XLOPER>   FP;
	typedef XFP<XLOPER12> FP12;
	typedef XFP<XLOPERX>  FPX;

	// Excel SDK struct
#ifdef EXCEL12
	typedef _FP12 _FPX;
#else
	typedef _FP   _FPX;
#endif

} // namespace xll

