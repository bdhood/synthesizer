#ifndef ACTIVENOTES_H
#define ACTIVENOTES_H

#include <stdbool.h>

#include "note.h"

void activenotes_load();
void activenotes_add(NOTE *n);
void activenotes_rm(NOTE *n);
bool activenotes_isactive(NOTE *n);
NOTE *activenotes_next(NOTE *n);
int activenotes_get_total();

#endif
