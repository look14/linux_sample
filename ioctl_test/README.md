# ioctl_test

## example
```shell
rmmod ioctltest.ko; insmod ioctltest.ko; dmesg -c

mknod /dev/ioctltest c 177 123
chmod 766 /dev/ioctltest
```