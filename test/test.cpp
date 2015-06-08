// test.cpp - Test xll library
// Copyright (c) KALX, LLC. All rights reserved. No warranty made.
#include <functional>
//#include"../xll/utility/random.h"
#include "test.h"

using namespace xll;

typedef traits<XLOPERX>::xcstr xcstr;

inline std::function<int(void)> msg(LPCTSTR what)
{
	return [what](void) -> int { MessageBox(GetActiveWindow(), what, _T("Info"), MB_OK); return 1; };
}
int test_auto(void)
{
	Auto<OpenX> xao_test(msg(_T("AutoOpen")));
	Auto<OpenAfterX> xaoa_test(msg(_T("AutoOpenAfter")));
	Auto<CloseX> xac_test(msg(_T("AutoClose")));
	Auto<AddX> xaa_test(msg(_T("AutoAdd")));
	Auto<RemoveBeforeX> xarb_test(msg(_T("AutoRemoveBefore")));
	Auto<RemoveX> xar_test(msg(_T("AutoRemove")));

	return 1;
}
//static run run_auto(test_auto);

void test_error(void)
{
	DWORD xal;
	xal = xll_alert_level;
	xll_alert_level = 7;

	XLL_ERROR("(Not really an) Error", true);
	XLL_WARNING("Warning (about nothing)", true);
	char buf[1024] = "Info: xll_alert_level = ";
	_itoa(xal, buf + strlen(buf), 16);
	XLL_INFO(buf, true);

	xll_alert_level = xal;

	HMODULE hm;
	hm = xll_Instance;
}

XLL_ENUM(1.23, OTT, "Cat", "Desc");

// test_xloper

void test_oper(void)
{
	OPERX nil;
	ensure (nil.xltype == xltypeNil);
	ensure (!nil);
	ensure (nil == nil);
	ensure (nil == OPERX());
	ensure (OPERX() == nil);

	OPERX num(1.23);
	ensure (num.xltype == xltypeNum);
	ensure (num == num);
	ensure (num != nil);
	ensure (num == 1.23);
	ensure (1.23 == num);
	num = 2.34;
	ensure (2.34 == num);
	num = 4;
	ensure (num.xltype == xltypeInt); // like Excel
	ensure (num == 4);
	ensure (4 == num);

	OPERX str(_T("str"));
	ensure (str.xltype == xltypeStr);
	ensure (str.val.str[0] == 3);
	ensure (str == _T("str"));
//	ensure (num < str);
	XLOPERX xstr;
	xstr.xltype = xltypeStr;
	xstr.val.str = _T("\003Str");
	ensure (str == xstr);
	ensure (xstr == str);
	str = xstr;
	ensure (str.val.str[1] == 'S');

	xstr = str; // ok, but xstr points at garbage after str dies
	ensure (xstr == str);

	OPERX xbool(true);
	ensure (xbool.xltype == xltypeBool);
	ensure (xbool);
	ensure (xbool == true);
	ensure (xbool != false);
	ensure (!!xbool);
	ensure (str < xbool);
	xbool = false;
	ensure (xbool.xltype == xltypeBool);
	ensure (xbool == false);
	ensure (!xbool);
	ensure (xbool != true);

	OPERX multi(2, 3);
	ensure (multi.rows() == 2);
	ensure (multi.columns() == 3);
	ensure (multi.size() == 6);
	ensure (multi == multi);

	for (xword i = 0; i < multi.size(); ++i)
		ensure (multi[i] == OPERX());
	ensure (xbool < multi);

	multi[0] = num;
	multi(0, 1) = str;
	multi(1, 0) = xbool;
	ensure (multi(0,0) == num);
	ensure (multi[1] == str);
	ensure (multi[3] == xbool);

	// multi(1,2) = multi; // runtime exception
	OPERX m12 = multi;
	multi(1,2) = m12; // m12 does not step on multi memory
	ensure (multi(1,2) == m12);
	ensure (multi(1,2) != multi);
	ensure (multi[5] == m12); 
	ensure (multi[5] == multi(1,2)); 

	OPERX e(xlerr::Div0);
	ensure (e.xltype == xltypeErr);
	ensure (e.val.err == xlerrDiv0);
	ensure (e == xlerr::Div0);
	ensure (!e);
	ensure (e == e);
	ensure (e <= e);
	ensure (!(e!=e));
	ensure (!(e < e));
	ensure (e >= e);
	ensure (!(e > e));
}

