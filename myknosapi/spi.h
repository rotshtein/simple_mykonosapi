#pragma once
#ifndef _SPI_H
#define _SPI_H

#define SPI_TRAMSIVER	1
#define SPI_CLOCKS		2

void spi_build_buffer(unsigned short addr, char data, char *buf);
void spi_write(unsigned short addr, char data);
char spi_read(unsigned short addr);
#endif