#include <stdexcept>
#undef UNICODE
#undef _UNICODE
#define _MBCS
#include <Windows.h>
#include "XLCALL.H"

typedef struct _EnumStruct
{
  HWND hwnd;  // Return value for Excel main hWnd.
  unsigned short wLoword; //Contains LowWord of the Excel main hWnd
} EnumStruct;

#define CLASS_NAME_BUFFER  50

BOOL CALLBACK EnumProc(HWND hwnd, EnumStruct * pEnum)
{
  // First check the class of the window. Must be "XLMAIN".
  char rgsz[CLASS_NAME_BUFFER];

  GetClassName(hwnd, rgsz, CLASS_NAME_BUFFER);

  if (!lstrcmpi(rgsz, "XLMAIN"))
  {
    // If that hits, check the loword of the window handle.
    if (LOWORD((DWORD) hwnd) == pEnum->wLoword)
    {
      // We have a match, return Excel main hWnd.
      pEnum->hwnd = hwnd;
      return FALSE;
    }
  }

  // No match - continue the enumeration.
  return TRUE;
}

BOOL GetHwnd(HWND * pHwnd)
{
  XLOPER x;

  //
  // xlGetHwnd only returns the LoWord of Excel hWnd
  // so all the windows have to be enumerated to see
  // which match the LoWord retuned by xlGetHwnd.
  //
  if (Excel4(xlGetHwnd, &x, 0) == xlretSuccess)
  {
    EnumStruct enm;

    enm.hwnd = NULL;
    enm.wLoword = x.val.w;

    EnumWindows((WNDENUMPROC) EnumProc, (LPARAM) &enm);

    if (enm.hwnd != NULL)
    {
      *pHwnd = enm.hwnd;
      return TRUE;
    }
  }
  return FALSE;
}

HWND GetHwnd(void)
{
	HWND h(0);

	if (XLCallVer() >= 0x0C00) {
		XLOPER12 x;
		if (xlretSuccess == Excel12(xlGetHwnd, &x, 0) && x.xltype == xltypeInt) {
			h = (HWND)(x.val.w);
		}
		else {
			throw std::runtime_error("GetHwnd failed");
		}
	}
	else {
		if (!GetHwnd(&h)) {
			throw std::runtime_error("GetHwnd failed");
		}
	}

	return h;
}