
//Alan Achtenberg
//ECEN 449
//HW3
//PR2

//PART A
/*Module to convert pwm to ppm signal. To implement this module i made
  the assumption that the pwm signal always starts at the beginning of
  our slow clock period and that the index of ppm corresponds to the
  width of the pwm signal. ie a 0 width pwm would have a ppm 
  signal(1 fast clock wide) at index 0. 
  
  Note in the unlikely case that the pwm signal is high for the entire 
  slow clock cycle the width is greater than our maximum index of timeslots-1 
  therefore we will interperet a signal with a width of timeslots to simply be 
  a ppm signal with no index(a flat signal) in other words ppm will never be
*/


module pwm_to_ppm(ClkTimeFrame, ClkTimeSlot, PWM,PPM);
	input ClkTimeFrame, ClkTimeSlot;//Slow and Fast clk
	input PWM;//our pulse width signal
	output reg PPM;// our pulse position signal

	
	reg ppm_set;//flag to determine if ppm has been set low or high by pwm 
		   //NOTE does not represent if ppm was set low or high by other signals 
	always@(posedge ClkTimeSlot)begin

		if (PWM==1)
			begin
			PPM<=0;
			ppm_set<=0;//
			end
		if (PPM==1)
			PPM<=0;//only allow ppm to be set high for one cycle, no ppm_set=0 because we want it to wait for posedge of pwm
		else if (PWM==0&& ppm_set==0)//because we know pwm was high at the beginning
			begin		     //we can set ppm high as soon as it goes low
			PPM<=1;		     //this will generate the ppm at index=width
			ppm_set<=1;//use this flag to prevent setting ppm high multiple times
			end	   //before pwm goes high again. 				
	end		
endmodule

//PART B

/* The ppm signal does not always start at the begining of the time frame
   I cannot simply produce a simliar solution from part a, in part a I used the extra information that
   whenever the pwm signal is 1 the ppm signal is always 0 to implement conversion with no knowledge
   of the timeframe clk. Unfortunately the reverse does not hold true, whenever the ppm signal is 0 
   the pwm signal can be both 1 or 0. You might say that when the ppm signal is 1 the pwm signal is always 0
   but this not helpful, because the ppm signal is always only one fast clk period wide and does not start
   at our slow clock period.

   Thus I will determine the number of timeslots in T to correctly set the pwm signal high at the
   beginning of every Time period and then set the pwm signal low if ppm is high

*/
module ppm_to_pwm(ClkTimeFrame, ClkTime, PPM, PWM);
	input ClkTimeFrame, ClkTimeSlot;//Slow and Fast clk
	input PPM;// our pulse position signal
	output reg PWM;//our pulse width signal
	
	reg [31:0] count;//count
	reg count_reset;// flag to allow reset of count
	reg reset_pwm;//flag to reset pwm
	always@(posedge ClkTimeSlot)begin		
		if (ClkTimeFrame==0 && count_reset==0)
			count_reset<=1;//we have hit the period of the clock where ClkTimeFrame is always low, 
				       //can now allow it to check if time frame returns to high and then reset count
		if (ClkTimeFrame==1 && count_reset==1)//count will only reset at the beginning of T, 
			begin
			count<=0;
			reset_pwm<=1;//setting this flag will allow us to set pwm in next block			
			count_reset<=0;//setting this flag to 0 will prevent the count from being reset multiple times per frame
			end
		else
			count<=count+1;
			reset_pwm<=0;//reset_pwm will only ever be high for one clock at the beginning of T
	end
	
	reg [31:0] index;
	always@(posedge ClkTimeSlot)begin		
		if (reset_pwm=1 && PPM!=1)
			 PWM<=1;//set PWM high at the beginning of every T
		if (PPM==1)
			PWM<=0;	
			
	end
	
endmodule


//PART C

/*
 PCM cannot be converted in real time. because PCM only needs to send log2(p)bits per time T
while PPM has to send p bits per time T to represent the same amount of data.

*/