void test_push_back()
{
	OPERX n;
	ensure (n.size() == 0);
	ensure (n == OPERX(xltype::Nil));
	// nil is nil
	n.push_back(n);
	ensure (n == OPERX());

	OPERX m(xltype::Missing);
	ensure (m.size() == 1);

	m.push_back(m);
	ensure (m.rows() == 2 && m.columns() == 1);
	ensure (m == m);
	ensure (m[0] == OPERX(xltype::Missing));
	ensure (m[1] == OPERX(xltype::Missing));
	m.push_back(n);
	ensure (m.rows() == 2 && m.columns() == 1);
	ensure (m[0] == OPERX(xltype::Missing));
	ensure (m[1] == OPERX(xltype::Missing));

	m.resize(0,0);

	m.push_back(OPERX(1.23));
	ensure (m.size() == 1);
	ensure (m.xltype == xltypeNum);
	ensure (m.val.num == 1.23);
	// but it also looks like a 1x1 range
	ensure (m.rows() == 1 && m.columns() == 1);
	ensure (m[0].xltype == xltypeNum);
	ensure (m[0] == 1.23);

	m.push_back(OPERX(_T("foo")));
	ensure (m.rows() == 2);
	ensure (m.columns() == 1);
	ensure (m[0] == 1.23);
	ensure (m[1] == _T("fOo"));

	LPXLOPERX pa = m.val.array.lparray;
	m.resize(1, 2);
	ensure (pa == m.val.array.lparray);
	m.push_back(OPERX(2,2));
	ensure (m.rows() == 3);
	ensure (m.columns() == 2);
	ensure (m(0,0) == 1.23);
	ensure (m(0,1) == _T("FoO"));
	ensure (m(1,0) == OPERX());
	ensure (m(2,1) == OPERX());
}

inline LOPERX Coerce(xcstr str)
{
	return Excel<XLOPERX>(xlCoerce, OPERX(str));
}

void test_excel(void)
{

	LOPERX lo(Excel<XLOPERX>(xlCoerce, OPERX(_T("Str"))));
	ensure (lo == _T("Str"));
	OPERX o(lo);
	ensure (o == _T("Str"));
	o = lo;
	ensure (o == _T("sTr"));

	LOPERX lo2;
	lo2 = Excel<XLOPERX>(xlfConcatenate, Coerce(_T("Hello ")), Coerce(_T("World!")));
	ensure (lo2 == _T("Hello World!"));

	OPERX o2;
	o2 = Excel<XLOPERX>(xlfConcatenate, Coerce(_T("Hello ")), Coerce(_T("World!")));
	ensure (o2 == _T("Hello World!"));

}

static AddInX xai_bad_macro(_T("?xll_bad_macro"), _T("BAD.MACRO"));
int WINAPI xll_bad_macro(void)
{
#pragma XLLEXPORT

	XLL_XLC(Alert, OPERX(_T("BAD.MACRO called")));

	return 1;
}
void test_on(void)
{
	On<Key> ok(ON_CTRL ON_SHIFT _T("k"), _T("BAD.MACRO"));
}

void test_name(void)
{
	Excel<XLOPERX>(xlfSetName, OPERX(_T("foo")), OPERX(_T("bar")));
	OPERX foo = Excel<XLOPERX>(xlfGetName, OPERX(_T("foo")));
	ensure (foo == _T("=\"bar\""));
	ensure (Excel<XLOPERX>(xlfEvaluate, foo) == _T("bar"));

	OPERX o(2,3);
	o[0] = 1.23;
	o[1] = _T("foo");
	o[2] = false;
	o[3] = OPERX(xlerr::NA);

	Excel<XLOPERX>(xlfSetName, OPERX(_T("foo")), o);
	OPERX o2 = Excel<XLOPERX>(xlfGetName, OPERX(_T("foo")));
	o[4] = o[5] = o[3]; // xltypeMissing gets converted to #N/A.
	ensure (o == Excel<XLOPERX>(xlfEvaluate, o2));

	Excel<XLOPERX>(xlfSetName, OPERX(_T("foo")));
}

