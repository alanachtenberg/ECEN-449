#include <xparameters.h>
#include <xgpio.h>

#define WAIT_VAL 0x1000000

int delay(void);

int main(){

	int count;
	int count_masked;
	XGpio led;

	XGpio_Initialize(&led, XPAR_LEDS_DEVICE_ID);
	XGpio_SetDataDirection(&led,1,0);
	count=0;
	while(1)
	{	
		count_masked = count & 0xf;
		XGpio_DiscreteWrite(&led,1,count_masked);)
		xil_printf("LEDs = 0x%x \n\r",count_masked);
		delay();
		count++;	
	}
	
	return (0);
}

int delay (void)
{
	volatile int delay_count=0;
	while(delay_count<WAIT_VAL)
		delay_count++;
	return(0);
}
