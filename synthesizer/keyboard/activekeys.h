#ifndef ACTIVEKEYS_H
#define ACTIVEKEYS_H

void activekeys_load();
void activekeys_add(unsigned char vkcode);
void activekeys_rm(unsigned char vkcode);
unsigned char activekeys_next(unsigned char vkcode);
unsigned char activekeys_first();
int activekeys_get_total();

#endif
