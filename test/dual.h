// dual.h - dual numbers X = x0 + x1 J + x2 J^2
#pragma once
#include <algorithm>
#include <functional>
#include <initializer_list>
#include <stdexcept>

namespace dual {
	template<class T>
	struct number_base {
		// must override _size
		size_t size() const 
		{ 
			return _size(); 
		}
		// must override _begin
		T* begin()
		{
			return _begin();
		}
		const T* begin() const
		{
			return _begin();
		}
		T* end()
		{
			return _begin() + _size();
		}
		const T* end() const
		{
			return _begin() + _size();
		}
		T* begin()
		{
			return _begin();
		}

		T& operator[](size_t i)
		{
			return *(begin() + i);
		}
		const T& operator[](size_t i) const
		{
			return *(begin() + i);
		}

		// scalar operators
		// += aJ^k
		number_base& plus_eq(const T& a, size_t k = 0)
		{
			if (a != 0 && k < size())
				index(k) += a;

			return *this;
		}
		number_base& operator+=(const T& a)
		{
			return plus_eq(a, 0);
		}
		number_base& operator-=(const T& a)
		{
			return operator+=(-a);
		}
		number_base& operator*=(const T& a)
		{
			for (size_t i = 0; i < size(); ++i)
				index(i) *= a;

			return *this;
		}
		number_base& operator-()
		{
			return operator*=(-1);
		}
		number_base& operator/=(const T& a)
		{
			return operator*=(1/a);
		}

		// multiply by J^k
		number_base& shift(size_t k)
		{
			if (k < size())
				std::copy_backward(begin(), begin() + size() - k, begin() + k);
			else
				std::fill(begin(), end(), 0);

			return *this;
		}
		number_base& operator+=(const number_base& A)
		{
			for (size_t i = 0; i < size(); ++i)
				index(i) += A[i];

			return *this;
		}
		number_base& operator-=(const number_base& A)
		{
			return operator+=(-A);
		}
		number_base& operator*=(const number_base& A)
		{
			for (size_t i = 0; i < size(); ++i)
				plus_eq(A[i], i);

			return *this;
		}
		// AB = sum_{0 <= i <= n} [ sum_{j + k = i} a_j b_k ] J^i
		// 0 = a0 b4 + ... + a4 b0 so b4 = -(a1 b3 + a2 b2 + a3 b1 + a4 b0)/a0.
		// 0 = a0 b3 + ... + a3 b0 so b3 = -(a1 b2 + a2 b1 + a3 b0)/a0
		// 0 = a0 b2 + ... + a2 a0 so b2 = -(a1 b1 + a2 b0)/a0
		// 0 = a0 b1 + a1 b0       so b1 = -(a1 b0)/a0
		// 1 = a0 b0               so b0 = 1/a0
		// b0 = 1/a0
		// b1 = -(a1 b0)/a0 = -(a1 b0)/(1/a0) = -(a1 b0) a0
		// b2 = -(a1 b1 + a2 b0)/a0 = -
		number_base& invert()
		{
			if (size() == 0)
				return *this;

			index(0) = 1/index(0);
			index(1) = -(index(1);
			for (size_t k = 1; k < size(); ++k) {
				index(k) = -index(i)*index()
			}
		}
		number_base& operator/=(const number_base& A)
	private:
		// override these in derived class
		virtual _size() const = 0;
		virtual T& _begin(size_t i) = 0;

	};
/*
	// wrap pre-existing data
	template<class T>
	class array_wrap {
		T* a_;
		size_t n_;
	public:
		typedef T value_type;
		array_wrap(T* a, size_t n)
			: a_(a), n_(n)
		{ }
		array_wrap(std::initializer_list<T> l)
			: a_(l.begin()), n_(l.size())
		{ }
		array_wrap(const array_wrap& a)
			: a_(a.a_), n_(a.n_)
		{ }
		array_wrap& operator=(const array_wrap& a)
		{
			if (this != &a) {
				a_ = a.a_;
				n_ = a.n_;
			}

			return *this;
		}
		~array_wrap()
		{ }

		T* data()
		{
			return a_;
		}
		size_t size() const
		{
			return n_;
		}

		T* begin()
		{
			return a_;
		}
		const T* begin() const
		{
			return a_;
		}
		T* end()
		{
			return a_ + n_;
		}
		const T* end() const
		{
			return a_ + n_;
		}
		T operator[](size_t i) const
		{
			return a_[i];
		}
		T& operator[](size_t i)
		{
			return a_[i];
		}
	};
*/
	// base class w/o copy or assignment
	template<class T>
	class number {
		T* a_;
		size_t n_;
		number(const number&);
		number& operator=(const number&);
	public:
		number(size_t n, double* a)
			: n_(n), a_(a)
		{ }
		number(std::initializer_list<T> l) // lifetime???
			: n_(l.size_t()), a_(l.begin())
		{ }
		~number()
		{ }

		size_t size() const
		{
			return n_;
		}

		// a0 I
		number& operator=(T a0)
		{
			if (size_ > 0)
				a[0] = a0;

			std::fill(a_.begin() + 1, a_.end(), 0);
		}
		number& operator+=(const number& b)
		{ 
			if (a_.size() < b.size())
				throw std::runtime_error("dual::number: incompatible sizes");

			std::transform(a_.begin(), a_.begin() + b.size(), b.a_.begin(), a_.begin(), std::plus<T>());

			return *this;
		}
		// *= J^n
		number& operator*=(size_t n)
		{
			if (n < size())
				std::copy_backwards(begin()+n, end(), end());
			
			std::fill(begin(), begin() + std::min(n, size()), 0);

			return *this;
		}
		// *= scalar
		number& operator*=(T a)
		{
			std::transform(begin(), end(), begin(), [a](T x) { return a*x; });

			return *this;
		}
		// a *= b = a *= b[0], 
		number& operator*=(const number& b)
		{
			for (size_t i = 0; i < b.size(); ++i) {
				a += b[i]; // J^k is shift by k
			}

			return *this;
		}
	};

	template<class T>
	class Number : public number<T>, private std::vector<T> {
	public:
		Number(size_t n, const T* a)
			: std::vector<T>(a, a + n)
		{
			n_ = std::vector<T>::size();
			a_ = std::vector<T>::begin();
		}
	}

} // namespace dual