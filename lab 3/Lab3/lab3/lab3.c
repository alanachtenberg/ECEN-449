#include <xparameters.h>
#include <multiply.h>

#define WAIT_VAL 0x1000000 // iterations to wait in delay

int delay(void);

int main(){
	while(1){// repeat indefinitely, issues connecting with kermit, make sure it will eventually display as long as connection is good
	int i=0;
	for (i;i<15;++i)
		delay(); //delay to not flood the screen
	xil_printf("Testing Multiply\n\r");
	i=0;// set i back to 0 to use in new for loop
	for(i;i<16;++i)//tests multiply of all sqaures less than 16
	{	
		unsigned int result=0;//initialize just in case
		MULTIPLY_mWriteSlaveReg0(XPAR_MULTIPLY_0_BASEADDR,0,i);//write i(0 to 15) to argument reg 0
		MULTIPLY_mWriteSlaveReg1(XPAR_MULTIPLY_0_BASEADDR,0,i);//write i to argument reg 1
		result= MULTIPLY_mReadSlaveReg2(XPAR_MULTIPLY_0_BASEADDR,0); //read ouput from reg 2
		xil_printf("%d times %d equals %d\n\r", i, i, result); //print result
	}
	i=0;
	for(i;i<16;++i) //just for funsies multiply (15 to 0) by (0 to 15)
	{	
		unsigned int result=0;
		MULTIPLY_mWriteSlaveReg0(XPAR_MULTIPLY_0_BASEADDR,0,15-i);// set input reg 0 to 15-i
		MULTIPLY_mWriteSlaveReg1(XPAR_MULTIPLY_0_BASEADDR,0,i);// set input reg 1 to i
		result= MULTIPLY_mReadSlaveReg2(XPAR_MULTIPLY_0_BASEADDR,0); //read ouput reg 2
		xil_printf("%d times %d equals %d\n\r", 15-i, i, result);// print result
	}
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
