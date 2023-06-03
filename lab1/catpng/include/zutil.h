#ifndef CATPNG_INCLUDE_ZUTIL_H
#define CATPNG_INCLUDE_ZUTIL_H

/* INCLUDES */
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>
#include "zlib.h"

/* DEFINES */
#if defined(MSDOS) || defined(OS2) || defined(WIN32) || defined(__CYGWIN__)
#  include <fcntl.h>
#  include <io.h>
#  define SET_BINARY_MODE(file) setmode(fileno(file), O_BINARY)
#else
#  define SET_BINARY_MODE(file)
#endif

#define CHUNK 16384  /* =256*64 on the order of 128K or 256K should be used */

/* FUNCTION PROTOTYPES */
int mem_def(uint8_t *dest, uint64_t *dest_len, uint8_t *source,  uint64_t source_len, int level);
int mem_inf(uint8_t *dest, uint64_t *dest_len, uint8_t *source,  uint64_t source_len);
void zerr(int ret);

#endif /* CATPNG_INCLUDE_ZUTIL_H */