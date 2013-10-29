// dllmain.cpp
// Copyright (c) KALX, LLC. All rights reserved. No warranty is made.
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

HMODULE xll_Instance;

#pragma warning(disable: 4100)
extern "C"
BOOL WINAPI
DllMain(HMODULE hDLL, ULONG reason, LPVOID lpReserved)
{
	switch (reason) {
		case DLL_PROCESS_ATTACH: 
			xll_Instance = hDLL;
			DisableThreadLibraryCalls(hDLL);		
			break;		
		case DLL_THREAD_ATTACH:
			break;
		case DLL_THREAD_DETACH:
			break;
		case DLL_PROCESS_DETACH:
			break;
	}

	return TRUE;
}
