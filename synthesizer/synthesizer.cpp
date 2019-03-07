// synth2.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <Windows.h>

#include "interface.h"
#include "keyboard.h"


int main()
{
	CreateThread(NULL, 0, keyboard_start, NULL, 0, NULL);
	CreateThread(NULL, 0, interface_load, NULL, 0, NULL);

	MSG Msg;
	while (GetMessage(&Msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}

	ExitProcess(0);
	return 0;
}