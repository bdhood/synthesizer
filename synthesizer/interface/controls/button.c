#include <Windows.h>
#include <stdlib.h>

#include "button.h"

#pragma warning (disable : 4996)

#define BORDER_WIDTH	1
HBRUSH button_forecolor = CreateSolidBrush(RGB(220, 200, 20));


void button_mouse(HDC hdc, BUTTON *button, BUTTON_STATE state) {

}

void button_paint(HDC hdc, BUTTON *button) {
	char buffer[17];
	const char *text = button->text;
	if (text == 0) {
		itoa(button->value, buffer, 10);
		text = buffer;
	}
	DrawTextA(hdc, text, -1, &button->r, 0);
}
