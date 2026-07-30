module tick_gen #(
    parameter DIVISOR = 25_000_000
) (
    input clk,
    input rst,
    output reg tick
);
   reg [24:0] cnt;

   always @(posedge clk) begin
      if (rst) begin
         cnt  <= 0;
         tick <= 1'b0;
      end else if (cnt == DIVISOR - 1) begin
         cnt  <= 0;
         tick <= 1'b1;
      end else begin
         cnt  <= cnt + 1'b1;
         tick <= 1'b0;
      end
   end
endmodule
