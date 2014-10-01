`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    10:36:20 09/12/2014 
// Design Name: 
// Module Name:    switch 
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
module switch(
    input [3:0] SWITCHES,//Signals defined in UCF file
    output [3:0] LEDS
    );
	 
	 assign LEDS[3:0]=SWITCHES[3:0];//Continuously assigns LEDS to value of SWITCHES

endmodule
