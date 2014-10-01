`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    17:35:10 09/17/2014 
// Design Name: 
// Module Name:    jackpot 
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
module jackpot(
    input CLOCK,input RESET, input [3:0] SWITCHES,
    output [3:0] LEDS
    );
	 
	 reg [3:0]index;//4 bit reg corresponding to which leds are on
	 reg WINSTATE; //register to store state of win 0 for not won, 1 for won
	 wire switch0,switch1,switch2,switch3;//to get around always block sucking
	 wire newCLK;
	 clock_divider divder(CLOCK,RESET,newCLK);//divides clock to make led changes visible
	 
	 assign LEDS[3:0]=index[3:0];//sets continuously sets leds to index
 
	always@(posedge newCLK or posedge RESET)
	begin
		 if(RESET==1)
				begin
					index<=4'b1000;//set index to 1 on reset
				end
			else
				begin
					if (WINSTATE==1)
						begin
							index<=4'b1111;//if we won, then index=LEDS=1111, aka all on
						end
					else
						begin
							case(index)
							4'b1000:index<=4'b0100;//just shifting index of lit led
							4'b0100:index<=4'b0010;
							4'b0010:index<=4'b0001;
							4'b0001:index<=4'b1000;//wrap back to begining
							endcase
						end
						
				end
	end
	
	always@(SWITCHES or RESET)
	begin
		if(SWITCHES[3:0]==index[3:0]&&RESET!=1)	  
			WINSTATE<=1'b1;//SET state to WON
		else
			WINSTATE<=1'b0;//if SWITCHES does not match index or if RESET is 1 set winstate to 0
	end

endmodule 