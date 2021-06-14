#include <thread>

extern "C" {
#include "ui.h"
#include "keyboard.h"
#include "synth.h"
#include "settings.h"
}

auto thread_proc(void* arg) {
	keyboard_start((SETTINGS*)arg);
}


int main(int argc, char* argv[])
{
	SETTINGS* settings = settings_load("settings.json");
	std::thread keyboard_thread(thread_proc, (void*)settings);
	ui_start(settings);
	//while (1) { continue; }
	return 0;
}