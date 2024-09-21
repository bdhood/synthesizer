#ifndef KEYMAP_H
#define KEYMAP_H

#include "note.h"

#include "cJSON.h"

void keymap_load(cJSON *keys_root);
NOTE *keymap_get(int c);
int keymap_get_transpose();
void keymap_add_transpose(int x);

#endif