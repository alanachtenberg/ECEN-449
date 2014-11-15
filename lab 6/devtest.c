#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define DEVICE_FILE "/dev/multiplier"

int main()
{
	unsigned int result;
	int fd; // file descriptor
	int i, j; // loop variables
	
	char input = 0;
	
	// open device file for reading and writing
	// use "open" to open '/dev/multiplier'
        fd = open('DEVICE_FILE', O_RDRW); // open a file with read and write priveleges
	
	// handle error opening file
	if(fd == -1) {
		printf("Failed to open device file!\n");
		return -1;
	}
	
	while(input != 'q') { // continue unless user enters 'q'
		for(i=0; i<=16; i++) {
			for(j=0; j<=16; j++) {
			
			// write values to registers using char dev
			// use write to write i and j to peripheral
			write(fd, &i, sizeof(i));
			write(fd, &j, sizeof(j));
			
			// read i, j, and result using char dev
			// use read to read from peripheral
			
			
			// print unsigned ints to screen
			printf("%u * %u = %u ",read_i,read_j, result);
			
			// validate result
			if(result == (i*j))
				printf(" Result Correct!");
			else
				printf(" Result Incorrect!");
				
			// read from terminal
			input = getchar();
			}
		}	
	}
	close(fd);
	return 0;
}
		
	
	
	
	
	
	
	
	
	
