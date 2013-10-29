// dual.h - dual numbers X = x0 + x1 J + x2 J^2
#pragma once
#include <algorithm>
#include <functional>
#include <initializer_list>
#include <stdexcept>

namespace dual {

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

	// e.g. number a(array<double,4>({1,2,3,4}))
	template<class T>
	class number {
		T* a_;
		size_t n_;
	public:
		// user responsible for lifetime of a
		template<class A>
		explicit number(const A& a)
			: a_(a.begin()), n_(a.size())
		{ }
		template<class A>
		explicit number(A& a)
			: a_(a.begin()), n_(a.size())
		{ }
		number(std::initializer_list<T> l) // lifetime???
			: a_(l.begin(), l.end())
		{ }
		number(const number& a)
			: a_(a)
		{ }
		// a J^k
		number(T a = 1, size_t k = 0)
		{

		}
		number& operator=(const number& a)
		{
			if (this != & a) {
				a_ = a;
			}

			return *this;
		}
		~number()
		{ }

		size_t size() const
		{
			return a_.size();
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

} // namespace dual