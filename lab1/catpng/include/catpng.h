#ifndef CATPNG_INCLUDE_CATPNG_H
#define CATPNG_INCLUDE_CATPNG_H

#include <stdint.h>

#define PNG_TYPE_BYTES 4

typedef struct {
    uint32_t length;
    uint8_t type[PNG_TYPE_BYTES];
    uint8_t *data;
    uint32_t crc;
} png_chunk_t;

typedef struct {
    uint64_t length;
    uint8_t *data;
} uncompressed_data_t;

int catPng(char* fileNames[], int fileCount);

#endif /* CATPNG_INCLUDE_CATPNG_H */