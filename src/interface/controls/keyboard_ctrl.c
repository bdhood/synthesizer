#include <Windows.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#include "activekeys.h"

COLORREF keyboard_ctrl_color_BACKCOLOR = RGB(50, 50, 50);
COLORREF keyboard_ctrl_color_UNUSED_TRIM = RGB(130, 130, 130);
COLORREF keyboard_ctrl_color_UNUSED_TOP = RGB(125, 125, 125);
COLORREF keyboard_ctrl_color_UNUSED_BOTTOM = RGB(125, 125, 125);

COLORREF keyboard_ctrl_color_ACTIVE_TRIM = RGB(164, 215, 65);
COLORREF keyboard_ctrl_color_ACTIVE_TOP = RGB(154, 206, 50);
COLORREF keyboard_ctrl_color_ACTIVE_BOTTOM = RGB(140, 195, 38);

COLORREF keyboard_ctrl_color_INACTIVE_TRIM = RGB(164, 215, 65);
COLORREF keyboard_ctrl_color_INACTIVE_TOP = RGB(154, 206, 50);
COLORREF keyboard_ctrl_color_INACTIVE_BOTTOM = RGB(140, 195, 38);

HBRUSH keyboard_ctrl_brush_BACKCOLOR;
HBRUSH keyboard_ctrl_brush_UNUSED_TRIM;
HBRUSH keyboard_ctrl_brush_UNUSED_TOP;
HBRUSH keyboard_ctrl_brush_UNUSED_BOTTOM;

HBRUSH keyboard_ctrl_brush_ACTIVE_TRIM;
HBRUSH keyboard_ctrl_brush_ACTIVE_TOP;
HBRUSH keyboard_ctrl_brush_ACTIVE_BOTTOM;

HBRUSH keyboard_ctrl_brush_INACTIVE_TRIM;
HBRUSH keyboard_ctrl_brush_INACTIVE_TOP;
HBRUSH keyboard_ctrl_brush_INACTIVE_BOTTOM;

const char *keyboard_ctrl_keys[] = { " 23 567 90 =\x08", "QWERTYUIOP[]\\", " SD GHJ L; ", "\xa0ZXCVBNM,./", 0 };
const wchar_t *keyboard_ctrl_keys_mapped[] = { 
	L" 23 567 90 \xbb\x08", 
	L"QWERTYUIOP\xdb\xdd\xdc", 
	L" SD GHJ L\xba ", 
	L"\xa0ZXCVBNM\xbc\xbe\xbf", 
	0 
};

void keyboard_ctrl_grow(RECT *rect, int size) {
    rect->left -= size;
    rect->top -= size;
    rect->right += size;
    rect->bottom += size;
}

void keyboard_ctrl_init() {
	keyboard_ctrl_brush_BACKCOLOR = CreateSolidBrush(RGB(50, 50, 50));
	keyboard_ctrl_brush_UNUSED_TRIM = CreateSolidBrush(RGB(130, 130, 130));
	keyboard_ctrl_brush_UNUSED_TOP = CreateSolidBrush(RGB(125, 125, 125));
	keyboard_ctrl_brush_UNUSED_BOTTOM = CreateSolidBrush(RGB(125, 125, 125));

	keyboard_ctrl_brush_ACTIVE_TRIM = CreateSolidBrush(RGB(200, 200, 200));
	keyboard_ctrl_brush_ACTIVE_TOP = CreateSolidBrush(RGB(74, 106, 25));
	keyboard_ctrl_brush_ACTIVE_BOTTOM = CreateSolidBrush(RGB(65, 85, 15));

	keyboard_ctrl_brush_INACTIVE_TRIM = CreateSolidBrush(RGB(100, 100, 100));
	keyboard_ctrl_brush_INACTIVE_TOP = CreateSolidBrush(RGB(30, 30, 30));
	keyboard_ctrl_brush_INACTIVE_BOTTOM = CreateSolidBrush(RGB(15, 15, 15));
}

void keyboard_ctrl_paint(HDC hdc) {

	RECT r;
	int keySizeX = 40;
	int keySizeY = 40;
	int yPadding = 2;
	int xPadding = 2;
	int x = 10, y = 300, offset = 0;
	char text[5];
	r.left = x;
	r.top = y;
	r.right = r.left + 626;
	r.bottom = r.top + 187;
	FillRect(hdc, &r, keyboard_ctrl_brush_BACKCOLOR);
	x += 30;
	y += 10;
	for (int i = 0; keyboard_ctrl_keys[i] != 0; i++) {
		for (int j = 0; keyboard_ctrl_keys[i][j] != 0; j++) {
			r.left = x;
			r.top = y;
			r.right = x + keySizeX;
			r.bottom = y + keySizeY;

			if (i == 0 && keyboard_ctrl_keys[i][j] == 0x08) {
				r.right += keySizeX;
			}
			if (i == 1 && keyboard_ctrl_keys[i][j] == '\\') {
				r.right += keySizeX / 2;
			}
			if (i == 2 && keyboard_ctrl_keys[i][j] ==';') {
				offset -= 2 * keySizeX;
			}
			
			if (i == 3 && keyboard_ctrl_keys[i][j] == '\xa0') {
				r.right += keySizeX;
				x += keySizeX;
			}
			FillRect(hdc, &r, keyboard_ctrl_brush_INACTIVE_TRIM);

			if (keyboard_ctrl_keys[i][j] == ' ') {
                // unused key

                FillRect(hdc, &r, keyboard_ctrl_brush_UNUSED_TRIM);
                keyboard_ctrl_grow(&r, -1);
                FillRect(hdc, &r, keyboard_ctrl_brush_UNUSED_TOP);
                r.top += keySizeY / 2;
                FillRect(hdc, &r, keyboard_ctrl_brush_UNUSED_BOTTOM);

            } else if (activekeys_isactive((int)keyboard_ctrl_keys_mapped[i][j])) {
                // active key

                FillRect(hdc, &r, keyboard_ctrl_brush_ACTIVE_TRIM);
                keyboard_ctrl_grow(&r, -1);
                FillRect(hdc, &r, keyboard_ctrl_brush_ACTIVE_TOP);
                r.top += keySizeY / 2;
                FillRect(hdc, &r, keyboard_ctrl_brush_ACTIVE_BOTTOM);

            } else {
                // inactive key

                FillRect(hdc, &r, keyboard_ctrl_brush_INACTIVE_TRIM);
                keyboard_ctrl_grow(&r, -1);
                FillRect(hdc, &r, keyboard_ctrl_brush_INACTIVE_TOP);
                r.top += keySizeY / 2;
                FillRect(hdc, &r, keyboard_ctrl_brush_INACTIVE_BOTTOM);
            }

			r.top -= keySizeY / 2;
			if (keyboard_ctrl_keys[i][j] == '\xa0') {
				text[0] = 'S';
				text[1] = 'H';
				text[2] = 'F';
				text[3] = 0;
			} else if (keyboard_ctrl_keys[i][j] == 0x08) {
				text[0] = 'B';
				text[1] = 'C';
				text[2] = 'K';
				text[3] = 0;
			} else {			
				text[0] = keyboard_ctrl_keys[i][j];
				text[1] = 0;
			}

			DrawText(hdc, text, (int)strlen(text), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

			x += keySizeX + xPadding;
		}
		y += keySizeY + yPadding;
		offset += keySizeX / 2;
		x = offset + 40;
	}
}