#include "../include/f_util.h"
#include "../include/png.h"

#include <stdbool.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>

/**
 * @brief checks if a given file is a real png file
 *
 * @param fd - file descriptor of the file to check
 *
 * @return bool - true if it is a png file otherwise false
*/
bool pngCheck(char *path){
    if(path == NULL){
        perror("invalid path\n");
        return false;
    }
    FILE *fd = fopen(path, "r");
    if(fd == NULL){
        perror("Error: could not open the file\n");
        return false;
    }
    unsigned char valid_header[] = { PNG_HEADER_BYTE_ONE,   \
                                     PNG_HEADER_BYTE_TWO,   \
                                     PNG_HEADER_BYTE_THREE, \
                                     PNG_HEADER_BYTE_FOUR,  \
                                     PNG_HEADER_BYTE_FIVE,  \
                                     PNG_HEADER_BYTE_SIX,   \
                                     PNG_HEADER_BYTE_SEVEN, \
                                     PNG_HEADER_BYTE_EIGHT };
    unsigned char bytes[5*PNG_HEADER_SIZE];
    if(fread(bytes, PNG_HEADER_SIZE, 1, fd) < 0){
        perror("Could not read from the file!");
        return false;
    }
    for(int i = 0; i < PNG_HEADER_SIZE; ++i){
        if(bytes[i] != valid_header[i]){
            fclose(fd);
            return false;
        }
    }
    fclose(fd);
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
 * @param dirPath - directory to check
*/
void directoryPngCheck(char *dirPath, bool *pngFound){
    DIR *p_dir;
    struct dirent *p_dirent;

    if (dirPath == NULL) {
        perror("No directory specified\n");
        exit(1);
    }

    if ((p_dir = opendir(dirPath)) == NULL) {;
        perror("Unable to open Directory!\n");
        exit(2);
    }

    while ((p_dirent = readdir(p_dir)) != NULL) {
        char *relPath = p_dirent->d_name;  /* relative path name */

        if (strcmp(relPath, ".") == 0 || strcmp(relPath, "..") == 0) {
            continue;
        }
        //printf("relative path is %s\n", relPath);

        char fullPath[PATH_MAX];  // Array to hold the complete path
        snprintf(fullPath, sizeof(fullPath), "%s/%s", dirPath, relPath);

        if (relPath == NULL) {
            fprintf(stderr,"Null pointer found!"); 
            exit(3);
        }
        else if (fileType(fullPath) == FILE_TYPE_REGULAR){
            if(pngCheck(fullPath)){
                printf("%s\n", fullPath);
                *pngFound = true;
            }
        }
        else if (fileType(fullPath) == FILE_TYPE_DIRECTORY){
            directoryPngCheck(fullPath, pngFound);
        }
    }

    if ( closedir(p_dir) != 0 ) {
        perror("closedir");
        exit(3);
    }
}