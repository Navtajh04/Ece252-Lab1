/********************************************************************
 * @file: crc.c
 * @brief: PNG crc calculation
 * Reference: https://www.w3.org/TR/PNG-CRCAppendix.html
 */
#include "../include/crc.h"

/* Table of CRCs of all 8-bit messages. */
unsigned long crcTable[256];

/* Flag: has the table been computed? Initially false. */
int createdCrcTable = 0;

/* Make the table for a fast CRC. */
void createCrcTable(void)
{
    unsigned long c;
    int n, k;

    for (n = 0; n < 256; n++) {
        c = (unsigned long) n;
        for (k = 0; k < 8; k++) {
            if (c & 1)
                c = 0xedb88320L ^ (c >> 1);
            else
                c = c >> 1;
        }
        crcTable[n] = c;
    }
    createdCrcTable = 1;
}

/* Update a running CRC with the bytes buf[0..len-1]--the CRC
   should be initialized to all 1's, and the transmitted value
   is the 1's complement of the final running CRC (see the
   crc() routine below)). */

unsigned long updateCrc(unsigned long crc, unsigned char *buf, int len)
{
    unsigned long c = crc;
    int n;

    if (!createdCrcTable)
        createCrcTable();
    for (n = 0; n < len; n++) {
        c = crcTable[(c ^ buf[n]) & 0xff] ^ (c >> 8);
    }
    return c;
}

/* Return the CRC of the bytes buf[0..len-1]. */
unsigned long crc(unsigned char *buf, int len)
{
    return updateCrc(0xffffffffL, buf, len) ^ 0xffffffffL;
}

unsigned long calculateCrc(unsigned char *buf, int len) {
    return updateCrc(0xFFFFFFFFL, buf, len) ^ 0xFFFFFFFFL;
}