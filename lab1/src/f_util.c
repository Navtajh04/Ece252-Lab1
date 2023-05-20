#include "../include/f_util.h"
#include "../include/png.h"

#include <stdbool.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/**
 * @brief checks if a given file is a real png file
 *
 * @param fd - file descriptor of the file to check
 *
 * @return bool - true if it is a png file otherwise false
*/
bool pngCheck(char *path){
    FILE *fd = fopen(path, 'r');
    unsigned char valid_header[] = { PNG_HEADER_BYTE_ONE,   \
                                     PNG_HEADER_BYTE_TWO,   \
                                     PNG_HEADER_BYTE_THREE, \
                                     PNG_HEADER_BYTE_FOUR,  \
                                     PNG_HEADER_BYTE_FIVE,  \
                                     PNG_HEADER_BYTE_SIX,   \
                                     PNG_HEADER_BYTE_SEVEN, \
                                     PNG_HEADER_BYTE_EIGHT };
    unsigned char bytes[PNG_HEADER_SIZE];
    if(fread(bytes, PNG_HEADER_SIZE, fd, NULL) < 0){
        perror("Could not read from the file!");
        return false;
    }
    for(int i = 0; i < PNG_HEADER_SIZE; ++i){
        if(bytes[i] != valid_header[i]){
            return false;
        }
    }
    return true;
}

/**
 * @brief checks the filetype of a given file
 *
 * @param fd - file descriptor of the file to check
 *
 * @return fileType_t - returns what type of file the file is
*/
fileType_t fileType(char *fd){
    struct stat buf;
    if (lstat(fd, &buf) < 0) {
        perror("lstat error");
        return FILE_TYPE_INVALID;
    }
    if (S_ISREG(buf.st_mode)){
        return FILE_TYPE_REGULAR;
    }
    else if (S_ISDIR(buf.st_mode)){
        return FILE_TYPE_DIRECTORY;
    }
    else{
        return FILE_TYPE_OTHER;
    }
}

/**
 * @brief traverses a directory and all its subdirectories and prints all png files
 * 
 * @param fd - directory to check
*/
void directoryPngCheck(char *fd){

}