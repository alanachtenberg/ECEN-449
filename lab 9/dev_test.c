

#include <sys/types.h>
#include <sys/stat.h>
#include <linux/unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include "sound.h"
//#define O_RDWR 0

#define AC97_VOL_MUTE     0x8000
#define AC97_VOL_MIN      0x1f1f
#define AC97_VOL_MID      0x0a0a
#define AC97_VOL_MAX      0x0000


#define AC97_PCM_RATE_8000_HZ  0x1F40
#define AC97_PCM_RATE_11025_HZ 0x2B11
#define AC97_PCM_RATE_16000_HZ 0x3E80
#define AC97_PCM_RATE_22050_HZ 0x5622
#define AC97_PCM_RATE_44100_HZ 0xAC44
#define AC97_PCM_RATE_48000_HZ 0xBB80

#define DEVICE_FILE "/dev/ac97"
#define WAIT_VAL 0x1000000 // iterations to wait in delay
int main()
{
	printf("Beginning DEV TEST\n");
	int fd; // file descriptor
	unsigned int i=0; // loop variables
	// open device file for reading and writing
	// use "open" to open '/dev/multiplier'
        fd = open(DEVICE_FILE, O_RDWR); // open a file R/WR permissions
	// handle error opening file
	if(fd == -1) {
		printf("Failed to open device file!\n");
		return -1;
	}
	printf("Device Opened");
	printf("Default Playback speed and volume\n");
	delay();
	printf("Changing headphone volume to max\n");
	unsigned int val;
	val=AC97_VOL_MAX;
	ioctl(fd,ADJUST_AUX_VOL,&val);
	delay();
	printf("Changing playback speed to 16000 HZ\n");
	val=AC97_PCM_RATE_16000_HZ;
	ioctl(fd,ADJUST_PLAYBACK_RATE, &val);
	delay();

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
		
	
	
	
	
	
	
	
	
	
