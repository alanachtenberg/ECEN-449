

#include <sys/types.h>
#include <sys/stat.h>
#include <linux/unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

//#define O_RDWR 0
#define DEVICE_FILE "/dev/irq_test"
#define WAIT_VAL 0x0100000 // iterations to wait in delay
int main()
{
	printf("Beginning DEV TEST\n");
	unsigned int result;
	int fd; // file descriptor
	unsigned int i=0; // loop variables
	
	int input=0;
	// open device file for reading and writing
	// use "open" to open '/dev/multiplier'
        fd = open(DEVICE_FILE, O_RDWR); // open a file R/WR permissions
	
	// handle error opening file
	if(fd == -1) {
		printf("Failed to open device file!\n");
		return -1;
	}
	char* buffer=(char*)malloc(2*10);//each message is only 2 bytes
	int message;
	char* msg_ptr=(char*)&message;//char buffer for copy of int
	for (i=0;i<10;++i){
			read(fd, (char *)buffer, 2*sizeof(char)*10);//blocking
			delay();//give me time to let go of the button so that interupt
			int j=0;//does not make console look ugly
			for (j=0;j<10;++j){
				msg_ptr[0]=0;
				msg_ptr[1]=0;
				msg_ptr[2]=buffer[2*j];//offset int to make sense
				msg_ptr[3]=buffer[2*j+1];
				printf("NEW Message Recieved %x\n",message &0x0000FFFF);//only care about lower two bytes
			}
				delay();//give me time to let go of the button so that interupt

		}
	free(buffer);
	close(fd);
	return 0;
}



int delay (void)
{
	volatile int delay_count=0; // volatile prevents compiler optimization
	while(delay_count<WAIT_VAL)
		delay_count++;// each iteration of while loop is 2 clock cycles
	return(0);
}
		
	
	
	
	
	
	
	
	
	
