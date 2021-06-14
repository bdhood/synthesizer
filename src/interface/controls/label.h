#ifndef LABEL_H
#define LABEL_H

#include <Windows.h>

void label_init();
void label_paint(HDC hdc, RECT r, const char *sz_value, int value);

#endif