#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/module.h>
#include <linux/cdev.h>
#include "ioctl_test.h"

MODULE_LICENSE("GPL");

#define DEVICE_MAJOR	177
#define DEVICE_MINOR	123
#define DEVICE_CNT		1
#define DEVICE_NAME		"ioctltest"

static dev_t g_dev;
static struct cdev g_cdev;

static void init(void)
{
}

static unsigned long read_reg(int addr)
{
	return 0xCC;
}

static void write_reg(int addr, unsigned long data)
{
}

static int test_open(struct inode *node, struct file *file)
{
	return 0;
}

static int test_close(struct inode *node, struct file *file)
{
	return 0;
}

static ssize_t test_read(struct file *file, char __user *_buffer, size_t sz, loff_t *loff)
{
	char tmp[32] = "I am fine.";
	int len = strlen(tmp) + 1;

	printk("[%s %d] %ld\n", __func__, __LINE__, sz);

	if(copy_to_user(_buffer, tmp, len)) {
		return -EFAULT;
	}

	return len;
}

static ssize_t test_write(struct file *file, const char __user *_buffer, size_t sz, loff_t *loff)
{
	char* tmp = kmalloc(sz, GFP_KERNEL);

	if(NULL == tmp)
		return -EFAULT;

	if(copy_from_user(tmp, _buffer, sz)) {
		kfree(tmp);
		return -EFAULT;
	}

	printk("[%s %d] %ld %s\n", __func__, __LINE__, sz, tmp);
	kfree(tmp);
	
	return sz;
}

static long test_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
	int ret = 0;
	struct msg _msg;

	/* check device type */
	if(_IOC_TYPE(cmd) != IOC_MAGIC) {
		pr_err("[%s] command type [%c] error!\n", __func__, _IOC_TYPE(cmd));
		return -ENOTTY;
	}

	/* check number */
	if(_IOC_NR(cmd) > IOC_MAXNR) {
		pr_err("[%s] command number [%d] exceeded!\n", __func__, _IOC_NR(cmd));
		return -ENOTTY;
	}

	/* check access */
	if(_IOC_DIR(cmd) & _IOC_READ) {
		ret = !access_ok(VERIFY_WRITE, (void __user*)arg, _IOC_SIZE(cmd));
	}
	else if(_IOC_DIR(cmd & _IOC_WRITE)) {
		ret = !access_ok(VERIFY_READ, (void __user*)arg, _IOC_SIZE(cmd));
	}

	if(ret)
		return -EFAULT;

	switch(cmd)
	{
	case IOCINIT:
		init();
		break;

	case IOCGREG:
		if((ret = copy_from_user(&_msg, (struct msg __user*)arg, sizeof(_msg)))) {
			return -EFAULT;
		}

		_msg.data = read_reg(_msg.addr);

		if((ret = copy_to_user((struct msg __user*)arg, &_msg, sizeof(_msg)))) {
			return -EFAULT;
		}
		break;

	case IOCWREG:
		if((ret = copy_from_user(&_msg, (struct msg __user*)arg, sizeof(_msg)))) {
			return -EFAULT;
		}

		write_reg(_msg.addr, _msg.data);
		break;

	default:
		return -ENOTTY;
	}

	return 0;
}

static const struct file_operations g_fops = {
	.owner = THIS_MODULE,
	.open = test_open,
	.release = test_close,
	.read = test_read,
	.write = test_write,
	.unlocked_ioctl = test_ioctl,
};

static int ioctl_test_init(void)
{
	int ret;

	printk("ioctl_test_init\n");

	g_dev = MKDEV(DEVICE_MAJOR, DEVICE_MINOR);

	if((ret = register_chrdev_region(g_dev, DEVICE_CNT, DEVICE_NAME))) {
		printk(KERN_ERR "unable to register minors for %s\n", DEVICE_NAME);
		return -EINVAL;
	}
	else {
		printk(KERN_INFO "register_chrdev_region sucess\n");
	}

	cdev_init(&g_cdev, &g_fops);

	if((ret = cdev_add(&g_cdev, g_dev, DEVICE_CNT))) {
		printk(KERN_ERR "unable to cdev_add\n");
		return -EINVAL;
	}
	else {
		printk(KERN_INFO "cdev_add success\n");
	}

	return 0;
}

static void ioctl_test_exit(void)
{
	printk("ioctl_test_exit\n");

	cdev_del(&g_cdev);
	unregister_chrdev_region(g_dev, DEVICE_CNT);
}

module_init(ioctl_test_init);
module_exit(ioctl_test_exit);
