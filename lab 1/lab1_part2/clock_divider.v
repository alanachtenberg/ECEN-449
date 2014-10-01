`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    11:17:41 09/12/2014 
// Design Name: 
// Module Name:    clock_divider 
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
module clock_divider(
    input CLK,input Rst,
    output reg OUTCLK
    );
	 reg [31:0]Counter; //register value to store count that divides CLK
	 always @( posedge CLK)//synchronous logic
	 begin
		if(Rst==1)
		begin
			Counter<=32'd0; //resets count back to 0 on reset to keep new clock in sync with old clock
		end
		else
		if (Counter==32'd100000000) //value that were dividing the clock by
			begin
			OUTCLK<=1'b1; //sets new clock high
			Counter<=32'd0; //rolls clock back over
			end
		else
		begin
			OUTCLK<=1'b0; // sets new clock low
			Counter<=Counter+1;
		end
	 end
		


endmodule
