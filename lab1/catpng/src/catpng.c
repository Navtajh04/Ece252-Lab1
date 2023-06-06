#include "../include/catpng.h"
#include "../include/zutil.h"
#include "../include/crc.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <arpa/inet.h>
#include <zlib.h>


#define OUTPUT_FILE_NAME    "all.png"
#define PNG_SIGNATURE_SIZE  8 /* number of bytes of png image signature data */
#define CHUNK_LEN_SIZE      4 /* chunk length field size in bytes */          
#define CHUNK_TYPE_SIZE     4 /* chunk type field size in bytes */
#define CHUNK_CRC_SIZE      4 /* chunk CRC field size in bytes */
#define WIDTH_FIELD_SIZE    4 /* number of bytes of the width field */
#define DATA_IHDR_SIZE      13 /* IHDR chunk data field size */

static int createPNGFile(FILE* destinationFile, uint32_t width, uint32_t height,
                  uint8_t* idatData, uint32_t idatDataLength);

// void catPng(char* fileNames[], int fileCount) {
//     FILE* fp = fopen("all.png", "wb");
//     if (fp == NULL) {
//         printf("Failed to open output file\n");
        
//     }
//     if(fileCount > UINT32_MAX || fileCount < 1){
//         printf("Invalid file count");
//     }

//     // Variables to keep track of the maximum width and total height
//     int maxWidth = 0;
//     int totalHeight = 0;

//     uint32_t i;
//     for (i = 0; i < fileCount; i++) {
//         char* fileName = fileNames[i];
//         FILE* inFile = fopen(fileName, "rb");
//         if (inFile == NULL) {
//             printf("Failed to open input file: %s\n", fileName);
//             fclose(fp);
            
//         }

//         // Read the entire file into memory
//         fseek(inFile, 0, SEEK_END);
//         long fileSize = ftell(inFile);
//         rewind(inFile);
//         unsigned char* fileData = (unsigned char*)malloc(fileSize);
//         if (fileData == NULL) {
//             printf("Memory allocation failed\n");
//             fclose(inFile);
//             fclose(fp);
            
//         }
//         if (fread(fileData, 1, fileSize, inFile) != fileSize) {
//             printf("Failed to read file: %s\n", fileName);
//             free(fileData);
//             fclose(inFile);
//             fclose(fp);
            
//         }
//         fclose(inFile);

//         // Check if the file is compressed
//         int isCompressed = (fileData[0] == 0x78 && fileData[1] == 0x9C);

//         if (isCompressed) {
//             // Decompress the data
//             unsigned long destLen = CHUNK;
//             unsigned char* dest = (unsigned char*)malloc(destLen);
//             if (dest == NULL) {
//                 printf("Memory allocation failed\n");
//                 free(fileData);
//                 fclose(fp);
                
//             }

//             z_stream strm;
//             strm.zalloc = Z_NULL;
//             strm.zfree = Z_NULL;
//             strm.opaque = Z_NULL;
//             strm.avail_in = fileSize;
//             strm.next_in = fileData;
//             strm.avail_out = destLen;
//             strm.next_out = dest;

//             if (inflateInit(&strm) != Z_OK) {
//                 printf("Failed to initialize zlib inflate\n");
//                 free(dest);
//                 free(fileData);
//                 fclose(fp);
                
//             }

//             int ret;
//             do {
//                 ret = inflate(&strm, Z_NO_FLUSH);
//                 if (ret == Z_STREAM_ERROR) {
//                     printf("Zlib inflate error\n");
//                     inflateEnd(&strm);
//                     free(dest);
//                     free(fileData);
//                     fclose(fp);
                    
//                 }

//                 if (ret == Z_OK || ret == Z_STREAM_END) {
//                     if (fwrite(dest, 1, destLen - strm.avail_out, fp) != destLen - strm.avail_out) {
//                         printf("Failed to write uncompressed data to file\n");
//                         inflateEnd(&strm);
//                         free(dest);
//                         free(fileData);
//                         fclose(fp);
                        
//                     }
//                 }

//                 if (ret == Z_NEED_DICT || ret == Z_DATA_ERROR || ret == Z_MEM_ERROR) {
//                     printf("Failed to uncompress data from file: %s\n", fileName);
//                     inflateEnd(&strm);
//                     free(dest);
//                     free(fileData);
//                     fclose(fp);
                    
//                 }

//                 strm.avail_out = destLen;
//                 strm.next_out = dest;
//             } while (ret != Z_STREAM_END);

//             inflateEnd(&strm);
//             free(dest);
//         } else {
//             // Write the uncompressed data directly to the output file
//             if (fwrite(fileData, 1, fileSize, fp) != fileSize) {
//                 printf("Failed to write uncompressed data to file\n");
//                 free(fileData);
//                 fclose(fp);
                
