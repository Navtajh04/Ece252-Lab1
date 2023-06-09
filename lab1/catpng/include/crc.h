/**
 * @file: crc.h
 * @brief: crc calculation functions for PNG file
 */

#pragma once

void makeCrcTable(void);
unsigned long updateCrc(unsigned long crc, unsigned char *buf, int len);
unsigned long crc(unsigned char *buf, int len);
unsigned long calculateCrc(unsigned char *buf, int len);