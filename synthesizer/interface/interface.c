#include <Windows.h>
#include <stdio.h>
#include <math.h>

#include "keyboard.h"
#include "waveform.h"
#include "groupbox.h"
#include "label.h"
#include "keymap.h"
#include "activekeys.h"
#include "synth.h"
#include "note.h"

#define WIDTH	990
#define HEIGHT  465

#define SetRect(A, B, C, D, E)		A.top = B; A.bottom = C; A.left = D; A.right = E;

const char g_szClassName[] = "synth4";

COLORREF darkGray_color = RGB(30, 30, 30);
COLORREF lightGray_color = RGB(90, 90, 90);
COLORREF active_color = RGB(150, 150, 70);
COLORREF black = RGB(0, 0, 0);
COLORREF interface_text_color = RGB(154, 205, 50);

HBRUSH ky_bg = CreateSolidBrush(RGB(50, 50, 50));
HBRUSH backColor = CreateSolidBrush(RGB(255, 255, 255));
HBRUSH darkGray = CreateSolidBrush(darkGray_color);
HBRUSH lightGray = CreateSolidBrush(lightGray_color);
HBRUSH trimInactive = CreateSolidBrush(RGB(130, 130, 130));
HBRUSH active = CreateSolidBrush(active_color);
HFONT hFont;

HWND hwnd;

const char *keys[] = { " 23 567 90 = <", "QWERTYUIOP[]\\", " SD GHJ L; ", "ZXCVBNM,./", 0 };
const wchar_t *keys_mapped[] = { 
	L" 23 567 90 \xbb \x08", 
	L"QWERTYUIOP\xdb\xdd\xdc", 
	L" SD GHJ L\xba ", 
	L"ZXCVBNM\xbc\xbe\xbf", 
	0 
};
HWND interface_get_hwnd() {
	return hwnd;
}

void ky_ctrl(HDC hdc) {

	RECT r;
	int keySizeX = 40;
	int keySizeY = 40;
	int yPadding = 2;
	int xPadding = 2;
	int x = 9, y = 300, offset = 0;
	char text[2];
	r.left = x;
	r.top = y;
	r.right = r.left + 626;
	r.bottom = r.top + 187;
	FillRect(hdc, &r, ky_bg);
	x += 10;
	y += 10;
	text[1] = 0;
	for (int i = 0; keys[i] != 0; i++) {
		for (int j = 0; keys[i][j] != 0; j++) {
			r.left = x;
			r.top = y;
			r.right = x + keySizeX;
			r.bottom = y + keySizeY;
			FillRect(hdc, &r, trimInactive);

			if (keys[i][j] != ' ') {
				r.left++;
				r.top++;
				r.bottom--;
				r.right--;
				if (i % 2 == 0) {
					SetBkColor(hdc, darkGray_color);
					FillRect(hdc, &r, darkGray);
				}
				else {
					SetBkColor(hdc, lightGray_color);
					FillRect(hdc, &r, lightGray);
				}


				for (unsigned char c = activekeys_first(); c != 0; c = activekeys_next(c)) {
					if ((unsigned char)keys_mapped[i][j] == c) {
						SetBkColor(hdc, active_color);
						FillRect(hdc, &r, active);
						break;
					}
				}


				text[0] = keys[i][j];

				DrawText(hdc, text, 1, &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			}

			x += keySizeX + xPadding;
		}
		y += keySizeY + yPadding;
		offset += keySizeX / 2;
		x = offset + 19;
	}
}

void interface_paint(HWND hWnd, HDC hdc) {
	RECT r;
	GetClientRect(hWnd, &r);
	FillRect(hdc, &r, backColor);
	ky_ctrl(hdc);

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

	//
	// Get the size of the client rectangle.
	//

	GetClientRect(hWnd, &rc);

	//
	// Create a compatible DC.
	//

	hdcMem = CreateCompatibleDC(lpPS->hdc);

	//
	// Create a bitmap big enough for our client rectangle.
	//

	hbmMem = CreateCompatibleBitmap(lpPS->hdc,
		rc.right - rc.left,
		rc.bottom - rc.top);

	//
	// Select the bitmap into the off-screen DC.
	//

	hbmOld = (HBITMAP)SelectObject(hdcMem, hbmMem);

	//
	// Render the image into the offscreen DC.
	//

	SetBkMode(hdcMem, TRANSPARENT);
	SelectObject(hdcMem, hFont);
	SetTextColor(hdcMem, interface_text_color);
	interface_paint(hWnd, hdcMem);

	//
	// Blt the changes to the screen DC.
	//

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

DWORD WINAPI interface_load(LPVOID lpParam)
{
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
		return 0;
	}

	hwnd = CreateWindowEx( WS_EX_CLIENTEDGE, g_szClassName, "Synth4", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, WIDTH, HEIGHT, NULL, NULL, wc.hInstance, NULL);

	if (hwnd == NULL)
	{
		MessageBox(NULL, "Window Creation Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}
	hFont = CreateFont(15, 0, 0, 0, FW_DONTCARE, 0, 0, 0, 0, 0,
		CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, VARIABLE_PITCH, TEXT("verdana"));

	ShowWindow(hwnd, SW_SHOW);
	UpdateWindow(hwnd);

	while (GetMessage(&Msg, hwnd, 0, 0) > 0)
	{
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}
	return Msg.wParam;
}