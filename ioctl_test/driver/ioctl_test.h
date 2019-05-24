#ifndef __IOCTL_TEST_H
#define __IOCTL_TEST_H

#include <linux/ioctl.h> // for kernel space
//#include <sys/ioctl.h> // for user space

/* device type */
#define IOC_MAGIC 	'c'

/* for device init */
#define IOCINIT		_IO(IOC_MAGIC, 0)

#define IOCGREG		_IOW(IOC_MAGIC, 1, int)
#define IOCWREG 	_IOR(IOC_MAGIC, 2, int)

#define IOC_MAXNR	3

struct msg {
	int addr;
	unsigned int data;
};

#endif
