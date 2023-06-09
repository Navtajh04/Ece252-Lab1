#ifndef CATPNG_INCLUDE_ZUTIL_H
#define CATPNG_INCLUDE_ZUTIL_H

/* INCLUDES */
#include <stdint.h>

/* DEFINES */
#if defined(MSDOS) || defined(OS2) || defined(WIN32) || defined(__CYGWIN__)
#  include <fcntl.h>
#  include <io.h>
#  define SET_BINARY_MODE(file) setmode(fileno(file), O_BINARY)
#else
#  define SET_BINARY_MODE(file)
#endif

/* FUNCTION PROTOTYPES */
int memDef(uint8_t *dest, uint64_t *destLen, uint8_t *source,  uint64_t sourceLen, int level);
int memInf(uint8_t *dest, uint64_t *destLen, uint8_t *source,  uint64_t sourceLen);
void zerr(int ret);

#endif /* CATPNG_INCLUDE_ZUTIL_H */