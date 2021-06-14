#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "settings.h"
#include "cJSON.h"
#include "fileio.h"
#include "resources.h"

#ifndef KEYMAP_CONFIG_FILE
#define KEYMAP_CONFIG_FILE "layouts/US-standard.json"
#endif

SETTINGS* settings_load(char *filename) {
    char *filedata = fileio_read(filename);
    if (filedata == 0) {
        printf("Warn: Could not read settings file '%s'. Using defaults\n", filename);
        fileio_write_binary(filename, default_settings, default_settings_length);
        if (!fileio_exists(KEYMAP_CONFIG_FILE)) {
            fileio_mkdir("layouts");
            fileio_write_binary(KEYMAP_CONFIG_FILE, default_key_layout, default_key_layout_length);
        }
        filedata = default_settings;
    }

    if (!fileio_exists("fonts/Segoe UI-Light.ttf")) {
        fileio_mkdir("fonts");
        fileio_write_binary("fonts/Segoe UI-Light.ttf", default_font, default_font_length);
    }

    SETTINGS* settings = (SETTINGS*)malloc(sizeof(SETTINGS));
    cJSON *root = cJSON_Parse(filedata);
    if (root == 0) {
        printf("Error: Could not parse json file '%s'\n", filename);
        exit(1);
    }
    for (cJSON *i = root->child; i != 0; i = i->next) {
        if (strcmp(i->string, "Sample Rate") == 0) {
            settings->sample_rate = i->valueint;
        }
        if (strcmp(i->string, "Target FPS") == 0) {
            settings->target_fps = i->valueint;
        }
        if (strcmp(i->string, "Frames Per Buffer") == 0) {
            settings->frames_per_buffer = i->valueint;
        }
        if (strcmp(i->string, "Transpose") == 0) {
            settings->transpose = i->valueint;
        }
        if (strcmp(i->string, "Keymap File") == 0) {
            settings->keys_config_file = i->valuestring;
        }
    }
    cJSON_free(root);
    char* keys_data = fileio_read(settings->keys_config_file);
    if (keys_data == 0) {
        printf("Error: Could not read file '%s'\n", settings->keys_config_file);
        exit(1);
    }
    settings->keys_root = cJSON_Parse(keys_data);
    if (settings->keys_root == 0) {
        printf("Error: Could not parse json file '%s'\n", settings->keys_config_file);
        exit(1);
    }
    return settings;
}