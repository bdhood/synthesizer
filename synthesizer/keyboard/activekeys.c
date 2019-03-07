#include <string.h>
#include <stdio.h>

#define SIZE 0x100

char active_keys[SIZE];

void activekeys_load() {
	memset(active_keys, 0, SIZE);
}

void activekeys_add(unsigned char vkcode) {
	active_keys[vkcode & 0xff] = 1;
}

void activekeys_rm(unsigned char vkcode) {
	active_keys[vkcode & 0xff] = 0;
}

unsigned char activekeys_next(unsigned char vkcode) {
	for (int i = vkcode + 1; i < sizeof(active_keys); i++) {
		if (active_keys[i] == 1) {
			return i;
		}
	}
	return 0;
}

unsigned char activekeys_first() {
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
