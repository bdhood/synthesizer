#include <string.h>
#include <stdio.h>
#include <stdbool.h>

#include "note.h"

#define SIZE 0x200

char active_keys[SIZE];

void activekeys_load() {
	memset(active_keys, 0, SIZE);
}

void activekeys_add(int vkcode) {
	active_keys[vkcode] = 1;
}

void activekeys_rm(int vkcode) {
	active_keys[vkcode] = 0;
}

int activekeys_next(int vkcode) {
	for (int i = vkcode + 1; i < sizeof(active_keys); i++) {
		if (active_keys[i] == 1) {
			return i;
		}
	}
	return 0;
}

int activekeys_first() {
	return activekeys_next(0);
}

int activekeys_get_total() {
	int total = 0;
	for (int i = 0; i < SIZE; i++) {
		if (active_keys[i] == 1) {
			total++;
		}
	}
	return total;
}

bool activekeys_isactive(int i) {
	return active_keys[i] == 1;
}
