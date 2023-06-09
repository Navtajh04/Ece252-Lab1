#ifndef CATPNG_INCLUDE_CATPNG_H
#define CATPNG_INCLUDE_CATPNG_H

#include <stdint.h>

#define PNG_TYPE_BYTES 4

typedef struct {
    uint64_t length;
    uint8_t *data;
} uncompressed_data_t;

int catPng(char* fileNames[], int fileCount);

#endif /* CATPNG_INCLUDE_CATPNG_H */