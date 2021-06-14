#include <Windows.h>

void fileio_mkdir(char *path) {
    CreateDirectoryA(path, NULL);
}