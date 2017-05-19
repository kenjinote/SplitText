#pragma comment(linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#include <windows.h>

TCHAR szClassName[] = TEXT("Window");

LPWSTR translate(LPWSTR string)
{
	LPWSTR here = string;
	size_t len = lstrlenW(string);
	int num;
	while (NULL != (here = wcschr(here, L'\\')))
	{
		switch (here[1])
		{
		case L'\\':
			break;
		case L'r':
			*here = L'\r';
			break;
		case L'n':
			*here = L'\n';
			break;
		case L't':
			*here = L'\t';
			break;
		}
		num = here - string + 1;
		here++;
		memmove(here, here + 1, len - num);
	}
	return string;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static HWND hButton, hEdit1, hEdit2, hEdit3, hEdit4;
	switch (msg)
	{
	case WM_CREATE:
		hEdit1 = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("EDIT"), 0, WS_VISIBLE | WS_CHILD | WS_VSCROLL | WS_HSCROLL | ES_MULTILINE | ES_AUTOVSCROLL | ES_AUTOHSCROLL, 0, 0, 0, 0, hWnd, (HMENU)1001, ((LPCREATESTRUCT)lParam)->hInstance, 0);
		SendMessage(hEdit1, EM_LIMITTEXT, 0, 0);
		hEdit2 = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("EDIT"), TEXT(" \\r\\n;,\\t"), WS_VISIBLE | WS_CHILD | ES_AUTOHSCROLL, 0, 0, 0, 0, hWnd, (HMENU)1002, ((LPCREATESTRUCT)lParam)->hInstance, 0);
		hEdit3 = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("EDIT"), TEXT(","), WS_VISIBLE | WS_CHILD | ES_AUTOHSCROLL, 0, 0, 0, 0, hWnd, (HMENU)1003, ((LPCREATESTRUCT)lParam)->hInstance, 0);
		hButton = CreateWindow(TEXT("BUTTON"), TEXT("変換"), WS_VISIBLE | WS_CHILD, 0, 0, 0, 0, hWnd, (HMENU)1000, ((LPCREATESTRUCT)lParam)->hInstance, 0);
		hEdit4 = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("EDIT"), 0, WS_VISIBLE | WS_CHILD | WS_VSCROLL | WS_HSCROLL | ES_MULTILINE | ES_AUTOVSCROLL | ES_AUTOHSCROLL | ES_READONLY, 0, 0, 0, 0, hWnd, (HMENU)1004, ((LPCREATESTRUCT)lParam)->hInstance, 0);
		SendMessage(hEdit4, EM_LIMITTEXT, 0, 0);
		break;
	case WM_SIZE:
		MoveWindow(hEdit1, 10, 10, LOWORD(lParam) - 20, 256, TRUE);
		MoveWindow(hEdit2, 10, 10 + 256 + 10, LOWORD(lParam) - 20, 32, TRUE);
		MoveWindow(hEdit3, 10, 10 + 256 + 10 + 32 + 10, LOWORD(lParam) - 20, 32, TRUE);
		MoveWindow(hButton, 10, 10 + 256 + 10 + 32 + 10 + 32 + 10, 128, 32, TRUE);
		MoveWindow(hEdit4, 10, 10 + 256 + 10 + 32 + 10 + 32 + 10 + 32 + 10, LOWORD(lParam) - 20, 256, TRUE);
		break;
	case WM_COMMAND:
		if (LOWORD(wParam) == 1000)
		{
			int nSize = GetWindowTextLengthW(hEdit1);
			LPWSTR lpszInputText = (LPWSTR)GlobalAlloc(0, sizeof(WCHAR) * (nSize + 1));
			GetWindowTextW(hEdit1, lpszInputText, nSize + 1);
			nSize = GetWindowTextLength(hEdit2);
			LPWSTR lpszSplitText = (LPWSTR)GlobalAlloc(0, sizeof(WCHAR) * (nSize + 1));
			GetWindowTextW(hEdit2, lpszSplitText, nSize + 1);
			translate(lpszSplitText);
			nSize = GetWindowTextLength(hEdit3);
			LPWSTR lpszDelimiterText = (LPWSTR)GlobalAlloc(0, sizeof(WCHAR) * (nSize + 1));
			GetWindowTextW(hEdit3, lpszDelimiterText, nSize + 1);
			LPWSTR token = wcstok(lpszInputText, lpszSplitText);
			SetWindowTextW(hEdit4, 0);
			while (token != NULL) {
				SendMessageW(hEdit4, EM_REPLACESEL, 0, (LPARAM)token);
				token = wcstok(NULL, lpszSplitText);
				if (token) {
					SendMessageW(hEdit4, EM_REPLACESEL, 0, (LPARAM)lpszDelimiterText);
				}
			}
			GlobalFree(lpszDelimiterText);
			GlobalFree(lpszSplitText);
			GlobalFree(lpszInputText);

			SetFocus(hEdit4);
			SendMessage(hEdit4, EM_SETSEL, 0, -1);
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPreInst, LPSTR pCmdLine, int nCmdShow)
{
	MSG msg;
	WNDCLASS wndclass = {
		CS_HREDRAW | CS_VREDRAW,
		WndProc,
		0,
		0,
		hInstance,
		0,
		LoadCursor(0,IDC_ARROW),
		(HBRUSH)(COLOR_WINDOW + 1),
		0,
		szClassName
	};
	RegisterClass(&wndclass);
	HWND hWnd = CreateWindow(
		szClassName,
		TEXT("入力されたテキストを（指定された文字群で）分割して（指定された区切り文字で）結合する"),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		0,
		CW_USEDEFAULT,
		0,
		0,
		0,
		hInstance,
		0
	);
	ShowWindow(hWnd, SW_SHOWDEFAULT);
	UpdateWindow(hWnd);
	while (GetMessage(&msg, 0, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return (int)msg.wParam;
}
