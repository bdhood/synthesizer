#include "waveform.h"
#include "synth.h"

#include <stdio.h>

WAVEFORM* waveform_init() {
	return (WAVEFORM*)malloc(sizeof(WAVEFORM));
}

void waveform_free(WAVEFORM* wf) {
	free(wf);
}

void waveform_render(WAVEFORM* wf, SDL_Renderer* r) {

	float perc;
	int left_amp, right_amp;

	int width = wf->rect.w;
	int height = wf->rect.h;

	float* buffer = synth_get_buffer();
	int nChannels = synth_get_channels();
	int bufferSize = synth_get_buffer_size();

	SDL_SetRenderDrawColor(r, wf->color_bg.r, wf->color_bg.g, wf->color_bg.b, 255);
	SDL_RenderFillRect(r, &wf->rect);

	for (int i = 0; i < width; i++) {

		perc = (float)i / (float)width;

		left_amp = (int)(perc * (float)bufferSize);
		if (left_amp % 2 == 1) {	// if odd
			left_amp++;				// make even
		}

		right_amp = left_amp + 1;

		float f = buffer[left_amp];
		//if (f != 0.0f) printf("%f\n", f);
		if (f > 1.0f) {				// visual clip
			f = 1.0f;
		}
		else if (f < -1.0f) {
			f = -1.0f;
		}
		left_amp = (int)(f * ((float)height / 4));
		f = buffer[right_amp];

		if (f > 1.0f) {				// visual clip
			f = 1.0f;
		}
		else if (f < -1.0f) {
			f = -1.0f;
		}
		right_amp = (int)(f * ((float)height / 4));

		//printf("%d %d\n", left_amp, right_amp);
		SDL_SetRenderDrawColor(r, wf->color_left.r, wf->color_left.g, wf->color_left.b, 255);
		SDL_RenderDrawLine(r, wf->rect.x + i, wf->rect.y + (height / 4), wf->rect.x + i, wf->rect.y + (height / 4) - left_amp);

		SDL_SetRenderDrawColor(r, wf->color_right.r, wf->color_right.g, wf->color_right.b, 255);
		SDL_RenderDrawLine(r, wf->rect.x + i, wf->rect.y + (3 * height / 4), wf->rect.x + i, wf->rect.y + (3 * height / 4) - right_amp);
		
	}
		
	SDL_SetRenderDrawColor(r, 0, 0, 0, 255);
}



/*
#include <Windows.h>
#include <stdio.h>

#include "synth.h"


HBRUSH wf_bg = NULL;

void waveform_paint(HDC hdc, RECT rect) {

	if (wf_bg == NULL) {
		wf_bg = CreateSolidBrush(RGB(0, 0, 0));
	}
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

		float f = buffer[left_amp];

		if (f > 1.0f) {				// visual clip
			f = 1.0f;
		}
		else if (f < -1.0f) {
			f = -1.0f;
		}
		left_amp = (int)(f * ((float)height / 4));
		f = buffer[right_amp];

		if (f > 1.0f) {				// visual clip
			f = 1.0f;
		}
		else if (f < -1.0f) {
			f = -1.0f;
		}
		right_amp = (int)(f * ((float)height / 4));


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
*/