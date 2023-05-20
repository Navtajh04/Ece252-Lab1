#ifndef INCLUDE_FTYPE_H
#define INCLUDE_FTYPE_H

typedef enum {
    FILE_TYPE_REGULAR = 0,
    FILE_TYPE_DIRECTORY,
    FILE_TYPE_SYMBOLIC_LINK,
    FILE_TYPE_OTHER
} fileType_t;

/**
 * @brief checks if a given file is a real png file
 *
 * @param fd - file descriptor of the file to check
 *
 * @return bool - true if it is a png file otherwise false
*/
bool pngCheck(char *fd);

fileType_t 

#endif /* INCLUDE_FTYPE_H */
