// ftap.cpp
#include <algorithm>
#include <numeric>
#include <iterator>
#include <tuple>
#include "../xll/xll.h"

// {n, -n}, {n, -n + 2}, ... {n, n}
template<int n>
struct binomial_iterator : std::iterator<std::input_iterator_tag, std::tuple<int,int>> {
	int k;
	binomial_iterator(int k = -n)
		: k(k)
	{
		ensure (-n <= k && k <= n && (n - k)%2 == 0);
	}
	binomial_iterator(const binomial_iterator&) = default;
	binomial_iterator& operator=(const binomial_iterator&) = default;
	~binomial_iterator()
	{ }

	bool operator==(const binomial_iterator& i) const
	{
		return k == i.k;
	}
	bool operator!=(const binomial_iterator& i) const
	{
		return k != i.k;
	}

	binomial_iterator& operator++()
	{
		k += 2;

		return *this;
	}
	std::tuple<int,int> operator*() const
	{
		return std::make_tuple(n, k);
	}

	static binomial_iterator begin()
	{
		return binomial_iterator(-n);
	}
	static binomial_iterator end()
	{
		return ++binomial_iterator(n);
	}
};

#pragma warning(disable: 189)
template<class F, class M>
inline double integrate(F f, M m)
{
	double fm{0};

	for (const auto& a : atoms(f))
		fm += f(a)*m(a);

	return fm;
}

// constexpr???
inline long choose(long n, long k)
{
	ensure (0 <= k && k <= n);

	k = (std::min)(k, n - k);

	return k == 0 ? 1 : choose(n - 1, k - 1) + choose(n - 1, k);
}

inline double binom(const std::tuple<int,int>& a)
{
	int n, k;

	std::tie(n, k) = a;

	ensure (-n <= k && k <= n);
	ensure ((n - k)%2 == 0);

	if (k > 0)
		k = -k;

	return k == -n ? 1/pow(2.,n) : 
		(
			binom(std::make_tuple(n - 1, k - 1))
			+ 
			binom(std::make_tuple(n - 1, k + 1))
		)/2;
}

struct discount {
	double r, dt;
	discount(double r, double dt)
		: r(r), dt(dt)
	{ }
	~discount()
	{ }
	double operator()(const std::tuple<int,int>& a) const
	{
		return exp(-r*dt*std::get<0>(a))*binom(a);
	}
};
struct stock {
	double s, mu, sigma, dt;
	stock(double s, double mu, double sigma, double dt)
		: s(s), mu(mu), sigma(sigma), dt(dt)
	{ }
	~stock()
	{ }
	double operator()(const std::tuple<int,int> a)
	{
		int n = std::get<0>(a);
		int k = std::get<1>(a);
		double t = n*dt;

		return s*exp(mu*t + sigma*k*sqrt(t))*pow(cosh(sigma*dt), -n);
	}
};
template<int n, class X>
struct restriction : public X {
	restriction(X x)
		: X(x)
	{ }
	binomial_iterator<n> atoms() const
	{
		return binomial_iterator<n>();
	}
};
template<int n, class X>
inline binomial_iterator<n> atoms(const restriction<n,X>& x)
{
	return x.atoms();
}

using namespace xll;

int xll_test_ftap()
{
	double x;
	x = binom(std::make_pair(1,1));
	x = binom(std::make_pair(1,-1));
	x = binom(std::make_pair(2,2));
	x = binom(std::make_pair(2,0));
	x = binom(std::make_pair(2,-2));
	x = binom(std::make_pair(3,3));
	x = binom(std::make_pair(3,1));
	x = binom(std::make_pair(3,-1));
	x = binom(std::make_pair(3,-3));

	discount D(0, 1);
	stock S(100, 0, .2, 1);

	double pv;

	pv = integrate(restriction<5,stock>(S), D);

	return 1;
}
static Auto<OpenX> xao_test_ftap(xll_test_ftap);