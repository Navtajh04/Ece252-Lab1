#include "../include/catpng.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <arpa/inet.h>

#define OUTPUT_FILE_NAME "all.png"

void catpng(char* fileNames[], int fileCount) {
    FILE* outputFile;
    FILE* inputFile;
    png_header_t header;
    uint8_t* buffer;
    uint32_t totalHeight = 0;
    uint32_t maxWidth = 0;

    // Open the output file for writing
    outputFile = fopen(OUTPUT_FILE_NAME, "wb");
    if (!outputFile) {
        printf("Error: Unable to open output file.\n");
        return;
    }

    // Write PNG signature
    uint8_t signature[8] = {137, 80, 78, 71, 13, 10, 26, 10};
    fwrite(signature, sizeof(uint8_t), 8, outputFile);

    // Iterate through input files
    for (int i = 0; i < fileCount; i++) {
        // Open the input file
        inputFile = fopen(fileNames[i], "rb");
        if (!inputFile) {
            printf("Error: Unable to open input file '%s'.\n", fileNames[i]);
            fclose(outputFile);
            return;
        }

        // Read the PNG header
        fread(&header, sizeof(png_header_t), 1, inputFile);

        // Update total height and maximum width
        uint32_t width = ntohl(*(uint32_t*)header.ihdrData);
        uint32_t height = ntohl(*(uint32_t*)(header.ihdrData + 4));
        totalHeight += height;
        if (width > maxWidth)
            maxWidth = width;

        // Calculate the image size
        fseek(inputFile, 0, SEEK_END);
        long imageSize = ftell(inputFile) - sizeof(png_header_t);

        // Allocate memory for the image data
        buffer = (uint8_t*)malloc(imageSize);
        if (!buffer) {
            printf("Error: Memory allocation failed.\n");
            fclose(inputFile);
            fclose(outputFile);
            return;
        }

        // Read the image data
        fseek(inputFile, sizeof(png_header_t), SEEK_SET);
        fread(buffer, sizeof(uint8_t), imageSize, inputFile);

        // Write image data
        fwrite(buffer, sizeof(uint8_t), imageSize, outputFile);

        // Free allocated memory and close input file
        free(buffer);
        fclose(inputFile);
    }

    // Update the total height in the output file's IHDR chunk
    fseek(outputFile, 16, SEEK_SET);
    *(uint32_t*)(header.ihdrData + 4) = htonl(totalHeight);
    fwrite(header.ihdrData, sizeof(uint8_t), sizeof(header.ihdrData), outputFile);

    // Write PNG IEND chunk
    png_chunk_header_t iend_header;
    iend_header.length = 0;
    iend_header.type = 0x49454E44;  // "IEND"
    fwrite(&iend_header, sizeof(png_chunk_header_t), 1, outputFile);

    // Close the output file
    fclose(outputFile);
}