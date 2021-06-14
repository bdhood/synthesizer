#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "SDL.h"
#include "portaudio.h"
#include "keymap.h"
#include "freqmap.h"
#include "activekeys.h"
#include "activenotes.h"
#include "mixer.h"

#define MONO	1
#define STEREO	2

#define CHANNEL STEREO
#define SAMPLERATE 96000
#define FRAMESPERBUFFER 2048

unsigned long framesPerBuffer = FRAMESPERBUFFER;
double sampleRate = (float)SAMPLERATE;

float backBuffer[CHANNEL * FRAMESPERBUFFER];

PaStream *stream;
PaError err;

int synth_get_channels() {
	return CHANNEL;
}

float *synth_get_buffer() {
	return backBuffer;
}

int synth_get_buffer_size() {
	return framesPerBuffer * CHANNEL;
}

unsigned long synth_get_framesPreBuffer() {
	return framesPerBuffer;
}

double synth_get_sampleRate() {
	return sampleRate;
}

void synth_set_framesPerBuffer(unsigned long x) {
	framesPerBuffer = x;
}

void synth_set_sampleRate(int rate) {
	sampleRate = (double)rate;
}

static int paCallback( const void *inputBuffer, void *outputBuffer, unsigned long framesPerBuffer,
                           const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags, void *userData )
{
	memcpy(backBuffer, (float*)outputBuffer, synth_get_buffer_size() * sizeof(float));
	mixer_callback(framesPerBuffer, (int)sampleRate, CHANNEL, (float*)outputBuffer);
    return 0;
}

void synth_check_err(PaError err) {
	if (err != paNoError) {
		fprintf( stderr, "An error occured while using the portaudio stream\n" );
		fprintf( stderr, "Error number: %d\n", err );
		fprintf( stderr, "Error message: %s\n", Pa_GetErrorText( err ) );
		exit(1);
	}
}

void synth_on() {

    /* Initialize library before making any other calls. */
    err = Pa_Initialize();
    synth_check_err(err);
    
    /* Open an audio I/O stream. */
    err = Pa_OpenDefaultStream( &stream,
                                0,          /* no input channels */
                                CHANNEL,          
								paFloat32,
								sampleRate,
								framesPerBuffer,        /* frames per buffer */
                                paCallback,
                                NULL );
	synth_check_err(err);
	err = Pa_StartStream(stream);
	synth_check_err(err);

	/*
	while (1) {
		Pa_Sleep(1);
	}
	*/
}

void synth_off() {
	err = Pa_StopStream(stream);
	synth_check_err(err);

	err = Pa_CloseStream(stream);
	synth_check_err(err);

	Pa_Terminate();
	stream = 0;
}

bool synth_is_on() {
	return stream != 0;
}