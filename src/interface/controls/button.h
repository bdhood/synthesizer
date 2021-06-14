#ifndef BUTTON_H
#define BUTTON_H

#include <Windows.h>

typedef struct BUTTON {
	void *callback;
	const char *text;
	int value;
	RECT r;
} BUTTON;

void button_init();
void button_mouse(HDC hdc, BUTTON *button, int state);
void button_paint(HDC hdc, BUTTON *button);

#endif