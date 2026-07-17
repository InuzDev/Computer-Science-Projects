// Calculadora para laboratorio 5
module Calculator (
    input clk,
    input [15:0] sw,
    input [3:0] btn,
    output [3:0] D0_AN,
    output [7:0] D0_SEG,
    output [3:0] D1_AN,
    output [7:0] D1_SEG
);

   wire [ 7:0] A = sw[7:0];
   wire [ 7:0] B = sw[15:8];

   reg  [15:0] result;
   always @(*) begin
      if (btn[0]) result = A + B;  // Add
      else if (btn[1]) result = A - B;  // Subtract (unsigned)
      else if (btn[2]) result = A * B;  // Multiply
      else result = 16'h0000;
   end

   // 7-segment decoder
   function [7:0] hex_to_seg;
      input [3:0] val;
      begin
         case (val)
            4'h0: hex_to_seg = 8'b1100_0000;
            4'h1: hex_to_seg = 8'b1111_1001;
            4'h2: hex_to_seg = 8'b1010_0100;
            4'h3: hex_to_seg = 8'b1011_0000;
            4'h4: hex_to_seg = 8'b1001_1001;
            4'h5: hex_to_seg = 8'b1001_0010;
            4'h6: hex_to_seg = 8'b1000_0010;
            4'h7: hex_to_seg = 8'b1111_1000;
            4'h8: hex_to_seg = 8'b1000_0000;
            4'h9: hex_to_seg = 8'b1001_0000;
            4'ha: hex_to_seg = 8'b1000_1000;
            4'hb: hex_to_seg = 8'b1000_0011;
            4'hc: hex_to_seg = 8'b1100_0110;
            4'hd: hex_to_seg = 8'b1010_0001;
            4'he: hex_to_seg = 8'b1000_0110;
            4'hf: hex_to_seg = 8'b1000_1110;
            default: hex_to_seg = 8'b1111_1111;
         endcase
      end
   endfunction

   // Refresh counter for multiplexing
   reg [15:0] refresh_counter = 0;
   always @(posedge clk) begin
      refresh_counter <= refresh_counter + 1;
   end
   wire [1:0] digit_sel = refresh_counter[15:14];

   reg  [3:0] d0_an_r;
   reg  [7:0] d0_seg_r;
   always @(*) begin
      case (digit_sel)
         2'b00: begin
            d0_an_r  = 4'b1110;
            d0_seg_r = hex_to_seg(B[3:0]);
         end
         2'b01: begin
            d0_an_r  = 4'b1101;
            d0_seg_r = hex_to_seg(B[7:4]);
         end
         2'b10: begin
            d0_an_r  = 4'b1011;
            d0_seg_r = hex_to_seg(A[3:0]);
         end
         2'b11: begin
            d0_an_r  = 4'b0111;
            d0_seg_r = hex_to_seg(A[7:4]);
         end
      endcase
   end

   reg [3:0] d1_an_r;
   reg [7:0] d1_seg_r;
   always @(*) begin
      case (digit_sel)
         2'b00: begin
            d1_an_r  = 4'b1110;
            d1_seg_r = hex_to_seg(result[3:0]);
         end
         2'b01: begin
            d1_an_r  = 4'b1101;
            d1_seg_r = hex_to_seg(result[7:4]);
         end
         2'b10: begin
            d1_an_r  = 4'b1011;
            d1_seg_r = hex_to_seg(result[11:8]);
         end
         2'b11: begin
            d1_an_r  = 4'b0111;
            d1_seg_r = hex_to_seg(result[15:12]);
         end
      endcase
   end

   assign D0_AN  = d0_an_r;
   assign D0_SEG = d0_seg_r;
   assign D1_AN  = d1_an_r;
   assign D1_SEG = d1_seg_r;

endmodule
