// dialog.cpp
#include <Windows.h>
#include <winuser.h>
#include "../xll/xll.h"

#define ID_HELP   150
#define ID_TEXT   200

BOOL CALLBACK DialogProc(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
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

LPWORD lpwAlign(LPWORD lpIn, ULONG dw2Power = 4)
{
    ULONG ul;

    ul = (ULONG)lpIn;
    ul += dw2Power-1;
    ul &= ~(dw2Power-1);
    return (LPWORD)ul;
}

struct Dialog {
	DLGTEMPLATE _;
	WORD w2[2];
	wchar_t title[sizeof(L"My Dialog")];
	Dialog()  
	{
		ZeroMemory(this, sizeof(*this));
		_.style = WS_POPUP | WS_BORDER | WS_SYSMENU | DS_MODALFRAME | WS_CAPTION;
		_.cdit = 1;
		_.x = 10;
		_.y = 10;
    	_.cx = 100; 
		_.cy = 100;
		w2[0] = 0;
		w2[1] = 0;
		MultiByteToWideChar(CP_ACP, 0, "My Dialog", -1, title, 50);
	}
};
struct D2 {
	Dialog d;
	DLGITEMTEMPLATE it0;
	D2()
	{
		it0.style = WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON;
		it0.id = IDOK;       // OK button identifier
		it0.x  = 10; it0.y  = 70;
		it0.cx = 80; it0.cy = 20;
	}
};

LRESULT DisplayMyMessage(HINSTANCE hinst, HWND hwndOwner, LPSTR lpszMessage)
{
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
    nchar = MultiByteToWideChar(CP_ACP, 0, "My Dialog", -1, lpwsz, 50);
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
    nchar = MultiByteToWideChar(CP_ACP, 0, "OK", -1, lpwsz, 50);
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
    nchar = MultiByteToWideChar(CP_ACP, 0, "Help", -1, lpwsz, 50);
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

    lpwsz = (LPWSTR)lpw;
    nchar = MultiByteToWideChar(CP_ACP, 0, lpszMessage, -1, lpwsz, 150);
    lpw += nchar;
    *lpw++ = 0;             // No creation data

	D2 d2;
	D2 *pd2;
	pd2 = &d2;

    GlobalUnlock(hgbl);
    ret = DialogBoxIndirect(hinst,
        (LPDLGTEMPLATE)hgbl,
        hwndOwner,
        (DLGPROC)DialogProc);
    GlobalFree(hgbl);
    return ret;
}

using namespace xll;

int xll_show_dialog(void)
{
	DisplayMyMessage(xll_Instance, xll_GetHwnd(), "My Message");

	return 1;
}
static Auto<OpenAfterX> xao_show_dialog(xll_show_dialog);