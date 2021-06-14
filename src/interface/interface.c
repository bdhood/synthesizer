
#include "SDL.h"

void __cdecl interface_start(void)
{
}
/*
#include <Windows.h>
#include <stdio.h>
#include <math.h>
#include <stdbool.h>

#include "keyboard_ctrl.h"
#include "keyboard.h"
#include "waveform.h"
#include "groupbox.h"
#include "label.h"
#include "keymap.h"
#include "activekeys.h"
#include "synth.h"
#include "note.h"
#include "button.h"

#define WIDTH	990
#define HEIGHT  465

#define SetRect(A, B, C, D, E)		A.top = B; A.bottom = C; A.left = D; A.right = E;

const char g_szClassName[] = "synth4";

COLORREF darkGray_color = RGB(30, 30, 30);
COLORREF lightGray_color = RGB(90, 90, 90);
COLORREF active_color = RGB(150, 150, 70);
COLORREF black = RGB(0, 0, 0);
COLORREF interface_text_color = RGB(154, 205, 50);

HBRUSH ky_bg;
HBRUSH backColor;
HBRUSH darkGray;
HBRUSH lightGray;
HBRUSH trimInactive;
HBRUSH active;
HFONT hFont;

HWND hwnd;

HWND interface_get_hwnd() {
	return hwnd;
}


void interface_paint(HWND hWnd, HDC hdc) {
	RECT r;
	GetClientRect(hWnd, &r);
	FillRect(hdc, &r, backColor);
	keyboard_ctrl_paint(hdc);

	SetRect(r, 10, 290, 9, 635)
	waveform_paint(hdc, r);

	SetRect(r, 10, 210, 645, 960);
	groupbox_paint(hdc, r);

	SetRect(r, 20, 220, 655, 855);
	label_paint(hdc, r, "Frames/Buffer: ", 0);
	SetRect(r, 20, 220, 755, 855);
	label_paint(hdc, r, 0, synth_get_framesPreBuffer());

	SetRect(r, 36, 240, 655, 855);
	label_paint(hdc, r, "Sample Rate: ", 0);
	SetRect(r, 36, 240, 755, 855);
	label_paint(hdc, r, 0, (int)synth_get_sampleRate());

	SetRect(r, 52, 240, 655, 855);
	label_paint(hdc, r, "Transpose: ", 0);
	SetRect(r, 52, 240, 755, 855);
	label_paint(hdc, r, 0, keymap_get_transpose());
}

void Paint(HWND hWnd, LPPAINTSTRUCT lpPS) {
	
	RECT rc;
	HDC hdcMem;
	HBITMAP hbmMem, hbmOld;

	GetClientRect(hWnd, &rc);
	hdcMem = CreateCompatibleDC(lpPS->hdc);

	hbmMem = CreateCompatibleBitmap(lpPS->hdc,
		rc.right - rc.left,
		rc.bottom - rc.top);

	hbmOld = (HBITMAP)SelectObject(hdcMem, hbmMem);

	SetBkMode(hdcMem, TRANSPARENT);
	SelectObject(hdcMem, hFont);
	SetTextColor(hdcMem, interface_text_color);

	interface_paint(hWnd, hdcMem);

	BitBlt(lpPS->hdc,
		rc.left, rc.top,
		rc.right - rc.left, rc.bottom - rc.top,
		hdcMem,
		0, 0,
		SRCCOPY);

	//
	// Done with off-screen bitmap and DC.
	//

	SelectObject(hdcMem, hbmOld);
	DeleteObject(hbmMem);
	DeleteDC(hdcMem);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;

	switch (msg)
	{
	case WM_PAINT:
		BeginPaint(hWnd, &ps);
		Paint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		break;
	case WM_CLOSE:
		synth_off();
		keyboard_stop();
		DestroyWindow(hWnd);
		ExitProcess(0);
		break;
	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	return 0;
}

DWORD WINAPI MyThreadFunction(LPVOID lpParam)
{
	while (1) {
		RedrawWindow(interface_get_hwnd(), NULL, NULL, RDW_INVALIDATE);
		Sleep(10);
	}
	return 0;
}

 
void __cdecl interface_start(void)
{
	ky_bg = CreateSolidBrush(RGB(50, 50, 50));
	backColor = CreateSolidBrush(RGB(255, 255, 255));
	darkGray = CreateSolidBrush(darkGray_color);
	lightGray = CreateSolidBrush(lightGray_color);
	trimInactive = CreateSolidBrush(RGB(130, 130, 130));
	active = CreateSolidBrush(active_color);
	WNDCLASSEX wc;
	MSG Msg;

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = 0;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = GetModuleHandle(NULL);
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = g_szClassName;
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	if (!RegisterClassEx(&wc))
	{
		MessageBox(NULL, "Window Registration Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
		return;
	}

	hwnd = CreateWindowEx( WS_EX_CLIENTEDGE, g_szClassName, "Synth4", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, WIDTH, HEIGHT, NULL, NULL, wc.hInstance, NULL);

	if (hwnd == NULL)
	{
		MessageBox(NULL, "Window Creation Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
		return;
	}
	hFont = CreateFont(15, 0, 0, 0, FW_DONTCARE, 0, 0, 0, 0, 0,
		CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, VARIABLE_PITCH, TEXT("verdana"));

	button_init();
	groupbox_init();
	keyboard_ctrl_init();
	label_init();

	ShowWindow(hwnd, SW_SHOW);
	UpdateWindow(hwnd);

	DWORD dwThreadId, dwThrdParam = 1;
	HANDLE hThread;

 

	hThread = CreateThread(NULL, 0, MyThreadFunction, &dwThrdParam, 0, &dwThreadId);
	if (hThread == NULL)
		ExitProcess(1337);

	while (GetMessage(&Msg, hwnd, 0, 0) > 0)
	{
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}

	return;
}
*/