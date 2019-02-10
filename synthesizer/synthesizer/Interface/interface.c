#include <Windows.h>
#include <stdio.h>
#include <cmath>

#include "keyboard.h"
#include "groupbox.h"
#include "activekeys.h"
#include "synth.h"


#define WIDTH	664
#define HEIGHT  465
const char g_szClassName[] = "synth4";

COLORREF darkGray_color = RGB(69, 69, 69);
COLORREF lightGray_color = RGB(191, 191, 191);
COLORREF active_color = RGB(154, 205, 50);
COLORREF black = RGB(0, 0, 0);

HBRUSH wf_bg = CreateSolidBrush(black);
HBRUSH ky_bg = CreateSolidBrush(RGB(50, 50, 50));
HBRUSH backColor = CreateSolidBrush(RGB(255, 255, 255));
HBRUSH darkGray = CreateSolidBrush(darkGray_color);
HBRUSH lightGray = CreateSolidBrush(lightGray_color);
HBRUSH trimInactive = CreateSolidBrush(RGB(130, 130, 130));
HBRUSH active = CreateSolidBrush(active_color);
HFONT hfnt = 0;

HWND hwnd;

const char *keys[] = { " 23 567 90 =", "QWERTYUIOP[]\\<", " SD GHJ L; ", "ZXCVBNM,./", 0 };
const wchar_t *keys_mapped[] = { 
	L" 23 567 90 \xbb", 
	L"QWERTYUIOP\xdb\xdd\xdc\x08", 
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
	int x = 9, y = 220, offset = 0;
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

void wf_ctrl(HDC hdc) {
	float *buffer = synth_get_buffer();
	int size = 100;
	int x = 9;
	int y = 10;
	int amp;
	int prev_amp = 0;
	RECT r;
	int max = (int)synth_get_framesPreBuffer() * synth_get_channels() / 4;
	r.top =	y;
	r.left = x;
	r.right = r.left + max + 70;
	r.bottom = r.top + size * 2;
	FillRect(hdc, &r, wf_bg);
	for (int i = -35; i < max + 35; i++) {
		if (i < 0) {
			amp = 0;
		}
		else if (i > max) {
			amp = 0;
		}
		else {
			amp = (int)(buffer[i] * 100.0f);
		}
		if (amp > 100)
			amp = 100;
		if (amp < -100) 
			amp = -100;

		if (prev_amp > amp) {
			for (int j = prev_amp; j >= amp; j--) {
				SetPixel(hdc, x, y + size + j, active_color);
			}
		}
		else {
			for (int j = prev_amp; j <= amp; j++) {
				SetPixel(hdc, x, y + size + j, active_color);
			}
		}
		prev_amp = amp;
		x += 1;
	}
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

	RECT r;
	GetClientRect(hWnd, &r);
	FillRect(hdcMem, &r, backColor);
	ky_ctrl(hdcMem);
	wf_ctrl(hdcMem);
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

	ShowWindow(hwnd, SW_SHOW);
	UpdateWindow(hwnd);

	while (GetMessage(&Msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}
	return Msg.wParam;
}