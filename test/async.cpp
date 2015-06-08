// async.cpp - Asynchronous function example.
// Copyright (c) 2011 KALX, LLC. All rights reserved. No warranty is made.
// http://msdn.microsoft.com/en-us/library/ff796219.aspx
// Put =XLL.ECHO(RAND()) in one cell and =RAND() in another.
// Hit F9 and note both cells update after 1 second.
// Put =XLL.ECHOA(RAND()) in one cell and =RAND() in another.
// Hit F9 and note =RAND() updates immediately.
#define EXCEL12
#include "../xll8/xll/xll.h"

using namespace xll;

// slow synchronous function
static AddIn12 xai_echo(
	Function12(XLL_LPOPER12, L"?xll_echo", L"XLL.ECHO")
	.Arg(XLL_LPOPER12, L"Arg", L"is an argument.")
	.Category(L"XLL")
	.FunctionHelp(L"Sleep one second and return twice Arg.")
);
LPOPER12 WINAPI
xll_echo(LPOPER12 po)
{
#pragma XLLEXPORT
	Sleep(1000);

	if (po->xltype & xltypeNum)
		po->val.num *= 2;

	return po;
}

// using asynchronous facilities

#include <process.h>

DWORD WINAPI xll_echow(LPVOID);

// start a thread from Excel
static AddIn12 xai_echoa(
	Function12(XLL_VOID12, L"?xll_echoa", L"XLL.ECHOA")
	.Num(L"Arg1", L"is an argument.")
	.Num(L"Arg2", L"is an argument.")
	.ThreadSafe()
	.Asynchronous()
	.Category(L"XLL")
	.FunctionHelp(L"Return immediately, then sleep one second and return twice Arg.")
);
void WINAPI xll_echoa(double arg1, double arg2, LPXLOPER12 ph)
{
#pragma XLLEXPORT
	try {
		OPER12& dh = *new OPER12(3, 1);

		dh[0] = *ph;
		ensure (ph->xltype == xltypeBigData);
		dh[1] = arg1;
		dh[2] = arg2;

		if (!CreateThread(0, 0, xll_echow, &dh, 0, 0)) { // use _begin/_endthread???
	//	if (-1L == _beginthread(xll_echow, 0, &dh)) { 
			// xll_echow not called
			delete &dh;
		}
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}
}

// do the work
DWORD WINAPI 
xll_echow(LPVOID arg)
{
	OPER12& dh = *(LPOPER12)arg;

	try {
		Sleep(1000);

		
			dh[0].val.num *= 2;

		// return result to xll_echoa
		int ret = traits<XLOPER12>::Excel(xlAsyncReturn, 0, 2, &dh[1], &dh[0]);
		ensure (ret == xlretSuccess);
//		Excel<XLOPER12>(xlAsyncReturn, dh[1], dh[0]); // note handle, then data

		delete &dh;
	//	_endthread();
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());

		return 1;
	}

	return 0;
}

#include <thread>
#include "../xll/utility/socket.h"
typedef xll::traits<XLOPER12>::xstr xstr;

struct sh {
	std::wstring str;
	OPER12 handle;
};

void call_server(const sh& sh)
{
	wsa::socket s{};
	wsa::connect(s, "localhost", "3456");
	wsa::send(s, "hi");
	std::string r = wsa::recv(s);
	Excel<XLOPER12>(xlAsyncReturn, sh.handle, OPER12(L"foo")); // note handle, then data
}

static AddIn12 xai_volatile_async(
	Function12(XLL_VOID12, L"?xll_volatile_async", L"V.ASYNC")
	.Arg(XLL_CSTRING12, L"A string", L"to be sent")
	.ThreadSafe()
	.Asynchronous()
	.Volatile()
	.FunctionHelp(L"Return a volatile async thingy.")
	.Category(L"XLL")
);
void WINAPI xll_volatile_async(xstr s, LPXLOPER12 ph)
{
#pragma XLLEXPORT
	sh sh{ s,*ph };

	wsa::socket s_{};
	wsa::connect(s_, "localhost", "3456");
	wsa::send(s_, "hi");
	std::string r = wsa::recv(s_);
	ensure(r == "hi");

	std::thread t(call_server, sh);

	return;
}