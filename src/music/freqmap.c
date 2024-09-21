#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "note.h"

#define OCTAVES 8
#define NOTES_PER_OCTAVE 12
#define TOTAL_NOTES (OCTAVES * NOTES_PER_OCTAVE)
#define A4_FREQ 440.0

float freqmap[TOTAL_NOTES];

float freqmap_get(NOTE *n) {
	return freqmap[note_to_int(n)];
}

void freqmap_load() {
	double twelth_root_of_2 = powl(2.0, (1.0 / 12.0));
	memset(freqmap, 0, sizeof(freqmap));
	for (int i = 0; i < TOTAL_NOTES; i++) {
		freqmap[i] = (float)(A4_FREQ * powl(twelth_root_of_2, i - 57));
	}
}
