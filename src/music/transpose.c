#include "note.h"

NOTE *transpose_add(NOTE *n, int halfSteps) {
	int value = note_to_int(n);
	value += halfSteps;
	note_from_int(n, value);
	return n;
}