void test_to_string(void)
{
	OPERX o;
	traits<XLOPERX>::xstring s;
/*
	s = o.to_string();
	ensure (s == _T("=\"\""));
	o = OPERX(s);
	ensure (o.xltype == xltypeStr && o.val.str[0] == 3);
	o = Excel<XLOPERX>(xlfEvaluate, OPERX(s));
	ensure (o == _T(""));

	o = OPERX(xltype::Missing);
	s = o.to_string();
	ensure (s == _T("=0"));

	o = 1.23;
	s = o.to_string();
	ensure (s == _T("=1.23"));
	ensure (o == Excel<XLOPERX>(xlfEvaluate, OPERX(s)));

	o = _T("string");
	s = o.to_string();
//	o = Excel<XLOPERX>(xlfEvaluate, OPERX(s));
	ensure (s == _T("=\"string\""));
	ensure (o == Excel<XLOPERX>(xlfEvaluate, OPERX(s)));

	o = true;
	s = o.to_string();
	ensure (s == _T("=TRUE"));
	ensure (o == Excel<XLOPERX>(xlfEvaluate, OPERX(s)));
	
	o = false;
	s = o.to_string();
	ensure (s == _T("=FALSE"));
	ensure (o == Excel<XLOPERX>(xlfEvaluate, OPERX(s)));

	o = OPERX(xlerr::NA);
	s = o.to_string();
	ensure (s == _T("=#N/A"));
	ensure (o == Excel<XLOPERX>(xlfEvaluate, OPERX(s)));

	o = OPERX(2,3);
	o[0] = 1.23;
	o[1] = _T("string");
	o[2] = true;
	o[3] = OPERX(xltype::Missing);
	o[4] = OPERX(xlerr::Ref);
	s = o.to_string();
	o = Excel<XLOPERX>(xlfEvaluate, OPERX(s));
	ensure (s == _T("={1.23,\"string\",TRUE;#VALUE!,#REF!,#N/A}"));
	ensure (o == Excel<XLOPERX>(xlfEvaluate, OPERX(s)));
*/
}

static XAddIn<XLOPERX> xai_foo(
	_T("?xll_foo"), XLL_DOUBLEX XLL_DOUBLEX,
	_T("Foo"), _T("Arg")
);
double WINAPI xll_foo(double x)
{
#pragma XLLEXPORT

	return 2*x;
}

void test_loper(void)
{
	LOPERX o;
	ensure (o.xltype == OPERX().xltype);

	o = XLL_XLF(Concatenate, 
			OPERX(_T("a")),
			XLL_XLF(Concatenate, OPERX(_T("b")), OPERX(_T("c")))
			);
	ensure (o == _T("abc"));

}

void test_initializer(void)
{
	OPERX o{OPERX(1.2), OPERX(_T("abc")), OPERX(false)};

	ensure (o.rows() == 1);
	ensure (o.columns() == 3);
	ensure (o[0] == 1.2);
	ensure (o[1] == _T("abc"));
	ensure (o[2] == false);
/*
	OPERX o2(1.2, _T("abc"), false);
	ensure (o2.rows() == 3);
	ensure (o2.columns() == 1);
	ensure (o2[0] == 1.2);
	ensure (o2[1] == _T("abc"));
	ensure (o2[2] == false);
*/
}

void test_to_operx(void)
{
	OPERX o;
	o = to_XOPER<XLOPERX>(_T("1.23"));
	ensure (o == 1.23);
	o = to_XOPER<XLOPERX>(_T("foobarbaz"));
	ensure (o == _T("foobarbaz"));
	o = to_XOPER<XLOPERX>(_T("\"foobarbaz\""));
	ensure (o == _T("foobarbaz"));
	o = to_XOPER<XLOPERX>(_T("FALSE"));
	ensure (o == false);
	o = to_XOPER<XLOPERX>(_T("#N/A"));
	ensure (o == OPERX(xlerr::NA));
	o = to_XOPER<XLOPERX>(_T("2013-1-2"));
	ensure (o == XLL_XLF(Date, OPERX(2013), OPERX(1), OPERX(2)));

}
#ifndef _DEBUG
#pragma warning(disable: 4127)
#endif

int xll_test(void)
{
	try {
//		_CrtSetBreakAlloc(1275);
		struct { int i; double d; char c; } s = {1, 1.23, 'c'};
		auto t = std::make_tuple<int, double , char>(1, 1.23, 'c');
		auto pt = &t;
		auto ps = &s;
		pt = pt; ps = ps;
		int i;
		i = std::get<0>(t);
		xai_foo.Register();
		size_t n;
		n = sizeof(double);
		ensure (n == 8);

		n = sizeof(XLOPER);
#ifdef _M_X64 
		ensure (n == 24);
#else
		ensure (n == 16);
#endif

		n = sizeof(XLOPER12);
		ensure (n == 32);
//		_crtBreakAlloc = 102;
		test_oper();
		test_push_back();
		test_excel();
		test_on();
		test_name();
		test_to_string();
		test_loper();
		test_initializer();
		test_to_operx();
	}
	catch (const std::exception& ex) {
		MessageBoxA(0, ex.what(), "Error", MB_OK);

		return 0;
	}
	catch (...) {
		MessageBoxA(0, "Unknown exception", "Error", MB_OK);

		return 0;
	}

	return 1;
}
// wait until Excel is open for business
static Auto<OpenAfterX> xao_test(xll_test);

