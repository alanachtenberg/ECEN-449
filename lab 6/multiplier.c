#include <linux/module.h> // Needed by all modules
#include <linux/kernel.h> // Needed for KERN_* and printk
#include <linux/init.h> // Needed for __init and __exit macros
#include <asm/io.h> // Needed for IO reads and writes
#include <linux/moduleparam.h>
#include <asm/uaccess.h>
#include "xparameters.h" // needed for IO reads and writes
#include <linux/ioport.h> // not sure if needed. Used for io memory allocation
#include <linux/fs.h> // Needed for dynamic allocation
#include <linux/sched.h>

#define PHY_ADDR XPAR_MULTIPLY_0_BASEADDR // physical address of multiplier
#define MEMSIZE XPAR_MULTIPLY_0_HIGHADDR - XPAR_MULTIPLY_0_BASEADDR + 1 // size of physical address range for multiply
#define DEVICE_NAME "multiplier"
#define BUF_LEN 8 // probably need to change this


void* virt_addr; // virtual address pointing to multiplier
static int Major; // should this be static?
static int Device_Open = 0; // Flag to signify open device

/* This structure defines the function pointers to our functions for
   opening, closing, reading and writing the device file.  There are
   lots of other pointers in this structure which we are not using,
   see the whole definition in linux/fs.h */
static struct file_operations fops = {
  .read = device_read,
  .write = device_write,
  .open = device_open,
  .release = device_release
}; 

/* Function prototypes, so we can setup the function pointers for dev
   file access correctly. */
static int device_open(struct inode *, struct file *);
static int device_release(struct inode *, struct file *);
static ssize_t device_read(struct file *, char *, size_t, loff_t *);
static ssize_t device_write(struct file *, const char *, size_t, loff_t *);
 

/*
 * This function is called when the module is loaded and registers a
 * device for the driver to use.
 */
static int __init my_init(void)
{

  /* Note that as soon as the device is registered we might get access
     to the functions pointed to by fops, hence the ordering. */
  
  // Linux kernel's version of printf
  printk(KERN_INFO "Mapping virtual address...\n");
	
  // map virtual address to multiplier physical address
  // use ioremap
  virt_addr = ioremap(PHY_ADDR, MEMSIZE); // might need to convert inputs to unsigned long
  printk(KERN_INFO "Multiplication physical address: %h \n", PHY_ADDR); // might need to change %p 
  printk(KERN_INFO "Multiplication virtual address: %h \n", virt_addr);
  
  /* This function call registers a device and returns a major number
     associated with it.  Be wary, the device file could be accessed
     as soon as you register it, make sure anything you need (ie
     buffers ect) are setup _BEFORE_ you register the device.*/

  Major = register_chrdev(0, DEVICE_NAME, &fops);
  
  /* Negative values indicate a problem */
  if (Major < 0) {		
    /* Make sure you release any other resources you've already
       grabbed if you get here so you don't leave the kernel in a
       broken state. */
    printk(KERN_ALERT "Registering char device failed with %d\n", Major);
    return Major;
  } 

  printk(KERN_INFO "Registered a device with dynamic Major number of %d\n", Major);
  //printk(KERN_INFO "Create a device file for this device with this command:\n'mknod /dev/%s c %d 0'.\n", DEVICE_NAME, Major);

  return 0;		/* success */
}

/* This function is run just prior to the module's removal from the system.
You should release ALL resources used by your module here (otherwise be
prepared for a reboot). */
static void __exit my_exit(void)
{
  unregister_chrdev(Major, DEVICE_NAME);	
  printk(KERN_ALERT "unmapping virtual address space...\n");
  iounmap((void*)virt_addr);
}


static int device_open(struct inode *inode, struct file *file)
{
  
  /* In these case we are only allowing one process to hold the device
     file open at a time. */
  if (Device_Open)		/* Device_Open is my flag for the
				   usage of the device file (defined
				   in my_chardev_mem.h)  */
    return -EBUSY;		/* Failure to open device is given
				   back to the userland program. */

  Device_Open++;		/* Keeping the count of the device
				   opens. */


  try_module_get(THIS_MODULE);	/* increment the module use count
				   (make sure this is accurate or you
				   won't be able to remove the module
				   later. */

  printk(KERN_INFO "Device has been opened \n");

  //cur_Ptr = msg_bf_Ptr;		// set the ptr to the beginning of the message
				   
  
  return 0;
}



/* 
 * Called when a process closes the device file.
 */
static int device_release(struct inode *inode, struct file *file)
{
  Device_Open--;		/* We're now ready for our next caller */
  
  /* 
   * Decrement the usage count, or else once you opened the file,
   * you'll never get get rid of the module.
   */
  module_put(THIS_MODULE);

  printk(KERN_INFO "Device has been closed \n");
  
  return 0;
}



/* 
 * Called when a process, which already opened the dev file, attempts to
 * read from it.
 */
static ssize_t device_read(struct file *filp,	/* see include/linux/fs.h   */
			   char *buffer,	/* buffer to fill with data */
			   size_t length,	/* length of the buffer     */
			   loff_t * offset)
{
  /*
   * Number of bytes actually written to the buffer 
   */
  int bytes_read = 0;
  
  int* int_buffer = (int*)kmalloc(length*sizeof(int), GFP_KERNEL);
  int_buffer[0] = ioread32(virt_addr+0);
  int_buffer[1] = ioread32(virt_addr+4);
  int_buffer[2] = ioread32(virt_addr+8);
  
  char* char_buffer = (char*)int_buffer; // transform integers read from hardware into chars
    
  /* 
   * Actually put the data into the buffer 
   */
  while (length && *char_buffer) {
    /* 
     * The buffer is in the user data segment, not the kernel 
     * segment so "*" assignment won't work.  We have to use 
     * put_user which copies data from the kernel data segment to
     * the user data segment. 
     */
    // put_user(*(cur_Ptr++), buffer++); /* one char at a time... */
    put_user(*(char_buffer++), buffer++);
	
    length--;
    bytes_read++;
  }
     
  /* 
   * Most read functions return the number of bytes put into the buffer
   */
  kfree(int_buffer);
  return bytes_read;
}

/* 
 * This function is called when somebody tries to write into our
 * device file.
 */
static ssize_t device_write(struct file *file, const char __user * buffer, size_t length, loff_t * offset)
{
  int i;
  
  char* msg_bf = (char *)kmalloc(length*sizeof(char), GFP_KERNEL);
  
  /* printk(KERN_INFO "device_write(%p,%s,%d)", file, buffer, (int)length); */
  
  /* get_user pulls message from userspace into kernel space */
  for (i = 0; i < length && i < BUF_LEN-1; i++)
    get_user(msg_bf[i], buffer + i); // place chars from user space into kernel buffer
  
  /* left one char early from buffer to leave space for null char*/
  msg_bf[length] = '\0';
  
  int* msg_buf = (int*)msg_bf;
  
  iowrite32(msg_buf[0], virt_addr + 0); // base address plus offset
  iowrite32(msg_buf[1], virt_addr + 4);

  /* 
   * Again, return the number of input characters used 
   */
  kfree(msg_bf);
  return i;

}

// These define info that can be displayed by modinfo
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Dennis Moore (and others)");
MODULE_DESCRIPTION("Simple multiplier module");

// Here we define which functions we want to use for initialization and cleanup
module_init(my_init);
module_exit(my_exit);
