#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

char* fileio_read(char* filename) {
    FILE* fp = fopen(filename, "rb");
    if (fp == NULL) {
        return 0;
    }
    fseek(fp, 0, SEEK_END);
    int len = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    if (len == 0) {
        fclose(fp);
        return 0;
    }
    else {
        char* filedata = (char*)malloc((len * sizeof(char)) + 1);
        if (filedata == 0) {
            return 0;
        }
        memset(filedata, 0, (len * sizeof(char)) + 1);
        fread(filedata, sizeof(char), len, fp);
        fclose(fp);
        return filedata;
    }
}

bool fileio_write_binary(char* filename, char* data, int length) {
    FILE* fp = fopen(filename, "wb");
    if (fp == NULL) {
        return false;
    }
    size_t ret = fwrite(data, sizeof(char), length, fp);
    fclose(fp);
    return length == ret;
}


bool fileio_write(char* filename, char *data) {
    return fileio_write_binary(filename, data, strlen(data));
}

bool fileio_exists(char* filename) {
    FILE* fp = fopen(filename, "rb");
    if (fp == NULL) {
        return false;
    }
    else {
        fclose(fp);
        return true;
    }
}