//             }
//         }

//         free(fileData);

//         // Update the maximum width and total height
//         if (i == 0) {
//             // For the first image, set the maximum width
//             maxWidth = fileData[16] * 256 + fileData[17];
//         }
//         totalHeight += fileData[20] * 256 + fileData[21];
//     }

//     // Update the IHDR chunk for the concatenated image
//     fseek(fp, 16, SEEK_SET);
//     unsigned char widthBytes[4];
//     widthBytes[0] = (unsigned char)(maxWidth >> 24);
//     widthBytes[1] = (unsigned char)(maxWidth >> 16);
//     widthBytes[2] = (unsigned char)(maxWidth >> 8);
//     widthBytes[3] = (unsigned char)(maxWidth);
//     fwrite(widthBytes, 1, 4, fp);
//     fseek(fp, 20, SEEK_SET);
//     unsigned char heightBytes[4];
//     heightBytes[0] = (unsigned char)(totalHeight >> 24);
//     heightBytes[1] = (unsigned char)(totalHeight >> 16);
//     heightBytes[2] = (unsigned char)(totalHeight >> 8);
//     heightBytes[3] = (unsigned char)(totalHeight);
//     fwrite(heightBytes, 1, 4, fp);

//     fclose(fp);
    
//         FILE *outputFile = fopen("all.png", "wb");
//     if (outputFile == NULL)
//     {
//         printf("Failed to open output file\n");
        
//     }

//     unsigned long crc = 0xFFFFFFFFL;
//     unsigned long newCrc;

//     // Iterate over the input files
//     for (int i = 0; i < fileCount; i++)
//     {
//         FILE *currentFile = fopen(fileNames[i], "rb");
//         if (currentFile == NULL)
//         {
//             printf("Failed to open file: %s\n", fileNames[i]);
//             fclose(outputFile);
            
//         }

//         unsigned char buf[CHUNK];
//         unsigned long bytesRead;

//         // Read and write the file data
//         while ((bytesRead = fread(buf, 1, CHUNK, currentFile)) > 0)
//         {
//             if (fwrite(buf, 1, bytesRead, outputFile) != bytesRead)
//             {
//                 printf("Failed to write data to the output file\n");
//                 fclose(outputFile);
//                 fclose(currentFile);
                
//             }
//         }

//         newCrc = calculateCrc(buf, bytesRead);
//         crc = crc32(crc, buf, bytesRead);
//         printf("CRC for %s: 0x%lx\n", fileNames[i], newCrc);

//         fclose(currentFile);
//     }

//     crc = crc ^ 0xFFFFFFFFL;
//     printf("Final CRC: 0x%lx\n", crc);

//     // Append the CRC bytes to the output file
//     unsigned char crcBytes[4];
//     crcBytes[0] = (crc >> 24) & 0xFF;
//     crcBytes[1] = (crc >> 16) & 0xFF;
//     crcBytes[2] = (crc >> 8) & 0xFF;
//     crcBytes[3] = crc & 0xFF;

//     if (fwrite(crcBytes, sizeof(unsigned char), 4, outputFile) != 4)
//     {
//         printf("Failed to write CRC bytes to the output file\n");
//         fclose(outputFile);
        
//     }

//     fclose(outputFile);

    
// }