static AddInX xai_bar(
	FunctionX(XLL_LPOPERX, _T("?xll_bar"), _T("XLL.BAR"))
	.Arg(XLL_DOUBLEX, _T("Arg"), _T("is an arg"))
	.Documentation(R_(
		This is some 
		documentation.
		It can use commas, 
		but not parentheses.
		Also,			white       space 
		gets     compressed.
	))
);
LPOPERX WINAPI
xll_bar(double arg)
{
#pragma XLLEXPORT
	static OPERX Arg;

	Arg = arg;

	return &Arg;
}

#if 0

namespace Global {
	class Memory {
		HGLOBAL h_;
		Memory(const Memory&);
		Memory& operator=(const Memory&);
	public:
		Memory(size_t n, UINT flags = GMEM_FIXED)
		{
			h_ = ::GlobalAlloc(flags, n);
		}
		~Memory()
		{
			::GlobalFree(h_);
		}
		operator void*()
		{
			return static_cast<void*>(h_);
		}
	};
}

/*	// generate manifest dependency
	// http://msdn.microsoft.com/en-us/library/ew0y5khy.aspx
#pragma comment(linker, \
    "\"/manifestdependency:type='Win32' "\
    "name='Microsoft.Windows.Common-Controls' "\
    "version='6.0.0.0' "\
    "processorArchitecture='*' "\
    "publicKeyToken='6595b64144ccf1df' "\
    "language='*'\"")

*/

INT_PTR CALLBACK DialogProc(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	lParam = lParam;

    switch (message)
    {
	case WM_DESTROY:
		PostQuitMessage(0);
		// fall through
    case WM_INITDIALOG:
        return TRUE;

    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case IDOK:
        case IDCANCEL:
            EndDialog(hwndDlg, LOWORD(wParam));
            return TRUE;
        }
    }
    return FALSE;
}

#define DLGTITLE L"Title"
#define DLGOK L"Ok"
#define DLGCancel L"Cancel"
#define NUMCHARS(x) (sizeof(x)/sizeof(*x))

#define ID_HELP   150
#define ID_TEXT   200

LPWORD lpwAlign(LPWORD lpIn)
{
    ULONG ul;

    ul = (ULONG)lpIn;
    ul ++;
    ul >>=1;
    ul <<=1;
    return (LPWORD)ul;
}

#pragma warning(disable: 4324)
struct Dialog {
    __declspec(align(4)) 
	DLGTEMPLATE dt;

    __declspec(align(4)) 
	WORD   menu;         // name or ordinal of a menu resource
    WORD   windowClass;  // name or ordinal of a window class
    WCHAR  wszTitle[NUMCHARS(DLGTITLE)]; // title string of the dialog box
//    short  pointsize;    // only if DS_SETFONT flag is set
//    WCHAR  wszFont[1];   // typeface name, if DS_SETFONT is set

	DLGITEMTEMPLATE dit;

    __declspec(align(4)) 
	WORD   sysClass;       // 0xFFFF identifies a system window class
	WORD   idClass;        // ordinal of a system window class
	WCHAR  wszOKTitle[NUMCHARS(DLGOK)];    // title string or ordinal of a resource
	WORD   cbCreationData; // bytes of following creation data
};

