#include <linux/module.h> // Needed by all modules
#include <linux/kernel.h> // Needed for KERN_* and printk
#include <linux/init.h> // Needed for __init and __exit macros
#include <asm/io.h> // Needed for IO reads and writes

#include "xparameters.h" // needed for IO reads and writes

#include <linux/ioport.h> // not sure if needed. Used for io memory allocation
#include <linux/fs.h> // Needed for dynamic allocation

// From xparameters.h
#define PHY_ADDR XPAR_MULTIPLY_0_BASEADDR // physical address of multiplier
// size of physical address range for multiply
#define MEMSIZE XPAR_MULTIPLY_0_HIGHADDR - XPAR_MULTIPLY_0_BASEADDR + 1
// used for registering the device
#define DEVICE_NAME "multiplier"
#define BUF_LEN 80 // probably need to change this


void* virt_addr; // virtual address pointing to multiplier



/*
 * This function is called when the module is loaded and registers a
 * device for the driver to use.
 */
static int __init my_init(void)
{



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

  static int Major; // should this be static?

  Major = register_chrdev(0, DEVICE_NAME, &fops);

  /* Negative values indicate a problem */
  if (Major < 0) {
    /* Make sure you release any other resources you've already
       grabbed if you get here so you don't leave the kernel in a
       broken state. */
    printk(KERN_ALERT "Registering char device failed with %d\n", Major);

    /* We won't need our memory so make sure to free it here... */
    kfree(msg_bf_Ptr);

    return Major;
  } // not sure if this is correct for dynamic assignment

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
  kfree(msg_bf_Ptr); // free our memory (note the ordering here)
  printk(KERN_ALERT "unmapping virtual address space...\n");
  iounmap((void*)virt_addr);
}


static int device_open(struct inode *inode, struct file *file)
{

  /* In these case we are only allowing one process to hold the device
     file open at a time. */
  if (Device_Open)		/* Device_Open is my flag for the
				   usage of the device file (definied
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

  cur_Ptr = msg_bf_Ptr;		/* set the ptr to the beginning of the
				   message */

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


  /*
   * Actually put the data into the buffer
   */
  while (length && *cur_Ptr) {
    /*
     * The buffer is in the user data segment, not the kernel
     * segment so "*" assignment won't work.  We have to use
     * put_user which copies data from the kernel data segment to
     * the user data segment.
     */
    put_user(*(cur_Ptr++), buffer++); /* one char at a time... */

    length--;
    bytes_read++;
  }


  /*
   * Most read functions return the number of bytes put into the buffer
   */
  return bytes_read;
}

/*
 * This function is called when somebody tries to write into our
 * device file.
 */
static ssize_t device_write(struct file *file, const char __user * buffer, size_t length, loff_t * offset)
{
  int i;

  /* printk(KERN_INFO "device_write(%p,%s,%d)", file, buffer, (int)length); */

  /* get_user pulls message from userspace into kernel space */
  for (i = 0; i < length && i < BUF_LEN-1; i++)
    get_user(msg_bf_Ptr[i], buffer + i);

  /* left one char early from buffer to leave space for null char*/
  msg_bf_Ptr[i] = '\0';

  /*
   * Again, return the number of input characters used
   */
  return i;

  // My portion
  // write 7 to register 0
  printk(KERN_INFO "Writing a 7 to register 0\n");
  iowrite32(7, virt_addr + 0); // base address plus offset
  // write 2 to register 1
  printk(KERN_INFO "Writing a 2 to register 1\n");
  iowrite32(7, virt_addr + 4);
}










// These define info that can be displayed by modinfo
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Dennis Moore (and others)");
MODULE_DESCRIPTION("Simple multiplier module");

// Here we define which functions we want to use for initialization and cleanup
module_init(my_init);
module_exit(my_exit);
