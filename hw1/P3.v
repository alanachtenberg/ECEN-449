
//MODULE CONVERTER, convertes 32 bit INTEGER into 20 bit FREAKY FLOAT

module Converter(input RESET, input wire [31:0]FIXED, output [19:0]FREAKY);
    reg [30:0]fixed;
    reg [19:0]FREAKY;
    reg [7:0] count;
    always@ (FIXED or RESET)
begin
    if(RESET==1)begin
    count=0;
    fixed=FIXED[30:0];//ignore sign bit
    end else begin
    fixed=FIXED[30:0];//ignore sign bit
    while ((fixed[30:0]>32'h00000001))begin//normalize by shifting number left
        fixed=fixed>>1;
        count=count+1; // keep track of amount you shifted
    end
    fixed=fixed<<1;//one last shift to ignore the always leading 1
    FREAKY[19]=FIXED[31];//SIGN bit
    FREAKY[18:12]=count+7'd64;//EXPONENT, add 64 for offset
    FREAKY[11:0]=fixed[30:18];//MANTISSA, truncates bits
   
    end

end


endmodule