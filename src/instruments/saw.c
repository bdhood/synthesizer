#include <stdio.h>

#include "activenotes.h"
#include "keymap.h"
#include "freqmap.h"
#include "note.h"


#pragma warning (disable:4244)

#define _USE_MATH_DEFINES
#include <math.h>

#define VOICES 16

float saw_voice_locations[VOICES];
float saw_voice_frequency[VOICES];

void saw_initialize() {
	for (int i = 0; i < VOICES; i++) {
		saw_voice_locations[i] = 0.0f;
		saw_voice_frequency[i] = 0.0f;
	}
}

float transformA(float x) {

	
	x -= (x * x);
	x += 0.5f;
	x *= 2;
	return x;
}
float transformB(float x) {
	return x;
}

void saw_callback(unsigned long framesPerBuffer, int sampleRate, int nChannels, float* out) {
	NOTE* n;
	NOTE n2;
	NOTE n3;
	bool flip2 = 0;
	int voice = 0;
	int nNotes = activenotes_get_total() * 2;
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

		for (int k = 0; k < 2; k++) {
			n2.note = n->note;
			n2.octave = n->octave + k;
			n2.sharp = n->sharp;
			float freq = freqmap_get(&n2);
			if (n2.sharp == 0) {
				//printf("%c%d  ", n2.note, n2.octave);
			}
			else {
				//printf("%c#%d ", n2.note, n2.octave);
			}
			int dir = 1;
			for (unsigned long i = 0; i < framesPerBuffer; i++) {

				saw_voice_frequency[voice] = freq / (float)sampleRate;
				saw_voice_locations[voice] += saw_voice_frequency[voice];
				if (saw_voice_locations[voice] > 1.0f) {
					saw_voice_locations[voice] -= 2.0f;
				}


				//out[(i * nChannels)] += transformA(saw_voice_locations[voice]);
				//out[(i * nChannels) + 1] += transformA(saw_voice_locations[voice]);


				if (index <= nNotes / 2) {
					//out[(i * nChannels)] += transform(saw_voice_locations[voice]);
					out[(i * nChannels)] += transformB(saw_voice_locations[voice]);
					//out[(i * nChannels) + 1] += transformA(saw_voice_locations[voice]);
				}
				if (index >= nNotes / 2) {
					//out[(i * nChannels) + 1] += transform(saw_voice_locations[voice]);
					//out[(i * nChannels)] += transformB(saw_voice_locations[voice]);
					out[(i * nChannels) + 1] += transformA(saw_voice_locations[voice]);
				}
			}
			voice++;
			voice %= VOICES;
			index++;
		}
		n = activenotes_next(n);
	}
	if (nNotes > 0) {
		//printf("\n");
	}
}
