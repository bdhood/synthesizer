#ifndef BUTTON_H
#define BUTTON_H

#include <Windows.h>

typedef struct BUTTON {
	void *callback;
	const char *text;
	int value;
	RECT r;
} BUTTON;

enum BUTTON_STATE {
	NONE = 0,
	HOVER = 1,
	DOWN = 2
};
void button_init();
void button_mouse(HDC hdc, BUTTON *button, BUTTON_STATE state);
void button_paint(HDC hdc, BUTTON *button);

#endif