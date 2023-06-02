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
 * @param absPath - the absolute path to the file to check
 *
 * @return bool - true if it is a png file otherwise false
*/
bool pngCheck(char *absPath){
    // make sure the argument provided is valid
    if(absPath == NULL){
        perror("invalid path\n");
        return false;
    }
    // open the file and store the file pointer in f
    FILE *f = fopen(absPath, "r");
    // Check if the file was successfully opened
    if(f == NULL){
        perror("Error: could not open the file\n");
        return false;
    }

    // array of first eight bytes in a real png file
    unsigned char valid_header[] = { PNG_HEADER_BYTE_ONE,   \
                                     PNG_HEADER_BYTE_TWO,   \
                                     PNG_HEADER_BYTE_THREE, \
                                     PNG_HEADER_BYTE_FOUR,  \
                                     PNG_HEADER_BYTE_FIVE,  \
                                     PNG_HEADER_BYTE_SIX,   \
                                     PNG_HEADER_BYTE_SEVEN, \
                                     PNG_HEADER_BYTE_EIGHT };
    // array to hold the bytes read from the file to be checked
    unsigned char bytes[PNG_HEADER_SIZE];

    // read PNG_HEADER_SIZE bytes from the file and check if there was an error
    if(fread(bytes, PNG_HEADER_SIZE, 1, f) < 0){
        perror("Could not read from the file!");
        return false;
    }
    // iterate through te first PNG_HEADER_SIZE bytes read from the file and make sure they match the expected values
    for(int i = 0; i < PNG_HEADER_SIZE; ++i){
        if(bytes[i] != valid_header[i]){
            fclose(f);
            return false;
        }
    }
    // close the file
    fclose(f);
    return true;
}

/**
 * @brief checks the filetype of a given file
 *
 * @param absPath - absolute path for the file to check
 *
 * @return fileType_t - returns what type of file the file is
*/
fileType_t fileType(char *absPath){
    struct stat buf;
    if (lstat(absPath, &buf) < 0) {
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
 * @param pngFound - pointer to a boolean that stores whether or not a png exists in the directory
*/
void directoryPngCheck(char *dirPath, bool *pngFound){
    DIR *p_dir;
    struct dirent *p_dirent;

    // make sure the path to the directory exists
    if (dirPath == NULL) {
        perror("No directory specified\n");
        exit(1);
    }

    // open the directory and exit if there is an error
    if ((p_dir = opendir(dirPath)) == NULL) {;
        perror("Unable to open Directory!\n");
        exit(2);
    }

    // continuously read the directory
    while ((p_dirent = readdir(p_dir)) != NULL) {
        char *relPath = p_dirent->d_name;  /* relative path name */

        if (strcmp(relPath, ".") == 0 || strcmp(relPath, "..") == 0) {
            continue;
        }

        char absPath[PATH_MAX];  // Array to hold the complete path

        // concatenate the directory path with the relative path to create the absolute path
        snprintf(absPath, sizeof(absPath), "%s/%s", dirPath, relPath);

        // make sure relPath was valid
        if (relPath == NULL) {
            fprintf(stderr,"Null pointer found!"); 
            exit(3);
        }
        else if (fileType(absPath) == FILE_TYPE_REGULAR){
            // if it was a regular file, check if it is a png file
            if(pngCheck(absPath)){
                // print out the path to the png file
                printf("%s\n", absPath);
                // set *pngFound to true so that the calling function knows that a png exists in the directory
                *pngFound = true;
            }
        }  
        else if (fileType(absPath) == FILE_TYPE_DIRECTORY){  // if the file is a directory, parse through that sub directory
            directoryPngCheck(absPath, pngFound);
        }
    }

    if ( closedir(p_dir) != 0 ) {
        perror("closedir");
        exit(3);
    }
}