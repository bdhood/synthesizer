#ifndef FILEIO_H
#define FILEIO_H

#include <stdbool.h>

char* fileio_read(char* filename);
bool fileio_write(char* filename, char *data);
bool fileio_write_binary(char* filename, char *data, int length);
bool fileio_exists(char* filename);
void fileio_mkdir(char* path);

#endif