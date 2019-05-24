# ioctl_test

## example
```shell
rmmod ioctltest.ko; insmod ioctltest.ko; dmesg -c

mknod /dev/ioctltest c 177 123
chmod 766 /dev/ioctltest

cp ioctltest.ko /lib/modules/$(uname -r)/kernel/drivers/char/
depmod

modprobe ioctltest >> /etc/rc.local

KERNEL=="ioctltest",MODE="0666" >> /etc/udev/rules.d/50-udev-default.rules
```
