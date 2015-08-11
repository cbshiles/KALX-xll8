// simple.cpp - a simple xll
#define EXCEL12
#include "../xll/udp.h"
#include "../xll/xll.h"

using namespace xll;

static WSA::Startup start{};
#if 0
int xll_xll(void)
{
	try {
		WSA::Socket s(AF_INET, SOCK_DGRAM, 0);
		char buf[255];
		int i = gethostname(buf, 255);
		hostent* phe = gethostbyname(buf);
		for (const auto& p = 
		char* hostip = inet_ntoa(*(struct in_addr*)phe->h_addr_list[0]);

//		strcpy(buf, "ping");
//		i = udp::sendto(s, buf, 4);
//		i = udp::recvfrom(s, buf, 255);

		OPERX ret;
		while (ret.xltype != xltypeStr)
			ret = Excel<XLOPERX>(xlfInput, OPERX(_T("Enter your e-mail address to get your token")), OPERX(2), OPERX(_T("e-mail")));

		std::string mailto = "mailto: ";
		mailto.append(traits<XLOPERX>::string(ret));
		udp::sendto(s, mailto.c_str(), static_cast<int>(mailto.length()));
		char ntoa[256];
		int n = udp::recvfrom(s, ntoa, 256);
		ensure (n > 0);
		ntoa[n] = 0;

		ret = Excel<XLOPERX>(xlfInput, OPERX(_T("Enter your token")), OPERX(1), OPERX(_T("token")));

	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());

		return FALSE;
	}

	return TRUE;
}
//static Auto<Open12> xao_xll(xll_xll);
#endif

AddIn12 xai_foo(
	L"?xll_foo", XLL_DOUBLE12 XLL_DOUBLE12,
	L"FOO", L"x"
);
double WINAPI xll_foo(double x)
{
#pragma XLLEXPORT
//	UDP_SEND

	return 2*x;
}
