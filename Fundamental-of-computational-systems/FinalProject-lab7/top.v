module top (
    input         clk,
    input  [15:0] sw,
    input  [ 3:0] btn,
    output [ 3:0] led,
    output [ 2:0] RGB0,
    output [ 2:0] RGB1,
    output [ 3:0] D1_AN,
    output [ 7:0] D1_SEG
);

   wire rst = sw[15];

   wire start_pulse;
   debounce_edge start_edge_inst (
       .clk  (clk),
       .raw  (sw[0]),
       .pulse(start_pulse)
   );

   wire [3:0] btn_edge;
   debounce_edge btn_edge0 (
       .clk  (clk),
       .raw  (btn[0]),
       .pulse(btn_edge[0])
   );
   debounce_edge btn_edge1 (
       .clk  (clk),
       .raw  (btn[1]),
       .pulse(btn_edge[1])
   );
   debounce_edge btn_edge2 (
       .clk  (clk),
       .raw  (btn[2]),
       .pulse(btn_edge[2])
   );
   debounce_edge btn_edge3 (
       .clk  (clk),
       .raw  (btn[3]),
       .pulse(btn_edge[3])
   );

   wire [3:0] level_bcd;

   simon_game game_inst (
       .clk        (clk),
       .rst        (rst),
       .start_pulse(start_pulse),
       .btn_edge   (btn_edge),
       .leds       (led),
       .rgb0       (RGB0),
       .rgb1       (RGB1),
       .level_bcd  (level_bcd)
   );

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
            4'hA: hex_to_seg = 8'b1000_1000;
            4'hB: hex_to_seg = 8'b1000_0011;
            4'hC: hex_to_seg = 8'b1100_0110;
            4'hD: hex_to_seg = 8'b1010_0001;
            4'hE: hex_to_seg = 8'b1000_0110;
            4'hF: hex_to_seg = 8'b1000_1110;
            default: hex_to_seg = 8'b1111_1111;
         endcase
      end
   endfunction

   assign D1_AN  = 4'b1110;
   assign D1_SEG = hex_to_seg(level_bcd);

endmodule
