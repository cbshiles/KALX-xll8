// environment.h - get/set environment variables
#pragma once
#include <processenv.h>
#include <string>

namespace Environment {

	template<class X>
	class Variable {
		typedef std::basic_string<X> xstring;
		xstring key_, val_;
	public:
		Variable(LPCTSTR key)
			: key_(key), val_(GetEnvironmentVariable(key, 0, 0), 0)
		{
			GetEnvironmentVariable(key, &val_[0], val_.length());
			val_.resize(val_.size() - 1); // remove trailing null
		}
		operator const xstring&() const
		{
			return val_;
		}
		const Variable& operator=(LPCTSTR val)
		{
			SetEnvironmentVariable(key_.c_str(), val);
			val_ = val;

			return *this;
		}
	};

}