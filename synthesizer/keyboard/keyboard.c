#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "note.h"
#include "keymap.h"
#include "freqmap.h"
#include "activekeys.h"
#include "synth.h"
#include "mixer.h"
#include "activenotes.h"
#include "interface.h"

HHOOK hHook;
bool space_down = false;

LRESULT keyboard_callback(int nCode, WPARAM wParam, LPARAM lParam) {

	KBDLLHOOKSTRUCT *kb = (LPKBDLLHOOKSTRUCT )lParam;

	RedrawWindow(interface_get_hwnd(), NULL, NULL, RDW_INVALIDATE);
	

	
	NOTE *n = keymap_get(kb->vkCode);

	if (wParam == WM_KEYDOWN) {
		activekeys_add(kb->vkCode);
		activenotes_add(n);
	}
	if (wParam == WM_KEYUP) {
		activekeys_rm(kb->vkCode);
		activenotes_rm(n);
	}

	return CallNextHookEx(NULL, nCode, wParam, lParam);
}

DWORD WINAPI keyboard_start(LPVOID lpParam) {
	MSG Msg;
	printf("Loading... ");
	keymap_load();
	freqmap_load();
	activekeys_load();
	activenotes_load();
	keymap_set_transpose(-5);
	synth_on();
	
	hHook = SetWindowsHookEx(WH_KEYBOARD_LL, (HOOKPROC)keyboard_callback, GetModuleHandle(NULL), 0);
	if (hHook == NULL) {
		printf("Failed to install keyboard hook, exiting");
		exit(1);
	}

	printf("Ready.\n");

	while (GetMessage(&Msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}
}

void keyboard_stop() {
	UnhookWindowsHookEx(hHook);
}