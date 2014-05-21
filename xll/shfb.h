// shfb.h - Sandcastle Help File Builder
#pragma once
#include <string>
#include <vector>
#include <iosfwd>
#include "xll.h"

namespace xml {

	template<class X, class C>
	class element {
		typedef xll::traits<X>::xcstr xcstr;
		typedef xll::traits<X>::xstring xstring;
		xstring element_, content_;
		std::vector<std::pair<xstring,xstring> attribute_;
	public:
		element(xcstr e, xcstr c = _T(""))
			: element_(e), content_(c)
		{
		}
		~element()
		{ }
		element& attribute(xcstr key, xcstr val)
		{
			attribute_.push_back(std::make_pair(key,val));

			return *this;
		}
		element& content(xcstr c)
		{
			content_.append(c);

			return *this;
		}
		std::ostream& operator<<(std::ostream& os)
		{
			os << _T("<") << element_;
			
			for (size_t i = 0; i < attribute_.size(); ++i)
				os << _T(" \"") << attribute_[i].first << _T("\"=\"") << attribute_[i].second;
			
			if (content_.size() {
				os << content_;
				os << _T("\n</") << element_ << _T(">\n");
			}
			else {
				os << _T("/>"); // minimized
			}
		}
	};

} // xml

namespace shfb {

	// xml for AddIn
	std::ostream& operator<<(std::ostream& os, const xll::AddInX& xai)
	{
		os << _T("<table>\n");

		const xll::ArgsX& args = xai.Args();
		for (int i = 0; i < args.size(); ++i) {
			os << _T("<row>\n");
			os << _T("<entry>");
			os << 
			os << _T("</row>\n");
		}

		os << _T("</table>");
	}

} // shfb