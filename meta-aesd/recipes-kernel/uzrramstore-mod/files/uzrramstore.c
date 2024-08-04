#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/of.h>
#include <linux/of_address.h>

#define DEVICE_NAME "uzrramstore"
#define CLASS_NAME  "uzrram"

static int    majorNumber;
static char   message[256] = {0};
static short  size_of_message;
static struct class*  uzrramClass  = NULL;
static struct device* uzrramDevice = NULL;

/////////////////////////////////
#define MAX_NUMBER_EVENTS 256
#define RESERVED_MEMORY_IDENTIFIER 0x00ABDEFL

struct uzr_info
{
   unsigned int initializedIdentifier;
   unsigned int kernelMode;
   unsigned int bootUpMode;
   unsigned int bootUpState;
   unsigned int bootToMode;
   unsigned int bootToState;
   unsigned int eventWriteIndex;
   unsigned int events[MAX_NUMBER_EVENTS];
   unsigned int traceWriteIndex;
};

static struct uzr_info* uzrInfoPtr = NULL;
static void* virtMem;
static char* trace;
static unsigned int traceBufferSize;
struct kobject* kobj_ref;

/////////////////////////

//static void __iomem *mem_base;
static size_t mem_size;

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

static ssize_t initialized_identifier_show(struct kobject *kobj, struct kobj_attribute *attr, char *buffer);
static ssize_t kernel_mode_show(struct kobject *kobj, struct kobj_attribute *attr, char *buffer);
static ssize_t bootup_mode_show(struct kobject *kobj, struct kobj_attribute *attr, char *buffer);
static ssize_t bootup_state_show(struct kobject *kobj, struct kobj_attribute *attr, char *buffer);
static ssize_t boot_to_mode_show(struct kobject *kobj, struct kobj_attribute *attr, char *buffer);
static ssize_t boot_to_state_show(struct kobject *kobj, struct kobj_attribute *attr, char *buffer);
static ssize_t reset_trace_show(struct kobject *kobj, struct kobj_attribute *attr, char *buffer);
static ssize_t events_size_show(struct kobject *kobj, struct kobj_attribute *attr, char *buffer);

static ssize_t initialized_identifier_store(struct kobject *kobj, struct kobj_attribute *attr,const char *buffer, size_t count);
static ssize_t kernel_mode_store(struct kobject *kobj, struct kobj_attribute *attr,const char *buffer, size_t count);
static ssize_t bootup_mode_store(struct kobject *kobj, struct kobj_attribute *attr,const char *buffer, size_t count);
static ssize_t bootup_state_store(struct kobject *kobj, struct kobj_attribute *attr,const char *buffer, size_t count);
static ssize_t boot_to_mode_store(struct kobject *kobj, struct kobj_attribute *attr,const char *buffer, size_t count);
static ssize_t boot_to_state_store(struct kobject *kobj, struct kobj_attribute *attr,const char *buffer, size_t count);
static ssize_t reset_trace_store(struct kobject *kobj, struct kobj_attribute *attr,const char *buffer, size_t count);
static ssize_t events_size_store(struct kobject *kobj, struct kobj_attribute *attr,const char *buffer, size_t count);

struct kobj_attribute initialized_identifier_attr = __ATTR(initialized-identifier, 0644, initialized_identifier_show, initialized_identifier_store);
struct kobj_attribute kernel_mode_attr = __ATTR(kernel-mode, 0644, kernel_mode_show, kernel_mode_store);
struct kobj_attribute bootup_mode_attr = __ATTR(bootup-mode, 0644, bootup_mode_show, bootup_mode_store);
struct kobj_attribute bootup_state_attr = __ATTR(bootup-state, 0644, bootup_state_show, bootup_state_store);
struct kobj_attribute boot_to_mode_attr = __ATTR(boot-to-mode, 0644, boot_to_mode_show, boot_to_mode_store);
struct kobj_attribute boot_to_state_attr = __ATTR(boot-to-state, 0644, boot_to_state_show, boot_to_state_store);
struct kobj_attribute reset_trace_attr = __ATTR(reset-trace, 0644, reset_trace_show, reset_trace_store);
struct kobj_attribute events_size_attr = __ATTR(events-size, 0644, events_size_show, events_size_store);







