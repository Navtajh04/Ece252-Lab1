#ifndef CATPNG_INCLUDE_CATPNG_H
#define CATPNG_INCLUDE_CATPNG_H

#include <stdint.h>

typedef struct {
    uint32_t length;
    uint32_t type;
} png_chunk_header_t;

typedef struct {
    uint8_t signature[8];
    png_chunk_header_t ihdrHeader;
    uint8_t ihdrData[13];
} png_header_t;

int catPng(char* fileNames[], int fileCount);
// void catPng(char* fileNames[], int fileCount);

#endif /* CATPNG_INCLUDE_CATPNG_H */