#ifndef KEYMAP_H
#define KEYMAP_H

#include "note.h"

void keymap_load();
NOTE *keymap_get(int c);
int keymap_get_transpose();
void keymap_set_transpose(int x);

#endif