/* All of our linux kernel includes. */
#include <linux/module.h>  /* Needed by all modules */
#include <linux/moduleparam.h>  /* Needed for module parameters */
#include <linux/kernel.h>  /* Needed for printk and KERN_* */
#include <linux/init.h>	   /* Need for __init macros  */
#include <linux/fs.h>	   /* Provides file ops structure */
#include <linux/sched.h>   /* Provides access to the "current" process
			      task structure */
#include <asm/uaccess.h>   /* Provides utilities to bring user space
			      data into kernel space.  Note, it is
			      processor arch specific. */
#include <linux/semaphore.h>	/* Provides semaphore support */
#include <linux/wait.h>		/* For wait_event and wake_up */
#include <linux/interrupt.h>	/* Provide irq support functions (2.6
				   only) */
					

/* Some defines */
#define DEVICE_NAME "ac97"
#define BUF_LEN 10 
#define MSG_SIZE 2 //messages are 2 bytes
#define IRQ_NUM 3 //number of our interupt

/* Function prototypes, so we can setup the function pointers for dev
   file access correctly. */
int init_module(void);
void cleanup_module(void);
static int device_open(struct inode *, struct file *);
static int device_release(struct inode *, struct file *);
static ssize_t device_read(struct file *, char *, size_t, loff_t *);
static ssize_t device_write(struct file *, const char *, size_t, loff_t *);
static irqreturn_t irq_handler(int irq, void *dev_id);

/* 
 * Global variables are declared as static, so are global but only
 * accessible within the file.
 */
static int Major;		/* Major number assigned to our device driver */
static char Device_Open = 0;	/* Flag to signify open device */
static char *msg_buffer; //buffer for messages
static char *msg_ptr; //pointer to current message
static struct semaphore sem;  /* mutual exclusion semaphore for race
				 on file open  */
static wait_queue_head_t queue; /* wait queue used by driver for
				   blocking I/O */

