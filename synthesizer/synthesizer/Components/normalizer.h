#ifndef NORMALIZER_H
#define NORMALIZER_H

#include <stdbool.h>

bool normalizer_get_enabled();
void normalizer_set_enabled(bool x);

int normalizer_get_amplitude();
void normalizer_set_amplitude(int x);

void normalizer_callback(unsigned long framesPerBuffer, int nChannels, float *out);

#endif