#ifndef SINE_H
#define SINE_H

void sine_initialize();
void sine_callback(unsigned long framesPerBuffer, int sampleRate, int nChannels, float *out);

#endif