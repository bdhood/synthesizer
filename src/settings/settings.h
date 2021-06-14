#ifndef SETTINGS_H
#define SETTINGS_H

#include "cJSON.h"

typedef struct SETTINGS {
	int target_fps;
	int sample_rate;
	int frames_per_buffer;
	int transpose;
	char* keys_config_file;
	cJSON *keys_root;
} SETTINGS;

SETTINGS* settings_load(char *filename);

#endif