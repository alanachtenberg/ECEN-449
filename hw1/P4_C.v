module ALU(C, A, B, OP);

output [31:0] C;
input [31:0] A, B;
input [1:0] OP;

assign C = (OP == 2'b00) ? A + B: (OP == 2'b01) ? A & B: (OP == 2'b10) ? A | B: ~A;

endmodule