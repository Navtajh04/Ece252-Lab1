/**
 * @file: crc.h
 * @brief: crc calculation functions for PNG file
 */

#pragma once

void make_crc_table(void);
unsigned long updateCrc(unsigned long crc, unsigned char *buf, int len);
unsigned long crc(unsigned char *buf, int len);
unsigned long calculateCrc(unsigned char *buf, int len);