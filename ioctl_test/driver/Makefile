obj-m := ioctltest.o
ioctltest-objs := ioctl_test.o
PWD := $(shell pwd)

CONFIG_MODULE_SIG = n

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	@rm -rf *.o *.ko .tmp_versions .*.ko.cmd .*.o.cmd *.symvers *.order .cache.mk *.mod.c
	