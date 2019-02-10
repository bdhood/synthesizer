#ifndef SAW_H
#define SAW_H

void saw_initialize();
void saw_callback(unsigned long framesPerBuffer, int sampleRate, int nChannels, float *out);

#endif