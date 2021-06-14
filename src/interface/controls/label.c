#include <Windows.h>
#include <stdlib.h>

#pragma warning (disable : 4996)

#define BORDER_WIDTH	1
HBRUSH label_forecolor;

void label_init() {
	label_forecolor = CreateSolidBrush(RGB(220, 200, 20));
}

void label_paint(HDC hdc, RECT r, const char *sz_value, int value) {
	char buffer[17];
	if (sz_value == 0) {
		itoa(value, buffer, 10);
		sz_value = buffer;
	}
	DrawTextA(hdc, sz_value, -1, &r, 0);
}