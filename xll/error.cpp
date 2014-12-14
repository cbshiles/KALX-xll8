#pragma warning(disable: 4996)
#include "xll.h"
#include "utility/log.h"

// All addins coordinate through the registry.
Reg::Object<char, DWORD> xll_alert_level(
	HKEY_CURRENT_USER, "Software\\KALX\\xll", "xll_alert_level",
	XLL_ALERT_ERROR|XLL_ALERT_WARNING|XLL_ALERT_INFO
);

Log::EventSource xll_log(_T("xll"));

extern "C" int 
XLL_ALERT(const char* text, const char* caption, WORD level, UINT type, bool force)
{
	if ((xll_alert_level&level) || force) {
		if (xll_alert_level&XLL_ALERT_LOG)
			xll_log.ReportEvent(level, 0, text);
		
		if (IDCANCEL == MessageBoxA(GetForegroundWindow(), text, caption, MB_OKCANCEL|type))
			xll_alert_level &= ~level;
	}

	return xll_alert_level;
}

int 
XLL_ERROR(const char* e, bool force)
{
	return XLL_ALERT(e, "Error", XLL_ALERT_ERROR, MB_ICONERROR, force);
}
int 
XLL_WARNING(const char* e, bool force)
{
	return XLL_ALERT(e, "Warning", XLL_ALERT_WARNING, MB_ICONWARNING, force);
}
int 
XLL_INFO(const char* e, bool force)
{
	return XLL_ALERT(e, "Information", XLL_ALERT_INFO, MB_ICONINFORMATION, force);
}

using namespace xll;

static AddIn xai_alert_level(
	Function(XLL_LPOPER, XLL_DECORATE("xll_alert_level_",4), "XLL.ALERT.LEVEL")
	.Arg(XLL_LPOPER, "Level", "is a bit mask of alert levels to set.")
	.Category("XLL")
	.FunctionHelp(
		"Return old alert level and set to new level if Level is specified. "
		"The levels are Error (0x1), Warning (0x2), Information (0x4), and Log (0x8)"
	)
);
extern "C" LPOPER __declspec(dllexport) WINAPI 
xll_alert_level_(LPOPER po)
{
	static OPER o;

	try {
		if (po->xltype != xltypeMissing) {
			ensure (po->xltype == xltypeNum);
			DWORD xal = static_cast<DWORD>(po->val.num);
			ensure (xal <= 0xF);
			xll_alert_level = xal;
		}

		o = static_cast<double>(xll_alert_level);
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());

		o = OPER(xlerr::NA);
	}

	return &o;
}

static AddIn xai_alert_error_toggle(Macro(XLL_DECORATE("xll_alert_error_toggle",0), "XLL.ALERT.ERROR.TOGGLE"));
extern "C" int __declspec(dllexport) WINAPI xll_alert_error_toggle(void)
{
	xll_alert_level ^= XLL_ALERT_ERROR;

	return 1;
}
static AddIn xai_alert_warning_toggle(Macro(XLL_DECORATE("xll_alert_warning_toggle",0), "XLL.ALERT.WARNING.TOGGLE"));
extern "C" int __declspec(dllexport) WINAPI xll_alert_warning_toggle(void)
{
	xll_alert_level ^= XLL_ALERT_WARNING;

	return 1;
}
static AddIn xai_alert_info_toggle(Macro(XLL_DECORATE("xll_alert_info_toggle",0), "XLL.ALERT.INFO.TOGGLE"));
extern "C" int __declspec(dllexport) WINAPI xll_alert_info_toggle(void)
{
	xll_alert_level ^= XLL_ALERT_INFO;

	return 1;
}

static AddIn xai_alert_error(
	Function(XLL_WORD, XLL_DECORATE("xll_alert_error",0), "XLL_ALERT_ERROR")
	.Category("XLL")
	.FunctionHelp("Return enumeration for the error alert level.")
);
extern "C" int __declspec(dllexport) WINAPI xll_alert_error(void)
{
	return XLL_ALERT_ERROR;
}
static AddIn xai_alert_warning(
	Function(XLL_WORD, XLL_DECORATE("xll_alert_warning",0), "XLL_ALERT_WARNING")
	.Category("XLL")
	.FunctionHelp("Return enumeration for the warning alert level.")
);
extern "C" int __declspec(dllexport) WINAPI xll_alert_warning(void)
{
	return XLL_ALERT_WARNING;
}
static AddIn xai_alert_info(
	Function(XLL_WORD, XLL_DECORATE("xll_alert_info",0), "XLL_ALERT_INFO")
	.Category("XLL")
	.FunctionHelp("Return enumeration for the information alert level.")
);
extern "C" int __declspec(dllexport) WINAPI xll_alert_info(void)
{
	return XLL_ALERT_INFO;
}
static AddIn xai_alert_log(
	Function(XLL_WORD, XLL_DECORATE("xll_alert_log",0), "XLL_ALERT_LOG")
	.Category("XLL")
	.FunctionHelp("Return enumeration for logging alerts.")
);
extern "C" int __declspec(dllexport) WINAPI xll_alert_log(void)
{
	return XLL_ALERT_LOG;
}

/*
static AddIn xai_alert_filter(XLL_DECORATE("xll_alert_filter", 0), "ALERT.FILTER");
extern "C" int __declspec(dllexport) WINAPI
xll_alert_filter(void)
{
	try {
		int w = 230, h = 90, b = 10;
		Dialog d("Alert Filter", 10, 10, w, h);

		d.Add(Dialog::GroupBox, "Show alerts:", b, b, w/2 - 2*b, h - 2*b);

		WORD error = d.Add(Dialog::CheckBox, "Error");
   		d.Set(error, Num(xll_alert_level&XLL_ALERT_ERROR));

		WORD warning = d.Add(Dialog::CheckBox, "Warning");
   		d.Set(warning, Num(xll_alert_level&XLL_ALERT_WARNING));

		WORD info = d.Add(Dialog::CheckBox, "Info");
   		d.Set(info, Num(xll_alert_level&XLL_ALERT_INFO));

		WORD ok = d.Add(Dialog::OKButton, "OK", w/2 + 2*b, 2*b);
		d.Add(Dialog::CancelButton, "Cancel", w/2 + 2*b, h/2 + b);

		if (ok == d.Show()) {
			if (d.Get(error))
				xll_alert_level |= XLL_ALERT_ERROR;
			else
				xll_alert_level &= ~XLL_ALERT_ERROR;

			if (d.Get(warning))
				xll_alert_level |= XLL_ALERT_WARNING;
			else
				xll_alert_level &= ~XLL_ALERT_WARNING;

			if (d.Get(info))
				xll_alert_level |= XLL_ALERT_INFO;
			else
				xll_alert_level &= ~XLL_ALERT_INFO;
		}
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());

		return 0;
	}
	catch (...) {
		XLL_ERROR("ALERT.FILTER: unknown exception");

		return 0;
	}

	return 1;
}
*/
