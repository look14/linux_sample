obj-m := world.o
world-objs := hello.o test.o
PWD := $(shell pwd)

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	@rm -rf *.o *.ko .tmp_versions .*.ko.cmd .*.o.cmd *.symvers *.order .cache.mk *.mod.c
	