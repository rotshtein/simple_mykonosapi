/*
 * HAL.h
 *
 *  Created on: Nov 18, 2019
 *      Author: x300
 */

#ifndef HAL_H_
#define HAL_H_
#include "spi.h"

void HAL_writeToLogFile(char *p,...);
int HAL_initSpi(int chipSelectIndex, unsigned char CPOL_CPHA, int spiClkFreq_Hz);
void HAL_closeSpi();
int HAL_spiWrite(char *txbuf, int len);
int HAL_spiRead(char *txbuf, int len, char *data);
void HAL_openLogFile(char *filename);
void HAL_closeLogFile();
void HAL_flushLogFile();
void HAL_setTimeout_ms(int timeOut_ms);
void HAL_setTimeout_us(int timeOut_us);
unsigned char HAL_hasTimeoutExpired();
int fpgaAxiReadWrite(char *uiod, int map_size, int len, int offset, int Zero_one, int data);

#endif
