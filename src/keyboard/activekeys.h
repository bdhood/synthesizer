#ifndef ACTIVEKEYS_H
#define ACTIVEKEYS_H

#include <stdbool.h>

void activekeys_load();
void activekeys_add(int vkcode);
void activekeys_rm(int vkcode);
int activekeys_next(int vkcode);
int activekeys_first();
int activekeys_get_total();
bool activekeys_isactive(int i);
#endif
