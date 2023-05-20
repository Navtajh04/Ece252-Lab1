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
 * @param fd - file descriptor of the file to check
 *
 * @return bool - true if it is a png file otherwise false
*/
bool pngCheck(char *fd);

/**
 * @brief checks the filetype of a given file
 *
 * @param fd - file descriptor of the file to check
 *
 * @return fileType_t - returns what type of file the file is
*/
fileType_t fileType(char *fd);

/**
 * @brief traverses a directory and all its subdirectories and prints all png files
 * 
 * @param fd - directory to check
*/
void DirectoryPngCheck(char *fd);



#endif /* INCLUDE_FTYPE_H */
