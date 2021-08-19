
#include<linux/init.h>
#include<linux/module.h>
#include<linux/kdev_t.h>
#include<linux/types.h>
#include<linux/fs.h>
#include<linux/cdev.h>
#include<linux/uaccess.h>


int open_ioctl(struct inode *inode,struct file *filp);
int release_ioctl(struct inode *inode,struct file *filp);
ssize_t write_ioctl(struct file *filp,const char __user *ubuff,size_t count,loff_t *offp);
ssize_t read_ioctl(struct file *filp,char __user *ubuff,size_t count,loff_t *offp);
long ioctl_c(struct file *filp,unsigned int cmd,unsigned long arg);

struct file_operations fops = 
{
	.owner = THIS_MODULE,
	.open  = open_ioctl,
	.read  = read_ioctl,
	.write = write_ioctl,
	.release=release_ioctl,
	.unlocked_ioctl=ioctl_c,
};
dev_t chardev;
struct cdev *my_cdev;
static int __init ioctl_init(void)
{
	int result;
	int MAJOR,MINOR;
	chardev= MKDEV(255,0);
	MAJOR=MAJOR(chardev);
	MINOR=MINOR(chardev);
	printk("the major number is %d and minor number is %d\n",MAJOR,MINOR);
	result=register_chrdev_region(chardev,1,"ioctl");//static allocation of device number
	if(result<0)
	{
		printk("error getting device\n");
		return(-1);
	}
	my_cdev=cdev_alloc();//allocate memory to char device
	my_cdev->ops=&fops;//link file operation to the char device

	result=cdev_add(my_cdev,chardev,1);
	if(result<0)
	{
		printk("the char device has not been created\n");
		unregister_chrdev_region(chardev,1);
		return(-1);
	}
	return 0;

}

static void __exit ioctl_exit(void)
{
	unregister_chrdev_region(chardev,1);//unregister device number
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
struct stats
{
	int size;
	char buff[];
	int r_w
//ioctl function
long ioctl_c(struct file *filp,unsigned int cmd,unsigned long arg)
{
        struct stats *stats;
        char buff[50];
	switch(cmd)
	{
		case GETSTATS:copy_form_user(buff,(struct stats *)arg,sizeof(struct stats));
			      printk("received form user :%s\n",stats->ubuff);
			      break;
		default:printk("invalid cmd\n");
	}
}
module_init(ioctl_init);
module_exit(ioctl_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("MADHU");


