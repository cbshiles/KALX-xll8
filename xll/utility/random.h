// random.h - random data generation
#include <random>

namespace random {

	inline std::default_random_engine& default_random_engine(void)
	{
		static std::default_random_engine dre;

		return dre;
	}

	// uniform [a, b)
	inline double uniform_real(double a = 0, double b = 1)
	{
		std::uniform_real_distribution<> u(a, b);

		return u(default_random_engine());
	}
	// uniform [a, b]
	inline int uniform_int(int a = 0, int b = 1)
	{
		std::uniform_int_distribution<> u(a, b);

		return u(default_random_engine());
	}
/*
	inline int random_xltype(void)
	{
		int xltype[] = {
			xltypeNum,
			xltypeStr,
			xltypeBool,
//			xltypeRef,
			xltypeErr,
//			xltypeFlow,
			xltypeMulti,
			xltypeMissing,	
			xltypeNil,
//			xltypeSRef,
			xltypeInt,
		};
	}
*/

} // namespace random