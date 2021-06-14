#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "note.h"

#define OCTAVES 8

#define FREQ_ADD(A, B, C, D) freqmap[A][B][C] = (float)(a4 * powl(twelth_root_of_2, D));

float freqmap[0x100][2][OCTAVES];

float freqmap_get(NOTE *n) {
	if (n->sharp >= 2 || n->octave > OCTAVES) {
		printf("invalid note");
		exit(1);
	}
	return freqmap[n->note][n->sharp][n->octave];
}

void freqmap_load() {
	memset(freqmap, 0, sizeof(freqmap));
	
	double twelth_root_of_2 = powl(2.0, (1.0 / 12.0));
	double a4 = 440.0;

	
	freqmap['c'][0][0] = 16.35f;
	freqmap['c'][1][0] = 17.32f;
	freqmap['d'][0][0] = 18.35f;
	freqmap['d'][1][0] = 19.45f;
	freqmap['e'][0][0] = 20.60f;
	freqmap['f'][0][0] = 21.83f;
	freqmap['f'][1][0] = 23.12f;
	freqmap['g'][0][0] = 24.50f;
	freqmap['g'][1][0] = 25.96f;
	freqmap['a'][0][0] = 27.50f;
	freqmap['a'][1][0] = 29.14f;
	freqmap['b'][0][0] = 30.87f;

	freqmap['c'][0][1] = 32.70f;
	freqmap['c'][1][1] = 34.65f;
	freqmap['d'][0][1] = 36.71f;
	freqmap['d'][1][1] = 38.89f;
	freqmap['e'][0][1] = 41.20f;
	freqmap['f'][0][1] = 43.65f;
	freqmap['f'][1][1] = 46.25f;
	freqmap['g'][0][1] = 49.00f;
	freqmap['g'][1][1] = 51.91f;
	freqmap['a'][0][1] = 55.00f;
	freqmap['a'][1][1] = 58.27f;
	freqmap['b'][0][1] = 61.74f;
	
	freqmap['c'][0][2] = 65.41f;
	freqmap['c'][1][2] = 69.30f;
	freqmap['d'][0][2] = 73.42f;
	freqmap['d'][1][2] = 77.78f;
	freqmap['e'][0][2] = 82.41f;
	freqmap['f'][0][2] = 87.31f;
	freqmap['f'][1][2] = 92.50f;
	freqmap['g'][0][2] = 98.00f;
	freqmap['g'][1][2] = 103.83f;
	freqmap['a'][0][2] = 110.00f;
	freqmap['a'][1][2] = 116.54f;
	freqmap['b'][0][2] = 123.47f;
	
	FREQ_ADD('c', 0, 3, -21);
	FREQ_ADD('c', 1, 3, -20);
	FREQ_ADD('d', 0, 3, -19);
	FREQ_ADD('d', 1, 3, -18);
	FREQ_ADD('e', 0, 3, -17);
	FREQ_ADD('f', 0, 3, -16);
	FREQ_ADD('f', 1, 3, -15);
	FREQ_ADD('g', 0, 3, -14);
	FREQ_ADD('g', 1, 3, -13);
	FREQ_ADD('a', 0, 3, -12);
	FREQ_ADD('a', 1, 3, -11);
	FREQ_ADD('b', 0, 3, -10);

	
	FREQ_ADD('c', 0, 4, -9);
	FREQ_ADD('c', 1, 4, -8);
	FREQ_ADD('d', 0, 4, -7);
	FREQ_ADD('d', 1, 4, -6);
	FREQ_ADD('e', 0, 4, -5);
	FREQ_ADD('f', 0, 4, -4);
	FREQ_ADD('f', 1, 4, -3);
	FREQ_ADD('g', 0, 4, -2);
	FREQ_ADD('g', 1, 4, -1);
	FREQ_ADD('a', 0, 4, 0);
	FREQ_ADD('a', 1, 4, 1);
	FREQ_ADD('b', 0, 4, 2);
	
	FREQ_ADD('c', 0, 5, 3);
	FREQ_ADD('c', 1, 5, 4);
	FREQ_ADD('d', 0, 5, 5);
	FREQ_ADD('d', 1, 5, 6);
	FREQ_ADD('e', 0, 5, 7);
	FREQ_ADD('f', 0, 5, 8);
	FREQ_ADD('f', 1, 5, 9);
	FREQ_ADD('g', 0, 5, 10);
	FREQ_ADD('g', 1, 5, 11);
	FREQ_ADD('a', 0, 5, 12);
	FREQ_ADD('a', 1, 5, 13);
	FREQ_ADD('b', 0, 5, 14);
	
	freqmap['c'][0][6] = 1046.50f;
	freqmap['c'][1][6] = 1108.73f;
	freqmap['d'][0][6] = 1174.66f;
	freqmap['d'][1][6] = 1244.51f;
	freqmap['e'][0][6] = 1318.51f;
	freqmap['f'][0][6] = 1396.91f;
	freqmap['f'][1][6] = 1479.98f;
	freqmap['g'][0][6] = 1567.98f;
	freqmap['g'][1][6] = 1661.22f;
	freqmap['a'][0][6] = 1760.00f;
	freqmap['a'][1][6] = 1864.66f;
	freqmap['b'][0][6] = 1975.53f;

	freqmap['c'][0][7] = 2093.00f;
	freqmap['c'][1][7] = 2217.46f;
	freqmap['d'][0][7] = 2349.32f;
	freqmap['d'][1][7] = 2489.02f;
	freqmap['e'][0][7] = 2637.02f;
	freqmap['f'][0][7] = 2793.83f;
	freqmap['f'][1][7] = 2959.96f;
	freqmap['g'][0][7] = 3135.96f;
	freqmap['g'][1][7] = 3322.44f;
	freqmap['a'][0][7] = 3520.00f;
	freqmap['a'][1][7] = 3729.31f;
	freqmap['b'][0][7] = 3951.07f;

	
	
}