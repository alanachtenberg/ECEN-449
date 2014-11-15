/*  my_chardev.c - Simple character device module
 *  
 *  Demonstrates module creation of character device for user
 *  interaction.
 *
 *  (Adapted from various example modules including those found in the
 *  Linux Kernel Programming Guide, Linux Device Drivers book and
 *  FSM's device driver tutorial)
 */

/* Moved all prototypes and includes into the headerfile */
#include "my_chardev.h"


/*
 * This function is called when the module is loaded and registers a
 * device for the driver to use.
 */
int my_init(void)
{

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

}


/* 
 * Called when a process tries to open the device file, like "cat
 * /dev/my_chardev".  Link to this function placed in file operations
 * structure for our device file.
 */
static int device_open(struct inode *inode, struct file *file)
{
  static int counter = 0;	/* note this static variable only gets
				   initialized the first time this
				   function is called. */
  
  /* In these case we are only allowing one process to hold the device
     file open at a time. */
  if (Device_Open)		/* Device_Open is my flag for the
				   usage of the device file (definied
				   in my_chardev.h)  */
    return -EBUSY;		/* Failure to open device is given
				   back to the userland program. */

  Device_Open++;		/* Keeping the count of the device
				   opens. */

  /* Create a string to output when the device is opened.  This string
     is given to the user program in device_read. Note: We are using
     the "current" task structure which contains information about the
     process that opened the device file.*/
  sprintf(msg, "\"%s\" (pid %i): This device has been accessed %d times\n", current->comm, current->pid, counter++);
  msg_Ptr = msg;
  try_module_get(THIS_MODULE);	/* increment the module use count
				   (make sure this is accurate or you
				   won't be able to remove the module
				   later. */
  
  return 0;
}

/* 
 * Called when a process closes the device file.
 */
static int device_release(struct inode *inode, struct file *file)
{
  Device_Open--;		/* We're now ready for our next
				   caller */
  
  /* 
   * Decrement the usage count, or else once you opened the file,
   * you'll never get get rid of the module.
   */
  module_put(THIS_MODULE);
  
  return 0;
}

/* 
 * Called when a process, which already opened the dev file, attempts
 * to read from it.
 */
static ssize_t device_read(struct file *filp, /* see include/linux/fs.h*/
			   char *buffer,      /* buffer to fill with
						 data */
			   size_t length,     /* length of the
						 buffer  */
			   loff_t * offset)
{
  /*
   * Number of bytes actually written to the buffer
   */
  int bytes_read = 0;
  
  /*
   * If we're at the end of the message, return 0 signifying end of
   * file
   */
  if (*msg_Ptr == 0)
    return 0;
  
  /* 
   * Actually put the data into the buffer
   */
  while (length && *msg_Ptr) {
    
    /* 
     * The buffer is in the user data segment, not the kernel segment
     * so "*" assignment won't work.  We have to use put_user which
     * copies data from the kernel data segment to the user data
     * segment.
     */
    put_user(*(msg_Ptr++), buffer++); /* one char at a time... */
    
    length--;
    bytes_read++;
  }
  
  /* 
   * Most read functions return the number of bytes put into the
   * buffer
   */
  return bytes_read;
}

/*  
 * Called when a process writes to dev file: echo "hi" > /dev/hello
 * Next time we'll make this one do something interesting.
 */
static ssize_t
device_write(struct file *filp, const char *buff, size_t len, loff_t * off)
{

  /* not allowing writes for now, just printing a message in the
     kernel logs. */
  printk(KERN_ALERT "Sorry, this operation isn't supported.\n");
  return -EINVAL;		/* Fail */
}


/* These define info that can be displayed by modinfo */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Paul V. Gratz (and others)");
MODULE_DESCRIPTION("Module which creates a character device and allows user interaction with it");

/* Here we define which functions we want to use for initialization
   and cleanup */
module_init(my_init);
module_exit(my_cleanup);
