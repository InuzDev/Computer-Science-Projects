module hello_world (
    input clk,
    output [3:0] D0_AN,
    output [7:0] D0_SEG,
    output [3:0] D1_AN,
    output [7:0] D1_SEG
);

   localparam SEG_H = 8'b1000_1001;
   localparam SEG_E = 8'b1000_0110;
   localparam SEG_L = 8'b1100_0111;
   localparam SEG_O = 8'b1100_0000;
   localparam SEG_BLANK = 8'b1111_1111;

   reg [15:0] refresh_counter = 0;
   always @(posedge clk) begin
      refresh_counter <= refresh_counter + 1'b1;
   end

   wire [1:0] digit_sel = refresh_counter[15:14];

   reg  [7:0] d0_seg_r;
   reg  [3:0] d0_an_r;

   always @(*) begin

      case (digit_sel)
         2'b00: begin
            d0_an_r  = 4'b1110;
            d0_seg_r = SEG_L;
         end  // AN0: L
         2'b01: begin
            d0_an_r  = 4'b1101;
            d0_seg_r = SEG_L;
         end  // AN1: L
         2'b10: begin
            d0_an_r  = 4'b1011;
            d0_seg_r = SEG_E;
         end  // AN2: E
         2'b11: begin
            d0_an_r  = 4'b0111;
            d0_seg_r = SEG_H;
         end  // AN3 (leftmost): H
      endcase
   end

   assign D0_AN  = d0_an_r;
   assign D0_SEG = d0_seg_r;

   assign D1_AN  = 4'b0111;
   assign D1_SEG = SEG_O;

endmodule
