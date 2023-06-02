#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_FILE_COUNT 10
#define OUTPUT_FILE_NAME "all.png"

typedef struct {
    uint32_t width;
    uint32_t height;
    uint8_t bitDepth;
    uint8_t colorType;
    uint8_t compressionMethod;
    uint8_t filterMethod;
    uint8_t interlaceMethod;
} Png_header_t;

void catpng(const char* fileNames[], int fileCount) {
    FILE* outputFile;
    FILE* inputFile;
    Png_header_t header;
    uint8_t* buffer;
    uint32_t totalHeight = 0;
    uint32_t maxWidth = 0;

    // Open the output file for writing
    outputFile = fopen(OUTPUT_FILE_NAME, "wb");
    if (!outputFile) {
        printf("Error: Unable to open output file.\n");
        return;
    }

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
        fread(&header, sizeof(Png_header_t), 1, inputFile);

        // Update total height and maximum width
        totalHeight += header.height;
        if (header.width > maxWidth)
            maxWidth = header.width;

        // Allocate memory for the image data
        buffer = (uint8_t*)malloc(header.width * header.height * 4);  // Assuming 4 bytes per pixel (RGBA)

        // Read the image data
        fread(buffer, sizeof(uint8_t), header.width * header.height * 4, inputFile);

        // Write the image data to the output file
        fwrite(buffer, sizeof(uint8_t), header.width * header.height * 4, outputFile);

        // Free allocated memory and close input file
        free(buffer);
        fclose(inputFile);
    }

    // Update the total height in the output file's header
    fseek(outputFile, 8, SEEK_SET);
    fwrite(&totalHeight, sizeof(uint32_t), 1, outputFile);

    // Update the maximum width in the output file's header
    fseek(outputFile, 16, SEEK_SET);
    fwrite(&maxWidth, sizeof(uint32_t), 1, outputFile);

    // Close the output file
    fclose(outputFile);

    printf("Concatenated PNG files vertically into '%s'.\n", OUTPUT_FILE_NAME);
}