#ifndef SYNTH_H
#define SYNTH_H

#include <stdbool.h>

void synth_on();
void synth_off();
bool synth_is_on();

unsigned long synth_get_framesPreBuffer();
double synth_get_sampleRate();
void synth_set_framesPerBuffer(unsigned long x);
void synth_set_sampleRate(int rate);
float *synth_get_buffer();
int synth_get_buffer_size();
int synth_get_channels();

#endif