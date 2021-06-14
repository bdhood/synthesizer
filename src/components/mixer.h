#ifndef MIXER_H
#define MIXER_H

void mixer_callback(unsigned long framesPerBuffer, int sampleRate, int nChannels, float *out);
int mixer_get_instrument();
void mixer_set_instrument(int x);
const char *mixer_get_instrument_string();

#endif