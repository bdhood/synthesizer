#include <Windows.h>
#include <stdio.h>

#include "activekeys.h"
#include "keymap.h"
#include "freqmap.h"
#include "note.h"


#pragma warning (disable:4244)

#define _USE_MATH_DEFINES
#include <cmath>

#define VOICES 16

float square_voice_locations[VOICES];
bool square_voice_directions[VOICES];

void square_initialize() {
	for (int i = 0; i < VOICES; i++) {
		square_voice_locations[i] = 0.0f;
	}
}

void square_callback(unsigned long framesPerBuffer, int sampleRate, int nChannels, float *out) {
	float left_phase = 0.0f, right_phase = 0.0f;

	unsigned char c;
	NOTE *n;
	NOTE n2;
	bool flip = 0;
	int voice = 0;
	float *outputBuffer = out;
	for (c = activekeys_first(); c != 0; c = activekeys_next(c)) {
		n = keymap_get(c);

		if (n->note == 0)
			continue;

		flip = 1;
		if (n->sharp == 0) {
			printf("%c%d ", n->note, n->octave);
		}
		else {
			printf("%c#%d ", n->note, n->octave);
		}
		for (int k = 0; k < 1; k++) {
			n2.note = n->note;
			n2.octave = n->octave + k;
			n2.sharp = n->sharp;
			float freq = freqmap_get(&n2);

			for (unsigned long i = 0; i < framesPerBuffer; i++) {
				if (square_voice_directions[voice]) {
					square_voice_locations[voice] += 2 * freq / (float)sampleRate;
				}
				else {
					square_voice_locations[voice] -= 2 * freq / (float)sampleRate;
				}

				if (square_voice_locations[voice] > 0.8f) {
					square_voice_directions[voice] = 0;
				}
				else if (square_voice_locations[voice] < -0.8f) {
					square_voice_directions[voice] = 1;
				}

				for (int j = 0; j < nChannels; j++)
					*out++ += square_voice_locations[voice];
			}
			out = outputBuffer;
			voice++;
			voice %= VOICES;
		}
	}

	if (flip == 1)
		printf("\n");
}