#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>

#define DEVICE_MAJOR  240
#define DEVICE_NAME "mydev_joi"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Jóhannes Bergur");
MODULE_DESCRIPTION("A simple Linux LKM that accepts a single parameter");
MODULE_VERSION("0.1");

static char *name = "world";
static int majorNumber;

module_param(name, charp, S_IRUGO); // S_IRUGO : only root can change; S_IWUSR anyone can change
MODULE_PARM_DESC(name, "The name to display in /var/log/kern.log");

static int     hello_open(struct inode *, struct file *);
static int     hello_release(struct inode *, struct file *);
static ssize_t hello_write(struct file *, const char *, size_t, loff_t *);

struct file_operations fops = 
{
	.write = hello_write,
	.open = hello_open,
	.release = hello_release,
};


static int hello_open(struct inode *inode, struct file *filep)
{
	printk("mydev_joi open: successful\n");
	return 0;
}

static int hello_release(struct inode *inode, struct file *file)
{
	printk(KERN_INFO "mydev_joi release: successful\n");
	return 0;
}


static ssize_t hello_write(struct file *file, const char *buf, size_t count, loff_t *ppos)
{
	printk("mydev_joi_write: accepting %zu bytes \n",count);
	return count;
}

static int __init hello_init(void){
	majorNumber=register_chrdev(DEVICE_MAJOR, DEVICE_NAME, &fops);
	if(majorNumber < 0)
	{
		printk(KERN_ALERT "mydev_joi failed to register a major number\n");
		return majorNumber;
	}
    printk(KERN_INFO "mydev_joi: Hello %s from the RaPi LKM! \n",name);
	printk(KERN_INFO "mydev_joi: registered correctly with majornumber %d\n",majorNumber);
    return 0;
}


static void __exit hello_exit(void){
    printk(KERN_INFO "mydev_joi: Goodbye %s from the RaPi LKM!\n", name);
	unregister_chrdev(majorNumber, DEVICE_NAME);
}

module_init(hello_init);
module_exit(hello_exit);