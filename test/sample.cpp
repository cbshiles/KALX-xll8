#include <bitset>
#include <cmath> // for sqrt
#include "xll/xll.h"

using namespace xll;

#pragma region xai_sqrt

static AddIn xai_sqrt(
	Function(XLL_LPOPER, "?xll_sqrt", "XLL.SQRT")
	.Num("Number", "is a floating point number.", "=1+2")
	.Category("XLL")
	.FunctionHelp("Return the square root of Number or #NUM! if less than 0.")
);
LPOPER WINAPI xll_sqrt(double num)
{
#pragma XLLEXPORT
	static OPER o;

	if (num >= 0)
		o = sqrt(num);
	else
		o = OPER(xlerr::Num);

	return &o;
}

#pragma endregion xai_sqrt

#pragma region xai_xltype

//XLL_ENUM(xltypeNum, FooBarX, "XLL", "Excel OPER type xltypeNum = " ENSURE_STRZ_(xltypeNum) ".");

// Illustrate OPER types
static AddIn xai_xltype(
	Function(XLL_LPOPER, "?xll_xltype", "XLTYPE")
	.Arg(XLL_LPXLOPER, "Type", "is an xltype value.")
	.Arg(XLL_BOOL, "_Coerce", "is an optional Boolean value indicating Type should be coerced.")
	.Category("XLL")
	.FunctionHelp("Return an example with xltype Type.")
);
LPOPER WINAPI xll_xltype(LPXLOPER pType, BOOL coerce)
{
#pragma XLLEXPORT
	static OPER o;

	auto isflag = [](const OPER& o) {
		// double as unsigned 32 bit int has exactly 1 bit set
		return o.xltype == xltypeNum && std::bitset<32>(static_cast<_Uint32t>(o.val.num)).count() == 1;
	};

	if (coerce)
		o = XLL_XL_(Coerce, *pType);
	else
		o = *pType;

	if (isflag(o)) {
		SHORT type = static_cast<SHORT>(o.val.num);
		switch (type) {
		case xltypeNum:
			o = 1.23;
			break;
		case xltypeStr:
			o = "A string.";
			break;
		case xltypeBool:
			o = true;
			break;
		case xltypeMulti:
			o = OPER(2,3); // !!! OPER({1, "string", true, OPER(xlerr::NA)}); // rows, values
			// OPER({{1,2}, {3,4}, {5,6}}); 
			break;
		default:
			o = OPER(xlerr::NA);
		}
	}
	else {
		o = pType->xltype;
	}

	return &o;
}

#pragma endregion xai_xltype
