/*Write a character driver wiwq
th open, read, write and close functionalities. Test the driver through a user application by reading data from the driver and writing data to the driver*/
#include<linux/init.h>
#include<linux/module.h>
#include<linux/kdev_t.h>
#include<linux/types.h>
#include<linux/fs.h>
#include<linux/cdev.h>
#include<linux/uaccess.h>
#include<linux/semaphore.h>


int open_char(struct inode *inode,struct file *filp);
int release_char(struct inode *inode,struct file *filp);
ssize_t write_char(struct file *filp,const char __user *ubuff,size_t count,loff_t *offp);
ssize_t read_char(struct file *filp,char __user *ubuff,size_t count,loff_t *offp);

struct file_operations fops = 
{
	.owner = THIS_MODULE,
	.open  = open_char,
	.read  = read_char,
	.write = write_char,
	.release=release_char,
};
dev_t chardev;
dev_t chardev1;
struct cdev *my_cdev;
struct semaphore sem;

static int __init char_init(void)
{
	int result;
	int MAJOR,MINOR;
	chardev= MKDEV(255,0);
	chardev1=MKDEV(255,1);
	MAJOR=MAJOR(chardev);
	MINOR=MINOR(chardev);
	printk("the major number is %d and minor number is %d\n",MAJOR,MINOR);
	result=register_chrdev_region(chardev,2,"chardevice");//static allocation of device number
	if(result<0)
	{
		printk("error getting device\n");
		return(-1);
	}
	my_cdev=cdev_alloc();//allocate memory to char device
	my_cdev->ops=&fops;//link file operation to the char device

	result=cdev_add(my_cdev,chardev,1);
	result=cdev_add(my_cdev,chardev1,1);
	if(result<0)
	{
		printk("the char device has not been created\n");
		unregister_chrdev_region(chardev,1);
		return(-1);
	}
	return 0;

}

static void __exit char_exit(void)
{
	unregister_chrdev_region(chardev,2);//unregister device number
	cdev_del(my_cdev);
	printk("Bye");
	return;
}
//open system call
int open_char(struct inode *inode,struct file *filp)
{
	printk("kernel open call\n");
	return 0;
}
//close system call
int release_char(struct inode *inode,struct file *filp)
{
	printk("kernel close and Bye\n");
	return 0;
}
//read functionality
ssize_t read_char(struct file *filp,char __user *ubuff,size_t count,loff_t *offp)
{
	char kbuff[50]="hello world";
	unsigned long result;
	ssize_t retval;
	result=copy_to_user((char *)ubuff,(char *)kbuff,count);
	if(result==0)
	{
		printk("data successful read.\n");
		retval=count;
		return retval;
	}
	else if(result>0)
	{
		retval=(count-result);
		return retval;
	}
	else 
	{
		printk("error writing data.\n");
		retval=-EFAULT;
		return retval;
	}
}
//write functionality
ssize_t write_char(struct file *filp,const char __user *ubuff,size_t count,loff_t *offp)
{
	char kbuff[100];
	unsigned long result;
	ssize_t retval;
	result=copy_from_user((char *)kbuff,(char *)ubuff,count);
	if(result==0)
	{
		printk("received from user : %s\n",kbuff);
		retval=count;
		return retval;
	}
	else if(result>0)
	{
		printk("received from user : %s\n",kbuff);
		retval=(count-result);
		return retval;
	}
	else
	{
		printk("error in writing data.\n");
		retval=-EFAULT;
		return retval;
	}
}

module_init(char_init);
module_exit(char_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("MADHU");


