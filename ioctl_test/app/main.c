#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>

#include "ioctl_test.h"

int main(int argc, char* argv[])
{
	int fd;
	int ret;
	struct msg _msg;
	char buffer[32];

	fd = open("/dev/ioctltest", O_RDWR);
	if(fd < 0) {
		perror("open");
		return -2;
	}

	ret = ioctl(fd, IOCINIT);
	if(ret) {
		perror("ioctl init:");
		return -3;
	}

	memset(&_msg, 0, sizeof(_msg));
	_msg.addr = 0x01;
	ret = ioctl(fd, IOCGREG, &_msg);
	if(ret) {
		perror("ioctl write");
		return -5;
	}

	printf("read_reg:0x%02X\n", _msg.data);

	strcpy(buffer, "how are you?");
	ret = write(fd, buffer, strlen(buffer)+1);
	printf("write %d\n", ret);

	ret = read(fd, buffer, sizeof(buffer));
	printf("read %d %s\n", ret, buffer);

	return 0;
}
