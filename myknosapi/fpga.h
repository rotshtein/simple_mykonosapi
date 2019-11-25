/*
 * fpga.h
 *
 *  Created on: Nov 18, 2019
 *      Author: x300
 */

#ifndef FPGA_H_
#define FPGA_H_

#define SPI_RX_DATA 0x40
#define SPI_TX_DATA 0x44
#define SPI_STATUS	0x48
#define SPI_CHIP_SELECT	0x54

#define RX_READY 0x80
#define TX_READY 0x40

typedef enum
{
	FPGA_OK = 0,
	FPGA_FAILED
} fpgaErr_t;

fpgaErr_t fpga_init ();
void fpga_close(); 
fpgaErr_t fpga_write(int offset, unsigned int data);
fpgaErr_t fpga_read(int offset, unsigned int *data);

#endif /* FPGA_H_ */
