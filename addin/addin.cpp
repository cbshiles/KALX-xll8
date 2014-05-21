// addin.cpp - initial add-in to configure all add-ins
//#define EXCEL12
#include "../xll/xll.h"
#include <Shlwapi.h>
#include "../xll/utility/environment.h"
#include "../xll/utility/registry.h"

#pragma comment(lib, "shlwapi.lib")

#define ONEDRIVE_URL _T("https://apis.live.net/v5.0/me")
#define KEY_KALX _T("Software\\KALX\\xll")

using namespace xll;

typedef traits<XLOPERX>::xchar xchar;
typedef traits<XLOPERX>::xcstr xcstr;
typedef traits<XLOPERX>::xstring xstring;

#define XLL_STR(o) o.val.str+1, o.val.str+o.val.str[0]+1

inline bool xll_key_exists(LPCTSTR pKey)
{
	HKEY h;

	return ERROR_SUCCESS == RegOpenKeyEx(HKEY_CURRENT_USER, pKey, 0, KEY_ALL_ACCESS, &h)
		? RegCloseKey(h), true
		: false;
}

inline OPERX xl_ver(void)
{
	return XLL_XLF(GetWorkspace, OPERX(2));
}
inline xstring str_ver(void)
{
	LOPERX v = XLL_XLF(Evaluate, xl_ver());

	return xstring(XLL_STR(v));
}
inline xstring aim_key(void)
{
	xstring key(_T("Software\\Microsoft\\Office\\"));
	key.append(str_ver());
	key.append(_T("\\Excel\\Add-in Manager"));

	return key;
}
inline xstring opt_key(void)
{
	xstring key(_T("Software\\Microsoft\\Office\\"));
	key.append(str_ver());
	key.append(_T("\\Excel\\Options"));

	return key;
}

// Set file for Add-In Manager
void xll_addin_manager(const OPERX& file)
{
	Reg::OpenKey<xchar> aim(HKEY_CURRENT_USER, aim_key().c_str());
	aim.SetValue(xstring(XLL_STR(file)).c_str(), _T(""));
}

void xll_options_open(const OPERX& file)
{
	Reg::OpenKey<xchar> xoo(HKEY_CURRENT_USER, opt_key().c_str());
	int i = 0;
	do {
		xstring open(_T("Open"));
		try {
			xoo.QueryValue<const xchar*>(open.c_str());
			open = xstring(_T("Open")) + std::to_string(++i);
		}
		catch (...) {
			xoo.SetValue<const xchar*>(open.c_str(), xstring(XLL_STR(file)).c_str());
			break;
		}
	} while (i >= 0);
}

// return install directory
OPERX xll_init(void)
{
	OPERX module = XLL_XL_(GetName);

	OPERX path(module);
	PathRemoveFileSpec (path.val.str + 1);
	path.val.str[0] = traits<XLOPERX>::strlen(path.val.str + 1);
	
	OPERX file(module);
	file.append(_T(""), 1); // null terminate
	PathStripPath(file.val.str + 1);
	file.val.str[0] = traits<XLOPERX>::strlen(file.val.str + 1);
	
	// suggested storage location
	Environment::Variable<xchar> appdata(_T("APPDATA"));
	xstring xll = appdata;
	xll += _T("\\xll");

	OPERX cwd = XLL_XLF(Directory);

	if (!PathIsDirectory(xll.c_str()))
		ensure (CreateDirectory(xll.c_str(), 0));
	SetCurrentDirectory(xll.c_str());

	// user can save anywhere
	OPERX save = XLL_XLF(SaveDialog, file, OPERX(_T("Choose folder to save add-in.")), OPERX(_T("Save")), OPERX(_T("(*.xll), *.xll")));

	XLL_XLF(Directory, cwd);

	if (save) {
		ensure (CopyFile(xstring(XLL_STR(module)).c_str(), xstring(XLL_STR(save)).c_str(), FALSE));

//		XLL_XLC(AddinManager, OPERX(3), save); // fails
		xll_addin_manager(save);

		if (XLL_XLC(Alert, OPERX(_T("Load on Excel startup?"))))
			xll_options_open(save);
	}

	return save;
}

void xll_load(const OPERX& dir)
{
//	for (const auto& f : XLL_XLF(Files, XLL_XLF(Concatenate, dir, OPERX(_T("\\*.xll")))))
//		XLL_XLC(Open, f);
}

int xll_open(void)
{
	try {
		OPERX dir = xll_init();

		if (dir)
			xll_load(dir);
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());

		return 0;
	}

	return 1;
}
//static Auto<OpenX> xao_open(xll_open);

int xll_close(void)
{
	try {
		if (xll_key_exists(KEY_KALX _T("InstallDir"))) {
			Reg::Object<xchar, const xchar*> dir(HKEY_CURRENT_USER, KEY_KALX, _T("InstallDir"));
		}
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());

		return 0;
	}

	return 1;
}
static Auto<CloseX> xac_close(xll_close);

static AddInX xai_version(
	FunctionX(XLL_DOUBLEX, _T("?xll_version"), _T("XLL.VERSION"))
	.Category(_T("XLL"))
	.FunctionHelp(_T("Returns the current version."))
);
double WINAPI xll_version(void)
{
#pragma XLLEXPORT

	return 1.0;
}

#include "../xll/utility/google.h"

static AddIn xai_google_post(
	Function(XLL_CSTRING, "?xll_google_post", "GOOGLE.POST")
	.Arg(XLL_CSTRING, "Post", "is a string")
	.Category("XLL")
	.FunctionHelp("Post to google analytics")
);
xcstr WINAPI xll_google_post(xcstr type)
{
#pragma XLLEXPORT
	static std::string post;
	try {
		google::analytics ga;

		post = ga.post(type).get();
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());

		return 0;
	}

	return post.c_str();
}