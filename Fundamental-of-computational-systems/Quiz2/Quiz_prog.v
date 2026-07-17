module multiplicador4bits (
    input  [3:0] A,
    input  [3:0] B,
    output [7:0] product,
    output       zero,
    output       even,
    output       overflowFlag
);

   assign product = A * B;
   assign zero = (product == 8'd0);
   assign even = ~product[0];
   assign overflowFlag = (product > 8'd15);

endmodule
