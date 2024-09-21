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

float __abs(float f) {
	if (f < 0.0f) {
		return -f;
	}
	else {
		return f;
	}
}

void normalizer_callback(unsigned long framesPerBuffer,  int nChannels, float *out)
{
	if (!enabled)
		return;

	unsigned long i, channel;

	for (channel = 0; channel < nChannels; channel++) {

		float max = 0.0f;

		for (i = channel; i < framesPerBuffer * nChannels; i += nChannels) {
			if (max < __abs(out[i]))
				max = __abs(out[i]);
		}

		if (max == 0.0f)
			break;

		for (i = channel; i < framesPerBuffer * nChannels; i += nChannels) {
			out[i] /= max;
			out[i] *= amplitude;
		}
	}
}