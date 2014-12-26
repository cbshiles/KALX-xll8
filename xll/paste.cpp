// paste.cpp - paste functions into Excel
#include "xll.h"

using namespace xll;

template<class X>
inline bool paste_basic(void)
{
	try {
		auto xstr = [](const char* s) { return traits<X>::string(s, 0); };
		XOPER<X> ac = Excel<X>(xlfActiveCell);
		XOPER<X> id = Excel<X>(xlCoerce, ac);

		const XAddIn<X>* pa = XAddIn<X>::Find(id);
		if (!pa)
			return false;
		const XArgs<X>& a = pa->Args();

		XOPER<X> formula(xstr("="));
		formula.append(a.FunctionText());
		formula.append(traits<X>::string("("));

		for (xword i = 0; i < a.Arity(); ++i) {
			XOPER<X> xOff = Excel<X>(xlfOffset, ac, XOPER<X>(i + 1), XOPER<X>(0));
			Excel<X>(xlSet, xOff, a.Default(i));
			
			formula.append(xstr(i > 1 ? ", " : ""));
			formula.append(xstr("R["));
			formula.append(XOPER<X>(i + 1));
			formula.append(xstr("]C"));
		}
		formula.append(xstr(")"));

//		Excel<X>(xlcSelect, ac);
		Excel<X>(xlcFormula, formula);
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());

		return false;
	}

	return true;
}

// Read register id in active cell and paste formula hooked up to default arguments.
static AddIn xai_paste_basic(XLL_DECORATE("xll_paste_basic",0), "XLL.PASTE.BASIC");
extern "C" int __declspec(dllexport) WINAPI xll_paste_basic(void)
{
	try {
		ensure (paste_basic<XLOPER>() || paste_basic<XLOPER12>());
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());

		return FALSE;
	}

	return TRUE;
}
// Ctrl-Shift-B (similar to Ctrl-Shift-A)
static On<Key> xok_paste_basic(ON_CTRL ON_SHIFT _T("B"), _T("XLL.PASTE.BASIC"));