static int __init uzrramstore_init(void){
   struct device_node *npName;
   struct device_node *np;
   struct resource res;

   printk(KERN_INFO "UZR_RAM_Store: Initializing the UZRRAMStore LKM\n");

   npName = of_find_node_by_name(NULL, "uzrramstore");
   if (!npName) {
      printk(KERN_ALERT "UZR_RAM_Store: Device tree node not found\n");
      return -ENODEV;
   }

   np = of_parse_phandle(npName, "memory-region", 0);
   if (!np) {
      printk(KERN_ALERT "UZR_RAM_Store: Device tree node not found\n");
      return -ENODEV;
   }


   if (of_address_to_resource(np, 0, &res)) {
      printk(KERN_ALERT "UZR_RAM_Store: Cannot get reserved memory resource\n");
      return -ENODEV;
   }
   mem_size = resource_size(&res);
   printk(KERN_INFO "UZR_RAM_Store: Reserved memory at %pa, size %zu\n", &res.start, mem_size);

   virtMem = memremap(res.start, resource_size(&res), MEMREMAP_WB);
   if (!virtMem) {
      printk(KERN_ALERT "UZR_RAM_Store: Failed to remap reserved memory\n");
      return -ENOMEM;
   }

   mem_size = resource_size(&res);
   printk(KERN_INFO "UZR_RAM_Store: remapped memory at %pa to %p\n", &res.start, virtMem);


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

  // Point boot up related fields to viritual addresses; initialize virtual memory if needed
   uzrInfoPtr = (struct uzr_info*)virtMem;
   if (uzrInfoPtr->initializedIdentifier != RESERVED_MEMORY_IDENTIFIER)
   {
      printk(KERN_INFO "uzrinfo: initializing memory\n");
      memset(virtMem, 0, resource_size(&res));
      uzrInfoPtr->initializedIdentifier = RESERVED_MEMORY_IDENTIFIER;
   }
   else
   {
      uzrInfoPtr->bootUpMode = uzrInfoPtr->bootToMode;
      uzrInfoPtr->bootUpState = uzrInfoPtr->bootToState;
   }
   trace = ((char*) uzrInfoPtr) + sizeof(struct uzr_info);
   traceBufferSize = resource_size(&res) - sizeof(struct uzr_info);   




   // Initialization of sysfs: /sys/uzrinfo/...
   kobj_ref = kobject_create_and_add("uzrinfo", NULL);

   if (sysfs_create_file(kobj_ref, &initialized_identifier_attr.attr))
   {
      printk(KERN_ALERT "Failed to create initialized-indentifier sysfs file\n");
      goto r_sysfs;
   }
   printk(KERN_INFO "initialized-identifier sysfs file created correctly\n");

   if (sysfs_create_file(kobj_ref, &kernel_mode_attr.attr))
   {
      printk(KERN_ALERT "Failed to create kernel-mode sysfs file\n");
      goto r_sysfs;
   }
   printk(KERN_INFO "kernel-mode sysfs file created correctly\n");

   if (sysfs_create_file(kobj_ref, &bootup_mode_attr.attr))
   {
      printk(KERN_ALERT "Failed to create bootup-mode sysfs file\n");
      goto r_sysfs;
   }
   printk(KERN_INFO "bootup-mode sysfs file created correctly\n");

   if (sysfs_create_file(kobj_ref, &bootup_state_attr.attr))
   {
      printk(KERN_ALERT "Failed to create bootup-state sysfs file\n");
      goto r_sysfs;
   }
   printk(KERN_INFO "bootup-state sysfs file created correctly\n");

   if (sysfs_create_file(kobj_ref, &boot_to_mode_attr.attr))
   {
      printk(KERN_ALERT "Failed to create boot-to-mode sysfs file\n");
      goto r_sysfs;
   }
   printk(KERN_INFO "boot-to-mode sysfs file created correctly\n");

   if (sysfs_create_file(kobj_ref, &boot_to_state_attr.attr))
   {
      printk(KERN_ALERT "Failed to create boot-to-state sysfs file\n");
      goto r_sysfs;
   }
   printk(KERN_INFO "boot-to-state sysfs file created correctly\n");

   if (sysfs_create_file(kobj_ref, &reset_trace_attr.attr))
   {
      printk(KERN_ALERT "Failed to create reset-trace sysfs file\n");
      goto r_sysfs;
   }
   printk(KERN_INFO "reset-trace sysfs file created correctly\n");

   if (sysfs_create_file(kobj_ref, &events_size_attr.attr))
   {
      printk(KERN_ALERT "Failed to create events-size sysfs file\n");
      goto r_sysfs;
   }
   printk(KERN_INFO "events-size sysfs file created correctly\n");

   printk(KERN_INFO "successfully initialized\n");
   return 0;

r_sysfs:
   sysfs_remove_file(kobj_ref, &initialized_identifier_attr.attr);
   sysfs_remove_file(kobj_ref, &kernel_mode_attr.attr);
   sysfs_remove_file(kobj_ref, &bootup_mode_attr.attr);
   sysfs_remove_file(kobj_ref, &bootup_state_attr.attr);
   sysfs_remove_file(kobj_ref, &boot_to_mode_attr.attr);
   sysfs_remove_file(kobj_ref, &boot_to_state_attr.attr);
   sysfs_remove_file(kobj_ref, &reset_trace_attr.attr);
   kobject_put(kobj_ref);












   return 0;
}

