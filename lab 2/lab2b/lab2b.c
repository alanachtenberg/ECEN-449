#include <xparameters.h>
#include <xgpio.h>

#define WAIT_VAL 0x1000000 // iterations to wait in delay
#define SWITCH1 0x50
#define SWITCH2 0x40
#define SWITCH3 0x30 
#define SWITCH4 0x20 
#define NORTH   0x10
#define SOUTH   0x8
#define EAST	 0x4
#define WEST	 0x2
#define CENTER  0x1

#define true 1
#define false 0
#define bool _Bool // bool not defined in old c std, _Bool is

int delay(void);

int main(){

	int count=0; //init to 0
	int count_masked=0;

	//true for right/east button/ dip status mode
	bool dip_mode=false;//false for left/west button/counter mode

	int prev=0;//determine when to show update of switches
	
	//Set up Hardware
	XGpio led; //Hardware io
	
	XGpio inputs;//switches 0:3 and buttons 4:8

	XGpio_Initialize(&led, XPAR_LEDS_DEVICE_ID);
	XGpio_SetDataDirection(&led,1,0); //setting led direction to output

	XGpio_Initialize(&inputs, XPAR_INPUTS_DEVICE_ID);

	XGpio_SetDataDirection(&inputs,1,1); //setting direction ro input
	
	while(1) //infinite loop to keep circuit always executing
	{
		//int debug_inf=XGpio_DiscreteRead(&inputs,1);
	 	//	xil_printf("Looping 0x%x\n\r",debug_inf); //Print count to console

		if ((CENTER & XGpio_DiscreteRead(&inputs,1))==CENTER){
					count=0;//RESET count
			xil_printf("RESET: 0x%x\n\r", count); //Print count to console

		}
		if ((WEST & XGpio_DiscreteRead(&inputs,1))==WEST){
			dip_mode=false;//set mode to count
			xil_printf("mode switch: Counter 0x%x\n\r", count_masked); //Print count to console

		}
		else if ((EAST & XGpio_DiscreteRead(&inputs,1))==EAST){
			dip_mode=true;//set mode to dip status
			xil_printf("mode switch: DIP 0x%x\n\r", count_masked); //Print count to console	
	}
			
		if (!dip_mode){
			if ((NORTH & XGpio_DiscreteRead(&inputs,1))==NORTH){ //& operation masks non-NORTH bits
				count++;			
				count_masked = count & 0xf;//get lower 4 bits of count, so that count masked rolls over every 16 counts
				XGpio_DiscreteWrite(&led,1,count_masked);//write count masked to leds
				xil_printf("incremented 0x%x\n\r",count_masked); //Print count to console
			}
			else if((SOUTH & XGpio_DiscreteRead(&inputs,1))==SOUTH){
				count--;					
				count_masked = count & 0xf;//get lower 4 bits of count, so that count masked rolls over every 16 counts
				XGpio_DiscreteWrite(&led,1,count_masked);//write count masked to leds
				xil_printf("decremented 0x%x\n\r",count_masked); //Print count to console
			}
		}
		else{
			count_masked = 0x1f0 & XGpio_DiscreteRead(&inputs,1); //takes upper bits of switches
			count_masked = count_masked>>5; 
			
			if (prev!=count_masked){
				XGpio_DiscreteWrite(&led,1,count_masked);//writes value of switches
				xil_printf("Switches changed 0x%x \n\r",count_masked); //Print count to console
			}
			prev=count_masked;
		}
		delay();
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
