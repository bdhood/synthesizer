#include <string.h>
#include <stdio.h>

char active[0x100];

void activekeys_load() {
	memset(active, 0, 0x100);
}

void activekeys_add(unsigned char vkcode) {
	active[vkcode & 0xff] = 1;
}

void activekeys_rm(unsigned char vkcode) {
	active[vkcode & 0xff] = 0;
}

unsigned char activekeys_next(unsigned char vkcode) {
	for (int i = vkcode + 1; i < sizeof(active); i++) {
		if (active[i] == 1) {
			return i;
		}
	}
	return 0;
}

unsigned char activekeys_first() {
	return activekeys_next(0);
}

