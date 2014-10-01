`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    12:03:16 09/12/2014 
// Design Name: 
// Module Name:    Led_counter 
// Project Name: 
// Target Devices: 
// Tool versions: 
// Description: 
//
// Dependencies: 
//
// Revision: 
// Revision 0.01 - File Created
// Additional Comments: 
//
//////////////////////////////////////////////////////////////////////////////////
module Led_counter(
    input CLOCK,
	 input RESET,
    input UP,
    input DOWN,
    output [3:0] LEDS
    );
	 
	 wire DividedCLK; // intermediate for clock divider module
	 reg [3:0] Count; // keep track of current count with 4 bit reg
	 
	 clock_divider divider (CLOCK,RESET,DividedCLK); //divide old clock to new 1 Hz clock
	 
	 assign LEDS[3:0]=Count[3:0];
	 
	 always @(posedge DividedCLK or posedge RESET)
	 begin
			if(RESET==1)
				Count<=0; //rest count value on reset
			else
			begin
				Count<=Count;//case where neither UP or Down
				if(UP==1)
					Count<=Count+1; //if up increment
				else if(DOWN==1)
					Count<=Count-1; // if down decrement
			end
	 end


endmodule