static void __exit uzrramstore_exit(void){


   // sysfs files   
   sysfs_remove_file(kobj_ref, &initialized_identifier_attr.attr);
   sysfs_remove_file(kobj_ref, &kernel_mode_attr.attr);
   sysfs_remove_file(kobj_ref, &bootup_mode_attr.attr);
   sysfs_remove_file(kobj_ref, &bootup_state_attr.attr);
   sysfs_remove_file(kobj_ref, &boot_to_mode_attr.attr);
   sysfs_remove_file(kobj_ref, &boot_to_state_attr.attr);
   kobject_put(kobj_ref);



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

static ssize_t dev_read(struct file *filep, char *buffer, size_t len, loff_t *offset)
{
   int retVal = 0;
   size_t remainderSize = uzrInfoPtr->traceWriteIndex - *offset;
   ssize_t bytesToCopy = min(remainderSize, len);

   if (bytesToCopy <= 0)
   {
      return 0;
   }

   retVal = copy_to_user(buffer, (trace + *offset), bytesToCopy);
 
   if (retVal == 0)
   {            
      printk(KERN_INFO "UZR_RAM_Store: read: Sent %zd characters to the user\n", bytesToCopy);
      *offset += bytesToCopy;
      return bytesToCopy;
   }
   else 
   {
      printk(KERN_INFO "UZR_RAM_Store: read: Failed to send %d characters to the user\n", retVal);
      return -EFAULT;
   }
}

static ssize_t dev_write(struct file *filep, const char *buffer, size_t len, loff_t *offset)
{
   int result = 0;

   unsigned int remainingTraceSize = traceBufferSize - uzrInfoPtr->traceWriteIndex;

   if (len <= remainingTraceSize)
   {
      char* traceWritePtr = trace + uzrInfoPtr->traceWriteIndex;
      result = copy_from_user(traceWritePtr, buffer, len);
   }
   else 
   {
      printk(KERN_INFO "UZR_RAM_Store: write len %zu does not fit in remaining space %u\n", len, remainingTraceSize);
      return -EFAULT;
   }


   if (result == 0)
   {
      // NULL at end of line ALSO move to next line on a 4 byte boundary
      int lenMod = (len + 1) + ((4 - ((len + 1) % 4)) % 4);
      uzrInfoPtr->traceWriteIndex += lenMod;

       printk(KERN_INFO "UZR_RAM_Store: write: Received %zu characters from the user\n", len);
      return len;
   }
   else 
   {
       printk(KERN_INFO "UZR_RAM_Store: write: Failed to receive %d characters from the user\n", result);
      return -EFAULT;
   }

}



static ssize_t initialized_identifier_show(struct kobject *kobj, struct kobj_attribute *attr, char *buffer)
{
   printk(KERN_DEBUG "uzrinfo - initialized identifier: read\n");
   return sprintf(buffer, "%u", uzrInfoPtr->initializedIdentifier);
}
static ssize_t initialized_identifier_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buffer, size_t count)
{
   printk(KERN_DEBUG "uzrinfo - initialized identifier: write\n");
   if (sscanf(buffer, "%u", &uzrInfoPtr->initializedIdentifier) < 1)
   {
      printk(KERN_ERR "uzrinfo - initialized identifier: Failed - invalid buffer data received");
      return -EFAULT;
   }
   return count;
}