int catPng(char* fileNames[], int fileCount) {
    FILE* outputFile = fopen(OUTPUT_FILE_NAME, "wb");
    if (outputFile == NULL) {
        printf("Failed to open output file\n");
        
    }
    if(fileCount > UINT32_MAX || fileCount < 1){
        printf("Invalid file count");
    }

    uint32_t data = 0;
    uint32_t totalHeight = 0;
    uint64_t uncompressedDataLen = 0;

    // read the width field
    FILE *firstFile = fopen(fileNames[0], "rb");
    if (firstFile == NULL) {
        printf("Failed to open the destination file.\n");
        fclose(firstFile);
        return 1;
    }

    fseek(firstFile, PNG_SIGNATURE_SIZE + CHUNK_LEN_SIZE + CHUNK_TYPE_SIZE, SEEK_SET);

    // Read the width from the source file
    if (fread(&data, sizeof(uint32_t), 1, firstFile) != 1) {
        printf("Failed to read the uint32_t from the source file.\n");
        fclose(firstFile);
        fclose(outputFile);
        return 1;
    }

    data = ntohl(data);
    printf("width is %u\n", data);

    uint8_t *idatUncompressedData = NULL;

    for(uint32_t i = 0; i < fileCount; ++i){
        char *currentFilePath = fileNames[i];
        uint32_t pngHeight;
        FILE* currentFile = fopen(currentFilePath, "rb");

        // move the file pointer to be at the beginning of the height info
        fseek(currentFile, PNG_SIGNATURE_SIZE + CHUNK_LEN_SIZE + CHUNK_TYPE_SIZE + WIDTH_FIELD_SIZE, SEEK_SET);

        // Read the integer from the file
        if (fread(&pngHeight, sizeof(uint32_t), 1, currentFile) != 1) {
            printf("Failed to read the integer from the file. 1\n");
            fclose(currentFile);
            fclose(outputFile);
            return 1;
        }

        // Convert the integer from network byte order to host byte order
        pngHeight = ntohl(pngHeight);

        totalHeight += pngHeight;

        uint32_t currentFileDataLen = 0;

        // Move file pointer to the IDAT data length field for the current file
        fseek(currentFile, PNG_SIGNATURE_SIZE + CHUNK_LEN_SIZE + CHUNK_TYPE_SIZE + DATA_IHDR_SIZE + CHUNK_CRC_SIZE, SEEK_SET);

        if(fread(&currentFileDataLen, sizeof(uint32_t), 1, currentFile) != 1){
            printf("Failed to read the integer from the file. 2\n");
            fclose(currentFile);
            fclose(outputFile); 
            return 1;
        }

        currentFileDataLen = ntohl(currentFileDataLen);

        // Move the file pointer to the end of the file
        fseek(currentFile, 0, SEEK_END);

        // Get the current position of the file pointer (which is the file size)
        printf("file size is %ld\n", ftell(currentFile));

        // skip over the chunk type field to get to the data field
        fseek(currentFile, PNG_SIGNATURE_SIZE + 2*CHUNK_LEN_SIZE + 2*CHUNK_TYPE_SIZE + DATA_IHDR_SIZE + CHUNK_CRC_SIZE, SEEK_SET);

        uint8_t *currentFileCompressedData = malloc(currentFileDataLen); 
        
        if(fread(currentFileCompressedData, sizeof(uint8_t), currentFileDataLen, currentFile) != currentFileDataLen){
            printf("Failed to read the integer from the file. 3\n");
            fclose(currentFile);
            fclose(outputFile);
            return 1;
        }

        uint64_t bytesWritten = 0;
        uint8_t *currentFileUncompressedData = malloc(pngHeight*((data*4)+ 1));
        memset(currentFileUncompressedData, 0, pngHeight*((data*4)+ 1));

        printf("currentFileDataLen: %u\n", currentFileDataLen);

        if(mem_inf(currentFileUncompressedData, &bytesWritten, currentFileCompressedData, (uint64_t) currentFileDataLen) != 0){
            printf("could not uncompress the data\n");
            fclose(currentFile);
            fclose(outputFile);
            return 1;
        }

        idatUncompressedData = realloc(idatUncompressedData, uncompressedDataLen + bytesWritten);
        memcpy(idatUncompressedData, currentFileUncompressedData, uncompressedDataLen + bytesWritten);

        uncompressedDataLen += bytesWritten;

        free(currentFileCompressedData);
        free(currentFileUncompressedData);

        if(fclose(currentFile) != 0){
            printf("failed to close the file\n");
            fclose(outputFile);
            return 1;
        }
    }
    
    uint8_t *idatCompressedData = malloc(uncompressedDataLen);
    uint64_t idatCompressedDataLen = 0;
    mem_def(idatCompressedData, &idatCompressedDataLen, idatUncompressedData, uncompressedDataLen, Z_DEFAULT_COMPRESSION);
    if(createPNGFile(outputFile, data, totalHeight, idatCompressedData, idatCompressedDataLen) != 0){
        printf("Could not create the PNG file\n");
        fclose(outputFile);
        return 1;
    }

    if(fclose(outputFile) != 0){
        printf("could not close %s", OUTPUT_FILE_NAME);
        return 1;
    }
    return 0;
}

