`timescale 1ns / 1ps //time unit is 1 nano second and the simulation steps are in peco seconds
`define STRLEN 32

/*This test bench is full of non-synthesizable constructs, which basically means it is
 *restricted to simulation only!*/
module Converter_test;

   /*A task is similar to a procedure in the traditional programming language*/
   /*This particular task simply checks the output of our circuit against a 
     known answer and prints a message based on the outcome. Additionally, 
     this task increments the variable we are using to keep track of the 
     number of tests successfully passed.*/
   task passTest;
		input [31:0] actualOut, expectedOut;
		input [`STRLEN*8:0] testType;
		inout [7:0] passed;
	
		if(actualOut == expectedOut) begin $display ("%s passed", testType); passed = passed + 1; end
		else $display ("%s failed: %x should be %x", testType, actualOut, expectedOut);
	endtask
	
    /*this task simply informs the user of the final outcome of the test*/
	task allPassed;
		input [7:0] passed;
		input [7:0] numTests;
		
		if(passed == numTests) $display ("All tests passed");
		else $display("Some tests failed");
	endtask

	// Inputs
	reg RESET;
	reg [31:0] FIXED;
	
    reg [7:0] passed;

	// Outputs
	wire [19:0] FREAKY;

	// Instantiate the Unit Under Test (UUT)
	Converter uut (
		.RESET(RESET), 
		.FIXED(FIXED),
		.FREAKY(FREAKY)
	);

	initial begin
		// Initialize Inputs
		FIXED = 1;
		RESET = 1;
        passed = 0;
    
        // Add stimulus here	
        /*perform a walking one's test on inputs*/	
        /*wait 10 time units*/
        /*check output against known answer*/
		{RESET,FIXED} = {1'b1,32'h00000001}; #10; //RESET CIRCUIT
        {RESET,FIXED} = {1'b0,32'h00000001}; #10; passTest(FREAKY, 20'b01000000000000000000, "integer 1 to freaky float", passed);
        allPassed(passed,7'd1 );//did all the tests pass???
        $stop;//that's all folks!
	end
      
endmodule

