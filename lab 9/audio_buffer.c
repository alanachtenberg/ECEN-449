/*  irq_test.c - Simple character device module
 *  
 *  Demonstrates interrupt driven character device.  Note: Assumption
 *  here is some hardware will strobe a given hard coded IRQ number
 *  (200 in this case).  This hardware is not implemented, hence reads
 *  will block forever, consider this a non-working example.  Could be
 *  tied to some device to make it work as expected.
 *
 *  (Adapted from various example modules including those found in the
 *  Linux Kernel Programming Guide, Linux Device Drivers book and
 *  FSM's device driver tutorial)
 */

/* Moved all prototypes and includes into the headerfile */
#include <asm/io.h> // Needed for IO reads and writes
#include <linux/ioctl.h>
#include <asm/ioctl.h>
#include "audio_buffer.h"
#include "xac97.h"
#include "audio_samples.h"
#include <linux/slab.h>//for malloc and free
#include "sound.h"
// From xac97.h
#define DEVICE_NAME "ac97"
#define IRQ_NUM	1 //interrupt code

#define BASE_ADDRESS 0x74400000
#define HIGH_ADDRESS 0x7440ffff
// size of physical address range for multiply
#define MEMSIZE HIGH_ADDRESS - BASE_ADDRESS + 1

void* virt_addr;


/* This structure defines the function pointers to our functions for
   opening, closing, reading and writing the device file.  There are
   lots of other pointers in this structure which we are not using,
   see the whole definition in linux/fs.h */
static int device_ioctl(struct inode *inode, struct file * file, unsigned int cmd, unsigned int *val_ptr);

static struct file_operations fops = {
  .read = device_read,
  .write = device_write,
  .ioctl = device_ioctl,
  .open = device_open,
  .release = device_release,
  
};

/*
 * This function is called when the module is loaded and registers a
 * device for the driver to use.
 */
int my_init(void)
{
	
	// Linux kernel's version of printf
	  printk(KERN_INFO "Mapping virtual address...\n");

	  // map virtual address to multiplier physical address
	  //io remap returns virtual address to physical address of memory
	  virt_addr=ioremap(BASE_ADDRESS,MEMSIZE); //first arg is physical address of mem, 2nd arg is size of mem to remap
  	  printk(KERN_INFO "PHYS ADDRESS %x\n", BASE_ADDRESS);
	  printk(KERN_INFO "VIRT ADDRESS %x\n", virt_addr);
  
  init_waitqueue_head(&queue);	/* initialize the wait queue */

  /* Initialize the semaphor we will use to protect against multiple
     users opening the device  */
  sema_init(&sem, 1);

  Major = register_chrdev(0, DEVICE_NAME, &fops);
  if (Major < 0) {		
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
	iounmap((void*)virt_addr);
}


/* 
 * Called when a process tries to open the device file, like "cat
 * /dev/irq_test".  Link to this function placed in file operations
 * structure for our device file.
 */
static int device_open(struct inode *inode, struct file *file)
{
  int irq_ret;

  if (down_interruptible (&sem))
	return -ERESTARTSYS;	

  /* We are only allowing one process to hold the device file open at
     a time. */
  if (Device_Open){
    up(&sem);
    return -EBUSY;
  }
  Device_Open++;
  
  /* OK we are now past the critical section, we can release the
     semaphore and all will be well */
  up(&sem);
  
    //dynamically allocate memory for buffer
  XAC97_InitAudio(virt_addr, 0);//init audio
	
  XAC97_WriteReg(virt_addr, AC97_PCM_ADC_Rate, AC97_PCM_RATE_11025_HZ); //sets pcm data rate
  XAC97_WriteReg(virt_addr, AC97_PCM_DAC_Rate, AC97_PCM_RATE_11025_HZ); //sets pcm data rate
  printk(KERN_ALERT "Initialized XAC97\r\n");
  /* request a fast IRQ and set handler */
  irq_ret = request_irq(IRQ_NUM, irq_handler, 0 /*flags*/ , DEVICE_NAME, NULL);
  if (irq_ret < 0) {		/* handle errors */
    printk(KERN_ALERT "Registering IRQ failed with %d\n", irq_ret);
    return irq_ret;
  }

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
  Device_Open--;		/* We're now ready for our next caller */
  
  free_irq(IRQ_NUM, NULL);
  XAC97_ClearFifos(virt_addr);//clear the pipes
  XAC97_SoftReset(virt_addr);//reset the driver
  
  /* F
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
  /* not allowing reads for now, just printing a message in the
     kernel logs. */
  printk(KERN_ALERT "Sorry, this operation isn't supported.\n");
  return -EINVAL;		/* Fail */
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


static int device_ioctl(struct inode *inode, struct file * file, unsigned int cmd, unsigned int *val_ptr){

	u16 val; //temporary value
	get_user (val, (u16*)val_ptr);//grab value from user space
	
	switch(cmd){
		case ADJUST_AUX_VOL:
		 	XAC97_WriteReg(virt_addr, AC97_AuxOutVol, val); //sets headphone volume
			break;
		case ADJUST_MAST_VOL:
			XAC97_WriteReg(virt_addr, AC97_MasterVol, val); //sets master volume
		case ADJUST_PLAYBACK_RATE:
			XAC97_WriteReg(virt_addr, AC97_PCM_ADC_Rate, val); //sets pcm data rate
			XAC97_WriteReg(virt_addr, AC97_PCM_DAC_Rate, val); //sets pcm data rate
			break;

		default:
			printk(KERN_INFO "Unsupported control command!\v\r");
			return -EINVAL;

	}
	return 0;
}

irqreturn_t irq_handler(int irq, void *dev_id) {
	static unsigned int index=0;
	while (!XAC97_isInFIFOFull(virt_addr))//while fifo is empty, fill fifo
	{
		//printk(KERN_ALERT "WRITING TO FIFO %d  %x \r\n", index,audio_samples[index]);
		XAC97_WriteFifo(virt_addr,(u32)audio_samples[index]);
		++index;
		if (index==NUM_SAMPLES)//wrap around the pointer
			index=0;

	}

  return IRQ_HANDLED; 
}




/* These define info that can be displayed by modinfo */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Paul V. Gratz (and others)");
MODULE_DESCRIPTION("Module which creates a character device and allows user interaction with it");

/* Here we define which functions we want to use for initialization
   and cleanup */
module_init(my_init);
module_exit(my_cleanup);