// Function to create a PNG file by writing a signature and copying over the chunk info from a source file
static int createPNGFile(FILE* destinationFile, uint32_t width, uint32_t height,
                  uint8_t* idatData, uint32_t idatDataLength) {


    if (destinationFile == NULL) {
        printf("Failed to open the destination file.\n");
        
        return 1;
    }

    // Write PNG signature
    const unsigned char pngSignature[8] = { 137, 80, 78, 71, 13, 10, 26, 10 };
    if (fwrite(pngSignature, sizeof(unsigned char), 8, destinationFile) != 8) {
        printf("Failed to write the PNG signature to the destination file.\n");
        
        fclose(destinationFile);
        return 1;
    }

    // Write IHDR chunk
    uint32_t ihdrLength = 13;
    uint32_t ihdrType = 0x49484452;  // ASCII code for "IHDR"

    // Prepare IHDR chunk data
    unsigned char ihdrData[13];
    ihdrData[0] = (width >> 24) & 0xFF;
    ihdrData[1] = (width >> 16) & 0xFF;
    ihdrData[2] = (width >> 8) & 0xFF;
    ihdrData[3] = width & 0xFF;
    ihdrData[4] = (height >> 24) & 0xFF;
    ihdrData[5] = (height >> 16) & 0xFF;
    ihdrData[6] = (height >> 8) & 0xFF;
    ihdrData[7] = height & 0xFF;
    ihdrData[8] = 8;  // Bit depth of 8
    ihdrData[9] = 6;  // Colour type of 6 (RGBA image)
    ihdrData[10] = 0; // Compression method of 0
    ihdrData[11] = 0; // Filter method of 0
    ihdrData[12] = 0; // Interlace method of 0

    // Write IHDR chunk length
    uint32_t ihdrLengthNBO = htonl(ihdrLength);
    if (fwrite(&ihdrLengthNBO, sizeof(uint32_t), 1, destinationFile) != 1) {
        printf("Failed to write the IHDR chunk length to the destination file.\n");
        
        fclose(destinationFile);
        return 1;
    }

    // Write IHDR chunk type
    uint32_t ihdrTypeNBO = htonl(ihdrType);
    if (fwrite(&ihdrTypeNBO, sizeof(uint32_t), 1, destinationFile) != 1) {
        printf("Failed to write the IHDR chunk type to the destination file.\n");
        
        fclose(destinationFile);
        return 1;
    }

    // Write IHDR chunk data
    if (fwrite(ihdrData, sizeof(unsigned char), ihdrLength, destinationFile) != ihdrLength) {
        printf("Failed to write the IHDR chunk data to the destination file.\n");
        
        fclose(destinationFile);
        return 1;
    }

    // Recalculate and write IHDR chunk CRC
    unsigned long ihdrCrc = calculateCrc(ihdrData, ihdrLength);
    uint32_t ihdrCrcNBO = htonl(ihdrCrc);

    if (fwrite(&ihdrCrcNBO, sizeof(uint32_t), 1, destinationFile) != 1) {
        printf("Failed to write the IHDR chunk CRC to the destination file.\n");
        
        fclose(destinationFile);
        return 1;
    }

    // Write IDAT chunk
    uint32_t idatDataLengthNBO = htonl(idatDataLength);
    uint32_t idatType = 0x49444154;  // ASCII code for "IDAT"

    // Write IDAT chunk length
    if (fwrite(&idatDataLengthNBO, sizeof(uint32_t), 1, destinationFile) != 1) {
        printf("Failed to write the IDAT chunk length to the destination file.\n");
        
        fclose(destinationFile);
        return 1;
    }

    // Write IDAT chunk type
    if (fwrite(&idatType, sizeof(uint32_t), 1, destinationFile) != 1) {
        printf("Failed to write the IDAT chunk type to the destination file.\n");
        
        fclose(destinationFile);
        return 1;
    }

    // Write IDAT chunk data
    if (fwrite(idatData, sizeof(unsigned char), idatDataLength, destinationFile) != idatDataLength) {
        printf("Failed to write the IDAT chunk data to the destination file.\n");
        
        fclose(destinationFile);
        return 1;
    }

    // Recalculate and write IDAT chunk CRC
    unsigned long idatCrc = calculateCrc(idatData, idatDataLength);
    uint32_t idatCrcNBO = htonl(idatCrc);

    if (fwrite(&idatCrcNBO, sizeof(uint32_t), 1, destinationFile) != 1) {
        printf("Failed to write the IDAT chunk CRC to the destination file.\n");
        
        fclose(destinationFile);
        return 1;
    }

    // Write IEND chunk
    uint32_t iendLength = 0;
    uint32_t iendType = 0x49454E44;  // ASCII code for "IEND"

    // Write IEND chunk length
    if (fwrite(&iendLength, sizeof(uint32_t), 1, destinationFile) != 1) {
        printf("Failed to write the IEND chunk length to the destination file.\n");
        
        fclose(destinationFile);
        return 1;
    }

    // Write IEND chunk type
    if (fwrite(&iendType, sizeof(uint32_t), 1, destinationFile) != 1) {
        printf("Failed to write the IEND chunk type to the destination file.\n");
        
        fclose(destinationFile);
        return 1;
    }

    printf("PNG file created successfully.\n");

    return 0;
}