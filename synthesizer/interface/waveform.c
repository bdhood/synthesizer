#include <Windows.h>
#include <stdio.h>

#include "synth.h"


HBRUSH wf_bg = CreateSolidBrush(RGB(0, 0, 0));

void waveform_paint(HDC hdc, RECT rect) {
	float *buffer = synth_get_buffer();
	int nChannels = synth_get_channels();
	int bufferSize = synth_get_buffer_size();
	int width = rect.right - rect.left;
	int height = rect.bottom - rect.top;
	int left_amp, right_amp;
	float perc;
	
	FillRect(hdc, &rect, wf_bg);
	for (int i = 0; i < width; i++) {

		perc = (float)i / (float)width;

		left_amp = (int)(perc * (float)bufferSize);
		if (left_amp % 2 == 1) {	// if odd
			left_amp++;				// make even
		}

		right_amp = left_amp + 1;

		left_amp = (int)(buffer[left_amp] * ((float)height / 4));
		right_amp = (int)(buffer[right_amp] * ((float)height / 4));


		if (left_amp > 0) {
			for (int j = 0; j <= left_amp; j++) {
				SetPixel(hdc, rect.left + i, rect.top + (height / 4) - j, RGB(0, 200, 200));
			}
		}
		else {
			for (int j = 0; j >= left_amp; j--) {
				SetPixel(hdc, rect.left + i, rect.top + (height / 4) - j, RGB(0, 200, 200));
			}
		}		
		if (right_amp > 0) {
			for (int j = 0; j <= right_amp; j++) {
				SetPixel(hdc, rect.left + i, rect.top + (3 * height / 4) - j, RGB(200, 200, 0));
			}
		}
		else {
			for (int j = 0; j >= right_amp; j--) {
				SetPixel(hdc, rect.left + i, rect.top + (3 * height / 4) - j, RGB(200, 200, 0));
			}
		}
	}
}