int xll_dialog(void)
{
	Dialog d = 
	{
		{
			WS_POPUP | WS_BORDER | WS_SYSMENU | DS_MODALFRAME | WS_CAPTION,
			0, // extended style
			1, // number of controls
			10, 10, // x, y
			100, 100 // w, h
		},
		0, // menu
		0, // window class
		DLGTITLE,
		{
			WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
			WS_EX_NOPARENTNOTIFY,
			10, 70,
			80, 20,
			IDOK,
		},
		0xFFFF, 0x0080, // button
		DLGOK, 
		0,
	};

	DWORD* pdw;
	pdw = (DWORD*)&d;
	HWND hwnd = xll_GetHwnd();
	INT_PTR p;
//	MessageBox(hwnd, _T("text"), _T("caption"), MB_OK);
	p = DialogBoxIndirect(xllInstance, (LPDLGTEMPLATE)&d, hwnd, DialogProc);
	DWORD gle;
	gle = GetLastError();

    HGLOBAL hgbl;
    LPDLGTEMPLATE lpdt;
    LPDLGITEMTEMPLATE lpdit;
    LPWORD lpw;
    LPWSTR lpwsz;
    LRESULT ret;
    int nchar;

    hgbl = GlobalAlloc(GMEM_ZEROINIT, 1024);
    if (!hgbl)
        return -1;
 
    lpdt = (LPDLGTEMPLATE)GlobalLock(hgbl);
 
    // Define a dialog box.
 
    lpdt->style = WS_POPUP | WS_BORDER | WS_SYSMENU | DS_MODALFRAME | WS_CAPTION;
    lpdt->cdit = 3;         // Number of controls
    lpdt->x  = 10;  lpdt->y  = 10;
    lpdt->cx = 100; lpdt->cy = 100;

    lpw = (LPWORD)(lpdt + 1);
    *lpw++ = 0;             // No menu
   *lpw++ = 0;             // Predefined dialog box class (by default)

    lpwsz = (LPWSTR)lpw;
    nchar = 1 + MultiByteToWideChar(CP_ACP, 0, "My Dialog", -1, lpwsz, 50);
    lpw += nchar;

    //-----------------------
    // Define an OK button.
    //-----------------------
    lpw = lpwAlign(lpw);    // Align DLGITEMTEMPLATE on DWORD boundary
    lpdit = (LPDLGITEMTEMPLATE)lpw;
    lpdit->x  = 10; lpdit->y  = 70;
    lpdit->cx = 80; lpdit->cy = 20;
    lpdit->id = IDOK;       // OK button identifier
    lpdit->style = WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON;

    lpw = (LPWORD)(lpdit + 1);
    *lpw++ = 0xFFFF;
    *lpw++ = 0x0080;        // Button class

    lpwsz = (LPWSTR)lpw;
    nchar = 1 + MultiByteToWideChar(CP_ACP, 0, "OK", -1, lpwsz, 50);
    lpw += nchar;
    *lpw++ = 0;             // No creation data

    //-----------------------
    // Define a Help button.
    //-----------------------
    lpw = lpwAlign(lpw);    // Align DLGITEMTEMPLATE on DWORD boundary
    lpdit = (LPDLGITEMTEMPLATE)lpw;
    lpdit->x  = 55; lpdit->y  = 10;
    lpdit->cx = 40; lpdit->cy = 20;
    lpdit->id = ID_HELP;    // Help button identifier
    lpdit->style = WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON;

    lpw = (LPWORD)(lpdit + 1);
    *lpw++ = 0xFFFF;
    *lpw++ = 0x0080;        // Button class atom

    lpwsz = (LPWSTR)lpw;
    nchar = 1 + MultiByteToWideChar(CP_ACP, 0, "Help", -1, lpwsz, 50);
    lpw += nchar;
    *lpw++ = 0;             // No creation data

    //-----------------------
    // Define a static text control.
    //-----------------------
    lpw = lpwAlign(lpw);    // Align DLGITEMTEMPLATE on DWORD boundary
    lpdit = (LPDLGITEMTEMPLATE)lpw;
    lpdit->x  = 10; lpdit->y  = 10;
    lpdit->cx = 40; lpdit->cy = 20;
    lpdit->id = ID_TEXT;    // Text identifier
    lpdit->style = WS_CHILD | WS_VISIBLE | SS_LEFT;

    lpw = (LPWORD)(lpdit + 1);
    *lpw++ = 0xFFFF;
    *lpw++ = 0x0082;        // Static class

	LPSTR lpszMessage = "Message";
	for (lpwsz = (LPWSTR)lpw; *lpwsz++ = (WCHAR)*lpszMessage++;);
    lpw = (LPWORD)lpwsz;
    *lpw++ = 0;             // No creation data

    GlobalUnlock(hgbl); 
    ret = DialogBoxIndirect(xllInstance, 
                           (LPDLGTEMPLATE)hgbl, 
                           GetHwnd(), 
                           (DLGPROC)DialogProc); 
    GlobalFree(hgbl); 
	return 1;
}
//static Auto<OpenAfterX> xao_dialog(xll_dialog);

// return capitals in lower case or original string if no caps
inline xstring canonicalize(const xchar* s, xchar n = 0)
{
	xstring cs;

	for (xchar n_ = n; *s; ++s) {
		if (::isupper(*s))
			cs.push_back(*s);
		if (n && !--n_)
			break;
	}

	return cs.length() ? cs : n ? xstring(s, n) : xstring(s);
}

struct Struct {
	int i;
	double d;
	char* s;
};

#endif
