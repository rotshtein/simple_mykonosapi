#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#define __USE_MISC
#include <sys/time.h>

#include "HAL.h"
#include "fpga.h"
#include "spi.h"


static 	FILE *pLogFile = NULL;
static struct timeval  _tv;


/************************************************  SPI ***************************************************/
int HAL_initSpi(int chipSelectIndex, unsigned char CPOL_CPHA, int spiClkFreq_Hz)
{
	if ((chipSelectIndex == 0) || (chipSelectIndex > 3))
	{
		return 1;
	}

	unsigned int reg_54_state;
	fpga_read(SPI_CHIP_SELECT, &reg_54_state);
	char buf[3];
	if (chipSelectIndex & SPI_CLOCKS)
	{
		fpga_write(SPI_CHIP_SELECT, 2);

		//fpga_write(0x44, 0x99);		// Assert Soft Reset, Set 4wire SPI Mode
		spi_write(0, 0x99);
		
		//fpga_write(0x44, 0xF01);	// Commit Write
		spi_write(0x44, 1);
		
		//fpga_write(0x44, 0x18);		// Deassert soft Reset, Set 4wire SPI Mode
		spi_write(0x00, 0x18);

		//fpga_write(0x44, 0xF01);	// Commit Write
		spi_write(0x0F, 0x1);
	}

	if (chipSelectIndex & SPI_TRAMSIVER)
	{
		fpga_write(SPI_CHIP_SELECT, 1);

		//fpga_write(0x44, 0x18);		// Set 4wire SPI Mode
		spi_write(0x00, 0x18);

		//fpga_write(0x44, 0x180);	// Set Single Instruction Mode
		spi_write(0x01, 0x80);


	}

	fpga_write(SPI_CHIP_SELECT, reg_54_state);

	return 0;
}

void HAL_closeSpi()
{}

int HAL_spiWrite(char *txbuf, int len)
{
	if ((len % 3) != 0)
	{
		HAL_writeToLogFile("Error SPI data len [%d]\n", len);
		return 1;
	}

	unsigned int status = 0;
	while (len)
	{
		while (!(status & (1 << 6)))
		{
			fpga_read(0x48, &status);
			usleep(10);
		}
		char data[4] = { 0 };
		data[2] = *txbuf++;
		data[1] = *txbuf++;
		data[0] = *txbuf++;
		
		fpga_write(0x44, *((unsigned int *)data));

		len-=3;
	}
	return 0;
}

int HAL_spiRead(char *txbuf, int len, char *data)
{
	unsigned int status = 0;
	txbuf[0] |= 0x80;
	HAL_spiWrite(txbuf, 3);
	while (!(status & (1 << 7)))
	{
		fpga_read(0x48, &status);
		usleep(10);
	}
	fpga_read(0x40, (unsigned int *)data);
}

/************************************************ Log ***************************************************/
void HAL_openLogFile(char *filename)
{
	if (pLogFile != NULL)
	{
		fclose(pLogFile);
	}

	pLogFile = fopen(filename, "at");

	if (!pLogFile) pLogFile = fopen(filename, "wt");
	if (!pLogFile)
	{
		printf("can not open %s for writing.\n", filename);
	}
	time_t ltime; /* calendar time */
	ltime = time(NULL); /* get current cal time */
	HAL_writeToLogFile("Starting at %s", asctime(localtime(&ltime)));
}

void HAL_closeLogFile()
{
	if (pLogFile != NULL)
	{
		time_t ltime; /* calendar time */
		ltime = time(NULL); /* get current cal time */
		HAL_writeToLogFile("Closing logfile %s", asctime(localtime(&ltime)));
		fclose(pLogFile);
	}
}

void HAL_writeToLogFile(char *format,...)
{
	if (pLogFile == NULL)
	{
		return;
	}
	char buf[256];
	va_list argptr;
	va_start(argptr, format);
	vsprintf(buf, format, argptr);
	va_end(argptr);

	fwrite(buf, strlen(buf),1,pLogFile);
}

void HAL_flushLogFile()
{
	if (pLogFile == NULL)
	{
		return;
	}

	fflush(pLogFile);
}

/************************************************  Timer ***************************************************/
void HAL_setTimeout_ms(int timeOut_ms)
{
	struct timeval t, tt;
	gettimeofday(&tt, NULL);
	timerclear(&t);
	if (timeOut_ms >= 1000)
	{
		t.tv_sec = timeOut_ms / 1000;
		timeOut_ms -= (t.tv_sec * 1000000);
	}
	t.tv_usec = (timeOut_ms * 1000);
	timeradd(&tt, &t, &_tv);
}

void HAL_setTimeout_us(int timeOut_us)
{
	struct timeval t, tt;
	gettimeofday(&tt, NULL);
	timerclear(&t);
	if (timeOut_us >= 1000000)
	{
		t.tv_sec = timeOut_us / 1000000;
		timeOut_us -= (t.tv_sec * 1000000);
	}
	t.tv_usec = timeOut_us;
	
	timeradd(&tt, &t, &_tv);
}

unsigned char HAL_hasTimeoutExpired()
{
	struct timeval  t_now;

	gettimeofday(&t_now, NULL);

	if (t_now.tv_sec > _tv.tv_sec)
	{
		return 0;
	}

	if (t_now.tv_sec < _tv.tv_sec)
	{
		return 1;
	}

	if (t_now.tv_usec < _tv.tv_usec)
	{
		return 1;
	}

	return 0;
}

/************************************************ FPGA ***************************************************/
int fpgaAxiReadWrite(char *uiod, int map_size, int len, int offset, int read_0_writre_1, int data)
{
	HAL_writeToLogFile("Access to FPGA Axi to offset %d", offset);
	return 1;
	
	switch (read_0_writre_1)
	{
	case 0:
		fpga_read(offset, &data);
		break;

	case 1:
		fpga_write(offset, &data);
		break;

	default:
		return 1;
		break;
	}
  return 0;
}



