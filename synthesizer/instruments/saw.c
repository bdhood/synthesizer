#include <Windows.h>
#include <stdio.h>

#include "activenotes.h"
#include "keymap.h"
#include "freqmap.h"
#include "note.h"


#pragma warning (disable:4244)

#define _USE_MATH_DEFINES
#include <cmath>

#define VOICES 16

float saw_voice_locations[VOICES];

void saw_initialize() {
	for (int i = 0; i < VOICES; i++) {
		saw_voice_locations[i] = 0.0f;
	}
}

void saw_callback(unsigned long framesPerBuffer, int sampleRate, int nChannels, float *out) {
	float left_phase = 0.0f, right_phase = 0.0f;

	NOTE *n;
	NOTE n2;
	NOTE n3;
	bool flip = 0;
	bool flip2 = 0;
	int voice = 0;
	float *outputBuffer = out;
	int nNotes = activenotes_get_total() * 1;
	int index = 0;
	n3.note = 'a';
	n3.octave = 0;
	n3.sharp = 0;
	n = activenotes_next(&n3);
	while (n != 0) {
		if (n == 0)
			break;

		if (n->note == 0)
			continue;

		flip = 1;

		for (int k = 0; k < 1; k++) {
			n2.note = n->note;
			n2.octave = n->octave + k;
			n2.sharp = n->sharp;
			float freq = freqmap_get(&n2);
			if ((index >= nNotes / 2) && flip2 == 0) {
				flip2 = 1;
				printf("\t\t\t\t");
			}
			if (n2.sharp == 0) {
				printf("%c%d ", n2.note, n2.octave);
			}
			else {
				printf("%c#%d ", n2.note, n2.octave);
			}
			for (unsigned long i = 0; i < framesPerBuffer; i++) {

				saw_voice_locations[voice] += freq / (float)sampleRate;

				if (saw_voice_locations[voice] > 1.0f) {
					saw_voice_locations[voice] -= 2.0f;
				}

				if (index < nNotes / 2) {
					out[(i * nChannels)] += 0.30f * saw_voice_locations[voice];
					out[(i * nChannels) + 1] += saw_voice_locations[voice];
				}
				else if (index >= nNotes / 2) {
					out[(i * nChannels)] += saw_voice_locations[voice];
					out[(i * nChannels) + 1] += 0.30f * saw_voice_locations[voice];
				}

			}
			voice++;
			voice %= VOICES;
			index++;
		}
		n = activenotes_next(n);
	}

	if (flip == 1)
		printf("\n");
}