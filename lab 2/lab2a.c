#include <xparameters.h>
#include <xgpio.h>

#define WAIT_VAL 0x1000000 // iterations to wait in delay

int delay(void);

int main(){

	int count;
	int count_masked;
	XGpio led; //Hardware io
	
	XGpio_Initialize(&led, XPAR_LEDS_DEVICE_ID);
	XGpio_SetDataDirection(&led,1,0); //setting direction of bits
	count=0;
	while(1) //infinite loop to keep circuit always executing
	{	
		count_masked = count & 0xf;//get lower 4 bits of count, so that count masked rolls over every 16 counts
		XGpio_DiscreteWrite(&led,1,count_masked);//write count masked to leds
		xil_printf("LEDs = 0x%x \n\r",count_masked); //Print count to console
		delay();
		count++;	
	}
	
	return (0);
}

int delay (void)
{
	volatile int delay_count=0; // volatile prevents compiler optimization
	while(delay_count<WAIT_VAL)
		delay_count++;// each iteration of while loop is 2 clock cycles
	return(0);
}
