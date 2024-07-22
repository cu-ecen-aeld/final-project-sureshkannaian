#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>

#define DEVICE_NAME "uzrramstore"
#define CLASS_NAME  "uzrram"

static int    majorNumber;
static char   message[256] = {0};
static short  size_of_message;
static struct class*  uzrramClass  = NULL;
static struct device* uzrramDevice = NULL;

static int     dev_open(struct inode *, struct file *);
static int     dev_release(struct inode *, struct file *);
static ssize_t dev_read(struct file *, char *, size_t, loff_t *);
static ssize_t dev_write(struct file *, const char *, size_t, loff_t *);

static struct file_operations fops =
{
   .open = dev_open,
   .read = dev_read,
   .write = dev_write,
   .release = dev_release,
};

static int __init uzrramstore_init(void){
   printk(KERN_INFO "UZR_RAM_Store: Initializing the UZRRAMStore LKM\n");

   majorNumber = register_chrdev(0, DEVICE_NAME, &fops);
   if (majorNumber<0){
      printk(KERN_ALERT "UZR_RAM_Store failed to register a major number\n");
      return majorNumber;
   }
   printk(KERN_INFO "UZR_RAM_Store: registered correctly with major number %d\n", majorNumber);

   uzrramClass = class_create(CLASS_NAME);
   if (IS_ERR(uzrramClass)){
      unregister_chrdev(majorNumber, DEVICE_NAME);
      printk(KERN_ALERT "Failed to register device class\n");
      return PTR_ERR(uzrramClass);
   }
   printk(KERN_INFO "UZR_RAM_Store: device class registered correctly\n");

   uzrramDevice = device_create(uzrramClass, NULL, MKDEV(majorNumber, 0), NULL, DEVICE_NAME);
   if (IS_ERR(uzrramDevice)){
      class_destroy(uzrramClass);
      unregister_chrdev(majorNumber, DEVICE_NAME);
      printk(KERN_ALERT "Failed to create the device\n");
      return PTR_ERR(uzrramDevice);
   }
   printk(KERN_INFO "UZR_RAM_Store: device class created correctly\n");
   return 0;
}

static void __exit uzrramstore_exit(void){
   device_destroy(uzrramClass, MKDEV(majorNumber, 0));
   class_unregister(uzrramClass);
   class_destroy(uzrramClass);
   unregister_chrdev(majorNumber, DEVICE_NAME);
   printk(KERN_INFO "UZR_RAM_Store: Goodbye from the LKM!\n");
}

static int dev_open(struct inode *inodep, struct file *filep){
   printk(KERN_INFO "UZR_RAM_Store: Device has been opened\n");
   return 0;
}

static ssize_t dev_read(struct file *filep, char *buffer, size_t len, loff_t *offset){
   int error_count = 0;
   error_count = copy_to_user(buffer, message, size_of_message);

   if (error_count==0){
      printk(KERN_INFO "UZR_RAM_Store: Sent %d characters to the user\n", size_of_message);
      return (size_of_message=0);
   }
   else {
      printk(KERN_INFO "UZR_RAM_Store: Failed to send %d characters to the user\n", error_count);
      return -EFAULT;
   }
}

static ssize_t dev_write(struct file *filep, const char *buffer, size_t len, loff_t *offset){
   sprintf(message, "%s(%zu letters)", buffer, len);
   size_of_message = strlen(message);
   printk(KERN_INFO "UZR_RAM_Store: Received %zu characters from the user\n", len);
   return len;
}

static int dev_release(struct inode *inodep, struct file *filep){
   printk(KERN_INFO "UZR_RAM_Store: Device successfully closed\n");
   return 0;
}

module_init(uzrramstore_init);
module_exit(uzrramstore_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Suresh Kannaian");
MODULE_DESCRIPTION("A UZR RAM Store project");
MODULE_VERSION("0.1");
