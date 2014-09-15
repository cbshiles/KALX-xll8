// struct.h - make structs accessible from Excel
/*

struct isd {
	int i;
	const char* s;
	double d;
};

Struct<isd> Sisd;
Sisd.Member<const char*>(offsetof(S, s), "s", "is a string");

const char* s = Sisd.get("s");
Sisd.set("s", "value of isd.s");

Excel

h=Sisd(kvpairs)
=Sisd.set(h, "s", "string from Excel")
=Sisd.get(h, "s") -> "string from Excel"

s = to_struct(kv)
{
	Struct<S> s;

	for (xword = 0; i < kv.rows(); ++i) {
		s[kv(i, 0)] = kv(i, 1);
	}
	
	return s; // operator S
}
kv = from_struct(Struct<S> s)
{
	for (auto i  = s.begin(); i != s.end(); ++i) {
		
	}
}

*/

namespace xll {

	template<struct S>
	class Struct : public S {
		std::map<std::string, 
	public:
		template<typename T>
		void Member(T t, size_t offset, const char* name, const char* desc)
		{
		}
	};

} // namespace xll