#ifndef CATPNG_INCLUDE_CRC_H
#define CATPNG_INCLUDE_CRC_H

void createCrcTable(void);
unsigned long updateCrc(unsigned long crc, unsigned char *buf, int len);
unsigned long calculateCrc(unsigned char *buf, int len);

#endif /* CATPNG_INCLUDE_CRC_H */