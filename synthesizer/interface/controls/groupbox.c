#include <Windows.h>

#define BORDER_WIDTH	1
HBRUSH groupbox_background = CreateSolidBrush(RGB(45, 45, 45));
HBRUSH groupbox_trim = CreateSolidBrush(RGB(120, 120, 120));

void groupbox_paint(HDC hdc, RECT rect) {
	FillRect(hdc, &rect, groupbox_background);
	InflateRect(&rect, -BORDER_WIDTH, -BORDER_WIDTH);
	FillRect(hdc, &rect, groupbox_trim);
	InflateRect(&rect, BORDER_WIDTH, BORDER_WIDTH);
}