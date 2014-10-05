`timescale 1ns / 1ps //time unit is 1 nano second and the simulation steps are in peco seconds
`define STRLEN 32

/*This test bench is full of non-synthesizable constructs, which basically means it is
 *restricted to simulation only!*/
module ALU_test;

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
	reg [31:0] A;
	reg [31:0] B;
	reg [1:0] OP;
    reg [7:0] passed;

	// Outputs
	wire [31:0] C;

	// Instantiate the Unit Under Test (UUT)
	ALU uut (
		.C(C), 
		.A(A), 
		.B(B), 
		.OP(OP)
	);

	initial begin
		// Initialize Inputs
		A = 0;
		B = 0;
		OP = 0;
        passed = 0;
    
        // Add stimulus here	
        /*perform a walking one's test on inputs*/	
        /*wait 10 time units*/
        /*check output against known answer*/
        {A, B, OP} = {32'b0111, 32'b1001, 2'b00}; #10; passTest(C, 32'b10000, "A+B", passed);
        {A, B, OP} = {32'b0111, 32'b1001, 2'b01}; #10; passTest(C, 32'b0001, "A&B", passed);
        {A, B, OP} = {32'b0111, 32'b1001, 2'b10}; #10; passTest(C, 32'b1111, "A|B", passed);
        {A, B, OP} = {32'b0111, 32'b1001, 2'b11}; #10; passTest(C, 32'hFFFFFFF8, "~A", passed);
        allPassed(passed, 7'd4);//did all the tests pass???
        $stop;//that's all folks!
	end
      
endmodule

