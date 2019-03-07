#include <stdbool.h>
#include <math.h>

bool enabled = true;
float amplitude = 0.8f;

bool normalizer_get_enabled() {
	return enabled;
}

void normalizer_set_enabled(bool x) {
	enabled = x;
}

int normalizer_get_amplitude() {
	return (int)(amplitude * 100);
}

void normalizer_set_amplitude(int x) {
	amplitude = (float)x / 100.0f;
}

void normalizer_callback(unsigned long framesPerBuffer,  int nChannels, float *out)
{
	if (!enabled)
		return;

	float max = 0.0f;
	unsigned long i;

	for (i = 0; i < framesPerBuffer * nChannels; i++) {
		if (max < abs(out[i]))
			max = abs(out[i]);
	}

	if (max == 0.0f)
		return;

	for (i = 0; i < framesPerBuffer * nChannels; i++) {
		out[i] /= max;
		out[i] *= amplitude;
	}
}