// auto.cpp - Implement well known Excel interfaces.
// Copyright (c) KALX, LLC. All rights reserved. No warranty made.
#include "xll.h"

using namespace xll;

// Called by Excel when the xll is opened.
extern "C" 
int __declspec(dllexport) WINAPI
xlAutoOpen(void)
{
	try {
		ensure (Auto<Open>::Call());
		ensure (AddIn::RegisterAll());
		ensure (Auto<OpenAfter>::Call());

		if (XLCallVer() >= 0x0C00) {
			ensure (Auto<Open12>::Call());
			ensure (AddIn12::RegisterAll());
			ensure (Auto<OpenAfter12>::Call());
		}

		// register OnXXX macros
		ensure ((On<Data,XLOPER>::Open()));
		ensure ((On<Doubleclick,XLOPER>::Open()));
		ensure ((On<Entry,XLOPER>::Open()));
		ensure ((On<Key,XLOPER>::Open()));
		ensure ((On<Recalc,XLOPER>::Open()));
		ensure ((On<Sheet,XLOPER>::Open()));
		ensure ((On<Time,XLOPER>::Open()));
		ensure ((On<Window,XLOPER>::Open()));
		if (XLCallVer() >= 0x0C00) {
			ensure ((On<Data,XLOPER12>::Open()));
			ensure ((On<Doubleclick,XLOPER12>::Open()));
			ensure ((On<Entry,XLOPER12>::Open()));
			ensure ((On<Key,XLOPER12>::Open()));
			ensure ((On<Recalc,XLOPER12>::Open()));
			ensure ((On<Sheet,XLOPER12>::Open()));
			ensure ((On<Time,XLOPER12>::Open()));
			ensure ((On<Window,XLOPER12>::Open()));
		}
	}
	catch (const std::exception& ex) {
		MessageBoxA(GetActiveWindow(), ex.what(), "AutoOpen", MB_OK);

		return 0;
	}
	catch (...) {
		MessageBoxA(GetActiveWindow(), "Unknown exception", "AutoOpen", MB_OK);

		return 0;
	}

	return 1;
}

extern "C"
int __declspec(dllexport) WINAPI
xlAutoClose(void)
{
	try {
		ensure (Auto<Close>::Call());
		if (XLCallVer() >= 0x0C00) {
			ensure (Auto<Close12>::Call());
		}
	}
	catch (const std::exception& ex) {
		MessageBoxA(GetActiveWindow(), ex.what(), "AutoClose", MB_OK);

		return 0;
	}
	catch (...) {
		MessageBoxA(GetActiveWindow(), "Unknown exception", "AutoClose", MB_OK);

		return 0;
	}

	return 1;
}

extern "C"
int __declspec(dllexport) WINAPI
xlAutoAdd(void)
{
	try {
		ensure (Auto<Add>::Call());
		if (XLCallVer() >= 0x0C00) {
			ensure (Auto<Add12>::Call());
		}
	}
	catch (const std::exception& ex) {
		MessageBoxA(GetActiveWindow(), ex.what(), "AutoAdd", MB_OK);

		return 0;
	}
	catch (...) {
		MessageBoxA(GetActiveWindow(), "Unknown exception", "AutoAdd", MB_OK);

		return 0;
	}

	return 1;
}

extern "C"
int __declspec(dllexport) WINAPI
xlAutoRemove(void)
{
	try {
		ensure (Auto<RemoveBefore>::Call());
//		ensure (AddIn::UnregisterAll());
		ensure (Auto<Remove>::Call());

		if (XLCallVer() >= 0x0C00) {
			ensure (Auto<RemoveBefore12>::Call());
//			ensure (AddIn12::UnregisterAll());
			ensure (Auto<Remove12>::Call());
		}

		// unregister OnXXX macros
		ensure (On<Data>::Close());
		ensure (On<Doubleclick>::Close());
		ensure (On<Entry>::Close());
		ensure (On<Key>::Close());
		ensure (On<Recalc>::Close());
		ensure (On<Sheet>::Close());
		ensure (On<Time>::Close());
		ensure (On<Window>::Close());
	}
	catch (const std::exception& ex) {
		MessageBoxA(GetActiveWindow(), ex.what(), "AutoRemove", MB_OK);

		return 0;
	}
	catch (...) {
		MessageBoxA(GetActiveWindow(), "Unknown exception", "AutoRemove", MB_OK);

		return 0;
	}

	return 1;
}

extern "C"
void __declspec(dllexport) WINAPI
xlAutoFree(LPXLOPER px)
{
	if ((px->xltype)&xlbitDLLFree)
		delete px;
	else if ((px->xltype)&xlbitXLFree)
		Excel4(xlFree, 0, 1, px);
}

extern "C"
void __declspec(dllexport) WINAPI
xlAutoFree12(LPXLOPER12 px)
{
	if ((px->xltype)&xlbitDLLFree)
		delete px;
	else if ((px->xltype)&xlbitXLFree)
		Excel12(xlFree, 0, 1, px);
}

extern "C"
LPXLOPER __declspec(dllexport) WINAPI 
xlAutoRegister(LPXLOPER pxName)
{
	static XLOPER xResult;

	xResult = *pxName;
	/*
	try {
		const Args* parg = ArgsMap::Find(*pxName);

		if (parg) {
			xResult = xll::Register(*parg);
		}
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());

		xResult = Err(xlerrValue);
	}
	*/
	return &xResult;
}

extern "C"
LPXLOPER12 __declspec(dllexport) WINAPI 
xlAutoRegister12(LPXLOPER12 pxName)
{
	static XLOPER12 xResult;

	xResult = *pxName;
	/*
	try {
		const Args12* parg = ArgsMap12::Find(*pxName);

		if (parg) {
			xResult = xll::Register(*parg);
		}
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());

		xResult = Err12(xlerrValue);
	}
	*/
	return &xResult;
}
