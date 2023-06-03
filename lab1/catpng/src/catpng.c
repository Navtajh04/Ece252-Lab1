#include "../include/catpng.h"
#include "../include/zutil.h"
#include "../include/crc.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <arpa/inet.h>
#include <zlib.h>


#define OUTPUT_FILE_NAME "all.png"
#define SIGNATURE_BYTES 8


void catPng(char* fileNames[], int fileCount) {
    FILE* fp = fopen("all.png", "wb");
    if (fp == NULL) {
        printf("Failed to open output file\n");
        
    }

    // Variables to keep track of the maximum width and total height
    int maxWidth = 0;
    int totalHeight = 0;

    int i;
    for (i = 0; i < fileCount; i++) {
        char* fileName = fileNames[i];
        FILE* inFile = fopen(fileName, "rb");
        if (inFile == NULL) {
            printf("Failed to open input file: %s\n", fileName);
            fclose(fp);
            
        }

        // Read the entire file into memory
        fseek(inFile, 0, SEEK_END);
        long fileSize = ftell(inFile);
        rewind(inFile);
        unsigned char* fileData = (unsigned char*)malloc(fileSize);
        if (fileData == NULL) {
            printf("Memory allocation failed\n");
            fclose(inFile);
            fclose(fp);
            
        }
        if (fread(fileData, 1, fileSize, inFile) != fileSize) {
            printf("Failed to read file: %s\n", fileName);
            free(fileData);
            fclose(inFile);
            fclose(fp);
            
        }
        fclose(inFile);

        // Check if the file is compressed
        int isCompressed = (fileData[0] == 0x78 && fileData[1] == 0x9C);

        if (isCompressed) {
            // Decompress the data
            unsigned long destLen = CHUNK;
            unsigned char* dest = (unsigned char*)malloc(destLen);
            if (dest == NULL) {
                printf("Memory allocation failed\n");
                free(fileData);
                fclose(fp);
                
            }

            z_stream strm;
            strm.zalloc = Z_NULL;
            strm.zfree = Z_NULL;
            strm.opaque = Z_NULL;
            strm.avail_in = fileSize;
            strm.next_in = fileData;
            strm.avail_out = destLen;
            strm.next_out = dest;

            if (inflateInit(&strm) != Z_OK) {
                printf("Failed to initialize zlib inflate\n");
                free(dest);
                free(fileData);
                fclose(fp);
                
            }

            int ret;
            do {
                ret = inflate(&strm, Z_NO_FLUSH);
                if (ret == Z_STREAM_ERROR) {
                    printf("Zlib inflate error\n");
                    inflateEnd(&strm);
                    free(dest);
                    free(fileData);
                    fclose(fp);
                    
                }

                if (ret == Z_OK || ret == Z_STREAM_END) {
                    if (fwrite(dest, 1, destLen - strm.avail_out, fp) != destLen - strm.avail_out) {
                        printf("Failed to write uncompressed data to file\n");
                        inflateEnd(&strm);
                        free(dest);
                        free(fileData);
                        fclose(fp);
                        
                    }
                }

                if (ret == Z_NEED_DICT || ret == Z_DATA_ERROR || ret == Z_MEM_ERROR) {
                    printf("Failed to uncompress data from file: %s\n", fileName);
                    inflateEnd(&strm);
                    free(dest);
                    free(fileData);
                    fclose(fp);
                    
                }

                strm.avail_out = destLen;
                strm.next_out = dest;
            } while (ret != Z_STREAM_END);

            inflateEnd(&strm);
            free(dest);
        } else {
            // Write the uncompressed data directly to the output file
            if (fwrite(fileData, 1, fileSize, fp) != fileSize) {
                printf("Failed to write uncompressed data to file\n");
                free(fileData);
                fclose(fp);
                
            }
        }

        free(fileData);

        // Update the maximum width and total height
        if (i == 0) {
            // For the first image, set the maximum width
            maxWidth = fileData[16] * 256 + fileData[17];
        }
        totalHeight += fileData[20] * 256 + fileData[21];
    }

    // Update the IHDR chunk for the concatenated image
    fseek(fp, 16, SEEK_SET);
    unsigned char widthBytes[4];
    widthBytes[0] = (unsigned char)(maxWidth >> 24);
    widthBytes[1] = (unsigned char)(maxWidth >> 16);
    widthBytes[2] = (unsigned char)(maxWidth >> 8);
    widthBytes[3] = (unsigned char)(maxWidth);
    fwrite(widthBytes, 1, 4, fp);
    fseek(fp, 20, SEEK_SET);
    unsigned char heightBytes[4];
    heightBytes[0] = (unsigned char)(totalHeight >> 24);
    heightBytes[1] = (unsigned char)(totalHeight >> 16);
    heightBytes[2] = (unsigned char)(totalHeight >> 8);
    heightBytes[3] = (unsigned char)(totalHeight);
    fwrite(heightBytes, 1, 4, fp);

    fclose(fp);
    
        FILE *outputFile = fopen("all.png", "wb");
    if (outputFile == NULL)
    {
        printf("Failed to open output file\n");
        
    }

    unsigned long crc = 0xFFFFFFFFL;
    unsigned long newCrc;

    // Iterate over the input files
    for (int i = 0; i < fileCount; i++)
    {
        FILE *currentFile = fopen(fileNames[i], "rb");
        if (currentFile == NULL)
        {
            printf("Failed to open file: %s\n", fileNames[i]);
            fclose(outputFile);
            
        }

        unsigned char buf[CHUNK];
        unsigned long bytesRead;

        // Read and write the file data
        while ((bytesRead = fread(buf, 1, CHUNK, currentFile)) > 0)
        {
            if (fwrite(buf, 1, bytesRead, outputFile) != bytesRead)
            {
                printf("Failed to write data to the output file\n");
                fclose(outputFile);
                fclose(currentFile);
                
            }
        }

        newCrc = calculateCrc(buf, bytesRead);
        crc = crc32(crc, buf, bytesRead);
        printf("CRC for %s: 0x%lx\n", fileNames[i], newCrc);

        fclose(currentFile);
    }

    crc = crc ^ 0xFFFFFFFFL;
    printf("Final CRC: 0x%lx\n", crc);

    // Append the CRC bytes to the output file
    unsigned char crcBytes[4];
    crcBytes[0] = (crc >> 24) & 0xFF;
    crcBytes[1] = (crc >> 16) & 0xFF;
    crcBytes[2] = (crc >> 8) & 0xFF;
    crcBytes[3] = crc & 0xFF;

    if (fwrite(crcBytes, sizeof(unsigned char), 4, outputFile) != 4)
    {
        printf("Failed to write CRC bytes to the output file\n");
        fclose(outputFile);
        
    }

    fclose(outputFile);

    
}