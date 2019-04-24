#include "note.h"
#include <math.h>

int transpose_note_to_int(NOTE *n) {
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

void transpose_int_to_note(NOTE *n, int value) {
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

NOTE *transpose_add(NOTE *n, int halfSteps) {
	int value = transpose_note_to_int(n);
	value += halfSteps;
	transpose_int_to_note(n, value);
	return n;
}