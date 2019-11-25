#include "spi.h"

void spi_build_buffer(unsigned short addr, char data, char *buf)
{
	buf[0] = (char)((addr >> 8) & 0xff);
	buf[1] = (char)(addr & 0xff);
	buf[2] = data;
}

void spi_write(unsigned short addr, char data)
{
	char buf[3];
	spi_build_buffer(addr, data, buf);
	HAL_spiWrite(buf, sizeof(buf));
}

char spi_read(unsigned short addr)
{
	char buf[3];
	unsigned int data;
	addr |= 0x8000;
	spi_build_buffer(addr, 0, buf);
	HAL_spiRead(buf, sizeof(buf), &data);

	return (char)(data & 0xff);

}