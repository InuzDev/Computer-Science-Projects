module debounce_edge (
    input clk,
    input raw,
    output reg pulse
);
   reg sync1, sync2, prev;

   always @(posedge clk) begin
      sync1 <= raw;
      sync2 <= sync1;
      prev  <= sync2;
      pulse <= sync2 & ~prev;
   end
endmodule
