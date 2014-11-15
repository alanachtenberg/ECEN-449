/*  my_chardev_mem.c - Simple character device module
 *  
 *  Demonstrates module creation of character device for user
 *  interaction.  Also utilizes/demostrates kmalloc and write.
 *
 *  (Adapted from various example modules including those found in the
 *  Linux Kernel Programming Guide, Linux Device Drivers book and
 *  FSM's device driver tutorial)
 */

/* Moved all prototypes and includes into the headerfile */
#include "my_chardev_mem.h"



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

/*
 * This function is called when the module is loaded and registers a
 * device for the driver to use.  We also allocate a little memory for
 * the driver to use as a backing store for data written to the device
 * file from userland, emulating a hardware device. Note: if there
 * were a real hardware device (with associated memory mapped io) we
 * wanted to read and write from we'd have to call ioremap to get a
 * kernel virtual memory address that maps to the physical address of
 * the device.
 */
int my_init(void)
{

  /* We need to allocate the memspace _BEFORE_ registering the device
     to avoid any race conditions */
  msg_bf_Ptr = (char *)kmalloc(BUF_LEN*sizeof(char), GFP_KERNEL);
  
  /* Note: kmalloc can fail, even on a non-borked kernel, always exit
     gracefully.  In the event of a failure pointer will be set to
     NULL. */
  if (msg_bf_Ptr == NULL) {		
    /* Failed to get memory, exit gracefully */
    printk(KERN_ALERT "Unable to allocate needed memory\n");

    return 10; 			/* Defining error code of 10 for
				   "Unable to allocate memory" */
  }

  /* Note that as soon as the device is registered we might get access
     to the functions pointed to by fops, hence the ordering. */

  /* Also putting in a setup string in here. */
  sprintf(msg_bf_Ptr, "This device has not been written to yet\n");
  cur_Ptr = msg_bf_Ptr;


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

    /* We won't need our memory so make sure to free it here... */
    kfree(msg_bf_Ptr);

    return Major;
  }

  printk(KERN_INFO "Registered a device with dynamic Major number of %d\n", Major);
  
  printk(KERN_INFO "Create a device file for this device with this command:\n'mknod /dev/%s c %d 0'.\n", DEVICE_NAME, Major);

  return 0;		/* success */
}

/*
 * This function is called when the module is unloaded, it releases
 * the device file.
 */
void my_cleanup(void)
{
  /* 
   * Unregister the device 
   */
  unregister_chrdev(Major, DEVICE_NAME);

  kfree(msg_bf_Ptr);		/* free our memory (note the ordering
				   here) */

}


/* 
 * Called when a process tries to open the device file, like "cat
 * /dev/my_chardev_mem".  Link to this function placed in file operations
 * structure for our device file.
 */
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
}


/* These define info that can be displayed by modinfo */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Paul V. Gratz (and others)");
MODULE_DESCRIPTION("Module which creates a character device and allows user interaction with it");

/* Here we define which functions we want to use for initialization
   and cleanup */
module_init(my_init);
module_exit(my_cleanup);
