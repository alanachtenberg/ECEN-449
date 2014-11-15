

#include <xparameters.h> //addresses of hardware
#include <ir_demod.h> //functions to read from demodulator

#define WAIT_VAL 0x0110000 // iterations to wait in delay

int main(){
	Xuint32 prev, current, count;//for storing decoded messages
	prev=IR_DEMOD_mReadSlaveReg0(XPAR_IR_DEMOD_0_BASEADDR,0);//get initial value
	xil_printf("FPGA Connected"); //Print to console
	while (1){
		count=IR_DEMOD_mReadSlaveReg1(XPAR_IR_DEMOD_0_BASEADDR,0);//read new value
		current=IR_DEMOD_mReadSlaveReg0(XPAR_IR_DEMOD_0_BASEADDR,0);//read new value
		if (current!=prev){//check if change has occured
			xil_printf("New IR code recieved %x\r\n",current); //Print to console
			xil_printf("Message count %x \r\n", count); //Print to console
		}
		prev=current;// set old value to new value
		}

	return 0;
}


int delay (void)
{
	volatile int delay_count=0; // volatile prevents compiler optimization
	while(delay_count<WAIT_VAL)
		delay_count++;// each iteration of while loop is 2 clock cycles
	return(0);
}
