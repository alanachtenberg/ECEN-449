
module ALU(C, A, B, OP);

output reg [31:0] C;
input [31:0] A, B;
input [1:0] OP;

always @ (A or B or OP) 
begin

	case(OP)
	2'b00: C = A + B; // opcode = 0, add A and B
	2'b01: C = A & B; // opcode = 1, bitwise AND
	2'b10: C = A | B; // opcode = 2, bitwise OR
	2'b11: C = ~A;    // opcode = 3, bitwise complement
	endcase
	end

endmodule
	
	