static ssize_t kernel_mode_show(struct kobject *kobj, struct kobj_attribute *attr, char *buffer)
{
   printk(KERN_DEBUG "uzrinfo - kernel mode: read\n");
   return sprintf(buffer, "%u", uzrInfoPtr->kernelMode);
}
static ssize_t kernel_mode_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buffer, size_t count)
{
   printk(KERN_DEBUG "uzrinfo - kernel mode: write\n");
   if (sscanf(buffer, "%u", &uzrInfoPtr->kernelMode) < 1)
   {
      printk(KERN_ERR "uzrinfo - kernel mode: Failed - invalid buffer data received");
      return -EFAULT;
   }
   return count;
}

static ssize_t bootup_mode_show(struct kobject *kobj, struct kobj_attribute *attr, char *buffer)
{
   printk(KERN_DEBUG "uzrinfo - bootup mode: read\n");
   return sprintf(buffer, "%u", uzrInfoPtr->bootUpMode);
}
static ssize_t bootup_mode_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buffer, size_t count)
{
   printk(KERN_DEBUG "uzrinfo - bootup mode: write\n");
   if (sscanf(buffer, "%u", &uzrInfoPtr->bootUpMode) < 1)
   {
      printk(KERN_ERR "uzrinfo - bootup mode: Failed - invalid buffer data received");
      return -EFAULT;
   }
   return count;
}

static ssize_t bootup_state_show(struct kobject *kobj, struct kobj_attribute *attr, char *buffer)
{
   printk(KERN_DEBUG "uzrinfo - bootup state: read\n");
   return sprintf(buffer, "%u", uzrInfoPtr->bootUpState);
}
static ssize_t bootup_state_store(struct kobject *kobj, struct kobj_attribute *attr,const char *buffer, size_t count)
{
   printk(KERN_DEBUG "uzrinfo - bootup state: write\n");
   if (sscanf(buffer, "%u", &uzrInfoPtr->bootUpState) < 1)
   {
      printk(KERN_ERR "uzrinfo - bootup state: Failed - invalid buffer data received");
      return -EFAULT;
   }
   return count;
}

static ssize_t boot_to_mode_show(struct kobject *kobj, struct kobj_attribute *attr, char *buffer)
{
   printk(KERN_DEBUG "uzrinfo - boot to mode: read\n");
   return sprintf(buffer, "%u", uzrInfoPtr->bootToMode);
}
static ssize_t boot_to_mode_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buffer, size_t count)
{
   printk(KERN_DEBUG "uzrinfo - boot to mode: write\n");
   if (sscanf(buffer, "%u", &uzrInfoPtr->bootToMode) < 1)
   {
      printk(KERN_ERR "uzrinfo - boot to mode: Failed - invalid buffer data received");
      return -EFAULT;
   }
   return count;
}

static ssize_t boot_to_state_show(struct kobject *kobj, struct kobj_attribute *attr, char *buffer)
{
   printk(KERN_DEBUG "uzrinfo - boot to state: read\n");
   return sprintf(buffer, "%u", uzrInfoPtr->bootToState);
}
static ssize_t boot_to_state_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buffer, size_t count)
{
   printk(KERN_DEBUG "uzrinfo - boot to state: write\n");
   if (sscanf(buffer, "%u", &uzrInfoPtr->bootToState) < 1)
   {
      printk(KERN_ERR "uzrinfo - boot to state: Failed - invalid buffer data received");
      return -EFAULT;
   }
   return count;
}

static ssize_t reset_trace_show(struct kobject *kobj, struct kobj_attribute *attr, char *buffer)
{
   printk(KERN_DEBUG "uzrinfo - reset trace: read\n");
   return 0;
}
static ssize_t reset_trace_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buffer, size_t count)
{
   int i;

   printk(KERN_DEBUG "uzrinfo - reset trace: write\n");

   uzrInfoPtr->traceWriteIndex = 0;
   memset(trace, 0, traceBufferSize);
   
   for (i = 0; i < uzrInfoPtr->eventWriteIndex; i++)
   {
      uzrInfoPtr->events[i] = 0;
   }
   uzrInfoPtr->eventWriteIndex = 0;

   return count;
}

static ssize_t events_size_show(struct kobject *kobj, struct kobj_attribute *attr, char *buffer)
{
   printk(KERN_DEBUG "uzrinfo - events size: read\n");
   return sprintf(buffer, "%u", uzrInfoPtr->eventWriteIndex);
}
static ssize_t events_size_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buffer, size_t count)
{
   printk(KERN_DEBUG "uzrinfo - events size: write\n");
   return 0;
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
