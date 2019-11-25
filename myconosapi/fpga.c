/*
 * fpga.c
 *
 *  Created on: Nov 18, 2019
 *      Author: x300
 */
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

static	int           fd = 0;
static	void          *virtual_base = NULL;


//=============================================
int fpga_init ()
{
  fd = open(DEV, O_RDWR|O_SYNC);
  if (fd == -1)
  {
    printf( "ERROR: could not open \"%s\" : %s\n", DEV, strerror(errno));
    return -1;
  }

  // Map the address space for the Lightweight bridge into user space so we can interact with them.
  virtual_base = mmap(NULL,                     /* addr */
                      HW_REGS_LEN,              /* length */
                      PROT_READ|PROT_WRITE,     /* prot */
                      MAP_SHARED,               /* flags */
                      fd,                       /* fd */
                      HW_REGS_BASE);            /* offset */

  if (virtual_base == MAP_FAILED)
  {
    printf("ERROR: mmap() failed...\n" );
    close(fd);
    return -1;
  }

  return 0;
}
