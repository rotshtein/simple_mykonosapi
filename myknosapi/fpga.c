/*
 * fpga.c
 *
 *  Created on: Nov 18, 2019
 *      Author: x300
 */
#include "fpga.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <pthread.h>
#include <sys/mman.h>
#include <sys/ioctl.h>

#define DEV             "/dev/mem"
#define HW_REGS_BASE    0xFF200000
#define HW_REGS_LEN     0x1000
#define HW_REGS_MASK    HW_REGS_LEN-1

static	int           _fd = 0;
static	void          *_virtual_base = NULL;


//=============================================
fpgaErr_t fpga_init ()
{
  _fd = open(DEV, O_RDWR|O_SYNC);
  if (_fd == -1)
  {
    printf( "ERROR: could not open \"%s\" : %s\n", DEV, strerror(errno));
    return FPGA_FAILED;
  }

  // Map the address space for the Lightweight bridge into user space so we can interact with them.
  _virtual_base = mmap(NULL,                     /* addr */
                      HW_REGS_LEN,              /* length */
                      PROT_READ|PROT_WRITE,     /* prot */
                      MAP_SHARED,               /* flags */
                      _fd,                       /* fd */
                      HW_REGS_BASE);            /* offset */

  if (_virtual_base == MAP_FAILED)
  {
    printf("ERROR: mmap() failed...\n" );
    close(_fd);
	_fd = 0;
	_virtual_base = NULL;
    return FPGA_FAILED;
  }

  return FPGA_OK;
}

void fpga_close()
{
	if (_fd != 0)
	{
		close(_fd);
		_fd = 0;
		_virtual_base = NULL;
	}
}

fpgaErr_t fpga_write(int offset, unsigned int data)
{
	if (_virtual_base == NULL)
	{ 
		if (fpga_init() != FPGA_OK)
		{
			return FPGA_FAILED;
		}
	}
	
	*(unsigned int*)(_virtual_base + offset) = data;

	return FPGA_OK;
}

fpgaErr_t fpga_read(int offset, unsigned int *data)
{
	if (_virtual_base == NULL)
	{
		if (fpga_init() != FPGA_OK)
		{
			return FPGA_FAILED;
		}
	}
	*data = *(unsigned int*)(_virtual_base + offset);
	return FPGA_OK;
}
