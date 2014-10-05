module ALU(C, A, B, OP);

output reg [31:0] C;
input [31:0] A, B;
input [1:0] OP;

always @ (A or B or OP) 
begin
	if (OP == 2'b00) begin
		C = A + B;
	end
	else if (OP == 2'b01) begin
		C = A & B;
	end
	else if (OP == 2'b10) begin
		C = A | B;
	end
	else begin
		C = ~A;
	end
end

endmodule
