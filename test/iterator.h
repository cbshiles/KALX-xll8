// iterator.h
#pragma once
#include <algorithm>
#include <iterator>
#include <numeric>

// N-dimensional array wrapper class
template<class T, size_t N>
class narray {
	T* a_;
	size_t dim_[N];
public:
	narray(T* a, size_t* dim)
		: a_(a), std::copy(dim, dim + N, dim_)
	{ }
	narray(const narray& a)
		: a_(a.a_), std::copy(a.dim_, a.dim_ + N, dim_)
	{ }
	narray& operator=(const narray& a)
	{
		if (this != &a) {
			a_ = a.a_;
			std::copy(a.dim_, a.dim_ + N, dim_);
		}

		return *this;
	}
	~narray()
	{ }

	narray<T,N-1> operator[](size_t i)
	{
		ensure (i < dim_[0]);

		return narray<T,N-1>(a_ + i*std::accumuate(dim_ + 1, dim_ + N, 1, std::multiplies<T>), dim_ + 1);
	}
};

namespace iterator {

	template<class T>
	class input : public std::iterator<std::input_iterator_tag, input<T>> {
		T* i_;
		size_t n_;
	public:
		input(T* i = 0, size_t n = 1)
			: i_(i), n_(n)
		{ }
		input(const input& i)
			: i_(i.i_), n_(i.n_)
		{ }
		input& operator=(const input& i)
		{
			if (this != &i) {
				ensure (n_ == i.n_);
				std::copy(i.i_, i.i_ + i.n_, i_);
			}

			return *this;
		}
		~input()
		{ }

		bool operator==(const input& i)
		{
			return n_ == i.n_ && std::equal(i_, i_ + n_, i.i_);
		}
		void swap(input& i)
		{
			std::swap_ranges(i.i_, i.i_ + i.n_, i_);
		}

		input<T> operator*()
		{
			return input(i_, n_);
		}
		input& operator++()
		{
			i_ += n_;

			return *this;
		}
		input operator++(int)
		{
			input i(i_, n_);

			i_ += n_;

			return i;
		}
	};

}