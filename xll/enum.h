// enum.h - Enumerated values
// Generic mapping from names to members of a struct.
// OPERX x = xll::get(t, _T("name")); xll::set(t, _T("name"), v)
/*
#define INFO(_T("The Long Name"), t, m) { xcstr name = ""; decltype(((T*)nullptr)->m); offsetof(t, m) }
template<class T>
struct getset {
	typedef typename double type;
	const size_t offset = offsetof(t, dmember);
};
static getset<some_struct> 
};
template<>
inline OPERX get(const T& t, xcstr name)
{
	getset<T> m(t, name);

	return OPERX(*static_cast<m::type*>(&t + m::offset));
}
*/
#include <algorithm>
#include <string>

namespace xll {
	// pull out the capital letters and transform to lower case
	// e.g., "DigitalRightsManagement" -> "drm"
	inline std::basic_string<char> canonicalize(const char* name, size_t n = 0)
	{
		std::basic_string<char> s;

		while (*name) {
			if (::isupper(*name)) {
				s.push_back((char)::tolower(*name));
			}
		}

		if (!s.length()) {
			s = name;
		}

		std::transform(s.begin(), s.end(), s.begin(), ::tolower);

		return s;
	}

} // xll