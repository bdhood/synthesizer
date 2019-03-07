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

int n; // location in sine wave

float voice_locations[VOICES];

void sine_initialize() {
	for (int i = 0; i < VOICES; i++) {
		voice_locations[i] = 0;
	}
}

void sine_callback(unsigned long framesPerBuffer, int sampleRate, int nChannels, float *out) {
	float left_phase = 0.0f, right_phase = 0.0f;

	unsigned char c;
	NOTE *n;

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
		float freq = freqmap_get(n);

		for (unsigned long i = 0; i < framesPerBuffer; i++) {

			voice_locations[voice] += freq / (float)sampleRate;

			if (voice_locations[voice] > 10.0f) {
				voice_locations[voice] -= 10.0f;
			}
			float theta = 2.0f * M_PI * voice_locations[voice];
			theta = sinf(theta);
			
			left_phase = theta;

			for (int j = 0; j < nChannels; j++)
				*out++ += left_phase;
		}
		out = outputBuffer;
		voice++;
		voice %= VOICES;
	}
	for (int i = voice; i < VOICES; i++) {
		voice_locations[i] = fmod(voice_locations[i], 1.0f) / 2.0f;

	}

	if (flip == 1)
		printf("\n");
}