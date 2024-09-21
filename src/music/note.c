#include "note.h"

void note_from_string(NOTE* n, char* str) {
	if (str == 0) {
		printf("NULL note string\n");
		return;
	}

	if (str[0] >= 'a' && str[0] <= 'g') {
		n->note = str[0];
	}
	else if (str[0] >= 'A' && str[0] <= 'G') {
		n->note = str[0] + ('A' - 'a');
	}
	else {
		printf("Invalid note string '%s'\n", str);
		return;
	}

	char octave = str[1];
	if (octave == '#') {
		n->sharp = 1;
		octave = str[2];
	}
	else {
		n->sharp = 0;
	}

	if (octave >= '0' && octave <= '7') {
		n->octave = octave - '0';
	}
	else {
		printf("Invalid octave '%s'\n", str);
		return;
	}
}

int note_to_int(NOTE *n) {
	if (n->sharp >= 2 || n->octave > 8 || n->sharp < 0 || n->octave < 0) {
		printf("invalid note");
		exit(1);
	}
	int value = n->octave * 12;
	switch (n->note) {
	case 'c':
		value += 0;
		break;
	case 'd':
		value += 2;
		break;
	case 'e':
		value += 4;
		break;
	case 'f':
		value += 5;
		break;
	case 'g':
		value += 7;
		break;
	case 'a':
		value += 9;
		break;
	case 'b':
		value += 11;
		break;
	}
	if (n->sharp == 1)
		value++;
	return value;
}

void note_from_int(NOTE *n, int value) {
	n->octave = value / 12;
	n->sharp = 0;
	switch ((int)abs(value) % 12) {
	case 0:
		n->note = 'c';
		break;
	case 1:
		n->note = 'c';
		n->sharp = 1;
		break;
	case 2:
		n->note = 'd';
		break;
	case 3:
		n->note = 'd';
		n->sharp = 1;
		break;
	case 4:
		n->note = 'e';
		break;
	case 5:
		n->note = 'f';
		break;
	case 6:
		n->note = 'f';
		n->sharp = 1;
		break;
	case 7:
		n->note = 'g';
		break;
	case 8:
		n->note = 'g';
		n->sharp = 1;
		break;
	case 9:
		n->note = 'a';
		break;
	case 10:
		n->note = 'a';
		n->sharp = 1;
		break;
	case 11:
		n->note = 'b';
		break;
	}
}
