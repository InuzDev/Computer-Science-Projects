module lfsr4 (
    input clk,
    input rst,
    output reg [3:0] value
);
   always @(posedge clk) begin
      if (rst) value <= 4'b1011;  // nonzero seed -- an all-zero LFSR would lock up
      else value <= {value[2:0], value[3] ^ value[2]};
   end
endmodule
