#include "../include/zutil.h"

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>

#include "zlib.h"

#define CHUNK 16384  /* =256*64 on the order of 128K or 256K should be used */

/**
 * @brief compresses an array of bytes to be stored in a PNG file
 * 
 * @param dest buffer to hold the compressed bytes
 * @param destLen total number of bytes written to the dest buffer
 * @param source buffer containing the data that needs to be compressed
 * @param sourceLen length of the source buffer
 * @param level compression level that should be used
 * 
 * @return int - 0 if successful
*/
int memDef(uint8_t *dest, uint64_t *destLen, uint8_t *source,  uint64_t sourceLen, int level)
{
    z_stream strm;    /* pass info. to and from zlib routines   */
    uint8_t out[CHUNK];    /* output buffer for deflate()            */
    int ret = 0;      /* zlib return code                       */
    int have = 0;     /* amount of data returned from deflate() */
    int def_len = 0;  /* accumulated deflated data length       */
    uint8_t *p_dest = dest;/* first empty slot in dest buffer        */

    
    strm.zalloc = Z_NULL;
    strm.zfree  = Z_NULL;
    strm.opaque = Z_NULL;

    ret = deflateInit(&strm, level);
    if (ret != Z_OK) {
        return ret;
    }

    /* set input data stream */
    strm.avail_in = sourceLen;
    strm.next_in = source;

    /* call deflate repetitively since the out buffer size is fixed
       and the deflated output data length is not known ahead of time */

    do {
        strm.avail_out = CHUNK;
        strm.next_out = out;
        ret = deflate(&strm, Z_FINISH); /* source contains the whole data */
        assert(ret != Z_STREAM_ERROR);
        have = CHUNK - strm.avail_out; 
        memcpy(p_dest, out, have);
        p_dest += have;  /* advance to the next free byte to write */
        def_len += have; /* increment deflated data length         */
    } while (strm.avail_out == 0);

    assert(strm.avail_in == 0);   /* all input will be used  */
    assert(ret == Z_STREAM_END);  /* stream will be complete */

    /* clean up and return */
    (void) deflateEnd(&strm);
    *destLen = def_len;
    return Z_OK;
}

/**
 * @brief: inflate in memory data from source to dest 
 * @param: dest uint8_t* output buffer, caller supplies, should be big enough
 *         to hold the deflated data
 * @param: dest_len, uint64_t* output parameter, length of inflated data
 * @param: source uint8_t* source buffer, contains zlib data to be inflated
 * @param: source_len uint64_t length of source data
 * 
 * @return =0  on success
 *         <>0 error
 */
int memInf(uint8_t *dest, uint64_t *destLen, uint8_t *source,  uint64_t sourceLen)
{
    z_stream strm;    /* pass info. to and from zlib routines   */
    uint8_t out[CHUNK];    /* output buffer for inflate()            */
    int ret = 0;      /* zlib return code                       */
    int have = 0;     /* amount of data returned from inflate() */
    int inf_len = 0;  /* accumulated inflated data length       */
    uint8_t *p_dest = dest;/* first empty slot in dest buffer        */

    /* allocate inflate state 8 */
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    strm.avail_in = 0;        /* no input data being provided   */
    strm.next_in = Z_NULL;    /* no input data being provided   */
    ret = inflateInit(&strm);
    if (ret != Z_OK) {
        return ret;
    }

    /* set input data stream */
    strm.avail_in = sourceLen;
    strm.next_in = source;

    /* run inflate() on input until output buffer not full */
    do {
        strm.avail_out = CHUNK;
        strm.next_out = out;

        /* zlib format is self-terminating, no need to flush */
        ret = inflate(&strm, Z_NO_FLUSH);
        if(ret == Z_STREAM_ERROR){
            zerr(ret);
            return ret;
        }
        switch(ret) {
        case Z_NEED_DICT:
            ret = Z_DATA_ERROR;  /* and fall through */
        case Z_DATA_ERROR:
        case Z_MEM_ERROR:
            (void) inflateEnd(&strm);
			return ret;
        }
        have = CHUNK - strm.avail_out;
        memcpy(p_dest, out, have);
        p_dest += have;  /* advance to the next free byte to write */
        inf_len += have; /* increment inflated data length         */
    } while (strm.avail_out == 0 );

    /* clean up and return */
    (void) inflateEnd(&strm);
    *destLen = inf_len;
    
    return (ret == Z_STREAM_END) ? Z_OK : Z_DATA_ERROR;
}

/**
 * @brief reports a zlib or i/o error
 * 
 * @param ret the error number
*/
void zerr(int ret)
{
    fputs("zutil: ", stderr);
    switch (ret) {
    case Z_STREAM_ERROR:
        fputs("invalid compression level\n", stderr);
        break;
    case Z_DATA_ERROR:
        fputs("invalid or incomplete deflate data\n", stderr);
        break;
    case Z_MEM_ERROR:
        fputs("out of memory\n", stderr);
        break;
    case Z_VERSION_ERROR:
        fputs("zlib version mismatch!\n", stderr);
    default:
	fprintf(stderr, "zlib returns err %d!\n", ret);
    }
}
