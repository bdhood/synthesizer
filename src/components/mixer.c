#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>

#include "portaudio.h"

#include "sine.h"
#include "saw.h"
#include "square.h"

#include "normalizer.h"
#include "mixer.h"

int instrument = 0;

const char *instruments[] = {
	"Sine",
	"Saw",
	"Square"
};

int mixer_get_instrument() {
	return instrument;
}

const char *mixer_get_instrument_string() {
	return instruments[mixer_get_instrument()];
}

void mixer_set_instrument(int x) {
	instrument = abs(x);
}

void mixer_clear(unsigned long framesPerBuffer, int sampleRate, int nChannels, float *out) {
	for (unsigned long i = 0; i < framesPerBuffer * nChannels; i++) {
		out[i] = 0.0f;
	}
}

void mixer_initialize() {
	sine_initialize();
	saw_initialize();
	square_initialize();
}

void mixer_callback(unsigned long framesPerBuffer, int sampleRate, int nChannels, float *out) {
	mixer_clear(framesPerBuffer, sampleRate, nChannels, out);

	switch (instrument) {
		case 0:
			sine_callback(framesPerBuffer, sampleRate, nChannels, out);
			break;
		case 1:
			saw_callback(framesPerBuffer, sampleRate, nChannels, out);
			break;
		case 2:
			square_callback(framesPerBuffer, sampleRate, nChannels, out);
			break;
		default:
			printf("Error: Invalid instrument id\n");
			exit(1);
	}

	normalizer_callback(framesPerBuffer, nChannels, out);
}