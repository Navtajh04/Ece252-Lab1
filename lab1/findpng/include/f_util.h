#ifndef INCLUDE_FTYPE_H
#define INCLUDE_FTYPE_H

#include <stdbool.h>

typedef enum {
    FILE_TYPE_REGULAR = 0,
    FILE_TYPE_DIRECTORY,
    FILE_TYPE_OTHER,
    FILE_TYPE_INVALID
} fileType_t;

/**
 * @brief checks if a given file is a real png file
 *
 * @param absPath - the absolute path to the file to check
 *
 * @return bool - true if it is a png file otherwise false
*/
bool pngCheck(char *absPath);

/**
 * @brief checks the filetype of a given file
 *
 * @param absPath - absolute path for the file to check
 *
 * @return fileType_t - returns what type of file the file is
*/
fileType_t fileType(char *absPath);

/**
 * @brief traverses a directory and all its subdirectories and prints all png files
 * 
 * @param dirPath - directory to check
 * @param pngFound - pointer to a boolean that stores whether or not a png exists in the directory
*/
void directoryPngCheck(char *dirPath, bool *pngFound);

#endif /* INCLUDE_FTYPE_H */
