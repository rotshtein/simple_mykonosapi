#include <stdio.h>
#include <stdarg.h>
#include <string.h>

static 	FILE *pLogFile = NULL;


int HAL_initSpi(int chipSelectIndex, unsigned char CPOL_CPHA, int spiClkFreq_Hz)
{
	return 0;
}

void HAL_closeSpi()
{}

int HAL_spiWrite(char *txbuf, int len)
{
	return 0;
}

int HAL_spiRead(char *txbuf, int len, char *data)
{
	return 0;
}


void HAL_openLogFile(char *filename)
{
	if (pLogFile != NULL)
	{
		fclose(pLogFile);
	}

	pLogFile = fopen("logfile.txt", "at");

	if (!pLogFile) pLogFile = fopen("logfile.txt", "wt");
	if (!pLogFile)
	{
		printf("can not open logfile.txt for writing.\n");
	}
}

void HAL_closeLogFile()
{
	if (pLogFile != NULL)
	{
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

void HAL_setTimeout_ms(int timeOut_ms)
{

}

void HAL_setTimeout_us(int timeOut_us)
{}

unsigned char HAL_hasTimeoutExpired()
{
	return 0;
}

int fpgaAxiReadWrite(char *uiod, int map_size, int len, int offset, int Zero_one, int data)
{
  return 0;
}



