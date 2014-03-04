// simple.cpp - a simple xll
//#define EXCEL12
#include "../xll/xll.h"
#include "../xll/utility/inet.h"
#include "../xll/utility/google.h"

using namespace xll;

int xll_simple(void)
{
	OPERX o;
	o = XLL_XLF(GetWorkspace, OPERX(23)); // startup dir
	o = XLL_XLF(GetWorkspace, OPERX(24)); // alt dir
	o = XLL_XLF(GetWorkspace, OPERX(48)); // library dir
	o = o;

	google::analytics::post("xlAdd");
/*
//	Inet::Open io(_T("Agent"));
//	Inet::Open::URL iou(io, _T("https://google.com/index.html"));
//	buf = iou.Read();
//	std::string buf = Inet::Open(_T("Agent")).OpenUrl(_T("https://google.com/index.html")).Read();
//	size_t n; 
//	n = buf.size();
	Inet::Open io(_T("xll-library"));
	Inet::Open::Connection ioc(io, "accounts.google.com", INTERNET_DEFAULT_HTTPS_PORT);
	Inet::Open::Connection::Request iocr(ioc, "/o/oauth2/auth", "POST", "HTTP/1.1");
	iocr.AddHeader("response_type", "token");
	iocr.AddHeader("client_id", "963445590917.apps.googleusercontent.com");
	iocr.AddHeader("scope", "profile");
	{"web":
	{"auth_uri":"https://accounts.google.com/o/oauth2/auth",
		"client_secret":"VD17RCBepdsnEKQlsKz-ZBjp",
		"token_uri":"https://accounts.google.com/o/oauth2/token",
		"client_email":"963445590917@developer.gserviceaccount.com",
		"client_x509_cert_url":"https://www.googleapis.com/robot/v1/metadata/x509/963445590917@developer.gserviceaccount.com",
		"client_id":"963445590917.apps.googleusercontent.com",
		"auth_provider_x509_cert_url":"https://www.googleapis.com/oauth2/v1/certs"}
	}
*/
	return 1;
}
static Auto<OpenAfterX> xao_simple(xll_simple);
